#include "espress_cipher/depend.h"
#include "espress_cipher/uint.h"

#define _U_T_BIT_WIDTH (U_T_WIDTH * 8)

#if 512 <= ESPRESS_CIPHER_UINT_MAX_WIDTH
#	define _BUFFER_SIZE sizeof(struct espress_cipher_uint4096)
#elif 256 <= ESPRESS_CIPHER_UINT_MAX_WIDTH
#	define _BUFFER_SIZE sizeof(struct espress_cipher_uint2048)
#elif 128 <= ESPRESS_CIPHER_UINT_MAX_WIDTH
#	define _BUFFER_SIZE sizeof(struct espress_cipher_uint1024)
#elif 64 <= ESPRESS_CIPHER_UINT_MAX_WIDTH
#	define _BUFFER_SIZE sizeof(struct espress_cipher_uint512)
#elif 32 <= ESPRESS_CIPHER_UINT_MAX_WIDTH
#	define _BUFFER_SIZE sizeof(struct espress_cipher_uint256)
#elif 16 <= ESPRESS_CIPHER_UINT_MAX_WIDTH
#	define _BUFFER_SIZE sizeof(struct espress_cipher_uint128)
#elif 8 <= ESPRESS_CIPHER_UINT_MAX_WIDTH
#	define _BUFFER_SIZE sizeof(struct espress_cipher_uint64)
#elif 4 <= ESPRESS_CIPHER_UINT_MAX_WIDTH
#	define _BUFFER_SIZE sizeof(struct espress_cipher_uint32)
#elif 2 <= ESPRESS_CIPHER_UINT_MAX_WIDTH
#	define _BUFFER_SIZE sizeof(struct espress_cipher_uint16)
#elif 1 <= ESPRESS_CIPHER_UINT_MAX_WIDTH
#	define _BUFFER_SIZE sizeof(struct espress_cipher_uint8)
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
#if 2 <= ESPRESS_CIPHER_UINT_MAX_WIDTH
#	if 2 <= MAX_U_T_WIDTH
		{1 - ESPRESS_CIPHER_IS_HOST_BYTE_ORDER_LITTLE, ESPRESS_CIPHER_IS_HOST_BYTE_ORDER_LITTLE},
#	else
		{offsetof(struct espress_cipher_uint16, l), offsetof(struct espress_cipher_uint16, h)},
#endif
#endif
#if 4 <= ESPRESS_CIPHER_UINT_MAX_WIDTH
#	if 4 <= MAX_U_T_WIDTH
		{(1 - ESPRESS_CIPHER_IS_HOST_BYTE_ORDER_LITTLE) << 1, ESPRESS_CIPHER_IS_HOST_BYTE_ORDER_LITTLE << 1},
#	else
		{offsetof(struct espress_cipher_uint32, l), offsetof(struct espress_cipher_uint32, h)},
#	endif
#endif
#if 8 <= ESPRESS_CIPHER_UINT_MAX_WIDTH
#	if 8 <= MAX_U_T_WIDTH
		{(1 - ESPRESS_CIPHER_IS_HOST_BYTE_ORDER_LITTLE) << 2, ESPRESS_CIPHER_IS_HOST_BYTE_ORDER_LITTLE << 2},
#	else
		{offsetof(struct espress_cipher_uint64, l), offsetof(struct espress_cipher_uint64, h)},
#	endif
#endif
#if 16 <= ESPRESS_CIPHER_UINT_MAX_WIDTH
#	if 16 <= MAX_U_T_WIDTH
		{(1 - ESPRESS_CIPHER_IS_HOST_BYTE_ORDER_LITTLE) << 3, ESPRESS_CIPHER_IS_HOST_BYTE_ORDER_LITTLE << 3},
#	else
		{offsetof(struct espress_cipher_uint128, l), offsetof(struct espress_cipher_uint128, h)},
#	endif
#endif
#if 32 <= ESPRESS_CIPHER_UINT_MAX_WIDTH
#	if 32 <= MAX_U_T_WIDTH
		{(1 - ESPRESS_CIPHER_IS_HOST_BYTE_ORDER_LITTLE) << 4, ESPRESS_CIPHER_IS_HOST_BYTE_ORDER_LITTLE << 4},
#	else
		{offsetof(struct espress_cipher_uint256, l), offsetof(struct espress_cipher_uint256, h)},
#	endif
#endif
#if 64 <= ESPRESS_CIPHER_UINT_MAX_WIDTH
#	if 64 <= MAX_U_T_WIDTH
		{(1 - ESPRESS_CIPHER_IS_HOST_BYTE_ORDER_LITTLE) << 5, ESPRESS_CIPHER_IS_HOST_BYTE_ORDER_LITTLE << 5},
#	else
		{offsetof(struct espress_cipher_uint512, l), offsetof(struct espress_cipher_uint512, h)},
#	endif
#endif
#if 128 <= ESPRESS_CIPHER_UINT_MAX_WIDTH
#	if 128 <= MAX_U_T_WIDTH
		{(1 - ESPRESS_CIPHER_IS_HOST_BYTE_ORDER_LITTLE) << 6, ESPRESS_CIPHER_IS_HOST_BYTE_ORDER_LITTLE << 6},
#	else
		{offsetof(struct espress_cipher_uint1024, l), offsetof(struct espress_cipher_uint1024, h)},
#	endif
#endif
#if 256 <= ESPRESS_CIPHER_UINT_MAX_WIDTH
#	if 256 <= MAX_U_T_WIDTH
		{(1 - ESPRESS_CIPHER_IS_HOST_BYTE_ORDER_LITTLE) << 7, ESPRESS_CIPHER_IS_HOST_BYTE_ORDER_LITTLE << 7,
#	else
		{offsetof(struct espress_cipher_uint2048, l), offsetof(struct espress_cipher_uint2048, h)},
#	endif
#endif
#if 512 <= ESPRESS_CIPHER_UINT_MAX_WIDTH
#	if 512 <= MAX_U_T_WIDTH
		{(1 - ESPRESS_CIPHER_IS_HOST_BYTE_ORDER_LITTLE) << 8, ESPRESS_CIPHER_IS_HOST_BYTE_ORDER_LITTLE << 8,
#	else
		{offsetof(struct espress_cipher_uint4096, l), offsetof(struct espress_cipher_uint4096, h)},
#	endif
#endif
};

#if ESPRESS_CIPHER_IS_HOST_BYTE_ORDER_LITTLE
#	define _O(w, n) (n)
#else
#	define _O(w, n) ((1 << (w)) - 1 - (n))
#endif

#if 512 <= MAX_U_T_WIDTH
#	define _espress_cipher_uint espress_cipher_uint4096
#	define _np_last(w, this, __func__) do{if (8 == (w)) {espress_cipher_uint4096_##__func__ (this); goto EXIT;}}while (0)
#	define _last(w, this, __func__, ...) do{if (8 == (w)) {espress_cipher_uint4096_##__func__ (this, __VA_ARGS__); goto EXIT;}}while (0)
#	define _r_last(r, w, this, __func__, ...) do{if (8 == (w)) {r = espress_cipher_uint4096_##__func__ (this, __VA_ARGS__); goto EXIT;}}while (0)
#	define _va_last(w, __func__, this, np) do{if (8 == (w)) {espress_cipher_uint4096_##__func__ (this, np); goto EXIT;}}while (0)
#elif 256 <= MAX_U_T_WIDTH
#	define _espress_cipher_uint espress_cipher_uint2048
#	define _np_last(w, this, __func__) do{if (8 == (w)) {espress_cipher_uint2048_##__func__ (this); goto EXIT;}}while (0)
#	define _last(w, this, __func__, ...) do{if (8 == (w)) {espress_cipher_uint2048_##__func__ (this, __VA_ARGS__); goto EXIT;}}while (0)
#	define _r_last(r, w, this, __func__, ...) do{if (8 == (w)) {r = espress_cipher_uint2048_##__func__ (this, __VA_ARGS__); goto EXIT;}}while (0)
#	define _va_last(w, __func__, this, np) do{if (8 == (w)) {espress_cipher_uint2048_##__func__ (this, np); goto EXIT;}}while (0)
#elif 128 <= MAX_U_T_WIDTH
#	define _espress_cipher_uint espress_cipher_uint1024
#	define _np_last(w, this, __func__) do{if (7 == (w)) {espress_cipher_uint1024_##__func__ (this); goto EXIT;}}while (0)
#	define _last(w, this, __func__, ...) do{if (7 == (w)) {espress_cipher_uint1024_##__func__ (this, __VA_ARGS__); goto EXIT;}}while (0)
#	define _r_last(r, w, this, __func__, ...) do{if (7 == (w)) {r = espress_cipher_uint1024_##__func__ (this, __VA_ARGS__); goto EXIT;}}while (0)
#	define _va_last(w, __func__, this, np) do{if (7 == (w)) {espress_cipher_uint1024_##__func__ (this, np); goto EXIT;}}while (0)
#elif 64 <= MAX_U_T_WIDTH
#	define _espress_cipher_uint espress_cipher_uint512
#	define _np_last(w, this, __func__) do{if (6 == (w)) {espress_cipher_uint512_##__func__ (this); goto EXIT;}}while (0)
#	define _last(w, this, __func__, ...) do{if (6 == (w)) {espress_cipher_uint512_##__func__ (this, __VA_ARGS__); goto EXIT;}}while (0)
#	define _r_last(r, w, this, __func__, ...) do{if (6 == (w)) {r = espress_cipher_uint512_##__func__ (this, __VA_ARGS__); goto EXIT;}}while (0)
#	define _va_last(w, __func__, this, np) do{if (6 == (w)) {espress_cipher_uint512_##__func__ (this, np); goto EXIT;}}while (0)
#elif 32 <= MAX_U_T_WIDTH
#	define _espress_cipher_uint espress_cipher_uint256
#	define _np_last(w, this, __func__) do{if (5 == (w)) {espress_cipher_uint256_##__func__ (this); goto EXIT;}}while (0)
#	define _last(w, this, __func__, ...) do{if (5 == (w)) {espress_cipher_uint256_##__func__ (this, __VA_ARGS__); goto EXIT;}}while (0)
#	define _r_last(r, w, this, __func__, ...) do{if (5 == (w)) {r = espress_cipher_uint256_##__func__ (this, __VA_ARGS__); goto EXIT;}}while (0)
#	define _va_last(w, __func__, this, np) do{if (5 == (w)) {espress_cipher_uint256_##__func__ (this, np); goto EXIT;}}while (0)
#elif 16 <= MAX_U_T_WIDTH
#	define _espress_cipher_uint espress_cipher_uint128
#	define _np_last(w, __func__, this) do{if (4 == (w)) {espress_cipher_uint128_##__func__ (this); goto EXIT;}}while (0)
#	define _last(w, __func__, this, ...) do{if (4 == (w)) {espress_cipher_uint128_##__func__ (this, __VA_ARGS__); goto EXIT;}}while (0)
#	define _r_last(r, w, __func__, this, ...) do{if (4 == (w)) {r = espress_cipher_uint128_##__func__ (this, __VA_ARGS__); goto EXIT;}}while (0)
#	define _va_last(w, __func__, this, np) do{if (4 == (w)) {espress_cipher_uint128_##__func__ (this, np); goto EXIT;}}while (0)
#elif 8 <= MAX_U_T_WIDTH
#	define _espress_cipher_uint espress_cipher_uint64
#	define _np_last(w, __func__, this) do{if (3 == (w)) {espress_cipher_uint64_##__func__ (this); goto EXIT;}}while (0)
#	define _last(w, __func__, this, ...) do{if (3 == (w)) {espress_cipher_uint64_##__func__ (this, __VA_ARGS__); goto EXIT;}}while (0)
#	define _r_last(r, w, __func__, this, ...) do{if (3 == (w)) {r = espress_cipher_uint64_##__func__ (this, __VA_ARGS__); goto EXIT;}}while (0)
#	define _va_last(w, __func__, this, np) do{if (3 == (w)) {espress_cipher_uint64_##__func__ (this, np); goto EXIT;}}while (0)
#elif 4 <= MAX_U_T_WIDTH
#	define _espress_cipher_uint espress_cipher_uint32
#	define _np_last(w, __func__, this) do{if (2 == (w)) {espress_cipher_uint32_##__func__ (this); goto EXIT;}}while (0)
#	define _last(w, __func__, this, ...) do{if (2 == (w)) {espress_cipher_uint32_##__func__ (this, __VA_ARGS__); goto EXIT;}}while (0)
#	define _r_last(r, w, __func__, this, ...) do{if (2 == (w)) {r = espress_cipher_uint32_##__func__ (this, __VA_ARGS__); goto EXIT;}}while (0)
#	define _va_last(w, __func__, this, np) do{if (2 == (w)) {espress_cipher_uint32_##__func__ (this, np); goto EXIT;}}while (0)
#elif 2 <= MAX_U_T_WIDTH
#	define _espress_cipher_uint espress_cipher_uint16
#	define _np_last(w, __func__, this) do{if (1 == (w)) {espress_cipher_uint16_##__func__ (this); goto EXIT;}}while (0)
#	define _last(w, __func__, this, ...) do{if (1 == (w)) {espress_cipher_uint16_##__func__ (this, __VA_ARGS__); goto EXIT;}}while (0)
#	define _r_last(r, w, __func__, this, ...) do{if (1 == (w)) {r = espress_cipher_uint16_##__func__ (this, __VA_ARGS__); goto EXIT;}}while (0)
#	define _va_last(w, __func__, this, np) do{if (1 == (w)) {espress_cipher_uint16_##__func__ (this, np); goto EXIT;}}while (0)
#else
#	define _espress_cipher_uint espress_cipher_uint8
#	define _np_last(w, __func__, this) do{if (0 == (w)) {espress_cipher_uint8_##__func__ (this); goto EXIT;}}while (0)
#	define _last(r, w, __func__, this, ...) do{if (0 == (w)) {espress_cipher_uint8_##__func__ (this, __VA_ARGS__); goto EXIT;}}while (0)
#	define _r_last(r, w, __func__, this, ...) do{if (0 == (w)) {r = espress_cipher_uint8_##__func__ (this, __VA_ARGS__); goto EXIT;}}while (0)
#	define _va_last(w, __func__, this, np) do{if (0 == (w)) {espress_cipher_uint8_##__func__ (this, np); goto EXIT;}}while (0)
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
	_va_last(w, va_set, (struct _espress_cipher_uint *)this, np);
	_uint_va_set(ww, this + _uint[w].h, np);
	_uint_va_set(ww, this + _uint[w].l, np);
EXIT:
	return;
}

static void _uint_va_get(i_t w, b_t const *this, va_list *np)
{
	i_t const ww = w - 1;
	_va_last(w, va_get, (struct _espress_cipher_uint const *)this, np);
	_uint_va_get(ww, this + _uint[w].h, np);
	_uint_va_get(ww, this + _uint[w].l, np);
EXIT:
	return;
}

static void _uint_set_u(i_t w, b_t *this, u_t n)
{
	i_t const ww = w - 1;
	i_t const bww = 1 << (ww + 3);
	_last(w, set_u, (struct _espress_cipher_uint *)this, n);
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
	_last(w, get_u, (struct _espress_cipher_uint const *)this, n);
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
	_last(w, assign, (struct _espress_cipher_uint *)this, (struct _espress_cipher_uint const *)n);
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
	_r_last(is_of, w, add, (struct _espress_cipher_uint *)this, (struct _espress_cipher_uint const *)n);
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
	_r_last(is_uf, w, sub, (struct _espress_cipher_uint *)this, (struct _espress_cipher_uint const *)n);
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
	_last(w, mul, (struct _espress_cipher_uint *)this, (struct _espress_cipher_uint const *)n);
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
	_last(w, mul, (struct _espress_cipher_uint *)this, (struct _espress_cipher_uint const *)n);
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
	_last(w, and, (struct _espress_cipher_uint *)this, (struct _espress_cipher_uint const *)n);
	_uint_and(ww, this + _uint[w].l, n + _uint[w].l);
	_uint_and(ww, this + _uint[w].h, n + _uint[w].h);
EXIT:
	return;
}

static void _uint_or(i_t w, b_t *this, b_t const *n)
{
	i_t const ww = w - 1;
	_last(w, or, (struct _espress_cipher_uint *)this, (struct _espress_cipher_uint const *)n);
	_uint_or(ww, this + _uint[w].l, n + _uint[w].l);
	_uint_or(ww, this + _uint[w].h, n + _uint[w].h);
EXIT:
	return;
}

static void _uint_xor(i_t w, b_t *this, b_t const *n)
{
	i_t const ww = w - 1;
	_last(w, xor, (struct _espress_cipher_uint *)this, (struct _espress_cipher_uint const *)n);
	_uint_xor(ww, this + _uint[w].l, n + _uint[w].l);
	_uint_xor(ww, this + _uint[w].h, n + _uint[w].h);
EXIT:
	return;
}

static void _uint_not(i_t w, b_t *this)
{
	i_t const ww = w - 1;
	_np_last(w, not, (struct _espress_cipher_uint *)this);
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
	_last(w, shift, (struct _espress_cipher_uint *)this, n);
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
	_r_last(ret, w, eq, (struct _espress_cipher_uint const *)this, (struct _espress_cipher_uint const *)n);
	ret = _uint_eq(ww, this + _uint[w].l, n + _uint[w].l) && _uint_eq(ww, this + _uint[w].h, n + _uint[w].h);
EXIT:
	return ret;
}

static i_t _uint_lt(i_t w, b_t const *this, b_t const *n)
{
	i_t const ww = w - 1;
	i_t ret;
	_r_last(ret, w, lt, (struct _espress_cipher_uint const *)this, (struct _espress_cipher_uint const *)n);
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
	_np_last(w, bswap, (struct _espress_cipher_uint *)this);
	_uint_bswap(ww, this + _uint[w].l);
	_uint_bswap(ww, this + _uint[w].h);
	_uint_rotate(w, this, bww);
EXIT:
	return;
}

struct espress_cipher_uint8 const espress_cipher_uint8_0[] = {ESPRESS_CIPHER_UINT8_0};
struct espress_cipher_uint8 const espress_cipher_uint8_1[] = {ESPRESS_CIPHER_UINT8_1};
struct espress_cipher_uint8 const espress_cipher_uint8_max[] = {ESPRESS_CIPHER_UINT8_MAX};

#if 2 <= ESPRESS_CIPHER_UINT_MAX_WIDTH
	struct espress_cipher_uint16 const espress_cipher_uint16_0[] = {ESPRESS_CIPHER_UINT16_0};
	struct espress_cipher_uint16 const espress_cipher_uint16_1[] = {ESPRESS_CIPHER_UINT16_1};
	struct espress_cipher_uint16 const espress_cipher_uint16_max[] = {ESPRESS_CIPHER_UINT16_MAX};
#	if 2 <= MAX_U_T_WIDTH
#	else
		void espress_cipher_uint16_set(struct espress_cipher_uint16 *this, b_t n1, b_t n0) {_uint_set(1, (b_t *)this, n1, n0);}
		void espress_cipher_uint16_get(struct espress_cipher_uint16 const *this, b_t *n1, b_t *n0) {_uint_get(1, (b_t const *)this, n1, n0);}
		void espress_cipher_uint16_va_set(struct espress_cipher_uint16 *this, va_list *np) {_uint_va_set(1, (b_t *)this, np);}
		void espress_cipher_uint16_va_get(struct espress_cipher_uint16 const *this, va_list *np) {_uint_va_get(1, (b_t const *)this, np);}
		void espress_cipher_uint16_set_u(struct espress_cipher_uint16 *this, u_t n) {_uint_set_u(1, (b_t *)this, n);}
		void espress_cipher_uint16_get_u(struct espress_cipher_uint16 const *this, u_t *n) {_uint_get_u(1, (b_t const *)this, n);}
		void espress_cipher_uint16_assign(struct espress_cipher_uint16 *this, struct espress_cipher_uint16 const *n) {_uint_assign(1, (b_t *)this, (b_t const *)n);}
		i_t espress_cipher_uint16_add(struct espress_cipher_uint16 *this, struct espress_cipher_uint16 const *n) {return _uint_add(1, (b_t *)this, (b_t const *)n);}
		i_t espress_cipher_uint16_sub(struct espress_cipher_uint16 *this, struct espress_cipher_uint16 const *n) {return _uint_sub(1, (b_t *)this, (b_t const *)n);}
		void espress_cipher_uint16_mul(struct espress_cipher_uint16 *this, struct espress_cipher_uint16 const *n) {_uint_mul(1, (b_t *)this, (b_t const *)n);}
		void espress_cipher_uint16_div(struct espress_cipher_uint16 *this, struct espress_cipher_uint16 const *n) {_uint_div(1, (b_t *)this, (b_t const *)n);}
		void espress_cipher_uint16_and(struct espress_cipher_uint16 *this, struct espress_cipher_uint16 const *n) {_uint_and(1, (b_t *)this, (b_t const *)n);}
		void espress_cipher_uint16_or(struct espress_cipher_uint16 *this, struct espress_cipher_uint16 const *n) {_uint_or(1, (b_t *)this, (b_t const *)n);}
		void espress_cipher_uint16_xor(struct espress_cipher_uint16 *this, struct espress_cipher_uint16 const *n) {_uint_xor(1, (b_t *)this, (b_t const *)n);}
		void espress_cipher_uint16_not(struct espress_cipher_uint16 *this) {_uint_not(1, (b_t *)this);}
		void espress_cipher_uint16_shift(struct espress_cipher_uint16 *this, i_t n) {_uint_shift(1, (b_t *)this, n);}
		void espress_cipher_uint16_rotate(struct espress_cipher_uint16 *this, i_t n) {_uint_rotate(1, (b_t *)this, n);}
		i_t espress_cipher_uint16_eq(struct espress_cipher_uint16 const *this, struct espress_cipher_uint16 const *n) {return _uint_eq(1, (b_t const *)this, (b_t const *)n);}
		i_t espress_cipher_uint16_lt(struct espress_cipher_uint16 const *this, struct espress_cipher_uint16 const *n) {return _uint_lt(1, (b_t const *)this, (b_t const *)n);}
		i_t espress_cipher_uint16_le(struct espress_cipher_uint16 const *this, struct espress_cipher_uint16 const *n) {return _uint_le(1, (b_t const *)this, (b_t const *)n);}
		void espress_cipher_uint16_bswap(struct espress_cipher_uint16 *this) {_uint_bswap(1, (b_t *)this);}
#	endif
#endif

#if 4 <= ESPRESS_CIPHER_UINT_MAX_WIDTH
	struct espress_cipher_uint32 const espress_cipher_uint32_0[] = {ESPRESS_CIPHER_UINT32_0};
	struct espress_cipher_uint32 const espress_cipher_uint32_1[] = {ESPRESS_CIPHER_UINT32_1};
	struct espress_cipher_uint32 const espress_cipher_uint32_max[] = {ESPRESS_CIPHER_UINT32_MAX};
#	if 4<= MAX_U_T_WIDTH
#	else
		void espress_cipher_uint32_set(struct espress_cipher_uint32 *this, b_t n3, b_t n2, b_t n1, b_t n0) {_uint_set(2, (b_t *)this, n3, n2, n1, n0);}
		void espress_cipher_uint32_get(struct espress_cipher_uint32 const *this, b_t *n3, b_t *n2, b_t *n1, b_t *n0) {_uint_get(2, (b_t const *)this, n3, n2, n1, n0);}
		void espress_cipher_uint32_va_set(struct espress_cipher_uint32 *this, va_list *np) {_uint_va_set(2, (b_t *)this, np);}
		void espress_cipher_uint32_va_get(struct espress_cipher_uint32 const *this, va_list *np) {_uint_va_get(2, (b_t const *)this, np);}
		void espress_cipher_uint32_set_u(struct espress_cipher_uint32 *this, u_t n) {_uint_set_u(2, (b_t *)this, n);}
		void espress_cipher_uint32_get_u(struct espress_cipher_uint32 const *this, u_t *n) {_uint_get_u(2, (b_t const *)this, n);}
		void espress_cipher_uint32_assign(struct espress_cipher_uint32 *this, struct espress_cipher_uint32 const *n) {_uint_assign(2, (b_t *)this, (b_t const *)n);}
		i_t espress_cipher_uint32_add(struct espress_cipher_uint32 *this, struct espress_cipher_uint32 const *n) {return _uint_add(2, (b_t *)this, (b_t const *)n);}
		i_t espress_cipher_uint32_sub(struct espress_cipher_uint32 *this, struct espress_cipher_uint32 const *n) {return _uint_sub(2, (b_t *)this, (b_t const *)n);}
		void espress_cipher_uint32_mul(struct espress_cipher_uint32 *this, struct espress_cipher_uint32 const *n) {_uint_mul(2, (b_t *)this, (b_t const *)n);}
		void espress_cipher_uint32_div(struct espress_cipher_uint32 *this, struct espress_cipher_uint32 const *n) {_uint_div(2, (b_t *)this, (b_t const *)n);}
		void espress_cipher_uint32_and(struct espress_cipher_uint32 *this, struct espress_cipher_uint32 const *n) {_uint_and(2, (b_t *)this, (b_t const *)n);}
		void espress_cipher_uint32_or(struct espress_cipher_uint32 *this, struct espress_cipher_uint32 const *n) {_uint_or(2, (b_t *)this, (b_t const *)n);}
		void espress_cipher_uint32_xor(struct espress_cipher_uint32 *this, struct espress_cipher_uint32 const *n) {_uint_xor(2, (b_t *)this, (b_t const *)n);}
		void espress_cipher_uint32_not(struct espress_cipher_uint32 *this) {_uint_not(2, (b_t *)this);}
		void espress_cipher_uint32_shift(struct espress_cipher_uint32 *this, i_t n) {_uint_shift(2, (b_t *)this, n);}
		void espress_cipher_uint32_rotate(struct espress_cipher_uint32 *this, i_t n) {_uint_rotate(2, (b_t *)this, n);}
		i_t espress_cipher_uint32_eq(struct espress_cipher_uint32 const *this, struct espress_cipher_uint32 const *n) {return _uint_eq(2, (b_t const *)this, (b_t const *)n);}
		i_t espress_cipher_uint32_lt(struct espress_cipher_uint32 const *this, struct espress_cipher_uint32 const *n) {return _uint_lt(2, (b_t const *)this, (b_t const *)n);}
		i_t espress_cipher_uint32_le(struct espress_cipher_uint32 const *this, struct espress_cipher_uint32 const *n) {return _uint_le(2, (b_t const *)this, (b_t const *)n);}
		void espress_cipher_uint32_bswap(struct espress_cipher_uint32 *this) {_uint_bswap(2, (b_t *)this);}
#	endif
#endif

#if 8 <= ESPRESS_CIPHER_UINT_MAX_WIDTH
	struct espress_cipher_uint64 const espress_cipher_uint64_0[] = {ESPRESS_CIPHER_UINT64_0};
	struct espress_cipher_uint64 const espress_cipher_uint64_1[] = {ESPRESS_CIPHER_UINT64_1};
	struct espress_cipher_uint64 const espress_cipher_uint64_max[] = {ESPRESS_CIPHER_UINT64_MAX};
#	if 8 <= MAX_U_T_WIDTH
#	else
		void espress_cipher_uint64_set(struct espress_cipher_uint64 *this, b_t n7, b_t n6, b_t n5, b_t n4, b_t n3, b_t n2, b_t n1, b_t n0) {_uint_set(3, (b_t *)this, n7, n6, n5, n4, n3, n2, n1, n0);}
		void espress_cipher_uint64_get(struct espress_cipher_uint64 const *this, b_t *n7, b_t *n6, b_t *n5, b_t *n4, b_t *n3, b_t *n2, b_t *n1, b_t *n0) {_uint_get(3, (b_t const *)this, n7, n6, n5, n4, n3, n2, n1, n0);}
		void espress_cipher_uint64_va_set(struct espress_cipher_uint64 *this, va_list *np) {_uint_va_set(3, (b_t *)this, np);}
		void espress_cipher_uint64_va_get(struct espress_cipher_uint64 const *this, va_list *np) {_uint_va_get(3, (b_t const *)this, np);}
		void espress_cipher_uint64_set_u(struct espress_cipher_uint64 *this, u_t n) {_uint_set_u(3, (b_t *)this, n);}
		void espress_cipher_uint64_get_u(struct espress_cipher_uint64 const *this, u_t *n) {_uint_get_u(3, (b_t const *)this, n);}
		void espress_cipher_uint64_assign(struct espress_cipher_uint64 *this, struct espress_cipher_uint64 const *n) {_uint_assign(3, (b_t *)this, (b_t const *)n);}
		i_t espress_cipher_uint64_add(struct espress_cipher_uint64 *this, struct espress_cipher_uint64 const *n) {return _uint_add(3, (b_t *)this, (b_t const *)n);}
		i_t espress_cipher_uint64_sub(struct espress_cipher_uint64 *this, struct espress_cipher_uint64 const *n) {return _uint_sub(3, (b_t *)this, (b_t const *)n);}
		void espress_cipher_uint64_mul(struct espress_cipher_uint64 *this, struct espress_cipher_uint64 const *n) {_uint_mul(3, (b_t *)this, (b_t const *)n);}
		void espress_cipher_uint64_div(struct espress_cipher_uint64 *this, struct espress_cipher_uint64 const *n) {_uint_div(3, (b_t *)this, (b_t const *)n);}
		void espress_cipher_uint64_and(struct espress_cipher_uint64 *this, struct espress_cipher_uint64 const *n) {_uint_and(3, (b_t *)this, (b_t const *)n);}
		void espress_cipher_uint64_or(struct espress_cipher_uint64 *this, struct espress_cipher_uint64 const *n) {_uint_or(3, (b_t *)this, (b_t const *)n);}
		void espress_cipher_uint64_xor(struct espress_cipher_uint64 *this, struct espress_cipher_uint64 const *n) {_uint_xor(3, (b_t *)this, (b_t const *)n);}
		void espress_cipher_uint64_not(struct espress_cipher_uint64 *this) {_uint_not(3, (b_t *)this);}
		void espress_cipher_uint64_shift(struct espress_cipher_uint64 *this, i_t n) {_uint_shift(3, (b_t *)this, n);}
		void espress_cipher_uint64_rotate(struct espress_cipher_uint64 *this, i_t n) {_uint_rotate(3, (b_t *)this, n);}
		i_t espress_cipher_uint64_eq(struct espress_cipher_uint64 const *this, struct espress_cipher_uint64 const *n) {return _uint_eq(3, (b_t const *)this, (b_t const *)n);}
		i_t espress_cipher_uint64_lt(struct espress_cipher_uint64 const *this, struct espress_cipher_uint64 const *n) {return _uint_lt(3, (b_t const *)this, (b_t const *)n);}
		i_t espress_cipher_uint64_le(struct espress_cipher_uint64 const *this, struct espress_cipher_uint64 const *n) {return _uint_le(3, (b_t const *)this, (b_t const *)n);}
		void espress_cipher_uint64_bswap(struct espress_cipher_uint64 *this) {_uint_bswap(3, (b_t *)this);}
#	endif
#endif

#if 16 <= ESPRESS_CIPHER_UINT_MAX_WIDTH
	struct espress_cipher_uint128 const espress_cipher_uint128_0[] = {ESPRESS_CIPHER_UINT128_0};
	struct espress_cipher_uint128 const espress_cipher_uint128_1[] = {ESPRESS_CIPHER_UINT128_1};
	struct espress_cipher_uint128 const espress_cipher_uint128_max[] = {ESPRESS_CIPHER_UINT128_MAX};
#	if 16 <= MAX_U_T_WIDTH
#	else
		void espress_cipher_uint128_set(struct espress_cipher_uint128 *this,
			b_t n15, b_t n14, b_t n13, b_t n12, b_t n11, b_t n10, b_t n9, b_t n8,
			b_t n7, b_t n6, b_t n5, b_t n4, b_t n3, b_t n2, b_t n1, b_t n0)
		{
			_uint_set(4, (b_t *)this, n15, n14, n13, n12, n11, n10, n9, n8, n7, n6, n5, n4, n3, n2, n1, n0);
		}
		void espress_cipher_uint128_get(struct espress_cipher_uint128 const *this,
			b_t *n15, b_t *n14, b_t *n13, b_t *n12, b_t *n11, b_t *n10, b_t *n9, b_t *n8,
			b_t *n7, b_t *n6, b_t *n5, b_t *n4, b_t *n3, b_t *n2, b_t *n1, b_t *n0)
		{
			_uint_get(4, (b_t const *)this, n15, n14, n13, n12, n11, n10, n9, n8, n7, n6, n5, n4, n3, n2, n1, n0);
		}
		void espress_cipher_uint128_va_set(struct espress_cipher_uint128 *this, va_list *np) {_uint_va_set(4, (b_t *)this, np);}
		void espress_cipher_uint128_va_get(struct espress_cipher_uint128 const *this, va_list *np) {_uint_va_get(4, (b_t const *)this, np);}
		void espress_cipher_uint128_set_u(struct espress_cipher_uint128 *this, u_t n) {_uint_set_u(4, (b_t *)this, n);}
		void espress_cipher_uint128_get_u(struct espress_cipher_uint128 const *this, u_t *n) {_uint_get_u(4, (b_t const *)this, n);}
		void espress_cipher_uint128_assign(struct espress_cipher_uint128 *this, struct espress_cipher_uint128 const *n) {_uint_assign(4, (b_t *)this, (b_t const *)n);}
		i_t espress_cipher_uint128_add(struct espress_cipher_uint128 *this, struct espress_cipher_uint128 const *n) {return _uint_add(4, (b_t *)this, (b_t const *)n);}
		i_t espress_cipher_uint128_sub(struct espress_cipher_uint128 *this, struct espress_cipher_uint128 const *n) {return _uint_sub(4, (b_t *)this, (b_t const *)n);}
		void espress_cipher_uint128_mul(struct espress_cipher_uint128 *this, struct espress_cipher_uint128 const *n) {_uint_mul(4, (b_t *)this, (b_t const *)n);}
		void espress_cipher_uint128_div(struct espress_cipher_uint128 *this, struct espress_cipher_uint128 const *n) {_uint_div(4, (b_t *)this, (b_t const *)n);}
		void espress_cipher_uint128_and(struct espress_cipher_uint128 *this, struct espress_cipher_uint128 const *n) {_uint_and(4, (b_t *)this, (b_t const *)n);}
		void espress_cipher_uint128_or(struct espress_cipher_uint128 *this, struct espress_cipher_uint128 const *n) {_uint_or(4, (b_t *)this, (b_t const *)n);}
		void espress_cipher_uint128_xor(struct espress_cipher_uint128 *this, struct espress_cipher_uint128 const *n) {_uint_xor(4, (b_t *)this, (b_t const *)n);}
		void espress_cipher_uint128_not(struct espress_cipher_uint128 *this) {_uint_not(4, (b_t *)this);}
		void espress_cipher_uint128_shift(struct espress_cipher_uint128 *this, i_t n) {_uint_shift(4, (b_t *)this, n);}
		void espress_cipher_uint128_rotate(struct espress_cipher_uint128 *this, i_t n) {_uint_rotate(4, (b_t *)this, n);}
		i_t espress_cipher_uint128_eq(struct espress_cipher_uint128 const *this, struct espress_cipher_uint128 const *n) {return _uint_eq(4, (b_t const *)this, (b_t const *)n);}
		i_t espress_cipher_uint128_lt(struct espress_cipher_uint128 const *this, struct espress_cipher_uint128 const *n) {return _uint_lt(4, (b_t const *)this, (b_t const *)n);}
		i_t espress_cipher_uint128_le(struct espress_cipher_uint128 const *this, struct espress_cipher_uint128 const *n) {return _uint_le(4, (b_t const *)this, (b_t const *)n);}
		void espress_cipher_uint128_bswap(struct espress_cipher_uint128 *this) {_uint_bswap(4, (b_t *)this);}
#	endif
#endif

#if 32 <= ESPRESS_CIPHER_UINT_MAX_WIDTH
	struct espress_cipher_uint256 const espress_cipher_uint256_0[] = {ESPRESS_CIPHER_UINT256_0};
	struct espress_cipher_uint256 const espress_cipher_uint256_1[] = {ESPRESS_CIPHER_UINT256_1};
	struct espress_cipher_uint256 const espress_cipher_uint256_max[] = {ESPRESS_CIPHER_UINT256_MAX};
#	if 32 <= MAX_U_T_WIDTH
#	else
		void espress_cipher_uint256_set(struct espress_cipher_uint256 *this,
			b_t n31, b_t n30, b_t n29, b_t n28, b_t n27, b_t n26, b_t n25, b_t n24,
			b_t n23, b_t n22, b_t n21, b_t n20, b_t n19, b_t n18, b_t n17, b_t n16,
			b_t n15, b_t n14, b_t n13, b_t n12, b_t n11, b_t n10, b_t n9, b_t n8,
			b_t n7, b_t n6, b_t n5, b_t n4, b_t n3, b_t n2, b_t n1, b_t n0)
		{
			_uint_set(5, (b_t *)this, n31, n30, n29, n28, n27, n26, n25, n24, n23, n22, n21, n20, n19, n18, n17, n16, n15, n14, n13, n12, n11, n10, n9, n8, n7, n6, n5, n4, n3, n2, n1, n0);
		}
		void espress_cipher_uint256_get(struct espress_cipher_uint256 const *this,
			b_t *n31, b_t *n30, b_t *n29, b_t *n28, b_t *n27, b_t *n26, b_t *n25, b_t *n24,
			b_t *n23, b_t *n22, b_t *n21, b_t *n20, b_t *n19, b_t *n18, b_t *n17, b_t *n16,
			b_t *n15, b_t *n14, b_t *n13, b_t *n12, b_t *n11, b_t *n10, b_t *n9, b_t *n8,
			b_t *n7, b_t *n6, b_t *n5, b_t *n4, b_t *n3, b_t *n2, b_t *n1, b_t *n0)
		{
			_uint_get(5, (b_t const *)this, n31, n30, n29, n28, n27, n26, n25, n24, n23, n22, n21, n20, n19, n18, n17, n16, n15, n14, n13, n12, n11, n10, n9, n8, n7, n6, n5, n4, n3, n2, n1, n0);
		}
		void espress_cipher_uint256_va_set(struct espress_cipher_uint256 *this, va_list *np) {_uint_va_set(5, (b_t *)this, np);}
		void espress_cipher_uint256_va_get(struct espress_cipher_uint256 const *this, va_list *np) {_uint_va_get(5, (b_t const *)this, np);}
		void espress_cipher_uint256_set_u(struct espress_cipher_uint256 *this, u_t n) {_uint_set_u(5, (b_t *)this, n);}
		void espress_cipher_uint256_get_u(struct espress_cipher_uint256 const *this, u_t *n) {_uint_get_u(5, (b_t const *)this, n);}
		void espress_cipher_uint256_assign(struct espress_cipher_uint256 *this, struct espress_cipher_uint256 const *n) {_uint_assign(5, (b_t *)this, (b_t const *)n);}
		i_t espress_cipher_uint256_add(struct espress_cipher_uint256 *this, struct espress_cipher_uint256 const *n) {return _uint_add(5, (b_t *)this, (b_t const *)n);}
		i_t espress_cipher_uint256_sub(struct espress_cipher_uint256 *this, struct espress_cipher_uint256 const *n) {return _uint_sub(5, (b_t *)this, (b_t const *)n);}
		void espress_cipher_uint256_mul(struct espress_cipher_uint256 *this, struct espress_cipher_uint256 const *n) {_uint_mul(5, (b_t *)this, (b_t const *)n);}
		void espress_cipher_uint256_div(struct espress_cipher_uint256 *this, struct espress_cipher_uint256 const *n) {_uint_div(5, (b_t *)this, (b_t const *)n);}
		void espress_cipher_uint256_and(struct espress_cipher_uint256 *this, struct espress_cipher_uint256 const *n) {_uint_and(5, (b_t *)this, (b_t const *)n);}
		void espress_cipher_uint256_or(struct espress_cipher_uint256 *this, struct espress_cipher_uint256 const *n) {_uint_or(5, (b_t *)this, (b_t const *)n);}
		void espress_cipher_uint256_xor(struct espress_cipher_uint256 *this, struct espress_cipher_uint256 const *n) {_uint_xor(5, (b_t *)this, (b_t const *)n);}
		void espress_cipher_uint256_not(struct espress_cipher_uint256 *this) {_uint_not(5, (b_t *)this);}
		void espress_cipher_uint256_shift(struct espress_cipher_uint256 *this, i_t n) {_uint_shift(5, (b_t *)this, n);}
		void espress_cipher_uint256_rotate(struct espress_cipher_uint256 *this, i_t n) {_uint_rotate(5, (b_t *)this, n);}
		i_t espress_cipher_uint256_eq(struct espress_cipher_uint256 const *this, struct espress_cipher_uint256 const *n) {return _uint_eq(5, (b_t const *)this, (b_t const *)n);}
		i_t espress_cipher_uint256_lt(struct espress_cipher_uint256 const *this, struct espress_cipher_uint256 const *n) {return _uint_lt(5, (b_t const *)this, (b_t const *)n);}
		i_t espress_cipher_uint256_le(struct espress_cipher_uint256 const *this, struct espress_cipher_uint256 const *n) {return _uint_le(5, (b_t const *)this, (b_t const *)n);}
		void espress_cipher_uint256_bswap(struct espress_cipher_uint256 *this) {_uint_bswap(5, (b_t *)this);}
#	endif
#endif

#if 64 <= ESPRESS_CIPHER_UINT_MAX_WIDTH
	struct espress_cipher_uint512 const espress_cipher_uint512_0[] = {ESPRESS_CIPHER_UINT512_0};
	struct espress_cipher_uint512 const espress_cipher_uint512_1[] = {ESPRESS_CIPHER_UINT512_1};
	struct espress_cipher_uint512 const espress_cipher_uint512_max[] = {ESPRESS_CIPHER_UINT512_MAX};
#	if 64 <= MAX_U_T_WIDTH
#	else
		void espress_cipher_uint512_set(struct espress_cipher_uint512 *this,
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
		void espress_cipher_uint512_get(struct espress_cipher_uint512 const *this,
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
		void espress_cipher_uint512_va_set(struct espress_cipher_uint512 *this, va_list *np) {_uint_va_set(6, (b_t *)this, np);}
		void espress_cipher_uint512_va_get(struct espress_cipher_uint512 const *this, va_list *np) {_uint_va_get(6, (b_t const *)this, np);}
		void espress_cipher_uint512_set_u(struct espress_cipher_uint512 *this, u_t n) {_uint_set_u(6, (b_t *)this, n);}
		void espress_cipher_uint512_get_u(struct espress_cipher_uint512 const *this, u_t *n) {_uint_get_u(6, (b_t const *)this, n);}
		void espress_cipher_uint512_assign(struct espress_cipher_uint512 *this, struct espress_cipher_uint512 const *n) {_uint_assign(6, (b_t *)this, (b_t const *)n);}
		i_t espress_cipher_uint512_add(struct espress_cipher_uint512 *this, struct espress_cipher_uint512 const *n) {return _uint_add(6, (b_t *)this, (b_t const *)n);}
		i_t espress_cipher_uint512_sub(struct espress_cipher_uint512 *this, struct espress_cipher_uint512 const *n) {return _uint_sub(6, (b_t *)this, (b_t const *)n);}
		void espress_cipher_uint512_mul(struct espress_cipher_uint512 *this, struct espress_cipher_uint512 const *n) {_uint_mul(6, (b_t *)this, (b_t const *)n);}
		void espress_cipher_uint512_div(struct espress_cipher_uint512 *this, struct espress_cipher_uint512 const *n) {_uint_div(6, (b_t *)this, (b_t const *)n);}
		void espress_cipher_uint512_and(struct espress_cipher_uint512 *this, struct espress_cipher_uint512 const *n) {_uint_and(6, (b_t *)this, (b_t const *)n);}
		void espress_cipher_uint512_or(struct espress_cipher_uint512 *this, struct espress_cipher_uint512 const *n) {_uint_or(6, (b_t *)this, (b_t const *)n);}
		void espress_cipher_uint512_xor(struct espress_cipher_uint512 *this, struct espress_cipher_uint512 const *n) {_uint_xor(6, (b_t *)this, (b_t const *)n);}
		void espress_cipher_uint512_not(struct espress_cipher_uint512 *this) {_uint_not(6, (b_t *)this);}
		void espress_cipher_uint512_shift(struct espress_cipher_uint512 *this, i_t n) {_uint_shift(6, (b_t *)this, n);}
		void espress_cipher_uint512_rotate(struct espress_cipher_uint512 *this, i_t n) {_uint_rotate(6, (b_t *)this, n);}
		i_t espress_cipher_uint512_eq(struct espress_cipher_uint512 const *this, struct espress_cipher_uint512 const *n) {return _uint_eq(6, (b_t const *)this, (b_t const *)n);}
		i_t espress_cipher_uint512_lt(struct espress_cipher_uint512 const *this, struct espress_cipher_uint512 const *n) {return _uint_lt(6, (b_t const *)this, (b_t const *)n);}
		i_t espress_cipher_uint512_le(struct espress_cipher_uint512 const *this, struct espress_cipher_uint512 const *n) {return _uint_le(6, (b_t const *)this, (b_t const *)n);}
		void espress_cipher_uint512_bswap(struct espress_cipher_uint512 *this) {_uint_bswap(6, (b_t *)this);}
#	endif
#endif

#if 128 <= ESPRESS_CIPHER_UINT_MAX_WIDTH
	struct espress_cipher_uint1024 const espress_cipher_uint1024_0[] = {ESPRESS_CIPHER_UINT1024_0};
	struct espress_cipher_uint1024 const espress_cipher_uint1024_1[] = {ESPRESS_CIPHER_UINT1024_1};
	struct espress_cipher_uint1024 const espress_cipher_uint1024_max[] = {ESPRESS_CIPHER_UINT1024_MAX};
#	if 128 <= MAX_U_T_WIDTH
#	else
		void espress_cipher_uint1024_set(struct espress_cipher_uint1024 *this,
			b_t n127, b_t n126, b_t n125, b_t n124, b_t n123, b_t n122, b_t n121, b_t n120,
			b_t n119, b_t n118, b_t n117, b_t n116, b_t n115, b_t n114, b_t n113, b_t n112,
			b_t n111, b_t n110, b_t n109, b_t n108, b_t n107, b_t n106, b_t n105, b_t n104,
			b_t n103, b_t n102, b_t n101, b_t n100, b_t n99, b_t n98, b_t n97, b_t n96,
			b_t n95, b_t n94, b_t n93, b_t n92, b_t n91, b_t n90, b_t n89, b_t n88,
			b_t n87, b_t n86, b_t n85, b_t n84, b_t n83, b_t n82, b_t n81, b_t n80,
			b_t n79, b_t n78, b_t n77, b_t n76, b_t n75, b_t n74, b_t n73, b_t n72,
			b_t n71, b_t n70, b_t n69, b_t n68, b_t n67, b_t n66, b_t n65, b_t n64,
			b_t n63, b_t n62, b_t n61, b_t n60, b_t n59, b_t n58, b_t n57, b_t n56,
			b_t n55, b_t n54, b_t n53, b_t n52, b_t n51, b_t n50, b_t n49, b_t n48,
			b_t n47, b_t n46, b_t n45, b_t n44, b_t n43, b_t n42, b_t n41, b_t n40,
			b_t n39, b_t n38, b_t n37, b_t n36, b_t n35, b_t n34, b_t n33, b_t n32,
			b_t n31, b_t n30, b_t n29, b_t n28, b_t n27, b_t n26, b_t n25, b_t n24,
			b_t n23, b_t n22, b_t n21, b_t n20, b_t n19, b_t n18, b_t n17, b_t n16,
			b_t n15, b_t n14, b_t n13, b_t n12, b_t n11, b_t n10, b_t n9, b_t n8,
			b_t n7, b_t n6, b_t n5, b_t n4, b_t n3, b_t n2, b_t n1, b_t n0)
		{
			_uint_set(7, (b_t *)this,
				n127, n126, n125, n124, n123, n122, n121, n120, n119, n118, n117, n116, n115, n114, n113, n112, n111, n110, n109, n108, n107, n106, n105, n104, n103, n102, n101, n100, n99, n98, n97, n96,
				n95, n94, n93, n92, n91, n90, n89, n88, n87, n86, n85, n84, n83, n82, n81, n80, n79, n78, n77, n76, n75, n74, n73, n72, n71, n70, n69, n68, n67, n66, n65, n64,
				n63, n62, n61, n60, n59, n58, n57, n56, n55, n54, n53, n52, n51, n50, n49, n48, n47, n46, n45, n44, n43, n42, n41, n40, n39, n38, n37, n36, n35, n34, n33, n32,
				n31, n30, n29, n28, n27, n26, n25, n24, n23, n22, n21, n20, n19, n18, n17, n16, n15, n14, n13, n12, n11, n10, n9, n8, n7, n6, n5, n4, n3, n2, n1, n0);
		}
		void espress_cipher_uint1024_get(struct espress_cipher_uint1024 const *this,
			b_t *n127, b_t *n126, b_t *n125, b_t *n124, b_t *n123, b_t *n122, b_t *n121, b_t *n120,
			b_t *n119, b_t *n118, b_t *n117, b_t *n116, b_t *n115, b_t *n114, b_t *n113, b_t *n112,
			b_t *n111, b_t *n110, b_t *n109, b_t *n108, b_t *n107, b_t *n106, b_t *n105, b_t *n104,
			b_t *n103, b_t *n102, b_t *n101, b_t *n100, b_t *n99, b_t *n98, b_t *n97, b_t *n96,
			b_t *n95, b_t *n94, b_t *n93, b_t *n92, b_t *n91, b_t *n90, b_t *n89, b_t *n88,
			b_t *n87, b_t *n86, b_t *n85, b_t *n84, b_t *n83, b_t *n82, b_t *n81, b_t *n80,
			b_t *n79, b_t *n78, b_t *n77, b_t *n76, b_t *n75, b_t *n74, b_t *n73, b_t *n72,
			b_t *n71, b_t *n70, b_t *n69, b_t *n68, b_t *n67, b_t *n66, b_t *n65, b_t *n64,
			b_t *n63, b_t *n62, b_t *n61, b_t *n60, b_t *n59, b_t *n58, b_t *n57, b_t *n56,
			b_t *n55, b_t *n54, b_t *n53, b_t *n52, b_t *n51, b_t *n50, b_t *n49, b_t *n48,
			b_t *n47, b_t *n46, b_t *n45, b_t *n44, b_t *n43, b_t *n42, b_t *n41, b_t *n40,
			b_t *n39, b_t *n38, b_t *n37, b_t *n36, b_t *n35, b_t *n34, b_t *n33, b_t *n32,
			b_t *n31, b_t *n30, b_t *n29, b_t *n28, b_t *n27, b_t *n26, b_t *n25, b_t *n24,
			b_t *n23, b_t *n22, b_t *n21, b_t *n20, b_t *n19, b_t *n18, b_t *n17, b_t *n16,
			b_t *n15, b_t *n14, b_t *n13, b_t *n12, b_t *n11, b_t *n10, b_t *n9, b_t *n8,
			b_t *n7, b_t *n6, b_t *n5, b_t *n4, b_t *n3, b_t *n2, b_t *n1, b_t *n0)
		{
			_uint_get(7, (b_t const *)this,
				n127, n126, n125, n124, n123, n122, n121, n120, n119, n118, n117, n116, n115, n114, n113, n112, n111, n110, n109, n108, n107, n106, n105, n104, n103, n102, n101, n100, n99, n98, n97, n96,
				n95, n94, n93, n92, n91, n90, n89, n88, n87, n86, n85, n84, n83, n82, n81, n80, n79, n78, n77, n76, n75, n74, n73, n72, n71, n70, n69, n68, n67, n66, n65, n64,
				n63, n62, n61, n60, n59, n58, n57, n56, n55, n54, n53, n52, n51, n50, n49, n48, n47, n46, n45, n44, n43, n42, n41, n40, n39, n38, n37, n36, n35, n34, n33, n32,
				n31, n30, n29, n28, n27, n26, n25, n24, n23, n22, n21, n20, n19, n18, n17, n16, n15, n14, n13, n12, n11, n10, n9, n8, n7, n6, n5, n4, n3, n2, n1, n0);
		}
		void espress_cipher_uint1024_va_set(struct espress_cipher_uint1024 *this, va_list *np) {_uint_va_set(7, (b_t *)this, np);}
		void espress_cipher_uint1024_va_get(struct espress_cipher_uint1024 const *this, va_list *np) {_uint_va_get(7, (b_t const *)this, np);}
		void espress_cipher_uint1024_set_u(struct espress_cipher_uint1024 *this, u_t n) {_uint_set_u(7, (b_t *)this, n);}
		void espress_cipher_uint1024_get_u(struct espress_cipher_uint1024 const *this, u_t *n) {_uint_get_u(7, (b_t const *)this, n);}
		void espress_cipher_uint1024_assign(struct espress_cipher_uint1024 *this, struct espress_cipher_uint1024 const *n) {_uint_assign(7, (b_t *)this, (b_t const *)n);}
		i_t espress_cipher_uint1024_add(struct espress_cipher_uint1024 *this, struct espress_cipher_uint1024 const *n) {return _uint_add(7, (b_t *)this, (b_t const *)n);}
		i_t espress_cipher_uint1024_sub(struct espress_cipher_uint1024 *this, struct espress_cipher_uint1024 const *n) {return _uint_sub(7, (b_t *)this, (b_t const *)n);}
		void espress_cipher_uint1024_mul(struct espress_cipher_uint1024 *this, struct espress_cipher_uint1024 const *n) {_uint_mul(7, (b_t *)this, (b_t const *)n);}
		void espress_cipher_uint1024_div(struct espress_cipher_uint1024 *this, struct espress_cipher_uint1024 const *n) {_uint_div(7, (b_t *)this, (b_t const *)n);}
		void espress_cipher_uint1024_and(struct espress_cipher_uint1024 *this, struct espress_cipher_uint1024 const *n) {_uint_and(7, (b_t *)this, (b_t const *)n);}
		void espress_cipher_uint1024_or(struct espress_cipher_uint1024 *this, struct espress_cipher_uint1024 const *n) {_uint_or(7, (b_t *)this, (b_t const *)n);}
		void espress_cipher_uint1024_xor(struct espress_cipher_uint1024 *this, struct espress_cipher_uint1024 const *n) {_uint_xor(7, (b_t *)this, (b_t const *)n);}
		void espress_cipher_uint1024_not(struct espress_cipher_uint1024 *this) {_uint_not(7, (b_t *)this);}
		void espress_cipher_uint1024_shift(struct espress_cipher_uint1024 *this, i_t n) {_uint_shift(7, (b_t *)this, n);}
		void espress_cipher_uint1024_rotate(struct espress_cipher_uint1024 *this, i_t n) {_uint_rotate(7, (b_t *)this, n);}
		i_t espress_cipher_uint1024_eq(struct espress_cipher_uint1024 const *this, struct espress_cipher_uint1024 const *n) {return _uint_eq(7, (b_t const *)this, (b_t const *)n);}
		i_t espress_cipher_uint1024_lt(struct espress_cipher_uint1024 const *this, struct espress_cipher_uint1024 const *n) {return _uint_lt(7, (b_t const *)this, (b_t const *)n);}
		i_t espress_cipher_uint1024_le(struct espress_cipher_uint1024 const *this, struct espress_cipher_uint1024 const *n) {return _uint_le(7, (b_t const *)this, (b_t const *)n);}
		void espress_cipher_uint1024_bswap(struct espress_cipher_uint1024 *this) {_uint_bswap(7, (b_t *)this);}
#	endif
#endif

#if 256 <= ESPRESS_CIPHER_UINT_MAX_WIDTH
	struct espress_cipher_uint2048 const espress_cipher_uint2048_0[] = {ESPRESS_CIPHER_UINT2048_0};
	struct espress_cipher_uint2048 const espress_cipher_uint2048_1[] = {ESPRESS_CIPHER_UINT2048_1};
	struct espress_cipher_uint2048 const espress_cipher_uint2048_max[] = {ESPRESS_CIPHER_UINT2048_MAX};
#	if 256 <= MAX_U_T_WIDTH
#	else
		void espress_cipher_uint2048_set(struct espress_cipher_uint2048 *this,
			b_t n255, b_t n254, b_t n253, b_t n252, b_t n251, b_t n250, b_t n249, b_t n248,
			b_t n247, b_t n246, b_t n245, b_t n244, b_t n243, b_t n242, b_t n241, b_t n240,
			b_t n239, b_t n238, b_t n237, b_t n236, b_t n235, b_t n234, b_t n233, b_t n232,
			b_t n231, b_t n230, b_t n229, b_t n228, b_t n227, b_t n226, b_t n225, b_t n224,
			b_t n223, b_t n222, b_t n221, b_t n220, b_t n219, b_t n218, b_t n217, b_t n216,
			b_t n215, b_t n214, b_t n213, b_t n212, b_t n211, b_t n210, b_t n209, b_t n208,
			b_t n207, b_t n206, b_t n205, b_t n204, b_t n203, b_t n202, b_t n201, b_t n200,
			b_t n199, b_t n198, b_t n197, b_t n196, b_t n195, b_t n194, b_t n193, b_t n192,
			b_t n191, b_t n190, b_t n189, b_t n188, b_t n187, b_t n186, b_t n185, b_t n184,
			b_t n183, b_t n182, b_t n181, b_t n180, b_t n179, b_t n178, b_t n177, b_t n176,
			b_t n175, b_t n174, b_t n173, b_t n172, b_t n171, b_t n170, b_t n169, b_t n168,
			b_t n167, b_t n166, b_t n165, b_t n164, b_t n163, b_t n162, b_t n161, b_t n160,
			b_t n159, b_t n158, b_t n157, b_t n156, b_t n155, b_t n154, b_t n153, b_t n152,
			b_t n151, b_t n150, b_t n149, b_t n148, b_t n147, b_t n146, b_t n145, b_t n144,
			b_t n143, b_t n142, b_t n141, b_t n140, b_t n139, b_t n138, b_t n137, b_t n136,
			b_t n135, b_t n134, b_t n133, b_t n132, b_t n131, b_t n130, b_t n129, b_t n128, 
			b_t n127, b_t n126, b_t n125, b_t n124, b_t n123, b_t n122, b_t n121, b_t n120,
			b_t n119, b_t n118, b_t n117, b_t n116, b_t n115, b_t n114, b_t n113, b_t n112,
			b_t n111, b_t n110, b_t n109, b_t n108, b_t n107, b_t n106, b_t n105, b_t n104,
			b_t n103, b_t n102, b_t n101, b_t n100, b_t n99, b_t n98, b_t n97, b_t n96,
			b_t n95, b_t n94, b_t n93, b_t n92, b_t n91, b_t n90, b_t n89, b_t n88,
			b_t n87, b_t n86, b_t n85, b_t n84, b_t n83, b_t n82, b_t n81, b_t n80,
			b_t n79, b_t n78, b_t n77, b_t n76, b_t n75, b_t n74, b_t n73, b_t n72,
			b_t n71, b_t n70, b_t n69, b_t n68, b_t n67, b_t n66, b_t n65, b_t n64,
			b_t n63, b_t n62, b_t n61, b_t n60, b_t n59, b_t n58, b_t n57, b_t n56,
			b_t n55, b_t n54, b_t n53, b_t n52, b_t n51, b_t n50, b_t n49, b_t n48,
			b_t n47, b_t n46, b_t n45, b_t n44, b_t n43, b_t n42, b_t n41, b_t n40,
			b_t n39, b_t n38, b_t n37, b_t n36, b_t n35, b_t n34, b_t n33, b_t n32,
			b_t n31, b_t n30, b_t n29, b_t n28, b_t n27, b_t n26, b_t n25, b_t n24,
			b_t n23, b_t n22, b_t n21, b_t n20, b_t n19, b_t n18, b_t n17, b_t n16,
			b_t n15, b_t n14, b_t n13, b_t n12, b_t n11, b_t n10, b_t n9, b_t n8,
			b_t n7, b_t n6, b_t n5, b_t n4, b_t n3, b_t n2, b_t n1, b_t n0)
		{
			_uint_set(8, (b_t *)this,
				n255, n254, n253, n252, n251, n250, n249, n248, n247, n246, n245, n244, n243, n242, n241, n240, n239, n238, n237, n236, n235, n234, n233, n232, n231, n230, n229, n228, n227, n226, n225, n224,
				n223, n222, n221, n220, n219, n218, n217, n216, n215, n214, n213, n212, n211, n210, n209, n208, n207, n206, n205, n204, n203, n202, n201, n200, n199, n198, n197, n196, n195, n194, n193, n192,
				n191, n190, n189, n188, n187, n186, n185, n184, n183, n182, n181, n180, n179, n178, n177, n176, n175, n174, n173, n172, n171, n170, n169, n168, n167, n166, n165, n164, n163, n162, n161, n160,
				n159, n158, n157, n156, n155, n154, n153, n152, n151, n150, n149, n148, n147, n146, n145, n144, n143, n142, n141, n140, n139, n138, n137, n136, n135, n134, n133, n132, n131, n130, n129, n128,
				n127, n126, n125, n124, n123, n122, n121, n120, n119, n118, n117, n116, n115, n114, n113, n112, n111, n110, n109, n108, n107, n106, n105, n104, n103, n102, n101, n100, n99, n98, n97, n96,
				n95, n94, n93, n92, n91, n90, n89, n88, n87, n86, n85, n84, n83, n82, n81, n80, n79, n78, n77, n76, n75, n74, n73, n72, n71, n70, n69, n68, n67, n66, n65, n64,
				n63, n62, n61, n60, n59, n58, n57, n56, n55, n54, n53, n52, n51, n50, n49, n48, n47, n46, n45, n44, n43, n42, n41, n40, n39, n38, n37, n36, n35, n34, n33, n32,
				n31, n30, n29, n28, n27, n26, n25, n24, n23, n22, n21, n20, n19, n18, n17, n16, n15, n14, n13, n12, n11, n10, n9, n8, n7, n6, n5, n4, n3, n2, n1, n0);
		}
		void espress_cipher_uint2048_get(struct espress_cipher_uint2048 const *this,
			b_t *n255, b_t *n254, b_t *n253, b_t *n252, b_t *n251, b_t *n250, b_t *n249, b_t *n248,
			b_t *n247, b_t *n246, b_t *n245, b_t *n244, b_t *n243, b_t *n242, b_t *n241, b_t *n240,
			b_t *n239, b_t *n238, b_t *n237, b_t *n236, b_t *n235, b_t *n234, b_t *n233, b_t *n232,
			b_t *n231, b_t *n230, b_t *n229, b_t *n228, b_t *n227, b_t *n226, b_t *n225, b_t *n224,
			b_t *n223, b_t *n222, b_t *n221, b_t *n220, b_t *n219, b_t *n218, b_t *n217, b_t *n216,
			b_t *n215, b_t *n214, b_t *n213, b_t *n212, b_t *n211, b_t *n210, b_t *n209, b_t *n208,
			b_t *n207, b_t *n206, b_t *n205, b_t *n204, b_t *n203, b_t *n202, b_t *n201, b_t *n200,
			b_t *n199, b_t *n198, b_t *n197, b_t *n196, b_t *n195, b_t *n194, b_t *n193, b_t *n192,
			b_t *n191, b_t *n190, b_t *n189, b_t *n188, b_t *n187, b_t *n186, b_t *n185, b_t *n184,
			b_t *n183, b_t *n182, b_t *n181, b_t *n180, b_t *n179, b_t *n178, b_t *n177, b_t *n176,
			b_t *n175, b_t *n174, b_t *n173, b_t *n172, b_t *n171, b_t *n170, b_t *n169, b_t *n168,
			b_t *n167, b_t *n166, b_t *n165, b_t *n164, b_t *n163, b_t *n162, b_t *n161, b_t *n160,
			b_t *n159, b_t *n158, b_t *n157, b_t *n156, b_t *n155, b_t *n154, b_t *n153, b_t *n152,
			b_t *n151, b_t *n150, b_t *n149, b_t *n148, b_t *n147, b_t *n146, b_t *n145, b_t *n144,
			b_t *n143, b_t *n142, b_t *n141, b_t *n140, b_t *n139, b_t *n138, b_t *n137, b_t *n136,
			b_t *n135, b_t *n134, b_t *n133, b_t *n132, b_t *n131, b_t *n130, b_t *n129, b_t *n128, 
			b_t *n127, b_t *n126, b_t *n125, b_t *n124, b_t *n123, b_t *n122, b_t *n121, b_t *n120,
			b_t *n119, b_t *n118, b_t *n117, b_t *n116, b_t *n115, b_t *n114, b_t *n113, b_t *n112,
			b_t *n111, b_t *n110, b_t *n109, b_t *n108, b_t *n107, b_t *n106, b_t *n105, b_t *n104,
			b_t *n103, b_t *n102, b_t *n101, b_t *n100, b_t *n99, b_t *n98, b_t *n97, b_t *n96,
			b_t *n95, b_t *n94, b_t *n93, b_t *n92, b_t *n91, b_t *n90, b_t *n89, b_t *n88,
			b_t *n87, b_t *n86, b_t *n85, b_t *n84, b_t *n83, b_t *n82, b_t *n81, b_t *n80,
			b_t *n79, b_t *n78, b_t *n77, b_t *n76, b_t *n75, b_t *n74, b_t *n73, b_t *n72,
			b_t *n71, b_t *n70, b_t *n69, b_t *n68, b_t *n67, b_t *n66, b_t *n65, b_t *n64,
			b_t *n63, b_t *n62, b_t *n61, b_t *n60, b_t *n59, b_t *n58, b_t *n57, b_t *n56,
			b_t *n55, b_t *n54, b_t *n53, b_t *n52, b_t *n51, b_t *n50, b_t *n49, b_t *n48,
			b_t *n47, b_t *n46, b_t *n45, b_t *n44, b_t *n43, b_t *n42, b_t *n41, b_t *n40,
			b_t *n39, b_t *n38, b_t *n37, b_t *n36, b_t *n35, b_t *n34, b_t *n33, b_t *n32,
			b_t *n31, b_t *n30, b_t *n29, b_t *n28, b_t *n27, b_t *n26, b_t *n25, b_t *n24,
			b_t *n23, b_t *n22, b_t *n21, b_t *n20, b_t *n19, b_t *n18, b_t *n17, b_t *n16,
			b_t *n15, b_t *n14, b_t *n13, b_t *n12, b_t *n11, b_t *n10, b_t *n9, b_t *n8,
			b_t *n7, b_t *n6, b_t *n5, b_t *n4, b_t *n3, b_t *n2, b_t *n1, b_t *n0)
		{
			_uint_get(8, (b_t const *)this,
				n255, n254, n253, n252, n251, n250, n249, n248, n247, n246, n245, n244, n243, n242, n241, n240, n239, n238, n237, n236, n235, n234, n233, n232, n231, n230, n229, n228, n227, n226, n225, n224,
				n223, n222, n221, n220, n219, n218, n217, n216, n215, n214, n213, n212, n211, n210, n209, n208, n207, n206, n205, n204, n203, n202, n201, n200, n199, n198, n197, n196, n195, n194, n193, n192,
				n191, n190, n189, n188, n187, n186, n185, n184, n183, n182, n181, n180, n179, n178, n177, n176, n175, n174, n173, n172, n171, n170, n169, n168, n167, n166, n165, n164, n163, n162, n161, n160,
				n159, n158, n157, n156, n155, n154, n153, n152, n151, n150, n149, n148, n147, n146, n145, n144, n143, n142, n141, n140, n139, n138, n137, n136, n135, n134, n133, n132, n131, n130, n129, n128,
				n127, n126, n125, n124, n123, n122, n121, n120, n119, n118, n117, n116, n115, n114, n113, n112, n111, n110, n109, n108, n107, n106, n105, n104, n103, n102, n101, n100, n99, n98, n97, n96,
				n95, n94, n93, n92, n91, n90, n89, n88, n87, n86, n85, n84, n83, n82, n81, n80, n79, n78, n77, n76, n75, n74, n73, n72, n71, n70, n69, n68, n67, n66, n65, n64,
				n63, n62, n61, n60, n59, n58, n57, n56, n55, n54, n53, n52, n51, n50, n49, n48, n47, n46, n45, n44, n43, n42, n41, n40, n39, n38, n37, n36, n35, n34, n33, n32,
				n31, n30, n29, n28, n27, n26, n25, n24, n23, n22, n21, n20, n19, n18, n17, n16, n15, n14, n13, n12, n11, n10, n9, n8, n7, n6, n5, n4, n3, n2, n1, n0);
		}
		void espress_cipher_uint2048_va_set(struct espress_cipher_uint2048 *this, va_list *np) {_uint_va_set(8, (b_t *)this, np);}
		void espress_cipher_uint2048_va_get(struct espress_cipher_uint2048 const *this, va_list *np) {_uint_va_get(8, (b_t const *)this, np);}
		void espress_cipher_uint2048_set_u(struct espress_cipher_uint2048 *this, u_t n) {_uint_set_u(8, (b_t *)this, n);}
		void espress_cipher_uint2048_get_u(struct espress_cipher_uint2048 const *this, u_t *n) {_uint_get_u(8, (b_t const *)this, n);}
		void espress_cipher_uint2048_assign(struct espress_cipher_uint2048 *this, struct espress_cipher_uint2048 const *n) {_uint_assign(8, (b_t *)this, (b_t const *)n);}
		i_t espress_cipher_uint2048_add(struct espress_cipher_uint2048 *this, struct espress_cipher_uint2048 const *n) {return _uint_add(8, (b_t *)this, (b_t const *)n);}
		i_t espress_cipher_uint2048_sub(struct espress_cipher_uint2048 *this, struct espress_cipher_uint2048 const *n) {return _uint_sub(8, (b_t *)this, (b_t const *)n);}
		void espress_cipher_uint2048_mul(struct espress_cipher_uint2048 *this, struct espress_cipher_uint2048 const *n) {_uint_mul(8, (b_t *)this, (b_t const *)n);}
		void espress_cipher_uint2048_div(struct espress_cipher_uint2048 *this, struct espress_cipher_uint2048 const *n) {_uint_div(8, (b_t *)this, (b_t const *)n);}
		void espress_cipher_uint2048_and(struct espress_cipher_uint2048 *this, struct espress_cipher_uint2048 const *n) {_uint_and(8, (b_t *)this, (b_t const *)n);}
		void espress_cipher_uint2048_or(struct espress_cipher_uint2048 *this, struct espress_cipher_uint2048 const *n) {_uint_or(8, (b_t *)this, (b_t const *)n);}
		void espress_cipher_uint2048_xor(struct espress_cipher_uint2048 *this, struct espress_cipher_uint2048 const *n) {_uint_xor(8, (b_t *)this, (b_t const *)n);}
		void espress_cipher_uint2048_not(struct espress_cipher_uint2048 *this) {_uint_not(8, (b_t *)this);}
		void espress_cipher_uint2048_shift(struct espress_cipher_uint2048 *this, i_t n) {_uint_shift(8, (b_t *)this, n);}
		void espress_cipher_uint2048_rotate(struct espress_cipher_uint2048 *this, i_t n) {_uint_rotate(8, (b_t *)this, n);}
		i_t espress_cipher_uint2048_eq(struct espress_cipher_uint2048 const *this, struct espress_cipher_uint2048 const *n) {return _uint_eq(8, (b_t const *)this, (b_t const *)n);}
		i_t espress_cipher_uint2048_lt(struct espress_cipher_uint2048 const *this, struct espress_cipher_uint2048 const *n) {return _uint_lt(8, (b_t const *)this, (b_t const *)n);}
		i_t espress_cipher_uint2048_le(struct espress_cipher_uint2048 const *this, struct espress_cipher_uint2048 const *n) {return _uint_le(8, (b_t const *)this, (b_t const *)n);}
		void espress_cipher_uint2048_bswap(struct espress_cipher_uint2048 *this) {_uint_bswap(8, (b_t *)this);}
#	endif
#endif

#if 512 <= ESPRESS_CIPHER_UINT_MAX_WIDTH
	struct espress_cipher_uint4096 const espress_cipher_uint4096_0[] = {ESPRESS_CIPHER_UINT4096_0};
	struct espress_cipher_uint4096 const espress_cipher_uint4096_1[] = {ESPRESS_CIPHER_UINT4096_1};
	struct espress_cipher_uint4096 const espress_cipher_uint4096_max[] = {ESPRESS_CIPHER_UINT4096_MAX};
#	if 512 <= MAX_U_T_WIDTH
#	else
		void espress_cipher_uint4096_set(struct espress_cipher_uint4096 *this,
			b_t n511, b_t n510, b_t n509, b_t n508, b_t n507, b_t n506, b_t n505, b_t n504,
			b_t n503, b_t n502, b_t n501, b_t n500, b_t n499, b_t n498, b_t n497, b_t n496,
			b_t n495, b_t n494, b_t n493, b_t n492, b_t n491, b_t n490, b_t n489, b_t n488,
			b_t n487, b_t n486, b_t n485, b_t n484, b_t n483, b_t n482, b_t n481, b_t n480,
			b_t n479, b_t n478, b_t n477, b_t n476, b_t n475, b_t n474, b_t n473, b_t n472,
			b_t n471, b_t n470, b_t n469, b_t n468, b_t n467, b_t n466, b_t n465, b_t n464,
			b_t n463, b_t n462, b_t n461, b_t n460, b_t n459, b_t n458, b_t n457, b_t n456,
			b_t n455, b_t n454, b_t n453, b_t n452, b_t n451, b_t n450, b_t n449, b_t n448,
			b_t n447, b_t n446, b_t n445, b_t n444, b_t n443, b_t n442, b_t n441, b_t n440,
			b_t n439, b_t n438, b_t n437, b_t n436, b_t n435, b_t n434, b_t n433, b_t n432,
			b_t n431, b_t n430, b_t n429, b_t n428, b_t n427, b_t n426, b_t n425, b_t n424,
			b_t n423, b_t n422, b_t n421, b_t n420, b_t n419, b_t n418, b_t n417, b_t n416,
			b_t n415, b_t n414, b_t n413, b_t n412, b_t n411, b_t n410, b_t n409, b_t n408,
			b_t n407, b_t n406, b_t n405, b_t n404, b_t n403, b_t n402, b_t n401, b_t n400,
			b_t n399, b_t n398, b_t n397, b_t n396, b_t n395, b_t n394, b_t n393, b_t n392,
			b_t n391, b_t n390, b_t n389, b_t n388, b_t n387, b_t n386, b_t n385, b_t n384,
			b_t n383, b_t n382, b_t n381, b_t n380, b_t n379, b_t n378, b_t n377, b_t n376,
			b_t n375, b_t n374, b_t n373, b_t n372, b_t n371, b_t n370, b_t n369, b_t n368,
			b_t n367, b_t n366, b_t n365, b_t n364, b_t n363, b_t n362, b_t n361, b_t n360,
			b_t n359, b_t n358, b_t n357, b_t n356, b_t n355, b_t n354, b_t n353, b_t n352,
			b_t n351, b_t n350, b_t n349, b_t n348, b_t n347, b_t n346, b_t n345, b_t n344,
			b_t n343, b_t n342, b_t n341, b_t n340, b_t n339, b_t n338, b_t n337, b_t n336,
			b_t n335, b_t n334, b_t n333, b_t n332, b_t n331, b_t n330, b_t n329, b_t n328,
			b_t n327, b_t n326, b_t n325, b_t n324, b_t n323, b_t n322, b_t n321, b_t n320,
			b_t n319, b_t n318, b_t n317, b_t n316, b_t n315, b_t n314, b_t n313, b_t n312,
			b_t n311, b_t n310, b_t n309, b_t n308, b_t n307, b_t n306, b_t n305, b_t n304,
			b_t n303, b_t n302, b_t n301, b_t n300, b_t n299, b_t n298, b_t n297, b_t n296,
			b_t n295, b_t n294, b_t n293, b_t n292, b_t n291, b_t n290, b_t n289, b_t n288,
			b_t n287, b_t n286, b_t n285, b_t n284, b_t n283, b_t n282, b_t n281, b_t n280,
			b_t n279, b_t n278, b_t n277, b_t n276, b_t n275, b_t n274, b_t n273, b_t n272,
			b_t n271, b_t n270, b_t n269, b_t n268, b_t n267, b_t n266, b_t n265, b_t n264,
			b_t n263, b_t n262, b_t n261, b_t n260, b_t n259, b_t n258, b_t n257, b_t n256,
			b_t n255, b_t n254, b_t n253, b_t n252, b_t n251, b_t n250, b_t n249, b_t n248,
			b_t n247, b_t n246, b_t n245, b_t n244, b_t n243, b_t n242, b_t n241, b_t n240,
			b_t n239, b_t n238, b_t n237, b_t n236, b_t n235, b_t n234, b_t n233, b_t n232,
			b_t n231, b_t n230, b_t n229, b_t n228, b_t n227, b_t n226, b_t n225, b_t n224,
			b_t n223, b_t n222, b_t n221, b_t n220, b_t n219, b_t n218, b_t n217, b_t n216,
			b_t n215, b_t n214, b_t n213, b_t n212, b_t n211, b_t n210, b_t n209, b_t n208,
			b_t n207, b_t n206, b_t n205, b_t n204, b_t n203, b_t n202, b_t n201, b_t n200,
			b_t n199, b_t n198, b_t n197, b_t n196, b_t n195, b_t n194, b_t n193, b_t n192,
			b_t n191, b_t n190, b_t n189, b_t n188, b_t n187, b_t n186, b_t n185, b_t n184,
			b_t n183, b_t n182, b_t n181, b_t n180, b_t n179, b_t n178, b_t n177, b_t n176,
			b_t n175, b_t n174, b_t n173, b_t n172, b_t n171, b_t n170, b_t n169, b_t n168,
			b_t n167, b_t n166, b_t n165, b_t n164, b_t n163, b_t n162, b_t n161, b_t n160,
			b_t n159, b_t n158, b_t n157, b_t n156, b_t n155, b_t n154, b_t n153, b_t n152,
			b_t n151, b_t n150, b_t n149, b_t n148, b_t n147, b_t n146, b_t n145, b_t n144,
			b_t n143, b_t n142, b_t n141, b_t n140, b_t n139, b_t n138, b_t n137, b_t n136,
			b_t n135, b_t n134, b_t n133, b_t n132, b_t n131, b_t n130, b_t n129, b_t n128, 
			b_t n127, b_t n126, b_t n125, b_t n124, b_t n123, b_t n122, b_t n121, b_t n120,
			b_t n119, b_t n118, b_t n117, b_t n116, b_t n115, b_t n114, b_t n113, b_t n112,
			b_t n111, b_t n110, b_t n109, b_t n108, b_t n107, b_t n106, b_t n105, b_t n104,
			b_t n103, b_t n102, b_t n101, b_t n100, b_t n99, b_t n98, b_t n97, b_t n96,
			b_t n95, b_t n94, b_t n93, b_t n92, b_t n91, b_t n90, b_t n89, b_t n88,
			b_t n87, b_t n86, b_t n85, b_t n84, b_t n83, b_t n82, b_t n81, b_t n80,
			b_t n79, b_t n78, b_t n77, b_t n76, b_t n75, b_t n74, b_t n73, b_t n72,
			b_t n71, b_t n70, b_t n69, b_t n68, b_t n67, b_t n66, b_t n65, b_t n64,
			b_t n63, b_t n62, b_t n61, b_t n60, b_t n59, b_t n58, b_t n57, b_t n56,
			b_t n55, b_t n54, b_t n53, b_t n52, b_t n51, b_t n50, b_t n49, b_t n48,
			b_t n47, b_t n46, b_t n45, b_t n44, b_t n43, b_t n42, b_t n41, b_t n40,
			b_t n39, b_t n38, b_t n37, b_t n36, b_t n35, b_t n34, b_t n33, b_t n32,
			b_t n31, b_t n30, b_t n29, b_t n28, b_t n27, b_t n26, b_t n25, b_t n24,
			b_t n23, b_t n22, b_t n21, b_t n20, b_t n19, b_t n18, b_t n17, b_t n16,
			b_t n15, b_t n14, b_t n13, b_t n12, b_t n11, b_t n10, b_t n9, b_t n8,
			b_t n7, b_t n6, b_t n5, b_t n4, b_t n3, b_t n2, b_t n1, b_t n0)
		{
			_uint_set(9, (b_t *)this,
				n511, n510, n509, n508, n507, n506, n505, n504, n503, n502, n501, n500, n499, n498, n497, n496, n495, n494, n493, n492, n491, n490, n489, n488, n487, n486, n485, n484, n483, n482, n481, n480,
				n479, n478, n477, n476, n475, n474, n473, n472, n471, n470, n469, n468, n467, n466, n465, n464, n463, n462, n461, n460, n459, n458, n457, n456, n455, n454, n453, n452, n451, n450, n449, n448,
				n447, n446, n445, n444, n443, n442, n441, n440, n439, n438, n437, n436, n435, n434, n433, n432, n431, n430, n429, n428, n427, n426, n425, n424, n423, n422, n421, n420, n419, n418, n417, n416,
				n415, n414, n413, n412, n411, n410, n409, n408, n407, n406, n405, n404, n403, n402, n401, n400, n399, n398, n397, n396, n395, n394, n393, n392, n391, n390, n389, n388, n387, n386, n385, n384,
				n383, n382, n381, n380, n379, n378, n377, n376, n375, n374, n373, n372, n371, n370, n369, n368, n367, n366, n365, n364, n363, n362, n361, n360, n359, n358, n357, n356, n355, n354, n353, n352,
				n351, n350, n349, n348, n347, n346, n345, n344, n343, n342, n341, n340, n339, n338, n337, n336, n335, n334, n333, n332, n331, n330, n329, n328, n327, n326, n325, n324, n323, n322, n321, n320,
				n319, n318, n317, n316, n315, n314, n313, n312, n311, n310, n309, n308, n307, n306, n305, n304, n303, n302, n301, n300, n299, n298, n297, n296, n295, n294, n293, n292, n291, n290, n289, n288,
				n287, n286, n285, n284, n283, n282, n281, n280, n279, n278, n277, n276, n275, n274, n273, n272, n271, n270, n269, n268, n267, n266, n265, n264, n263, n262, n261, n260, n259, n258, n257, n256,
				n255, n254, n253, n252, n251, n250, n249, n248, n247, n246, n245, n244, n243, n242, n241, n240, n239, n238, n237, n236, n235, n234, n233, n232, n231, n230, n229, n228, n227, n226, n225, n224,
				n223, n222, n221, n220, n219, n218, n217, n216, n215, n214, n213, n212, n211, n210, n209, n208, n207, n206, n205, n204, n203, n202, n201, n200, n199, n198, n197, n196, n195, n194, n193, n192,
				n191, n190, n189, n188, n187, n186, n185, n184, n183, n182, n181, n180, n179, n178, n177, n176, n175, n174, n173, n172, n171, n170, n169, n168, n167, n166, n165, n164, n163, n162, n161, n160,
				n159, n158, n157, n156, n155, n154, n153, n152, n151, n150, n149, n148, n147, n146, n145, n144, n143, n142, n141, n140, n139, n138, n137, n136, n135, n134, n133, n132, n131, n130, n129, n128,
				n127, n126, n125, n124, n123, n122, n121, n120, n119, n118, n117, n116, n115, n114, n113, n112, n111, n110, n109, n108, n107, n106, n105, n104, n103, n102, n101, n100, n99, n98, n97, n96,
				n95, n94, n93, n92, n91, n90, n89, n88, n87, n86, n85, n84, n83, n82, n81, n80, n79, n78, n77, n76, n75, n74, n73, n72, n71, n70, n69, n68, n67, n66, n65, n64,
				n63, n62, n61, n60, n59, n58, n57, n56, n55, n54, n53, n52, n51, n50, n49, n48, n47, n46, n45, n44, n43, n42, n41, n40, n39, n38, n37, n36, n35, n34, n33, n32,
				n31, n30, n29, n28, n27, n26, n25, n24, n23, n22, n21, n20, n19, n18, n17, n16, n15, n14, n13, n12, n11, n10, n9, n8, n7, n6, n5, n4, n3, n2, n1, n0);
		}
		void espress_cipher_uint4096_get(struct espress_cipher_uint4096 const *this,
			b_t *n511, b_t *n510, b_t *n509, b_t *n508, b_t *n507, b_t *n506, b_t *n505, b_t *n504,
			b_t *n503, b_t *n502, b_t *n501, b_t *n500, b_t *n499, b_t *n498, b_t *n497, b_t *n496,
			b_t *n495, b_t *n494, b_t *n493, b_t *n492, b_t *n491, b_t *n490, b_t *n489, b_t *n488,
			b_t *n487, b_t *n486, b_t *n485, b_t *n484, b_t *n483, b_t *n482, b_t *n481, b_t *n480,
			b_t *n479, b_t *n478, b_t *n477, b_t *n476, b_t *n475, b_t *n474, b_t *n473, b_t *n472,
			b_t *n471, b_t *n470, b_t *n469, b_t *n468, b_t *n467, b_t *n466, b_t *n465, b_t *n464,
			b_t *n463, b_t *n462, b_t *n461, b_t *n460, b_t *n459, b_t *n458, b_t *n457, b_t *n456,
			b_t *n455, b_t *n454, b_t *n453, b_t *n452, b_t *n451, b_t *n450, b_t *n449, b_t *n448,
			b_t *n447, b_t *n446, b_t *n445, b_t *n444, b_t *n443, b_t *n442, b_t *n441, b_t *n440,
			b_t *n439, b_t *n438, b_t *n437, b_t *n436, b_t *n435, b_t *n434, b_t *n433, b_t *n432,
			b_t *n431, b_t *n430, b_t *n429, b_t *n428, b_t *n427, b_t *n426, b_t *n425, b_t *n424,
			b_t *n423, b_t *n422, b_t *n421, b_t *n420, b_t *n419, b_t *n418, b_t *n417, b_t *n416,
			b_t *n415, b_t *n414, b_t *n413, b_t *n412, b_t *n411, b_t *n410, b_t *n409, b_t *n408,
			b_t *n407, b_t *n406, b_t *n405, b_t *n404, b_t *n403, b_t *n402, b_t *n401, b_t *n400,
			b_t *n399, b_t *n398, b_t *n397, b_t *n396, b_t *n395, b_t *n394, b_t *n393, b_t *n392,
			b_t *n391, b_t *n390, b_t *n389, b_t *n388, b_t *n387, b_t *n386, b_t *n385, b_t *n384,
			b_t *n383, b_t *n382, b_t *n381, b_t *n380, b_t *n379, b_t *n378, b_t *n377, b_t *n376,
			b_t *n375, b_t *n374, b_t *n373, b_t *n372, b_t *n371, b_t *n370, b_t *n369, b_t *n368,
			b_t *n367, b_t *n366, b_t *n365, b_t *n364, b_t *n363, b_t *n362, b_t *n361, b_t *n360,
			b_t *n359, b_t *n358, b_t *n357, b_t *n356, b_t *n355, b_t *n354, b_t *n353, b_t *n352,
			b_t *n351, b_t *n350, b_t *n349, b_t *n348, b_t *n347, b_t *n346, b_t *n345, b_t *n344,
			b_t *n343, b_t *n342, b_t *n341, b_t *n340, b_t *n339, b_t *n338, b_t *n337, b_t *n336,
			b_t *n335, b_t *n334, b_t *n333, b_t *n332, b_t *n331, b_t *n330, b_t *n329, b_t *n328,
			b_t *n327, b_t *n326, b_t *n325, b_t *n324, b_t *n323, b_t *n322, b_t *n321, b_t *n320,
			b_t *n319, b_t *n318, b_t *n317, b_t *n316, b_t *n315, b_t *n314, b_t *n313, b_t *n312,
			b_t *n311, b_t *n310, b_t *n309, b_t *n308, b_t *n307, b_t *n306, b_t *n305, b_t *n304,
			b_t *n303, b_t *n302, b_t *n301, b_t *n300, b_t *n299, b_t *n298, b_t *n297, b_t *n296,
			b_t *n295, b_t *n294, b_t *n293, b_t *n292, b_t *n291, b_t *n290, b_t *n289, b_t *n288,
			b_t *n287, b_t *n286, b_t *n285, b_t *n284, b_t *n283, b_t *n282, b_t *n281, b_t *n280,
			b_t *n279, b_t *n278, b_t *n277, b_t *n276, b_t *n275, b_t *n274, b_t *n273, b_t *n272,
			b_t *n271, b_t *n270, b_t *n269, b_t *n268, b_t *n267, b_t *n266, b_t *n265, b_t *n264,
			b_t *n263, b_t *n262, b_t *n261, b_t *n260, b_t *n259, b_t *n258, b_t *n257, b_t *n256, 
			b_t *n255, b_t *n254, b_t *n253, b_t *n252, b_t *n251, b_t *n250, b_t *n249, b_t *n248,
			b_t *n247, b_t *n246, b_t *n245, b_t *n244, b_t *n243, b_t *n242, b_t *n241, b_t *n240,
			b_t *n239, b_t *n238, b_t *n237, b_t *n236, b_t *n235, b_t *n234, b_t *n233, b_t *n232,
			b_t *n231, b_t *n230, b_t *n229, b_t *n228, b_t *n227, b_t *n226, b_t *n225, b_t *n224,
			b_t *n223, b_t *n222, b_t *n221, b_t *n220, b_t *n219, b_t *n218, b_t *n217, b_t *n216,
			b_t *n215, b_t *n214, b_t *n213, b_t *n212, b_t *n211, b_t *n210, b_t *n209, b_t *n208,
			b_t *n207, b_t *n206, b_t *n205, b_t *n204, b_t *n203, b_t *n202, b_t *n201, b_t *n200,
			b_t *n199, b_t *n198, b_t *n197, b_t *n196, b_t *n195, b_t *n194, b_t *n193, b_t *n192,
			b_t *n191, b_t *n190, b_t *n189, b_t *n188, b_t *n187, b_t *n186, b_t *n185, b_t *n184,
			b_t *n183, b_t *n182, b_t *n181, b_t *n180, b_t *n179, b_t *n178, b_t *n177, b_t *n176,
			b_t *n175, b_t *n174, b_t *n173, b_t *n172, b_t *n171, b_t *n170, b_t *n169, b_t *n168,
			b_t *n167, b_t *n166, b_t *n165, b_t *n164, b_t *n163, b_t *n162, b_t *n161, b_t *n160,
			b_t *n159, b_t *n158, b_t *n157, b_t *n156, b_t *n155, b_t *n154, b_t *n153, b_t *n152,
			b_t *n151, b_t *n150, b_t *n149, b_t *n148, b_t *n147, b_t *n146, b_t *n145, b_t *n144,
			b_t *n143, b_t *n142, b_t *n141, b_t *n140, b_t *n139, b_t *n138, b_t *n137, b_t *n136,
			b_t *n135, b_t *n134, b_t *n133, b_t *n132, b_t *n131, b_t *n130, b_t *n129, b_t *n128, 
			b_t *n127, b_t *n126, b_t *n125, b_t *n124, b_t *n123, b_t *n122, b_t *n121, b_t *n120,
			b_t *n119, b_t *n118, b_t *n117, b_t *n116, b_t *n115, b_t *n114, b_t *n113, b_t *n112,
			b_t *n111, b_t *n110, b_t *n109, b_t *n108, b_t *n107, b_t *n106, b_t *n105, b_t *n104,
			b_t *n103, b_t *n102, b_t *n101, b_t *n100, b_t *n99, b_t *n98, b_t *n97, b_t *n96,
			b_t *n95, b_t *n94, b_t *n93, b_t *n92, b_t *n91, b_t *n90, b_t *n89, b_t *n88,
			b_t *n87, b_t *n86, b_t *n85, b_t *n84, b_t *n83, b_t *n82, b_t *n81, b_t *n80,
			b_t *n79, b_t *n78, b_t *n77, b_t *n76, b_t *n75, b_t *n74, b_t *n73, b_t *n72,
			b_t *n71, b_t *n70, b_t *n69, b_t *n68, b_t *n67, b_t *n66, b_t *n65, b_t *n64,
			b_t *n63, b_t *n62, b_t *n61, b_t *n60, b_t *n59, b_t *n58, b_t *n57, b_t *n56,
			b_t *n55, b_t *n54, b_t *n53, b_t *n52, b_t *n51, b_t *n50, b_t *n49, b_t *n48,
			b_t *n47, b_t *n46, b_t *n45, b_t *n44, b_t *n43, b_t *n42, b_t *n41, b_t *n40,
			b_t *n39, b_t *n38, b_t *n37, b_t *n36, b_t *n35, b_t *n34, b_t *n33, b_t *n32,
			b_t *n31, b_t *n30, b_t *n29, b_t *n28, b_t *n27, b_t *n26, b_t *n25, b_t *n24,
			b_t *n23, b_t *n22, b_t *n21, b_t *n20, b_t *n19, b_t *n18, b_t *n17, b_t *n16,
			b_t *n15, b_t *n14, b_t *n13, b_t *n12, b_t *n11, b_t *n10, b_t *n9, b_t *n8,
			b_t *n7, b_t *n6, b_t *n5, b_t *n4, b_t *n3, b_t *n2, b_t *n1, b_t *n0)
		{
			_uint_get(9, (b_t const *)this,
				n511, n510, n509, n508, n507, n506, n505, n504, n503, n502, n501, n500, n499, n498, n497, n496, n495, n494, n493, n492, n491, n490, n489, n488, n487, n486, n485, n484, n483, n482, n481, n480,
				n479, n478, n477, n476, n475, n474, n473, n472, n471, n470, n469, n468, n467, n466, n465, n464, n463, n462, n461, n460, n459, n458, n457, n456, n455, n454, n453, n452, n451, n450, n449, n448,
				n447, n446, n445, n444, n443, n442, n441, n440, n439, n438, n437, n436, n435, n434, n433, n432, n431, n430, n429, n428, n427, n426, n425, n424, n423, n422, n421, n420, n419, n418, n417, n416,
				n415, n414, n413, n412, n411, n410, n409, n408, n407, n406, n405, n404, n403, n402, n401, n400, n399, n398, n397, n396, n395, n394, n393, n392, n391, n390, n389, n388, n387, n386, n385, n384,
				n383, n382, n381, n380, n379, n378, n377, n376, n375, n374, n373, n372, n371, n370, n369, n368, n367, n366, n365, n364, n363, n362, n361, n360, n359, n358, n357, n356, n355, n354, n353, n352,
				n351, n350, n349, n348, n347, n346, n345, n344, n343, n342, n341, n340, n339, n338, n337, n336, n335, n334, n333, n332, n331, n330, n329, n328, n327, n326, n325, n324, n323, n322, n321, n320,
				n319, n318, n317, n316, n315, n314, n313, n312, n311, n310, n309, n308, n307, n306, n305, n304, n303, n302, n301, n300, n299, n298, n297, n296, n295, n294, n293, n292, n291, n290, n289, n288,
				n287, n286, n285, n284, n283, n282, n281, n280, n279, n278, n277, n276, n275, n274, n273, n272, n271, n270, n269, n268, n267, n266, n265, n264, n263, n262, n261, n260, n259, n258, n257, n256,
				n255, n254, n253, n252, n251, n250, n249, n248, n247, n246, n245, n244, n243, n242, n241, n240, n239, n238, n237, n236, n235, n234, n233, n232, n231, n230, n229, n228, n227, n226, n225, n224,
				n223, n222, n221, n220, n219, n218, n217, n216, n215, n214, n213, n212, n211, n210, n209, n208, n207, n206, n205, n204, n203, n202, n201, n200, n199, n198, n197, n196, n195, n194, n193, n192,
				n191, n190, n189, n188, n187, n186, n185, n184, n183, n182, n181, n180, n179, n178, n177, n176, n175, n174, n173, n172, n171, n170, n169, n168, n167, n166, n165, n164, n163, n162, n161, n160,
				n159, n158, n157, n156, n155, n154, n153, n152, n151, n150, n149, n148, n147, n146, n145, n144, n143, n142, n141, n140, n139, n138, n137, n136, n135, n134, n133, n132, n131, n130, n129, n128,
				n127, n126, n125, n124, n123, n122, n121, n120, n119, n118, n117, n116, n115, n114, n113, n112, n111, n110, n109, n108, n107, n106, n105, n104, n103, n102, n101, n100, n99, n98, n97, n96,
				n95, n94, n93, n92, n91, n90, n89, n88, n87, n86, n85, n84, n83, n82, n81, n80, n79, n78, n77, n76, n75, n74, n73, n72, n71, n70, n69, n68, n67, n66, n65, n64,
				n63, n62, n61, n60, n59, n58, n57, n56, n55, n54, n53, n52, n51, n50, n49, n48, n47, n46, n45, n44, n43, n42, n41, n40, n39, n38, n37, n36, n35, n34, n33, n32,
				n31, n30, n29, n28, n27, n26, n25, n24, n23, n22, n21, n20, n19, n18, n17, n16, n15, n14, n13, n12, n11, n10, n9, n8, n7, n6, n5, n4, n3, n2, n1, n0);
		}
		void espress_cipher_uint4096_va_set(struct espress_cipher_uint4096 *this, va_list *np) {_uint_va_set(9, (b_t *)this, np);}
		void espress_cipher_uint4096_va_get(struct espress_cipher_uint4096 const *this, va_list *np) {_uint_va_get(9, (b_t const *)this, np);}
		void espress_cipher_uint4096_set_u(struct espress_cipher_uint4096 *this, u_t n) {_uint_set_u(9, (b_t *)this, n);}
		void espress_cipher_uint4096_get_u(struct espress_cipher_uint4096 const *this, u_t *n) {_uint_get_u(9, (b_t const *)this, n);}
		void espress_cipher_uint4096_assign(struct espress_cipher_uint4096 *this, struct espress_cipher_uint4096 const *n) {_uint_assign(9, (b_t *)this, (b_t const *)n);}
		i_t espress_cipher_uint4096_add(struct espress_cipher_uint4096 *this, struct espress_cipher_uint4096 const *n) {return _uint_add(9, (b_t *)this, (b_t const *)n);}
		i_t espress_cipher_uint4096_sub(struct espress_cipher_uint4096 *this, struct espress_cipher_uint4096 const *n) {return _uint_sub(9, (b_t *)this, (b_t const *)n);}
		void espress_cipher_uint4096_mul(struct espress_cipher_uint4096 *this, struct espress_cipher_uint4096 const *n) {_uint_mul(9, (b_t *)this, (b_t const *)n);}
		void espress_cipher_uint4096_div(struct espress_cipher_uint4096 *this, struct espress_cipher_uint4096 const *n) {_uint_div(9, (b_t *)this, (b_t const *)n);}
		void espress_cipher_uint4096_and(struct espress_cipher_uint4096 *this, struct espress_cipher_uint4096 const *n) {_uint_and(9, (b_t *)this, (b_t const *)n);}
		void espress_cipher_uint4096_or(struct espress_cipher_uint4096 *this, struct espress_cipher_uint4096 const *n) {_uint_or(9, (b_t *)this, (b_t const *)n);}
		void espress_cipher_uint4096_xor(struct espress_cipher_uint4096 *this, struct espress_cipher_uint4096 const *n) {_uint_xor(9, (b_t *)this, (b_t const *)n);}
		void espress_cipher_uint4096_not(struct espress_cipher_uint4096 *this) {_uint_not(9, (b_t *)this);}
		void espress_cipher_uint4096_shift(struct espress_cipher_uint4096 *this, i_t n) {_uint_shift(9, (b_t *)this, n);}
		void espress_cipher_uint4096_rotate(struct espress_cipher_uint4096 *this, i_t n) {_uint_rotate(9, (b_t *)this, n);}
		i_t espress_cipher_uint4096_eq(struct espress_cipher_uint4096 const *this, struct espress_cipher_uint4096 const *n) {return _uint_eq(9, (b_t const *)this, (b_t const *)n);}
		i_t espress_cipher_uint4096_lt(struct espress_cipher_uint4096 const *this, struct espress_cipher_uint4096 const *n) {return _uint_lt(9, (b_t const *)this, (b_t const *)n);}
		i_t espress_cipher_uint4096_le(struct espress_cipher_uint4096 const *this, struct espress_cipher_uint4096 const *n) {return _uint_le(9, (b_t const *)this, (b_t const *)n);}
		void espress_cipher_uint4096_bswap(struct espress_cipher_uint4096 *this) {_uint_bswap(9, (b_t *)this);}
#	endif
#endif
