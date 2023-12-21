#ifndef __ESPRESS_CIPHER_SHA512_H__
#define __ESPRESS_CIPHER_SHA512_H__

#define ESPRESS_CIPHER_SHA512_CHUNK_SIZE (128)
#define ESPRESS_CIPHER_SHA512_NUM_ROUND (80)
#define ESPRESS_CIPHER_SHA512_DIGEST_SIZE (64)

struct espress_cipher_sha512
{
	struct espress_cipher_uint64 h[ESPRESS_CIPHER_SHA512_CHUNK_SIZE / 16];
	b_t buffer[ESPRESS_CIPHER_SHA512_CHUNK_SIZE];
	u_t buffer_size;
	u_t total_size;
};

struct espress_cipher_sha512_ops
{
	void (*initialize)(struct espress_cipher_sha512 *this);
	void (*finalize)(struct espress_cipher_sha512 *this, b_t *digest);
	void (*update)(struct espress_cipher_sha512 *this, b_t const *message, u_t size);
};

extern struct espress_cipher_sha512_ops const espress_cipher_sha512[];

#endif /* __ESPRESS_CIPHER_SHA512_H__ */
