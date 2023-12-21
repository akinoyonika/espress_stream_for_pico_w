#ifndef __ESPRESS_STREAM_RING_H__
#define __ESPRESS_STREAM_RING_H__

struct espress_stream_ring
{
	void * const *buffer;
	u_t mask;
	u_t head;
	u_t tail;
};

struct espress_stream_ring_ops
{
	void (*initialize)(struct espress_stream_ring *this, void * const *buffer, u_t size);
	void (*finalize)(struct espress_stream_ring *this);
	u_t (*en)(struct espress_stream_ring *this, u_t num);
	u_t (*de)(struct espress_stream_ring *this, u_t num);
	i_t (*is_empty)(struct espress_stream_ring *this);
	i_t (*is_full)(struct espress_stream_ring *this);
	u_t (*size)(struct espress_stream_ring *this);
	u_t (*num_valid)(struct espress_stream_ring *this);
	u_t (*num_invalid)(struct espress_stream_ring *this);
	void * const *(*head)(struct espress_stream_ring *this, u_t offset);
	void * const *(*tail)(struct espress_stream_ring *this, u_t offset);
};

extern struct espress_stream_ring_ops const espress_stream_ring[];

#endif /* __ESPRESS_STREAM_RING_H__ */
