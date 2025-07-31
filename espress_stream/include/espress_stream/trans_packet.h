#ifndef __ESPRESS_STREAM_TRANS_PACKET_H__
#define __ESPRESS_STREAM_TRANS_PACKET_H__

#define ESPRESS_STREAM_TRANS_PACKET_NUM_YIELD (ESPRESS_STREAM_SEND_QUEUE_SIZE + ESPRESS_STREAM_RECV_QUEUE_SIZE + 2 + 1)

struct espress_stream_trans_packet_interface
{
	struct espress_stream_dgram dgram[1];
	struct espress_stream_dgram_address address[1];
	struct espress_stream_event event[1];
	struct espress_stream_mutex mutex[1];
	struct espress_stream_packet send_packet[ESPRESS_STREAM_SEND_QUEUE_SIZE];
	struct espress_stream_packet recv_packet[ESPRESS_STREAM_RECV_QUEUE_SIZE];
	struct espress_stream_dgram_buffer *send_buffers[ESPRESS_STREAM_SEND_QUEUE_SIZE];
	struct espress_stream_dgram_buffer *recv_buffers[ESPRESS_STREAM_RECV_QUEUE_SIZE];
	struct espress_stream_trans_ring send_ring[1];
	struct espress_stream_trans_ring recv_ring[1];
#	if 0 < ESPRESS_STREAM_MAX_NUM_SESSION
		struct espress_stream_trans_packet *trans_packet[ESPRESS_STREAM_MAX_NUM_SESSION];
		struct espress_stream_trans_packet **ptpl;
#	else
		struct espress_stream_trans_packet_interface_trans_packet_list
		{
			struct espress_stream_trans_packet *trans_packet;
			struct espress_stream_trans_packet_interface_trans_packet_list *next;
		} *trans_packet_list;
		struct espress_stream_trans_packet_interface_trans_packet_list *ptpl;
#	endif
};

struct espress_stream_trans_packet
{
	struct espress_stream_event *outer_event;

	struct espress_stream_trans_packet_interface *interface;
	i_t is_server;
	struct espress_stream_key send_key[1];
	struct espress_stream_key recv_key[1];

	struct espress_stream_dgram_address *src_address;
	struct espress_stream_uint32 connection_id[1];
	b_t src_private_key[ESPRESS_STREAM_KEY_EX_KEY_SIZE];
	b_t src_public_key[ESPRESS_STREAM_KEY_EX_KEY_SIZE];
	b_t src_random[32];
	struct espress_stream_uint32 src_counter_number[1];
	struct espress_stream_uint32 src_receive_counter_number[1];

	struct espress_stream_dgram_address dst_address[1];
	struct espress_stream_uint32 dst_connection_id[1];
	b_t dst_public_key[ESPRESS_STREAM_KEY_EX_KEY_SIZE];
	b_t shared_key[ESPRESS_STREAM_KEY_EX_KEY_SIZE];
	b_t dst_random[32];
	struct espress_stream_uint32 dst_counter_number[1];
	struct espress_stream_uint32 dst_receive_counter_number[1];
	struct espress_stream_uint32 dst_receive_window_address[1];

	struct espress_stream_uint16 const *src_packet_type_connect;
	struct espress_stream_uint16 const *src_packet_type_disconnect;
	struct espress_stream_uint16 const *src_packet_type_envelope;
	struct espress_stream_uint16 const *dst_packet_type_connect;
	struct espress_stream_uint16 const *dst_packet_type_disconnect;
	struct espress_stream_uint16 const *dst_packet_type_envelope;

	struct espress_stream_trans_packet_rto_measurement
	{
		i_t is_in_progress;
		struct espress_stream_uint32 origin_counter_number[1];
		struct espress_stream_uint32 start_counter_number_inc[1];
		struct espress_stream_uint32 start_time[1];
		i_t is_rtt_valid;
		struct espress_stream_uint32 rtt[1];

		i_t is_rto_calculated;
		struct espress_stream_uint32 rto[1];
		struct espress_stream_uint32 srtt[1];
		struct espress_stream_uint32 rttvar[1];
	} rto_measurement[1];

	struct espress_stream_trans_packet_duplicate_ack_count
	{
		u_t count;
	} duplicate_ack_count[1];
};

struct espress_stream_trans_packet_ops
{
	i_t (*initialize)(struct espress_stream_trans_packet_interface *this,
		struct espress_stream_dgram_address const *address);
	void (*finalize)(struct espress_stream_trans_packet_interface *this);
	u_t (*mtu)(struct espress_stream_trans_packet_interface *this);
	i_t (*open)(struct espress_stream_trans_packet *this,
		struct espress_stream_trans_packet_interface *interface,
		struct espress_stream_event *event,
		i_t is_server);
	void (*close)(struct espress_stream_trans_packet *this);
	i_t (*send_connect)(struct espress_stream_trans_packet *this,
		struct espress_stream_dgram_address const *address,
		struct espress_stream_uint32 *connection_param,
		u_t initial_window_size,
		b_t const *payload, u_t payload_size);
	i_t (*recv_connect)(struct espress_stream_trans_packet *this,
		struct espress_stream_dgram_address *address,
		struct espress_stream_uint32 *connection_param,
		u_t *initial_window_size,
		b_t *payload, u_t *payload_size);
	i_t (*send_disconnect)(struct espress_stream_trans_packet *this,
		b_t const *payload, u_t payload_size);
	i_t (*recv_disconnect)(struct espress_stream_trans_packet *this,
		b_t *payload, u_t *payload_size);
	i_t (*send_envelope)(struct espress_stream_trans_packet *this,
		struct espress_stream_uint32 const *receive_window_address,
		struct espress_stream_uint32 const *receive_window_size,
		struct espress_stream_uint32 const *send_data_address,
		b_t const *payload, u_t payload_size,
		b_t const *add_payload, u_t add_payload_size);
	i_t (*recv_envelope)(struct espress_stream_trans_packet *this,
		struct espress_stream_uint32 *receive_window_address,
		struct espress_stream_uint32 *receive_window_size,
		i_t *is_ack_true,
		struct espress_stream_uint32 *send_data_address,
		struct espress_stream_uint32 const *payload_address,
		b_t *payload, u_t *payload_size,
		b_t *add_payload, u_t *add_payload_size);
	i_t (*send_ack)(struct espress_stream_trans_packet *this,
		struct espress_stream_uint32 const *receive_window_address,
		struct espress_stream_uint32 const *receive_window_size);
	i_t (*recv_ack)(struct espress_stream_trans_packet *this,
		struct espress_stream_uint32 *receive_window_address,
		struct espress_stream_uint32 *receive_window_size,
		i_t *is_ack_true);
	i_t (*purge)(struct espress_stream_trans_packet *this);
	void (*src_address)(struct espress_stream_trans_packet *this, struct espress_stream_dgram_address *address);
	void (*dst_address)(struct espress_stream_trans_packet *this, struct espress_stream_dgram_address *address);
	void (*connection_id)(struct espress_stream_trans_packet *this, struct espress_stream_uint32 *connection_id);
	void (*shared_key)(struct espress_stream_trans_packet *this, b_t *shared_key);
	i_t (*rtt)(struct espress_stream_trans_packet *this, struct espress_stream_uint32 *rtt);
	void (*rto)(struct espress_stream_trans_packet *this, struct espress_stream_uint32 *rto);
	void (*tell_timeout)(struct espress_stream_trans_packet *this);
	u_t (*dac)(struct espress_stream_trans_packet *this);
	void (*tell_over_dac)(struct espress_stream_trans_packet *this);
	void (*yield[ESPRESS_STREAM_TRANS_PACKET_NUM_YIELD + 1])(struct espress_stream_trans_packet_interface *this, struct espress_stream_uint32 const *timeout);
	void (*yield_break[ESPRESS_STREAM_TRANS_PACKET_NUM_YIELD + 1])(struct espress_stream_trans_packet_interface *this);
};

extern struct espress_stream_trans_packet_ops const espress_stream_trans_packet[];

#endif /* __ESPRESS_STREAM_TRANS_PACKET_H__ */
