#ifndef __ESPRESS_STREAM_RING_BUFFER_H__
#define __ESPRESS_STREAM_RING_BUFFER_H__

struct espress_stream_ring_buffer
{
#	if MAX_U_T_WIDTH < 4
		struct
		{
			i_t is_valid;
			struct espress_stream_mutex mutex[1];
		} mutex[1];
#	endif
	b_t *buffer;
	u_t mask;
	u_t add_size;
	struct espress_stream_uint32 head[1];
	struct espress_stream_uint32 tail[1];
};

struct espress_stream_ring_buffer_ops
{
	void (*initialize)(struct espress_stream_ring_buffer *this, b_t *buffer, u_t size);
	void (*finalize)(struct espress_stream_ring_buffer *this);
	u_t (*en)(struct espress_stream_ring_buffer *this, u_t num);
	u_t (*de)(struct espress_stream_ring_buffer *this, u_t num);
	i_t (*is_empty)(struct espress_stream_ring_buffer *this);
	i_t (*is_full)(struct espress_stream_ring_buffer *this);
	b_t *(*buffer)(struct espress_stream_ring_buffer *this);
	u_t (*size)(struct espress_stream_ring_buffer *this);
	u_t (*num_valid)(struct espress_stream_ring_buffer *this);
	u_t (*num_invalid)(struct espress_stream_ring_buffer *this);
	b_t *(*head)(struct espress_stream_ring_buffer *this, struct espress_stream_uint32 *address, u_t offset);
	b_t *(*tail)(struct espress_stream_ring_buffer *this, struct espress_stream_uint32 *address, u_t offset);
	b_t *(*valid_data)(struct espress_stream_ring_buffer *this, struct espress_stream_uint32 const *address);
	b_t *(*invalid_data)(struct espress_stream_ring_buffer *this, struct espress_stream_uint32 const *address);
};

extern struct espress_stream_ring_buffer_ops const espress_stream_ring_buffer[];

#endif /* __ESPRESS_STREAM_RING_BUFFER_H__ */
