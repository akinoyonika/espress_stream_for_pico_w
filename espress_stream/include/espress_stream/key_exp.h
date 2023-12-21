#ifndef __ESPRESS_STREAM_KEY_EXP_H__
#define __ESPRESS_STREAM_KEY_EXP_H__

struct espress_stream_key_exp
{
	b_t prk[ESPRESS_STREAM_HKDF_HASH_SIZE];
	b_t t1[ESPRESS_STREAM_HKDF_HASH_SIZE];
	u_t t1_size;
	u_t t1_rest;
	b_t count;
};

struct espress_stream_key_exp_ops
{
	void (*initialize)(struct espress_stream_key_exp *this, b_t const *key, u_t key_size, b_t const *salt, u_t salt_size);
	void (*finalize)(struct espress_stream_key_exp *this);
	void (*expand)(struct espress_stream_key_exp *this, b_t *exp_key, u_t exp_key_size, b_t const *info, u_t info_size);
};

extern struct espress_stream_key_exp_ops const espress_stream_key_exp[];

#endif /* __ESPRESS_STREAM_KEY_EXP_H__ */
