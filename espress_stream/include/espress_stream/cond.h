#ifndef __ESPRESS_STREAM_COND_H__
#define __ESPRESS_STREAM_COND_H__

struct espress_stream_cond
{
	i_t cond;
	struct espress_stream_mutex mutex[1];
	i_t is_signaled;
};

struct espress_stream_cond_ops
{
	i_t (*initialize)(struct espress_stream_cond *this);
	void (*finalize)(struct espress_stream_cond *this);
	void (*signal_lock)(struct espress_stream_cond *this);
	void (*signal_unlock)(struct espress_stream_cond *this);
	void (*signal)(struct espress_stream_cond *this);
	void (*wait_lock)(struct espress_stream_cond *this);
	void (*wait_unlock)(struct espress_stream_cond *this);
	i_t (*wait)(struct espress_stream_cond *this, struct espress_stream_uint32 const *timeout);
	i_t (*poll)(struct espress_stream_cond * const *these, u_t num, struct espress_stream_uint32 const *timeout);
};

extern struct espress_stream_cond_ops const espress_stream_cond[];

#endif /* __ESPRESS_STREAM_COND_H__ */
