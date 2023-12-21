#include "espress_stream/config.h"
#include "espress_stream/depend.h"
#include "espress_stream/ring.h"

static void _initialize(struct espress_stream_ring *this, void * const *buffer, u_t size);
static void _finalize(struct espress_stream_ring *this);
static u_t _en(struct espress_stream_ring *this, u_t num);
static u_t _de(struct espress_stream_ring *this, u_t num);
static i_t _is_empty(struct espress_stream_ring *this);
static i_t _is_full(struct espress_stream_ring *this);
static u_t _size(struct espress_stream_ring *this);
static u_t _num_valid(struct espress_stream_ring *this);
static u_t _num_invalid(struct espress_stream_ring *this);
static void * const *_head(struct espress_stream_ring *this, u_t offset);
static void * const *_tail(struct espress_stream_ring *this, u_t offset);

struct espress_stream_ring_ops const espress_stream_ring[] =
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
	_tail
}};

static void _initialize(struct espress_stream_ring *this, void * const *buffer, u_t size)
{
	this->buffer = buffer;
	for (this->mask = 1 ; this->mask <= size ; this->mask <<= 1);
	this->mask = (this->mask >> 1) - 1;
	this->head = 0;
	this->tail = 0;
}

static void _finalize(struct espress_stream_ring *this)
{
	memset(this, 0, sizeof(*this));
}

static u_t _en(struct espress_stream_ring *this, u_t num)
{
	if (_num_invalid(this) < num)
	{
		num = _num_invalid(this);
	}
	this->head += num;
	return num;
}

static u_t _de(struct espress_stream_ring *this, u_t num)
{
	if (_num_valid(this) < num)
	{
		num = _num_valid(this);
	}
	this->tail += num;
	return num;
}

static i_t _is_empty(struct espress_stream_ring *this)
{
	return 0 == _num_valid(this);
}

static i_t _is_full(struct espress_stream_ring *this)
{
	return 0 == _num_invalid(this);
}

static u_t _size(struct espress_stream_ring *this)
{
	return this->mask + 1;
}

static u_t _num_valid(struct espress_stream_ring *this)
{
	return this->head - this->tail;
}

static u_t _num_invalid(struct espress_stream_ring *this)
{
	return _size(this) - _num_valid(this);
}

static void * const *_head(struct espress_stream_ring *this, u_t offset)
{
	void * const *data;
	if (_num_invalid(this) <= offset)
	{
		data = NULL;
		goto EXIT;
	}
	data = this->buffer + ((this->head + offset) & this->mask);
EXIT:
	return data;
}

static void * const *_tail(struct espress_stream_ring *this, u_t offset)
{
	void * const *data;
	if (_num_valid(this) <= offset)
	{
		data = NULL;
		goto EXIT;
	}
	data = this->buffer + ((this->tail + offset) & this->mask);
EXIT:
	return data;
}
