#include "espress_stream/config.h"
#include "espress_stream/depend.h"
#include "espress_stream/uint.h"

#define _U_T_BIT_WIDTH (U_T_WIDTH * 8)

#if 64 <= ESPRESS_STREAM_UINT_MAX_WIDTH
#	define _BUFFER_SIZE sizeof(struct espress_stream_uint512)
#elif 32 <= ESPRESS_STREAM_UINT_MAX_WIDTH
#	define _BUFFER_SIZE sizeof(struct espress_stream_uint256)
#elif 16 <= ESPRESS_STREAM_UINT_MAX_WIDTH
#	define _BUFFER_SIZE sizeof(struct espress_stream_uint128)
#elif 8 <= ESPRESS_STREAM_UINT_MAX_WIDTH
#	define _BUFFER_SIZE sizeof(struct espress_stream_uint64)
#elif 4 <= ESPRESS_STREAM_UINT_MAX_WIDTH
#	define _BUFFER_SIZE sizeof(struct espress_stream_uint32)
#elif 2 <= ESPRESS_STREAM_UINT_MAX_WIDTH
#	define _BUFFER_SIZE sizeof(struct espress_stream_uint16)
#elif 1 <= ESPRESS_STREAM_UINT_MAX_WIDTH
#	define _BUFFER_SIZE sizeof(struct espress_stream_uint8)
#else
#error
#endif

static struct
{
	i_t l;
	i_t h;
} const _uint[] =
{
	{0, 0},
#if 2 <= ESPRESS_STREAM_UINT_MAX_WIDTH
#	if 2 <= MAX_U_T_WIDTH
		{1 - ESPRESS_STREAM_IS_HOST_BYTE_ORDER_LITTLE, ESPRESS_STREAM_IS_HOST_BYTE_ORDER_LITTLE},
#	else
		{offsetof(struct espress_stream_uint16, l), offsetof(struct espress_stream_uint16, h)},
#endif
#endif
#if 4 <= ESPRESS_STREAM_UINT_MAX_WIDTH
#	if 4 <= MAX_U_T_WIDTH
		{(1 - ESPRESS_STREAM_IS_HOST_BYTE_ORDER_LITTLE) << 1, ESPRESS_STREAM_IS_HOST_BYTE_ORDER_LITTLE << 1},
#	else
		{offsetof(struct espress_stream_uint32, l), offsetof(struct espress_stream_uint32, h)},
#	endif
#endif
#if 8 <= ESPRESS_STREAM_UINT_MAX_WIDTH
#	if 8 <= MAX_U_T_WIDTH
		{(1 - ESPRESS_STREAM_IS_HOST_BYTE_ORDER_LITTLE) << 2, ESPRESS_STREAM_IS_HOST_BYTE_ORDER_LITTLE << 2},
#	else
		{offsetof(struct espress_stream_uint64, l), offsetof(struct espress_stream_uint64, h)},
#	endif
#endif
#if 16 <= ESPRESS_STREAM_UINT_MAX_WIDTH
#	if 16 <= MAX_U_T_WIDTH
		{(1 - ESPRESS_STREAM_IS_HOST_BYTE_ORDER_LITTLE) << 3, ESPRESS_STREAM_IS_HOST_BYTE_ORDER_LITTLE << 3},
#	else
		{offsetof(struct espress_stream_uint128, l), offsetof(struct espress_stream_uint128, h)},
#	endif
#endif
#if 32 <= ESPRESS_STREAM_UINT_MAX_WIDTH
#	if 32 <= MAX_U_T_WIDTH
		{(1 - ESPRESS_STREAM_IS_HOST_BYTE_ORDER_LITTLE) << 4, ESPRESS_STREAM_IS_HOST_BYTE_ORDER_LITTLE << 4},
#	else
		{offsetof(struct espress_stream_uint256, l), offsetof(struct espress_stream_uint256, h)},
#	endif
#endif
#if 64 <= ESPRESS_STREAM_UINT_MAX_WIDTH
#	if 64 <= MAX_U_T_WIDTH
		{(1 - ESPRESS_STREAM_IS_HOST_BYTE_ORDER_LITTLE) << 5, ESPRESS_STREAM_IS_HOST_BYTE_ORDER_LITTLE << 5},
#	else
		{offsetof(struct espress_stream_uint512, l), offsetof(struct espress_stream_uint512, h)},
#	endif
#endif
};

#if ESPRESS_STREAM_IS_HOST_BYTE_ORDER_LITTLE
#	define _O(w, n) (n)
#else
#	define _O(w, n) ((1 << (w)) - 1 - (n))
#endif

#if 64 <= MAX_U_T_WIDTH
#	define _espress_stream_uint espress_stream_uint512
#	define _np_last(w, this, __func__) do{if (6 == (w)) {espress_stream_uint512_##__func__ (this); goto EXIT;}}while (0)
#	define _last(w, this, __func__, ...) do{if (6 == (w)) {espress_stream_uint512_##__func__ (this, __VA_ARGS__); goto EXIT;}}while (0)
#	define _r_last(r, w, this, __func__, ...) do{if (6 == (w)) {r = espress_stream_uint512_##__func__ (this, __VA_ARGS__); goto EXIT;}}while (0)
#	define _va_last(w, __func__, this, np) do{if (6 == (w)) {espress_stream_uint512_##__func__ (this, np); goto EXIT;}}while (0)
#elif 32 <= MAX_U_T_WIDTH
#	define _espress_stream_uint espress_stream_uint256
#	define _np_last(w, this, __func__) do{if (5 == (w)) {espress_stream_uint256_##__func__ (this); goto EXIT;}}while (0)
#	define _last(w, this, __func__, ...) do{if (5 == (w)) {espress_stream_uint256_##__func__ (this, __VA_ARGS__); goto EXIT;}}while (0)
#	define _r_last(r, w, this, __func__, ...) do{if (5 == (w)) {r = espress_stream_uint256_##__func__ (this, __VA_ARGS__); goto EXIT;}}while (0)
#	define _va_last(w, __func__, this, np) do{if (5 == (w)) {espress_stream_uint256_##__func__ (this, np); goto EXIT;}}while (0)
#elif 16 <= MAX_U_T_WIDTH
#	define _espress_stream_uint espress_stream_uint128
#	define _np_last(w, __func__, this) do{if (4 == (w)) {espress_stream_uint128_##__func__ (this); goto EXIT;}}while (0)
#	define _last(w, __func__, this, ...) do{if (4 == (w)) {espress_stream_uint128_##__func__ (this, __VA_ARGS__); goto EXIT;}}while (0)
#	define _r_last(r, w, __func__, this, ...) do{if (4 == (w)) {r = espress_stream_uint128_##__func__ (this, __VA_ARGS__); goto EXIT;}}while (0)
#	define _va_last(w, __func__, this, np) do{if (4 == (w)) {espress_stream_uint128_##__func__ (this, np); goto EXIT;}}while (0)
#elif 8 <= MAX_U_T_WIDTH
#	define _espress_stream_uint espress_stream_uint64
#	define _np_last(w, __func__, this) do{if (3 == (w)) {espress_stream_uint64_##__func__ (this); goto EXIT;}}while (0)
#	define _last(w, __func__, this, ...) do{if (3 == (w)) {espress_stream_uint64_##__func__ (this, __VA_ARGS__); goto EXIT;}}while (0)
#	define _r_last(r, w, __func__, this, ...) do{if (3 == (w)) {r = espress_stream_uint64_##__func__ (this, __VA_ARGS__); goto EXIT;}}while (0)
#	define _va_last(w, __func__, this, np) do{if (3 == (w)) {espress_stream_uint64_##__func__ (this, np); goto EXIT;}}while (0)
#elif 4 <= MAX_U_T_WIDTH
#	define _espress_stream_uint espress_stream_uint32
#	define _np_last(w, __func__, this) do{if (2 == (w)) {espress_stream_uint32_##__func__ (this); goto EXIT;}}while (0)
#	define _last(w, __func__, this, ...) do{if (2 == (w)) {espress_stream_uint32_##__func__ (this, __VA_ARGS__); goto EXIT;}}while (0)
#	define _r_last(r, w, __func__, this, ...) do{if (2 == (w)) {r = espress_stream_uint32_##__func__ (this, __VA_ARGS__); goto EXIT;}}while (0)
#	define _va_last(w, __func__, this, np) do{if (2 == (w)) {espress_stream_uint32_##__func__ (this, np); goto EXIT;}}while (0)
#elif 2 <= MAX_U_T_WIDTH
#	define _espress_stream_uint espress_stream_uint16
#	define _np_last(w, __func__, this) do{if (1 == (w)) {espress_stream_uint16_##__func__ (this); goto EXIT;}}while (0)
#	define _last(w, __func__, this, ...) do{if (1 == (w)) {espress_stream_uint16_##__func__ (this, __VA_ARGS__); goto EXIT;}}while (0)
#	define _r_last(r, w, __func__, this, ...) do{if (1 == (w)) {r = espress_stream_uint16_##__func__ (this, __VA_ARGS__); goto EXIT;}}while (0)
#	define _va_last(w, __func__, this, np) do{if (1 == (w)) {espress_stream_uint16_##__func__ (this, np); goto EXIT;}}while (0)
#else
#	define _espress_stream_uint espress_stream_uint8
#	define _np_last(w, __func__, this) do{if (0 == (w)) {espress_stream_uint8_##__func__ (this); goto EXIT;}}while (0)
#	define _last(r, w, __func__, this, ...) do{if (0 == (w)) {espress_stream_uint8_##__func__ (this, __VA_ARGS__); goto EXIT;}}while (0)
#	define _r_last(r, w, __func__, this, ...) do{if (0 == (w)) {r = espress_stream_uint8_##__func__ (this, __VA_ARGS__); goto EXIT;}}while (0)
#	define _va_last(w, __func__, this, np) do{if (0 == (w)) {espress_stream_uint8_##__func__ (this, np); goto EXIT;}}while (0)
#endif

static void _uint_set(i_t w, b_t *this, ...);
static void _uint_get(i_t w, b_t const *this, ...);
static void _uint_va_set(i_t w, b_t *this, va_list *np);
static void _uint_va_get(i_t w, b_t const *this, va_list *np);
static void _uint_set_u(i_t w, b_t *this, u_t n);
static void _uint_get_u(i_t w, b_t const *this, u_t *n);
static void _uint_assign(i_t w, b_t *this, b_t const *n);
static i_t _uint_add(i_t w, b_t *this, b_t const *n);
static i_t _uint_sub(i_t w, b_t *this, b_t const *n);
static void _uint_mul(i_t w, b_t *this, b_t const *n);
static void _uint_div(i_t w, b_t *this, b_t const *n);
static void _uint_and(i_t w, b_t *this, b_t const *n);
static void _uint_or(i_t w, b_t *this, b_t const *n);
static void _uint_xor(i_t w, b_t *this, b_t const *n);
static void _uint_not(i_t w, b_t *this);
static void _uint_shift(i_t w, b_t *this, i_t n);
static void _uint_rotate(i_t w, b_t *this, i_t n);
static i_t _uint_eq(i_t w, b_t const *this, b_t const *n);
static i_t _uint_lt(i_t w, b_t const *this, b_t const *n);
static i_t _uint_le(i_t w, b_t const *this, b_t const *n);
static void _uint_bswap(i_t w, b_t *this);

static void _uint_set(i_t w, b_t *this, ...)
{
	va_list np;
	va_start(np, this);
	_uint_va_set(w, this, &np);
	va_end(np);
}

static void _uint_get(i_t w, b_t const *this, ...)
{
	va_list np;
	va_start(np, this);
	_uint_va_get(w, this, &np);
	va_end(np);
}

static void _uint_va_set(i_t w, b_t *this, va_list *np)
{
	i_t const ww = w - 1;
	_va_last(w, va_set, (struct _espress_stream_uint *)this, np);
	_uint_va_set(ww, this + _uint[w].h, np);
	_uint_va_set(ww, this + _uint[w].l, np);
EXIT:
	return;
}

static void _uint_va_get(i_t w, b_t const *this, va_list *np)
{
	i_t const ww = w - 1;
	_va_last(w, va_get, (struct _espress_stream_uint const *)this, np);
	_uint_va_get(ww, this + _uint[w].h, np);
	_uint_va_get(ww, this + _uint[w].l, np);
EXIT:
	return;
}

static void _uint_set_u(i_t w, b_t *this, u_t n)
{
	i_t const ww = w - 1;
	i_t const bww = 1 << (ww + 3);
	_last(w, set_u, (struct _espress_stream_uint *)this, n);
	if (_U_T_BIT_WIDTH <= bww)
	{
		_uint_set_u(ww, this + _uint[w].l, n);
		_uint_set_u(ww, this + _uint[w].h, 0);
	}
	else
	{
		_uint_set_u(ww, this + _uint[w].l, n & (~(u_t)0 >> (_U_T_BIT_WIDTH - bww)));
		_uint_set_u(ww, this + _uint[w].h, (n >> bww) & (~(u_t)0 >> (_U_T_BIT_WIDTH - bww)));
	}
EXIT:
	return;
}

static void _uint_get_u(i_t w, b_t const *this, u_t *n)
{
	i_t const ww = w - 1;
	i_t const bww = 1 << (ww + 3);
	u_t l;
	u_t h;
	_last(w, get_u, (struct _espress_stream_uint const *)this, n);
	_uint_get_u(ww, this + _uint[w].l, &l);
	_uint_get_u(ww, this + _uint[w].h, &h);
	if (_U_T_BIT_WIDTH <= bww)
	{
		*n = l;
	}
	else
	{
		*n = l + (h << bww);
	}
EXIT:
	return;
}

static void _uint_assign(i_t w, b_t *this, b_t const *n)
{
	i_t const ww = w - 1;
	_last(w, assign, (struct _espress_stream_uint *)this, (struct _espress_stream_uint const *)n);
	_uint_assign(ww, this + _uint[w].l, n + _uint[w].l);
	_uint_assign(ww, this + _uint[w].h, n + _uint[w].h);
EXIT:
	return;
}

static i_t _uint_add(i_t w, b_t *this, b_t const *n)
{
	i_t const ww = w - 1;
	i_t is_of = 0;
	b_t one[_BUFFER_SIZE];
	_r_last(is_of, w, add, (struct _espress_stream_uint *)this, (struct _espress_stream_uint const *)n);
	if (_uint_add(ww, this + _uint[w].l,  n + _uint[w].l))
	{
		_uint_set_u(ww, one, 1);
		if (_uint_add(ww, this + _uint[w].h, one))
		{
			is_of = 1;
		}
	}
	if (_uint_add(ww, this + _uint[w].h, n + _uint[w].h))
	{
		is_of = 1;
	}
EXIT:
	return is_of;
}

static i_t _uint_sub(i_t w, b_t *this, b_t const *n)
{
	i_t const ww = w - 1;
	i_t is_uf = 0;
	b_t one[_BUFFER_SIZE];
	_r_last(is_uf, w, sub, (struct _espress_stream_uint *)this, (struct _espress_stream_uint const *)n);
	if (_uint_sub(ww, this + _uint[w].l, n + _uint[w].l))
	{
		_uint_set_u(ww, one, 1);
		if (_uint_sub(ww, this + _uint[w].h, one))
		{
			is_uf = 1;
		}
	}
	if (_uint_sub(ww, this + _uint[w].h, n + _uint[w].h))
	{
		is_uf = 1;
	}
EXIT:
	return is_uf;
}

static void __uint_mul(i_t w, b_t *this, b_t const *n)
{
	i_t i;
	i_t j;
	i_t const ww = w - 1;
	b_t b[_BUFFER_SIZE];
	b_t b0[_BUFFER_SIZE];
	b_t b1[_BUFFER_SIZE];
	b_t r[_BUFFER_SIZE] = {0};
	i_t l;
	i_t h;
	i_t lhlh[4];
	_last(w, mul, (struct _espress_stream_uint *)this, (struct _espress_stream_uint const *)n);
	l = _uint[w].l;
	h = _uint[w].h;
	lhlh[0] = l + _uint[ww].l;
	lhlh[1] = l + _uint[ww].h;
	lhlh[2] = h + _uint[ww].l;
	lhlh[3] = h + _uint[ww].h;
#	if 1
		memcpy(b + lhlh[0], this + lhlh[0], 1 << (ww - 1));
		memset(b + lhlh[1], 0, 1 << (ww - 1));
		memcpy(b + lhlh[2], this + lhlh[1], 1 << (ww - 1));
		memset(b + lhlh[3], 0, 1 << (ww - 1));
		if (_uint_lt(ww, b + l, b + h))
		{
			_uint_sub(ww, b + h, b + l);
			i = 0;
		}
		else
		{
			_uint_sub(ww, b + l, b + h);
			i = 1;
		}
		memcpy(b0 + lhlh[0], n + lhlh[0], 1 << (ww - 1));
		memset(b0 + lhlh[1], 0, 1 << (ww - 1));
		memcpy(b0 + lhlh[2], n + lhlh[1], 1 << (ww - 1));
		memset(b0 + lhlh[3], 0, 1 << (ww - 1));
		if (_uint_lt(ww, b0 + l, b0 + h))
		{
			_uint_sub(ww, b0 + h, b0 + l);
			j = 0;
		}
		else
		{
			_uint_sub(ww, b0 + l, b0 + h);
			j = 1;
		}
		__uint_mul(ww, i ? b + l : b + h, j ? b0 + l : b0 + h);
		if (!i)
		{
			memcpy(b + l, b + h, 1 << ww);
		}
		memset(b + h, 0, 1 << ww);

		memcpy(r + lhlh[0], this + lhlh[0], 1 << (ww - 1));
		memset(r + lhlh[1], 0, 1 << (ww - 1));
		memcpy(b0 + lhlh[2], n + lhlh[0], 1 << (ww - 1));
		memset(b0 + lhlh[3], 0, 1 << (ww - 1));
		__uint_mul(ww, r + l, b0 + h);
		memset(r + h, 0, 1 << ww);

		memcpy(b1 + lhlh[0], this + lhlh[1], 1 << (ww - 1));
		memset(b1 + lhlh[1], 0, 1 << (ww - 1));
		memcpy(b1 + lhlh[2], n + lhlh[1], 1 << (ww - 1));
		memset(b1 + lhlh[3], 0, 1 << (ww - 1));
		__uint_mul(ww, b1 + l, b1 + h);
		memset(b1 + h, 0, 1 << ww);

		_uint_assign(w, b0, r);
		_uint_add(w, b0, b1);
		if (i ^ j)
		{
			_uint_add(w, b0, b);
		}
		else
		{
			_uint_sub(w, b0, b);
		}

		_uint_shift(w, b0, 1 << (ww + 2));
		_uint_add(w, r, b0);
		_uint_shift(w, b1, 1 << (ww + 3));
		_uint_add(w, r, b1);
#	else
		for (i = 0 ; i < 2 ; i++)
		{
			for (j = 0 ; j < 2 ; j++)
			{
				memcpy(b + lhlh[0], this + lhlh[i], 1 << (ww - 1));
				memset(b + lhlh[1], 0, 1 << (ww - 1));
				memcpy(b + lhlh[2], n + lhlh[j], 1 << (ww - 1));
				memset(b + lhlh[3], 0, 1 << (ww - 1));
				__uint_mul(ww, b + l, b + h);
				memset(b + h, 0, 1 << ww);
				_uint_shift(w, b, (1 << (ww + 2)) * (i + j));
				_uint_add(w, r, b);
			}
		}
#	endif
	_uint_assign(w, this, r);
EXIT:
	return;
}

static void _uint_mul(i_t w, b_t *this, b_t const *n)
{
	i_t i;
	i_t j;
	i_t k;
	i_t const ww = w - 1;
	b_t b[_BUFFER_SIZE];
	b_t r[_BUFFER_SIZE] = {0};
	i_t l;
	i_t h;
	i_t lhlh[4];
	_last(w, mul, (struct _espress_stream_uint *)this, (struct _espress_stream_uint const *)n);
	l = _uint[w].l;
	h = _uint[w].h;
	lhlh[0] = l + _uint[ww].l;
	lhlh[1] = l + _uint[ww].h;
	lhlh[2] = h + _uint[ww].l;
	lhlh[3] = h + _uint[ww].h;
	for (i = 0, k = 4 ; i < 4 ; i++, k--)
	{
		for (j = 0 ; j < k ; j++)
		{
			memcpy(b + lhlh[0], this + lhlh[i], 1 << (ww - 1));
			memset(b + lhlh[1], 0, 1 << (ww - 1));
			memcpy(b + lhlh[2], n + lhlh[j], 1 << (ww - 1));
			memset(b + lhlh[3], 0, 1 << (ww - 1));
			__uint_mul(ww, b + l, b + h);
			memset(b + h, 0, 1 << ww);
			_uint_shift(w, b, (1 << (ww + 2)) * (i + j));
			_uint_add(w, r, b);
		}
	}
	_uint_assign(w, this, r);
EXIT:
	return;
}

static void _uint_div(i_t w, b_t *this, b_t const *n)
{
	i_t i;
	u_t u;
	i_t const bw = 1 << (w + 3);
	b_t b[_BUFFER_SIZE];
	b_t r[_BUFFER_SIZE] = {0};
	b_t t[_BUFFER_SIZE];
	_uint_assign(w, b, n);
	for (i = 0 ; i < bw ; i++)
	{
		_uint_get_u(w, this, &u);
		if (0 != (u & 1))
		{
			break;
		}
		_uint_get_u(w, b, &u);
		if (0 != (u & 1))
		{
			break;
		}
		_uint_shift(w, this, -1);
		_uint_shift(w, b, -1);
	}
	for (i = 0 ; i < bw ; i++)
	{
		_uint_rotate(w, b, 1);
		_uint_get_u(w, b, &u);
		if (0 != (u & 1))
		{
			_uint_rotate(w, b, -1);
			break;
		}
	}
	_uint_set_u(w, t, 1);
	_uint_shift(w, t, i);
	for ( ; 0 <= i ; i--)
	{
		if (_uint_le(w, b, this))
		{
			_uint_or(w, r, t);
			_uint_sub(w, this, b);
		}
		_uint_shift(w, b, -1);
		_uint_shift(w, t, -1);
	}
	_uint_assign(w, this, r);
}

static void _uint_and(i_t w, b_t *this, b_t const *n)
{
	i_t const ww = w - 1;
	_last(w, and, (struct _espress_stream_uint *)this, (struct _espress_stream_uint const *)n);
	_uint_and(ww, this + _uint[w].l, n + _uint[w].l);
	_uint_and(ww, this + _uint[w].h, n + _uint[w].h);
EXIT:
	return;
}

static void _uint_or(i_t w, b_t *this, b_t const *n)
{
	i_t const ww = w - 1;
	_last(w, or, (struct _espress_stream_uint *)this, (struct _espress_stream_uint const *)n);
	_uint_or(ww, this + _uint[w].l, n + _uint[w].l);
	_uint_or(ww, this + _uint[w].h, n + _uint[w].h);
EXIT:
	return;
}

static void _uint_xor(i_t w, b_t *this, b_t const *n)
{
	i_t const ww = w - 1;
	_last(w, xor, (struct _espress_stream_uint *)this, (struct _espress_stream_uint const *)n);
	_uint_xor(ww, this + _uint[w].l, n + _uint[w].l);
	_uint_xor(ww, this + _uint[w].h, n + _uint[w].h);
EXIT:
	return;
}

static void _uint_not(i_t w, b_t *this)
{
	i_t const ww = w - 1;
	_np_last(w, not, (struct _espress_stream_uint *)this);
	_uint_not(ww, this + _uint[w].l);
	_uint_not(ww, this + _uint[w].h);
EXIT:
	return;
}

static void _uint_shift(i_t w, b_t *this, i_t n)
{
	i_t const ww = w - 1;
	i_t const bww = 1 << (ww + 3);
	i_t m = n % bww;
	b_t u[_BUFFER_SIZE];
	_last(w, shift, (struct _espress_stream_uint *)this, n);
	if (m < 0)
	{
		_uint_assign(ww, u, this + _uint[w].h);
		_uint_shift(ww, u, bww + m);
		_uint_shift(ww, this + _uint[w].l, m);
		_uint_or(ww, this + _uint[w].l, u);
		_uint_shift(ww, this + _uint[w].h, m);
	}
	else if (0 < m)
	{
		_uint_assign(ww, u, this + _uint[w].l);
		_uint_shift(ww, u, m - bww);
		_uint_shift(ww, this + _uint[w].l, m);
		_uint_shift(ww, this + _uint[w].h, m);
		_uint_or(ww, this + _uint[w].h, u);
	}
	n = (n - m) % (bww << 1);
	if (n < 0)
	{
		_uint_assign(ww, this + _uint[w].l, this + _uint[w].h);
		_uint_set_u(ww, this + _uint[w].h, 0);
	}
	else if (0 < n)
	{
		_uint_assign(ww, this + _uint[w].h, this + _uint[w].l);
		_uint_set_u(ww, this + _uint[w].l, 0);
	}
EXIT:
	return;
}

static void _uint_rotate(i_t w, b_t *this, i_t n)
{
	i_t const bw = 1 << (w + 3);
	b_t u[_BUFFER_SIZE];
	n %= bw;
	if (0 != n)
	{
		_uint_assign(w, u, this);
		_uint_shift(w, u, (n < 0) ? (bw + n) : (n - bw));
		_uint_shift(w, this, n);
		_uint_or(w, this, u);
	}
}

static i_t _uint_eq(i_t w, b_t const *this, b_t const *n)
{
	i_t const ww = w - 1;
	i_t ret;
	_r_last(ret, w, eq, (struct _espress_stream_uint const *)this, (struct _espress_stream_uint const *)n);
	ret = _uint_eq(ww, this + _uint[w].l, n + _uint[w].l) && _uint_eq(ww, this + _uint[w].h, n + _uint[w].h);
EXIT:
	return ret;
}

static i_t _uint_lt(i_t w, b_t const *this, b_t const *n)
{
	i_t const ww = w - 1;
	i_t ret;
	_r_last(ret, w, lt, (struct _espress_stream_uint const *)this, (struct _espress_stream_uint const *)n);
	ret = _uint_lt(ww, this + _uint[w].h, n + _uint[w].h) || (_uint_eq(ww, this + _uint[w].h, n + _uint[w].h) && _uint_lt(ww, this + _uint[w].l, n + _uint[w].l));
EXIT:
	return ret;
}

static i_t _uint_le(i_t w, b_t const *this, b_t const *n)
{
	return !_uint_lt(w, n, this);
}

static void _uint_bswap(i_t w, b_t *this)
{
	i_t const ww = w - 1;
	i_t const bww = 1 << (ww + 3);
	_np_last(w, bswap, (struct _espress_stream_uint *)this);
	_uint_bswap(ww, this + _uint[w].l);
	_uint_bswap(ww, this + _uint[w].h);
	_uint_rotate(w, this, bww);
EXIT:
	return;
}

struct espress_stream_uint8 const espress_stream_uint8_0[] = {ESPRESS_STREAM_UINT8_0};
struct espress_stream_uint8 const espress_stream_uint8_1[] = {ESPRESS_STREAM_UINT8_1};
struct espress_stream_uint8 const espress_stream_uint8_max[] = {ESPRESS_STREAM_UINT8_MAX};

#if 2 <= ESPRESS_STREAM_UINT_MAX_WIDTH
	struct espress_stream_uint16 const espress_stream_uint16_0[] = {ESPRESS_STREAM_UINT16_0};
	struct espress_stream_uint16 const espress_stream_uint16_1[] = {ESPRESS_STREAM_UINT16_1};
	struct espress_stream_uint16 const espress_stream_uint16_max[] = {ESPRESS_STREAM_UINT16_MAX};
#	if 2 <= MAX_U_T_WIDTH
#	else
		void espress_stream_uint16_set(struct espress_stream_uint16 *this, b_t n1, b_t n0) {_uint_set(1, (b_t *)this, n1, n0);}
		void espress_stream_uint16_get(struct espress_stream_uint16 const *this, b_t *n1, b_t *n0) {_uint_get(1, (b_t const *)this, n1, n0);}
		void espress_stream_uint16_va_set(struct espress_stream_uint16 *this, va_list *np) {_uint_va_set(1, (b_t *)this, np);}
		void espress_stream_uint16_va_get(struct espress_stream_uint16 const *this, va_list *np) {_uint_va_get(1, (b_t const *)this, np);}
		void espress_stream_uint16_set_u(struct espress_stream_uint16 *this, u_t n) {_uint_set_u(1, (b_t *)this, n);}
		void espress_stream_uint16_get_u(struct espress_stream_uint16 const *this, u_t *n) {_uint_get_u(1, (b_t const *)this, n);}
		void espress_stream_uint16_assign(struct espress_stream_uint16 *this, struct espress_stream_uint16 const *n) {_uint_assign(1, (b_t *)this, (b_t const *)n);}
		i_t espress_stream_uint16_add(struct espress_stream_uint16 *this, struct espress_stream_uint16 const *n) {return _uint_add(1, (b_t *)this, (b_t const *)n);}
		i_t espress_stream_uint16_sub(struct espress_stream_uint16 *this, struct espress_stream_uint16 const *n) {return _uint_sub(1, (b_t *)this, (b_t const *)n);}
		void espress_stream_uint16_mul(struct espress_stream_uint16 *this, struct espress_stream_uint16 const *n) {_uint_mul(1, (b_t *)this, (b_t const *)n);}
		void espress_stream_uint16_div(struct espress_stream_uint16 *this, struct espress_stream_uint16 const *n) {_uint_div(1, (b_t *)this, (b_t const *)n);}
		void espress_stream_uint16_and(struct espress_stream_uint16 *this, struct espress_stream_uint16 const *n) {_uint_and(1, (b_t *)this, (b_t const *)n);}
		void espress_stream_uint16_or(struct espress_stream_uint16 *this, struct espress_stream_uint16 const *n) {_uint_or(1, (b_t *)this, (b_t const *)n);}
		void espress_stream_uint16_xor(struct espress_stream_uint16 *this, struct espress_stream_uint16 const *n) {_uint_xor(1, (b_t *)this, (b_t const *)n);}
		void espress_stream_uint16_not(struct espress_stream_uint16 *this) {_uint_not(1, (b_t *)this);}
		void espress_stream_uint16_shift(struct espress_stream_uint16 *this, i_t n) {_uint_shift(1, (b_t *)this, n);}
		void espress_stream_uint16_rotate(struct espress_stream_uint16 *this, i_t n) {_uint_rotate(1, (b_t *)this, n);}
		i_t espress_stream_uint16_eq(struct espress_stream_uint16 const *this, struct espress_stream_uint16 const *n) {return _uint_eq(1, (b_t const *)this, (b_t const *)n);}
		i_t espress_stream_uint16_lt(struct espress_stream_uint16 const *this, struct espress_stream_uint16 const *n) {return _uint_lt(1, (b_t const *)this, (b_t const *)n);}
		i_t espress_stream_uint16_le(struct espress_stream_uint16 const *this, struct espress_stream_uint16 const *n) {return _uint_le(1, (b_t const *)this, (b_t const *)n);}
		void espress_stream_uint16_bswap(struct espress_stream_uint16 *this) {_uint_bswap(1, (b_t *)this);}
#	endif
#endif

#if 4 <= ESPRESS_STREAM_UINT_MAX_WIDTH
	struct espress_stream_uint32 const espress_stream_uint32_0[] = {ESPRESS_STREAM_UINT32_0};
	struct espress_stream_uint32 const espress_stream_uint32_1[] = {ESPRESS_STREAM_UINT32_1};
	struct espress_stream_uint32 const espress_stream_uint32_max[] = {ESPRESS_STREAM_UINT32_MAX};
#	if 4 <= MAX_U_T_WIDTH
#	else
		void espress_stream_uint32_set(struct espress_stream_uint32 *this, b_t n3, b_t n2, b_t n1, b_t n0) {_uint_set(2, (b_t *)this, n3, n2, n1, n0);}
		void espress_stream_uint32_get(struct espress_stream_uint32 const *this, b_t *n3, b_t *n2, b_t *n1, b_t *n0) {_uint_get(2, (b_t const *)this, n3, n2, n1, n0);}
		void espress_stream_uint32_va_set(struct espress_stream_uint32 *this, va_list *np) {_uint_va_set(2, (b_t *)this, np);}
		void espress_stream_uint32_va_get(struct espress_stream_uint32 const *this, va_list *np) {_uint_va_get(2, (b_t const *)this, np);}
		void espress_stream_uint32_set_u(struct espress_stream_uint32 *this, u_t n) {_uint_set_u(2, (b_t *)this, n);}
		void espress_stream_uint32_get_u(struct espress_stream_uint32 const *this, u_t *n) {_uint_get_u(2, (b_t const *)this, n);}
		void espress_stream_uint32_assign(struct espress_stream_uint32 *this, struct espress_stream_uint32 const *n) {_uint_assign(2, (b_t *)this, (b_t const *)n);}
		i_t espress_stream_uint32_add(struct espress_stream_uint32 *this, struct espress_stream_uint32 const *n) {return _uint_add(2, (b_t *)this, (b_t const *)n);}
		i_t espress_stream_uint32_sub(struct espress_stream_uint32 *this, struct espress_stream_uint32 const *n) {return _uint_sub(2, (b_t *)this, (b_t const *)n);}
		void espress_stream_uint32_mul(struct espress_stream_uint32 *this, struct espress_stream_uint32 const *n) {_uint_mul(2, (b_t *)this, (b_t const *)n);}
		void espress_stream_uint32_div(struct espress_stream_uint32 *this, struct espress_stream_uint32 const *n) {_uint_div(2, (b_t *)this, (b_t const *)n);}
		void espress_stream_uint32_and(struct espress_stream_uint32 *this, struct espress_stream_uint32 const *n) {_uint_and(2, (b_t *)this, (b_t const *)n);}
		void espress_stream_uint32_or(struct espress_stream_uint32 *this, struct espress_stream_uint32 const *n) {_uint_or(2, (b_t *)this, (b_t const *)n);}
		void espress_stream_uint32_xor(struct espress_stream_uint32 *this, struct espress_stream_uint32 const *n) {_uint_xor(2, (b_t *)this, (b_t const *)n);}
		void espress_stream_uint32_not(struct espress_stream_uint32 *this) {_uint_not(2, (b_t *)this);}
		void espress_stream_uint32_shift(struct espress_stream_uint32 *this, i_t n) {_uint_shift(2, (b_t *)this, n);}
		void espress_stream_uint32_rotate(struct espress_stream_uint32 *this, i_t n) {_uint_rotate(2, (b_t *)this, n);}
		i_t espress_stream_uint32_eq(struct espress_stream_uint32 const *this, struct espress_stream_uint32 const *n) {return _uint_eq(2, (b_t const *)this, (b_t const *)n);}
		i_t espress_stream_uint32_lt(struct espress_stream_uint32 const *this, struct espress_stream_uint32 const *n) {return _uint_lt(2, (b_t const *)this, (b_t const *)n);}
		i_t espress_stream_uint32_le(struct espress_stream_uint32 const *this, struct espress_stream_uint32 const *n) {return _uint_le(2, (b_t const *)this, (b_t const *)n);}
		void espress_stream_uint32_bswap(struct espress_stream_uint32 *this) {_uint_bswap(2, (b_t *)this);}
#	endif
#endif

#if 8 <= ESPRESS_STREAM_UINT_MAX_WIDTH
	struct espress_stream_uint64 const espress_stream_uint64_0[] = {ESPRESS_STREAM_UINT64_0};
	struct espress_stream_uint64 const espress_stream_uint64_1[] = {ESPRESS_STREAM_UINT64_1};
	struct espress_stream_uint64 const espress_stream_uint64_max[] = {ESPRESS_STREAM_UINT64_MAX};
#	if 8 <= MAX_U_T_WIDTH
#	else
		void espress_stream_uint64_set(struct espress_stream_uint64 *this, b_t n7, b_t n6, b_t n5, b_t n4, b_t n3, b_t n2, b_t n1, b_t n0) {_uint_set(3, (b_t *)this, n7, n6, n5, n4, n3, n2, n1, n0);}
		void espress_stream_uint64_get(struct espress_stream_uint64 const *this, b_t *n7, b_t *n6, b_t *n5, b_t *n4, b_t *n3, b_t *n2, b_t *n1, b_t *n0) {_uint_get(3, (b_t const *)this, n7, n6, n5, n4, n3, n2, n1, n0);}
		void espress_stream_uint64_va_set(struct espress_stream_uint64 *this, va_list *np) {_uint_va_set(3, (b_t *)this, np);}
		void espress_stream_uint64_va_get(struct espress_stream_uint64 const *this, va_list *np) {_uint_va_get(3, (b_t const *)this, np);}
		void espress_stream_uint64_set_u(struct espress_stream_uint64 *this, u_t n) {_uint_set_u(3, (b_t *)this, n);}
		void espress_stream_uint64_get_u(struct espress_stream_uint64 const *this, u_t *n) {_uint_get_u(3, (b_t const *)this, n);}
		void espress_stream_uint64_assign(struct espress_stream_uint64 *this, struct espress_stream_uint64 const *n) {_uint_assign(3, (b_t *)this, (b_t const *)n);}
		i_t espress_stream_uint64_add(struct espress_stream_uint64 *this, struct espress_stream_uint64 const *n) {return _uint_add(3, (b_t *)this, (b_t const *)n);}
		i_t espress_stream_uint64_sub(struct espress_stream_uint64 *this, struct espress_stream_uint64 const *n) {return _uint_sub(3, (b_t *)this, (b_t const *)n);}
		void espress_stream_uint64_mul(struct espress_stream_uint64 *this, struct espress_stream_uint64 const *n) {_uint_mul(3, (b_t *)this, (b_t const *)n);}
		void espress_stream_uint64_div(struct espress_stream_uint64 *this, struct espress_stream_uint64 const *n) {_uint_div(3, (b_t *)this, (b_t const *)n);}
		void espress_stream_uint64_and(struct espress_stream_uint64 *this, struct espress_stream_uint64 const *n) {_uint_and(3, (b_t *)this, (b_t const *)n);}
		void espress_stream_uint64_or(struct espress_stream_uint64 *this, struct espress_stream_uint64 const *n) {_uint_or(3, (b_t *)this, (b_t const *)n);}
		void espress_stream_uint64_xor(struct espress_stream_uint64 *this, struct espress_stream_uint64 const *n) {_uint_xor(3, (b_t *)this, (b_t const *)n);}
		void espress_stream_uint64_not(struct espress_stream_uint64 *this) {_uint_not(3, (b_t *)this);}
		void espress_stream_uint64_shift(struct espress_stream_uint64 *this, i_t n) {_uint_shift(3, (b_t *)this, n);}
		void espress_stream_uint64_rotate(struct espress_stream_uint64 *this, i_t n) {_uint_rotate(3, (b_t *)this, n);}
		i_t espress_stream_uint64_eq(struct espress_stream_uint64 const *this, struct espress_stream_uint64 const *n) {return _uint_eq(3, (b_t const *)this, (b_t const *)n);}
		i_t espress_stream_uint64_lt(struct espress_stream_uint64 const *this, struct espress_stream_uint64 const *n) {return _uint_lt(3, (b_t const *)this, (b_t const *)n);}
		i_t espress_stream_uint64_le(struct espress_stream_uint64 const *this, struct espress_stream_uint64 const *n) {return _uint_le(3, (b_t const *)this, (b_t const *)n);}
		void espress_stream_uint64_bswap(struct espress_stream_uint64 *this) {_uint_bswap(3, (b_t *)this);}
#	endif
#endif

#if 16 <= ESPRESS_STREAM_UINT_MAX_WIDTH
	struct espress_stream_uint128 const espress_stream_uint128_0[] = {ESPRESS_STREAM_UINT128_0};
	struct espress_stream_uint128 const espress_stream_uint128_1[] = {ESPRESS_STREAM_UINT128_1};
	struct espress_stream_uint128 const espress_stream_uint128_max[] = {ESPRESS_STREAM_UINT128_MAX};
#	if 16 <= MAX_U_T_WIDTH
#	else
		void espress_stream_uint128_set(struct espress_stream_uint128 *this,
			b_t n15, b_t n14, b_t n13, b_t n12, b_t n11, b_t n10, b_t n9, b_t n8,
			b_t n7, b_t n6, b_t n5, b_t n4, b_t n3, b_t n2, b_t n1, b_t n0)
		{
			_uint_set(4, (b_t *)this, n15, n14, n13, n12, n11, n10, n9, n8, n7, n6, n5, n4, n3, n2, n1, n0);
		}
		void espress_stream_uint128_get(struct espress_stream_uint128 const *this,
			b_t *n15, b_t *n14, b_t *n13, b_t *n12, b_t *n11, b_t *n10, b_t *n9, b_t *n8,
			b_t *n7, b_t *n6, b_t *n5, b_t *n4, b_t *n3, b_t *n2, b_t *n1, b_t *n0)
		{
			_uint_get(4, (b_t const *)this, n15, n14, n13, n12, n11, n10, n9, n8, n7, n6, n5, n4, n3, n2, n1, n0);
		}
		void espress_stream_uint128_va_set(struct espress_stream_uint128 *this, va_list *np) {_uint_va_set(4, (b_t *)this, np);}
		void espress_stream_uint128_va_get(struct espress_stream_uint128 const *this, va_list *np) {_uint_va_get(4, (b_t const *)this, np);}
		void espress_stream_uint128_set_u(struct espress_stream_uint128 *this, u_t n) {_uint_set_u(4, (b_t *)this, n);}
		void espress_stream_uint128_get_u(struct espress_stream_uint128 const *this, u_t *n) {_uint_get_u(4, (b_t const *)this, n);}
		void espress_stream_uint128_assign(struct espress_stream_uint128 *this, struct espress_stream_uint128 const *n) {_uint_assign(4, (b_t *)this, (b_t const *)n);}
		i_t espress_stream_uint128_add(struct espress_stream_uint128 *this, struct espress_stream_uint128 const *n) {return _uint_add(4, (b_t *)this, (b_t const *)n);}
		i_t espress_stream_uint128_sub(struct espress_stream_uint128 *this, struct espress_stream_uint128 const *n) {return _uint_sub(4, (b_t *)this, (b_t const *)n);}
		void espress_stream_uint128_mul(struct espress_stream_uint128 *this, struct espress_stream_uint128 const *n) {_uint_mul(4, (b_t *)this, (b_t const *)n);}
		void espress_stream_uint128_div(struct espress_stream_uint128 *this, struct espress_stream_uint128 const *n) {_uint_div(4, (b_t *)this, (b_t const *)n);}
		void espress_stream_uint128_and(struct espress_stream_uint128 *this, struct espress_stream_uint128 const *n) {_uint_and(4, (b_t *)this, (b_t const *)n);}
		void espress_stream_uint128_or(struct espress_stream_uint128 *this, struct espress_stream_uint128 const *n) {_uint_or(4, (b_t *)this, (b_t const *)n);}
		void espress_stream_uint128_xor(struct espress_stream_uint128 *this, struct espress_stream_uint128 const *n) {_uint_xor(4, (b_t *)this, (b_t const *)n);}
		void espress_stream_uint128_not(struct espress_stream_uint128 *this) {_uint_not(4, (b_t *)this);}
		void espress_stream_uint128_shift(struct espress_stream_uint128 *this, i_t n) {_uint_shift(4, (b_t *)this, n);}
		void espress_stream_uint128_rotate(struct espress_stream_uint128 *this, i_t n) {_uint_rotate(4, (b_t *)this, n);}
		i_t espress_stream_uint128_eq(struct espress_stream_uint128 const *this, struct espress_stream_uint128 const *n) {return _uint_eq(4, (b_t const *)this, (b_t const *)n);}
		i_t espress_stream_uint128_lt(struct espress_stream_uint128 const *this, struct espress_stream_uint128 const *n) {return _uint_lt(4, (b_t const *)this, (b_t const *)n);}
		i_t espress_stream_uint128_le(struct espress_stream_uint128 const *this, struct espress_stream_uint128 const *n) {return _uint_le(4, (b_t const *)this, (b_t const *)n);}
		void espress_stream_uint128_bswap(struct espress_stream_uint128 *this) {_uint_bswap(4, (b_t *)this);}
#	endif
#endif

#if 32 <= ESPRESS_STREAM_UINT_MAX_WIDTH
	struct espress_stream_uint256 const espress_stream_uint256_0[] = {ESPRESS_STREAM_UINT256_0};
	struct espress_stream_uint256 const espress_stream_uint256_1[] = {ESPRESS_STREAM_UINT256_1};
	struct espress_stream_uint256 const espress_stream_uint256_max[] = {ESPRESS_STREAM_UINT256_MAX};
#	if 32 <= MAX_U_T_WIDTH
#	else
		void espress_stream_uint256_set(struct espress_stream_uint256 *this,
			b_t n31, b_t n30, b_t n29, b_t n28, b_t n27, b_t n26, b_t n25, b_t n24,
			b_t n23, b_t n22, b_t n21, b_t n20, b_t n19, b_t n18, b_t n17, b_t n16,
			b_t n15, b_t n14, b_t n13, b_t n12, b_t n11, b_t n10, b_t n9, b_t n8,
			b_t n7, b_t n6, b_t n5, b_t n4, b_t n3, b_t n2, b_t n1, b_t n0)
		{
			_uint_set(5, (b_t *)this, n31, n30, n29, n28, n27, n26, n25, n24, n23, n22, n21, n20, n19, n18, n17, n16, n15, n14, n13, n12, n11, n10, n9, n8, n7, n6, n5, n4, n3, n2, n1, n0);
		}
		void espress_stream_uint256_get(struct espress_stream_uint256 const *this,
			b_t *n31, b_t *n30, b_t *n29, b_t *n28, b_t *n27, b_t *n26, b_t *n25, b_t *n24,
			b_t *n23, b_t *n22, b_t *n21, b_t *n20, b_t *n19, b_t *n18, b_t *n17, b_t *n16,
			b_t *n15, b_t *n14, b_t *n13, b_t *n12, b_t *n11, b_t *n10, b_t *n9, b_t *n8,
			b_t *n7, b_t *n6, b_t *n5, b_t *n4, b_t *n3, b_t *n2, b_t *n1, b_t *n0)
		{
			_uint_get(5, (b_t const *)this, n31, n30, n29, n28, n27, n26, n25, n24, n23, n22, n21, n20, n19, n18, n17, n16, n15, n14, n13, n12, n11, n10, n9, n8, n7, n6, n5, n4, n3, n2, n1, n0);
		}
		void espress_stream_uint256_va_set(struct espress_stream_uint256 *this, va_list *np) {_uint_va_set(5, (b_t *)this, np);}
		void espress_stream_uint256_va_get(struct espress_stream_uint256 const *this, va_list *np) {_uint_va_get(5, (b_t const *)this, np);}
		void espress_stream_uint256_set_u(struct espress_stream_uint256 *this, u_t n) {_uint_set_u(5, (b_t *)this, n);}
		void espress_stream_uint256_get_u(struct espress_stream_uint256 const *this, u_t *n) {_uint_get_u(5, (b_t const *)this, n);}
		void espress_stream_uint256_assign(struct espress_stream_uint256 *this, struct espress_stream_uint256 const *n) {_uint_assign(5, (b_t *)this, (b_t const *)n);}
		i_t espress_stream_uint256_add(struct espress_stream_uint256 *this, struct espress_stream_uint256 const *n) {return _uint_add(5, (b_t *)this, (b_t const *)n);}
		i_t espress_stream_uint256_sub(struct espress_stream_uint256 *this, struct espress_stream_uint256 const *n) {return _uint_sub(5, (b_t *)this, (b_t const *)n);}
		void espress_stream_uint256_mul(struct espress_stream_uint256 *this, struct espress_stream_uint256 const *n) {_uint_mul(5, (b_t *)this, (b_t const *)n);}
		void espress_stream_uint256_div(struct espress_stream_uint256 *this, struct espress_stream_uint256 const *n) {_uint_div(5, (b_t *)this, (b_t const *)n);}
		void espress_stream_uint256_and(struct espress_stream_uint256 *this, struct espress_stream_uint256 const *n) {_uint_and(5, (b_t *)this, (b_t const *)n);}
		void espress_stream_uint256_or(struct espress_stream_uint256 *this, struct espress_stream_uint256 const *n) {_uint_or(5, (b_t *)this, (b_t const *)n);}
		void espress_stream_uint256_xor(struct espress_stream_uint256 *this, struct espress_stream_uint256 const *n) {_uint_xor(5, (b_t *)this, (b_t const *)n);}
		void espress_stream_uint256_not(struct espress_stream_uint256 *this) {_uint_not(5, (b_t *)this);}
		void espress_stream_uint256_shift(struct espress_stream_uint256 *this, i_t n) {_uint_shift(5, (b_t *)this, n);}
		void espress_stream_uint256_rotate(struct espress_stream_uint256 *this, i_t n) {_uint_rotate(5, (b_t *)this, n);}
		i_t espress_stream_uint256_eq(struct espress_stream_uint256 const *this, struct espress_stream_uint256 const *n) {return _uint_eq(5, (b_t const *)this, (b_t const *)n);}
		i_t espress_stream_uint256_lt(struct espress_stream_uint256 const *this, struct espress_stream_uint256 const *n) {return _uint_lt(5, (b_t const *)this, (b_t const *)n);}
		i_t espress_stream_uint256_le(struct espress_stream_uint256 const *this, struct espress_stream_uint256 const *n) {return _uint_le(5, (b_t const *)this, (b_t const *)n);}
		void espress_stream_uint256_bswap(struct espress_stream_uint256 *this) {_uint_bswap(5, (b_t *)this);}
#	endif
#endif

#if 64 <= ESPRESS_STREAM_UINT_MAX_WIDTH
	struct espress_stream_uint512 const espress_stream_uint512_0[] = {ESPRESS_STREAM_UINT512_0};
	struct espress_stream_uint512 const espress_stream_uint512_1[] = {ESPRESS_STREAM_UINT512_1};
	struct espress_stream_uint512 const espress_stream_uint512_max[] = {ESPRESS_STREAM_UINT512_MAX};
#	if 64 <= MAX_U_T_WIDTH
#	else
		void espress_stream_uint512_set(struct espress_stream_uint512 *this,
			b_t n63, b_t n62, b_t n61, b_t n60, b_t n59, b_t n58, b_t n57, b_t n56,
			b_t n55, b_t n54, b_t n53, b_t n52, b_t n51, b_t n50, b_t n49, b_t n48,
			b_t n47, b_t n46, b_t n45, b_t n44, b_t n43, b_t n42, b_t n41, b_t n40,
			b_t n39, b_t n38, b_t n37, b_t n36, b_t n35, b_t n34, b_t n33, b_t n32,
			b_t n31, b_t n30, b_t n29, b_t n28, b_t n27, b_t n26, b_t n25, b_t n24,
			b_t n23, b_t n22, b_t n21, b_t n20, b_t n19, b_t n18, b_t n17, b_t n16,
			b_t n15, b_t n14, b_t n13, b_t n12, b_t n11, b_t n10, b_t n9, b_t n8,
			b_t n7, b_t n6, b_t n5, b_t n4, b_t n3, b_t n2, b_t n1, b_t n0)
		{
			_uint_set(6, (b_t *)this,
				n63, n62, n61, n60, n59, n58, n57, n56, n55, n54, n53, n52, n51, n50, n49, n48, n47, n46, n45, n44, n43, n42, n41, n40, n39, n38, n37, n36, n35, n34, n33, n32,
				n31, n30, n29, n28, n27, n26, n25, n24, n23, n22, n21, n20, n19, n18, n17, n16, n15, n14, n13, n12, n11, n10, n9, n8, n7, n6, n5, n4, n3, n2, n1, n0);
		}
		void espress_stream_uint512_get(struct espress_stream_uint512 const *this,
			b_t *n63, b_t *n62, b_t *n61, b_t *n60, b_t *n59, b_t *n58, b_t *n57, b_t *n56,
			b_t *n55, b_t *n54, b_t *n53, b_t *n52, b_t *n51, b_t *n50, b_t *n49, b_t *n48,
			b_t *n47, b_t *n46, b_t *n45, b_t *n44, b_t *n43, b_t *n42, b_t *n41, b_t *n40,
			b_t *n39, b_t *n38, b_t *n37, b_t *n36, b_t *n35, b_t *n34, b_t *n33, b_t *n32,
			b_t *n31, b_t *n30, b_t *n29, b_t *n28, b_t *n27, b_t *n26, b_t *n25, b_t *n24,
			b_t *n23, b_t *n22, b_t *n21, b_t *n20, b_t *n19, b_t *n18, b_t *n17, b_t *n16,
			b_t *n15, b_t *n14, b_t *n13, b_t *n12, b_t *n11, b_t *n10, b_t *n9, b_t *n8,
			b_t *n7, b_t *n6, b_t *n5, b_t *n4, b_t *n3, b_t *n2, b_t *n1, b_t *n0)
		{
			_uint_get(6, (b_t const *)this,
				n63, n62, n61, n60, n59, n58, n57, n56, n55, n54, n53, n52, n51, n50, n49, n48, n47, n46, n45, n44, n43, n42, n41, n40, n39, n38, n37, n36, n35, n34, n33, n32,
				n31, n30, n29, n28, n27, n26, n25, n24, n23, n22, n21, n20, n19, n18, n17, n16, n15, n14, n13, n12, n11, n10, n9, n8, n7, n6, n5, n4, n3, n2, n1, n0);
		}
		void espress_stream_uint512_va_set(struct espress_stream_uint512 *this, va_list *np) {_uint_va_set(6, (b_t *)this, np);}
		void espress_stream_uint512_va_get(struct espress_stream_uint512 const *this, va_list *np) {_uint_va_get(6, (b_t const *)this, np);}
		void espress_stream_uint512_set_u(struct espress_stream_uint512 *this, u_t n) {_uint_set_u(6, (b_t *)this, n);}
		void espress_stream_uint512_get_u(struct espress_stream_uint512 const *this, u_t *n) {_uint_get_u(6, (b_t const *)this, n);}
		void espress_stream_uint512_assign(struct espress_stream_uint512 *this, struct espress_stream_uint512 const *n) {_uint_assign(6, (b_t *)this, (b_t const *)n);}
		i_t espress_stream_uint512_add(struct espress_stream_uint512 *this, struct espress_stream_uint512 const *n) {return _uint_add(6, (b_t *)this, (b_t const *)n);}
		i_t espress_stream_uint512_sub(struct espress_stream_uint512 *this, struct espress_stream_uint512 const *n) {return _uint_sub(6, (b_t *)this, (b_t const *)n);}
		void espress_stream_uint512_mul(struct espress_stream_uint512 *this, struct espress_stream_uint512 const *n) {_uint_mul(6, (b_t *)this, (b_t const *)n);}
		void espress_stream_uint512_div(struct espress_stream_uint512 *this, struct espress_stream_uint512 const *n) {_uint_div(6, (b_t *)this, (b_t const *)n);}
		void espress_stream_uint512_and(struct espress_stream_uint512 *this, struct espress_stream_uint512 const *n) {_uint_and(6, (b_t *)this, (b_t const *)n);}
		void espress_stream_uint512_or(struct espress_stream_uint512 *this, struct espress_stream_uint512 const *n) {_uint_or(6, (b_t *)this, (b_t const *)n);}
		void espress_stream_uint512_xor(struct espress_stream_uint512 *this, struct espress_stream_uint512 const *n) {_uint_xor(6, (b_t *)this, (b_t const *)n);}
		void espress_stream_uint512_not(struct espress_stream_uint512 *this) {_uint_not(6, (b_t *)this);}
		void espress_stream_uint512_shift(struct espress_stream_uint512 *this, i_t n) {_uint_shift(6, (b_t *)this, n);}
		void espress_stream_uint512_rotate(struct espress_stream_uint512 *this, i_t n) {_uint_rotate(6, (b_t *)this, n);}
		i_t espress_stream_uint512_eq(struct espress_stream_uint512 const *this, struct espress_stream_uint512 const *n) {return _uint_eq(6, (b_t const *)this, (b_t const *)n);}
		i_t espress_stream_uint512_lt(struct espress_stream_uint512 const *this, struct espress_stream_uint512 const *n) {return _uint_lt(6, (b_t const *)this, (b_t const *)n);}
		i_t espress_stream_uint512_le(struct espress_stream_uint512 const *this, struct espress_stream_uint512 const *n) {return _uint_le(6, (b_t const *)this, (b_t const *)n);}
		void espress_stream_uint512_bswap(struct espress_stream_uint512 *this) {_uint_bswap(6, (b_t *)this);}
#	endif
#endif
