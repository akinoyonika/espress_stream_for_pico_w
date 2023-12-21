#include "espress_stream/config.h"
#include "espress_stream/depend.h"
#include "espress_stream/uint.h"
#include "espress_stream/ec.h"

#if ESPRESS_STREAM_IS_HOST_BYTE_ORDER_LITTLE
#define _O(w, n) (n)
#else
#define _O(w, n) ((1 << (w)) - 1 - (n))
#endif

static void _smul(b_t *u, b_t const *k);
static void _prepare_key(b_t *k);

struct espress_stream_ec_ops const espress_stream_ec[] =
{{
	_smul,
	_prepare_key
}};

static void _select(b_t *zero, b_t const *one, u_t condition)
{
	struct espress_stream_uint256 u[1] = {ESPRESS_STREAM_UINT256_0};
	struct espress_stream_uint256 v[1];

	condition ^= 1;
	espress_stream_uint256_set_u(v, condition);
	espress_stream_uint256_sub(u, v);

	espress_stream_uint256_xor((struct espress_stream_uint256 *)zero, (struct espress_stream_uint256 *)one);
	espress_stream_uint256_and((struct espress_stream_uint256 *)zero, u);
	espress_stream_uint256_xor((struct espress_stream_uint256 *)zero, (struct espress_stream_uint256 *)one);
}

static void _normalize(b_t *x)
{
	i_t i;
	struct espress_stream_uint16 u[1];
	struct espress_stream_uint16 v[1];
	struct espress_stream_uint16 c[1];
	b_t minusp[ESPRESS_STREAM_EC_SIZE];

	espress_stream_uint16_set_u(c, (x[ESPRESS_STREAM_EC_SIZE - 1] >> 7) * 19);
	x[ESPRESS_STREAM_EC_SIZE - 1] &= 127;

	for (i = 0 ; i < ESPRESS_STREAM_EC_SIZE ; i++)
	{
		espress_stream_uint16_set_u(u, x[i]);
		espress_stream_uint16_add(c, u);
		x[i] = ((b_t *)c)[_O(1, 0)];
		espress_stream_uint16_shift(c, -8);
	}

	espress_stream_uint16_set_u(c, 19);

	for (i = 0 ; i < (ESPRESS_STREAM_EC_SIZE - 1) ; i++)
	{
		espress_stream_uint16_set_u(u, x[i]);
		espress_stream_uint16_add(c, u);
		minusp[i] = ((b_t *)c)[_O(1, 0)];
		espress_stream_uint16_shift(c, -8);
	}

	espress_stream_uint16_set_u(u, x[i]);
	espress_stream_uint16_set_u(v, 128);
	espress_stream_uint16_sub(u, v);
	espress_stream_uint16_add(c, u);
	minusp[i] = ((b_t *)c)[_O(1, 0)];

	espress_stream_uint16_shift(c, -15);
	espress_stream_uint16_and(c, espress_stream_uint16_1);
	_select(x, minusp, ((b_t *)c)[_O(1, 0)] ^ 1);
}

static void _add(b_t *r, b_t const *x, b_t const *y)
{
	i_t i;
	struct espress_stream_uint16 u[1];
	struct espress_stream_uint16 c[1] = {ESPRESS_STREAM_UINT16_0};

	for (i = 0 ; i < ESPRESS_STREAM_EC_SIZE ; i++)
	{
		espress_stream_uint16_shift(c, -8);
		espress_stream_uint16_set_u(u, x[i]);
		espress_stream_uint16_add(c, u);
		espress_stream_uint16_set_u(u, y[i]);
		espress_stream_uint16_add(c, u);
		r[i] = ((b_t *)c)[_O(1, 0)];
	}

	r[ESPRESS_STREAM_EC_SIZE - 1] &= 127;
	espress_stream_uint16_shift(c, -7);
	espress_stream_uint16_set_u(u, 19);
	espress_stream_uint16_mul(c, u);

	for (i = 0 ; i < ESPRESS_STREAM_EC_SIZE ; i++)
	{
		espress_stream_uint16_set_u(u, r[i]);
		espress_stream_uint16_add(c, u);
		r[i] = ((b_t *)c)[_O(1, 0)];
		espress_stream_uint16_shift(c, -8);
	}
}

static void _sub(b_t *r, b_t const *x, b_t const *y)
{
	i_t i;
	struct espress_stream_uint32 c[1];
	struct espress_stream_uint32 u[1];
	struct espress_stream_uint32 v[1];

	espress_stream_uint32_set_u(c, 218);
	for (i = 0 ; i < (ESPRESS_STREAM_EC_SIZE - 1) ; i++)
	{
		espress_stream_uint32_set_u(u, x[i]);
		espress_stream_uint32_set_u(v, y[i]);
		espress_stream_uint32_sub(u, v);
		espress_stream_uint32_set_u(v, 65280);
		espress_stream_uint32_add(u, v);
		espress_stream_uint32_add(c, u);
		r[i] = ((b_t *)c)[_O(2, 0)];
		espress_stream_uint32_shift(c, -8);
	}

	espress_stream_uint32_set_u(u, x[i]);
	espress_stream_uint32_set_u(v, y[i]);
	espress_stream_uint32_sub(u, v);
	espress_stream_uint32_add(c, u);
	r[i] = ((b_t *)c)[_O(2, 0)] & 127;
	espress_stream_uint32_shift(c, -7);
	espress_stream_uint32_set_u(u, 19);
	espress_stream_uint32_mul(c, u);

	for (i = 0 ; i < ESPRESS_STREAM_EC_SIZE ; i++)
	{
		espress_stream_uint32_set_u(u, r[i]);
		espress_stream_uint32_add(c, u);
		r[i] = ((b_t *)c)[_O(2, 0)];
		espress_stream_uint32_shift(c, -8);
	}
}

static void _mul(b_t *r, b_t const *x, b_t const *y)
{
	i_t i;
	i_t j;
	struct espress_stream_uint32 c[1] = {ESPRESS_STREAM_UINT32_0};
	struct espress_stream_uint32 u[1];
	struct espress_stream_uint32 v[1];

	for (i = 0 ; i < ESPRESS_STREAM_EC_SIZE ; i++)
	{
		espress_stream_uint32_shift(c, -8);

		for (j = 0 ; j <= i ; j++)
		{
			espress_stream_uint32_set_u(u, x[j]);
			espress_stream_uint32_set_u(v, y[i - j]);
			espress_stream_uint32_mul(u, v);
			espress_stream_uint32_add(c, u);
		}

		for ( ; j < ESPRESS_STREAM_EC_SIZE ; j++)
		{
			espress_stream_uint32_set_u(u, x[j]);
			espress_stream_uint32_set_u(v, y[i + ESPRESS_STREAM_EC_SIZE - j]);
			espress_stream_uint32_mul(u, v);
			espress_stream_uint32_set_u(v, 38);
			espress_stream_uint32_mul(u, v);
			espress_stream_uint32_add(c, u);
		}

		r[i] = ((b_t *)c)[_O(2, 0)];
	}

	r[ESPRESS_STREAM_EC_SIZE - 1] &= 127;
	espress_stream_uint32_shift(c, -7);
	espress_stream_uint32_set_u(u, 19);
	espress_stream_uint32_mul(c, u);

	for (i = 0 ; i < ESPRESS_STREAM_EC_SIZE ; i ++)
	{
		espress_stream_uint32_set_u(u, r[i]);
		espress_stream_uint32_add(c, u);
		r[i] = ((b_t *)c)[_O(2, 0)];
		espress_stream_uint32_shift(c, -8);
	}
}

static void _mul_b(b_t *r, b_t const *x, b_t y3, b_t y2, b_t y1, b_t y0)
{
	i_t i;
	struct espress_stream_uint32 c[1] = {ESPRESS_STREAM_UINT32_0};
	struct espress_stream_uint32 u[1];
	struct espress_stream_uint32 v[1];

	for (i = 0 ; i < ESPRESS_STREAM_EC_SIZE ; i ++)
	{
		espress_stream_uint32_shift(c, -8);
		espress_stream_uint32_set_u(u, x[i]);
		espress_stream_uint32_set(v, y3, y2, y1, y0);
		espress_stream_uint32_mul(u, v);
		espress_stream_uint32_add(c, u);
		r[i] = ((b_t *)c)[_O(2, 0)];
	}

	r[ESPRESS_STREAM_EC_SIZE - 1] &= 127;
	espress_stream_uint32_shift(c, -7);
	espress_stream_uint32_set_u(u, 19);
	espress_stream_uint32_mul(c, u);

	for (i = 0 ; i < ESPRESS_STREAM_EC_SIZE ; i ++)
	{
		espress_stream_uint32_set_u(u, r[i]);
		espress_stream_uint32_add(c, u);
		r[i] = ((b_t *)c)[_O(2, 0)];
		espress_stream_uint32_shift(c, -8);
	}
}

static void _inv(b_t *r, b_t const *x)
{
	i_t i;
	b_t s[ESPRESS_STREAM_EC_SIZE];

	_mul(s, x, x);
	_mul(r, s, x);

	for (i = 0 ; i < 248 ; i++)
	{
		_mul(s, r, r);
		_mul(r, s, x);
	}

	_mul(s, r, r);

	_mul(r, s, s);
	_mul(s, r, x);

	_mul(r, s, s);

	_mul(s, r, r);
	_mul(r, s, x);

	_mul(s, r, r);
	_mul(r, s, x);
}

static void _double(b_t *x3, b_t *z3, b_t const *x1, b_t const *z1)
{
	b_t x1_x1[ESPRESS_STREAM_EC_SIZE];
	b_t z1_z1[ESPRESS_STREAM_EC_SIZE];
	b_t x1_z1[ESPRESS_STREAM_EC_SIZE];
	b_t b[ESPRESS_STREAM_EC_SIZE];

	_mul(x1_x1, x1, x1);
	_mul(z1_z1, z1, z1);
	_mul(x1_z1, x1, z1);

	_sub(b, x1_x1, z1_z1);
	_mul(x3, b, b);

	_mul_b(b, x1_z1, 0x00, 0x07, 0x6d, 0x06);
	_add(b, x1_x1, b);
	_add(b, z1_z1, b);
	_mul(x1_x1, x1_z1, b);
	_mul_b(z3, x1_x1, 0x00, 0x00, 0x00, 0x04);
}

static void _diffadd(b_t *x5, b_t *z5, b_t const *x1, b_t const *z1, b_t const *x2, b_t const *z2, b_t const *x3, b_t const *z3)
{
	b_t a[ESPRESS_STREAM_EC_SIZE];
	b_t b[ESPRESS_STREAM_EC_SIZE];
	b_t * const c = a;
	b_t * const d = b;
	b_t ad[ESPRESS_STREAM_EC_SIZE];
	b_t bc[ESPRESS_STREAM_EC_SIZE];

	_add(a, x2, z2);
	_sub(d, x3, z3);
	_mul(ad, a, d);

	_sub(b, x2, z2);
	_add(c, x3, z3);
	_mul(bc, b, c);

	_add(a, ad, bc);
	_mul(b, a, a);
	_mul(x5, z1, b);

	_sub(a, ad, bc);
	_mul(b, a, a);
	_mul(z5, x1, b);
}

static b_t const _one[ESPRESS_STREAM_EC_SIZE] = {1};

static void _smul(b_t *u, b_t const *k)
{
	i_t i;
	i_t j;
	b_t xm[ESPRESS_STREAM_EC_SIZE];
	b_t zm[ESPRESS_STREAM_EC_SIZE] = {1};
	b_t xm1[ESPRESS_STREAM_EC_SIZE] = {1};
	b_t zm1[ESPRESS_STREAM_EC_SIZE] = {0};
	b_t xms[ESPRESS_STREAM_EC_SIZE];
	b_t zms[ESPRESS_STREAM_EC_SIZE];

	memcpy(xm, u, ESPRESS_STREAM_EC_SIZE);

	for (i = 253 ; 0 <= i ; i--)
	{
		j = (k[i >> 3] >> (i & 7)) & 1;

		_diffadd(xm1, zm1, u, _one, xm, zm, xm1, zm1);
		_double(xm, zm, xm, zm);

		_diffadd(xms, zms, xm1, zm1, xm, zm, u, _one);

		_select(xm1, xm, j);
		_select(zm1, zm, j);
		_select(xm, xms, j);
		_select(zm, zms, j);
	}

	_inv(zm1, zm);
	_mul(u, zm1, xm);
	_normalize(u);
}

static void _prepare_key(b_t *k)
{
	k[0] &= 0xf8;
	k[ESPRESS_STREAM_EC_SIZE - 1] &= 0x7f;
	k[ESPRESS_STREAM_EC_SIZE - 1] |= 0x40;
}
