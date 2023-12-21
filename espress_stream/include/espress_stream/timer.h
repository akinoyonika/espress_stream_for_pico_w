#ifndef __ESPRESS_STREAM_TIMER_H__
#define __ESPRESS_STREAM_TIMER_H__

struct espress_stream_timer
{
	struct espress_stream_uint32 start_time[1];
	struct espress_stream_uint32 timeout[1];
	struct espress_stream_cond cond[1];
	i_t is_active;
	void (*callback)(struct espress_stream_timer *this, void *param);
	void *callback_param;
};

struct espress_stream_timer_op
{
	i_t (*initialize)(struct espress_stream_timer *this);
	void (*finalize)(struct espress_stream_timer *this);
	i_t (*start)(struct espress_stream_timer *this,
		void (*callback)(struct espress_stream_timer *this, void *param),
		void *callback_param,
		struct espress_stream_uint32 const *timeout);
	i_t (*cancel)(struct espress_stream_timer *this);
	void (*yield)(struct espress_stream_timer *this, struct espress_stream_uint32 const *timeout);
	void (*yield_break)(struct espress_stream_timer *this);
};

extern struct espress_stream_timer_op const espress_stream_timer[];

#endif /* __ESPRESS_STREAM_TIMER_H__ */
