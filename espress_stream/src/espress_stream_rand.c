#include "espress_stream/config.h"
#include "espress_stream/depend.h"
#include "espress_stream/uint.h"
#include "espress_stream/mutex.h"
#include "espress_stream/tick.h"
#include "espress_stream/rand.h"

static i_t _initialize(void);
static void _finalize(void);
static void _get(b_t *buffer, u_t size);

struct espress_stream_rand_ops const espress_stream_rand[] =
{{
	_initialize,
	_finalize,
	_get
}};

static struct espress_stream_mutex _mutex[1];
static struct espress_stream_uint128 _s0[1];
static struct espress_stream_uint128 _s1[1];
static b_t _buffer[16];
static u_t _size;

static i_t _initialize(void)
{
	struct espress_stream_uint32 tick[1];
	espress_stream_tick->get_us(tick);
	memcpy((b_t *)(_s0) +  0, tick, 4);
	espress_stream_tick->get_us(tick);
	memcpy((b_t *)(_s0) +  4, tick, 4);
	espress_stream_tick->get_us(tick);
	memcpy((b_t *)(_s0) +  8, tick, 4);
	espress_stream_tick->get_us(tick);
	memcpy((b_t *)(_s0) + 12, tick, 4);
	espress_stream_tick->get_us(tick);
	memcpy((b_t *)(_s1) +  0, tick, 4);
	espress_stream_tick->get_us(tick);
	memcpy((b_t *)(_s1) +  4, tick, 4);
	espress_stream_tick->get_us(tick);
	memcpy((b_t *)(_s1) +  8, tick, 4);
	espress_stream_tick->get_us(tick);
	memcpy((b_t *)(_s1) + 12, tick, 4);
	_size = 0;
	return espress_stream_mutex->initialize(_mutex);
}

static void _finalize(void)
{
	espress_stream_mutex->finalize(_mutex);
	_size = 0;
	memset(_buffer, 0, sizeof(_buffer));
}

static void _get(b_t *buffer, u_t size)
{
	b_t *p = buffer;
	struct espress_stream_uint128 u[1];
	struct espress_stream_uint128 s0[1];
	struct espress_stream_uint128 s1[1];

	espress_stream_mutex->lock(_mutex);

	while (0 < size)
	{
		if (size < _size)
		{
			memcpy(p, _buffer + (16 - _size), size);
			_size -= size;
			p += size;
			size = 0;
		}
		else
		{
			memcpy(p, _buffer + (16 - _size), _size);
			p += _size;
			size -= _size;
			_size = 0;

			espress_stream_uint128_assign(s0, _s1);
			espress_stream_uint128_assign(s1, _s0);
			espress_stream_uint128_assign(u, s0);
			espress_stream_uint128_shift(u, -26);
			espress_stream_uint128_xor(s0, u);
			espress_stream_uint128_assign(u, s1);
			espress_stream_uint128_shift(u, 23);
			espress_stream_uint128_xor(s1, u);
			espress_stream_uint128_assign(u, s1);
			espress_stream_uint128_shift(u, -17);
			espress_stream_uint128_xor(s1, u);
			espress_stream_uint128_assign(_s0, _s1);
			espress_stream_uint128_assign(_s1, s0);
			espress_stream_uint128_xor(_s1, s1);
			espress_stream_uint128_assign(u, _s0);
			espress_stream_uint128_add(u, _s1);
			espress_stream_uint128_get(u,
				_buffer +  0,
				_buffer +  1,
				_buffer +  2,
				_buffer +  3,
				_buffer +  4,
				_buffer +  5,
				_buffer +  6,
				_buffer +  7,
				_buffer +  8,
				_buffer +  9,
				_buffer + 10,
				_buffer + 11,
				_buffer + 12,
				_buffer + 13,
				_buffer + 14,
				_buffer + 15);
			_size = 16;
		}
	}

	espress_stream_mutex->unlock(_mutex);
}
