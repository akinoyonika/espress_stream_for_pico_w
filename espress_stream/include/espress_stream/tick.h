#ifndef __ESPRESS_STREAM_TICK_H__
#define __ESPRESS_STREAM_TICK_H__

#define ESPRESS_STREAM_TICK_US_1_MSC ESPRESS_STREAM_UINT32(0x00, 0x00, 0x03, 0xe8)
#define ESPRESS_STREAM_TICK_US_1_SEC ESPRESS_STREAM_UINT32(0x00, 0x0f, 0x42, 0x40)
#define ESPRESS_STREAM_TICK_US_1_MIN ESPRESS_STREAM_UINT32(0x03, 0x93, 0x87, 0x00)
#define ESPRESS_STREAM_TICK_US_1_HOR ESPRESS_STREAM_UINT32(0xd6, 0x93, 0xa4, 0x00)

struct espress_stream_tick_ops
{
	i_t (*initialize)(void);
	void (*finalize)(void);
	void (*get_us)(struct espress_stream_uint32 *tick);
};

extern struct espress_stream_tick_ops const espress_stream_tick[];

extern struct espress_stream_uint32 const espress_stream_tick_us_1_msc[];
extern struct espress_stream_uint32 const espress_stream_tick_us_1_sec[];
extern struct espress_stream_uint32 const espress_stream_tick_us_1_min[];
extern struct espress_stream_uint32 const espress_stream_tick_us_1_hor[];

#endif /* __ESPRESS_STREAM_TICK_H__ */
