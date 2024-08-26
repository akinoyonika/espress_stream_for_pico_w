#ifndef __ESPRESS_STREAM_DEPEND_H__
#define __ESPRESS_STREAM_DEPEND_H__

#include <string.h>
//#include <byteswap.h>
#include <limits.h>
#include <stdint.h>
#include <stddef.h>
#include <stdarg.h>
//#include <time.h>
#include <unistd.h>
#include <errno.h>
//#include <sys/socket.h>
//#include <netinet/ip.h>
//#include <poll.h>
//#include <sched.h>
#include <stdlib.h>
//#include <sys/eventfd.h>

#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
#include "pico/rand.h"
#include "lwip/pbuf.h"
#include "lwip/udp.h"

struct sockaddr_storage {
	ip_addr_t address[1];
	u16_t port;
};

#define ESPRESS_STREAM_IS_HOST_BYTE_ORDER_LITTLE (1)
#define C_T_WIDTH CHAR_WIDTH
#define B_T_WIDTH UCHAR_WIDTH
#define I_T_WIDTH (4)
#define I_T_MIN (0x80000000)
#define I_T_MAX (0x7fffffff)
#define U_T_WIDTH (4)
#define U_T_MIN (0x00000000)
#define U_T_MAX (0xffffffff)
#define MAX_I_T_WIDTH (8)
#define MAX_I_T_MIN (0x8000000000000000)
#define MAX_I_T_MAX (0x7fffffffffffffff)
#define MAX_U_T_WIDTH (8)
#define MAX_U_T_MIN (0x0000000000000000)
#define MAX_U_T_MAX (0xffffffffffffffff)
#define c_t char
#define b_t unsigned char
#define i8_t char
#define u8_t unsigned char
#if (2 <= MAX_I_T_WIDTH) && (2 <= MAX_U_T_WIDTH)
#	define bswap_16(n) (((u16_t)((u8_t)n) << 8) | (u16_t)(u8_t)((n) >> 8))
#	define i16_t int16_t
#	define u16_t uint16_t
#	define swap_endian_16(n) (*(n) = bswap_16(*(n)))
#endif
#if (4 <= MAX_I_T_WIDTH) && (4 <= MAX_U_T_WIDTH)
#	define bswap_32(n) (((u32_t)bswap_16((u16_t)(n)) << 16) | (u32_t)bswap_16((u16_t)((n) >> 16)))
#	define i32_t int32_t
#	define u32_t uint32_t
#	define swap_endian_32(n) (*(n) = bswap_32(*(n)))
#endif
#if (8 <= MAX_I_T_WIDTH) && (8 <= MAX_U_T_WIDTH)
#	define bswap_64(n) (((u64_t)bswap_32((u32_t)(n)) << 32) | (u64_t)bswap_32((u32_t)((n) >> 32)))
#	define i64_t int64_t
#	define u64_t uint64_t
#	define swap_endian_64(n) (*(n) = bswap_64(*(n)))
#endif
#if (16 <= MAX_I_T_WIDTH) && (16 <= MAX_U_T_WIDTH)
#	define bswap_128(n) (((u128_t)bswap_64((u64_t)(n)) << 64) | (u128_t)bswap_64((u64_t)((n) >> 64)))
#	define i128_t __int128_t
#	define u128_t __uint128_t
#	define swap_endian_128(n) (*(n) = bswap_128(*(n)))
#endif
#if (2 == I_T_WIDTH) && (2 == U_T_WIDTH)
#	define i_t int16_t
#	define u_t uint16_t
#elif (4 == I_T_WIDTH) && (4 == U_T_WIDTH)
#	define i_t int32_t
#	define u_t uint32_t
#elif (8 == I_T_WIDTH) && (8 == U_T_WIDTH)
#	define i_t int64_t
#	define u_t uint64_t
#elif (16 == I_T_WIDTH) && (16 == U_T_WIDTH)
#	define i_t __int128_t
#	define u_t __uint128_t
#else
#	error
#endif

#endif /* __ESPRESS_STREAM_DEPEND_H__ */

