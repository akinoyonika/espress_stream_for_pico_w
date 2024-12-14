#include "espress_stream/config.h"
#include "espress_stream/depend.h"
#include "espress_stream/uint.h"
#include "espress_stream/mutex.h"
#include "espress_stream/cond.h"
#include "espress_stream/event.h"
#include "espress_stream/hash.h"
#include "espress_stream/mac.h"
#include "espress_stream/block_cipher.h"
#include "espress_stream/ctr_cipher.h"
#include "espress_stream/hash.h"
#include "espress_stream/mac.h"
#include "espress_stream/hkdf.h"
#include "espress_stream/key_exp.h"
#include "espress_stream/key.h"
#include "espress_stream/ring.h"
#include "espress_stream/dgram.h"
#include "espress_stream/trans_ring.h"
#include "espress_stream/packet.h"

#define _IS_ENCRYPTION_ENABLE (1)

#if ESPRESS_STREAM_IS_HOST_BYTE_ORDER_LITTLE
#	define _O(w, n) (n)
#else
#	define _O(w, n) ((1 << (w)) - 1 - (n))
#endif

/*
 * - connect
 * 
 *    0000H +-----------------------------------------+
 *          |4343H/6363H (uint16)|packet_size (uint16)|
 *    0004H +-----------------------------------------+
 *          |connection_id (uint32)                   |
 *    0008H +-----------------------------------------+
 *          |connection_param (uint32)                |
 *    000CH +-----------------------------------------+
 *          |initial_window_size (uint32)             |
 *    0010H +-----------------------------------------+
 *          |key_change (uint8[32])                   |
 *          :                                         :
 *          |                                         |
 *    0030H +-----------------------------------------+
 *          |random (uint8[32])                       |
 *          :                                         :
 *          |                                         |
 *    0050H +-----------------------------------------+
 *          |payload (uint8[n], n=packet_size-0050H)  |
 *          :                                         :
 *          |            |padding (uint8[(n+3)/4*4-n])|
 *          +-----------------------------------------+
 * 
 * - disconnect
 * 
 *    0000H +-----------------------------------------+
 *          |4444H/6464H (uint16)|packet_size (uint16)|
 *    0004H +-----------------------------------------+
 *          |connection_id (uint32)                   |
 *    0008H +-----------------------------------------+
 *          |counter_number (uint32)                  |
 *    000CH +-----------------------------------------+
 *          |payload (uint8[n], n=packet_size-000CH)  |
 *          :                                         :
 *          |            |padding (uint8[(n+3)/4*4-n])|
 *          +-----------------------------------------+
 * 
 * - envelope
 * 
 *    0000H +-----------------------------------------+  --+
 *          |4545H/6565H (uint16)|packet_size (uint16)|    |
 *    0004H +-----------------------------------------+    |
 *          |connection_id (uint32)                   |    |
 *    0008H +-----------------------------------------+    |
 *          |counter_number (uint32)                  |    a
 *    000CH +-----------------------------------------+    c
 *          |receive_counter_number (uint32)          |    k
 *    0010H +-----------------------------------------+    |
 *          |receive_window_address (uint32)          |    |
 *    0014H +-----------------------------------------+    |
 *          |receive_window_size (uint32)             |    |
 *    0018H +-----------------------------------------+  --+
 *          |send_data_address (uint32)               |
 *    001CH +-----------------------------------------+
 *          |payload (uint8[n], n=packet_size-001CH)  |
 *          :                                         :
 *          |            |padding (uint8[(n+3)/4*4-n])|
 *          +-----------------------------------------+
 */

#define _PACKET_TYPE_OFFSET (0x0000)
#define _PACKET_SIZE_OFFSET (0x0002)
#define _CONNECTION_ID_OFFSET (0x0004)
#define _CONNECTION_PARAM_OFFSET (0x0008)
#define _INITIAL_WINDOW_SIZE_OFFSET (0x000c)
#define _KEY_CHANGE_OFFSET (0x0010)
#define _RANDOM_OFFSET (0x0030)
#define _COUNTER_NUMBER_OFFSET (0x0008)
#define _RECEIVE_COUNTER_NUMBER_OFFSET (0x000c)
#define _RECEIVE_WINDOW_ADDRESS_OFFSET (0x0010)
#define _RECEIVE_WINDOW_SIZE_OFFSET (0x0014)
#define _SEND_DATA_ADDRESS_OFFSET (0x0018)
#define _CONNECT_PAYLOAD_OFFSET (0x0050)
#define _DISCONNECT_PAYLOAD_OFFSET (0x000c)
#define _ENVELOPE_PAYLOAD_OFFSET (0x001c)

#define _connect_uint16_start(this) (_packet_type(this))
#define _connect_uint16_count(this) ((void)(this), 2)
#define _connect_uint32_start(this) (_connection_id(this))
#define _connect_uint32_count(this) ((void)(this), 3)
#define _connect_uint8_start(this) (_key_change(this))
#define _connect_uint8_count(this) \
	((this)->data[_PACKET_SIZE_OFFSET + _O(1, 0)] + \
		((this)->data[_PACKET_SIZE_OFFSET + _O(1, 1)] << 8) - \
		_KEY_CHANGE_OFFSET)
#define _connect_cipher_start(this) ((void)(this), NULL)
#define _connect_cipher_count(this) ((void)(this), 0)
#define _connect_min_size(this) ((void)(this), ESPRESS_STREAM_PACKET_CONNECT_MIN_SIZE)

#define _disconnect_uint16_start(this) (_packet_type(this))
#define _disconnect_uint16_count(this) ((void)(this), 2)
#define _disconnect_uint32_start(this) (_connection_id(this))
#define _disconnect_uint32_count(this) ((void)(this), 2)
#define _disconnect_uint8_start(this) (_disconnect_payload(this))
#define _disconnect_uint8_count(this) \
	((this)->data[_PACKET_SIZE_OFFSET + _O(1, 0)] + \
		((this)->data[_PACKET_SIZE_OFFSET + _O(1, 1)] << 8) - \
		_DISCONNECT_PAYLOAD_OFFSET)
#define _disconnect_cipher_start(this) (_disconnect_uint8_start(this))
#define _disconnect_cipher_count(this) (_disconnect_uint8_count(this))
#define _disconnect_min_size(this) ((void)(this), ESPRESS_STREAM_PACKET_DISCONNECT_MIN_SIZE)

#define _envelope_uint16_start(this) (_packet_type(this))
#define _envelope_uint16_count(this) ((void)(this), 2)
#define _envelope_uint32_start(this) (_connection_id(this))
#define _envelope_uint32_count(this) ((void)(this), 6)
#define _envelope_uint8_start(this) (_envelope_payload(this))
#define _envelope_uint8_count(this) \
	((this)->data[_PACKET_SIZE_OFFSET + _O(1, 0)] + \
		((this)->data[_PACKET_SIZE_OFFSET + _O(1, 1)] << 8) - \
		_ENVELOPE_PAYLOAD_OFFSET)
#define _envelope_cipher_start(this) ((b_t *)_receive_window_address(this))
#define _envelope_cipher_count(this) \
	((this)->data[_PACKET_SIZE_OFFSET + _O(1, 0)] + \
		((this)->data[_PACKET_SIZE_OFFSET + _O(1, 1)] << 8) - \
		_RECEIVE_WINDOW_ADDRESS_OFFSET)
#define _envelope_ack_size(this) ((void)(this), ESPRESS_STREAM_PACKET_ENVELOPE_ACK_SIZE)
#define _envelope_min_size(this) ((void)(this), ESPRESS_STREAM_PACKET_ENVELOPE_MIN_SIZE)

enum
{
	_NONE,
	_ENCODE,
	_DECODE
};

struct _val
{
	struct espress_stream_uint16 *uint16_start;
	struct espress_stream_uint32 *uint32_start;
	b_t *uint8_start;
	b_t *cipher_start;
	u_t uint16_count;
	u_t uint32_count;
};

struct _var
{
	u_t packet_size;
	u_t uint8_count;
	u_t cipher_count;
};

static i_t _initialize(struct espress_stream_packet *this, struct espress_stream_event *event);
static void _finalize(struct espress_stream_packet *this);
static struct espress_stream_uint16 *_packet_type(struct espress_stream_packet *this);
static struct espress_stream_uint16 *_packet_size(struct espress_stream_packet *this);
static struct espress_stream_uint32 *_connection_id(struct espress_stream_packet *this);
static struct espress_stream_uint32 *_connection_param(struct espress_stream_packet *this);
static struct espress_stream_uint32 *_initial_window_size(struct espress_stream_packet *this);
static b_t *_key_change(struct espress_stream_packet *this);
static u_t _key_change_size(struct espress_stream_packet *this);
static b_t *_random(struct espress_stream_packet *this);
static u_t _random_size(struct espress_stream_packet *this);
static struct espress_stream_uint32 *_counter_number(struct espress_stream_packet *this);
static struct espress_stream_uint32 *_receive_counter_number(struct espress_stream_packet *this);
static struct espress_stream_uint32 *_receive_window_address(struct espress_stream_packet *this);
static struct espress_stream_uint32 *_receive_window_size(struct espress_stream_packet *this);
static struct espress_stream_uint32 *_send_data_address(struct espress_stream_packet *this);
static b_t *_connect_payload(struct espress_stream_packet *this);
static u_t _connect_payload_size(struct espress_stream_packet *this);
static b_t *_disconnect_payload(struct espress_stream_packet *this);
static u_t _disconnect_payload_size(struct espress_stream_packet *this);
static b_t *_envelope_payload(struct espress_stream_packet *this);
static u_t _envelope_payload_size(struct espress_stream_packet *this);
static void _encode(struct espress_stream_packet *this, i_t is_server, struct espress_stream_key *key);
static void _decode(struct espress_stream_packet *this, i_t is_server, struct espress_stream_key *key);
static void _abort_encode(struct espress_stream_packet *this);
static void _abort_decode(struct espress_stream_packet *this);
static void _yield(struct espress_stream_packet *this, struct espress_stream_uint32 const *timeout);
static void _yield_break(struct espress_stream_packet *this);

struct espress_stream_packet_ops const espress_stream_packet[] =
{{
	_initialize,
	_finalize,
	_packet_type,
	_packet_size,
	_connection_id,
	_connection_param,
	_initial_window_size,
	_key_change,
	_key_change_size,
	_random,
	_random_size,
	_counter_number,
	_receive_counter_number,
	_receive_window_address,
	_receive_window_size,
	_send_data_address,
	_connect_payload,
	_connect_payload_size,
	_disconnect_payload,
	_disconnect_payload_size,
	_envelope_payload,
	_envelope_payload_size,
	_encode,
	_decode,
	_abort_encode,
	_abort_decode,
	_yield,
	_yield_break
}};

struct espress_stream_uint16 const espress_stream_packet_type_client_connect[] = {ESPRESS_STREAM_PACKET_TYPE_CLIENT_CONNECT};
struct espress_stream_uint16 const espress_stream_packet_type_client_disconnect[] = {ESPRESS_STREAM_PACKET_TYPE_CLIENT_DISCONNECT};
struct espress_stream_uint16 const espress_stream_packet_type_client_envelope[] = {ESPRESS_STREAM_PACKET_TYPE_CLIENT_ENVELOPE};
struct espress_stream_uint16 const espress_stream_packet_type_server_connect[] = {ESPRESS_STREAM_PACKET_TYPE_SERVER_CONNECT};
struct espress_stream_uint16 const espress_stream_packet_type_server_disconnect[] = {ESPRESS_STREAM_PACKET_TYPE_SERVER_DISCONNECT};
struct espress_stream_uint16 const espress_stream_packet_type_server_envelope[] = {ESPRESS_STREAM_PACKET_TYPE_SERVER_ENVELOPE};

static void _set_val(struct espress_stream_packet *this, struct _val *val)
{
	u_t data_size;
	if (0 == this->data_size)
	{
		espress_stream_uint16_get_u(_packet_size(this), &data_size);
		data_size += ESPRESS_STREAM_MAC_MAC_SIZE;
	}
	else
	{
		data_size = this->data_size;
	}
	if (espress_stream_uint16_eq(_packet_type(this), this->packet_type_connect) &&
		((_connect_min_size(this) + ESPRESS_STREAM_MAC_MAC_SIZE) <= data_size) &&
		(data_size <= ESPRESS_STREAM_DGRAM_MTU))
	{
		val->uint16_start = _connect_uint16_start(this);
		val->uint32_start = _connect_uint32_start(this);
		val->uint8_start = _connect_uint8_start(this);
		val->cipher_start = _connect_cipher_start(this);
		val->uint16_count = _connect_uint16_count(this);
		val->uint32_count = _connect_uint32_count(this);
	}
	else if (espress_stream_uint16_eq(_packet_type(this), this->packet_type_disconnect) &&
		((_disconnect_min_size(this) + ESPRESS_STREAM_MAC_MAC_SIZE) <= data_size) &&
		(data_size <= ESPRESS_STREAM_DGRAM_MTU))
	{
		val->uint16_start = _disconnect_uint16_start(this);
		val->uint32_start = _disconnect_uint32_start(this);
		val->uint8_start = _disconnect_uint8_start(this);
		val->cipher_start = _disconnect_cipher_start(this);
		val->uint16_count = _disconnect_uint16_count(this);
		val->uint32_count = _disconnect_uint32_count(this);
	}
	else if (espress_stream_uint16_eq(_packet_type(this), this->packet_type_envelope) &&
		(((_envelope_ack_size(this) + ESPRESS_STREAM_MAC_MAC_SIZE) == data_size) ||
			((_envelope_min_size(this) + ESPRESS_STREAM_MAC_MAC_SIZE) <= data_size)) &&
		(data_size <= ESPRESS_STREAM_DGRAM_MTU))
	{
		val->uint16_start = _envelope_uint16_start(this);
		val->uint32_start = _envelope_uint32_start(this);
		val->uint8_start = _envelope_uint8_start(this);
		val->cipher_start = _envelope_cipher_start(this);
		val->uint16_count = _envelope_uint16_count(this);
		val->uint32_count = _envelope_uint32_count(this);
	}
	else
	{
		val->uint16_start = NULL;
		val->uint32_start = NULL;
		val->uint8_start = NULL;
		val->cipher_start = NULL;
		val->uint16_count = 0;
		val->uint32_count = 0;
	}
}

static void _set_var(struct espress_stream_packet *this, struct _var *var)
{
	espress_stream_uint16_get_u(_packet_size(this), &(var->packet_size));
	if (espress_stream_uint16_eq(_packet_type(this), this->packet_type_connect) &&
		(_connect_min_size(this) <= var->packet_size))
	{
		var->uint8_count = _connect_uint8_count(this);
		var->cipher_count = _connect_cipher_count(this);
	}
	else if (espress_stream_uint16_eq(_packet_type(this), this->packet_type_disconnect) &&
		(_disconnect_min_size(this) <= var->packet_size))
	{
		var->uint8_count = _disconnect_uint8_count(this);
		var->cipher_count = _disconnect_cipher_count(this);
	}
	else if (espress_stream_uint16_eq(_packet_type(this), this->packet_type_envelope) &&
		((_envelope_ack_size(this) == var->packet_size) || (_envelope_min_size(this) <= var->packet_size)))
	{
		var->uint8_count = _envelope_uint8_count(this);
		var->cipher_count = _envelope_cipher_count(this);
	}
	else
	{
		var->packet_size = 0;
		var->uint8_count = 0;
		var->cipher_count = 0;
	}
}

static i_t _initialize(struct espress_stream_packet *this, struct espress_stream_event *event)
{
	i_t ret = 0;

	this->data_size = 0;
	this->outer_event = event;
	this->key = NULL;
	if (0 != espress_stream_event->initialize(this->event))
	{
		ret = 1;
		goto EVENT_INIT_FAIL_EXIT;
	}
	if (0 != espress_stream_mutex->initialize(this->mutex))
	{
		ret = 1;
		goto MUTEX_INIT_FAIL_EXIT;
	}

	this->packet_type_connect = espress_stream_packet_type_client_connect;
	this->packet_type_disconnect = espress_stream_packet_type_client_disconnect;
	this->packet_type_envelope = espress_stream_packet_type_client_envelope;

	this->id = NULL;

MUTEX_INIT_FAIL_EXIT:
	if (0 != ret) espress_stream_event->finalize(this->event);
EVENT_INIT_FAIL_EXIT:
	return ret;
}

static void _finalize(struct espress_stream_packet *this)
{
	espress_stream_mutex->finalize(this->mutex);
	espress_stream_event->finalize(this->event);
	memset(this, 0, sizeof(*this));
}

static struct espress_stream_uint16 *_packet_type(struct espress_stream_packet *this)
{
	return (struct espress_stream_uint16 *)((this)->data + _PACKET_TYPE_OFFSET);
}

static struct espress_stream_uint16 *_packet_size(struct espress_stream_packet *this)
{
	return (struct espress_stream_uint16 *)((this)->data + _PACKET_SIZE_OFFSET);
}

static struct espress_stream_uint32 *_connection_id(struct espress_stream_packet *this)
{
	return (struct espress_stream_uint32 *)((this)->data + _CONNECTION_ID_OFFSET);
}

static struct espress_stream_uint32 *_connection_param(struct espress_stream_packet *this)
{
	return (struct espress_stream_uint32 *)((this)->data + _CONNECTION_PARAM_OFFSET);
}

static struct espress_stream_uint32 *_initial_window_size(struct espress_stream_packet *this)
{
	return (struct espress_stream_uint32 *)((this)->data + _INITIAL_WINDOW_SIZE_OFFSET);
}

static b_t *_key_change(struct espress_stream_packet *this)
{
	return (this)->data + _KEY_CHANGE_OFFSET;
}

static u_t _key_change_size(struct espress_stream_packet *this)
{
	(void)(this);
	return 32;
}

static b_t *_random(struct espress_stream_packet *this)
{
	return (this)->data + _RANDOM_OFFSET;
}

static u_t _random_size(struct espress_stream_packet *this)
{
	(void)(this);
	return 32;
}

static struct espress_stream_uint32 *_counter_number(struct espress_stream_packet *this)
{
	return (struct espress_stream_uint32 *)(this->data + _COUNTER_NUMBER_OFFSET);
}

static struct espress_stream_uint32 *_receive_counter_number(struct espress_stream_packet *this)
{
	return (struct espress_stream_uint32 *)(this->data + _RECEIVE_COUNTER_NUMBER_OFFSET);
}

static struct espress_stream_uint32 *_receive_window_address(struct espress_stream_packet *this)
{
	return (struct espress_stream_uint32 *)(this->data + _RECEIVE_WINDOW_ADDRESS_OFFSET);
}

static struct espress_stream_uint32 *_receive_window_size(struct espress_stream_packet *this)
{
	return (struct espress_stream_uint32 *)(this->data + _RECEIVE_WINDOW_SIZE_OFFSET);
}

static struct espress_stream_uint32 *_send_data_address(struct espress_stream_packet *this)
{
	return (struct espress_stream_uint32 *)(this->data + _SEND_DATA_ADDRESS_OFFSET);
}

static b_t *_connect_payload(struct espress_stream_packet *this)
{
	return this->data + _CONNECT_PAYLOAD_OFFSET;
}

static u_t _connect_payload_size(struct espress_stream_packet *this)
{
	return this->data[_PACKET_SIZE_OFFSET + _O(1, 0)] + (this->data[_PACKET_SIZE_OFFSET + _O(1, 1)] << 8) - _CONNECT_PAYLOAD_OFFSET;
}

static b_t *_disconnect_payload(struct espress_stream_packet *this)
{
	return this->data + _DISCONNECT_PAYLOAD_OFFSET;
}

static u_t _disconnect_payload_size(struct espress_stream_packet *this)
{
	return this->data[_PACKET_SIZE_OFFSET + _O(1, 0)] + (this->data[_PACKET_SIZE_OFFSET + _O(1, 1)] << 8) - _DISCONNECT_PAYLOAD_OFFSET;
}

static b_t *_envelope_payload(struct espress_stream_packet *this)
{
	return this->data + _ENVELOPE_PAYLOAD_OFFSET;
}

static u_t _envelope_payload_size(struct espress_stream_packet *this)
{
	return this->data[_PACKET_SIZE_OFFSET + _O(1, 0)] + (this->data[_PACKET_SIZE_OFFSET + _O(1, 1)] << 8) - _ENVELOPE_PAYLOAD_OFFSET;
}

static void _encode(struct espress_stream_packet *this, i_t is_server, struct espress_stream_key *key)
{
	espress_stream_mutex->lock(this->mutex);

	if (0 == this->data_size)
	{
		if (is_server)
		{
			this->packet_type_connect = espress_stream_packet_type_server_connect;
			this->packet_type_disconnect = espress_stream_packet_type_server_disconnect;
			this->packet_type_envelope = espress_stream_packet_type_server_envelope;
		}
		else
		{
			this->packet_type_connect = espress_stream_packet_type_client_connect;
			this->packet_type_disconnect = espress_stream_packet_type_client_disconnect;
			this->packet_type_envelope = espress_stream_packet_type_client_envelope;
		}
		this->key = key;
		espress_stream_event->set(this->event, _ENCODE);
	}
	else
	{
		espress_stream_event->set(this->outer_event, ESPRESS_STREAM_EVENT_PACKET_ENCODE_RESULT);
	}

	espress_stream_mutex->unlock(this->mutex);
}

static void _decode(struct espress_stream_packet *this, i_t is_server, struct espress_stream_key *key)
{
	espress_stream_mutex->lock(this->mutex);

	if ((0 < this->data_size) && (ESPRESS_STREAM_TRANS_RING_INVALID_DATA_SIZE != this->data_size))
	{
		if (is_server)
		{
			this->packet_type_connect = espress_stream_packet_type_server_connect;
			this->packet_type_disconnect = espress_stream_packet_type_server_disconnect;
			this->packet_type_envelope = espress_stream_packet_type_server_envelope;
		}
		else
		{
			this->packet_type_connect = espress_stream_packet_type_client_connect;
			this->packet_type_disconnect = espress_stream_packet_type_client_disconnect;
			this->packet_type_envelope = espress_stream_packet_type_client_envelope;
		}
		this->key = key;
		espress_stream_event->set(this->event, _DECODE);
	}
	else
	{
		espress_stream_event->set(this->outer_event, ESPRESS_STREAM_EVENT_PACKET_DECODE_RESULT);
	}

	espress_stream_mutex->unlock(this->mutex);
}

static void _abort_encode(struct espress_stream_packet *this)
{
	espress_stream_mutex->lock(this->mutex);

	if (0 == this->data_size)
	{
		this->data_size = ESPRESS_STREAM_TRANS_RING_INVALID_DATA_SIZE;
	}
	this->key = NULL;

	espress_stream_mutex->unlock(this->mutex);
}

static void _abort_decode(struct espress_stream_packet *this)
{
	espress_stream_mutex->lock(this->mutex);

	if ((0 < this->data_size) && (ESPRESS_STREAM_TRANS_RING_INVALID_DATA_SIZE != this->data_size))
	{
		this->data_size = ESPRESS_STREAM_TRANS_RING_INVALID_DATA_SIZE;
	}
	this->key = NULL;

	espress_stream_mutex->unlock(this->mutex);
}

static void _yield(struct espress_stream_packet *this, struct espress_stream_uint32 const *timeout)
{
	u_t u;
	u_t data_size;
	struct espress_stream_uint32 const *v;
	struct espress_stream_uint32 _v[1];
	struct espress_stream_key *k;
	struct espress_stream_key _k[1];
	struct espress_stream_event_flags flags[1];
	struct _val val[1];
	struct _var var[1];
#	if _IS_ENCRYPTION_ENABLE
		struct espress_stream_ctr_cipher cipher[1];
#	endif
	struct espress_stream_mac mac[1];
	b_t mac_key[ESPRESS_STREAM_KEY_MAC_KEY_SIZE];
	b_t cipher_key[ESPRESS_STREAM_KEY_CIPHER_KEY_SIZE];
	b_t mac_body[ESPRESS_STREAM_MAC_MAC_SIZE];
#	define counter_number(this)                                                                          \
	(                                                                                                    \
		((espress_stream_uint16_eq(_packet_type(this), espress_stream_packet_type_client_connect)) ||    \
			(espress_stream_uint16_eq(_packet_type(this), espress_stream_packet_type_server_connect))) ? \
			espress_stream_uint32_0 : _counter_number(this)                                              \
	)

	espress_stream_event->wait(this->event, flags, timeout);

	espress_stream_mutex->lock(this->mutex);

	if (espress_stream_event_flags->is_set(flags, _ENCODE))
	{
		if (0 == this->data_size)
		{
			data_size = ESPRESS_STREAM_TRANS_RING_INVALID_DATA_SIZE;
			_set_val(this, val);
			if (NULL != val->uint16_start)
			{
				_set_var(this, var);
				if (0 < var->packet_size)
				{
					if (NULL == this->key)
					{
						k = _k;
						espress_stream_key->initialize(k, (b_t const *)espress_stream_uint256_0, 32, (b_t const *)espress_stream_uint256_0, 32);
						v = espress_stream_uint32_0;
					}
					else
					{
						k = this->key;
						v = _counter_number(this);
					}
					if (espress_stream_key->get(k, mac_key, cipher_key, v) && espress_stream_key->update(k, v))
					{
						if (NULL != val->uint16_start)
						{
							for (u = 0 ; u < val->uint16_count ; u++) espress_stream_uint16_hton(val->uint16_start + u);
						}
						if (NULL != val->uint32_start)
						{
							for (u = 0 ; u < val->uint32_count ; u++) espress_stream_uint32_hton(val->uint32_start + u);
						}
						if (NULL != val->cipher_start)
						{
#							if _IS_ENCRYPTION_ENABLE
								espress_stream_ctr_cipher->initialize(cipher, this->data, espress_stream_uint32_0, cipher_key);
								espress_stream_ctr_cipher->encrypt(cipher, val->cipher_start, var->cipher_count);
								espress_stream_ctr_cipher->finalize(cipher);
#							endif
						}
						data_size = (var->packet_size + 3) / 4 * 4;
						memset(this->data + var->packet_size, 0, data_size - var->packet_size);
						espress_stream_mac->initialize(mac, mac_key, ESPRESS_STREAM_KEY_MAC_KEY_SIZE);
						espress_stream_mac->update(mac, this->data, data_size);
						espress_stream_mac->finalize(mac, this->data + data_size);
						data_size += ESPRESS_STREAM_MAC_MAC_SIZE;
					}
					if (NULL == this->key)
					{
						espress_stream_key->finalize(k);
					}
				}
			}
			this->data_size = data_size;
		}
		espress_stream_event->set(this->outer_event, ESPRESS_STREAM_EVENT_PACKET_ENCODE_RESULT);
	}

	if (espress_stream_event_flags->is_set(flags, _DECODE))
	{
		if ((0 < this->data_size) && (ESPRESS_STREAM_TRANS_RING_INVALID_DATA_SIZE != this->data_size))
		{
			data_size = ESPRESS_STREAM_TRANS_RING_INVALID_DATA_SIZE;
			_set_val(this, val);
			if (NULL != val->uint16_start)
			{
				if (NULL == this->key)
				{
					k = _k;
					espress_stream_key->initialize(k, (b_t const *)espress_stream_uint256_0, 32, (b_t const *)espress_stream_uint256_0, 32);
					v = espress_stream_uint32_0;
				}
				else
				{
					k = this->key;
					v = _v;
					espress_stream_uint32_assign(_v, _counter_number(this));
					espress_stream_uint32_ntoh(_v);
				}
				if (espress_stream_key->get(k, mac_key, cipher_key, v))
				{
					espress_stream_mac->initialize(mac, mac_key, ESPRESS_STREAM_KEY_MAC_KEY_SIZE);
					espress_stream_mac->update(mac, this->data, this->data_size - ESPRESS_STREAM_MAC_MAC_SIZE);
					espress_stream_mac->finalize(mac, mac_body);
					if (0 == memcmp(this->data + (this->data_size - ESPRESS_STREAM_MAC_MAC_SIZE), mac_body, ESPRESS_STREAM_MAC_MAC_SIZE))
					{
						if (NULL != val->uint16_start)
						{
							for (u = 0 ; u < val->uint16_count ; u++) espress_stream_uint16_ntoh(val->uint16_start + u);
						}
						_set_var(this, var);
						if ((0 < var->packet_size) &&
							(((var->packet_size + 3) / 4 * 4) == (this->data_size - ESPRESS_STREAM_MAC_MAC_SIZE)))
						{
							if (NULL != val->cipher_start)
							{
#								if _IS_ENCRYPTION_ENABLE
									espress_stream_ctr_cipher->initialize(cipher, this->data, espress_stream_uint32_0, cipher_key);
									espress_stream_ctr_cipher->decrypt(cipher, val->cipher_start, var->cipher_count);
									espress_stream_ctr_cipher->finalize(cipher);
#								endif
							}
							if (NULL != val->uint32_start)
							{
								for (u = 0 ; u < val->uint32_count ; u++) espress_stream_uint32_ntoh(val->uint32_start + u);
							}
							if (espress_stream_key->update(k, v))
							{
								data_size = 0;
							}
						}
					}
				}
				if (NULL == this->key)
				{
					espress_stream_key->finalize(k);
				}
			}
			this->data_size = data_size;
		}
		espress_stream_event->set(this->outer_event, ESPRESS_STREAM_EVENT_PACKET_DECODE_RESULT);
	}

	if (espress_stream_event_flags->is_set(flags, _NONE))
	{
		/* nop */
	}

	espress_stream_mutex->unlock(this->mutex);
}

static void _yield_break(struct espress_stream_packet *this)
{
	espress_stream_event->set(this->event, _NONE);
}
