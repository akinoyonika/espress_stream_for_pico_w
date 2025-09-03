#ifndef __ESPRESS_STREAM_DGRAM_H__
#define __ESPRESS_STREAM_DGRAM_H__

#define ESPRESS_STREAM_DGRAM_MTU (1280)

struct espress_stream_dgram_address
{
	struct sockaddr_storage storage[1];
};

#define espress_stream_dgram_buffer_header \
	struct espress_stream_dgram_address address[1]; \
	u_t data_size

struct espress_stream_dgram_buffer
{
	espress_stream_dgram_buffer_header;
	b_t data[1];
};

struct espress_stream_dgram
{
	struct espress_stream_event *send_outer_event;
	struct espress_stream_event *recv_outer_event;
	struct espress_stream_event send_event[1];
	struct espress_stream_event recv_event[1];
	struct espress_stream_mutex send_mutex[1];
	struct espress_stream_mutex recv_mutex[1];
	struct udp_pcb *pcb;
	struct espress_stream_event send_break_event[1];
	struct espress_stream_dgram_buffer * const *send_buffer;
	u_t num_send_buffer;
	u_t num_sent_buffer;
	i_t send_errno;
	struct espress_stream_event recv_break_event[1];
	struct espress_stream_dgram_buffer * const *recv_buffer;
	u_t num_recv_buffer;
	u_t num_rcvd_buffer;
	i_t recv_errno;

	struct pbuf *recv_pbuf;
	ip_addr_t recv_ipaddr[1];
	u16_t recv_port;
};

struct espress_stream_dgram_ops
{
	i_t (*initialize)(struct espress_stream_dgram *this, struct espress_stream_dgram_address const *bind_address, struct espress_stream_event *send_event, struct espress_stream_event *recv_event);
	void (*finalize)(struct espress_stream_dgram *this);
	i_t (*send_request)(struct espress_stream_dgram *this, struct espress_stream_dgram_buffer * const *buffer, u_t num, struct espress_stream_dgram_buffer * const *ex_buffer, u_t ex_num);
	i_t (*send_result)(struct espress_stream_dgram *this, u_t *num);
	i_t (*recv_request)(struct espress_stream_dgram *this, struct espress_stream_dgram_buffer * const *buffer, u_t num, struct espress_stream_dgram_buffer * const *ex_buffer, u_t ex_num);
	i_t (*recv_result)(struct espress_stream_dgram *this, u_t *num);
	u_t (*mtu)(struct espress_stream_dgram *this);
	void (*send_yield)(struct espress_stream_dgram *this, struct espress_stream_uint32 const *timeout);
	void (*recv_yield)(struct espress_stream_dgram *this, struct espress_stream_uint32 const *timeout);
	void (*send_yield_break)(struct espress_stream_dgram *this);
	void (*recv_yield_break)(struct espress_stream_dgram *this);
};

extern struct espress_stream_dgram_ops const espress_stream_dgram[];

#endif /* __ESPRESS_STREAM_DGRAM_H__ */
