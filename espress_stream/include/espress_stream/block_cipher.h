#ifndef __ESPRESS_STREAM_BLOCK_CIPHER_H__
#define __ESPRESS_STREAM_BLOCK_CIPHER_H__

#define ESPRESS_STREAM_BLOCK_CIPHER_NB (4)
#define ESPRESS_STREAM_BLOCK_CIPHER_NK (8)
#define ESPRESS_STREAM_BLOCK_CIPHER_NR (14)
#define ESPRESS_STREAM_BLOCK_CIPHER_NRK (ESPRESS_STREAM_BLOCK_CIPHER_NB * (ESPRESS_STREAM_BLOCK_CIPHER_NR + 1))
#define ESPRESS_STREAM_BLOCK_CIPHER_BLOCK_SIZE (ESPRESS_STREAM_BLOCK_CIPHER_NB * 4)
#define ESPRESS_STREAM_BLOCK_CIPHER_KEY_SIZE (ESPRESS_STREAM_BLOCK_CIPHER_NK * 4)

struct espress_stream_block_cipher
{
	struct espress_stream_uint32 round_key[ESPRESS_STREAM_BLOCK_CIPHER_NRK];
};

struct espress_stream_block_cipher_ops
{
	void (*initialize)(struct espress_stream_block_cipher *this, b_t const *key);
	void (*finalize)(struct espress_stream_block_cipher *this);
	void (*encrypt)(struct espress_stream_block_cipher *this, b_t *block);
};

extern struct espress_stream_block_cipher_ops espress_stream_block_cipher[];

#endif /* __ESPRESS_STREAM_BLOCK_CIPHER_H__ */
