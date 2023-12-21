#ifndef __ESPRESS_STREAM_TRANS_RING_H__
#define __ESPRESS_STREAM_TRANS_RING_H__

#define ESPRESS_STREAM_TRANS_RING_INVALID_DATA_SIZE (U_T_MAX)

struct espress_stream_trans_ring
{
	struct espress_stream_dgram *dgram;
	struct espress_stream_ring ring[1];
	i_t is_sending;
	i_t is_recving;
};

struct espress_stream_trans_ring_ops
{
	void (*initialize)(struct espress_stream_trans_ring *this, struct espress_stream_dgram_buffer * const *buffer, u_t size, struct espress_stream_dgram *dgram);
	void (*finalize)(struct espress_stream_trans_ring *this);
	u_t (*en)(struct espress_stream_trans_ring *this, u_t num);
	u_t (*de)(struct espress_stream_trans_ring *this, u_t num);
	i_t (*is_empty)(struct espress_stream_trans_ring *this);
	i_t (*is_full)(struct espress_stream_trans_ring *this);
	u_t (*size)(struct espress_stream_trans_ring *this);
	u_t (*num_valid)(struct espress_stream_trans_ring *this);
	u_t (*num_invalid)(struct espress_stream_trans_ring *this);
	 struct espress_stream_dgram_buffer * const *(*head)(struct espress_stream_trans_ring *this, u_t offset);
	 struct espress_stream_dgram_buffer * const *(*tail)(struct espress_stream_trans_ring *this, u_t offset);
	i_t (*send_request)(struct espress_stream_trans_ring *this);
	i_t (*send_result)(struct espress_stream_trans_ring *this, u_t *num);
	i_t (*recv_request)(struct espress_stream_trans_ring *this);
	i_t (*recv_result)(struct espress_stream_trans_ring *this, u_t *num);
};

extern struct espress_stream_trans_ring_ops const espress_stream_trans_ring[];

#endif /* __ESPRESS_STREAM_TRANS_RING_H__ */
