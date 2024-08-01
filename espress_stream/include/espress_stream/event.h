#ifndef __ESPRESS_STREAM_EVENT_H__
#define __ESPRESS_STREAM_EVENT_H__

enum
{
	ESPRESS_STREAM_EVENT_NONE,
	ESPRESS_STREAM_EVENT_DGRAM_SEND_RESULT,
	ESPRESS_STREAM_EVENT_DGRAM_RECV_RESULT,
	ESPRESS_STREAM_EVENT_PACKET_ENCODE_RESULT,
	ESPRESS_STREAM_EVENT_PACKET_DECODE_RESULT,
	ESPRESS_STREAM_EVENT_TRANS_PACKET_SEND,
	ESPRESS_STREAM_EVENT_TRANS_PACKET_RECV,
	ESPRESS_STREAM_EVENT_SESSION_CONNECT_RESULT,
	ESPRESS_STREAM_EVENT_SESSION_CONNECT_DETECT,
	ESPRESS_STREAM_EVENT_SESSION_DISCONNECT_RESULT,
	ESPRESS_STREAM_EVENT_SESSION_DISCONNECT_DETECT,
	ESPRESS_STREAM_EVENT_SESSION_SEND,
	ESPRESS_STREAM_EVENT_SESSION_RECV,
	ESPRESS_STREAM_EVENT_SESSION_ERROR
};

#define ESPRESS_STREAM_EVENT_FLAGS_MAX_NUM_FLAG_CLASS (32)
#if 16 == ESPRESS_STREAM_EVENT_FLAGS_MAX_NUM_FLAG_CLASS
#	define ESPRESS_STREAM_EVENT_FLAGS_UINT(s) ESPRESS_STREAM_UINT16 ## s
#	define espress_stream_event_flags_uint(s) espress_stream_uint16 ## s
#elif 32 == ESPRESS_STREAM_EVENT_FLAGS_MAX_NUM_FLAG_CLASS
#	define ESPRESS_STREAM_EVENT_FLAGS_UINT(s) ESPRESS_STREAM_UINT32 ## s
#	define espress_stream_event_flags_uint(s) espress_stream_uint32 ## s
#elif 64 == ESPRESS_STREAM_EVENT_FLAGS_MAX_NUM_FLAG_CLASS
#	define ESPRESS_STREAM_EVENT_FLAGS_UINT(s) ESPRESS_STREAM_UINT64 ## s
#	define espress_stream_event_flags_uint(s) espress_stream_uint64 ## s
#elif 128 == ESPRESS_STREAM_EVENT_FLAGS_MAX_NUM_FLAG_CLASS
#	define ESPRESS_STREAM_EVENT_FLAGS_UINT(s) ESPRESS_STREAM_UINT128 ## s
#	define espress_stream_event_flags_uint(s) espress_stream_uint128 ## s
#elif 256 == ESPRESS_STREAM_EVENT_FLAGS_MAX_NUM_FLAG_CLASS
#	define ESPRESS_STREAM_EVENT_FLAGS_UINT(s) ESPRESS_STREAM_UINT256 ## s
#	define espress_stream_event_flags_uint(s) espress_stream_uint256 ## s
#elif 512 == ESPRESS_STREAM_EVENT_FLAGS_MAX_NUM_FLAG_CLASS
#	define ESPRESS_STREAM_EVENT_FLAGS_UINT(s) ESPRESS_STREAM_UINT512 ## s
#	define espress_stream_event_flags_uint(s) espress_stream_uint512 ## s
#else
#	error
#endif

#define ESPRESS_STREAM_EVENT_FLAGS_INIT {{ESPRESS_STREAM_EVENT_FLAGS_UINT(_0)}}

struct espress_stream_event_flags
{
	struct espress_stream_event_flags_uint() flags[1];
};

struct espress_stream_event_flags_ops
{
	void (*reset_all)(struct espress_stream_event_flags *this);
	void (*set)(struct espress_stream_event_flags *this, u_t flag);
	void (*reset)(struct espress_stream_event_flags *this, u_t flag);
	i_t (*is_set)(struct espress_stream_event_flags const *this, u_t flag);
	i_t (*is_set_any)(struct espress_stream_event_flags const *this);
	void (*compo)(struct espress_stream_event_flags *this, struct espress_stream_event_flags const *flags);
};

extern struct espress_stream_event_flags_ops const espress_stream_event_flags[];

#define ESPRESS_STREAM_EVENT_MAX_NUM_POLL_EVENT (ESPRESS_STREAM_MAX_NUM_SESSION)

struct espress_stream_event
{
	struct espress_stream_cond cond[1];
	struct espress_stream_event_flags flags[1];
};

struct espress_stream_event_ops
{
	i_t (*initialize)(struct espress_stream_event *this);
	void (*finalize)(struct espress_stream_event *this);
	void (*set)(struct espress_stream_event *this, u_t flag);
	void (*wait)(struct espress_stream_event *this, struct espress_stream_event_flags *flags, struct espress_stream_uint32 const *timeout);
	i_t (*poll)(struct espress_stream_event * const *these, u_t num, struct espress_stream_uint32 const *timeout);
};

extern struct espress_stream_event_ops const espress_stream_event[];

#endif /* __ESPRESS_STREAM_EVENT_H__ */
