#include "espress_stream/config.h"
#include "espress_stream/depend.h"
#include "espress_stream/uint.h"
#include "espress_stream/mutex.h"
#include "espress_stream/cond.h"
#include "espress_stream/event.h"
#include "espress_stream/ring.h"
#include "espress_stream/dgram.h"
#include "espress_stream/trans_ring.h"

static void _initialize(struct espress_stream_trans_ring *this, struct espress_stream_dgram_buffer * const *buffer, u_t size, struct espress_stream_dgram *dgram);
static void _finalize(struct espress_stream_trans_ring *this);
static u_t _en(struct espress_stream_trans_ring *this, u_t num);
static u_t _de(struct espress_stream_trans_ring *this, u_t num);
static i_t _is_empty(struct espress_stream_trans_ring *this);
static i_t _is_full(struct espress_stream_trans_ring *this);
static u_t _size(struct espress_stream_trans_ring *this);
static u_t _num_valid(struct espress_stream_trans_ring *this);
static u_t _num_invalid(struct espress_stream_trans_ring *this);
static struct espress_stream_dgram_buffer * const *_head(struct espress_stream_trans_ring *this, u_t offset);
static struct espress_stream_dgram_buffer * const *_tail(struct espress_stream_trans_ring *this, u_t offset);
static i_t _send_request(struct espress_stream_trans_ring *this);
static i_t _send_result(struct espress_stream_trans_ring *this, u_t *num);
static i_t _recv_request(struct espress_stream_trans_ring *this);
static i_t _recv_result(struct espress_stream_trans_ring *this, u_t *num);

struct espress_stream_trans_ring_ops const espress_stream_trans_ring[] =
{{
	_initialize,
	_finalize,
	_en,
	_de,
	_is_empty,
	_is_full,
	_size,
	_num_valid,
	_num_invalid,
	_head,
	_tail,
	_send_request,
	_send_result,
	_recv_request,
	_recv_result
}};

static void _initialize(struct espress_stream_trans_ring *this, struct espress_stream_dgram_buffer * const *buffer, u_t size, struct espress_stream_dgram *dgram)
{
	this->dgram = dgram;
	this->is_sending = 0;
	this->is_recving = 0;

	espress_stream_ring->initialize(this->ring, (void * const *)buffer, size);
}

static void _finalize(struct espress_stream_trans_ring *this)
{
	espress_stream_ring->finalize(this->ring);
	memset(this, 0, sizeof(*this));
}

static u_t _en(struct espress_stream_trans_ring *this, u_t num)
{
	u_t ret;
	ret = espress_stream_ring->en(this->ring, num);
	return ret;
}

static u_t _de(struct espress_stream_trans_ring *this, u_t num)
{
	u_t ret;
	ret = espress_stream_ring->de(this->ring, num);
	return ret;
}

static i_t _is_empty(struct espress_stream_trans_ring *this)
{
	i_t ret;
	ret = espress_stream_ring->is_empty(this->ring);
	return ret;
}

static i_t _is_full(struct espress_stream_trans_ring *this)
{
	i_t ret;
	ret = espress_stream_ring->is_full(this->ring);
	return ret;
}

static u_t _size(struct espress_stream_trans_ring *this)
{
	u_t ret;
	ret = espress_stream_ring->size(this->ring);
	return ret;
}

static u_t _num_valid(struct espress_stream_trans_ring *this)
{
	u_t ret;
	ret = espress_stream_ring->num_valid(this->ring);
	return ret;
}

static u_t _num_invalid(struct espress_stream_trans_ring *this)
{
	u_t ret;
	ret = espress_stream_ring->num_invalid(this->ring);
	return ret;
}

static struct espress_stream_dgram_buffer * const *_head(struct espress_stream_trans_ring *this, u_t offset)
{
	struct espress_stream_dgram_buffer * const *ret;
	ret = (struct espress_stream_dgram_buffer * const *)espress_stream_ring->head(this->ring, offset);
	return ret;
}

static struct espress_stream_dgram_buffer * const *_tail(struct espress_stream_trans_ring *this, u_t offset)
{
	struct espress_stream_dgram_buffer * const *ret;
	ret = (struct espress_stream_dgram_buffer * const *)espress_stream_ring->tail(this->ring, offset);
	return ret;
}

static i_t _request(struct espress_stream_trans_ring *this,
	i_t (*trans_request)(struct espress_stream_dgram *this, struct espress_stream_dgram_buffer * const *buffer, u_t num, struct espress_stream_dgram_buffer * const *ex_buffer, u_t ex_num))
{
	u_t u;
	u_t v;
	u_t w;
	u_t x;
	struct espress_stream_dgram_buffer * const *b;
	struct espress_stream_dgram_buffer * const *p;

	for (u = 0 ;
		(NULL != (b = _tail(this, u))) &&
			(ESPRESS_STREAM_TRANS_RING_INVALID_DATA_SIZE == b[0]->data_size) ;
		u++);
	for (v = u, w = 0, x = 0, p = NULL ;
		(NULL != (b = _tail(this, v))) &&
			(0 < b[0]->data_size) &&
			(ESPRESS_STREAM_TRANS_RING_INVALID_DATA_SIZE != b[0]->data_size) ;
		v++)
	{
		if (p < b)
		{
			w++;
			p = b;
		}
		else
		{
			x++;
		}
	}
	return trans_request(this->dgram, _tail(this, u), w, _tail(this, u + w), x);
}

static i_t _result(struct espress_stream_trans_ring *this,
	i_t (*trans_result)(struct espress_stream_dgram *this, u_t *num),
	u_t *num)
{
	u_t u;
	u_t v;
	struct espress_stream_dgram_buffer * const *b;
	i_t ret;
	for (u = 0 ;
		(NULL != (b = _tail(this, u))) &&
			(ESPRESS_STREAM_TRANS_RING_INVALID_DATA_SIZE == b[0]->data_size) ;
		u++);
	ret = trans_result(this->dgram, num);
	if (0 < ret)
	{
		for (v = 0 ; v < *num ; v++)
		{
			_tail(this, u + v)[0]->data_size = ESPRESS_STREAM_TRANS_RING_INVALID_DATA_SIZE;
		}
	}
	*num += u;
	_de(this, *num);
	return ret;
}

static i_t _send_request(struct espress_stream_trans_ring *this)
{
	i_t ret = 0;
	if (this->is_sending)
	{
		ret = -1;
	}
	else
	{
		this->is_sending = 1;
		ret = _request(this, espress_stream_dgram->send_request);
	}
	return ret;
}

static i_t _send_result(struct espress_stream_trans_ring *this, u_t *num)
{
	i_t ret = 0;
	*num = 0;
	if (this->is_sending)
	{
		ret = _result(this, espress_stream_dgram->send_result, num);
		this->is_sending = 0;
	}
	else
	{
		ret = -1;
	}
	return ret;
}

static i_t _recv_request(struct espress_stream_trans_ring *this)
{
	i_t ret = 0;
	if (this->is_recving)
	{
		ret = -1;
	}
	else
	{
		this->is_recving = 1;
		ret = _request(this, espress_stream_dgram->recv_request);
	}
	return ret;
}

static i_t _recv_result(struct espress_stream_trans_ring *this, u_t *num)
{
	i_t ret = 0;
	*num = 0;
	if (this->is_recving)
	{
		ret = _result(this, espress_stream_dgram->recv_result, num);
		this->is_recving = 0;
	}
	else
	{
		ret = -1;
	}
	return ret;
}
