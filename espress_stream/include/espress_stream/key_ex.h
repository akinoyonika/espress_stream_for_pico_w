#ifndef __ESPRESS_STREAM_KEY_EX_H__
#define __ESPRESS_STREAM_KEY_EX_H__

#define ESPRESS_STREAM_KEY_EX_KEY_SIZE (ESPRESS_STREAM_EC_SIZE)

struct espress_stream_key_ex_ops
{
	void (*get_public_key)(b_t *public_key, b_t const *private_key);
	void (*get_shared_key)(b_t *shared_key, b_t const *private_key, b_t const *public_key);
};

extern struct espress_stream_key_ex_ops const espress_stream_key_ex[];

#endif /* __ESPRESS_STREAM_KEY_EX_H__ */
