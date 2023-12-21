#ifndef __ESPRESS_STREAM_RAND_H__
#define __ESPRESS_STREAM_RAND_H__

struct espress_stream_rand_ops
{
	i_t (*initialize)(void);
	void (*finalize)(void);
	void (*get)(b_t *buffer, u_t size);
};

extern struct espress_stream_rand_ops const espress_stream_rand[];

#endif /* __ESPRESS_STREAM_RAND_H__ */
