#include "espress_stream/config.h"
#include "espress_stream/depend.h"
#include "espress_stream/uint.h"
#if MAX_U_T_WIDTH < 4
#	include "espress_stream/mutex.h"
#endif
#include "espress_stream/ring_buffer.h"

static void _initialize(struct espress_stream_ring_buffer *this, b_t *buffer, u_t size);
static void _finalize(struct espress_stream_ring_buffer *this);
static u_t _en(struct espress_stream_ring_buffer *this, u_t num);
static u_t _de(struct espress_stream_ring_buffer *this, u_t num);
static i_t _is_empty(struct espress_stream_ring_buffer *this);
static i_t _is_full(struct espress_stream_ring_buffer *this);
static b_t *_buffer(struct espress_stream_ring_buffer *this);
static u_t _size(struct espress_stream_ring_buffer *this);
static u_t _num_valid(struct espress_stream_ring_buffer *this);
static u_t _num_invalid(struct espress_stream_ring_buffer *this);
static b_t *_head(struct espress_stream_ring_buffer *this, struct espress_stream_uint32 *address, u_t offset);
static b_t *_tail(struct espress_stream_ring_buffer *this, struct espress_stream_uint32 *address, u_t offset);
static b_t *_valid_data(struct espress_stream_ring_buffer *this, struct espress_stream_uint32 const *address);
static b_t *_invalid_data(struct espress_stream_ring_buffer *this, struct espress_stream_uint32 const *address);

struct espress_stream_ring_buffer_ops const espress_stream_ring_buffer[] =
{{
	_initialize,
	_finalize,
	_en,
	_de,
	_is_empty,
	_is_full,
	_buffer,
	_size,
	_num_valid,
	_num_invalid,
	_head,
	_tail,
	_valid_data,
	_invalid_data
}};

#if MAX_U_T_WIDTH < 4
#	define _mutex_initialize(m) ((m)->is_valid = (espress_stream_mutex->initialize((m)->mutex) ? 0 : 1))
#	define _mutex_finalize(m) do{if ((m)->is_valid) espress_stream_mutex->finalize((m)->mutex);}while (0);
#	define _mutex_lock(m) do{if ((m)->is_valid) espress_stream_mutex->lock(m->mutex);}while (0)
#	define _mutex_unlock(m) do{if ((m)->is_valid) espress_stream_mutex->unlock(m->mutex);}while (0)
#else
#	define _mutex_initialize(m) (void)0
#	define _mutex_finalize(m) (void)0
#	define _mutex_lock(m) (void)0
#	define _mutex_unlock(m) (void)0
#endif

static void _initialize(struct espress_stream_ring_buffer *this, b_t *buffer, u_t size)
{
	_mutex_initialize(this->mutex);
	this->buffer = buffer;
	for (this->mask = 1 ; this->mask <= size ; this->mask <<= 1);
	this->mask = (this->mask >> 1) - 1;
	espress_stream_uint32_assign(this->head, espress_stream_uint32_0);
	espress_stream_uint32_assign(this->tail, espress_stream_uint32_0);
}

static void _finalize(struct espress_stream_ring_buffer *this)
{
	_mutex_finalize(this->mutex);
	memset(this, 0, sizeof(*this));
}

static u_t _en(struct espress_stream_ring_buffer *this, u_t num)
{
	struct espress_stream_uint32 n[1];
	if (_num_invalid(this) < num)
	{
		num = _num_invalid(this);
	}
	espress_stream_uint32_set_u(n, num);
	_mutex_lock(this->mutex);
	espress_stream_uint32_add(this->head, n);
	_mutex_unlock(this->mutex);
	return num;
}

static u_t _de(struct espress_stream_ring_buffer *this, u_t num)
{
	struct espress_stream_uint32 n[1];
	if (_num_valid(this) < num)
	{
		num = _num_valid(this);
	}
	espress_stream_uint32_set_u(n, num);
	_mutex_lock(this->mutex);
	espress_stream_uint32_add(this->tail, n);
	_mutex_unlock(this->mutex);
	return num;
}

static i_t _is_empty(struct espress_stream_ring_buffer *this)
{
	return 0 == _num_valid(this);
}

static i_t _is_full(struct espress_stream_ring_buffer *this)
{
	return 0 == _num_invalid(this);
}

static b_t *_buffer(struct espress_stream_ring_buffer *this)
{
	return this->buffer;
}

static u_t _size(struct espress_stream_ring_buffer *this)
{
	return this->mask + 1;
}

static u_t _num_valid(struct espress_stream_ring_buffer *this)
{
	u_t n;
	struct espress_stream_uint32 u[1];
	_mutex_lock(this->mutex);
	espress_stream_uint32_assign(u, this->head);
	espress_stream_uint32_sub(u, this->tail);
	_mutex_unlock(this->mutex);
	espress_stream_uint32_get_u(u, &n);
	return n;
}

static u_t _num_invalid(struct espress_stream_ring_buffer *this)
{
	return _size(this) - _num_valid(this);
}

static b_t *_head(struct espress_stream_ring_buffer *this, struct espress_stream_uint32 *address, u_t offset)
{
	b_t *data;
	struct espress_stream_uint32 u[1];
	struct espress_stream_uint32 m[1];
	espress_stream_uint32_set_u(u, offset);
	_mutex_lock(this->mutex);
	espress_stream_uint32_add(u, this->head);
	_mutex_unlock(this->mutex);
	if (NULL != address)
	{
		espress_stream_uint32_assign(address, u);
	}
	if (_num_invalid(this) <= offset)
	{
		data = NULL;
		goto EXIT;
	}
	espress_stream_uint32_set_u(m, this->mask);
	espress_stream_uint32_and(u, m);
	espress_stream_uint32_get_u(u, &offset);
	data = this->buffer + offset;
EXIT:
	return data;
}

static b_t *_tail(struct espress_stream_ring_buffer *this, struct espress_stream_uint32 *address, u_t offset)
{
	b_t *data;
	struct espress_stream_uint32 u[1];
	struct espress_stream_uint32 m[1];
	espress_stream_uint32_set_u(u, offset);
	_mutex_lock(this->mutex);
	espress_stream_uint32_add(u, this->tail);
	_mutex_unlock(this->mutex);
	if (NULL != address)
	{
		espress_stream_uint32_assign(address, u);
	}
	if (_num_valid(this) <= offset)
	{
		data = NULL;
		goto EXIT;
	}
	espress_stream_uint32_set_u(m, this->mask);
	espress_stream_uint32_and(u, m);
	espress_stream_uint32_get_u(u, &offset);
	data = this->buffer + offset;
EXIT:
	return data;
}

static b_t *_valid_data(struct espress_stream_ring_buffer *this, struct espress_stream_uint32 const *address)
{
	b_t *data = NULL;
	struct espress_stream_uint32 u[1];
	struct espress_stream_uint32 v[1];
	u_t offset;
	espress_stream_uint32_assign(u, address);
	_mutex_lock(this->mutex);
	espress_stream_uint32_sub(u, this->tail);
	_mutex_unlock(this->mutex);
	espress_stream_uint32_set_u(v, _num_valid(this));
	if (espress_stream_uint32_lt(u, v))
	{
		espress_stream_uint32_get_u(u, &offset);
		data = _tail(this, NULL, offset);
	}
	return data;
}

static b_t *_invalid_data(struct espress_stream_ring_buffer *this, struct espress_stream_uint32 const *address)
{
	b_t *data = NULL;
	struct espress_stream_uint32 u[1];
	struct espress_stream_uint32 v[1];
	u_t offset;
	espress_stream_uint32_assign(u, address);
	_mutex_lock(this->mutex);
	espress_stream_uint32_sub(u, this->head);
	_mutex_unlock(this->mutex);
	espress_stream_uint32_set_u(v, _num_invalid(this));
	if (espress_stream_uint32_lt(u, v))
	{
		espress_stream_uint32_get_u(u, &offset);
		data = _head(this, NULL, offset);
	}
	return data;
}
