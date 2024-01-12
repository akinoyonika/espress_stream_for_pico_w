#include <stdio.h>

#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
#include "lwip/pbuf.h"
#include "lwip/udp.h"

#include "espress_stream/api.h"

#include "wifi_config.h"

#define ADDRESS "192.168.1.2"
#define PORT "1234"

static void _dump(unsigned char const *buffer, unsigned size)
{
	unsigned u;
	for (u = 0 ; u < size ; u++)
	{
		printf("%02x", buffer[u]);
	}
}

#undef assert
#define assert(b) do{if (!(b)){printf("error (%d)\n", __LINE__);for(;;);}}while (0)

static void test_main()
{
	int i;
	int j;
	int k;
	int l;
	int m;
	int n;
	unsigned long u;
	unsigned long v;
	unsigned w;
	struct sockaddr_storage address[1] = {0};
	void *ifd;
	void *client_sid;
	void *server_sid;
	void *sids[2];
	unsigned events[2];
#	define DATA_SIZE (1 << 10)
	static unsigned char send_data[DATA_SIZE];
	static unsigned char recv_data[DATA_SIZE];
	unsigned char client_session_hash[ESPRESS_STREAM_SESSION_HASH_SIZE];
	unsigned char server_session_hash[ESPRESS_STREAM_SESSION_HASH_SIZE];
#	define DATA_MASK (DATA_SIZE - 1)
#	define TEST_DATA_SIZE (1024 * DATA_SIZE)
#	define TEST_NUM_REPEAT (4)

	for (i = 0 ; i < sizeof(send_data) ; i++) {
		send_data[i] = (unsigned char)i;
	}

	i = espress_stream_begin();
	assert(i == 0);

	printf("%s\n", espress_stream_copyright());

	address->port = 1234;
	ipaddr_aton("0.0.0.0", address->address);
	i = espress_stream_begin_interface(&ifd, address);
	assert(i == 0);

	for (n = 0 ; n < TEST_NUM_REPEAT ; n++) {
		i = espress_stream_begin_session(&client_sid, ifd, 0, NULL);
		assert(i == 0);
		i = espress_stream_begin_session(&server_sid, ifd, 1, NULL);
		assert(i == 0);

		sids[0] = client_sid;
		sids[1] = server_sid;
		events[0] =
			ESPRESS_STREAM_EVENT_CONNECT_RESULT |
			ESPRESS_STREAM_EVENT_CONNECT_DETECT |
			ESPRESS_STREAM_EVENT_DISCONNECT_RESULT |
			ESPRESS_STREAM_EVENT_SEND |
			ESPRESS_STREAM_EVENT_RECV;
		events[1] = events[0];
		printf("api: ."); fflush(stdout);
		i = espress_stream_poll(sids, events, 2, 1000000);
		assert(i == 0);
		printf("."); fflush(stdout);
		i = espress_stream_poll(sids, events, 2, 1000000);
		assert(i == 0);
		printf("."); fflush(stdout);
		i = espress_stream_poll(sids, events, 2, 1000000);
		assert(i == 0);
		printf("\n");

		printf("api: server: listen\n");

		address->port = atoi(PORT);
		ipaddr_aton(ADDRESS, address->address);
		k = espress_stream_connect(client_sid, address);
		assert(k == 0);
		printf("api: client: connect\n");

		sids[0] = client_sid;
		sids[1] = server_sid;
		events[0] = ESPRESS_STREAM_EVENT_CONNECT_RESULT;
		events[1] = ESPRESS_STREAM_EVENT_CONNECT_DETECT;
		for (i = -1, j = -1, k = -1, l = -1 ; (i == -1) || (j == -1) ; ) {
			m = espress_stream_poll(sids, events, 2, 0xffffffff);
			assert(0 < m);

			if (i == -1) {
				i = espress_stream_connect_detect(server_sid, 0);
				assert(i <= 0);
				if (i == 0) {
					printf("api: server: connect_detect\n");
					events[1] = ESPRESS_STREAM_EVENT_RECV;
				}
			} else {
				if (k == -1) {
					w = sizeof(recv_data);
					k = espress_stream_recv(server_sid, recv_data, &w, 0);
					assert (k <= 0);
					if ((k == 0) && (w == 0)) {
						printf("api: server: disconnect_detect\n");
						k = espress_stream_disconnect(server_sid);
						assert(k == 0);
						events[1] = ESPRESS_STREAM_EVENT_DISCONNECT_RESULT;
					} else {
						k = -1;
					}
				} else {
					if (espress_stream_disconnect_result(server_sid, 0) == 0) {
						espress_stream_end_session(server_sid);
						m = espress_stream_begin_session(&server_sid, ifd, 1, NULL);
						assert(m == 0);
						printf("api: server: listen\n");
						i = -1;
						k = -1;
						events[1] = ESPRESS_STREAM_EVENT_CONNECT_DETECT;
					}
				}
			}

			if (j == -1) {
				j = espress_stream_connect_result(client_sid, 0);
				assert(j <= 0);
				if (j == 0) {
					printf("api: client: connect_result\n");
					events[0] = ESPRESS_STREAM_EVENT_RECV;
				}
			} else {
				if (l == -1) {
					w = sizeof(recv_data);
					l = espress_stream_recv(client_sid, recv_data, &w, 0);
					assert(l <= 0);
					if ((l == 0) && (w == 0)) {
						printf("api: client: disconnect_detect\n");
						l = espress_stream_disconnect(client_sid);
						assert(l == 0);
						events[0] = ESPRESS_STREAM_EVENT_DISCONNECT_RESULT;
					} else {
						l = -1;
					}
				} else {
					if (espress_stream_disconnect_result(client_sid, 0) == 0) {
						espress_stream_end_session(client_sid);
						m = espress_stream_begin_session(&client_sid, ifd, 1, NULL);
						assert(m == 0);
						m = espress_stream_connect(client_sid, address);
						assert(m == 0);
						printf("api: client: connect\n");
						j = -1;
						l = -1;
						events[0] = ESPRESS_STREAM_EVENT_CONNECT_RESULT;
					}
				}
			}
		}

		espress_stream_src_address(client_sid, address);
		printf("api: client: src_address = %s:%u\n",
			ipaddr_ntoa(address->address),
			address->port);
		espress_stream_src_address(server_sid, address);
		printf("api: server: src_address = %s:%u\n",
			ipaddr_ntoa(address->address),
			address->port);
		espress_stream_dst_address(client_sid, address);
		printf("api: client: dst_address = %s:%u\n",
			ipaddr_ntoa(address->address),
			address->port);
		espress_stream_dst_address(server_sid, address);
		printf("api: server: dst_address = %s:%u\n",
			ipaddr_ntoa(address->address),
			address->port);
		printf("api: client: connection_id = %08x\n", espress_stream_connection_id(client_sid));
		printf("api: server: connection_id = %08x\n", espress_stream_connection_id(server_sid));
		assert(espress_stream_connection_id(client_sid) == espress_stream_connection_id(server_sid));
		espress_stream_session_hash(client_sid, client_session_hash);
		printf("api: client: session_hash = "); _dump(client_session_hash, ESPRESS_STREAM_SESSION_HASH_SIZE); printf("\n");
		espress_stream_session_hash(server_sid, server_session_hash);
		printf("api: server: session_hash = "); _dump(server_session_hash, ESPRESS_STREAM_SESSION_HASH_SIZE); printf("\n");
		assert(0 == memcmp(client_session_hash, server_session_hash, ESPRESS_STREAM_SESSION_HASH_SIZE));

		sids[0] = client_sid;
		sids[1] = server_sid;
		events[0] = ESPRESS_STREAM_EVENT_SEND | ESPRESS_STREAM_EVENT_RECV;
		events[1] = ESPRESS_STREAM_EVENT_RECV;
		for (i = -1, j = -1, u = 0, v = 0 ; (i == -1) || (j == -1) ; ) {
			if (0 < u) {
				m = espress_stream_poll(sids, events, 2, 0xffffffff);
				assert(0 < m);
			}
			if (i == -1) {
				w = sizeof(recv_data);
				k = espress_stream_recv(client_sid, recv_data, &w, 0);
				assert(k <= 0);
				if ((k == 0) && (w == 0)) {
					printf("\n\n\napi: client: disconnect_detect\n");
					i = espress_stream_disconnect(client_sid);
					printf("api: client: disconnect\n");
					assert(i == 0);
				} else {
					if (u < TEST_DATA_SIZE) {
						do {
							w = sizeof(send_data) - (u & DATA_MASK);
							k = espress_stream_send(client_sid, send_data + (u & DATA_MASK), &w, 0);
							assert(k <= 0);
							if (k == 0) {
								assert(0 < w);
								u += w;
							}
						} while ((k == 0) && (u < TEST_DATA_SIZE));
					}
					if (u == TEST_DATA_SIZE) {
						i = espress_stream_flush(client_sid, 0);
						assert(i <= 0);
						if (i == 0) {
							i = espress_stream_disconnect(client_sid);
							printf("\n\n\napi: client: disconnect\n");
							assert(i == 0);
						}
					}
				}
			}
			if (j == -1) {
				for ( ; ; ) {
					w = sizeof(recv_data) - (v & DATA_MASK);
					k = espress_stream_recv(server_sid, recv_data, &w, 0);
					assert(k <= 0);
					if (k == 0) {
						if (0 < w) {
							assert(0 == memcmp(recv_data, send_data + (v & DATA_MASK), w));
							v += w;
						} else {
							printf("\n\n\napi: server: disconnect_detect\n");
							assert(v == TEST_DATA_SIZE);
							j = espress_stream_disconnect(server_sid);
							printf("api: server: disconnect\n");
							assert(j == 0);
							break;
						}
					} else {
						break;
					}
				}
			}
			printf("api: client (%016lu-%08u)\n", u, espress_stream_buffered_send_size(client_sid, NULL));
			printf("\t-(%08u, ", espress_stream_rtt(client_sid));
			printf("%08u, ", espress_stream_rto(client_sid));
			printf("%08u, ", espress_stream_send_timeout_count(client_sid));
			printf("%04u, ", espress_stream_dac(client_sid));
			printf("%08u, ", espress_stream_fast_retransmit_count(client_sid));
			printf("%04u)->\n", espress_stream_cwnd(client_sid));
			printf("\tserver (%016lu-%08u)\r", v, espress_stream_buffered_recv_size(server_sid, NULL));
			printf("\x1b[2A");
		}
		printf("\n\n\n");

		sids[0] = client_sid;
		sids[1] = server_sid;
		events[0] = ESPRESS_STREAM_EVENT_DISCONNECT_RESULT;
		events[1] = ESPRESS_STREAM_EVENT_DISCONNECT_RESULT;
		for (i = -1, j = -1 ; (i == -1) || (j == -1) ; ) {
			m = espress_stream_poll(sids, events, 2, 0xffffffff);
			assert(0 < m);

			if (i == -1) {
				i = espress_stream_disconnect_result(client_sid, 0);
				assert(i <= 0);
				if (i == 0) {
					printf("api: client: disconnect_result\n");
				}
			}
			if (j == -1) {
				j = espress_stream_disconnect_result(server_sid, 0);
				assert(j <= 0);
				if (j == 0) {
					printf("api: server: disconnect_result\n");
				}
			}
		}

		espress_stream_end_session(server_sid);
		espress_stream_end_session(client_sid);
	}

	espress_stream_end_interface(ifd);

	espress_stream_end();
}

int main()
{
	int ret = 0;
	stdio_init_all();
	if (cyw43_arch_init()) {
		printf("Wi-fi initialization failure.\n");
		ret = 1;
		goto EXIT;
	}
	cyw43_arch_enable_sta_mode();
	printf("Connecting to Wi-fi...\n");
	if (cyw43_arch_wifi_connect_timeout_ms(WIFI_SSID, WIFI_PASSWORD, CYW43_AUTH_WPA2_AES_PSK, 30000)) {
		printf("Wi-fi connection failure.\n");
	} else {
		printf("Wi-fi connection success (%s).\n", ip4addr_ntoa(netif_ip4_addr(netif_list)));
		test_main();
	}
	cyw43_arch_deinit();
EXIT:
	return ret;
}
