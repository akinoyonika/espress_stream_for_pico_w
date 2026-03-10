#include "espress_stream/config.h"
#include "espress_stream/depend.h"
#include "espress_stream/uint.h"
#include "espress_stream/hash.h"

static void _initialize(struct espress_stream_hash *this);
static void _finalize(struct espress_stream_hash *this, b_t *digest);
static void _update(struct espress_stream_hash *this, b_t const *message, u_t size);

struct espress_stream_hash_ops const espress_stream_hash[] =
{{
	_initialize,
	_finalize,
	_update
}};

static struct espress_stream_uint32 const _k[] =
{
	ESPRESS_STREAM_UINT32(0x42, 0x8a, 0x2f, 0x98),
	ESPRESS_STREAM_UINT32(0x71, 0x37, 0x44, 0x91),
	ESPRESS_STREAM_UINT32(0xb5, 0xc0, 0xfb, 0xcf),
	ESPRESS_STREAM_UINT32(0xe9, 0xb5, 0xdb, 0xa5),
	ESPRESS_STREAM_UINT32(0x39, 0x56, 0xc2, 0x5b),
	ESPRESS_STREAM_UINT32(0x59, 0xf1, 0x11, 0xf1),
	ESPRESS_STREAM_UINT32(0x92, 0x3f, 0x82, 0xa4),
	ESPRESS_STREAM_UINT32(0xab, 0x1c, 0x5e, 0xd5),
	ESPRESS_STREAM_UINT32(0xd8, 0x07, 0xaa, 0x98),
	ESPRESS_STREAM_UINT32(0x12, 0x83, 0x5b, 0x01),
	ESPRESS_STREAM_UINT32(0x24, 0x31, 0x85, 0xbe),
	ESPRESS_STREAM_UINT32(0x55, 0x0c, 0x7d, 0xc3),
	ESPRESS_STREAM_UINT32(0x72, 0xbe, 0x5d, 0x74),
	ESPRESS_STREAM_UINT32(0x80, 0xde, 0xb1, 0xfe),
	ESPRESS_STREAM_UINT32(0x9b, 0xdc, 0x06, 0xa7),
	ESPRESS_STREAM_UINT32(0xc1, 0x9b, 0xf1, 0x74),
	ESPRESS_STREAM_UINT32(0xe4, 0x9b, 0x69, 0xc1),
	ESPRESS_STREAM_UINT32(0xef, 0xbe, 0x47, 0x86),
	ESPRESS_STREAM_UINT32(0x0f, 0xc1, 0x9d, 0xc6),
	ESPRESS_STREAM_UINT32(0x24, 0x0c, 0xa1, 0xcc),
	ESPRESS_STREAM_UINT32(0x2d, 0xe9, 0x2c, 0x6f),
	ESPRESS_STREAM_UINT32(0x4a, 0x74, 0x84, 0xaa),
	ESPRESS_STREAM_UINT32(0x5c, 0xb0, 0xa9, 0xdc),
	ESPRESS_STREAM_UINT32(0x76, 0xf9, 0x88, 0xda),
	ESPRESS_STREAM_UINT32(0x98, 0x3e, 0x51, 0x52),
	ESPRESS_STREAM_UINT32(0xa8, 0x31, 0xc6, 0x6d),
	ESPRESS_STREAM_UINT32(0xb0, 0x03, 0x27, 0xc8),
	ESPRESS_STREAM_UINT32(0xbf, 0x59, 0x7f, 0xc7),
	ESPRESS_STREAM_UINT32(0xc6, 0xe0, 0x0b, 0xf3),
	ESPRESS_STREAM_UINT32(0xd5, 0xa7, 0x91, 0x47),
	ESPRESS_STREAM_UINT32(0x06, 0xca, 0x63, 0x51),
	ESPRESS_STREAM_UINT32(0x14, 0x29, 0x29, 0x67),
	ESPRESS_STREAM_UINT32(0x27, 0xb7, 0x0a, 0x85),
	ESPRESS_STREAM_UINT32(0x2e, 0x1b, 0x21, 0x38),
	ESPRESS_STREAM_UINT32(0x4d, 0x2c, 0x6d, 0xfc),
	ESPRESS_STREAM_UINT32(0x53, 0x38, 0x0d, 0x13),
	ESPRESS_STREAM_UINT32(0x65, 0x0a, 0x73, 0x54),
	ESPRESS_STREAM_UINT32(0x76, 0x6a, 0x0a, 0xbb),
	ESPRESS_STREAM_UINT32(0x81, 0xc2, 0xc9, 0x2e),
	ESPRESS_STREAM_UINT32(0x92, 0x72, 0x2c, 0x85),
	ESPRESS_STREAM_UINT32(0xa2, 0xbf, 0xe8, 0xa1),
	ESPRESS_STREAM_UINT32(0xa8, 0x1a, 0x66, 0x4b),
	ESPRESS_STREAM_UINT32(0xc2, 0x4b, 0x8b, 0x70),
	ESPRESS_STREAM_UINT32(0xc7, 0x6c, 0x51, 0xa3),
	ESPRESS_STREAM_UINT32(0xd1, 0x92, 0xe8, 0x19),
	ESPRESS_STREAM_UINT32(0xd6, 0x99, 0x06, 0x24),
	ESPRESS_STREAM_UINT32(0xf4, 0x0e, 0x35, 0x85),
	ESPRESS_STREAM_UINT32(0x10, 0x6a, 0xa0, 0x70),
	ESPRESS_STREAM_UINT32(0x19, 0xa4, 0xc1, 0x16),
	ESPRESS_STREAM_UINT32(0x1e, 0x37, 0x6c, 0x08),
	ESPRESS_STREAM_UINT32(0x27, 0x48, 0x77, 0x4c),
	ESPRESS_STREAM_UINT32(0x34, 0xb0, 0xbc, 0xb5),
	ESPRESS_STREAM_UINT32(0x39, 0x1c, 0x0c, 0xb3),
	ESPRESS_STREAM_UINT32(0x4e, 0xd8, 0xaa, 0x4a),
	ESPRESS_STREAM_UINT32(0x5b, 0x9c, 0xca, 0x4f),
	ESPRESS_STREAM_UINT32(0x68, 0x2e, 0x6f, 0xf3),
	ESPRESS_STREAM_UINT32(0x74, 0x8f, 0x82, 0xee),
	ESPRESS_STREAM_UINT32(0x78, 0xa5, 0x63, 0x6f),
	ESPRESS_STREAM_UINT32(0x84, 0xc8, 0x78, 0x14),
	ESPRESS_STREAM_UINT32(0x8c, 0xc7, 0x02, 0x08),
	ESPRESS_STREAM_UINT32(0x90, 0xbe, 0xff, 0xfa),
	ESPRESS_STREAM_UINT32(0xa4, 0x50, 0x6c, 0xeb),
	ESPRESS_STREAM_UINT32(0xbe, 0xf9, 0xa3, 0xf7),
	ESPRESS_STREAM_UINT32(0xc6, 0x71, 0x78, 0xf2)
};

static void _process_chunk(struct espress_stream_hash *this)
{
	i_t i;
	b_t *bp;
	struct espress_stream_uint32 u[1];
	struct espress_stream_uint32 *up;
	struct espress_stream_uint32 w[ESPRESS_STREAM_HASH_CHUNK_SIZE];
	struct espress_stream_uint32 s0[1];
	struct espress_stream_uint32 s1[1];
	struct espress_stream_uint32 a[1];
	struct espress_stream_uint32 b[1];
	struct espress_stream_uint32 c[1];
	struct espress_stream_uint32 d[1];
	struct espress_stream_uint32 e[1];
	struct espress_stream_uint32 f[1];
	struct espress_stream_uint32 g[1];
	struct espress_stream_uint32 h[1];
	struct espress_stream_uint32 ch[1];
	struct espress_stream_uint32 temp1[1];
	struct espress_stream_uint32 temp2[1];
	struct espress_stream_uint32 maj[1];

	for (i = 0 ; i < 16 ; i++)
	{
		bp = this->buffer + (i << 2);
		espress_stream_uint32_set(w + i, bp[0], bp[1], bp[2], bp[3]);
	}

	for ( ; i < ESPRESS_STREAM_HASH_NUM_ROUND ; i++)
	{
		up = w + (i - 15);
		espress_stream_uint32_assign(s0, up);
		espress_stream_uint32_rotate(s0, -7);
		espress_stream_uint32_assign(u, up);
		espress_stream_uint32_rotate(u, -18);
		espress_stream_uint32_xor(s0, u);
		espress_stream_uint32_assign(u, up);
		espress_stream_uint32_shift(u, -3);
		espress_stream_uint32_xor(s0, u);

		up = w + (i - 2);
		espress_stream_uint32_assign(s1, up);
		espress_stream_uint32_rotate(s1, -17);
		espress_stream_uint32_assign(u, up);
		espress_stream_uint32_rotate(u, -19);
		espress_stream_uint32_xor(s1, u);
		espress_stream_uint32_assign(u, up);
		espress_stream_uint32_shift(u, -10);
		espress_stream_uint32_xor(s1, u);

		up = w + i;
		espress_stream_uint32_assign(up, w + (i - 16));
		espress_stream_uint32_add(up, s0);
		espress_stream_uint32_add(up, w + (i - 7));
		espress_stream_uint32_add(up, s1);
	}

	espress_stream_uint32_assign(a, this->h + 0);
	espress_stream_uint32_assign(b, this->h + 1);
	espress_stream_uint32_assign(c, this->h + 2);
	espress_stream_uint32_assign(d, this->h + 3);
	espress_stream_uint32_assign(e, this->h + 4);
	espress_stream_uint32_assign(f, this->h + 5);
	espress_stream_uint32_assign(g, this->h + 6);
	espress_stream_uint32_assign(h, this->h + 7);

	for (i = 0 ; i < ESPRESS_STREAM_HASH_NUM_ROUND ; i++)
	{
		espress_stream_uint32_assign(s1, e);
		espress_stream_uint32_rotate(s1, -6);
		espress_stream_uint32_assign(u, e);
		espress_stream_uint32_rotate(u, -11);
		espress_stream_uint32_xor(s1, u);
		espress_stream_uint32_assign(u, e);
		espress_stream_uint32_rotate(u, -25);
		espress_stream_uint32_xor(s1, u);

		espress_stream_uint32_assign(ch, e);
		espress_stream_uint32_and(ch, f);
		espress_stream_uint32_assign(u, e);
		espress_stream_uint32_not(u);
		espress_stream_uint32_and(u, g);
		espress_stream_uint32_xor(ch, u);

		espress_stream_uint32_assign(temp1, h);
		espress_stream_uint32_add(temp1, s1);
		espress_stream_uint32_add(temp1, ch);
		espress_stream_uint32_add(temp1, _k + i);
		espress_stream_uint32_add(temp1, w + i);

		espress_stream_uint32_assign(s0, a);
		espress_stream_uint32_rotate(s0, -2);
		espress_stream_uint32_assign(u, a);
		espress_stream_uint32_rotate(u, -13);
		espress_stream_uint32_xor(s0, u);
		espress_stream_uint32_assign(u, a);
		espress_stream_uint32_rotate(u, -22);
		espress_stream_uint32_xor(s0, u);

		espress_stream_uint32_assign(maj, a);
		espress_stream_uint32_and(maj, b);
		espress_stream_uint32_assign(u, a);
		espress_stream_uint32_and(u, c);
		espress_stream_uint32_xor(maj, u);
		espress_stream_uint32_assign(u, b);
		espress_stream_uint32_and(u, c);
		espress_stream_uint32_xor(maj, u);

		espress_stream_uint32_assign(temp2, s0);
		espress_stream_uint32_add(temp2, maj);

		espress_stream_uint32_assign(h, g);
		espress_stream_uint32_assign(g, f);
		espress_stream_uint32_assign(f, e);
		espress_stream_uint32_assign(e, d);
		espress_stream_uint32_add(e, temp1);
		espress_stream_uint32_assign(d, c);
		espress_stream_uint32_assign(c, b);
		espress_stream_uint32_assign(b, a);
		espress_stream_uint32_assign(a, temp1);
		espress_stream_uint32_add(a, temp2);
	}

	espress_stream_uint32_add(this->h + 0, a);
	espress_stream_uint32_add(this->h + 1, b);
	espress_stream_uint32_add(this->h + 2, c);
	espress_stream_uint32_add(this->h + 3, d);
	espress_stream_uint32_add(this->h + 4, e);
	espress_stream_uint32_add(this->h + 5, f);
	espress_stream_uint32_add(this->h + 6, g);
	espress_stream_uint32_add(this->h + 7, h);
}

static void _initialize(struct espress_stream_hash *this)
{
	espress_stream_uint32_set(this->h + 0, 0x6a, 0x09, 0xe6, 0x67);
	espress_stream_uint32_set(this->h + 1, 0xbb, 0x67, 0xae, 0x85);
	espress_stream_uint32_set(this->h + 2, 0x3c, 0x6e, 0xf3, 0x72);
	espress_stream_uint32_set(this->h + 3, 0xa5, 0x4f, 0xf5, 0x3a);
	espress_stream_uint32_set(this->h + 4, 0x51, 0x0e, 0x52, 0x7f);
	espress_stream_uint32_set(this->h + 5, 0x9b, 0x05, 0x68, 0x8c);
	espress_stream_uint32_set(this->h + 6, 0x1f, 0x83, 0xd9, 0xab);
	espress_stream_uint32_set(this->h + 7, 0x5b, 0xe0, 0xcd, 0x19);
	this->buffer_size = 0;
	this->total_size = 0;
}

static void _finalize(struct espress_stream_hash *this, b_t *digest)
{
	struct espress_stream_uint64 u64[1];

	if (ESPRESS_STREAM_HASH_CHUNK_SIZE == this->buffer_size)
	{
		goto EXIT;
	}

	this->total_size += this->buffer_size;
	this->buffer[this->buffer_size] = 0x80;
	this->buffer_size++;

	if ((ESPRESS_STREAM_HASH_CHUNK_SIZE - 8) < this->buffer_size)
	{
		memset(this->buffer + this->buffer_size, 0, ESPRESS_STREAM_HASH_CHUNK_SIZE - this->buffer_size);
		_process_chunk(this);
		this->buffer_size = 0;
	}

	memset(this->buffer + this->buffer_size, 0, ESPRESS_STREAM_HASH_CHUNK_SIZE - 8 - this->buffer_size);
	espress_stream_uint64_set_u(u64, this->total_size);
	espress_stream_uint64_shift(u64, 3);
	espress_stream_uint64_get(u64,
		this->buffer + (ESPRESS_STREAM_HASH_CHUNK_SIZE - 8),
		this->buffer + (ESPRESS_STREAM_HASH_CHUNK_SIZE - 7),
		this->buffer + (ESPRESS_STREAM_HASH_CHUNK_SIZE - 6),
		this->buffer + (ESPRESS_STREAM_HASH_CHUNK_SIZE - 5),
		this->buffer + (ESPRESS_STREAM_HASH_CHUNK_SIZE - 4),
		this->buffer + (ESPRESS_STREAM_HASH_CHUNK_SIZE - 3),
		this->buffer + (ESPRESS_STREAM_HASH_CHUNK_SIZE - 2),
		this->buffer + (ESPRESS_STREAM_HASH_CHUNK_SIZE - 1));
	this->buffer_size = ESPRESS_STREAM_HASH_CHUNK_SIZE;
	_process_chunk(this);

EXIT:

	espress_stream_uint32_get(this->h + 0, digest +  0, digest +  1, digest +  2, digest +  3);
	espress_stream_uint32_get(this->h + 1, digest +  4, digest +  5, digest +  6, digest +  7);
	espress_stream_uint32_get(this->h + 2, digest +  8, digest +  9, digest + 10, digest + 11);
	espress_stream_uint32_get(this->h + 3, digest + 12, digest + 13, digest + 14, digest + 15);
	espress_stream_uint32_get(this->h + 4, digest + 16, digest + 17, digest + 18, digest + 19);
	espress_stream_uint32_get(this->h + 5, digest + 20, digest + 21, digest + 22, digest + 23);
	espress_stream_uint32_get(this->h + 6, digest + 24, digest + 25, digest + 26, digest + 27);
	espress_stream_uint32_get(this->h + 7, digest + 28, digest + 29, digest + 30, digest + 31);
}

static void _update(struct espress_stream_hash *this, b_t const *message, u_t size)
{
	b_t const *p = message;
	b_t const *pe = p + size;

	if (ESPRESS_STREAM_HASH_CHUNK_SIZE == this->buffer_size)
	{
		goto EXIT;
	}

	while (0 < size)
	{
		if ((ESPRESS_STREAM_HASH_CHUNK_SIZE - this->buffer_size) < size)
		{
			size = ESPRESS_STREAM_HASH_CHUNK_SIZE - this->buffer_size;
		}
		memcpy(this->buffer + this->buffer_size, p, size);
		this->buffer_size += size;
		p += size;
		size = pe - p;
		if (ESPRESS_STREAM_HASH_CHUNK_SIZE == this->buffer_size)
		{
			_process_chunk(this);
			this->total_size += ESPRESS_STREAM_HASH_CHUNK_SIZE;
			this->buffer_size = 0;
		}
	}

EXIT:

	return;
}
