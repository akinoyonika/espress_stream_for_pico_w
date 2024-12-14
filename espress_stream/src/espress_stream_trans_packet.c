#include "espress_stream/config.h"
#include "espress_stream/depend.h"
#include "espress_stream/uint.h"
#include "espress_stream/tick.h"
#include "espress_stream/mutex.h"
#include "espress_stream/cond.h"
#include "espress_stream/event.h"
#include "espress_stream/hash.h"
#include "espress_stream/mac.h"
#include "espress_stream/rand.h"
#include "espress_stream/block_cipher.h"
#include "espress_stream/ctr_cipher.h"
#include "espress_stream/hash.h"
#include "espress_stream/mac.h"
#include "espress_stream/hkdf.h"
#include "espress_stream/ec.h"
#include "espress_stream/key_ex.h"
#include "espress_stream/key_exp.h"
#include "espress_stream/key.h"
#include "espress_stream/ring.h"
#include "espress_stream/dgram.h"
#include "espress_stream/trans_ring.h"
#include "espress_stream/packet.h"
#include "espress_stream/trans_packet.h"

#define _IS_CONNECTION_ID_RANDOM (1)

struct espress_stream_uint32 const espress_stream_min_rto[1] = {ESPRESS_STREAM_UINT32(0x00, 0x03, 0x0d, 0x40)};
struct espress_stream_uint32 const espress_stream_max_rto[1] = {ESPRESS_STREAM_SESSION_TIMEOUT_TIME};

static i_t _initialize(struct espress_stream_trans_packet_interface *this,
	struct espress_stream_dgram_address const *address);
static void _finalize(struct espress_stream_trans_packet_interface *this);
static u_t _mtu(struct espress_stream_trans_packet_interface *this);
static i_t _open(struct espress_stream_trans_packet *this,
	struct espress_stream_trans_packet_interface *interface,
	struct espress_stream_event *event,
	i_t is_server);
static void _close(struct espress_stream_trans_packet *this);
static i_t _send_connect(struct espress_stream_trans_packet *this,
	struct espress_stream_dgram_address const *address,
	struct espress_stream_uint32 *connection_param,
	u_t initial_window_size,
	b_t const *payload, u_t payload_size);
static i_t _recv_connect(struct espress_stream_trans_packet *this,
	struct espress_stream_dgram_address *address,
	struct espress_stream_uint32 *connection_param,
	u_t *initial_window_size,
	b_t *payload, u_t *payload_size);
static i_t _send_disconnect(struct espress_stream_trans_packet *this,
	b_t const *payload, u_t payload_size);
static i_t _recv_disconnect(struct espress_stream_trans_packet *this,
		b_t *payload, u_t *payload_size);
static i_t _send_envelope(struct espress_stream_trans_packet *this,
	struct espress_stream_uint32 const *receive_window_address,
	struct espress_stream_uint32 const *receive_window_size,
	struct espress_stream_uint32 const *send_data_address,
	b_t const *payload, u_t payload_size,
	b_t const *add_payload, u_t add_payload_size);
static i_t _recv_envelope(struct espress_stream_trans_packet *this,
	struct espress_stream_uint32 *receive_window_address,
	struct espress_stream_uint32 *receive_window_size,
	i_t *is_ack_true,
	struct espress_stream_uint32 *send_data_address,
	struct espress_stream_uint32 const *payload_address,
	b_t *payload, u_t *payload_size,
	b_t *add_payload, u_t *add_payload_size);
static i_t _send_ack(struct espress_stream_trans_packet *this,
	struct espress_stream_uint32 const *receive_window_address,
	struct espress_stream_uint32 const *receive_window_size);
static i_t _recv_ack(struct espress_stream_trans_packet *this,
	struct espress_stream_uint32 *receive_window_address,
	struct espress_stream_uint32 *receive_window_size,
	i_t *is_ack_true);
static i_t _purge(struct espress_stream_trans_packet *this);
static void _src_address(struct espress_stream_trans_packet *this, struct espress_stream_dgram_address *address);
static void _dst_address(struct espress_stream_trans_packet *this, struct espress_stream_dgram_address *address);
static void _connection_id(struct espress_stream_trans_packet *this, struct espress_stream_uint32 *connection_id);
static void _shared_key(struct espress_stream_trans_packet *this, b_t *shared_key);
static i_t _rtt(struct espress_stream_trans_packet *this, struct espress_stream_uint32 *rtt);
static void _rto(struct espress_stream_trans_packet *this, struct espress_stream_uint32 *rto);
static void _tell_timeout(struct espress_stream_trans_packet *this);
static u_t _dac(struct espress_stream_trans_packet *this);

#define _SEND_PACKET_YIELD_PROTO(count) static void _send_packet_yield_ ## count(struct espress_stream_trans_packet_interface *this, struct espress_stream_uint32 const *timeout);
#define ESPRESS_STREAM_MACRO_REPEAT (ESPRESS_STREAM_SEND_QUEUE_SIZE)
#include "espress_stream/macro_repeat.h"
ESPRESS_STREAM_MACRO_REPEAT(_SEND_PACKET_YIELD_PROTO)
#undef ESPRESS_STREAM_MACRO_REPEAT

#define _RECV_PACKET_YIELD_PROTO(count) static void _recv_packet_yield_ ## count(struct espress_stream_trans_packet_interface *this, struct espress_stream_uint32 const *timeout);
#define ESPRESS_STREAM_MACRO_REPEAT (ESPRESS_STREAM_RECV_QUEUE_SIZE)
#include "espress_stream/macro_repeat.h"
ESPRESS_STREAM_MACRO_REPEAT(_RECV_PACKET_YIELD_PROTO)
#undef ESPRESS_STREAM_MACRO_REPEAT

static void _dgram_send_yield(struct espress_stream_trans_packet_interface *this, struct espress_stream_uint32 const *timeout);
static void _dgram_recv_yield(struct espress_stream_trans_packet_interface *this, struct espress_stream_uint32 const *timeout);

static void _this_yield(struct espress_stream_trans_packet_interface *this, struct espress_stream_uint32 const *timeout);

#define _SEND_PACKET_YIELD_BREAK_PROTO(count) static void _send_packet_yield_break_ ## count(struct espress_stream_trans_packet_interface *this);
#define ESPRESS_STREAM_MACRO_REPEAT (ESPRESS_STREAM_SEND_QUEUE_SIZE)
#include "espress_stream/macro_repeat.h"
ESPRESS_STREAM_MACRO_REPEAT(_SEND_PACKET_YIELD_BREAK_PROTO)
#undef ESPRESS_STREAM_MACRO_REPEAT

#define _RECV_PACKET_YIELD_BREAK_PROTO(count) static void _recv_packet_yield_break_ ## count(struct espress_stream_trans_packet_interface *this);
#define ESPRESS_STREAM_MACRO_REPEAT (ESPRESS_STREAM_RECV_QUEUE_SIZE)
#include "espress_stream/macro_repeat.h"
ESPRESS_STREAM_MACRO_REPEAT(_RECV_PACKET_YIELD_BREAK_PROTO)
#undef ESPRESS_STREAM_MACRO_REPEAT

static void _dgram_send_yield_break(struct espress_stream_trans_packet_interface *this);
static void _dgram_recv_yield_break(struct espress_stream_trans_packet_interface *this);

static void _this_yield_break(struct espress_stream_trans_packet_interface *this);

struct espress_stream_trans_packet_ops const espress_stream_trans_packet[] =
{{
	_initialize,
	_finalize,
	_mtu,
	_open,
	_close,
	_send_connect,
	_recv_connect,
	_send_disconnect,
	_recv_disconnect,
	_send_envelope,
	_recv_envelope,
	_send_ack,
	_recv_ack,
	_purge,
	_src_address,
	_dst_address,
	_connection_id,
	_shared_key,
	_rtt,
	_rto,
	_tell_timeout,
	_dac,
	{
#		define _SEND_PACKET_YIELD(count) _send_packet_yield_ ## count,
#		define ESPRESS_STREAM_MACRO_REPEAT (ESPRESS_STREAM_SEND_QUEUE_SIZE)
#		include "espress_stream/macro_repeat.h"
		ESPRESS_STREAM_MACRO_REPEAT(_SEND_PACKET_YIELD)
#		undef ESPRESS_STREAM_MACRO_REPEAT

#		define _RECV_PACKET_YIELD(count) _recv_packet_yield_ ## count,
#		define ESPRESS_STREAM_MACRO_REPEAT (ESPRESS_STREAM_RECV_QUEUE_SIZE)
#		include "espress_stream/macro_repeat.h"
		ESPRESS_STREAM_MACRO_REPEAT(_RECV_PACKET_YIELD)
#		undef ESPRESS_STREAM_MACRO_REPEAT

		_dgram_send_yield,
		_dgram_recv_yield,

		_this_yield,

		NULL
	},

	{
#		define _SEND_PACKET_YIELD_BREAK(count) _send_packet_yield_break_ ## count,
#		define ESPRESS_STREAM_MACRO_REPEAT (ESPRESS_STREAM_SEND_QUEUE_SIZE)
#		include "espress_stream/macro_repeat.h"
		ESPRESS_STREAM_MACRO_REPEAT(_SEND_PACKET_YIELD_BREAK)
#		undef ESPRESS_STREAM_MACRO_REPEAT

#		define _RECV_PACKET_YIELD_BREAK(count) _recv_packet_yield_break_ ## count,
#		define ESPRESS_STREAM_MACRO_REPEAT (ESPRESS_STREAM_RECV_QUEUE_SIZE)
#		include "espress_stream/macro_repeat.h"
		ESPRESS_STREAM_MACRO_REPEAT(_RECV_PACKET_YIELD_BREAK)
#		undef ESPRESS_STREAM_MACRO_REPEAT

		_dgram_send_yield_break,
		_dgram_recv_yield_break,

		_this_yield_break,

		NULL
	}
}};

#define _espress_stream_packet_envelope_payload_max_size(this) \
	(espress_stream_packet_envelope_payload_max_size(_mtu((this)->interface)))

#if 0 < ESPRESS_STREAM_MAX_NUM_SESSION
	static void _init_trans_packet(struct espress_stream_trans_packet_interface *this)
	{
		memset(this->trans_packet, 0, sizeof(this->trans_packet));
	}
	static void _final_trans_packet(struct espress_stream_trans_packet_interface *this)
	{
		memset(this->trans_packet, 0, sizeof(this->trans_packet));
	}
	static struct espress_stream_trans_packet *_add_trans_packet(struct espress_stream_trans_packet_interface *this, struct espress_stream_trans_packet *tp)
	{
		i_t i;
		struct espress_stream_trans_packet *ret = NULL;
		for (i = 0 ; i < ESPRESS_STREAM_MAX_NUM_SESSION ; i++)
		{
			if (NULL == this->trans_packet[i])
			{
				break;
			}
		}
		if (i < ESPRESS_STREAM_MAX_NUM_SESSION)
		{
			this->trans_packet[i] = tp;
			ret = tp;
		}
		return ret;
	}
	static void _del_trans_packet(struct espress_stream_trans_packet_interface *this, struct espress_stream_trans_packet *tp)
	{
		i_t i;
		for (i = 0 ; i < ESPRESS_STREAM_MAX_NUM_SESSION ; i++)
		{
			if (tp == this->trans_packet[i])
			{
				break;
			}
		}
		if (i < ESPRESS_STREAM_MAX_NUM_SESSION)
		{
			this->trans_packet[i] = NULL;
		}
	}
	static struct espress_stream_trans_packet *_find_trans_packet(struct espress_stream_trans_packet_interface *this, i_t is_server, struct espress_stream_uint32 const *connection_id)
	{
		i_t i;
		struct espress_stream_trans_packet *ret = NULL;
		for (i = 0 ; i < ESPRESS_STREAM_MAX_NUM_SESSION ; i++)
		{
			if ((NULL != this->trans_packet[i]) &&
				(is_server ? this->trans_packet[i]->is_server : !this->trans_packet[i]->is_server) &&
				espress_stream_uint32_eq(connection_id, this->trans_packet[i]->connection_id))
			{
				break;
			}
		}
		if (i < ESPRESS_STREAM_MAX_NUM_SESSION)
		{
			ret = this->trans_packet[i];
		}
		return ret;
	}
	static struct espress_stream_trans_packet *_get_next_trans_packet(struct espress_stream_trans_packet_interface *this, struct espress_stream_trans_packet *tp)
	{
		struct espress_stream_trans_packet *ret = NULL;
		if ((NULL == tp) || (*(this->ptpl) == tp))
		{
			if (NULL == tp)
			{
				this->ptpl = this->trans_packet;
			}
			else
			{
				(this->ptpl)++;
			}
			for ( ; (this->ptpl - this->trans_packet) < ESPRESS_STREAM_MAX_NUM_SESSION ; (this->ptpl)++)
			{
				if (NULL != *(this->ptpl))
				{
					ret = *(this->ptpl);
					break;
				}
			}
		}
		return ret;
	}
	static struct espress_stream_trans_packet *_get_first_trans_packet(struct espress_stream_trans_packet_interface *this)
	{
		return _get_next_trans_packet(this, NULL);
	}
#else
	static void _init_trans_packet(struct espress_stream_trans_packet_interface *this)
	{
		this->trans_packet_list = NULL;
	}
	static void _final_trans_packet(struct espress_stream_trans_packet_interface *this)
	{
		struct espress_stream_trans_packet_interface_trans_packet_list *p;
		struct espress_stream_trans_packet_interface_trans_packet_list *n;
		for (p = NULL, n = this->trans_packet_list ; NULL != n ; p = n, n = n->next)
		{
			if (NULL != p)
			{
				memset(p, 0, sizeof(*p));
				free(p);
			}
		}
		if (NULL != p)
		{
			memset(p, 0, sizeof(*p));
			free(p);
		}
	}
	static struct espress_stream_trans_packet *_add_trans_packet(struct espress_stream_trans_packet_interface *this, struct espress_stream_trans_packet *tp)
	{
		struct espress_stream_trans_packet *ret = NULL;
		struct espress_stream_trans_packet_interface_trans_packet_list *p;
		struct espress_stream_trans_packet_interface_trans_packet_list *n;
		n = (struct espress_stream_trans_packet_interface_trans_packet_list *)malloc(sizeof(struct espress_stream_trans_packet));
		if (NULL != n)
		{
			for (p = this->trans_packet_list ; NULL != p ; p = p->next)
			{
				if (NULL == p->next)
				{
					p->next = n;
					break;
				}
			}
			if (NULL == p)
			{
				this->trans_packet_list = n;
			}
			n->trans_packet = tp;
			n->next = NULL;
			ret = n->trans_packet;
		}
		return ret;
	}
	static void _del_trans_packet(struct espress_stream_trans_packet_interface *this, struct espress_stream_trans_packet *tp)
	{
		struct espress_stream_trans_packet_interface_trans_packet_list *p;
		struct espress_stream_trans_packet_interface_trans_packet_list *n;
		for (p = NULL, n = this->trans_packet_list ; NULL != n ; p = n, n = n->next)
		{
			if (tp == n->trans_packet)
			{
				break;
			}
		}
		if (NULL != n)
		{
			if (NULL == p)
			{
				this->trans_packet_list = n->next;
			}
			else
			{
				p->next = n->next;
			}
			memset(n, 0, sizeof(*n));
			free(n);
		}
	}
	static struct espress_stream_trans_packet *_find_trans_packet(struct espress_stream_trans_packet_interface *this, i_t is_server, struct espress_stream_uint32 const *connection_id)
	{
		struct espress_stream_trans_packet *ret = NULL;
		struct espress_stream_trans_packet_interface_trans_packet_list *n;
		for (n = this->trans_packet_list ; NULL != n ; n = n->next)
		{
			if ((is_server ? n->trans_packet->is_server : !n->trans_packet->is_server) &&
				espress_stream_uint32_eq(connection_id, n->trans_packet->connection_id))
			{
				break;
			}
		}
		if (NULL != n)
		{
			ret = n->trans_packet;
		}
		return ret;
	}
	static struct espress_stream_trans_packet *_get_next_trans_packet(struct espress_stream_trans_packet_interface *this, struct espress_stream_trans_packet *tp)
	{
		struct espress_stream_trans_packet *ret = NULL;
		if ((NULL == tp) || (this->ptpl->trans_packet == tp))
		{
			if (NULL == tp)
			{
				this->ptpl = this->trans_packet_list;
			}
			else
			{
				this->ptpl = this->ptpl->next;
			}
			if (NULL != this->ptpl)
			{
				ret = this->ptpl->trans_packet;
			}
		}
		return ret;
	}
	static struct espress_stream_trans_packet *_get_first_trans_packet(struct espress_stream_trans_packet_interface *this)
	{
		return _get_next_trans_packet(this, NULL);
	}
#endif

static i_t _initialize(struct espress_stream_trans_packet_interface *this,
	struct espress_stream_dgram_address const *address)
{
	i_t ret = 0;
	i_t i;
	u_t u;
	struct espress_stream_packet *p;

	if (0 != espress_stream_mutex->initialize(this->mutex))
	{
		ret = 1;
		goto MUTEX_INIT_FAIL_EXIT;
	}
	if (0 != espress_stream_event->initialize(this->event))
	{
		ret = 1;
		goto EVENT_INIT_FAIL_EXIT;
	}
	memcpy(this->address, address, sizeof(*(this->address)));
	if (0 != espress_stream_dgram->initialize(this->dgram, address, this->event, this->event))
	{
		ret = 1;
		goto DGRAM_INIT_FAIL_EXIT;
	}
	for (i = 0 ; i < ESPRESS_STREAM_SEND_QUEUE_SIZE ; i++)
	{
		this->send_buffers[i] = (struct espress_stream_dgram_buffer *)(this->send_packet + i);
	}
	for (i = 0 ; i < ESPRESS_STREAM_RECV_QUEUE_SIZE ; i++)
	{
		this->recv_buffers[i] = (struct espress_stream_dgram_buffer *)(this->recv_packet + i);
	}
	espress_stream_trans_ring->initialize(this->send_ring, this->send_buffers, ESPRESS_STREAM_SEND_QUEUE_SIZE, this->dgram);
	espress_stream_trans_ring->initialize(this->recv_ring, this->recv_buffers, ESPRESS_STREAM_RECV_QUEUE_SIZE, this->dgram);
	for (u = 0 ; NULL != espress_stream_trans_ring->head(this->send_ring, u) ; u++)
	{
		p = (struct espress_stream_packet *)(espress_stream_trans_ring->head(this->send_ring, u)[0]);
		if (0 != espress_stream_packet->initialize(p, this->event))
		{
			ret = 1;
			goto PACKET_INIT_FAIL_EXIT;
		}
	}
	for ( ; NULL != espress_stream_trans_ring->head(this->recv_ring, 0) ; )
	{
		p = (struct espress_stream_packet *)(espress_stream_trans_ring->head(this->recv_ring, 0)[0]);
		if (0 != espress_stream_packet->initialize(p, this->event))
		{
			ret = 1;
			goto PACKET_INIT_FAIL_EXIT;
		}
		p->data_size = espress_stream_dgram->mtu(this->dgram);
		espress_stream_trans_ring->en(this->recv_ring, 1);
	}
	espress_stream_trans_ring->recv_request(this->recv_ring);

	_init_trans_packet(this);

PACKET_INIT_FAIL_EXIT:
	if (0 != ret) espress_stream_dgram->finalize(this->dgram);
DGRAM_INIT_FAIL_EXIT:
	if (0 != ret) espress_stream_event->finalize(this->event);
EVENT_INIT_FAIL_EXIT:
	if (0 != ret) espress_stream_mutex->finalize(this->mutex);
MUTEX_INIT_FAIL_EXIT:

	return ret;
}

static void _finalize(struct espress_stream_trans_packet_interface *this)
{
	_final_trans_packet(this);
	espress_stream_trans_ring->finalize(this->recv_ring);
	espress_stream_trans_ring->finalize(this->send_ring);
	espress_stream_dgram->finalize(this->dgram);
	espress_stream_event->finalize(this->event);
	espress_stream_mutex->finalize(this->mutex);
}

static u_t _mtu(struct espress_stream_trans_packet_interface *this)
{
	return espress_stream_dgram->mtu(this->dgram);
}

static void _init_rto_measurement(struct espress_stream_trans_packet_rto_measurement *this)
{
	this->is_in_progress = 0;
	this->is_rtt_valid = 0;

	this->is_rto_calculated = 0;
	espress_stream_uint32_assign(this->rto, espress_stream_tick_us_1_sec);
}

static void _final_rto_measurement(struct espress_stream_trans_packet_rto_measurement *this)
{
	(void)this;
}

static void _start_rto_measurement(struct espress_stream_trans_packet_rto_measurement *this,
	struct espress_stream_uint32 const *origin,
	struct espress_stream_uint32 const *counter_number)
{
	struct espress_stream_uint32 u[1];
	struct espress_stream_uint32 v[1];

	if (this->is_in_progress)
	{
		espress_stream_tick->get_us(u);
		espress_stream_uint32_assign(v, espress_stream_max_rto);
		espress_stream_uint32_shift(v, 1);
		espress_stream_uint32_sub(u, this->start_time);
		if (espress_stream_uint32_lt(v, u))
		{
			this->is_in_progress = 0;
		}
	}

	if (!this->is_in_progress)
	{
		this->is_in_progress = 1;
		espress_stream_uint32_assign(this->origin_counter_number, origin);
		espress_stream_uint32_assign(this->start_counter_number_inc, counter_number);
		espress_stream_uint32_sub(this->start_counter_number_inc, origin);
		espress_stream_tick->get_us(this->start_time);
	}
}

static void _stop_rto_measurement(struct espress_stream_trans_packet_rto_measurement *this,
	struct espress_stream_uint32 const *counter_number)
{
	struct espress_stream_uint32 u[1];
	struct espress_stream_uint32 v[1];

	if (this->is_in_progress)
	{
		espress_stream_uint32_assign(u, counter_number);
		espress_stream_uint32_sub(u, this->origin_counter_number);
		if (espress_stream_uint32_le(this->start_counter_number_inc, u))
		{
			if (espress_stream_uint32_eq(this->start_counter_number_inc, u))
			{
				espress_stream_tick->get_us(this->rtt);
				espress_stream_uint32_sub(this->rtt, this->start_time);
				this->is_rtt_valid = 1;
			}
			this->is_in_progress = 0;
		}
	}

	if (this->is_rtt_valid)
	{
		if (this->is_rto_calculated)
		{
			espress_stream_uint32_assign(u, this->rttvar);
			espress_stream_uint32_shift(u, -3);
			if (espress_stream_uint32_lt(this->srtt, this->rtt))
			{
				espress_stream_uint32_assign(v, this->rtt);
				espress_stream_uint32_sub(v, this->srtt);
			}
			else
			{
				espress_stream_uint32_assign(v, this->srtt);
				espress_stream_uint32_sub(v, this->rtt);
			}
			espress_stream_uint32_shift(v, -3);
			espress_stream_uint32_sub(this->rttvar, u);
			espress_stream_uint32_add(this->rttvar, v);

			espress_stream_uint32_assign(u, this->srtt);
			espress_stream_uint32_shift(u, -2);
			espress_stream_uint32_assign(v, this->rtt);
			espress_stream_uint32_shift(v, -2);
			espress_stream_uint32_sub(this->srtt, u);
			espress_stream_uint32_add(this->srtt, v);
		}
		else
		{
			espress_stream_uint32_assign(this->rttvar, this->rtt);
			espress_stream_uint32_shift(this->rttvar, -1);
			espress_stream_uint32_assign(this->srtt, this->rtt);
			this->is_rto_calculated = 1;
		}

		espress_stream_uint32_assign(this->rto, this->rttvar);
		espress_stream_uint32_shift(this->rto, 2);
		if (espress_stream_uint32_lt(this->rto, espress_stream_uint32_1))
		{
			espress_stream_uint32_assign(this->rto, espress_stream_uint32_1);
		}
		espress_stream_uint32_add(this->rto, this->srtt);

		if (espress_stream_uint32_lt(this->rto, espress_stream_min_rto))
		{
			espress_stream_uint32_assign(this->rto, espress_stream_min_rto);
		}
		if (espress_stream_uint32_lt(espress_stream_max_rto, this->rto))
		{
			espress_stream_uint32_assign(this->rto, espress_stream_max_rto);
		}
	}
}

static void _tell_timeout_rto_measurement(struct espress_stream_trans_packet_rto_measurement *this)
{
	espress_stream_uint32_shift(this->rto, 1);
	if (espress_stream_uint32_lt(espress_stream_max_rto, this->rto))
	{
		espress_stream_uint32_assign(this->rto, espress_stream_max_rto);
	}
}

static void _init_duplicate_ack_count(struct espress_stream_trans_packet_duplicate_ack_count *this)
{
	this->count = 0;
}

static void _final_duplicate_ack_count(struct espress_stream_trans_packet_duplicate_ack_count *this)
{
	(void)this;
}

static void _check_duplicate_ack_count(struct espress_stream_trans_packet_duplicate_ack_count *this,
	struct espress_stream_uint32 const *prev_counter_number,
	struct espress_stream_uint32 const *prev_window_address,
	struct espress_stream_uint32 const *counter_number,
	struct espress_stream_uint32 const *window_address)
{
	if (espress_stream_uint32_eq(prev_window_address, window_address))
	{
		if (!espress_stream_uint32_eq(prev_counter_number, counter_number))
		{
			this->count++;
		}
	}
	else
	{
		this->count = 0;
	}
}

static i_t _uint32_lt(struct espress_stream_uint32 const *this, struct espress_stream_uint32 const *n)
{
	struct espress_stream_uint32 u[1];
	espress_stream_uint32_assign(u, this);
	espress_stream_uint32_sub(u, n);
	espress_stream_uint32_rotate(u, 1);
	espress_stream_uint32_and(u, espress_stream_uint32_1);
	return espress_stream_uint32_eq(u, espress_stream_uint32_1);
}

static i_t _uint32_le(struct espress_stream_uint32 const *this, struct espress_stream_uint32 const *n)
{
	return !_uint32_lt(n, this);
}

static i_t _open(struct espress_stream_trans_packet *this,
	struct espress_stream_trans_packet_interface *interface,
	struct espress_stream_event *event,
	i_t is_server)
{
	i_t ret = 0;

	this->outer_event = event;

	this->interface = interface;
	this->is_server = is_server;
	this->src_address = this->interface->address;

	espress_stream_key->initialize(this->send_key, (b_t const *)espress_stream_uint256_0, 32, (b_t const *)espress_stream_uint256_0, 32);
	espress_stream_key->initialize(this->recv_key, (b_t const *)espress_stream_uint256_0, 32, (b_t const *)espress_stream_uint256_0, 32);

	espress_stream_rand->get(this->src_private_key, ESPRESS_STREAM_KEY_EX_KEY_SIZE);
	espress_stream_key_ex->get_public_key(this->src_public_key, this->src_private_key);

	espress_stream_uint32_assign(this->connection_id, espress_stream_uint32_0);
	espress_stream_uint32_assign(this->src_counter_number, espress_stream_uint32_0);
	espress_stream_uint32_assign(this->src_receive_counter_number, espress_stream_uint32_max);
	espress_stream_uint32_assign(this->dst_connection_id, espress_stream_uint32_0);
	espress_stream_uint32_assign(this->dst_counter_number, espress_stream_uint32_max);
	espress_stream_uint32_assign(this->dst_receive_counter_number, espress_stream_uint32_max);
	espress_stream_uint32_assign(this->dst_receive_window_address, espress_stream_uint32_max);

	espress_stream_mutex->lock(this->interface->mutex);

	if (NULL == _add_trans_packet(this->interface, this))
	{
		ret = 1;
		goto SESSION_FULL_EXIT;
	}

	if (this->is_server)
	{
		this->src_packet_type_connect = espress_stream_packet_type_server_connect;
		this->src_packet_type_disconnect = espress_stream_packet_type_server_disconnect;
		this->src_packet_type_envelope = espress_stream_packet_type_server_envelope;
		this->dst_packet_type_connect = espress_stream_packet_type_client_connect;
		this->dst_packet_type_disconnect = espress_stream_packet_type_client_disconnect;
		this->dst_packet_type_envelope = espress_stream_packet_type_client_envelope;
	}
	else
	{
		this->src_packet_type_connect = espress_stream_packet_type_client_connect;
		this->src_packet_type_disconnect = espress_stream_packet_type_client_disconnect;
		this->src_packet_type_envelope = espress_stream_packet_type_client_envelope;
		this->dst_packet_type_connect = espress_stream_packet_type_server_connect;
		this->dst_packet_type_disconnect = espress_stream_packet_type_server_disconnect;
		this->dst_packet_type_envelope = espress_stream_packet_type_server_envelope;
	}

	_init_rto_measurement(this->rto_measurement);
	_init_duplicate_ack_count(this->duplicate_ack_count);

SESSION_FULL_EXIT:

	espress_stream_mutex->unlock(this->interface->mutex);

	return ret;
}

static void _close(struct espress_stream_trans_packet *this)
{
	u_t u;
	struct espress_stream_packet *p;
	espress_stream_mutex->lock(this->interface->mutex);
	for (u = 0 ; NULL != espress_stream_trans_ring->tail(this->interface->send_ring, u) ; u++)
	{
		p = (struct espress_stream_packet *)espress_stream_trans_ring->tail(this->interface->send_ring, u)[0];
		if (p->id == (void *)this)
		{
			espress_stream_packet->yield(p, espress_stream_uint32_0);
			p->id = NULL;
		}
	}
	for (u = 0 ; NULL != espress_stream_trans_ring->head(this->interface->recv_ring, u) ; u++)
	{
		p = (struct espress_stream_packet *)espress_stream_trans_ring->head(this->interface->recv_ring, u)[0];
		if (p->id == (void *)this)
		{
			espress_stream_packet->abort_decode(p);
			p->id = NULL;
		}
	}
	_final_duplicate_ack_count(this->duplicate_ack_count);
	_final_rto_measurement(this->rto_measurement);
	_del_trans_packet(this->interface, this);
	espress_stream_key->finalize(this->recv_key);
	espress_stream_key->finalize(this->send_key);
	espress_stream_mutex->unlock(this->interface->mutex);
	memset(this, 0, sizeof(*this));
}

static i_t _send_connect(struct espress_stream_trans_packet *this,
	struct espress_stream_dgram_address const *address,
	struct espress_stream_uint32 *connection_param,
	u_t initial_window_size,
	b_t const *payload, u_t payload_size)
{
	i_t ret = 0;
	struct espress_stream_packet *p;
	struct espress_stream_uint32 u[1];
	struct espress_stream_uint32 connection_param_var[1];
	struct espress_stream_uint32 const reject_param_val[1] = {ESPRESS_STREAM_UINT32(0xf0, 0x00, 0x00, 0x00)};

	memcpy(this->dst_address, address, sizeof(*(this->dst_address)));
	espress_stream_uint32_assign(connection_param_var, connection_param);

	if (NULL == payload)
	{
		payload_size = 0;
	}
	if ((ESPRESS_STREAM_PACKET_MAX_SIZE - ESPRESS_STREAM_PACKET_CONNECT_MIN_SIZE) < payload_size)
	{
		payload_size = ESPRESS_STREAM_PACKET_MAX_SIZE - ESPRESS_STREAM_PACKET_CONNECT_MIN_SIZE;
	}

	espress_stream_mutex->lock(this->interface->mutex);

	if (this->is_server)
	{
		if (NULL == _find_trans_packet(this->interface, 1, this->dst_connection_id))
		{
			espress_stream_uint32_assign(this->connection_id, this->dst_connection_id);
		}
		else
		{
			espress_stream_uint32_or(connection_param_var, reject_param_val);
		}
	}
	else
	{
#		if !_IS_CONNECTION_ID_RANDOM
			espress_stream_uint32_assign(u, espress_stream_uint32_0);
#		endif
		for ( ; ; )
		{
#			if _IS_CONNECTION_ID_RANDOM
				espress_stream_rand->get((b_t *)u, sizeof(*u));
#			else
				espress_stream_uint32_add(u, espress_stream_uint32_1);
#			endif
			if (espress_stream_uint32_eq(u, espress_stream_uint32_0) ||
				espress_stream_uint32_eq(u, espress_stream_uint32_max))
			{
				continue;
			}
			if (NULL == _find_trans_packet(this->interface, 0, u))
			{
				espress_stream_uint32_assign(this->connection_id, u);
				break;
			}
		}
	}

	espress_stream_rand->get(this->src_random, sizeof(this->src_random));

	if (NULL == espress_stream_trans_ring->head(this->interface->send_ring, 0))
	{
		ret = -1;
		goto SEND_QUEUE_FULL_EXIT;
	}
	p = (struct espress_stream_packet *)espress_stream_trans_ring->head(this->interface->send_ring, 0)[0];
	p->data_size = 0;
	memcpy(p->address, this->dst_address, sizeof(*(p->address)));
	espress_stream_uint16_assign(espress_stream_packet->packet_type(p), this->src_packet_type_connect);
	espress_stream_uint16_set_u(espress_stream_packet->packet_size(p), ESPRESS_STREAM_PACKET_CONNECT_MIN_SIZE + payload_size);
	if (this->is_server)
	{
		espress_stream_uint32_assign(espress_stream_packet->connection_id(p), this->dst_connection_id);
	}
	else
	{
		espress_stream_uint32_assign(espress_stream_packet->connection_id(p), this->connection_id);
	}
	espress_stream_uint32_assign(espress_stream_packet->connection_param(p), connection_param_var);
	espress_stream_uint32_set_u(espress_stream_packet->initial_window_size(p), initial_window_size);
	memcpy(espress_stream_packet->key_change(p), this->src_public_key, espress_stream_packet->key_change_size(p));
	memcpy(espress_stream_packet->random(p), this->src_random, espress_stream_packet->random_size(p));
	if (0 < payload_size)
	{
		memcpy(espress_stream_packet->connect_payload(p), payload, payload_size);
	}

	p->id = (void *)this;
	espress_stream_trans_ring->en(this->interface->send_ring, 1);
	espress_stream_packet->encode(p, this->is_server, NULL);

	espress_stream_uint32_assign(connection_param, connection_param_var);

	if (this->is_server)
	{
		espress_stream_uint32_and(connection_param_var, reject_param_val);
		if (espress_stream_uint32_eq(connection_param_var, espress_stream_uint32_0))
		{
			espress_stream_key->finalize(this->recv_key);
			espress_stream_key->finalize(this->send_key);
			espress_stream_key_ex->get_shared_key(this->shared_key, this->src_private_key, this->dst_public_key);
			espress_stream_key->initialize(this->send_key, this->shared_key, sizeof(this->shared_key), this->src_random, sizeof(this->src_random));
			espress_stream_key->initialize(this->recv_key, this->shared_key, sizeof(this->shared_key), this->dst_random, sizeof(this->dst_random));
		}
	}

SEND_QUEUE_FULL_EXIT:

	espress_stream_mutex->unlock(this->interface->mutex);

	return ret;
}

static i_t _recv_connect(struct espress_stream_trans_packet *this,
	struct espress_stream_dgram_address *address,
	struct espress_stream_uint32 *connection_param,
	u_t *initial_window_size,
	b_t *payload, u_t *payload_size)
{
	i_t ret = 0;
	u_t u;
	struct espress_stream_packet *p;
	struct espress_stream_uint32 const reject_param_val[1] = {ESPRESS_STREAM_UINT32(0xf0, 0x00, 0x00, 0x00)};
	u_t payload_size_var;

	if ((NULL == payload) || (NULL == payload_size))
	{
		payload_size_var = 0;
	}
	else
	{
		payload_size_var = *payload_size;
	}

	espress_stream_mutex->lock(this->interface->mutex);

	if (NULL == espress_stream_trans_ring->head(this->interface->recv_ring, 0))
	{
		ret = -1;
		goto RECV_QUEUE_EMPTY_EXIT;
	}
	p = (struct espress_stream_packet *)(espress_stream_trans_ring->head(this->interface->recv_ring, 0)[0]);
	if (this != (struct espress_stream_trans_packet *)(p->id))
	{
		ret = -1;
		goto MISMATCH_PACKET_ID_EXIT;
	}
	if (ESPRESS_STREAM_TRANS_RING_INVALID_DATA_SIZE == p->data_size)
	{
		ret = 1;
		goto ILLEGAL_PACKET_EXIT;
	}
	if (0 != p->data_size)
	{
		ret = -1;
		goto NOT_DECODED_EXIT;
	}
	if (!espress_stream_uint16_eq(espress_stream_packet->packet_type(p), this->dst_packet_type_connect))
	{
		ret = 1;
		goto MISMATCH_PACKET_TYPE_EXIT;
	}
	espress_stream_uint16_get_u(espress_stream_packet->packet_size(p), &u);
	if (u < ESPRESS_STREAM_PACKET_CONNECT_MIN_SIZE)
	{
		ret = 1;
		goto ILLEGAL_PACKET_SIZE_EXIT;
	}
	if ((u - ESPRESS_STREAM_PACKET_CONNECT_MIN_SIZE) < payload_size_var)
	{
		payload_size_var = u - ESPRESS_STREAM_PACKET_CONNECT_MIN_SIZE;
	}
	espress_stream_uint32_assign(this->dst_connection_id, espress_stream_packet->connection_id(p));
	if (this->is_server)
	{
		if (espress_stream_uint32_eq(this->dst_connection_id, espress_stream_uint32_0) ||
			espress_stream_uint32_eq(this->dst_connection_id, espress_stream_uint32_max))
		{
			ret = 1;
			goto ILLEGAL_CONNECTION_ID_EXIT;
		}
	}
	else
	{
		if (!espress_stream_uint32_eq(this->dst_connection_id, this->connection_id))
		{
			ret = 1;
			goto MISMATCH_CONNECTION_ID_EXIT;
		}
	}
	espress_stream_uint32_assign(connection_param, espress_stream_packet->connection_param(p));
#	if 4 <= U_T_WIDTH
	{
		espress_stream_uint32_get_u(espress_stream_packet->initial_window_size(p), initial_window_size);
	}
#	else
	{
		struct espress_stream_uint32 v[1];
		espress_stream_uint32_set_u(v, U_T_MAX);
		if (espress_stream_uint32_lt(espress_stream_packet->initial_window_size(p), v))
		{
			espress_stream_uint32_get_u(espress_stream_packet->initial_window_size(p), initial_window_size);
		}
		else
		{
			*initial_window_size = U_T_MAX;
		}
	}
#	endif
	memcpy(this->dst_public_key, espress_stream_packet->key_change(p), sizeof(this->dst_public_key));
	memcpy(this->dst_random, espress_stream_packet->random(p), sizeof(this->dst_random));
	if (0 < payload_size_var)
	{
		memcpy(payload, espress_stream_packet->connect_payload(p), payload_size_var);
	}

	if (NULL != payload_size)
	{
		*payload_size = payload_size_var;
	}

	if (!this->is_server)
	{
		espress_stream_uint32_and(connection_param, reject_param_val);
		if (espress_stream_uint32_eq(connection_param, espress_stream_uint32_0))
		{
			espress_stream_key->finalize(this->recv_key);
			espress_stream_key->finalize(this->send_key);
			espress_stream_key_ex->get_shared_key(this->shared_key, this->src_private_key, this->dst_public_key);
			espress_stream_key->initialize(this->send_key, this->shared_key, sizeof(this->shared_key), this->src_random, sizeof(this->src_random));
			espress_stream_key->initialize(this->recv_key, this->shared_key, sizeof(this->shared_key), this->dst_random, sizeof(this->dst_random));
		}
	}

	memcpy(this->dst_address, p->address, sizeof(*(this->dst_address)));
	memcpy(address, p->address, sizeof(*address));

MISMATCH_CONNECTION_ID_EXIT:
ILLEGAL_CONNECTION_ID_EXIT:
ILLEGAL_PACKET_SIZE_EXIT:
MISMATCH_PACKET_TYPE_EXIT:
NOT_DECODED_EXIT:
ILLEGAL_PACKET_EXIT:
MISMATCH_PACKET_ID_EXIT:
RECV_QUEUE_EMPTY_EXIT:

	espress_stream_mutex->unlock(this->interface->mutex);

	return ret;
}

static i_t _send_disconnect(struct espress_stream_trans_packet *this,
	b_t const *payload, u_t payload_size)
{
	i_t ret = 0;
	struct espress_stream_packet *p;

	if (NULL == payload)
	{
		payload_size = 0;
	}
	if ((ESPRESS_STREAM_PACKET_MAX_SIZE - ESPRESS_STREAM_PACKET_DISCONNECT_MIN_SIZE) < payload_size)
	{
		payload_size = ESPRESS_STREAM_PACKET_MAX_SIZE - ESPRESS_STREAM_PACKET_DISCONNECT_MIN_SIZE;
	}

	espress_stream_mutex->lock(this->interface->mutex);

	if (NULL == espress_stream_trans_ring->head(this->interface->send_ring, 0))
	{
		ret = -1;
		goto SEND_QUEUE_FULL_EXIT;
	}
	p = (struct espress_stream_packet *)(espress_stream_trans_ring->head(this->interface->send_ring, 0)[0]);
	p->data_size = 0;
	memcpy(p->address, this->dst_address, sizeof(*(p->address)));
	espress_stream_uint16_assign(espress_stream_packet->packet_type(p), this->src_packet_type_disconnect);
	espress_stream_uint16_set_u(espress_stream_packet->packet_size(p), ESPRESS_STREAM_PACKET_DISCONNECT_MIN_SIZE + payload_size);
	espress_stream_uint32_assign(espress_stream_packet->connection_id(p), this->connection_id);
	espress_stream_uint32_assign(espress_stream_packet->counter_number(p), this->src_counter_number);
	if (0 < payload_size)
	{
		memcpy(espress_stream_packet->disconnect_payload(p), payload, payload_size);
	}

	p->id = (void *)this;
	espress_stream_trans_ring->en(this->interface->send_ring, 1);
	espress_stream_packet->encode(p, this->is_server, this->send_key);

	espress_stream_uint32_add(this->src_counter_number, espress_stream_uint32_1);

SEND_QUEUE_FULL_EXIT:

	espress_stream_mutex->unlock(this->interface->mutex);

	return ret;
}

static i_t _recv_disconnect(struct espress_stream_trans_packet *this,
		b_t *payload, u_t *payload_size)
{
	u_t u;
	i_t ret = 0;
	struct espress_stream_packet *p;
	u_t payload_size_var;

	if ((NULL == payload) || (NULL == payload_size))
	{
		payload_size_var = 0;
	}
	else
	{
		payload_size_var = *payload_size;
	}

	espress_stream_mutex->lock(this->interface->mutex);

	if (NULL == espress_stream_trans_ring->head(this->interface->recv_ring, 0))
	{
		ret = -1;
		goto RECV_QUEUE_EMPTY_EXIT;
	}
	p = (struct espress_stream_packet *)(espress_stream_trans_ring->head(this->interface->recv_ring, 0)[0]);
	if (this != (struct espress_stream_trans_packet *)(p->id))
	{
		ret = -1;
		goto MISMATCH_PACKET_ID_EXIT;
	}
	if (ESPRESS_STREAM_TRANS_RING_INVALID_DATA_SIZE == p->data_size)
	{
		ret = 1;
		goto ILLEGAL_PACKET_EXIT;
	}
	if (0 != p->data_size)
	{
		ret = -1;
		goto NOT_DECODED_EXIT;
	}

	if (!espress_stream_uint16_eq(espress_stream_packet->packet_type(p), this->dst_packet_type_disconnect))
	{
		ret = 1;
		goto MISMATCH_PACKET_TYPE_EXIT;
	}
	espress_stream_uint16_get_u(espress_stream_packet->packet_size(p), &u);
	if (u < ESPRESS_STREAM_PACKET_DISCONNECT_MIN_SIZE)
	{
		ret = 1;
		goto ILLEGAL_PACKET_SIZE_EXIT;
	}
	if ((u - ESPRESS_STREAM_PACKET_DISCONNECT_MIN_SIZE) < payload_size_var)
	{
		payload_size_var = u - ESPRESS_STREAM_PACKET_DISCONNECT_MIN_SIZE;
	}
	espress_stream_uint32_assign(this->dst_connection_id, espress_stream_packet->connection_id(p));
	if (!espress_stream_uint32_eq(this->dst_connection_id, this->connection_id))
	{
		ret = 1;
		goto MISMATCH_CONNECTION_ID_EXIT;
	}
	espress_stream_uint32_assign(this->dst_counter_number, espress_stream_packet->counter_number(p));
	if (0 < payload_size_var)
	{
		memcpy(payload, espress_stream_packet->disconnect_payload(p), payload_size_var);
	}

	if (NULL != payload_size)
	{
		*payload_size = payload_size_var;
	}

	memcpy(this->dst_address, p->address, sizeof(*(this->dst_address)));

MISMATCH_CONNECTION_ID_EXIT:
ILLEGAL_PACKET_SIZE_EXIT:
MISMATCH_PACKET_TYPE_EXIT:
NOT_DECODED_EXIT:
ILLEGAL_PACKET_EXIT:
MISMATCH_PACKET_ID_EXIT:
RECV_QUEUE_EMPTY_EXIT:

	espress_stream_mutex->unlock(this->interface->mutex);

	return ret;
}

static i_t _send_envelope(struct espress_stream_trans_packet *this,
	struct espress_stream_uint32 const *receive_window_address,
	struct espress_stream_uint32 const *receive_window_size,
	struct espress_stream_uint32 const *send_data_address,
	b_t const *payload, u_t payload_size,
	b_t const *add_payload, u_t add_payload_size)
{
	i_t ret = 0;
	struct espress_stream_packet *p;
	u_t total_payload_size;

	if (NULL == payload)
	{
		payload_size = 0;
	}
	if (NULL == add_payload)
	{
		add_payload_size = 0;
	}
	total_payload_size = payload_size + add_payload_size;
	if (_espress_stream_packet_envelope_payload_max_size(this) < total_payload_size)
	{
		total_payload_size = _espress_stream_packet_envelope_payload_max_size(this);
	}

	espress_stream_mutex->lock(this->interface->mutex);

	if (NULL == espress_stream_trans_ring->head(this->interface->send_ring, 0))
	{
		ret = -1;
		goto SEND_QUEUE_FULL_EXIT;
	}
	p = (struct espress_stream_packet *)(espress_stream_trans_ring->head(this->interface->send_ring, 0)[0]);
	p->data_size = 0;
	memcpy(p->address, this->dst_address, sizeof(*(p->address)));
	espress_stream_uint16_assign(espress_stream_packet->packet_type(p), this->src_packet_type_envelope);
	espress_stream_uint16_set_u(espress_stream_packet->packet_size(p), ESPRESS_STREAM_PACKET_ENVELOPE_MIN_SIZE + total_payload_size);
	espress_stream_uint32_assign(espress_stream_packet->connection_id(p), this->connection_id);
	espress_stream_uint32_assign(espress_stream_packet->counter_number(p), this->src_counter_number);
	espress_stream_uint32_assign(espress_stream_packet->receive_counter_number(p), this->src_receive_counter_number);
	espress_stream_uint32_assign(espress_stream_packet->receive_window_address(p), receive_window_address);
	espress_stream_uint32_assign(espress_stream_packet->receive_window_size(p), receive_window_size);
	espress_stream_uint32_assign(espress_stream_packet->send_data_address(p), send_data_address);
	if (0 < total_payload_size)
	{
		if (payload_size < total_payload_size)
		{
			memcpy(espress_stream_packet->envelope_payload(p), payload, payload_size);
			memcpy(espress_stream_packet->envelope_payload(p) + payload_size, add_payload, total_payload_size - payload_size);
		}
		else
		{
			memcpy(espress_stream_packet->envelope_payload(p), payload, total_payload_size);
		}
	}

	p->id = (void *)this;
	espress_stream_trans_ring->en(this->interface->send_ring, 1);
	espress_stream_packet->encode(p, this->is_server, this->send_key);

	_start_rto_measurement(this->rto_measurement, this->dst_receive_counter_number, this->src_counter_number);

	espress_stream_uint32_add(this->src_counter_number, espress_stream_uint32_1);

SEND_QUEUE_FULL_EXIT:

	espress_stream_mutex->unlock(this->interface->mutex);

	return ret;
}

static i_t _recv_envelope(struct espress_stream_trans_packet *this,
	struct espress_stream_uint32 *receive_window_address,
	struct espress_stream_uint32 *receive_window_size,
	i_t *is_ack_true,
	struct espress_stream_uint32 *send_data_address,
	struct espress_stream_uint32 const *payload_address,
	b_t *payload, u_t *payload_size,
	b_t *add_payload, u_t *add_payload_size)
{
	i_t ret = 0;
	i_t const is_first = (NULL != receive_window_address) && (NULL != receive_window_size) && (NULL != is_ack_true);
	struct espress_stream_packet *p;
	struct espress_stream_uint32 u[1];
	u_t packet_payload_size;
	struct espress_stream_uint32 receive_counter_number[1];
	struct espress_stream_uint32 packet_send_data_address[1];
	struct espress_stream_uint32 packet_send_data_tail_address[1];
	u_t payload_size_var;
	u_t add_payload_size_var;
	u_t total_payload_size_var;
	struct espress_stream_uint32 total_payload_size[1];
	struct espress_stream_uint32 payload_tail_address[1];
	u_t offset;

	if ((NULL == payload) || (NULL == payload_size))
	{
		payload_size_var = 0;
	}
	else
	{
		payload_size_var = *payload_size;
	}
	if ((NULL == add_payload) || (NULL == add_payload_size))
	{
		add_payload_size_var = 0;
	}
	else
	{
		add_payload_size_var = *add_payload_size;
	}
	espress_stream_uint32_set_u(total_payload_size, payload_size_var);
	espress_stream_uint32_set_u(u, add_payload_size_var);
	espress_stream_uint32_add(total_payload_size, u);
	espress_stream_uint32_assign(payload_tail_address, payload_address);
	espress_stream_uint32_add(payload_tail_address, total_payload_size);

	espress_stream_mutex->lock(this->interface->mutex);

	if (NULL == espress_stream_trans_ring->head(this->interface->recv_ring, 0))
	{
		ret = -1;
		goto RECV_QUEUE_EMPTY_EXIT;
	}
	p = (struct espress_stream_packet *)(espress_stream_trans_ring->head(this->interface->recv_ring, 0)[0]);
	if (this != (struct espress_stream_trans_packet *)(p->id))
	{
		ret = -1;
		goto MISMATCH_PACKET_ID_EXIT;
	}
	if (ESPRESS_STREAM_TRANS_RING_INVALID_DATA_SIZE == p->data_size)
	{
		ret = 1;
		goto ILLEGAL_PACKET_EXIT;
	}
	if (0 != p->data_size)
	{
		ret = -1;
		goto NOT_DECODED_EXIT;
	}

	if (!espress_stream_uint16_eq(espress_stream_packet->packet_type(p), this->dst_packet_type_envelope))
	{
		ret = 1;
		goto MISMATCH_PACKET_TYPE_EXIT;
	}
	espress_stream_uint16_get_u(espress_stream_packet->packet_size(p), &packet_payload_size);
	if (packet_payload_size < ESPRESS_STREAM_PACKET_ENVELOPE_MIN_SIZE)
	{
		ret = 1;
		goto ILLEGAL_PACKET_SIZE_EXIT;
	}
	packet_payload_size -= ESPRESS_STREAM_PACKET_ENVELOPE_MIN_SIZE;
	espress_stream_uint32_assign(this->dst_connection_id, espress_stream_packet->connection_id(p));
	if (!espress_stream_uint32_eq(this->dst_connection_id, this->connection_id))
	{
		ret = 1;
		goto MISMATCH_CONNECTION_ID_EXIT;
	}
	if (is_first)
	{
		espress_stream_uint32_assign(this->dst_counter_number, espress_stream_packet->counter_number(p));
		espress_stream_uint32_assign(receive_counter_number, espress_stream_packet->receive_counter_number(p));
		espress_stream_uint32_assign(receive_window_address, espress_stream_packet->receive_window_address(p));
		espress_stream_uint32_assign(receive_window_size, espress_stream_packet->receive_window_size(p));
	}
	espress_stream_uint32_assign(packet_send_data_address, espress_stream_packet->send_data_address(p));
	espress_stream_uint32_assign(packet_send_data_tail_address, packet_send_data_address);
	espress_stream_uint32_set_u(u, packet_payload_size);
	espress_stream_uint32_add(packet_send_data_tail_address, u);
	if (_uint32_le(payload_address, packet_send_data_tail_address) &&
		_uint32_lt(packet_send_data_address, payload_tail_address))
	{
		espress_stream_uint32_assign(u, packet_send_data_tail_address);
		espress_stream_uint32_sub(u, payload_address);
		if (espress_stream_uint32_lt(u, total_payload_size))
		{
			espress_stream_uint32_assign(total_payload_size, u);
		}
		espress_stream_uint32_get_u(total_payload_size, &total_payload_size_var);
		if (_uint32_lt(payload_address, packet_send_data_address))
		{
			espress_stream_uint32_assign(u, packet_send_data_address);
			espress_stream_uint32_sub(u, payload_address);
			espress_stream_uint32_get_u(u, &offset);
			if (offset < payload_size_var)
			{
				if (payload_size_var < total_payload_size_var)
				{
					add_payload_size_var = total_payload_size_var - payload_size_var;
					memcpy(payload + offset,
						espress_stream_packet->envelope_payload(p),
						payload_size_var - offset);
					memcpy(add_payload,
						espress_stream_packet->envelope_payload(p) + (payload_size_var - offset),
						add_payload_size_var);
				}
				else
				{
					payload_size_var = total_payload_size_var;
					add_payload_size_var = 0;
					memcpy(payload + offset,
						espress_stream_packet->envelope_payload(p),
						payload_size_var - offset);
				}
			}
			else
			{
				add_payload_size_var = total_payload_size_var - payload_size_var;
				memcpy(add_payload + (offset - payload_size_var),
					espress_stream_packet->envelope_payload(p),
					total_payload_size_var - offset);
			}
		}
		else
		{
			espress_stream_uint32_assign(u, payload_address);
			espress_stream_uint32_sub(u, packet_send_data_address);
			espress_stream_uint32_get_u(u, &offset);
			if (payload_size_var < total_payload_size_var)
			{
				add_payload_size_var = total_payload_size_var - payload_size_var;
				memcpy(payload,
					espress_stream_packet->envelope_payload(p) + offset,
					payload_size_var);
				memcpy(add_payload,
					espress_stream_packet->envelope_payload(p) + offset + payload_size_var,
					add_payload_size_var);
			}
			else
			{
				payload_size_var = total_payload_size_var;
				add_payload_size_var = 0;
				memcpy(payload,
					espress_stream_packet->envelope_payload(p) + offset,
					payload_size_var);
			}
			offset = 0;
		}
	}
	else
	{
		payload_size_var = 0;
		add_payload_size_var = 0;
		offset = 0;
	}

	if (NULL != payload_size)
	{
		*payload_size = payload_size_var;
	}
	if (NULL != add_payload_size)
	{
		*add_payload_size = add_payload_size_var;
	}
	espress_stream_uint32_assign(send_data_address, payload_address);
	if (0 < offset)
	{
		espress_stream_uint32_set_u(u, offset);
		espress_stream_uint32_add(send_data_address, u);
	}

	if (is_first && (0 < (payload_size_var + add_payload_size_var)))
	{
		espress_stream_uint32_assign(this->src_receive_counter_number, this->dst_counter_number);

		*is_ack_true = !espress_stream_uint32_eq(this->dst_receive_counter_number, receive_counter_number);

		if (*is_ack_true)
		{
			_stop_rto_measurement(this->rto_measurement, receive_counter_number);
		}
		_check_duplicate_ack_count(this->duplicate_ack_count,
			this->dst_receive_counter_number,
			this->dst_receive_window_address,
			receive_counter_number,
			receive_window_address);

		espress_stream_uint32_assign(this->dst_receive_counter_number, receive_counter_number);
		espress_stream_uint32_assign(this->dst_receive_window_address, receive_window_address);
	}

	memcpy(this->dst_address, p->address, sizeof(*(this->dst_address)));

MISMATCH_CONNECTION_ID_EXIT:
ILLEGAL_PACKET_SIZE_EXIT:
MISMATCH_PACKET_TYPE_EXIT:
NOT_DECODED_EXIT:
ILLEGAL_PACKET_EXIT:
MISMATCH_PACKET_ID_EXIT:
RECV_QUEUE_EMPTY_EXIT:

	espress_stream_mutex->unlock(this->interface->mutex);

	return ret;
}

static i_t _send_ack(struct espress_stream_trans_packet *this,
	struct espress_stream_uint32 const *receive_window_address,
	struct espress_stream_uint32 const *receive_window_size)
{
	i_t ret = 0;
	struct espress_stream_packet *p;

	espress_stream_mutex->lock(this->interface->mutex);

	if (NULL == espress_stream_trans_ring->head(this->interface->send_ring, 0))
	{
		ret = -1;
		goto SEND_QUEUE_FULL_EXIT;
	}
	p = (struct espress_stream_packet *)(espress_stream_trans_ring->head(this->interface->send_ring, 0)[0]);
	p->data_size = 0;
	memcpy(p->address, this->dst_address, sizeof(*(p->address)));
	espress_stream_uint16_assign(espress_stream_packet->packet_type(p), this->src_packet_type_envelope);
	espress_stream_uint16_set_u(espress_stream_packet->packet_size(p), ESPRESS_STREAM_PACKET_ENVELOPE_ACK_SIZE);
	espress_stream_uint32_assign(espress_stream_packet->connection_id(p), this->connection_id);
	espress_stream_uint32_assign(espress_stream_packet->counter_number(p), this->src_counter_number);
	espress_stream_uint32_assign(espress_stream_packet->receive_counter_number(p), this->src_receive_counter_number);
	espress_stream_uint32_assign(espress_stream_packet->receive_window_address(p), receive_window_address);
	espress_stream_uint32_assign(espress_stream_packet->receive_window_size(p), receive_window_size);

	p->id = (void *)this;
	espress_stream_trans_ring->en(this->interface->send_ring, 1);
	espress_stream_packet->encode(p, this->is_server, this->send_key);

	espress_stream_uint32_add(this->src_counter_number, espress_stream_uint32_1);

SEND_QUEUE_FULL_EXIT:

	espress_stream_mutex->unlock(this->interface->mutex);

	return ret;
}

static i_t _recv_ack(struct espress_stream_trans_packet *this,
	struct espress_stream_uint32 *receive_window_address,
	struct espress_stream_uint32 *receive_window_size,
	i_t *is_ack_true)
{
	u_t u;
	i_t ret = 0;
	struct espress_stream_packet *p;
	struct espress_stream_uint32 receive_counter_number[1];

	espress_stream_mutex->lock(this->interface->mutex);

	if (NULL == espress_stream_trans_ring->head(this->interface->recv_ring, 0))
	{
		ret = -1;
		goto RECV_QUEUE_EMPTY_EXIT;
	}
	p = (struct espress_stream_packet *)(espress_stream_trans_ring->head(this->interface->recv_ring, 0)[0]);
	if (this != (struct espress_stream_trans_packet *)(p->id))
	{
		ret = -1;
		goto MISMATCH_PACKET_ID_EXIT;
	}
	if (ESPRESS_STREAM_TRANS_RING_INVALID_DATA_SIZE == p->data_size)
	{
		ret = 1;
		goto ILLEGAL_PACKET_EXIT;
	}
	if (0 != p->data_size)
	{
		ret = -1;
		goto NOT_DECODED_EXIT;
	}

	if (!espress_stream_uint16_eq(espress_stream_packet->packet_type(p), this->dst_packet_type_envelope))
	{
		ret = 1;
		goto MISMATCH_PACKET_TYPE_EXIT;
	}
	espress_stream_uint16_get_u(espress_stream_packet->packet_size(p), &u);
	if (u != ESPRESS_STREAM_PACKET_ENVELOPE_ACK_SIZE)
	{
		ret = 1;
		goto ILLEGAL_PACKET_SIZE_EXIT;
	}
	espress_stream_uint32_assign(this->dst_connection_id, espress_stream_packet->connection_id(p));
	if (!espress_stream_uint32_eq(this->dst_connection_id, this->connection_id))
	{
		ret = 1;
		goto MISMATCH_CONNECTION_ID_EXIT;
	}
	espress_stream_uint32_assign(this->dst_counter_number, espress_stream_packet->counter_number(p));
	espress_stream_uint32_assign(receive_counter_number, espress_stream_packet->receive_counter_number(p));
	espress_stream_uint32_assign(receive_window_address, espress_stream_packet->receive_window_address(p));
	espress_stream_uint32_assign(receive_window_size, espress_stream_packet->receive_window_size(p));

	*is_ack_true = !espress_stream_uint32_eq(this->dst_receive_counter_number, receive_counter_number);

	if (*is_ack_true)
	{
		_stop_rto_measurement(this->rto_measurement, receive_counter_number);
	}
	_check_duplicate_ack_count(this->duplicate_ack_count,
		this->dst_receive_counter_number,
		this->dst_receive_window_address,
		receive_counter_number,
		receive_window_address);

	espress_stream_uint32_assign(this->dst_receive_counter_number, receive_counter_number);
	espress_stream_uint32_assign(this->dst_receive_window_address, receive_window_address);

	memcpy(this->dst_address, p->address, sizeof(*(this->dst_address)));

MISMATCH_CONNECTION_ID_EXIT:
ILLEGAL_PACKET_SIZE_EXIT:
MISMATCH_PACKET_TYPE_EXIT:
NOT_DECODED_EXIT:
ILLEGAL_PACKET_EXIT:
MISMATCH_PACKET_ID_EXIT:
RECV_QUEUE_EMPTY_EXIT:

	espress_stream_mutex->unlock(this->interface->mutex);

	return ret;
}

static i_t _purge(struct espress_stream_trans_packet *this)
{
	i_t ret = 0;
	struct espress_stream_packet *p;

	espress_stream_mutex->lock(this->interface->mutex);

	if (NULL == espress_stream_trans_ring->head(this->interface->recv_ring, 0))
	{
		ret = -1;
		goto RECV_QUEUE_EMPTY_EXIT;
	}
	p = (struct espress_stream_packet *)(espress_stream_trans_ring->head(this->interface->recv_ring, 0)[0]);
	if ((NULL != p->id) && (this != (struct espress_stream_trans_packet *)(p->id)))
	{
		espress_stream_event->set(((struct espress_stream_trans_packet *)(p->id))->outer_event, ESPRESS_STREAM_EVENT_TRANS_PACKET_RECV);
		ret = -1;
		goto MISMATCH_PACKET_ID_EXIT;
	}
	if (0 != p->data_size)
	{
		ret = -1;
		goto NOT_DECODED_EXIT;
	}

	p->id = NULL;
	p->data_size = espress_stream_dgram->mtu(this->interface->dgram);
	espress_stream_trans_ring->en(this->interface->recv_ring, 1);
	espress_stream_event->set(this->interface->event, ESPRESS_STREAM_EVENT_PACKET_DECODE_RESULT);

NOT_DECODED_EXIT:
MISMATCH_PACKET_ID_EXIT:
RECV_QUEUE_EMPTY_EXIT:

	espress_stream_mutex->unlock(this->interface->mutex);

	return ret;
}

static void _src_address(struct espress_stream_trans_packet *this, struct espress_stream_dgram_address *address)
{
	memcpy(address, this->src_address, sizeof(*address));
}

static void _dst_address(struct espress_stream_trans_packet *this, struct espress_stream_dgram_address *address)
{
	memcpy(address, this->dst_address, sizeof(*address));
}

static void _connection_id(struct espress_stream_trans_packet *this, struct espress_stream_uint32 *connection_id)
{
	espress_stream_mutex->lock(this->interface->mutex);
	espress_stream_uint32_assign(connection_id, this->connection_id);
	espress_stream_mutex->unlock(this->interface->mutex);
}

static void _shared_key(struct espress_stream_trans_packet *this, b_t *shared_key)
{
	espress_stream_mutex->lock(this->interface->mutex);
	memcpy(shared_key, this->shared_key, sizeof(this->shared_key));
	espress_stream_mutex->unlock(this->interface->mutex);
}

static i_t _rtt(struct espress_stream_trans_packet *this, struct espress_stream_uint32 *rtt)
{
	i_t ret = 0;

	espress_stream_mutex->lock(this->interface->mutex);

	if (!this->rto_measurement->is_rtt_valid)
	{
		ret = 1;
		goto INVALID_RTT_EXIT;
	}

	espress_stream_uint32_assign(rtt, this->rto_measurement->rtt);

INVALID_RTT_EXIT:

	espress_stream_mutex->unlock(this->interface->mutex);

	return ret;
}

static void _rto(struct espress_stream_trans_packet *this, struct espress_stream_uint32 *rto)
{
	espress_stream_mutex->lock(this->interface->mutex);
	espress_stream_uint32_assign(rto, this->rto_measurement->rto);
	espress_stream_mutex->unlock(this->interface->mutex);
}

static void _tell_timeout(struct espress_stream_trans_packet *this)
{
	espress_stream_mutex->lock(this->interface->mutex);
	_tell_timeout_rto_measurement(this->rto_measurement);
	espress_stream_mutex->unlock(this->interface->mutex);
}

static u_t _dac(struct espress_stream_trans_packet *this)
{
	u_t ret;
	espress_stream_mutex->lock(this->interface->mutex);
	ret = this->duplicate_ack_count->count;
	espress_stream_mutex->unlock(this->interface->mutex);
	return ret;
}

#define _SEND_PACKET_YIELD_FUNC(count) static void _send_packet_yield_ ## count(struct espress_stream_trans_packet_interface *this, struct espress_stream_uint32 const *timeout) \
{ \
	espress_stream_packet->yield(this->send_packet + count, timeout); \
}
#define ESPRESS_STREAM_MACRO_REPEAT (ESPRESS_STREAM_SEND_QUEUE_SIZE)
#include "espress_stream/macro_repeat.h"
ESPRESS_STREAM_MACRO_REPEAT(_SEND_PACKET_YIELD_FUNC)
#undef ESPRESS_STREAM_MACRO_REPEAT

#define _RECV_PACKET_YIELD_FUNC(count) static void _recv_packet_yield_ ## count(struct espress_stream_trans_packet_interface *this, struct espress_stream_uint32 const *timeout) \
{ \
	espress_stream_packet->yield(this->recv_packet + count, timeout); \
}
#define ESPRESS_STREAM_MACRO_REPEAT (ESPRESS_STREAM_RECV_QUEUE_SIZE)
#include "espress_stream/macro_repeat.h"
ESPRESS_STREAM_MACRO_REPEAT(_RECV_PACKET_YIELD_FUNC)
#undef ESPRESS_STREAM_MACRO_REPEAT

static void _dgram_send_yield(struct espress_stream_trans_packet_interface *this, struct espress_stream_uint32 const *timeout)
{
	espress_stream_dgram->send_yield(this->dgram, timeout);
}

static void _dgram_recv_yield(struct espress_stream_trans_packet_interface *this, struct espress_stream_uint32 const *timeout)
{
	espress_stream_dgram->recv_yield(this->dgram, timeout);
}

static void _this_yield(struct espress_stream_trans_packet_interface *this, struct espress_stream_uint32 const *timeout)
{
	u_t u;
	u_t v;
	struct espress_stream_packet *p;
	struct espress_stream_event_flags flags[1];
	struct espress_stream_uint16 *packet_type;
	struct espress_stream_uint32 connection_id[1];
	struct espress_stream_trans_packet *trans_packet;
	i_t is_server;
	struct espress_stream_key *key;

	espress_stream_event->wait(this->event, flags, timeout);

	espress_stream_mutex->lock(this->mutex);

	if (espress_stream_event_flags->is_set(flags, ESPRESS_STREAM_EVENT_PACKET_ENCODE_RESULT))
	{
		espress_stream_trans_ring->send_request(this->send_ring);
	}
	if (espress_stream_event_flags->is_set(flags, ESPRESS_STREAM_EVENT_PACKET_DECODE_RESULT))
	{
		while (NULL != espress_stream_trans_ring->head(this->recv_ring, 0))
		{
			p = (struct espress_stream_packet *)(espress_stream_trans_ring->head(this->recv_ring, 0)[0]);
			if ((0 == p->data_size) || (ESPRESS_STREAM_TRANS_RING_INVALID_DATA_SIZE == p->data_size))
			{
				if (NULL == p->id)
				{
					p->data_size = espress_stream_dgram->mtu(this->dgram);
					espress_stream_trans_ring->en(this->recv_ring, 1);
					continue;
				}
				else
				{
					trans_packet = (struct espress_stream_trans_packet *)(p->id);
					espress_stream_event->set(trans_packet->outer_event, ESPRESS_STREAM_EVENT_TRANS_PACKET_RECV);
				}
			}
			break;
		}
		espress_stream_trans_ring->recv_request(this->recv_ring);
	}
	if (espress_stream_event_flags->is_set(flags, ESPRESS_STREAM_EVENT_DGRAM_SEND_RESULT))
	{
		espress_stream_trans_ring->send_result(this->send_ring, &u);
		if (!espress_stream_trans_ring->is_empty(this->send_ring))
		{
			espress_stream_trans_ring->send_request(this->send_ring);
		}
		if (0 == u)
		{
			espress_stream_event->set(this->event, ESPRESS_STREAM_EVENT_PACKET_ENCODE_RESULT);
		}
		else
		{
			for (trans_packet = _get_first_trans_packet(this) ;
				NULL != trans_packet ;
				trans_packet = _get_next_trans_packet(this, trans_packet))
			{
				espress_stream_event->set(trans_packet->outer_event, ESPRESS_STREAM_EVENT_TRANS_PACKET_SEND);
			}
		}
	}
	if (espress_stream_event_flags->is_set(flags, ESPRESS_STREAM_EVENT_DGRAM_RECV_RESULT))
	{
		espress_stream_trans_ring->recv_result(this->recv_ring, &u);
		if (!espress_stream_trans_ring->is_empty(this->recv_ring))
		{
			espress_stream_trans_ring->recv_request(this->recv_ring);
		}
		if (0 == u)
		{
			espress_stream_event->set(this->event, ESPRESS_STREAM_EVENT_PACKET_DECODE_RESULT);
		}
		for (v = espress_stream_trans_ring->num_invalid(this->recv_ring) - u ;
			NULL != espress_stream_trans_ring->head(this->recv_ring, v) ;
			v++)
		{
			p = (struct espress_stream_packet *)(espress_stream_trans_ring->head(this->recv_ring, v)[0]);
			is_server = 0;
			trans_packet = NULL;
			key = NULL;
			if ((ESPRESS_STREAM_TRANS_RING_INVALID_DATA_SIZE != p->data_size) &&
				((ESPRESS_STREAM_PACKET_MIN_SIZE + ESPRESS_STREAM_MAC_MAC_SIZE) <= p->data_size))
			{
				packet_type = espress_stream_packet->packet_type(p);
				espress_stream_uint32_assign(connection_id, espress_stream_packet->connection_id(p));
				espress_stream_uint32_ntoh(connection_id);
				if (espress_stream_uint32_eq(connection_id, espress_stream_uint32_0) ||
					espress_stream_uint32_eq(connection_id, espress_stream_uint32_max))
				{
					is_server = 0;
					trans_packet = NULL;
				}
				else if (espress_stream_uint16_eq(packet_type, espress_stream_packet_type_client_envelope))
				{
					is_server = 1;
					trans_packet = _find_trans_packet(this, is_server, connection_id);
					if (NULL != trans_packet)
					{
						key = trans_packet->recv_key;
					}
				}
				else if (espress_stream_uint16_eq(packet_type, espress_stream_packet_type_server_envelope))
				{
					is_server = 0;
					trans_packet = _find_trans_packet(this, is_server, connection_id);
					if (NULL != trans_packet)
					{
						key = trans_packet->recv_key;
					}
				}
				else if (espress_stream_uint16_eq(packet_type, espress_stream_packet_type_client_disconnect))
				{
					is_server = 1;
					trans_packet = _find_trans_packet(this, is_server, connection_id);
					if (NULL != trans_packet)
					{
						key = trans_packet->recv_key;
					}
				}
				else if (espress_stream_uint16_eq(packet_type, espress_stream_packet_type_server_disconnect))
				{
					is_server = 0;
					trans_packet = _find_trans_packet(this, is_server, connection_id);
					if (NULL != trans_packet)
					{
						key = trans_packet->recv_key;
					}
				}
				else if (espress_stream_uint16_eq(packet_type, espress_stream_packet_type_client_connect))
				{
					is_server = 1;
					trans_packet = _find_trans_packet(this, is_server, espress_stream_uint32_0);
					if (NULL != trans_packet)
					{
						espress_stream_uint32_assign(trans_packet->connection_id, espress_stream_uint32_max);
					}
				}
				else if (espress_stream_uint16_eq(packet_type, espress_stream_packet_type_server_connect))
				{
					is_server = 0;
					trans_packet = _find_trans_packet(this, is_server, connection_id);
				}
			}
			p->id = (void *)trans_packet;
			if (NULL == p->id)
			{
				p->data_size = ESPRESS_STREAM_TRANS_RING_INVALID_DATA_SIZE;
			}
			espress_stream_packet->decode(p, !is_server, key);
		}
	}
	if (espress_stream_event_flags->is_set(flags, ESPRESS_STREAM_EVENT_NONE))
	{
		/* nop */
	}

	espress_stream_mutex->unlock(this->mutex);
}

#define _SEND_PACKET_YIELD_BREAK_FUNC(count) static void _send_packet_yield_break_ ## count(struct espress_stream_trans_packet_interface *this) \
{ \
	espress_stream_packet->yield_break(this->send_packet + count); \
}
#define ESPRESS_STREAM_MACRO_REPEAT (ESPRESS_STREAM_SEND_QUEUE_SIZE)
#include "espress_stream/macro_repeat.h"
ESPRESS_STREAM_MACRO_REPEAT(_SEND_PACKET_YIELD_BREAK_FUNC)
#undef ESPRESS_STREAM_MACRO_REPEAT

#define _RECV_PACKET_YIELD_BREAK_FUNC(count) static void _recv_packet_yield_break_ ## count(struct espress_stream_trans_packet_interface *this) \
{ \
	espress_stream_packet->yield_break(this->recv_packet + count); \
}
#define ESPRESS_STREAM_MACRO_REPEAT (ESPRESS_STREAM_RECV_QUEUE_SIZE)
#include "espress_stream/macro_repeat.h"
ESPRESS_STREAM_MACRO_REPEAT(_RECV_PACKET_YIELD_BREAK_FUNC)
#undef ESPRESS_STREAM_MACRO_REPEAT

static void _dgram_send_yield_break(struct espress_stream_trans_packet_interface *this)
{
	espress_stream_dgram->send_yield_break(this->dgram);
}

static void _dgram_recv_yield_break(struct espress_stream_trans_packet_interface *this)
{
	espress_stream_dgram->recv_yield_break(this->dgram);
}

static void _this_yield_break(struct espress_stream_trans_packet_interface *this)
{
	espress_stream_event->set(this->event, ESPRESS_STREAM_EVENT_NONE);
}
