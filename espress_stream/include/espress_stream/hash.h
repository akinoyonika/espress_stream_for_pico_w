#ifndef __ESPRESS_STREAM_HASH_H__
#define __ESPRESS_STREAM_HASH_H__

#define ESPRESS_STREAM_HASH_CHUNK_SIZE (64)
#define ESPRESS_STREAM_HASH_NUM_ROUND (64)
#define ESPRESS_STREAM_HASH_DIGEST_SIZE (32)

struct espress_stream_hash
{
	struct espress_stream_uint32 h[ESPRESS_STREAM_HASH_CHUNK_SIZE / 8];
	b_t buffer[ESPRESS_STREAM_HASH_CHUNK_SIZE];
	u_t buffer_size;
	u_t total_size;
};

struct espress_stream_hash_ops
{
	void (*initialize)(struct espress_stream_hash *this);
	void (*finalize)(struct espress_stream_hash *this, b_t *digest);
	void (*update)(struct espress_stream_hash *this, b_t const *message, u_t size);
};

extern struct espress_stream_hash_ops const espress_stream_hash[];

#endif /* __ESPRESS_STREAM_HASH_H__ */
