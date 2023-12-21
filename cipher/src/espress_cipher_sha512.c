#include "espress_cipher/depend.h"
#include "espress_cipher/uint.h"
#include "espress_cipher/sha512.h"

static void _initialize(struct espress_cipher_sha512 *this);
static void _finalize(struct espress_cipher_sha512 *this, b_t *digest);
static void _update(struct espress_cipher_sha512 *this, b_t const *message, u_t size);

struct espress_cipher_sha512_ops const espress_cipher_sha512[] =
{{
	_initialize,
	_finalize,
	_update
}};

static struct espress_cipher_uint64 const _k[] =
{
	ESPRESS_CIPHER_UINT64(0x42, 0x8a, 0x2f, 0x98, 0xd7, 0x28, 0xae, 0x22),
	ESPRESS_CIPHER_UINT64(0x71, 0x37, 0x44, 0x91, 0x23, 0xef, 0x65, 0xcd),
	ESPRESS_CIPHER_UINT64(0xb5, 0xc0, 0xfb, 0xcf, 0xec, 0x4d, 0x3b, 0x2f),
	ESPRESS_CIPHER_UINT64(0xe9, 0xb5, 0xdb, 0xa5, 0x81, 0x89, 0xdb, 0xbc),
	ESPRESS_CIPHER_UINT64(0x39, 0x56, 0xc2, 0x5b, 0xf3, 0x48, 0xb5, 0x38),
	ESPRESS_CIPHER_UINT64(0x59, 0xf1, 0x11, 0xf1, 0xb6, 0x05, 0xd0, 0x19),
	ESPRESS_CIPHER_UINT64(0x92, 0x3f, 0x82, 0xa4, 0xaf, 0x19, 0x4f, 0x9b),
	ESPRESS_CIPHER_UINT64(0xab, 0x1c, 0x5e, 0xd5, 0xda, 0x6d, 0x81, 0x18),
	ESPRESS_CIPHER_UINT64(0xd8, 0x07, 0xaa, 0x98, 0xa3, 0x03, 0x02, 0x42),
	ESPRESS_CIPHER_UINT64(0x12, 0x83, 0x5b, 0x01, 0x45, 0x70, 0x6f, 0xbe),
	ESPRESS_CIPHER_UINT64(0x24, 0x31, 0x85, 0xbe, 0x4e, 0xe4, 0xb2, 0x8c),
	ESPRESS_CIPHER_UINT64(0x55, 0x0c, 0x7d, 0xc3, 0xd5, 0xff, 0xb4, 0xe2),
	ESPRESS_CIPHER_UINT64(0x72, 0xbe, 0x5d, 0x74, 0xf2, 0x7b, 0x89, 0x6f),
	ESPRESS_CIPHER_UINT64(0x80, 0xde, 0xb1, 0xfe, 0x3b, 0x16, 0x96, 0xb1),
	ESPRESS_CIPHER_UINT64(0x9b, 0xdc, 0x06, 0xa7, 0x25, 0xc7, 0x12, 0x35),
	ESPRESS_CIPHER_UINT64(0xc1, 0x9b, 0xf1, 0x74, 0xcf, 0x69, 0x26, 0x94),
	ESPRESS_CIPHER_UINT64(0xe4, 0x9b, 0x69, 0xc1, 0x9e, 0xf1, 0x4a, 0xd2),
	ESPRESS_CIPHER_UINT64(0xef, 0xbe, 0x47, 0x86, 0x38, 0x4f, 0x25, 0xe3),
	ESPRESS_CIPHER_UINT64(0x0f, 0xc1, 0x9d, 0xc6, 0x8b, 0x8c, 0xd5, 0xb5),
	ESPRESS_CIPHER_UINT64(0x24, 0x0c, 0xa1, 0xcc, 0x77, 0xac, 0x9c, 0x65),
	ESPRESS_CIPHER_UINT64(0x2d, 0xe9, 0x2c, 0x6f, 0x59, 0x2b, 0x02, 0x75),
	ESPRESS_CIPHER_UINT64(0x4a, 0x74, 0x84, 0xaa, 0x6e, 0xa6, 0xe4, 0x83),
	ESPRESS_CIPHER_UINT64(0x5c, 0xb0, 0xa9, 0xdc, 0xbd, 0x41, 0xfb, 0xd4),
	ESPRESS_CIPHER_UINT64(0x76, 0xf9, 0x88, 0xda, 0x83, 0x11, 0x53, 0xb5),
	ESPRESS_CIPHER_UINT64(0x98, 0x3e, 0x51, 0x52, 0xee, 0x66, 0xdf, 0xab),
	ESPRESS_CIPHER_UINT64(0xa8, 0x31, 0xc6, 0x6d, 0x2d, 0xb4, 0x32, 0x10),
	ESPRESS_CIPHER_UINT64(0xb0, 0x03, 0x27, 0xc8, 0x98, 0xfb, 0x21, 0x3f),
	ESPRESS_CIPHER_UINT64(0xbf, 0x59, 0x7f, 0xc7, 0xbe, 0xef, 0x0e, 0xe4),
	ESPRESS_CIPHER_UINT64(0xc6, 0xe0, 0x0b, 0xf3, 0x3d, 0xa8, 0x8f, 0xc2),
	ESPRESS_CIPHER_UINT64(0xd5, 0xa7, 0x91, 0x47, 0x93, 0x0a, 0xa7, 0x25),
	ESPRESS_CIPHER_UINT64(0x06, 0xca, 0x63, 0x51, 0xe0, 0x03, 0x82, 0x6f),
	ESPRESS_CIPHER_UINT64(0x14, 0x29, 0x29, 0x67, 0x0a, 0x0e, 0x6e, 0x70),
	ESPRESS_CIPHER_UINT64(0x27, 0xb7, 0x0a, 0x85, 0x46, 0xd2, 0x2f, 0xfc),
	ESPRESS_CIPHER_UINT64(0x2e, 0x1b, 0x21, 0x38, 0x5c, 0x26, 0xc9, 0x26),
	ESPRESS_CIPHER_UINT64(0x4d, 0x2c, 0x6d, 0xfc, 0x5a, 0xc4, 0x2a, 0xed),
	ESPRESS_CIPHER_UINT64(0x53, 0x38, 0x0d, 0x13, 0x9d, 0x95, 0xb3, 0xdf),
	ESPRESS_CIPHER_UINT64(0x65, 0x0a, 0x73, 0x54, 0x8b, 0xaf, 0x63, 0xde),
	ESPRESS_CIPHER_UINT64(0x76, 0x6a, 0x0a, 0xbb, 0x3c, 0x77, 0xb2, 0xa8),
	ESPRESS_CIPHER_UINT64(0x81, 0xc2, 0xc9, 0x2e, 0x47, 0xed, 0xae, 0xe6),
	ESPRESS_CIPHER_UINT64(0x92, 0x72, 0x2c, 0x85, 0x14, 0x82, 0x35, 0x3b),
	ESPRESS_CIPHER_UINT64(0xa2, 0xbf, 0xe8, 0xa1, 0x4c, 0xf1, 0x03, 0x64),
	ESPRESS_CIPHER_UINT64(0xa8, 0x1a, 0x66, 0x4b, 0xbc, 0x42, 0x30, 0x01),
	ESPRESS_CIPHER_UINT64(0xc2, 0x4b, 0x8b, 0x70, 0xd0, 0xf8, 0x97, 0x91),
	ESPRESS_CIPHER_UINT64(0xc7, 0x6c, 0x51, 0xa3, 0x06, 0x54, 0xbe, 0x30),
	ESPRESS_CIPHER_UINT64(0xd1, 0x92, 0xe8, 0x19, 0xd6, 0xef, 0x52, 0x18),
	ESPRESS_CIPHER_UINT64(0xd6, 0x99, 0x06, 0x24, 0x55, 0x65, 0xa9, 0x10),
	ESPRESS_CIPHER_UINT64(0xf4, 0x0e, 0x35, 0x85, 0x57, 0x71, 0x20, 0x2a),
	ESPRESS_CIPHER_UINT64(0x10, 0x6a, 0xa0, 0x70, 0x32, 0xbb, 0xd1, 0xb8),
	ESPRESS_CIPHER_UINT64(0x19, 0xa4, 0xc1, 0x16, 0xb8, 0xd2, 0xd0, 0xc8),
	ESPRESS_CIPHER_UINT64(0x1e, 0x37, 0x6c, 0x08, 0x51, 0x41, 0xab, 0x53),
	ESPRESS_CIPHER_UINT64(0x27, 0x48, 0x77, 0x4c, 0xdf, 0x8e, 0xeb, 0x99),
	ESPRESS_CIPHER_UINT64(0x34, 0xb0, 0xbc, 0xb5, 0xe1, 0x9b, 0x48, 0xa8),
	ESPRESS_CIPHER_UINT64(0x39, 0x1c, 0x0c, 0xb3, 0xc5, 0xc9, 0x5a, 0x63),
	ESPRESS_CIPHER_UINT64(0x4e, 0xd8, 0xaa, 0x4a, 0xe3, 0x41, 0x8a, 0xcb),
	ESPRESS_CIPHER_UINT64(0x5b, 0x9c, 0xca, 0x4f, 0x77, 0x63, 0xe3, 0x73),
	ESPRESS_CIPHER_UINT64(0x68, 0x2e, 0x6f, 0xf3, 0xd6, 0xb2, 0xb8, 0xa3),
	ESPRESS_CIPHER_UINT64(0x74, 0x8f, 0x82, 0xee, 0x5d, 0xef, 0xb2, 0xfc),
	ESPRESS_CIPHER_UINT64(0x78, 0xa5, 0x63, 0x6f, 0x43, 0x17, 0x2f, 0x60),
	ESPRESS_CIPHER_UINT64(0x84, 0xc8, 0x78, 0x14, 0xa1, 0xf0, 0xab, 0x72),
	ESPRESS_CIPHER_UINT64(0x8c, 0xc7, 0x02, 0x08, 0x1a, 0x64, 0x39, 0xec),
	ESPRESS_CIPHER_UINT64(0x90, 0xbe, 0xff, 0xfa, 0x23, 0x63, 0x1e, 0x28),
	ESPRESS_CIPHER_UINT64(0xa4, 0x50, 0x6c, 0xeb, 0xde, 0x82, 0xbd, 0xe9),
	ESPRESS_CIPHER_UINT64(0xbe, 0xf9, 0xa3, 0xf7, 0xb2, 0xc6, 0x79, 0x15),
	ESPRESS_CIPHER_UINT64(0xc6, 0x71, 0x78, 0xf2, 0xe3, 0x72, 0x53, 0x2b),
	ESPRESS_CIPHER_UINT64(0xca, 0x27, 0x3e, 0xce, 0xea, 0x26, 0x61, 0x9c),
	ESPRESS_CIPHER_UINT64(0xd1, 0x86, 0xb8, 0xc7, 0x21, 0xc0, 0xc2, 0x07),
	ESPRESS_CIPHER_UINT64(0xea, 0xda, 0x7d, 0xd6, 0xcd, 0xe0, 0xeb, 0x1e),
	ESPRESS_CIPHER_UINT64(0xf5, 0x7d, 0x4f, 0x7f, 0xee, 0x6e, 0xd1, 0x78),
	ESPRESS_CIPHER_UINT64(0x06, 0xf0, 0x67, 0xaa, 0x72, 0x17, 0x6f, 0xba),
	ESPRESS_CIPHER_UINT64(0x0a, 0x63, 0x7d, 0xc5, 0xa2, 0xc8, 0x98, 0xa6),
	ESPRESS_CIPHER_UINT64(0x11, 0x3f, 0x98, 0x04, 0xbe, 0xf9, 0x0d, 0xae),
	ESPRESS_CIPHER_UINT64(0x1b, 0x71, 0x0b, 0x35, 0x13, 0x1c, 0x47, 0x1b),
	ESPRESS_CIPHER_UINT64(0x28, 0xdb, 0x77, 0xf5, 0x23, 0x04, 0x7d, 0x84),
	ESPRESS_CIPHER_UINT64(0x32, 0xca, 0xab, 0x7b, 0x40, 0xc7, 0x24, 0x93),
	ESPRESS_CIPHER_UINT64(0x3c, 0x9e, 0xbe, 0x0a, 0x15, 0xc9, 0xbe, 0xbc),
	ESPRESS_CIPHER_UINT64(0x43, 0x1d, 0x67, 0xc4, 0x9c, 0x10, 0x0d, 0x4c),
	ESPRESS_CIPHER_UINT64(0x4c, 0xc5, 0xd4, 0xbe, 0xcb, 0x3e, 0x42, 0xb6),
	ESPRESS_CIPHER_UINT64(0x59, 0x7f, 0x29, 0x9c, 0xfc, 0x65, 0x7e, 0x2a),
	ESPRESS_CIPHER_UINT64(0x5f, 0xcb, 0x6f, 0xab, 0x3a, 0xd6, 0xfa, 0xec),
	ESPRESS_CIPHER_UINT64(0x6c, 0x44, 0x19, 0x8c, 0x4a, 0x47, 0x58, 0x17)
};

static void _process_chunk(struct espress_cipher_sha512 *this)
{
	i_t i;
	b_t *bp;
	struct espress_cipher_uint64 u[1];
	struct espress_cipher_uint64 *up;
	struct espress_cipher_uint64 w[ESPRESS_CIPHER_SHA512_CHUNK_SIZE];
	struct espress_cipher_uint64 s0[1];
	struct espress_cipher_uint64 s1[1];
	struct espress_cipher_uint64 a[1];
	struct espress_cipher_uint64 b[1];
	struct espress_cipher_uint64 c[1];
	struct espress_cipher_uint64 d[1];
	struct espress_cipher_uint64 e[1];
	struct espress_cipher_uint64 f[1];
	struct espress_cipher_uint64 g[1];
	struct espress_cipher_uint64 h[1];
	struct espress_cipher_uint64 ch[1];
	struct espress_cipher_uint64 temp1[1];
	struct espress_cipher_uint64 temp2[1];
	struct espress_cipher_uint64 maj[1];

	for (i = 0 ; i < 16 ; i++)
	{
		bp = this->buffer + (i << 3);
		espress_cipher_uint64_set(w + i, bp[0], bp[1], bp[2], bp[3], bp[4], bp[5], bp[6], bp[7]);
	}

	for ( ; i < ESPRESS_CIPHER_SHA512_NUM_ROUND ; i++)
	{
		up = w + (i - 15);
		espress_cipher_uint64_assign(s0, up);
		espress_cipher_uint64_rotate(s0, -1);
		espress_cipher_uint64_assign(u, up);
		espress_cipher_uint64_rotate(u, -8);
		espress_cipher_uint64_xor(s0, u);
		espress_cipher_uint64_assign(u, up);
		espress_cipher_uint64_shift(u, -7);
		espress_cipher_uint64_xor(s0, u);

		up = w + (i - 2);
		espress_cipher_uint64_assign(s1, up);
		espress_cipher_uint64_rotate(s1, -19);
		espress_cipher_uint64_assign(u, up);
		espress_cipher_uint64_rotate(u, -61);
		espress_cipher_uint64_xor(s1, u);
		espress_cipher_uint64_assign(u, up);
		espress_cipher_uint64_shift(u, -6);
		espress_cipher_uint64_xor(s1, u);

		up = w + i;
		espress_cipher_uint64_assign(up, w + (i - 16));
		espress_cipher_uint64_add(up, s0);
		espress_cipher_uint64_add(up, w + (i - 7));
		espress_cipher_uint64_add(up, s1);
	}

	espress_cipher_uint64_assign(a, this->h + 0);
	espress_cipher_uint64_assign(b, this->h + 1);
	espress_cipher_uint64_assign(c, this->h + 2);
	espress_cipher_uint64_assign(d, this->h + 3);
	espress_cipher_uint64_assign(e, this->h + 4);
	espress_cipher_uint64_assign(f, this->h + 5);
	espress_cipher_uint64_assign(g, this->h + 6);
	espress_cipher_uint64_assign(h, this->h + 7);

	for (i = 0 ; i < ESPRESS_CIPHER_SHA512_NUM_ROUND ; i++)
	{
		espress_cipher_uint64_assign(s1, e);
		espress_cipher_uint64_rotate(s1, -14);
		espress_cipher_uint64_assign(u, e);
		espress_cipher_uint64_rotate(u, -18);
		espress_cipher_uint64_xor(s1, u);
		espress_cipher_uint64_assign(u, e);
		espress_cipher_uint64_rotate(u, -41);
		espress_cipher_uint64_xor(s1, u);

		espress_cipher_uint64_assign(ch, e);
		espress_cipher_uint64_and(ch, f);
		espress_cipher_uint64_assign(u, e);
		espress_cipher_uint64_not(u);
		espress_cipher_uint64_and(u, g);
		espress_cipher_uint64_xor(ch, u);

		espress_cipher_uint64_assign(temp1, h);
		espress_cipher_uint64_add(temp1, s1);
		espress_cipher_uint64_add(temp1, ch);
		espress_cipher_uint64_add(temp1, _k + i);
		espress_cipher_uint64_add(temp1, w + i);

		espress_cipher_uint64_assign(s0, a);
		espress_cipher_uint64_rotate(s0, -28);
		espress_cipher_uint64_assign(u, a);
		espress_cipher_uint64_rotate(u, -34);
		espress_cipher_uint64_xor(s0, u);
		espress_cipher_uint64_assign(u, a);
		espress_cipher_uint64_rotate(u, -39);
		espress_cipher_uint64_xor(s0, u);

		espress_cipher_uint64_assign(maj, a);
		espress_cipher_uint64_and(maj, b);
		espress_cipher_uint64_assign(u, a);
		espress_cipher_uint64_and(u, c);
		espress_cipher_uint64_xor(maj, u);
		espress_cipher_uint64_assign(u, b);
		espress_cipher_uint64_and(u, c);
		espress_cipher_uint64_xor(maj, u);

		espress_cipher_uint64_assign(temp2, s0);
		espress_cipher_uint64_add(temp2, maj);

		espress_cipher_uint64_assign(h, g);
		espress_cipher_uint64_assign(g, f);
		espress_cipher_uint64_assign(f, e);
		espress_cipher_uint64_assign(e, d);
		espress_cipher_uint64_add(e, temp1);
		espress_cipher_uint64_assign(d, c);
		espress_cipher_uint64_assign(c, b);
		espress_cipher_uint64_assign(b, a);
		espress_cipher_uint64_assign(a, temp1);
		espress_cipher_uint64_add(a, temp2);
	}

	espress_cipher_uint64_add(this->h + 0, a);
	espress_cipher_uint64_add(this->h + 1, b);
	espress_cipher_uint64_add(this->h + 2, c);
	espress_cipher_uint64_add(this->h + 3, d);
	espress_cipher_uint64_add(this->h + 4, e);
	espress_cipher_uint64_add(this->h + 5, f);
	espress_cipher_uint64_add(this->h + 6, g);
	espress_cipher_uint64_add(this->h + 7, h);
}

static void _initialize(struct espress_cipher_sha512 *this)
{
	espress_cipher_uint64_set(this->h + 0, 0x6a, 0x09, 0xe6, 0x67, 0xf3, 0xbc, 0xc9, 0x08);
	espress_cipher_uint64_set(this->h + 1, 0xbb, 0x67, 0xae, 0x85, 0x84, 0xca, 0xa7, 0x3b);
	espress_cipher_uint64_set(this->h + 2, 0x3c, 0x6e, 0xf3, 0x72, 0xfe, 0x94, 0xf8, 0x2b);
	espress_cipher_uint64_set(this->h + 3, 0xa5, 0x4f, 0xf5, 0x3a, 0x5f, 0x1d, 0x36, 0xf1);
	espress_cipher_uint64_set(this->h + 4, 0x51, 0x0e, 0x52, 0x7f, 0xad, 0xe6, 0x82, 0xd1);
	espress_cipher_uint64_set(this->h + 5, 0x9b, 0x05, 0x68, 0x8c, 0x2b, 0x3e, 0x6c, 0x1f);
	espress_cipher_uint64_set(this->h + 6, 0x1f, 0x83, 0xd9, 0xab, 0xfb, 0x41, 0xbd, 0x6b);
	espress_cipher_uint64_set(this->h + 7, 0x5b, 0xe0, 0xcd, 0x19, 0x13, 0x7e, 0x21, 0x79);
	this->buffer_size = 0;
	this->total_size = 0;
}

static void _finalize(struct espress_cipher_sha512 *this, b_t *digest)
{
	struct espress_cipher_uint128 u128[1];

	if (ESPRESS_CIPHER_SHA512_CHUNK_SIZE == this->buffer_size)
	{
		goto EXIT;
	}

	this->total_size += this->buffer_size;
	this->buffer[this->buffer_size] = 0x80;
	this->buffer_size++;

	if ((ESPRESS_CIPHER_SHA512_CHUNK_SIZE - 16) < this->buffer_size)
	{
		memset(this->buffer + this->buffer_size, 0, ESPRESS_CIPHER_SHA512_CHUNK_SIZE - this->buffer_size);
		_process_chunk(this);
		this->buffer_size = 0;
	}

	memset(this->buffer + this->buffer_size, 0, ESPRESS_CIPHER_SHA512_CHUNK_SIZE - 16 - this->buffer_size);
	espress_cipher_uint128_set_u(u128, this->total_size);
	espress_cipher_uint128_shift(u128, 3);
	espress_cipher_uint128_get(u128,
		this->buffer + (ESPRESS_CIPHER_SHA512_CHUNK_SIZE - 16),
		this->buffer + (ESPRESS_CIPHER_SHA512_CHUNK_SIZE - 15),
		this->buffer + (ESPRESS_CIPHER_SHA512_CHUNK_SIZE - 14),
		this->buffer + (ESPRESS_CIPHER_SHA512_CHUNK_SIZE - 13),
		this->buffer + (ESPRESS_CIPHER_SHA512_CHUNK_SIZE - 12),
		this->buffer + (ESPRESS_CIPHER_SHA512_CHUNK_SIZE - 11),
		this->buffer + (ESPRESS_CIPHER_SHA512_CHUNK_SIZE - 10),
		this->buffer + (ESPRESS_CIPHER_SHA512_CHUNK_SIZE -  9),
		this->buffer + (ESPRESS_CIPHER_SHA512_CHUNK_SIZE -  8),
		this->buffer + (ESPRESS_CIPHER_SHA512_CHUNK_SIZE -  7),
		this->buffer + (ESPRESS_CIPHER_SHA512_CHUNK_SIZE -  6),
		this->buffer + (ESPRESS_CIPHER_SHA512_CHUNK_SIZE -  5),
		this->buffer + (ESPRESS_CIPHER_SHA512_CHUNK_SIZE -  4),
		this->buffer + (ESPRESS_CIPHER_SHA512_CHUNK_SIZE -  3),
		this->buffer + (ESPRESS_CIPHER_SHA512_CHUNK_SIZE -  2),
		this->buffer + (ESPRESS_CIPHER_SHA512_CHUNK_SIZE -  1));
	this->buffer_size = ESPRESS_CIPHER_SHA512_CHUNK_SIZE;
	_process_chunk(this);

EXIT:

	espress_cipher_uint64_get(this->h + 0, digest +  0, digest +  1, digest +  2, digest +  3, digest +  4, digest +  5, digest +  6, digest +  7);
	espress_cipher_uint64_get(this->h + 1, digest +  8, digest +  9, digest + 10, digest + 11, digest + 12, digest + 13, digest + 14, digest + 15);
	espress_cipher_uint64_get(this->h + 2, digest + 16, digest + 17, digest + 18, digest + 19, digest + 20, digest + 21, digest + 22, digest + 23);
	espress_cipher_uint64_get(this->h + 3, digest + 24, digest + 25, digest + 26, digest + 27, digest + 28, digest + 29, digest + 30, digest + 31);
	espress_cipher_uint64_get(this->h + 4, digest + 32, digest + 33, digest + 34, digest + 35, digest + 36, digest + 37, digest + 38, digest + 39);
	espress_cipher_uint64_get(this->h + 5, digest + 40, digest + 41, digest + 42, digest + 43, digest + 44, digest + 45, digest + 46, digest + 47);
	espress_cipher_uint64_get(this->h + 6, digest + 48, digest + 49, digest + 50, digest + 51, digest + 52, digest + 53, digest + 54, digest + 55);
	espress_cipher_uint64_get(this->h + 7, digest + 56, digest + 57, digest + 58, digest + 59, digest + 60, digest + 61, digest + 62, digest + 63);
}

static void _update(struct espress_cipher_sha512 *this, b_t const *message, u_t size)
{
	b_t const *p = message;
	b_t const *pe = p + size;

	if (ESPRESS_CIPHER_SHA512_CHUNK_SIZE == this->buffer_size)
	{
		goto EXIT;
	}

	while(0 < size)
	{
		if ((ESPRESS_CIPHER_SHA512_CHUNK_SIZE - this->buffer_size) < size)
		{
			size = ESPRESS_CIPHER_SHA512_CHUNK_SIZE - this->buffer_size;
		}
		memcpy(this->buffer + this->buffer_size, p, size);
		this->buffer_size += size;
		p += size;
		size = pe - p;
		if (ESPRESS_CIPHER_SHA512_CHUNK_SIZE == this->buffer_size)
		{
			_process_chunk(this);
			this->total_size += ESPRESS_CIPHER_SHA512_CHUNK_SIZE;
			this->buffer_size = 0;
		}
	}

EXIT:

	return;
}
