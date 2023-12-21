#ifndef __ESPRESS_STREM_MUTEX_H__
#define __ESPRESS_STREM_MUTEX_H__

struct espress_stream_mutex
{
	u_t counter;
};

struct espress_stream_mutex_ops
{
	i_t (*initialize)(struct espress_stream_mutex *this);
	void (*finalize)(struct espress_stream_mutex *this);
	void (*lock)(struct espress_stream_mutex *this);
	void (*unlock)(struct espress_stream_mutex *this);
	i_t (*try_lock)(struct espress_stream_mutex *this);
};

extern struct espress_stream_mutex_ops const espress_stream_mutex[];

#endif /* __ESPRESS_STREM_MUTEX_H__ */
