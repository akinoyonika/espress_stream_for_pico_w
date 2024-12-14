#include "espress_stream/config.h"
#include "espress_stream/depend.h"
#include "espress_stream/uint.h"
#include "espress_stream/tick.h"
#include "espress_stream/mutex.h"
#include "espress_stream/cond.h"
#include "espress_stream/event.h"
#include "espress_stream/timer.h"
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
#include "espress_stream/ring_buffer.h"
#include "espress_stream/session.h"

struct espress_stream_uint32 const espress_stream_protocol_version[1] = {ESPRESS_STREAM_UINT32(0x00, 0x00, ((ESPRESS_STREAM_PROTOCOL_VERSION >> 8) & 0xff), ESPRESS_STREAM_PROTOCOL_VERSION & 0xff)};
struct espress_stream_uint32 const espress_stream_send_session_timeout_time[1] = {ESPRESS_STREAM_SESSION_TIMEOUT_TIME};
struct espress_stream_uint32 const espress_stream_recv_session_timeout_time[1] = {ESPRESS_STREAM_SESSION_TIMEOUT_TIME};
struct espress_stream_uint32 const espress_stream_beacon_interval_time[1] = {ESPRESS_STREAM_BEACON_INTERVAL_TIME};

static i_t _initialize(struct espress_stream_session *this,
	struct espress_stream_trans_packet_interface *interface,
	struct espress_stream_event *event,
	i_t is_server,
	struct espress_stream_ring_buffer *send_buffer,
	struct espress_stream_ring_buffer *recv_buffer,
	i_t (*send_connect_payload)(struct espress_stream_session *this, i_t (*callback)(struct espress_stream_session *this, b_t const *payload, u_t payload_size, i_t is_rejected), void *param), void *send_connect_payload_param,
	i_t (*recv_connect_payload)(struct espress_stream_session *this, i_t (*callback)(struct espress_stream_session *this, b_t *payload, u_t *payload_size), void *param), void *recv_connect_payload_param,
	i_t (*send_disconnect_payload)(struct espress_stream_session *this, i_t (*callback)(struct espress_stream_session *this, b_t const *payload, u_t payload_size), void *param), void *send_disconnect_payload_param,
	i_t (*recv_disconnect_payload)(struct espress_stream_session *this, i_t (*callback)(struct espress_stream_session *this, b_t *payload, u_t *payload_size), void *param), void *recv_disconnect_payload_param);
static void _finalize(struct espress_stream_session *this);
static i_t _connect(struct espress_stream_session *this,
	struct espress_stream_dgram_address const *address);
static i_t _connect_result(struct espress_stream_session *this);
static i_t _connect_detect(struct espress_stream_session *this);
static i_t _disconnect(struct espress_stream_session *this);
static i_t _disconnect_result(struct espress_stream_session *this);
static i_t _disconnect_detect(struct espress_stream_session *this);
static i_t _send(struct espress_stream_session *this);
static i_t _flush(struct espress_stream_session *this);
static i_t _recv(struct espress_stream_session *this);
static void _src_address(struct espress_stream_session *this, struct espress_stream_dgram_address *address);
static void _dst_address(struct espress_stream_session *this, struct espress_stream_dgram_address *address);
static void _connection_id(struct espress_stream_session *this, struct espress_stream_uint32 *connection_id);
static void _shared_key(struct espress_stream_session *this, b_t *shared_key);
static i_t _rtt(struct espress_stream_session *this, struct espress_stream_uint32 *rtt);
static void _rto(struct espress_stream_session *this, struct espress_stream_uint32 *rto);
static u_t _dac(struct espress_stream_session *this);
static u_t _cwnd(struct espress_stream_session *this);
static u_t _send_timeout_count(struct espress_stream_session *this);
static u_t _fast_retransmit_count(struct espress_stream_session *this);
static void _send_timer_yield(struct espress_stream_session *this, struct espress_stream_uint32 const *timeout);
static void _send_session_timer_yield(struct espress_stream_session *this, struct espress_stream_uint32 const *timeout);
static void _recv_session_timer_yield(struct espress_stream_session *this, struct espress_stream_uint32 const *timeout);
static void _beacon_timer_yield(struct espress_stream_session *this, struct espress_stream_uint32 const *timeout);
static void _this_yield(struct espress_stream_session *this, struct espress_stream_uint32 const *timeout);
static void _send_timer_yield_break(struct espress_stream_session *this);
static void _send_session_timer_yield_break(struct espress_stream_session *this);
static void _recv_session_timer_yield_break(struct espress_stream_session *this);
static void _beacon_timer_yield_break(struct espress_stream_session *this);
static void _this_yield_break(struct espress_stream_session *this);

struct espress_stream_session_ops const espress_stream_session[] =
{{
	_initialize,
	_finalize,
	_connect,
	_connect_result,
	_connect_detect,
	_disconnect,
	_disconnect_result,
	_disconnect_detect,
	_send,
	_flush,
	_recv,
	_src_address,
	_dst_address,
	_connection_id,
	_shared_key,
	_rtt,
	_rto,
	_dac,
	_cwnd,
	_send_timeout_count,
	_fast_retransmit_count,
	{
		_send_timer_yield,
		_send_session_timer_yield,
		_recv_session_timer_yield,
		_beacon_timer_yield,
		_this_yield,
		NULL
	},
	{
		_send_timer_yield_break,
		_send_session_timer_yield_break,
		_recv_session_timer_yield_break,
		_beacon_timer_yield_break,
		_this_yield_break,
		NULL
	}
}};

enum
{
	_INIT = 0,
	_OPENED,
	_CLOSED,
	_CONNECT,
	_WAIT_CONNECT,
	_DISCONNECT,
	_ESTABLISHED
};

#define _espress_stream_packet_envelope_payload_max_size(this) \
	(espress_stream_packet_envelope_payload_max_size(espress_stream_trans_packet->mtu((this)->interface)))

static void _init_fast_retransmit(struct espress_stream_session_fast_retransmit *this)
{
	espress_stream_uint32_assign(this->start_data_address, espress_stream_uint32_0);
	espress_stream_uint32_assign(this->dac_avoid_size, espress_stream_uint32_0);
}

static void _final_fast_retransmit(struct espress_stream_session_fast_retransmit *this)
{
	(void)this;
}

static i_t _check_fast_retransmit(struct espress_stream_session_fast_retransmit *this,
	u_t dac,
	struct espress_stream_session_send_window *send_window)
{
	i_t ret = 0;
	struct espress_stream_uint32 u[1];

	if (espress_stream_uint32_eq(this->dac_avoid_size, espress_stream_uint32_0))
	{
		if (3 <= dac)
		{
			espress_stream_ring_buffer->tail(send_window->buffer, this->start_data_address, 0);
			espress_stream_uint32_assign(this->dac_avoid_size, send_window->data_address);
			espress_stream_uint32_sub(this->dac_avoid_size, this->start_data_address);
			espress_stream_uint32_assign(send_window->data_address, this->start_data_address);
			ret = 1;
		}
	}
	else
	{
		espress_stream_uint32_assign(u, send_window->data_address);
		espress_stream_uint32_sub(u, this->start_data_address);
		if (espress_stream_uint32_le(this->dac_avoid_size, u))
		{
			espress_stream_uint32_assign(this->dac_avoid_size, espress_stream_uint32_0);
		}
	}

	return ret;
}

static void _init_cong_avoid(struct espress_stream_session_cong_avoid *this, u_t window_unit, u_t clamp)
{
	this->cwnd = 10;
	this->ssthresh = U_T_MAX;
	this->cwnd_clamp = clamp;
	this->cwnd_cnt = 0;
	this->window_unit = window_unit;
	this->cwnd_size = this->cwnd * this->window_unit;
}

static void _final_cong_avoid(struct espress_stream_session_cong_avoid *this)
{
	(void)this;
}

static void _check_cong_avoid(struct espress_stream_session_cong_avoid *this,
	u_t window_unit,
/*	u_t max_packets_out,
	i_t is_cwnd_limited,*/
	u_t acked)
{
	u_t cwnd;
	u_t delta;
#	define is_in_slow_start (this->cwnd < this->ssthresh)

	if (this->window_unit != window_unit)
	{
		_init_cong_avoid(this, window_unit, this->cwnd_clamp);
	}

/*	if (!is_cwnd_limited)
	{
		if (!is_in_slow_start)
		{
			goto EXIT;
		}
		if ((max_packets_out << 1) <= this->cwnd)
		{
			goto EXIT;
		}
	}*/

	if (is_in_slow_start)
	{
		cwnd = this->cwnd + acked;
		if (this->ssthresh < cwnd)
		{
			cwnd = this->ssthresh;
		}
		acked -= (cwnd - this->cwnd);
		this->cwnd = cwnd;
		if (this->cwnd_clamp < this->cwnd)
		{
			this->cwnd = this->cwnd_clamp;
		}
		if (0 == acked)
		{
			goto EXIT;
		}
	}

	cwnd = this->cwnd;
	if (this->cwnd <= this->cwnd_cnt)
	{
		this->cwnd_cnt = 0;
		this->cwnd++;
	}

	this->cwnd_cnt += acked;
	if (cwnd <= this->cwnd_cnt)
	{
		delta = this->cwnd_cnt / cwnd;
		this->cwnd_cnt -= (delta * cwnd);
		this->cwnd +=  delta;
	}
	if (this->cwnd_clamp < this->cwnd)
	{
		this->cwnd = this->cwnd_clamp;
	}

	this->cwnd_size = this->cwnd * this->window_unit;

EXIT:

	return;
}

static void _tell_timeout_cong_avoid(struct espress_stream_session_cong_avoid *this)
{
	this->cwnd = 1;
}

static void _tell_fast_retransmit_cong_avoid(struct espress_stream_session_cong_avoid *this)
{
	this->cwnd >>= 1;
	if (this->cwnd < 2)
	{
		this->cwnd = 2;
	}
}

static i_t _initialize(struct espress_stream_session *this,
	struct espress_stream_trans_packet_interface *interface,
	struct espress_stream_event *event,
	i_t is_server,
	struct espress_stream_ring_buffer *send_buffer,
	struct espress_stream_ring_buffer *recv_buffer,
	i_t (*send_connect_payload)(struct espress_stream_session *this, i_t (*callback)(struct espress_stream_session *this, b_t const *payload, u_t payload_size, i_t is_rejected), void *param), void *send_connect_payload_param,
	i_t (*recv_connect_payload)(struct espress_stream_session *this, i_t (*callback)(struct espress_stream_session *this, b_t *payload, u_t *payload_size), void *param), void *recv_connect_payload_param,
	i_t (*send_disconnect_payload)(struct espress_stream_session *this, i_t (*callback)(struct espress_stream_session *this, b_t const *payload, u_t payload_size), void *param), void *send_disconnect_payload_param,
	i_t (*recv_disconnect_payload)(struct espress_stream_session *this, i_t (*callback)(struct espress_stream_session *this, b_t *payload, u_t *payload_size), void *param), void *recv_disconnect_payload_param)
{
	i_t ret = 0;

	this->interface = interface;
	this->outer_event = event;
	this->is_server = is_server;
	this->send_connect_payload = send_connect_payload;
	this->send_connect_payload_param = send_connect_payload_param;
	this->recv_connect_payload = recv_connect_payload;
	this->recv_connect_payload_param = recv_connect_payload_param;
	this->send_disconnect_payload = send_disconnect_payload;
	this->send_disconnect_payload_param = send_disconnect_payload_param;
	this->recv_disconnect_payload = recv_disconnect_payload;
	this->recv_disconnect_payload_param = recv_disconnect_payload_param;
	this->send_window->buffer = send_buffer;
	this->send_window->size = 0;
	espress_stream_uint32_assign(this->send_window->data_address, espress_stream_uint32_0);
	this->send_window->flush_size = 0;
	this->recv_window->buffer = recv_buffer;
#	if ESPRESS_STREAM_IS_ADD_DATA_ENABLED
		espress_stream_uint32_assign(this->recv_window->add_data_address, espress_stream_uint32_0);
		this->recv_window->add_data_size = 0;
#	endif

	this->send_state = _INIT;
	this->recv_state = _INIT;

	if (0 != espress_stream_event->initialize(this->event))
	{
		ret = 1;
		goto EVENT_INIT_FAIL_EXIT;
	}

	if (0 != espress_stream_trans_packet->open(this->trans_packet, this->interface, this->event, this->is_server))
	{
		ret = 1;
		goto TRANS_PACKET_OPEN_FAIL_EXIT;
	}

	if (0 != espress_stream_mutex->initialize(this->mutex))
	{
		ret = 1;
		goto MUTEX_INIT_FAIL_EXIT;
	}
	if (0 != espress_stream_timer->initialize(this->send_timer))
	{
		ret = 1;
		goto SEND_TIMER_INIT_FAIL_EXIT;
	}
	if (0 != espress_stream_timer->initialize(this->send_session_timer))
	{
		ret = 1;
		goto SEND_SESSION_TIMER_INIT_FAIL_EXIT;
	}
	if (0 != espress_stream_timer->initialize(this->recv_session_timer))
	{
		ret = 1;
		goto RECV_SESSION_TIMER_INIT_FAIL_EXIT;
	}
	if (0 != espress_stream_timer->initialize(this->beacon_timer))
	{
		ret = 1;
		goto BEACON_TIMER_INIT_FAIL_EXIT;
	}

	this->is_ack_ready = 0;

	this->send_timeout_count = 0;
	this->fast_retransmit_count = 0;

	_init_fast_retransmit(this->fast_retransmit);

	_init_cong_avoid(this->cong_avoid,
		_espress_stream_packet_envelope_payload_max_size(this),
		espress_stream_ring_buffer->size(this->send_window->buffer) / _espress_stream_packet_envelope_payload_max_size(this));

	if (this->is_server)
	{
		this->send_state = _OPENED;
		this->recv_state = _WAIT_CONNECT;
		espress_stream_event->set(this->event, ESPRESS_STREAM_EVENT_TRANS_PACKET_RECV);
	}
	else
	{
		this->send_state = _OPENED;
		this->recv_state = _OPENED;
	}

BEACON_TIMER_INIT_FAIL_EXIT:
	if (0 != ret) espress_stream_timer->finalize(this->recv_session_timer);
RECV_SESSION_TIMER_INIT_FAIL_EXIT:
	if (0 != ret) espress_stream_timer->finalize(this->send_session_timer);
SEND_SESSION_TIMER_INIT_FAIL_EXIT:
	if (0 != ret) espress_stream_timer->finalize(this->send_timer);
SEND_TIMER_INIT_FAIL_EXIT:
	if (0 != ret) espress_stream_mutex->finalize(this->mutex);
MUTEX_INIT_FAIL_EXIT:
	if (0 != ret) espress_stream_trans_packet->close(this->trans_packet);
TRANS_PACKET_OPEN_FAIL_EXIT:
	if (0 != ret) espress_stream_event->finalize(this->event);
EVENT_INIT_FAIL_EXIT:

	return ret;
}

static void _finalize(struct espress_stream_session *this)
{
	this->send_state = _CLOSED;
	this->recv_state = _CLOSED;
	_final_cong_avoid(this->cong_avoid);
	_final_fast_retransmit(this->fast_retransmit);
	espress_stream_timer->finalize(this->beacon_timer);
	espress_stream_timer->finalize(this->recv_session_timer);
	espress_stream_timer->finalize(this->send_session_timer);
	espress_stream_timer->finalize(this->send_timer);
	espress_stream_mutex->finalize(this->mutex);
	espress_stream_trans_packet->close(this->trans_packet);
	espress_stream_event->finalize(this->event);
	memset(this, 0, sizeof(*this));
}

static i_t _connect(struct espress_stream_session *this,
	struct espress_stream_dgram_address const *address)
{
	i_t ret = 0;

	espress_stream_mutex->lock(this->mutex);

	if ((_OPENED != this->send_state) || (_OPENED != this->recv_state))
	{
		ret = 1;
		goto MISMATCH_STATE_EXIT;
	}

	memcpy(this->dst_address, address, sizeof(*(this->dst_address)));

	this->send_state = _CONNECT;
	this->recv_state = _OPENED;

	espress_stream_event->set(this->event, ESPRESS_STREAM_EVENT_TRANS_PACKET_SEND);

MISMATCH_STATE_EXIT:

	espress_stream_mutex->unlock(this->mutex);

	return ret;
}

static i_t _connect_result(struct espress_stream_session *this)
{
	i_t ret = 0;

	espress_stream_mutex->lock(this->mutex);

	if (this->is_server)
	{
		ret = 1;
		goto MISMATCH_ROLE_EXIT;
	}

	if ((_ESTABLISHED != this->send_state) || (_ESTABLISHED != this->recv_state))
	{
		ret = 1;
		goto MISMATCH_STATE_EXIT;
	}

MISMATCH_STATE_EXIT:
MISMATCH_ROLE_EXIT:

	espress_stream_mutex->unlock(this->mutex);

	return ret;
}

static i_t _connect_detect(struct espress_stream_session *this)
{
	i_t ret = 0;

	espress_stream_mutex->lock(this->mutex);

	if (!this->is_server)
	{
		ret = 1;
		goto MISMATCH_ROLE_EXIT;
	}

	if ((_ESTABLISHED != this->send_state) || (_ESTABLISHED != this->recv_state))
	{
		ret = 1;
		goto MISMATCH_STATE_EXIT;
	}

MISMATCH_STATE_EXIT:
MISMATCH_ROLE_EXIT:

	espress_stream_mutex->unlock(this->mutex);

	return ret;
}

static i_t _disconnect(struct espress_stream_session *this)
{
	i_t ret = 0;

	espress_stream_mutex->lock(this->mutex);

	if (_ESTABLISHED != this->send_state)
	{
		ret = 1;
		goto MISMATCH_STATE_EXIT;
	}

	espress_stream_timer->cancel(this->send_timer);
	espress_stream_timer->cancel(this->send_session_timer);
	espress_stream_timer->cancel(this->beacon_timer);
	this->send_state = _DISCONNECT;

	espress_stream_event->set(this->event, ESPRESS_STREAM_EVENT_TRANS_PACKET_SEND);

MISMATCH_STATE_EXIT:

	espress_stream_mutex->unlock(this->mutex);

	return ret;
}

static i_t _disconnect_result(struct espress_stream_session *this)
{
	i_t ret = 0;

	espress_stream_mutex->lock(this->mutex);

	if (_CLOSED != this->send_state)
	{
		ret = 1;
		goto MISMATCH_STATE_EXIT;
	}

MISMATCH_STATE_EXIT:

	espress_stream_mutex->unlock(this->mutex);

	return ret;
}

static i_t _disconnect_detect(struct espress_stream_session *this)
{
	i_t ret = 0;

	espress_stream_mutex->lock(this->mutex);

	if (_CLOSED != this->recv_state)
	{
		ret = 1;
		goto MISMATCH_STATE_EXIT;
	}

MISMATCH_STATE_EXIT:

	espress_stream_mutex->unlock(this->mutex);

	return ret;
}

static i_t _send(struct espress_stream_session *this)
{
	i_t ret = 0;

	espress_stream_mutex->lock(this->mutex);

	if (_ESTABLISHED != this->send_state)
	{
		ret = 1;
		goto MISMATCH_STATE_EXIT;
	}

	espress_stream_event->set(this->event, ESPRESS_STREAM_EVENT_TRANS_PACKET_SEND);

MISMATCH_STATE_EXIT:

	espress_stream_mutex->unlock(this->mutex);

	return ret;
}

static i_t _flush(struct espress_stream_session *this)
{
	i_t ret = 0;

	espress_stream_mutex->lock(this->mutex);

	if (_ESTABLISHED != this->send_state)
	{
		ret = 1;
		goto MISMATCH_STATE_EXIT;
	}

	this->send_window->flush_size = espress_stream_ring_buffer->num_valid(this->send_window->buffer);

	espress_stream_event->set(this->event, ESPRESS_STREAM_EVENT_TRANS_PACKET_SEND);

MISMATCH_STATE_EXIT:

	espress_stream_mutex->unlock(this->mutex);

	return ret;
}

static i_t _recv(struct espress_stream_session *this)
{
	i_t ret = 0;

	espress_stream_mutex->lock(this->mutex);

	if (_ESTABLISHED != this->recv_state)
	{
		ret = 1;
		goto MISMATCH_STATE_EXIT;
	}

	this->is_ack_ready = 1;
	espress_stream_event->set(this->event, ESPRESS_STREAM_EVENT_TRANS_PACKET_SEND);

MISMATCH_STATE_EXIT:

	espress_stream_mutex->unlock(this->mutex);

	return ret;
}

static void _src_address(struct espress_stream_session *this, struct espress_stream_dgram_address *address)
{
	espress_stream_mutex->lock(this->mutex);
	espress_stream_trans_packet->src_address(this->trans_packet, address);
	espress_stream_mutex->unlock(this->mutex);
}

static void _dst_address(struct espress_stream_session *this, struct espress_stream_dgram_address *address)
{
	espress_stream_mutex->lock(this->mutex);
	espress_stream_trans_packet->dst_address(this->trans_packet, address);
	espress_stream_mutex->unlock(this->mutex);
}

static void _connection_id(struct espress_stream_session *this, struct espress_stream_uint32 *connection_id)
{
	espress_stream_mutex->lock(this->mutex);
	espress_stream_trans_packet->connection_id(this->trans_packet, connection_id);
	espress_stream_mutex->unlock(this->mutex);
}

static void _shared_key(struct espress_stream_session *this, b_t *shared_key)
{
	espress_stream_mutex->lock(this->mutex);
	espress_stream_trans_packet->shared_key(this->trans_packet, shared_key);
	espress_stream_mutex->unlock(this->mutex);
}

static i_t _rtt(struct espress_stream_session *this, struct espress_stream_uint32 *rtt)
{
	i_t ret;
	espress_stream_mutex->lock(this->mutex);
	ret = espress_stream_trans_packet->rtt(this->trans_packet, rtt);
	espress_stream_mutex->unlock(this->mutex);
	return ret;
}

static void _rto(struct espress_stream_session *this, struct espress_stream_uint32 *rto)
{
	espress_stream_mutex->lock(this->mutex);
	espress_stream_trans_packet->rto(this->trans_packet, rto);
	espress_stream_mutex->unlock(this->mutex);
}

static u_t _dac(struct espress_stream_session *this)
{
	u_t ret;
	espress_stream_mutex->lock(this->mutex);
	ret = espress_stream_trans_packet->dac(this->trans_packet);
	espress_stream_mutex->unlock(this->mutex);
	return ret;
}

static u_t _cwnd(struct espress_stream_session *this)
{
	u_t ret;
	espress_stream_mutex->lock(this->mutex);
	ret = this->cong_avoid->cwnd;
	espress_stream_mutex->unlock(this->mutex);
	return ret;
}

static u_t _send_timeout_count(struct espress_stream_session *this)
{
	u_t ret;
	espress_stream_mutex->lock(this->mutex);
	ret = this->send_timeout_count;
	espress_stream_mutex->unlock(this->mutex);
	return ret;
}

static u_t _fast_retransmit_count(struct espress_stream_session *this)
{
	u_t ret;
	espress_stream_mutex->lock(this->mutex);
	ret = this->fast_retransmit_count;
	espress_stream_mutex->unlock(this->mutex);
	return ret;
}

static void _send_timeout_callback(struct espress_stream_timer *timer, void *param)
{
	struct espress_stream_uint32 u[1];
	struct espress_stream_session *this = (struct espress_stream_session *)param;
	espress_stream_mutex->lock(this->mutex);
	espress_stream_ring_buffer->tail(this->send_window->buffer, u, 0);
	if (!espress_stream_uint32_eq(this->send_window->data_address, u))
	{
		espress_stream_uint32_assign(this->send_window->data_address, u);
		espress_stream_trans_packet->tell_timeout(this->trans_packet);
		_tell_timeout_cong_avoid(this->cong_avoid);
		espress_stream_event->set(this->event, ESPRESS_STREAM_EVENT_TRANS_PACKET_SEND);
		this->send_timeout_count++;
	}
	espress_stream_mutex->unlock(this->mutex);
}

static void _send_session_timeout_callback(struct espress_stream_timer *timer, void *param)
{
	struct espress_stream_session *this = (struct espress_stream_session *)param;
	espress_stream_mutex->lock(this->mutex);
	espress_stream_timer->cancel(this->send_timer);
	espress_stream_timer->cancel(this->beacon_timer);
	this->send_state = _CLOSED;
	if (_ESTABLISHED == this->recv_state)
	{
		espress_stream_timer->cancel(this->recv_session_timer);
		this->recv_state = _CLOSED;
	}
	espress_stream_event->set(this->outer_event, ESPRESS_STREAM_EVENT_SESSION_ERROR);
	espress_stream_mutex->unlock(this->mutex);
}

static void _recv_session_timeout_callback(struct espress_stream_timer *timer, void *param)
{
	struct espress_stream_session *this = (struct espress_stream_session *)param;
	espress_stream_mutex->lock(this->mutex);
	this->recv_state = _CLOSED;
	espress_stream_event->set(this->outer_event, ESPRESS_STREAM_EVENT_SESSION_DISCONNECT_DETECT);
	espress_stream_mutex->unlock(this->mutex);
}

static void _beacon_timeout_callback(struct espress_stream_timer *timer, void *param)
{
	i_t ret;
	struct espress_stream_session *this = (struct espress_stream_session *)param;
	struct espress_stream_uint32 src_receive_window_address[1];
	struct espress_stream_uint32 src_receive_window_size[1];
	espress_stream_mutex->lock(this->mutex);
	espress_stream_ring_buffer->head(this->recv_window->buffer, src_receive_window_address, 0);
	espress_stream_uint32_set_u(src_receive_window_size, espress_stream_ring_buffer->num_invalid(this->recv_window->buffer));
	ret = espress_stream_trans_packet->send_envelope(this->trans_packet,
		src_receive_window_address,
		src_receive_window_size,
		this->send_window->data_address,
		NULL, 0,
		NULL, 0);
	if (0 == ret)
	{
		this->is_ack_ready = 0;
	}
	espress_stream_timer->start(this->beacon_timer, _beacon_timeout_callback, this, espress_stream_beacon_interval_time);
	espress_stream_mutex->unlock(this->mutex);
}

static void _send_timer_yield(struct espress_stream_session *this, struct espress_stream_uint32 const *timeout)
{
	espress_stream_timer->yield(this->send_timer, timeout);
}

static void _send_session_timer_yield(struct espress_stream_session *this, struct espress_stream_uint32 const *timeout)
{
	espress_stream_timer->yield(this->send_session_timer, timeout);
}

static void _recv_session_timer_yield(struct espress_stream_session *this, struct espress_stream_uint32 const *timeout)
{
	espress_stream_timer->yield(this->recv_session_timer, timeout);
}

static void _beacon_timer_yield(struct espress_stream_session *this, struct espress_stream_uint32 const *timeout)
{
	espress_stream_timer->yield(this->beacon_timer, timeout);
}

static i_t _send_connect_payload_callback(struct espress_stream_session *this, b_t const *payload, u_t payload_size, i_t is_rejected)
{
	i_t ret;
	u_t u;
	u_t v;
	struct espress_stream_uint32 const reject_param_val[1] = {ESPRESS_STREAM_UINT32(0xf0, 0x00, 0x00, 0x00)};
	if (this->is_server)
	{
		espress_stream_uint32_get_u(this->src_connection_param, &u);
		espress_stream_uint32_get_u(this->dst_connection_param, &v);
		if ((u & 0xffff) != (v & 0xffff))
		{
			is_rejected = 1;
		}
	}
	if (is_rejected)
	{
		espress_stream_uint32_or(this->src_connection_param, reject_param_val);
	}
	ret = espress_stream_trans_packet->send_connect(this->trans_packet,
		this->dst_address,
		this->src_connection_param,
		espress_stream_ring_buffer->size(this->recv_window->buffer),
		payload, payload_size);
	return ret;
}

static i_t _recv_connect_payload_callback(struct espress_stream_session *this, b_t *payload, u_t *payload_size)
{
	i_t ret;
	ret = espress_stream_trans_packet->recv_connect(this->trans_packet,
		this->dst_address,
		this->dst_connection_param,
		&(this->send_window->size),
		payload, payload_size);
	if (0 == ret)
	{
		if (espress_stream_ring_buffer->size(this->send_window->buffer) < this->send_window->size)
		{
			this->send_window->size = espress_stream_ring_buffer->size(this->send_window->buffer);
		}
	}
	return ret;
}

static i_t _send_disconnect_payload_callback(struct espress_stream_session *this, b_t const *payload, u_t payload_size)
{
	return espress_stream_trans_packet->send_disconnect(this->trans_packet,
		payload, payload_size);
}

static i_t _recv_disconnect_payload_callback(struct espress_stream_session *this, b_t *payload, u_t *payload_size)
{
	return espress_stream_trans_packet->recv_disconnect(this->trans_packet,
		payload, payload_size);
}

static void _post_process_recv_ack(struct espress_stream_session *this,
	struct espress_stream_uint32 const *dst_receive_window_address,
	struct espress_stream_uint32 const *dst_receive_window_size,
	i_t dst_is_ack_true)
{
	u_t u;
	struct espress_stream_uint32 v[1];
	struct espress_stream_uint32 tail_address[1];
	struct espress_stream_uint32 sent_size[1];
	struct espress_stream_uint32 rcvd_size[1];

	espress_stream_timer->cancel(this->send_session_timer);
	espress_stream_timer->start(this->send_session_timer, _send_session_timeout_callback, this, espress_stream_send_session_timeout_time);

	espress_stream_ring_buffer->tail(this->send_window->buffer, tail_address, 0);
	espress_stream_uint32_assign(sent_size, this->send_window->data_address);
	espress_stream_uint32_sub(sent_size, tail_address);
	espress_stream_uint32_assign(rcvd_size, dst_receive_window_address);
	espress_stream_uint32_sub(rcvd_size, tail_address);
	espress_stream_uint32_set_u(v, espress_stream_ring_buffer->num_valid(this->send_window->buffer));
	if (espress_stream_uint32_le(rcvd_size, v))
	{
		espress_stream_uint32_get_u(rcvd_size, &u);
		espress_stream_ring_buffer->de(this->send_window->buffer, u);
		if (espress_stream_uint32_lt(sent_size, rcvd_size))
		{
			espress_stream_uint32_assign(this->send_window->data_address, dst_receive_window_address);
		}
		if (u < this->send_window->flush_size)
		{
			this->send_window->flush_size -= u;
		}
		else if (0 < this->send_window->flush_size)
		{
			this->send_window->flush_size = 0;
		}
#		if 4 <= U_T_WIDTH
		{
			espress_stream_uint32_get_u(dst_receive_window_size, &(this->send_window->size));
		}
#		else
		{
			espress_stream_uint32_set_u(v, U_T_MAX);
			if (espress_stream_uint32_lt(dst_receive_window_size, v))
			{
				espress_stream_uint32_get_u(dst_receive_window_size, &(this->send_window->size));
			}
			else
			{
				this->send_window->size = U_T_MAX;
			}
		}
#		endif
		espress_stream_event->set(this->outer_event, ESPRESS_STREAM_EVENT_SESSION_SEND);
		espress_stream_event->set(this->event, ESPRESS_STREAM_EVENT_TRANS_PACKET_SEND);
	}

	if (dst_is_ack_true)
	{
		if (_check_fast_retransmit(this->fast_retransmit,
			espress_stream_trans_packet->dac(this->trans_packet),
			this->send_window))
		{
			_tell_fast_retransmit_cong_avoid(this->cong_avoid);
			espress_stream_event->set(this->event, ESPRESS_STREAM_EVENT_TRANS_PACKET_SEND);
			this->fast_retransmit_count++;
		}

		_check_cong_avoid(this->cong_avoid, _espress_stream_packet_envelope_payload_max_size(this), 1);
	}
}

static i_t _yield_client_send_connect(struct espress_stream_session *this)
{
	i_t ret;

	espress_stream_uint32_assign(this->src_connection_param, espress_stream_protocol_version);
	if (NULL == this->send_connect_payload)
	{
		ret = _send_connect_payload_callback(this, NULL, 0, 0);
	}
	else
	{
		ret = this->send_connect_payload(this, _send_connect_payload_callback, this->send_connect_payload_param);
	}
	if (0 == ret)
	{
		this->send_state = _WAIT_CONNECT;
	}
	else if (0 < ret)
	{
		this->send_state = _CLOSED;
		this->recv_state = _CLOSED;
		espress_stream_event->set(this->outer_event, ESPRESS_STREAM_EVENT_SESSION_CONNECT_RESULT);
	}

	return ret;
}

static i_t _yield_send_send_disconnect(struct espress_stream_session *this)
{
	i_t ret;

	if (NULL == this->send_disconnect_payload)
	{
		ret = _send_disconnect_payload_callback(this, NULL, 0);
	}
	else
	{
		ret = this->send_disconnect_payload(this, _send_disconnect_payload_callback, this->send_disconnect_payload_param);
	}
	if (0 == ret)
	{
		this->send_state = _CLOSED;
		espress_stream_event->set(this->outer_event, ESPRESS_STREAM_EVENT_SESSION_DISCONNECT_RESULT);
	}
	else if (0 < ret)
	{
		this->send_state = _ESTABLISHED;
		espress_stream_event->set(this->outer_event, ESPRESS_STREAM_EVENT_SESSION_DISCONNECT_RESULT);
	}

	return ret;
}

static i_t _yield_send_send_established(struct espress_stream_session *this)
{
	i_t ret;
	struct espress_stream_uint32 u[1];
	u_t v;
	struct espress_stream_uint32 src_receive_window_address[1];
	struct espress_stream_uint32 src_receive_window_size[1];
	u_t sent_size;
	u_t rest_size;
	u_t min_payload_size;
	u_t send_window_size;

	espress_stream_ring_buffer->head(this->send_window->buffer, u, 0);
	espress_stream_uint32_sub(u, this->send_window->data_address);
	espress_stream_uint32_get_u(u, &rest_size);
	sent_size = espress_stream_ring_buffer->num_valid(this->send_window->buffer) - rest_size;
	if ((0 == this->send_window->size) && (0 == sent_size))
	{
		min_payload_size = 1;
		send_window_size = 1;
	}
	else
	{
		min_payload_size =
			((0 < this->send_window->flush_size) &&
				(this->send_window->flush_size < _espress_stream_packet_envelope_payload_max_size(this))) ?
				this->send_window->flush_size : _espress_stream_packet_envelope_payload_max_size(this);
		send_window_size =
			(this->cong_avoid->cwnd_size < this->send_window->size) ?
				this->cong_avoid->cwnd_size : this->send_window->size;
	}
	if ((min_payload_size <= rest_size) && (sent_size < send_window_size))
	{
		b_t *buffer;
		u_t buffer_size;
		b_t *data;
		u_t data_size;

		espress_stream_ring_buffer->head(this->recv_window->buffer, src_receive_window_address, 0);
		espress_stream_uint32_set_u(src_receive_window_size, espress_stream_ring_buffer->num_invalid(this->recv_window->buffer));

		buffer = espress_stream_ring_buffer->buffer(this->send_window->buffer);
		buffer_size =  espress_stream_ring_buffer->size(this->send_window->buffer);
		data = espress_stream_ring_buffer->valid_data(this->send_window->buffer, this->send_window->data_address);
		if (espress_stream_ring_buffer->num_valid(this->send_window->buffer) < send_window_size)
		{
			data_size = espress_stream_ring_buffer->num_valid(this->send_window->buffer) - sent_size;
		}
		else
		{
			data_size = send_window_size - sent_size;
		}
		if (_espress_stream_packet_envelope_payload_max_size(this) < data_size)
		{
			data_size = _espress_stream_packet_envelope_payload_max_size(this);
		}

		if (buffer_size - (data - buffer) <  data_size)
		{
			v = data_size - (buffer_size - (data - buffer));
		}
		else
		{
			v = 0;
		}
		ret = espress_stream_trans_packet->send_envelope(this->trans_packet,
			src_receive_window_address,
			src_receive_window_size,
			this->send_window->data_address,
			data, data_size - v,
			buffer, v);
		if (0 == ret)
		{
			this->is_ack_ready = 0;

			espress_stream_uint32_set_u(u, data_size);
			espress_stream_uint32_add(this->send_window->data_address, u);

			espress_stream_timer->cancel(this->send_timer);
			espress_stream_trans_packet->rto(this->trans_packet, u);
			espress_stream_timer->start(this->send_timer, _send_timeout_callback, (void *)this, u);

			espress_stream_timer->cancel(this->beacon_timer);
			espress_stream_timer->start(this->beacon_timer, _beacon_timeout_callback, this, espress_stream_beacon_interval_time);
		}
	}
	else
	{
		ret = 1;
	}

	return ret;
}

static i_t _yield_server_send_connect(struct espress_stream_session *this)
{
	i_t ret;
	struct espress_stream_uint32 connection_param[1];
	struct espress_stream_uint32 const reject_param_val[1] = {ESPRESS_STREAM_UINT32(0xf0, 0x00, 0x00, 0x00)};

	espress_stream_uint32_assign(this->src_connection_param, espress_stream_protocol_version);
	if (NULL == this->send_connect_payload)
	{
		ret = _send_connect_payload_callback(this, NULL, 0, 0);
	}
	else
	{
		ret = this->send_connect_payload(this, _send_connect_payload_callback, this->send_connect_payload_param);
	}
	if (0 == ret)
	{
		espress_stream_uint32_assign(connection_param, this->src_connection_param);
		espress_stream_uint32_and(connection_param, reject_param_val);
		if (espress_stream_uint32_eq(connection_param, espress_stream_uint32_0))
		{
			espress_stream_timer->start(this->send_session_timer, _send_session_timeout_callback, this, espress_stream_send_session_timeout_time);
			espress_stream_timer->start(this->beacon_timer, _beacon_timeout_callback, this, espress_stream_beacon_interval_time);
			this->send_state = _ESTABLISHED;
			espress_stream_timer->start(this->recv_session_timer, _recv_session_timeout_callback, this, espress_stream_recv_session_timeout_time);
			this->recv_state = _ESTABLISHED;
		}
		else
		{
			this->send_state = _CLOSED;
			this->recv_state = _CLOSED;
		}
		espress_stream_event->set(this->outer_event, ESPRESS_STREAM_EVENT_SESSION_CONNECT_DETECT);
	}
	else if (0 < ret)
	{
		this->send_state = _CLOSED;
		this->recv_state = _CLOSED;
		espress_stream_event->set(this->outer_event, ESPRESS_STREAM_EVENT_SESSION_CONNECT_DETECT);
	}

	return ret;
}

static i_t _yield_recv_send_established(struct espress_stream_session *this)
{
	i_t ret;
	struct espress_stream_uint32 u[1];
	struct espress_stream_uint32 v[1];

	if (this->is_ack_ready)
	{
		espress_stream_ring_buffer->head(this->recv_window->buffer, u, 0);
		espress_stream_uint32_set_u(v, espress_stream_ring_buffer->num_invalid(this->recv_window->buffer));
		ret = espress_stream_trans_packet->send_ack(this->trans_packet, u, v);
		if (0 == ret)
		{
			this->is_ack_ready = 0;
		}
	}
	else
	{
		ret = 1;
	}

	return ret;
}

static i_t _yield_client_recv_wait_connect(struct espress_stream_session *this)
{
	i_t ret;
	struct espress_stream_uint32 connection_param[1];
	struct espress_stream_uint32 const reject_param_val[1] = {ESPRESS_STREAM_UINT32(0xf0, 0x00, 0x00, 0x00)};

	if (NULL == this->recv_connect_payload)
	{
		ret = _recv_connect_payload_callback(this, NULL, NULL);
	}
	else
	{
		ret = this->recv_connect_payload(this, _recv_connect_payload_callback, this->recv_connect_payload_param);
	}
	if (0 == ret)
	{
		espress_stream_uint32_assign(connection_param, this->dst_connection_param);
		espress_stream_uint32_and(connection_param, reject_param_val);
		if (espress_stream_uint32_eq(connection_param, espress_stream_uint32_0))
		{
			espress_stream_timer->start(this->send_session_timer, _send_session_timeout_callback, this, espress_stream_send_session_timeout_time);
			espress_stream_timer->start(this->beacon_timer, _beacon_timeout_callback, this, espress_stream_beacon_interval_time);
			this->send_state = _ESTABLISHED;
			espress_stream_timer->start(this->recv_session_timer, _recv_session_timeout_callback, this, espress_stream_recv_session_timeout_time);
			this->recv_state = _ESTABLISHED;
		}
		else
		{
			this->send_state = _CLOSED;
			this->recv_state = _CLOSED;
		}
		espress_stream_event->set(this->outer_event, ESPRESS_STREAM_EVENT_SESSION_CONNECT_RESULT);
	}

	return ret;
}

static i_t _yield_send_recv_established(struct espress_stream_session *this)
{
	i_t ret;
	struct espress_stream_uint32 dst_receive_window_address[1];
	struct espress_stream_uint32 dst_receive_window_size[1];
	i_t dst_is_ack_true;

	ret = espress_stream_trans_packet->recv_ack(this->trans_packet,
		dst_receive_window_address,
		dst_receive_window_size,
		&dst_is_ack_true);
	if (0 == ret)
	{
		_post_process_recv_ack(this, dst_receive_window_address, dst_receive_window_size, dst_is_ack_true);
	}

	return ret;
}

static i_t _yield_server_recv_wait_connect(struct espress_stream_session *this)
{
	i_t ret;

	if (NULL == this->recv_connect_payload)
	{
		ret = _recv_connect_payload_callback(this, NULL, NULL);
	}
	else
	{
		ret = this->recv_connect_payload(this, _recv_connect_payload_callback, this->recv_connect_payload_param);
	}
	if (0 == ret)
	{
		this->recv_state = _CONNECT;
		espress_stream_event->set(this->event, ESPRESS_STREAM_EVENT_TRANS_PACKET_SEND);
	}
	else if (0 < ret)
	{
		this->send_state = _CLOSED;
		this->recv_state = _CLOSED;
		espress_stream_event->set(this->outer_event, ESPRESS_STREAM_EVENT_SESSION_CONNECT_DETECT);
	}

	return ret;
}

static i_t _recv_envelope(struct espress_stream_session *this,
	struct espress_stream_uint32 *dst_receive_window_address,
	struct espress_stream_uint32 *dst_receive_window_size,
	i_t *dst_is_ack_true,
	struct espress_stream_uint32 const *data_address,
	struct espress_stream_uint32 *dst_send_data_address,
	u_t *dst_send_data_size)
{
	i_t ret;
	u_t u;
	u_t v;
	b_t *buffer;
	u_t buffer_size;
	b_t *data;

	buffer = espress_stream_ring_buffer->buffer(this->recv_window->buffer);
	buffer_size =  espress_stream_ring_buffer->size(this->recv_window->buffer);
	data = espress_stream_ring_buffer->invalid_data(this->recv_window->buffer, data_address);
	if (buffer_size - (data - buffer) < *dst_send_data_size)
	{
		u = *dst_send_data_size - (buffer_size - (data - buffer));
	}
	else
	{
		u = 0;
	}
	v = *dst_send_data_size - u;
	ret = espress_stream_trans_packet->recv_envelope(this->trans_packet,
		dst_receive_window_address,
		dst_receive_window_size,
		dst_is_ack_true,
		dst_send_data_address,
		data_address,
		data, &v,
		buffer, &u);
	if (0 == ret)
	{
		*dst_send_data_size = u + v;
	}

	return ret;
}

static i_t _yield_recv_recv_established(struct espress_stream_session *this)
{
	i_t ret;
	struct espress_stream_uint32 dst_receive_window_address[1];
	struct espress_stream_uint32 dst_receive_window_size[1];
	i_t dst_is_ack_true;
	struct espress_stream_uint32 data_address[1];
	u_t data_size;
	struct espress_stream_uint32 dst_send_data_address[1];
	u_t dst_send_data_size;

#	if ESPRESS_STREAM_IS_ADD_DATA_ENABLED
		i_t i;
		struct espress_stream_uint32 u[1];
		u_t v;
		u_t offset;
		u_t total_dst_send_data_size;

		espress_stream_ring_buffer->head(this->recv_window->buffer, data_address, 0);
		if (0 < this->recv_window->add_data_size)
		{
			espress_stream_uint32_assign(u, this->recv_window->add_data_address);
			espress_stream_uint32_sub(u, data_address);
			espress_stream_uint32_get_u(u, &data_size);
		}
		else
		{
			data_size =  espress_stream_ring_buffer->num_invalid(this->recv_window->buffer);
		}
		dst_send_data_size = data_size;
		ret = _recv_envelope(this,
			dst_receive_window_address,
			dst_receive_window_size,
			&dst_is_ack_true,
			data_address,
			dst_send_data_address,
			&dst_send_data_size);
		if (0 == ret)
		{
			if (_ESTABLISHED == this->send_state)
			{
				_post_process_recv_ack(this, dst_receive_window_address, dst_receive_window_size, dst_is_ack_true);
			}

			espress_stream_timer->cancel(this->recv_session_timer);
			espress_stream_timer->start(this->recv_session_timer, _recv_session_timeout_callback, this, espress_stream_recv_session_timeout_time);

				/*
				 *               +- dst_send_data_size
				 *               |
				 *            <--+--->
				 *    ....####....====....
				 *            |   |
				 *            |   +- dst_send_data_address
				 *            |
				 *            +- data_address
				 */

			if (0 < dst_send_data_size)
			{
				total_dst_send_data_size = 0;
				i = 0;
				if (espress_stream_uint32_eq(data_address, dst_send_data_address))
				{
					espress_stream_ring_buffer->en(this->recv_window->buffer, dst_send_data_size);
					total_dst_send_data_size += dst_send_data_size;
					if ((0 < this->recv_window->add_data_size) && (data_size == dst_send_data_size))
					{
						/*
						 *    ....####....++++....
						 *            |
						 *            V
						 *    ....####====++++....
						 *            |
						 *            V
						 *    ....############....
						 */
						espress_stream_ring_buffer->en(this->recv_window->buffer, this->recv_window->add_data_size);
						total_dst_send_data_size += this->recv_window->add_data_size;
						this->recv_window->add_data_size = 0;
						i = 1;
					}
					else
					{
						/*
						 *    ....####........       / ....####....++++....
						 *            |             /          |
						 *            V            /           V
						 *    ....####====....    /    ....####===.++++....
						 *            |          /             |
						 *            V         /              V
						 *    ....########.... /       ....#######.++++....
						 */
					}
				}
				else
				{
					espress_stream_uint32_assign(u, dst_send_data_address);
					espress_stream_uint32_sub(u, data_address);
					espress_stream_uint32_get_u(u, &offset);
					espress_stream_uint32_assign(u, this->recv_window->add_data_address);
					espress_stream_uint32_assign(this->recv_window->add_data_address, dst_send_data_address);
					if (0 < this->recv_window->add_data_size)
					{
						espress_stream_uint32_sub(u, data_address);
						espress_stream_uint32_get_u(u, &v);
						if (dst_send_data_size < v)
						{
							/*
							 *    ....####....++++....
							 *            |
							 *            V
							 *    ....####.==.++++....
							 *            |
							 *            V
							 *    ....####.++.........
							 */
							this->recv_window->add_data_size = dst_send_data_size - offset;
						}
						else
						{
							/*
							 *    ....####....++++....
							 *            |
							 *            V
							 *    ....####.===++++....
							 *            |
							 *            V
							 *    ....####.+++++++....
							 */
							this->recv_window->add_data_size += (v - offset);
							i = 1;
						}
					}
					else
					{
						/*
						 *    ....####........
						 *            |
						 *            V
						 *    ....####....====
						 *            |
						 *            V
						 *    ....####....++++
						 */
						this->recv_window->add_data_size = dst_send_data_size - offset;
					}
				}

				if (i)
				{
					if (0 < this->recv_window->add_data_size)
					{
						espress_stream_uint32_assign(u, this->recv_window->add_data_address);
						espress_stream_ring_buffer->head(this->recv_window->buffer, data_address, 0);
						espress_stream_uint32_sub(u, data_address);
						espress_stream_uint32_get_u(u, &data_size);
						data_size += this->recv_window->add_data_size;
						espress_stream_uint32_set_u(u, data_size);
						espress_stream_uint32_add(data_address, u);
						data_size = espress_stream_ring_buffer->num_invalid(this->recv_window->buffer) - data_size;
					}
					else
					{
						espress_stream_ring_buffer->head(this->recv_window->buffer, data_address, 0);
						data_size =  espress_stream_ring_buffer->num_invalid(this->recv_window->buffer);
					}
					dst_send_data_size = data_size;
					if ((0 == _recv_envelope(this,
							NULL,
							NULL,
							NULL,
							data_address,
							dst_send_data_address,
							&dst_send_data_size)) &&
						(0 < dst_send_data_size))
					{

						/*
						*                       +- dst_send_data_size
						*                       |
						*                    <--+--->
						*    ....####....++++....====....
						*                    |   |
						*                    |   +- dst_send_data_address
						*                    |
						*                    +- data_address
						*/

						if (espress_stream_uint32_eq(data_address, dst_send_data_address))
						{
							if (0 < this->recv_window->add_data_size)
							{
								/*
								*    ....####....++++........
								*            |
								*            V
								*    ....####....++++====....
								*            |
								*            V
								*    ....####....++++++++....
								*/
								this->recv_window->add_data_size += dst_send_data_size;
							}
							else
							{
								/*
								*    ....####........
								*            |
								*            V
								*    ....####====....
								*            |
								*            V
								*    ....########....
								*/
								espress_stream_ring_buffer->en(this->recv_window->buffer, dst_send_data_size);
								total_dst_send_data_size += dst_send_data_size;
							}
						}
						else
						{
							espress_stream_uint32_assign(u, dst_send_data_address);
							espress_stream_uint32_sub(u, data_address);
							espress_stream_uint32_get_u(u, &offset);
							if (0 < this->recv_window->add_data_size)
							{
								/*
								*    ....####....++++........
								*            |
								*            V
								*    ....####....++++.===....
								*            |
								*            V
								*    ....####....++++........
								*/
							}
							else
							{
								/*
								*    ....####............
								*            |
								*            V
								*    ....####....====....
								*            |
								*            V
								*    ....####....++++....
								*/
								espress_stream_uint32_assign(this->recv_window->add_data_address, dst_send_data_address);
								this->recv_window->add_data_size = dst_send_data_size - offset;
							}
						}
					}
				}

				if (0 < total_dst_send_data_size)
				{
					espress_stream_event->set(this->outer_event, ESPRESS_STREAM_EVENT_SESSION_RECV);
				}
			}
			this->is_ack_ready = 1;
			espress_stream_event->set(this->event, ESPRESS_STREAM_EVENT_TRANS_PACKET_SEND);
			goto EXIT;
		}
#	else
		espress_stream_ring_buffer->head(this->recv_window->buffer, data_address, 0);
		data_size =  espress_stream_ring_buffer->num_invalid(this->recv_window->buffer);
		dst_send_data_size =  data_size;
		ret = _recv_envelope(this,
			dst_receive_window_address,
			dst_receive_window_size,
			&dst_is_ack_true,
			data_address,
			dst_send_data_address,
			&dst_send_data_size);
		if (0 == ret)
		{
			if (_ESTABLISHED == this->send_state)
			{
				_post_process_recv_ack(this, dst_receive_window_address, dst_receive_window_size, dst_is_ack_true);
			}

			espress_stream_timer->cancel(this->recv_session_timer);
			espress_stream_timer->start(this->recv_session_timer, _recv_session_timeout_callback, this, espress_stream_recv_session_timeout_time);

			if (0 < dst_send_data_size)
			{
				if (espress_stream_uint32_eq(data_address, dst_send_data_address))
				{
					espress_stream_ring_buffer->en(this->recv_window->buffer, dst_send_data_size);
					espress_stream_event->set(this->outer_event, ESPRESS_STREAM_EVENT_SESSION_RECV);
				}
				this->is_ack_ready = 1;
				espress_stream_event->set(this->event, ESPRESS_STREAM_EVENT_TRANS_PACKET_SEND);
			}
			goto EXIT;
		}
#	endif

	if (NULL == this->recv_disconnect_payload)
	{
		ret = _recv_disconnect_payload_callback(this, NULL, NULL);
	}
	else
	{
		ret = this->recv_disconnect_payload(this, _recv_disconnect_payload_callback, this->recv_disconnect_payload_param);
	}
	if (0 == ret)
	{
		this->recv_state = _CLOSED;
		espress_stream_event->set(this->outer_event, ESPRESS_STREAM_EVENT_SESSION_DISCONNECT_DETECT);
		goto EXIT;
	}

EXIT:

	return ret;
}

static void _this_yield(struct espress_stream_session *this, struct espress_stream_uint32 const *timeout)
{
	i_t ret;
	struct espress_stream_event_flags flags[1];

	espress_stream_event->wait(this->event, flags, timeout);

	espress_stream_mutex->lock(this->mutex);

	if (espress_stream_event_flags->is_set(flags, ESPRESS_STREAM_EVENT_TRANS_PACKET_SEND))
	{
		switch (this->send_state)
		{
		case _CONNECT:
			ret = _yield_client_send_connect(this);
			break;
		case _DISCONNECT:
			ret = _yield_send_send_disconnect(this);
			break;
		case _ESTABLISHED:
			ret = _yield_send_send_established(this);
			break;
		default:
			ret = 1;
			break;
		}

		if (ret < 0)
		{
			espress_stream_event->set(this->event, ESPRESS_STREAM_EVENT_TRANS_PACKET_SEND);
		}
		else if (0 < ret)
		{
			switch (this->recv_state)
			{
			case _CONNECT:
				ret = _yield_server_send_connect(this);
				break;
			case _ESTABLISHED:
				ret = _yield_recv_send_established(this);
				break;
			default:
				ret = 1;
				break;
			}

			if (ret < 0)
			{
				espress_stream_event->set(this->event, ESPRESS_STREAM_EVENT_TRANS_PACKET_SEND);
			}
		}
	}

	if (espress_stream_event_flags->is_set(flags, ESPRESS_STREAM_EVENT_TRANS_PACKET_RECV))
	{
		switch (this->send_state)
		{
		case _WAIT_CONNECT:
			ret = _yield_client_recv_wait_connect(this);
			break;
		case _ESTABLISHED:
			ret = _yield_send_recv_established(this);
			break;
		default:
			ret = 1;
			break;
		}

		if (0 == ret)
		{
			espress_stream_event->set(this->event, ESPRESS_STREAM_EVENT_TRANS_PACKET_RECV);
		}
		else if (0 < ret)
		{
			switch (this->recv_state)
			{
			case _WAIT_CONNECT:
				ret = _yield_server_recv_wait_connect(this);
				break;
			case _ESTABLISHED:
				ret = _yield_recv_recv_established(this);
				break;
			default:
				ret = 1;
				break;
			}

			if (0 <= ret)
			{
				espress_stream_event->set(this->event, ESPRESS_STREAM_EVENT_TRANS_PACKET_RECV);
			}
		}

		if (0 <= ret)
		{
			espress_stream_trans_packet->purge(this->trans_packet);
		}
	}

	if (espress_stream_event_flags->is_set(flags, ESPRESS_STREAM_EVENT_NONE))
	{
		/* nop */
	}

	espress_stream_mutex->unlock(this->mutex);
}

static void _send_timer_yield_break(struct espress_stream_session *this)
{
	espress_stream_timer->yield_break(this->send_timer);
}

static void _send_session_timer_yield_break(struct espress_stream_session *this)
{
	espress_stream_timer->yield_break(this->send_session_timer);
}

static void _recv_session_timer_yield_break(struct espress_stream_session *this)
{
	espress_stream_timer->yield_break(this->recv_session_timer);
}

static void _beacon_timer_yield_break(struct espress_stream_session *this)
{
	espress_stream_timer->yield_break(this->beacon_timer);
}

static void _this_yield_break(struct espress_stream_session *this)
{
	espress_stream_event->set(this->event, ESPRESS_STREAM_EVENT_NONE);
}
