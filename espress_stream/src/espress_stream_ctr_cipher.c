#include "espress_stream/config.h"
#include "espress_stream/depend.h"
#include "espress_stream/uint.h"
#include "espress_stream/block_cipher.h"
#include "espress_stream/ctr_cipher.h"

static void _initialize(struct espress_stream_ctr_cipher *this, b_t const *nonce, struct espress_stream_uint32 const *init_counter, b_t const *key);
static void _finalize(struct espress_stream_ctr_cipher *this);
static void _encrypt(struct espress_stream_ctr_cipher *this, b_t *text, u_t text_size);

struct espress_stream_ctr_cipher_ops const espress_stream_ctr_cipher[] =
{{
	_initialize,
	_finalize,
	_encrypt,
	_encrypt,
}};

static void _initialize(struct espress_stream_ctr_cipher *this, b_t const *nonce, struct espress_stream_uint32 const *init_counter, b_t const *key)
{
	espress_stream_block_cipher->initialize(this->block_cipher, key);
	memcpy(this->counter, nonce, ESPRESS_STREAM_CTR_CIPHER_NONCE_SIZE);
	memcpy(this->counter + ESPRESS_STREAM_CTR_CIPHER_NONCE_SIZE, init_counter, ESPRESS_STREAM_CTR_CIPHER_BLOCK_SIZE - ESPRESS_STREAM_CTR_CIPHER_NONCE_SIZE);
	this->total_size = 0;
}

static void _finalize(struct espress_stream_ctr_cipher *this)
{
	espress_stream_block_cipher->finalize(this->block_cipher);
	memset(this, 0, sizeof(*this));
}

static void _encrypt(struct espress_stream_ctr_cipher *this, b_t *text, u_t text_size)
{
	u_t u;
	b_t *p;
	b_t *t = text;
	b_t *te = t + text_size;

	while (t < te)
	{
		u = this->total_size % ESPRESS_STREAM_CTR_CIPHER_BLOCK_SIZE;
		p = t + (ESPRESS_STREAM_CTR_CIPHER_BLOCK_SIZE - u);
		if (0 == u)
		{
			memcpy(this->cipher_counter, this->counter, ESPRESS_STREAM_CTR_CIPHER_BLOCK_SIZE);
			espress_stream_uint32_htob((struct espress_stream_uint32 *)(this->cipher_counter + ESPRESS_STREAM_CTR_CIPHER_NONCE_SIZE));
			espress_stream_block_cipher->encrypt(this->block_cipher, this->cipher_counter);
			espress_stream_uint32_add((struct espress_stream_uint32 *)(this->counter + ESPRESS_STREAM_CTR_CIPHER_NONCE_SIZE), espress_stream_uint32_1);
		}
		if (te < p)
		{
			p = te;
		}
		for ( ; t < p ; t++, u++, this->total_size++)
		{
			*t ^= this->cipher_counter[u];
		}
	}
}
