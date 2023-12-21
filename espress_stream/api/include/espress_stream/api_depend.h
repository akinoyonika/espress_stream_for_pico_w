#ifndef __ESPRESS_STREAM_API_DEPEND_H__
#define __ESPRESS_STREAM_API_DEPEND_H__

#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
#include "lwip/pbuf.h"
#include "lwip/udp.h"

#ifndef __ESPRESS_STREAM_DEPEND_H__
	struct sockaddr_storage {
		ip_addr_t address[1];
		u16_t port;
	};
#endif

#if PICO_CYW43_ARCH_POLL
#	define sched_yield() cyw43_arch_poll()
#else
#	define sched_yield() do{}while (0)
#endif

#endif /* __ESPRESS_STREAM_API_DEPEND_H__ */