#ifndef __ESPRESS_STREAM_PACKET_H__
#define __ESPRESS_STREAM_PACKET_H__

#define ESPRESS_STREAM_PACKET_TYPE_BASE_CONNECT (0x43)
#define ESPRESS_STREAM_PACKET_TYPE_BASE_DISCONNECT (0x44)
#define ESPRESS_STREAM_PACKET_TYPE_BASE_ENVELOPE (0x45)
#define ESPRESS_STREAM_PACKET_TYPE_CLIENT_CONNECT ESPRESS_STREAM_UINT16( \
	ESPRESS_STREAM_PACKET_TYPE_BASE_CONNECT, ESPRESS_STREAM_PACKET_TYPE_BASE_CONNECT)
#define ESPRESS_STREAM_PACKET_TYPE_CLIENT_DISCONNECT ESPRESS_STREAM_UINT16( \
	ESPRESS_STREAM_PACKET_TYPE_BASE_DISCONNECT, ESPRESS_STREAM_PACKET_TYPE_BASE_DISCONNECT)
#define ESPRESS_STREAM_PACKET_TYPE_CLIENT_ENVELOPE ESPRESS_STREAM_UINT16( \
	ESPRESS_STREAM_PACKET_TYPE_BASE_ENVELOPE, ESPRESS_STREAM_PACKET_TYPE_BASE_ENVELOPE)
#define ESPRESS_STREAM_PACKET_TYPE_SERVER_CONNECT ESPRESS_STREAM_UINT16( \
	ESPRESS_STREAM_PACKET_TYPE_BASE_CONNECT + 0x20, ESPRESS_STREAM_PACKET_TYPE_BASE_CONNECT + 0x20)
#define ESPRESS_STREAM_PACKET_TYPE_SERVER_DISCONNECT ESPRESS_STREAM_UINT16( \
	ESPRESS_STREAM_PACKET_TYPE_BASE_DISCONNECT + 0x20, ESPRESS_STREAM_PACKET_TYPE_BASE_DISCONNECT + 0x20)
#define ESPRESS_STREAM_PACKET_TYPE_SERVER_ENVELOPE ESPRESS_STREAM_UINT16( \
	ESPRESS_STREAM_PACKET_TYPE_BASE_ENVELOPE + 0x20, ESPRESS_STREAM_PACKET_TYPE_BASE_ENVELOPE + 0x20)

#define ESPRESS_STREAM_PACKET_CONNECT_MIN_SIZE (0x0050)
#define ESPRESS_STREAM_PACKET_DISCONNECT_MIN_SIZE (0x000c)
#define ESPRESS_STREAM_PACKET_ENVELOPE_ACK_SIZE (0x0018)
#define ESPRESS_STREAM_PACKET_ENVELOPE_MIN_SIZE (0x001c)
#define ESPRESS_STREAM_PACKET_MIN_SIZE (ESPRESS_STREAM_PACKET_DISCONNECT_MIN_SIZE)
#define ESPRESS_STREAM_PACKET_MAX_SIZE (ESPRESS_STREAM_DGRAM_MTU - ESPRESS_STREAM_MAC_MAC_SIZE)
#define ESPRESS_STREAM_PACKET_CONNECT_PAYLOAD_MAX_SIZE (ESPRESS_STREAM_PACKET_MAX_SIZE - ESPRESS_STREAM_PACKET_CONNECT_MIN_SIZE)
#define ESPRESS_STREAM_PACKET_DISCONNECT_PAYLOAD_MAX_SIZE (ESPRESS_STREAM_PACKET_MAX_SIZE - ESPRESS_STREAM_PACKET_DISCONNECT_MIN_SIZE)
#define ESPRESS_STREAM_PACKET_ENVELOPE_PAYLOAD_MAX_SIZE (ESPRESS_STREAM_PACKET_MAX_SIZE - ESPRESS_STREAM_PACKET_ENVELOPE_MIN_SIZE)

#define espress_stream_packet_max_size(mtu) ((mtu) - ESPRESS_STREAM_MAC_MAC_SIZE)
#define espress_stream_packet_connect_payload_max_size(mtu) (espress_stream_packet_max_size(mtu) - ESPRESS_STREAM_PACKET_CONNECT_MIN_SIZE)
#define espress_stream_packet_disconnect_payload_max_size(mtu) (espress_stream_packet_max_size(mtu) - ESPRESS_STREAM_PACKET_DISCONNECT_MIN_SIZE)
#define espress_stream_packet_envelope_payload_max_size(mtu) (espress_stream_packet_max_size(mtu) - ESPRESS_STREAM_PACKET_ENVELOPE_MIN_SIZE)

struct espress_stream_packet
{
	espress_stream_dgram_buffer_header;
	b_t data[ESPRESS_STREAM_DGRAM_MTU];
	struct espress_stream_event *outer_event;
	struct espress_stream_key *key;
	struct espress_stream_event event[1];
	struct espress_stream_mutex mutex[1];

	struct espress_stream_uint16 const *packet_type_connect;
	struct espress_stream_uint16 const *packet_type_disconnect;
	struct espress_stream_uint16 const *packet_type_envelope;

	void *id;

	volatile int is_in_progress;
};

struct espress_stream_packet_ops
{
	i_t (*initialize)(struct espress_stream_packet *this, struct espress_stream_event *event);
	void (*finalize)(struct espress_stream_packet *this);
	struct espress_stream_uint16 *(*packet_type)(struct espress_stream_packet *this);
	struct espress_stream_uint16 *(*packet_size)(struct espress_stream_packet *this);
	struct espress_stream_uint32 *(*connection_id)(struct espress_stream_packet *this);
	struct espress_stream_uint32 *(*connection_param)(struct espress_stream_packet *this);
	struct espress_stream_uint32 *(*initial_window_size)(struct espress_stream_packet *this);
	b_t *(*key_change)(struct espress_stream_packet *this);
	u_t (*key_change_size)(struct espress_stream_packet *this);
	b_t *(*random)(struct espress_stream_packet *this);
	u_t (*random_size)(struct espress_stream_packet *this);
	struct espress_stream_uint32 *(*counter_number)(struct espress_stream_packet *this);
	struct espress_stream_uint32 *(*receive_counter_number)(struct espress_stream_packet *this);
	struct espress_stream_uint32 *(*receive_window_address)(struct espress_stream_packet *this);
	struct espress_stream_uint32 *(*receive_window_size)(struct espress_stream_packet *this);
	struct espress_stream_uint32 *(*send_data_address)(struct espress_stream_packet *this);
	b_t *(*connect_payload)(struct espress_stream_packet *this);
	u_t (*connect_payload_size)(struct espress_stream_packet *this);
	b_t *(*disconnect_payload)(struct espress_stream_packet *this);
	u_t (*disconnect_payload_size)(struct espress_stream_packet *this);
	b_t *(*envelope_payload)(struct espress_stream_packet *this);
	u_t (*envelope_payload_size)(struct espress_stream_packet *this);
	void (*encode)(struct espress_stream_packet *this, i_t is_server, struct espress_stream_key *key);
	void (*decode)(struct espress_stream_packet *this, i_t is_server, struct espress_stream_key *key);
	void (*abort_encode)(struct espress_stream_packet *this);
	void (*abort_decode)(struct espress_stream_packet *this);
	void (*yield)(struct espress_stream_packet *this, struct espress_stream_uint32 const *timeout);
	void (*yield_break)(struct espress_stream_packet *this);
};

extern struct espress_stream_packet_ops const espress_stream_packet[];

extern struct espress_stream_uint16 const espress_stream_packet_type_client_connect[];
extern struct espress_stream_uint16 const espress_stream_packet_type_client_disconnect[];
extern struct espress_stream_uint16 const espress_stream_packet_type_client_envelope[];
extern struct espress_stream_uint16 const espress_stream_packet_type_server_connect[];
extern struct espress_stream_uint16 const espress_stream_packet_type_server_disconnect[];
extern struct espress_stream_uint16 const espress_stream_packet_type_server_envelope[];

#endif /* __ESPRESS_STREAM_PACKET_H__ */
