#include "espress_stream/config.h"
#include "espress_stream/depend.h"
#include "espress_stream/uint.h"
#include "espress_stream/hash.h"
#include "espress_stream/mac.h"
#include "espress_stream/hkdf.h"
#include "espress_stream/key_exp.h"

static void _initialize(struct espress_stream_key_exp *this, b_t const *key, u_t key_size, b_t const *salt, u_t salt_size);
static void _finalize(struct espress_stream_key_exp *this);
static void _expand(struct espress_stream_key_exp *this, b_t *exp_key, u_t exp_key_size, b_t const *info, u_t info_size);

struct espress_stream_key_exp_ops const espress_stream_key_exp[] =
{{
	_initialize,
	_finalize,
	_expand
}};

static void _initialize(struct espress_stream_key_exp *this, b_t const *key, u_t key_size, b_t const *salt, u_t salt_size)
{
	espress_stream_hkdf->extract(this->prk, salt, salt_size, key, key_size);
	memset(this->t1, 0, ESPRESS_STREAM_HKDF_HASH_SIZE);
	this->t1_size = 0;
	this->t1_rest = 0;
	this->count = 0;
}

static void _finalize(struct espress_stream_key_exp *this)
{
	memset(this, 0, sizeof(*this));
}

static void _expand(struct espress_stream_key_exp *this, b_t *exp_key, u_t exp_key_size, b_t const *info, u_t info_size)
{
	u_t u;
	u_t v;

	for (u = 0 ; u < exp_key_size ; )
	{
		if (0 < this->t1_rest)
		{
			v = exp_key_size - u;
			if (this->t1_rest < v)
			{
				v = this->t1_rest;
			}
			memcpy(exp_key + u, this->t1 + (ESPRESS_STREAM_HKDF_HASH_SIZE - this->t1_rest), v);
			u += v;
			this->t1_rest -= v;
		}
		else
		{
			this->count++;
			espress_stream_hkdf->expand(this->t1, this->t1_size, this->prk, info, info_size, this->count);
			this->t1_size = ESPRESS_STREAM_HKDF_HASH_SIZE;
			this->t1_rest = ESPRESS_STREAM_HKDF_HASH_SIZE;
		}
	}
}
