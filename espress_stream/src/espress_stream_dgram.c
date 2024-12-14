#include "espress_stream/config.h"
#include "espress_stream/depend.h"
#include "espress_stream/uint.h"
#include "espress_stream/tick.h"
#include "espress_stream/mutex.h"
#include "espress_stream/cond.h"
#include "espress_stream/event.h"
#include "espress_stream/ring.h"
#include "espress_stream/dgram.h"

static i_t _initialize(struct espress_stream_dgram *this, struct espress_stream_dgram_address const *bind_address, struct espress_stream_event *send_event, struct espress_stream_event *recv_event);
static void _finalize(struct espress_stream_dgram *this);
static i_t _send_request(struct espress_stream_dgram *this, struct espress_stream_dgram_buffer * const *buffer, u_t num, struct espress_stream_dgram_buffer * const *ex_buffer, u_t ex_num);
static i_t _send_result(struct espress_stream_dgram *this, u_t *num);
static i_t _recv_request(struct espress_stream_dgram *this, struct espress_stream_dgram_buffer * const *buffer, u_t num, struct espress_stream_dgram_buffer * const *ex_buffer, u_t ex_num);
static i_t _recv_result(struct espress_stream_dgram *this, u_t *num);
static u_t _mtu(struct espress_stream_dgram *this);
static void _send_yield(struct espress_stream_dgram *this, struct espress_stream_uint32 const *timeout);
static void _recv_yield(struct espress_stream_dgram *this, struct espress_stream_uint32 const *timeout);
static void _send_yield_break(struct espress_stream_dgram *this);
static void _recv_yield_break(struct espress_stream_dgram *this);

struct espress_stream_dgram_ops const espress_stream_dgram[] =
{{
	_initialize,
	_finalize,
	_send_request,
	_send_result,
	_recv_request,
	_recv_result,
	_mtu,
	_send_yield,
	_recv_yield,
	_send_yield_break,
	_recv_yield_break
}};

static void _udp_recv_callback(void *arg, struct udp_pcb *pcb, struct pbuf *p, const ip_addr_t *addr, u16_t port)
{
	struct espress_stream_dgram *this = (struct espress_stream_dgram *)arg;
	espress_stream_mutex->lock(this->recv_mutex);
	if ((ERR_OK != this->udp_recvfrom) && (0 < this->num_recv_buffer) && (0 == this->num_rcvd_buffer))
	{
		if (p->len < this->recv_buffer[0]->data_size)
		{
			this->recv_buffer[0]->data_size = p->len;
		}
		memcpy(this->recv_buffer[0]->data, p->payload, this->recv_buffer[0]->data_size);
		memcpy(this->recv_buffer[0]->address->storage->address, addr, sizeof(*(this->recv_buffer[0]->address->storage->address)));
		this->recv_buffer[0]->address->storage->port = port;
		this->udp_recvfrom = ERR_OK;
	}
	pbuf_free(p);
	espress_stream_mutex->unlock(this->recv_mutex);
}

static i_t _initialize(struct espress_stream_dgram *this, struct espress_stream_dgram_address const *bind_address, struct espress_stream_event *send_event, struct espress_stream_event *recv_event)
{
	i_t ret = 0;
	i_t i;

	this->send_outer_event = send_event;
	this->recv_outer_event = recv_event;

	i = espress_stream_event->initialize(this->send_event);
	if (0 != i)
	{
		ret = 1;
		goto SEND_EVENT_INIT_FAIL_EXIT;
	}
	i = espress_stream_event->initialize(this->recv_event);
	if (0 != i)
	{
		ret = 1;
		goto RECV_EVENT_INIT_FAIL_EXIT;
	}
	i = espress_stream_mutex->initialize(this->send_mutex);
	if (0 != i)
	{
		ret = 1;
		goto SEND_MUTEX_INIT_FAIL_EXIT;
	}
	i = espress_stream_mutex->initialize(this->recv_mutex);
	if (0 != i)
	{
		ret = 1;
		goto RECV_MUTEX_INIT_FAIL_EXIT;
	}

	i = espress_stream_event->initialize(this->send_break_event);
	if (0 != i)
	{
		ret = 1;
		goto SEND_BREAK_EVENT_INIT_FAIL_EXIT;
	}
	i = espress_stream_event->initialize(this->recv_break_event);
	if (0 != i)
	{
		ret = 1;
		goto RECV_BREAK_EVENT_INIT_FAIL_EXIT;
	}

	this->pcb = udp_new_ip_type(IPADDR_TYPE_V4);
	if (NULL == this->pcb)
	{
		ret = 1;
		goto PCB_NEW_FAIL_EXIT;
	}
	i = udp_bind(this->pcb, bind_address->storage->address, bind_address->storage->port);
	if (ERR_OK != i)
	{
		ret = i;
		goto BIND_FAIL_EXIT;
	}

	this->num_send_buffer = 0;
	this->num_sent_buffer = 0;
	this->num_recv_buffer = 0;
	this->num_rcvd_buffer = 0;
	this->udp_recvfrom = -1;
	udp_recv(this->pcb, _udp_recv_callback, this);

BIND_FAIL_EXIT:
	if (0 != ret) udp_remove(this->pcb);
PCB_NEW_FAIL_EXIT:
	if (0 != ret) espress_stream_event->finalize(this->recv_break_event);
RECV_BREAK_EVENT_INIT_FAIL_EXIT:
	if (0 != ret) espress_stream_event->finalize(this->send_break_event);
SEND_BREAK_EVENT_INIT_FAIL_EXIT:
	if (0 != ret) espress_stream_mutex->finalize(this->recv_mutex);
RECV_MUTEX_INIT_FAIL_EXIT:
	if (0 != ret) espress_stream_mutex->finalize(this->send_mutex);
SEND_MUTEX_INIT_FAIL_EXIT:
	if (0 != ret) espress_stream_event->finalize(this->recv_event);
RECV_EVENT_INIT_FAIL_EXIT:
	if (0 != ret) espress_stream_event->finalize(this->send_event);
SEND_EVENT_INIT_FAIL_EXIT:

	return ret;
}

static void _finalize(struct espress_stream_dgram *this)
{
	udp_remove(this->pcb);
	espress_stream_event->finalize(this->recv_break_event);
	espress_stream_event->finalize(this->send_break_event);
	espress_stream_mutex->finalize(this->recv_mutex);
	espress_stream_mutex->finalize(this->send_mutex);
	espress_stream_event->finalize(this->recv_event);
	espress_stream_event->finalize(this->send_event);
	memset(this, 0, sizeof(*this));
}

static i_t _send_request(struct espress_stream_dgram *this, struct espress_stream_dgram_buffer * const *buffer, u_t num, struct espress_stream_dgram_buffer * const *ex_buffer, u_t ex_num)
{
	i_t ret = 0;
	(void)ex_buffer;
	(void)ex_num;
	espress_stream_mutex->lock(this->send_mutex);
	cyw43_arch_lwip_begin();
	if ((0 == this->num_send_buffer) && (0 == this->num_sent_buffer))
	{
		this->send_buffer = buffer;
		this->num_send_buffer = num;
		espress_stream_event->set(this->send_event, 1);
	}
	else
	{
		ret = -1;
	}
	cyw43_arch_lwip_end();
	espress_stream_mutex->unlock(this->send_mutex);
	return ret;
}

static i_t _send_result(struct espress_stream_dgram *this, u_t *num)
{
	i_t ret;
	espress_stream_mutex->lock(this->send_mutex);
	cyw43_arch_lwip_begin();
	if (0 == this->num_send_buffer)
	{
		*num = this->num_sent_buffer;
		ret = this->send_errno;
		this->num_sent_buffer = 0;
		this->send_errno = 0;
	}
	else
	{
		*num = 0;
		ret = -1;
	}
	cyw43_arch_lwip_end();
	espress_stream_mutex->unlock(this->send_mutex);
	return ret;
}

static i_t _recv_request(struct espress_stream_dgram *this, struct espress_stream_dgram_buffer * const *buffer, u_t num, struct espress_stream_dgram_buffer * const *ex_buffer, u_t ex_num)
{
	i_t ret = 0;
	(void)ex_buffer;
	(void)ex_num;
	espress_stream_mutex->lock(this->recv_mutex);
	cyw43_arch_lwip_begin();
	if ((0 == this->num_recv_buffer) && (0 == this->num_rcvd_buffer))
	{
		this->recv_buffer = buffer;
		this->num_recv_buffer = num;
		espress_stream_event->set(this->recv_event, 1);
	}
	else
	{
		ret = -1;
	}
	cyw43_arch_lwip_end();
	espress_stream_mutex->unlock(this->recv_mutex);
	return ret;
}

static i_t _recv_result(struct espress_stream_dgram *this, u_t *num)
{
	i_t ret;
	espress_stream_mutex->lock(this->recv_mutex);
	cyw43_arch_lwip_begin();
	if (0 == this->num_recv_buffer)
	{
		*num = this->num_rcvd_buffer;
		ret = this->recv_errno;
		this->num_rcvd_buffer = 0;
		this->recv_errno = 0;
	}
	else
	{
		*num = 0;
		ret = -1;
	}
	cyw43_arch_lwip_end();
	espress_stream_mutex->unlock(this->recv_mutex);
	return ret;
}

static u_t _mtu(struct espress_stream_dgram *this)
{
	return ESPRESS_STREAM_DGRAM_MTU;
}

static void _send_yield(struct espress_stream_dgram *this, struct espress_stream_uint32 const *timeout)
{
	struct espress_stream_event_flags f[1];
	struct espress_stream_uint32 u[1];
	struct espress_stream_uint32 v[1];
	struct pbuf *p;
	int errno;

	espress_stream_tick->get_us(u);
	espress_stream_event->wait(this->send_event, f, timeout);
	if (!espress_stream_event_flags->is_set_any(f))
	{
		goto EXIT;
	}

	cyw43_arch_lwip_begin();
	errno = 0;
	p = pbuf_alloc(PBUF_TRANSPORT, this->send_buffer[0]->data_size, PBUF_RAM);
	if ((NULL != p) && (0 < this->num_send_buffer) && (0 == this->num_sent_buffer))
	{
		memcpy(p->payload, this->send_buffer[0]->data, this->send_buffer[0]->data_size);
		while(ERR_OK != udp_sendto(this->pcb, p, this->send_buffer[0]->address->storage->address, this->send_buffer[0]->address->storage->port))
		{
			cyw43_arch_lwip_end();
#			if PICO_CYW43_ARCH_POLL
				cyw43_arch_poll();
#			endif
			espress_stream_tick->get_us(v);
			espress_stream_uint32_sub(v, u);
			cyw43_arch_lwip_begin();
			if (espress_stream_uint32_le(timeout, v))
			{
				errno = -1;
				break;
			}
			espress_stream_event->wait(this->send_break_event, f, espress_stream_uint32_0);
			if (espress_stream_event_flags->is_set_any(f))
			{
				errno = -1;
				break;
			}
		}
	}
	else
	{
		errno = -1;
	}
	cyw43_arch_lwip_end();
	if (NULL != p)
	{
		pbuf_free(p);
	}

	espress_stream_mutex->lock(this->send_mutex);

	if ((0 < this->num_send_buffer) && (0 == this->num_sent_buffer))
	{
		if (-1 == errno)
		{
			espress_stream_event->set(this->send_event, 1);
		}
		else
		{
			this->num_send_buffer = 0;
			this->num_sent_buffer = (0 == errno) ? 1 : 0;
			this->send_errno = errno;
			espress_stream_event->set(this->send_outer_event, ESPRESS_STREAM_EVENT_DGRAM_SEND_RESULT);
		}
	}
	else
	{
		espress_stream_event->set(this->send_outer_event, ESPRESS_STREAM_EVENT_DGRAM_SEND_RESULT);
	}

	espress_stream_mutex->unlock(this->send_mutex);

EXIT:

	return;
}

static void _recv_yield(struct espress_stream_dgram *this, struct espress_stream_uint32 const *timeout)
{
	struct espress_stream_event_flags f[1];
	struct espress_stream_uint32 u[1];
	struct espress_stream_uint32 v[1];
	int errno;

	espress_stream_tick->get_us(u);
	espress_stream_event->wait(this->recv_event, f, timeout);
	if (!espress_stream_event_flags->is_set_any(f))
	{
		goto EXIT;
	}

	cyw43_arch_lwip_begin();
	errno = 0;
	while (ERR_OK != this->udp_recvfrom)
	{
		cyw43_arch_lwip_end();
#		if PICO_CYW43_ARCH_POLL
			cyw43_arch_poll();
#		endif
		espress_stream_tick->get_us(v);
		espress_stream_uint32_sub(v, u);
		cyw43_arch_lwip_begin();
		if (espress_stream_uint32_le(timeout, v))
		{
			errno = -1;
			break;
		}
		espress_stream_event->wait(this->recv_break_event, f, espress_stream_uint32_0);
		if (espress_stream_event_flags->is_set_any(f))
		{
			errno = -1;
			break;
		}
	}

	espress_stream_mutex->lock(this->recv_mutex);

	if ((0 < this->num_recv_buffer) && (0 == this->num_rcvd_buffer))
	{
		if (-1 == errno)
		{
			espress_stream_event->set(this->recv_event, 1);
		}
		else
		{
			this->num_recv_buffer = 0;
			this->num_rcvd_buffer = (0 == errno) ? 1 : 0;
			this->recv_errno = errno;
			espress_stream_event->set(this->recv_outer_event, ESPRESS_STREAM_EVENT_DGRAM_RECV_RESULT);
			this->udp_recvfrom = -1;
		}
	}
	else
	{
		espress_stream_event->set(this->recv_outer_event, ESPRESS_STREAM_EVENT_DGRAM_RECV_RESULT);
	}

	espress_stream_mutex->unlock(this->recv_mutex);

EXIT:

	return;
}

static void _send_yield_break(struct espress_stream_dgram *this)
{
	espress_stream_event->set(this->send_event, 0);
	espress_stream_event->set(this->send_break_event, 0);
}

static void _recv_yield_break(struct espress_stream_dgram *this)
{
	espress_stream_event->set(this->recv_event, 0);
	espress_stream_event->set(this->recv_break_event, 0);
}
