#ifndef __ESPRESS_STREAM_CTR_CIPHER_H__
#define __ESPRESS_STREAM_CTR_CIPHER_H__

#define ESPRESS_STREAM_CTR_CIPHER_BLOCK_SIZE (ESPRESS_STREAM_BLOCK_CIPHER_BLOCK_SIZE)
#define ESPRESS_STREAM_CTR_CIPHER_KEY_SIZE (ESPRESS_STREAM_BLOCK_CIPHER_KEY_SIZE)
#define ESPRESS_STREAM_CTR_CIPHER_NONCE_SIZE (ESPRESS_STREAM_CTR_CIPHER_BLOCK_SIZE - 4)

struct espress_stream_ctr_cipher
{
	struct espress_stream_block_cipher block_cipher[1];
	b_t counter[ESPRESS_STREAM_CTR_CIPHER_BLOCK_SIZE];
	b_t cipher_counter[ESPRESS_STREAM_CTR_CIPHER_BLOCK_SIZE];
	u_t total_size;
};

struct espress_stream_ctr_cipher_ops
{
	void (*initialize)(struct espress_stream_ctr_cipher *this, b_t const *nonce, struct espress_stream_uint32 const *init_counter, b_t const *key);
	void (*finalize)(struct espress_stream_ctr_cipher *this);
	void (*encrypt)(struct espress_stream_ctr_cipher *this, b_t *text, u_t text_size);
	void (*decrypt)(struct espress_stream_ctr_cipher *this, b_t *text, u_t text_size);
};

extern struct espress_stream_ctr_cipher_ops const espress_stream_ctr_cipher[];

#endif /* __ESPRESS_STREAM_CTR_CIPHER_H__ */
