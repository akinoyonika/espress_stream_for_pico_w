#ifndef __ESPRESS_STREAM_SESSION_H__
#define __ESPRESS_STREAM_SESSION_H__

#define ESPRESS_STREAM_SESSION_NUM_YIELD (4)
#define ESPRESS_STREAM_IS_ADD_DATA_ENABLED (1)

struct espress_stream_session
{
	struct espress_stream_trans_packet_interface *interface;
	struct espress_stream_event *outer_event;
	i_t is_server;
	i_t (*send_connect_payload)(struct espress_stream_session *this, i_t (*callback)(struct espress_stream_session *this, b_t const *payload, u_t payload_size, i_t is_rejected), void *param);
	void *send_connect_payload_param;
	i_t (*recv_connect_payload)(struct espress_stream_session *this, i_t (*callback)(struct espress_stream_session *this, b_t *payload, u_t *payload_size), void *param);
	void *recv_connect_payload_param;
	i_t (*send_disconnect_payload)(struct espress_stream_session *this, i_t (*callback)(struct espress_stream_session *this, b_t const *payload, u_t payload_size), void *param);
	void *send_disconnect_payload_param;
	i_t (*recv_disconnect_payload)(struct espress_stream_session *this, i_t (*callback)(struct espress_stream_session *this, b_t *payload, u_t *payload_size), void *param);
	void *recv_disconnect_payload_param;
	struct espress_stream_session_send_window
	{
		struct espress_stream_ring_buffer *buffer;
		u_t size;
		struct espress_stream_uint32 data_address[1];
		u_t flush_size;
	} send_window[1];
	struct espress_stream_session_recv_window
	{
		struct espress_stream_ring_buffer *buffer;
#		if ESPRESS_STREAM_IS_ADD_DATA_ENABLED
			struct espress_stream_uint32 add_data_address[1];
			u_t add_data_size;
#		endif
	} recv_window[1];
	i_t send_state;
	i_t recv_state;
	struct espress_stream_event event[1];
	struct espress_stream_trans_packet trans_packet[1];
	struct espress_stream_mutex mutex[1];
	struct espress_stream_timer send_timer[1];
	struct espress_stream_timer session_timer[1];
	struct espress_stream_timer beacon_timer[1];

	struct espress_stream_dgram_address src_address[1];
	struct espress_stream_dgram_address dst_address[1];
	struct espress_stream_uint32 src_connection_param[1];
	struct espress_stream_uint32 dst_connection_param[1];

	i_t is_ack_ready;

	u_t send_timeout_count;
	u_t fast_retransmit_count;

	struct espress_stream_uint32 fast_retransmit_start_data_address[1];
	struct espress_stream_uint32 dac_avoid_size[1];

	struct espress_stream_session_fast_retransmit
	{
		struct espress_stream_uint32 start_data_address[1];
		struct espress_stream_uint32 dac_avoid_size[1];
	} fast_retransmit[1];

	struct espress_stream_session_cong_avoid
	{
		u_t cwnd;
		u_t ssthresh;
		u_t cwnd_clamp;
		u_t cwnd_cnt;
		u_t window_unit;
		u_t cwnd_size;
	} cong_avoid[1];
};

struct espress_stream_session_ops
{
	i_t (*initialize)(struct espress_stream_session *this,
		struct espress_stream_trans_packet_interface *interface,
		struct espress_stream_event *event,
		i_t is_server,
		struct espress_stream_ring_buffer *send_buffer,
		struct espress_stream_ring_buffer *recv_buffer,
		i_t (*send_connect_payload)(struct espress_stream_session *this, i_t (*callback)(struct espress_stream_session *this, b_t const *payload, u_t payload_size, i_t is_rejected), void *param), void *send_connect_payload_param,
		i_t (*recv_connect_payload)(struct espress_stream_session *this, i_t (*callback)(struct espress_stream_session *this, b_t *payload, u_t *payload_size), void *param), void *recv_connect_payload_param,
		i_t (*send_disconnect_payload)(struct espress_stream_session *this, i_t (*callback)(struct espress_stream_session *this, b_t const *payload, u_t payload_size), void *param), void *send_disconnect_payload_param,
		i_t (*recv_disconnect_payload)(struct espress_stream_session *this, i_t (*callback)(struct espress_stream_session *this, b_t *payload, u_t *payload_size), void *param), void *recv_disconnect_payload_param);
	void (*finalize)(struct espress_stream_session *this);
	i_t (*connect)(struct espress_stream_session *this,
		struct espress_stream_dgram_address const *address);
	i_t (*connect_result)(struct espress_stream_session *this);
	i_t (*connect_detect)(struct espress_stream_session *this);
	i_t (*disconnect)(struct espress_stream_session *this);
	i_t (*disconnect_result)(struct espress_stream_session *this);
	i_t (*disconnect_detect)(struct espress_stream_session *this);
	i_t (*send)(struct espress_stream_session *this);
	i_t (*flush)(struct espress_stream_session *this);
	i_t (*recv)(struct espress_stream_session *this);
	void (*src_address)(struct espress_stream_session *this, struct espress_stream_dgram_address *address);
	void (*dst_address)(struct espress_stream_session *this, struct espress_stream_dgram_address *address);
	void (*connection_id)(struct espress_stream_session *this, struct espress_stream_uint32 *connection_id);
	void (*shared_key)(struct espress_stream_session *this, b_t *shared_key);
	i_t (*rtt)(struct espress_stream_session *this, struct espress_stream_uint32 *rtt);
	void (*rto)(struct espress_stream_session *this, struct espress_stream_uint32 *rto);
	u_t (*dac)(struct espress_stream_session *this);
	u_t (*cwnd)(struct espress_stream_session *this);
	u_t (*send_timeout_count)(struct espress_stream_session *this);
	u_t (*fast_retransmit_count)(struct espress_stream_session *this);
	void (*yield[ESPRESS_STREAM_SESSION_NUM_YIELD + 1])(struct espress_stream_session *this, struct espress_stream_uint32 const *timeout);
	void (*yield_break[ESPRESS_STREAM_SESSION_NUM_YIELD + 1])(struct espress_stream_session *this);
};

extern struct espress_stream_session_ops const espress_stream_session[];

#endif /* __ESPRESS_STREAM_SESSION_H__ */
