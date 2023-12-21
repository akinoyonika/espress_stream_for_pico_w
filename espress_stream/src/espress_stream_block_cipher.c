#include "espress_stream/config.h"
#include "espress_stream/depend.h"
#include "espress_stream/uint.h"
#include "espress_stream/block_cipher.h"

#if ESPRESS_STREAM_IS_HOST_BYTE_ORDER_LITTLE
#	define _O(w, n) (n)
#else
#	define _O(w, n) ((1 << (w)) - 1 - (n))
#endif

#define _NB (ESPRESS_STREAM_BLOCK_CIPHER_NB)
#define _NK (ESPRESS_STREAM_BLOCK_CIPHER_NK)
#define _NR (ESPRESS_STREAM_BLOCK_CIPHER_NR)

static void _initialize(struct espress_stream_block_cipher *this, b_t const *key);
static void _finalize(struct espress_stream_block_cipher *this);
static void _encrypt(struct espress_stream_block_cipher *this, b_t *block);

struct espress_stream_block_cipher_ops espress_stream_block_cipher[] =
{{
	_initialize,
	_finalize,
	_encrypt
}};

static b_t const _s_box[] =
{
	0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5,
	0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76,
	0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0,
	0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0,
	0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc,
	0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15,
	0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a,
	0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75,
	0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0,
	0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84,
	0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b,
	0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf,
	0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85,
	0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8,
	0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5,
	0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2,
	0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17,
	0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73,
	0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88,
	0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb,
	0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c,
	0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79,
	0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9,
	0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08,
	0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6,
	0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a,
	0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e,
	0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e,
	0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94,
	0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf,
	0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68,
	0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16
};

static struct espress_stream_uint32 const _r_con[] =
{
	ESPRESS_STREAM_UINT32(0x00, 0x00, 0x00, 0x00),
	ESPRESS_STREAM_UINT32(0x01, 0x00, 0x00, 0x00),
	ESPRESS_STREAM_UINT32(0x02, 0x00, 0x00, 0x00),
	ESPRESS_STREAM_UINT32(0x04, 0x00, 0x00, 0x00),
	ESPRESS_STREAM_UINT32(0x08, 0x00, 0x00, 0x00),
	ESPRESS_STREAM_UINT32(0x10, 0x00, 0x00, 0x00),
	ESPRESS_STREAM_UINT32(0x20, 0x00, 0x00, 0x00),
	ESPRESS_STREAM_UINT32(0x40, 0x00, 0x00, 0x00),
	ESPRESS_STREAM_UINT32(0x80, 0x00, 0x00, 0x00),
	ESPRESS_STREAM_UINT32(0x1b, 0x00, 0x00, 0x00),
	ESPRESS_STREAM_UINT32(0x36, 0x00, 0x00, 0x00)
};

#define _sub_byte(b) (*(b) = _s_box[*(b)])

static void _sub_word(struct espress_stream_uint32 *w)
{
	_sub_byte((b_t *)w);
	_sub_byte((b_t *)w + 1);
	_sub_byte((b_t *)w + 2);
	_sub_byte((b_t *)w + 3);
}

static void _sub_bytes(struct espress_stream_uint32 *u)
{
	i_t i;

	for (i = 0 ; i < _NB ; i++)
	{
		_sub_word(u + i);
	}
}

static void _key_expansion(struct espress_stream_uint32 *round_key, b_t const *key)
{
	i_t i;
	struct espress_stream_uint32 u[1];

	for (i = 0 ; i < _NK ; i++)
	{
		espress_stream_uint32_set(round_key + i, key[i * 4], key[i * 4 + 1], key[i * 4 + 2], key[i * 4 + 3]);
	}

	for (i = _NK ; i < _NB * (_NR + 1) ; i++)
	{
		espress_stream_uint32_assign(u, round_key + (i - 1));
		if (0 == (i % _NK))
		{
			espress_stream_uint32_rotate(u, 8);
			_sub_word(u);
			espress_stream_uint32_xor(u, _r_con + (i / _NK));
		}
		else if ((6 < _NK) && (4 == (i % _NK)))
		{
			_sub_word(u);
		}
		espress_stream_uint32_assign(round_key + i, round_key + (i - _NK));
		espress_stream_uint32_xor(round_key + i, u);
	}
}

static b_t _mul(b_t a, b_t n)
{
	i_t i;
	b_t b;
	b_t r = 0;

	for (i = 0 ; i < 8 ; i++)
	{
		b = - (n & 1);
		r ^= (a & b);
		b = - ((a >> 7) & 1);
		a <<= 1;
		a ^= (0x1b & b);
		n >>= 1;
	}

	return r;
}

static void _add_round_key(struct espress_stream_uint32 *state, struct espress_stream_uint32 const *round_key)
{
	i_t i;

	for (i = 0 ; i < _NB ; i++)
	{
		espress_stream_uint32_xor(state + i, round_key + i);
	}
}

static void _shift_rows(struct espress_stream_uint32 *state)
{
	i_t i;
	b_t *sp0;
	b_t *sp1;
	b_t *sp2;
	b_t *sp3;
	struct espress_stream_uint32 u[1];

	for (i = 1 ; i < _NB ; i++)
	{
		sp0 = (b_t *)(state + 0) + (3 - _O(2, i));
		sp1 = (b_t *)(state + 1) + (3 - _O(2, i));
		sp2 = (b_t *)(state + 2) + (3 - _O(2, i));
		sp3 = (b_t *)(state + 3) + (3 - _O(2, i));
		espress_stream_uint32_set(u, *sp0, *sp1, *sp2, *sp3);
		espress_stream_uint32_rotate(u, i * 8);
		espress_stream_uint32_get(u, sp0, sp1, sp2, sp3);
	}
}

static void _mix_columns(struct espress_stream_uint32 *state)
{
	i_t i;
	b_t *sp0;
	b_t *sp1;
	b_t *sp2;
	b_t *sp3;
	b_t s0;
	b_t s1;
	b_t s2;
	b_t s3;

	for (i = 0 ; i < _NB ; i++)
	{
		sp0 = (b_t *)(state + i) + (3 - _O(2, 0));
		sp1 = (b_t *)(state + i) + (3 - _O(2, 1));
		sp2 = (b_t *)(state + i) + (3 - _O(2, 2));
		sp3 = (b_t *)(state + i) + (3 - _O(2, 3));
		s0 = *sp0;
		s1 = *sp1;
		s2 = *sp2;
		s3 = *sp3;
		*sp0 = _mul(s0, 2) ^ _mul(s1, 3) ^ s2 ^ s3;
		*sp1 = s0 ^ _mul(s1, 2) ^ _mul(s2, 3) ^ s3;
		*sp2 = s0 ^ s1 ^ _mul(s2, 2) ^ _mul(s3, 3);
		*sp3 = _mul(s0, 3) ^ s1 ^ s2 ^ _mul(s3, 2);
	}
}

static void _cipher(b_t *block, struct espress_stream_uint32 const *round_key)
{
	i_t i;
	struct espress_stream_uint32 * const state = (struct espress_stream_uint32 *)block;

	for (i = 0 ; i < _NB ; i++)
	{
		espress_stream_uint32_btoh(state + i);
	}

	_add_round_key(state, round_key);
	for (i = 1 ; i < _NR ; i++)
	{
		_sub_bytes(state);
		_shift_rows(state);
		_mix_columns(state);
		_add_round_key(state, round_key + i * _NB);
	}
	_sub_bytes(state);
	_shift_rows(state);
	_add_round_key(state, round_key + _NR * _NB);

	for (i = 0 ; i < _NB ; i++)
	{
		espress_stream_uint32_htob(state + i);
	}
}

static void _initialize(struct espress_stream_block_cipher *this, b_t const *key)
{
	_key_expansion(this->round_key, key);
}

static void _finalize(struct espress_stream_block_cipher *this)
{
	memset(this, 0, sizeof(*this));
}

static void _encrypt(struct espress_stream_block_cipher *this, b_t *block)
{
	_cipher(block, this->round_key);
}
