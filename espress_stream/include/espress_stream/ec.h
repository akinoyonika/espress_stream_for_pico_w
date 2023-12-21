#ifndef __ESPRESS_STREAM_EC_H__
#define __ESPRESS_STREAM_EC_H__

#define ESPRESS_STREAM_EC_SIZE (32)

struct espress_stream_ec_ops
{
	void (*smul)(b_t *u, b_t const *k);
	void (*prepare_key)(b_t *k);
};

extern struct espress_stream_ec_ops const espress_stream_ec[];

#endif /* __ESPRESS_STREAM_EC_H__ */
