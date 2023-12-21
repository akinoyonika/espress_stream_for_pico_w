#ifndef __ESPRESS_STREM_KEY_H__
#define __ESPRESS_STREM_KEY_H__

#define ESPRESS_STREAM_KEY_MAC_KEY_SIZE (ESPRESS_STREAM_KEY_CIPHER_KEY_SIZE)
#define ESPRESS_STREAM_KEY_CIPHER_KEY_SIZE (ESPRESS_STREAM_CTR_CIPHER_KEY_SIZE)
#define ESPRESS_STREAM_KEY_HISTORY_SIZE (64)
#define espress_stream_key_history_uint espress_stream_uint64
#define espress_stream_key_history_uint_(o) espress_stream_uint64_##o

struct espress_stream_key
{
	i_t is_valid;
	struct espress_stream_mutex mutex[1];
	struct espress_stream_key_exp key_exp[1];
	b_t mac_key[3][ESPRESS_STREAM_KEY_MAC_KEY_SIZE];
	b_t cipher_key[3][ESPRESS_STREAM_KEY_CIPHER_KEY_SIZE];
	i_t key_idx;
	struct espress_stream_key_history_uint checks[1];
	struct espress_stream_uint32 current[1];
};

struct espress_stream_key_ops
{
	void (*initialize)(struct espress_stream_key *this, b_t const *key, u_t key_size, b_t const *salt, u_t salt_size);
	void (*finalize)(struct espress_stream_key *this);
	i_t (*get)(struct espress_stream_key *this, b_t *mac_key, b_t *cipher_key, struct espress_stream_uint32 const *counter_num);
	i_t (*update)(struct espress_stream_key *this, struct espress_stream_uint32 const *counter_num);
};

extern struct espress_stream_key_ops const espress_stream_key[];

#endif /* __ESPRESS_STREM_KEY_H__ */
