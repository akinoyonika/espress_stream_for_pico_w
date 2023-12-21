#ifndef __ESPRESS_STREAM_HKDF_H__
#define __ESPRESS_STREAM_HKDF_H__

#define ESPRESS_STREAM_HKDF_HASH_SIZE (ESPRESS_STREAM_MAC_MAC_SIZE)

struct espress_stream_hkdf_ops
{
	void (*extract)(b_t *prk, b_t const *salt, u_t salt_size, b_t const *ikm, u_t ikm_size);
	void (*expand)(b_t *t1, u_t t1_size, b_t const *prk, b_t const *info, u_t info_size, b_t count);
};

extern struct espress_stream_hkdf_ops const espress_stream_hkdf[];

#endif /* __ESPRESS_STREAM_HKDF_H__ */
