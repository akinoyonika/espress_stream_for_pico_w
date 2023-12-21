#ifndef __ESPRESS_STREAM_MAC_H__
#define __ESPRESS_STREAM_MAC_H__

#define ESPRESS_STREAM_MAC_BLOCK_SIZE (ESPRESS_STREAM_HASH_CHUNK_SIZE)
#define ESPRESS_STREAM_MAC_MAC_SIZE (ESPRESS_STREAM_HASH_DIGEST_SIZE)

struct espress_stream_mac
{
	b_t key[ESPRESS_STREAM_MAC_BLOCK_SIZE];
	struct espress_stream_hash hash[1];
};

struct espress_stream_mac_ops
{
	void (*initialize)(struct espress_stream_mac *this, b_t const *key, u_t key_size);
	void (*finalize)(struct espress_stream_mac *this, b_t *mac);
	void (*update)(struct espress_stream_mac *this, b_t const *message, u_t size);
};

extern struct espress_stream_mac_ops const espress_stream_mac[];

#endif /* __ESPRESS_STREAM_MAC_H__ */
