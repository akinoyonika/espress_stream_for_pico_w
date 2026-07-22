#include <stdio.h>

#include "pico/stdlib.h"
#include "pico/rand.h"
#include "pico/cyw43_arch.h"
#include "lwip/pbuf.h"
#include "lwip/udp.h"
#include "tusb.h"

#include "espress_stream/api.h"
#include "cipher_api.h"

#include "wifi_config.h"

#define ADDRESS "192.168.1.2"
#define PORT "12345"
#define PUBLIC_KEY { \
	0x73, 0x35, 0xe6, 0x3a, 0x76, 0x21, 0x23, 0xa9, 0x41, 0xa2, 0x5e, 0x5e, 0x53, 0x8d, 0xd4, 0xd2, \
	0xe4, 0x7d, 0x86, 0x92, 0x80, 0xf8, 0x3a, 0xf7, 0x81, 0x0e, 0x0b, 0x3d, 0xae, 0x80, 0x69, 0xc9 \
}
#define HALF_CLOSE (0)

#define TIMEOUT (30 * 1000 * 1000)
#define BUFFER_SIZE (512)
#define CONNECT_MESSAGE_SIZE (64)

#define STDIN_FILENO (0)
#define STDOUT_FILENO (1)
#define STDERR_FILENO (2)
#define EAGAIN (11)
static int errno = 0;

static ssize_t read(int fd, void *buf, size_t count)
{
	ssize_t ret;
	uint32_t available;

	if (fd != STDIN_FILENO) {
		ret = -1;
		errno = 255;
		goto EXIT;
	}

	available = tud_cdc_available();

	if (available == 0) {
		ret = -1;
		errno = EAGAIN;
		goto EXIT;
	}

	if (buf == NULL) {
		ret = available;
		goto EXIT;
	}

	if (available < count) {
		count = available;
	}

	ret = tud_cdc_read(buf, count);

	if (ret == 0) {
		ret = -1;
		errno = 255;
	}

EXIT:

	return ret;
}

static ssize_t write(int fd, void const *buf, size_t count)
{
	ssize_t ret;
	uint32_t available;

	if ((fd != STDOUT_FILENO) && (fd != STDERR_FILENO)) {
		ret = -1;
		errno = 255;
		goto EXIT;
	}

	available = tud_cdc_write_available();

	if (available == 0) {
		ret = -1;
		errno = EAGAIN;
		goto EXIT;
	}

	if (buf == NULL) {
		ret = available;
		goto EXIT;
	}

	if (available < count) {
		count = available;
	}

	ret = tud_cdc_write(buf, count);

	if (ret == 0) {
		ret = -1;
		errno = 255;
	}

	tud_cdc_write_flush();

EXIT:

	return ret;
}

static int close(int fd)
{
	(void)fd;
	return 0;
}

static void *ifd;
static const char *arg_address = ADDRESS;
static const char *arg_port = PORT;
static unsigned char arg_public_key[] = PUBLIC_KEY;
static int arg_is_close_half = HALF_CLOSE;

struct session_attr {
	void *sid;
	volatile int is_running;
	unsigned char message[CONNECT_MESSAGE_SIZE];
	unsigned message_size;
	int event;
};
struct session_attr session_attr[1];

static unsigned getrandom(unsigned char *b, unsigned s, unsigned f)
{
	unsigned ret = 0;
	static unsigned char r[16];
	static unsigned r_size = 0;
	(void)f;
	if (0 < s) {
		if (r_size == 0) {
			get_rand_128((rng_128_t *)r);
			r_size = 16;
		}
		*b = r[16 - r_size];
		r_size--;
		ret = 1;
	}
	return ret;
}

static int send_connect_payload(void *sid, int (*callback)(void *sid, void const *payload, unsigned payload_size, int is_rejected), void *param)
{
	int ret;
	unsigned char *message = ((struct session_attr *)param)->message;
	unsigned *message_size = &(((struct session_attr *)param)->message_size);
#	define get_random(buf, buf_sz) do \
	{                                 \
		unsigned char *b = (buf);     \
		unsigned u = (buf_sz);        \
		ssize_t s;                    \
		while (0 < u) {               \
			s = getrandom(b, u, 0);   \
			if (0 < s) {              \
				b += s;               \
				u -= s;               \
			}                         \
		}                             \
	} while (0)
	if (arg_public_key == NULL) {
		*message_size = CONNECT_MESSAGE_SIZE; /* dummy size */
		ret = callback(sid, NULL, 0, 0);
	} else {
		*message_size = CONNECT_MESSAGE_SIZE;
		get_random(message, *message_size);
		ret = callback(sid, message, *message_size, 0);
	}
	return ret;
}

static int recv_connect_payload(void *sid, int (*callback)(void *sid, void *payload, unsigned *payload_size), void *param)
{
	int ret;
	unsigned char *message = ((struct session_attr *)param)->message;
	unsigned *message_size = &(((struct session_attr *)param)->message_size);
	unsigned char sign_message[CIPHER_ED25519_SIGNATURE_SIZE + CONNECT_MESSAGE_SIZE];
	unsigned sign_message_size = CIPHER_ED25519_SIGNATURE_SIZE + CONNECT_MESSAGE_SIZE;
	ret = callback(sid, sign_message, &sign_message_size);
	if ((arg_public_key != NULL) &&
			((ret != 0) ||
				(sign_message_size != CIPHER_ED25519_SIGNATURE_SIZE + CONNECT_MESSAGE_SIZE) ||
				(memcmp(message, sign_message + CIPHER_ED25519_SIGNATURE_SIZE, CONNECT_MESSAGE_SIZE) != 0) ||
				(cipher_ed25519_auth(sign_message, arg_public_key, sign_message + CIPHER_ED25519_SIGNATURE_SIZE, sign_message_size - CIPHER_ED25519_SIGNATURE_SIZE) != 0))) {
		*message_size = 0;
	}
	return ret;
}

static int conn(struct session_attr *attr,
	struct sockaddr_storage const *address,
	unsigned timeout)
{
	int i;
	unsigned t;
	volatile int *is_running = &(attr->is_running);
	void **sid = &(attr->sid);
	void *new_sid;
	void *old_sid;
	unsigned const start_time = espress_stream_time();
	struct espress_stream_session_ex_param ex_param[1] = {{
		send_connect_payload, NULL,
		recv_connect_payload, NULL,
		NULL, NULL,
		NULL, NULL
	}};
	unsigned *message_size = &(attr->message_size);

	i = espress_stream_connect(*sid, address);
	if (i == 0) {
		for (i = -1, t = 0 ; *is_running && (i == -1) ; ) {
			t = timeout - t;
			i = espress_stream_connect_result(*sid, t);
			if (0 < i) {
				ex_param->send_connect_payload_param = attr;
				ex_param->recv_connect_payload_param = attr;
				i = espress_stream_begin_session(&new_sid, ifd, 0, ex_param);
				if (i == 0) {
					old_sid = *sid;
					*sid = new_sid;
					espress_stream_end_session(old_sid);
					i = espress_stream_connect(*sid, address);
					if (i == 0) {
						i = -1;
					}
				}
			} else if ((i == 0) && (*message_size == 0)) {
				espress_stream_disconnect(*sid);
				espress_stream_disconnect_result(*sid, *is_running ? TIMEOUT : 0);
				i = 1;
			}

			if (timeout <= (t = ((espress_stream_time() - start_time) & 0xffffffff))) {
				break;
			}
		}
	}

	return i;
}

static int flush(struct session_attr *attr, unsigned timeout)
{
	int i;
	unsigned u;
	unsigned v;
	unsigned t;
	volatile int *is_running = &(attr->is_running);
	void **sid = &(attr->sid);
	unsigned start_time;

	espress_stream_flush(*sid, 0);
	for (i = -1, u = espress_stream_buffered_send_size(*sid, NULL), start_time = espress_stream_time(), t = 0 ;
		*is_running & (i == -1) ;
		u = v) {
		t = timeout - t;
		i = espress_stream_flush(*sid, t);

		v = espress_stream_buffered_send_size(*sid, NULL);
		if (u != v) {
			start_time = espress_stream_time();
		}

		if (timeout <= (t = ((espress_stream_time() - start_time) & 0xffffffff))) {
			break;
		}
	}

	return i;
}

static void wait(struct session_attr *attr,
	int is_stdin, int is_stdout, int is_sidin, int is_sidout,
	unsigned timeout)
{
	unsigned u;
	unsigned start_time = espress_stream_time();

	do {
		espress_stream_poll(NULL, NULL, 0, 0);
		if (is_stdin && (0 < read(STDIN_FILENO, NULL, 0))) break;
		if (is_stdout && (0 < write(STDOUT_FILENO, NULL, 0))) break;
		if (is_sidin && (u = 1, (-1 != espress_stream_recv(attr->sid, NULL, &u, 0)))) break;
		if (is_sidout && (u = 1, (-1 != espress_stream_send(attr->sid, NULL, &u, 0)))) break;
	} while ((espress_stream_time() - start_time) < timeout);
}

static void cat(struct session_attr *attr)
{
	int i;
	ssize_t st;
	unsigned u;
	unsigned v;
	volatile int *is_running = &(attr->is_running);
	void **sid = &(attr->sid);
	unsigned char read_buf[BUFFER_SIZE];
	unsigned read_buf_size = 0;
	unsigned char recv_buf[BUFFER_SIZE];
	unsigned recv_buf_size = 0;
	unsigned char *write_buf = recv_buf;
#	define write_buf_size recv_buf_size
	unsigned char *send_buf = read_buf;
#	define send_buf_size read_buf_size
	int is_stdin_open;
	int is_stdout_open;
	int is_sidin_open;
	int is_sidout_open;
	unsigned stdout_tm;
	unsigned sidout_tm;

	for (is_stdin_open = 1, is_stdout_open = 1, is_sidin_open = 1, is_sidout_open = 1, stdout_tm = sidout_tm = espress_stream_time() ;
		*is_running && (is_stdin_open || is_stdout_open || is_sidin_open || is_sidout_open) ;
		) {
		v = espress_stream_time();
		if (is_stdout_open && (0 < write_buf_size)) {
			u = (v - stdout_tm) & 0xffffffff;
			u = (u < TIMEOUT) ? (TIMEOUT - u) : TIMEOUT;
		} else {
			u = 0xffffffff;
		}
		if (is_sidout_open && (0 < send_buf_size)) {
			v = (v - sidout_tm) & 0xffffffff;
			v = (v < TIMEOUT) ? (TIMEOUT - v) : TIMEOUT;
		} else {
			v = 0xffffffff;
		}
		wait(attr,
			is_stdin_open && (read_buf_size == 0),
			is_stdout_open && (0 < write_buf_size),
			is_sidin_open && (recv_buf_size == 0),
			is_sidout_open && (0 < send_buf_size),
			*is_running ? ((u < v) ? u : v) : 0);

		if (is_stdin_open) {
			if (read_buf_size == 0) {
				st = read(STDIN_FILENO, read_buf, BUFFER_SIZE);
				if (0 < st) {
					read_buf_size = st;
				} else if (st == 0) {
					close(STDIN_FILENO);
					is_stdin_open = 0;
					if (send_buf_size == 0) {
						i = flush(attr, TIMEOUT);
						espress_stream_disconnect(*sid);
						espress_stream_disconnect_result(*sid, *is_running ? TIMEOUT : 0);
						is_sidout_open = 0;
						if (0 < i) {
							break;
						}
					}
				} else if (errno != EAGAIN) {
					break;
				}
			} else {
				st = read(STDIN_FILENO, NULL, 0);
				if ((st == -1) && (errno != EAGAIN)) {
					break;
				}
			}
		}

		if (is_sidin_open) {
			if (recv_buf_size == 0) {
				u = BUFFER_SIZE;
				i = espress_stream_recv(*sid, recv_buf, &u, 0);
				if (i == 0) {
					if (0 < u) {
						recv_buf_size = u;
					} else {
						is_sidin_open = 0;
						if (write_buf_size == 0) {
							close(STDOUT_FILENO);
							is_stdout_open = 0;
						}
						if (!arg_is_close_half && is_stdin_open)
						{
							close(STDIN_FILENO);
							is_stdin_open = 0;
							if (send_buf_size == 0) {
								i = flush(attr, TIMEOUT);
								espress_stream_disconnect(*sid);
								espress_stream_disconnect_result(*sid, *is_running ? TIMEOUT : 0);
								is_sidout_open = 0;
								if (0 < i) {
									break;
								}
							}
						}
					}
				} else if (0 < i) {
					break;
				}
			} else {
				u = 1;
				i = espress_stream_recv(*sid, NULL, &u, 0);
				if (0 < i) {
					break;
				}
			}
		}

		if (is_stdout_open) {
			if (0 < write_buf_size) {
				st = write(STDOUT_FILENO, write_buf, write_buf_size);
				if (0 < st) {
					write_buf += st;
					write_buf_size -= st;
					if (write_buf_size == 0) {
						write_buf = recv_buf;
						if (is_sidin_open) {
							//fsync(STDOUT_FILENO);
						} else {
							close(STDOUT_FILENO);
							is_stdout_open = 0;
						}
					}
					stdout_tm = espress_stream_time();
				} else if (errno != EAGAIN) {
					break;
				} else {
					if (TIMEOUT <= ((espress_stream_time() - stdout_tm) & 0xffffffff)) {
						break;
					}
				}
			} else {
				st = write(STDOUT_FILENO, NULL, 0);
				if ((st == -1) && (errno != EAGAIN)) {
					break;
				}
				stdout_tm = espress_stream_time();
			}
		}

		if (is_sidout_open) {
			if (0 < send_buf_size) {
				u = send_buf_size;
				i = espress_stream_send(*sid, send_buf, &u, 0);
				if (i == 0) {
					send_buf += u;
					send_buf_size -= u;
					if (send_buf_size == 0) {
						send_buf = read_buf;
						if (is_stdin_open) {
							espress_stream_flush(*sid, 0);
						} else {
							i = flush(attr, TIMEOUT);
							espress_stream_disconnect(*sid);
							espress_stream_disconnect_result(*sid, *is_running ? TIMEOUT : 0);
							is_sidout_open = 0;
							if (0 < i) {
								break;
							}
						}
					}
					sidout_tm = espress_stream_time();
				} else if (0 < i) {
					break;
				} else {
					if (TIMEOUT <= ((espress_stream_time() - sidout_tm) & 0xffffffff)) {
						break;
					}
				}
			} else {
				u = 1;
				i = espress_stream_send(*sid, NULL, &u, 0);
				if (0 < i) {
					break;
				}
				sidout_tm = espress_stream_time();
			}
		}
	}

	if (is_stdin_open) {
		close(STDIN_FILENO);
	}
	if (is_stdout_open) {
		close(STDOUT_FILENO);
	}
	if (is_sidin_open) {
		;
	}
	if (is_sidout_open) {
		espress_stream_disconnect(*sid);
		espress_stream_disconnect_result(*sid, *is_running ? TIMEOUT : 0);
	}
}

int es_ccat_main()
{
	int ret = 0;
	int i;
	struct sockaddr_storage address[1] = {0};
	struct sockaddr_storage src_address[1] = {0};
	struct session_attr *attr = session_attr;
	struct espress_stream_session_ex_param ex_param[1] = {{
		send_connect_payload, NULL,
		recv_connect_payload, NULL,
		NULL, NULL,
		NULL, NULL
	}};

	printf("destination address: %s\n", arg_address);
	printf("destination port: %s\n", arg_port);
	printf("destination public key: ");
	if (arg_public_key == NULL) {
		printf("(none)");
	} else {
		for (i = 0 ; i < CIPHER_ED25519_PUBLIC_KEY_SIZE ; i++) {
			printf("%02x", arg_public_key[i]);
		}
	}
	printf("\n");
	printf("close: %s\n", arg_is_close_half ? "half" : "full");

	ipaddr_aton(arg_address, address->address);
	address->port = atoi(arg_port);

	ipaddr_aton("0.0.0.0", src_address->address);
	src_address->port = 0;

	i = espress_stream_begin();
	if (i != 0) {
		ret = 1;
		fprintf(stderr, "error: can't begin stream.\n");
		goto BEGIN_STREAM_FAIL_EXIT;
	}

	i = espress_stream_begin_interface(&ifd, src_address);
	if (i != 0) {
		ret = 1;
		fprintf(stderr, "error: can't begin interface.\n");
		goto BEGIN_INTERFACE_FAIL_EXIT;
	}

	attr->is_running = 1;
	ex_param->send_connect_payload_param = attr;
	ex_param->recv_connect_payload_param = attr;
	i = espress_stream_begin_session(&(attr->sid), ifd, 0, ex_param);
	if (i != 0) {
		ret = 1;
		fprintf(stderr, "error: can't begin session.\n");
		goto BEGIN_SESSION_FAIL_EXIT;
	}

	i = conn(attr, address, attr->is_running ? TIMEOUT : 0);
	if (i != 0) {
		fprintf(stderr, "error: fail to connect.\n");
		ret = 1;
		goto CONN_FAIL_EXIT;
	}

	printf("CONNECT\n");
	fflush(stdout);

	cat(attr);

CONN_FAIL_EXIT:
	espress_stream_end_session(attr->sid);
	attr->sid = NULL;
BEGIN_SESSION_FAIL_EXIT:
BEGIN_INTERFACE_FAIL_EXIT:
	espress_stream_end();
BEGIN_STREAM_FAIL_EXIT:

	return ret;
}

int main()
{
	int ret = 0;
	unsigned u;
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
		printf("start\n\n");
		u = espress_stream_time();
		es_ccat_main();
		u = espress_stream_time() - u;
		printf("\nfinish (elapsed time = %u us)\n", u);
	}
	cyw43_arch_deinit();
EXIT:
	return ret;
}
