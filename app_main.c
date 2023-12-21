#include <stdio.h>

#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
#include "lwip/pbuf.h"
#include "lwip/udp.h"

#include "espress_stream/api.h"
#include "cipher_api.h"

#include "wifi_config.h"

#define ADDRESS "192.168.1.2"
#define PORT "1234"
#define PUBLIC_KEY { \
	0x73, 0x35, 0xe6, 0x3a, 0x76, 0x21, 0x23, 0xa9, 0x41, 0xa2, 0x5e, 0x5e, 0x53, 0x8d, 0xd4, 0xd2, \
	0xe4, 0x7d, 0x86, 0x92, 0x80, 0xf8, 0x3a, 0xf7, 0x81, 0x0e, 0x0b, 0x3d, 0xae, 0x80, 0x69, 0xc9 \
}
#define USER_ID "foo"
#define USER_PASSWORD "tpfBTht8"
#define FILE_NAME "test.dat"
#define FILE_SIZE (1024 * 1024)

#define IS_DEBUGGING (0)
#define TIMEOUT (30 * 1000 * 1000)
#define BUFFER_SIZE (512)
#define CONNECT_MESSAGE_SIZE (64)
#define USER_ID_SIZE (256)
#define USER_PASSWORD_SIZE (256)

static uint64_t be64toh(uint64_t be64)
{
	unsigned char *b = (unsigned char *)&be64;
	unsigned char h[8];
	h[0] = b[7];
	h[1] = b[6];
	h[2] = b[5];
	h[3] = b[4];
	h[4] = b[3];
	h[5] = b[2];
	h[6] = b[1];
	h[7] = b[0];
	return *(uint64_t *)h;
}

typedef struct {
	unsigned long size;
	unsigned long p;
} _FILE;

static _FILE *_fopen(char const *name, char const *mode)
{
	_FILE *f;
	(void)name;
	(void)mode;
	f = malloc(sizeof(_FILE));
	if (NULL != f) {
		f->size = FILE_SIZE;
		f->p = 0;
	}
	return f;
}

static int _fseek(_FILE *f, long offset, int whence)
{
	int ret = 0;
	unsigned w;
	switch (whence) {
	case SEEK_SET:
		w = 0;
		break;
	case SEEK_CUR:
		w = f->p;
		break;
	case SEEK_END:
		w = f->size;
		break;
	default:
		ret = 1;
		break;
	}
	if (ret == 0) {
		if (0 < offset) {
			if ((f->size - w) < offset) {
				f->p = f->size;
			} else {
				f->p = w + offset;
			}
		} else {
			if (w < -offset) {
				f->p = 0;
			} else {
				f->p = w + offset;
			}
		}
	}
	return ret;
}

static unsigned long _ftell(_FILE *f)
{
	return f->p;
}

static unsigned long _fread(void *buffer, unsigned long size, unsigned long nmemb, _FILE *f)
{
	unsigned char *p;
	unsigned char *pe;
	for (p = (unsigned char *)buffer, pe = p + size * nmemb ; (p < pe) && (f->p < f->size) ; p++, (f->p)++) {
		*p = (unsigned char)(f->p);
	}
	return p - (unsigned char *)buffer;
}

static void _fclose(_FILE *f)
{
	free(f);
}

#define FILE _FILE
#define fopen _fopen
#define fseek _fseek
#define ftell _ftell
#define fread _fread
#define fclose _fclose

static void *ifd;
static const char *arg_address = ADDRESS;
static const char *arg_port = PORT;
static const char *arg_file_name = FILE_NAME;
static unsigned char arg_public_key[] = PUBLIC_KEY;
static char const *arg_user_id = USER_ID;
static char const *arg_user_password = USER_PASSWORD;

struct file_tx_attr {
	unsigned index;
	int is_attached;
	pthread_t thread;
	void *sid;
	int is_running;
	struct sockaddr_storage const *address;
	char const *file_name;
	int ret;
	unsigned char message[CONNECT_MESSAGE_SIZE];
	unsigned message_size;
};

#if IS_DEBUGGING
	static void disp(unsigned index, char const *main_file_name, uint64_t file_size, uint64_t progress, int fin_state,
		unsigned bufdsz, unsigned rtt, unsigned rto, unsigned dac, unsigned cwnd, unsigned send_timeout_count, unsigned fast_retransmit_count)
	{
		unsigned u;
		//static pthread_mutex_t disp_mutex[1] = {PTHREAD_ADAPTIVE_MUTEX_INITIALIZER_NP};
#		define FILE_NAME_DISP_WIDTH (24)

		//pthread_mutex_lock(disp_mutex);

		for (u = 0 ; u < index ; u++) {
			putchar('\n');
			putchar('\n');
		}

		if (main_file_name == NULL) {
			printf("------------------------\r");
		}
		else if (FILE_NAME_DISP_WIDTH < strlen(main_file_name)) {
			printf("%.*s...\r", FILE_NAME_DISP_WIDTH - 3, main_file_name);
		} else {
			printf("%-*s\r", FILE_NAME_DISP_WIDTH, main_file_name);
		}

		printf("\x1b[24C");
		if (fin_state == -1) {
			printf("[XXXXXXXXXXXXXXXX(XXXXXXXX)/%016llu]\n", file_size);
		} else if (fin_state == 1) {
			printf("[OOOOOOOOOOOOOOOO(OOOOOOOO)/%016llu]\n", file_size);
		} else {
			printf("[%016llu(%08u)/%016llu]\n", progress, bufdsz, file_size);
		}
		if (fin_state == 0) {
			printf("rtt=%08u, ", rtt);
			printf("rto=%08u, ", rto);
			printf("dac=%04u, ", dac);
			printf("cwnd=%04u, ", cwnd);
			printf("stoc=%08u, ", send_timeout_count);
			printf("frxc=%08u\r\x1b[A", fast_retransmit_count);
		} else {
			printf("\x1b[A");
		}

		if (0 < index) {
			printf("\x1b[%uA", index * 2);
		}

		//fflush(stdout);

		//pthread_mutex_unlock(disp_mutex);
	}

#	define disp_fin(count) do{unsigned u; unsigned v = (count); for (u = 0 ; u < v ; u++) {putchar('\n'); putchar('\n');}}while (0)

#	define disp(index, main_file_name, file_size, progress, fin_state, bufdsz, rtt, rto, dac, cwnd, send_timeout_count, fast_retransmit_count) \
		disp(index, main_file_name, file_size, progress, fin_state, bufdsz, rtt, rto, dac, cwnd, send_timeout_count, fast_retransmit_count)
#else
	static void disp(unsigned index, char const *main_file_name, uint64_t file_size, uint64_t progress, int fin_state)
	{
		unsigned u;
		unsigned ratio;
		//static pthread_mutex_t disp_mutex[1] = {PTHREAD_ADAPTIVE_MUTEX_INITIALIZER_NP};
#		define FILE_NAME_DISP_WIDTH (24)
#		define disp_num(n) do               \
		{                                   \
			unsigned num;                   \
			char *au;                       \
			if (10000000000000 < (n)) {     \
				num = (n) / 1000000000000;  \
				au = "T";                   \
			} else if (10000000000 < (n)) { \
				num = (n) / 1000000000;     \
				au = "G";                   \
			} else if (10000000 < (n)) {    \
				num = (n) / 1000000;        \
				au = "M";                   \
			} else if (10000 < (n)) {       \
				num = (n) / 1000;           \
				au = "K";                   \
			} else {                        \
				num = (n);                  \
				au = "";                    \
			}                               \
			printf("%u%sB", num, au);       \
		} while(0)

		if (file_size == 0) {
			ratio = 0;
		} else {
			ratio = progress * 32 / file_size;
		}

		//pthread_mutex_lock(disp_mutex);

		for (u = 0 ; u < index ; u++) {
			putchar('\n');
		}

		if (main_file_name == NULL) {
			printf("------------------------\r");
		}
		else if (FILE_NAME_DISP_WIDTH < strlen(main_file_name)) {
			printf("%.*s...\r", FILE_NAME_DISP_WIDTH - 3, main_file_name);
		} else {
			printf("%-*s\r", FILE_NAME_DISP_WIDTH, main_file_name);
		}

		printf("\x1b[24C");
		if (file_size == 0) {
			if (fin_state == -1) {
				printf("[XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX] -/-");
			} else if (fin_state == 1) {
				printf("[OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO] -/-");
			} else {
				printf("[--------------------------------] -/-");
			}
		} else {
			if (fin_state == -1) {
				printf("[XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX] -/");
				disp_num(file_size);
			} else if (fin_state == 1) {
				printf("[OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO] ");
				disp_num(progress);
				putchar('/');
				disp_num(file_size);
			} else {
				printf("[%.*s%.*s] ",
					ratio, "================================",
					32 - ratio, "--------------------------------");
				disp_num(progress);
				putchar('/');
				disp_num(file_size);
			}
		}
		printf("\x1b[K\r");

		if (0 < index) {
			printf("\x1b[%uA", index);
		}

		//fflush(stdout);

		//pthread_mutex_unlock(disp_mutex);
	}

#	define disp_fin(count) do{unsigned u; unsigned v = (count); for (u = 0 ; u < v ; u++) {putchar('\n');}}while (0)

#	define disp(index, main_file_name, file_size, progress, fin_state, bufdsz, rtt, rto, dac, cwnd, send_timeout_count, fast_retransmit_count) \
		disp(index, main_file_name, file_size, progress, fin_state)
#endif

static unsigned getrandom(unsigned char *b, unsigned s, unsigned f)
{
	unsigned ret = 0;
	static unsigned char p = 0x55;
	uint32_t t;
	(void)f;
	if (0 < s) {
		p = p << 1 | p >> 7;
		t = time_us_32();
		*b = p;
		*b = *b ^ ((unsigned char *)&t)[0];
		*b = *b << 1 | *b >> 7;
		*b = *b ^ ((unsigned char *)&t)[1];
		*b = *b << 1 | *b >> 7;
		*b = *b ^ ((unsigned char *)&t)[2];
		*b = *b << 1 | *b >> 7;
		*b = *b ^ ((unsigned char *)&t)[3];
		*b = *b << 1 | *b >> 7;
		ret = 1;
	}
	return ret;
}

static int send_connect_payload(void *sid, int (*callback)(void *sid, void const *payload, unsigned payload_size, int is_rejected), void *param)
{
	int ret;
	unsigned char *message = ((struct file_tx_attr *)param)->message;
	unsigned *message_size = &(((struct file_tx_attr *)param)->message_size);
#	define get_random(buf, buf_sz) do \
	{                                 \
		unsigned char *b = (buf);     \
		unsigned u = (buf_sz);        \
		unsigned s;                   \
		while (0 < u) {               \
			s = getrandom(b, u, 0);   \
			if (0 < s) {              \
				b += s;               \
				u -= s;               \
			}                         \
		}                             \
	} while(0)
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
	unsigned char *message = ((struct file_tx_attr *)param)->message;
	unsigned *message_size = &(((struct file_tx_attr *)param)->message_size);
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

static int conn(struct file_tx_attr *attr,
	struct sockaddr_storage const *address,
	unsigned timeout, int *is_running)
{
	int i;
	unsigned t;
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

			if (timeout <= (t = (espress_stream_time() - start_time) & 0xffffffff)) {
				break;
			}
		}
	}

	return i;
}

static int send_recv(struct file_tx_attr *attr,
	void const *send_data, unsigned *send_data_size,
	void *recv_data, unsigned *recv_data_size,
	unsigned timeout, int *is_running)
{
	int i;
	int j;
	unsigned t;
	void *sid = attr->sid;
	unsigned const start_time = espress_stream_time();

	for (i = -1, j = -1, t = timeout ; *is_running && (i == -1) && (j == -1) ; ) {
		t = timeout - t;
		espress_stream_wait_any(sid, t);
		i = espress_stream_send(sid, send_data, send_data_size, 0);
		j = espress_stream_recv(sid, recv_data, recv_data_size, 0);

		if (timeout <= (t = (espress_stream_time() - start_time) & 0xffffffff)) {
			break;
		}
	}

	i = (i < 0) ? -1 : ((0 < i) ? 1 : 0);
	j = (j < 0) ? -1 : ((0 < j) ? 1 : 0);
	return (i & 0x03) | ((j & 0x03) << 2);
}

static int flush_recv(struct file_tx_attr *attr,
	void *recv_data, unsigned *recv_data_size,
	unsigned timeout, int *is_running)
{
	int i;
	int j;
	unsigned t;
	void *sid = attr->sid;
	unsigned const start_time = espress_stream_time();

	for (i = -1, j = -1, t = timeout ; *is_running && (i == -1) && (j == -1) ; ) {
		t = timeout - t;
		espress_stream_wait_any(sid, t);
		i = espress_stream_flush(sid, 0);
		j = espress_stream_recv(sid, recv_data, recv_data_size, 0);

		if (timeout <= (t = (espress_stream_time() - start_time) & 0xffffffff)) {
			break;
		}
	}

	i = (i < 0) ? -1 : ((0 < i) ? 1 : 0);
	j = (j < 0) ? -1 : ((0 < j) ? 1 : 0);
	return (i & 0x03) | ((j & 0x03) << 2);
}

static void *file_tx(void *param)
{
	int i;
	int j;
	size_t s;
	unsigned t;
	unsigned u;
	unsigned v;
	unsigned w;
	unsigned char b;
	void *ret = param;
	struct sockaddr_storage const *address = ((struct file_tx_attr *)param)->address;
	unsigned index = ((struct file_tx_attr *)param)->index;
	int *is_running = &(((struct file_tx_attr *)param)->is_running);
	char const *file_name = ((struct file_tx_attr *)param)->file_name;
	char const *main_file_name;
	FILE *file;
	uint64_t file_size = 0;
	void **sid = &(((struct file_tx_attr *)param)->sid);
	unsigned char buf[BUFFER_SIZE];
	uint64_t progress;
	unsigned start_time;

	main_file_name = strrchr(file_name, '/');
	if (main_file_name == NULL) {
		main_file_name = file_name;
	} else {
		if (*(++main_file_name) == '\0') {
			main_file_name = NULL;
			ret = NULL;
			goto FILE_NAME_INVALID_EXIT;
		}
	}

	file = fopen(file_name, "rb");
	if (file == NULL) {
		ret = NULL;
		goto FILE_OPEN_FAILURE_EXIT;
	}

	disp(index, main_file_name, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);

	fseek(file, 0, SEEK_END);
	file_size = ftell(file);
	fseek(file, 0, SEEK_SET);

	i = conn((struct file_tx_attr *)param, address, *is_running ? TIMEOUT : 0, is_running);
	if (i != 0) {
		ret = NULL;
		goto CONNECT_FAILURE_EXIT;
	}

	strcpy((char *)buf, arg_user_id);
	strcat((char *)buf, ":");
	strcat((char *)buf, arg_user_password);
	for (u = 0, s = strlen((char *)buf) + 1 ; u < s ; u += v) {
		v = s - u;
		w = 1;
		i = send_recv((struct file_tx_attr *)param, buf + u, &v, &b, &w, *is_running ? TIMEOUT : 0, is_running);
		j = (i >> 2) & 0x03; if (1 < j) j = -1;
		i = i & 0x03; if (1 < i) i = -1;
		if (i != 0) {
			i = 1;
			break;
		}
		if ((j == 0) && (w == 0)) {
			i = 1;
			break;
		}
		i = 0;
	}
	if (i != 0) {
		ret = NULL;
		goto USER_ID_PASS_SEND_FAILURE_EXIT;
	}
	i = espress_stream_flush(*sid, *is_running ? TIMEOUT : 0);
	if (i != 0) {
		ret = NULL;
		goto USER_ID_PASS_SEND_FAILURE_EXIT;
	}

	for (u = 0, s = strlen(main_file_name) + 1 ; u < s ; u += v) {
		v = s - u;
		w = 1;
		i = send_recv((struct file_tx_attr *)param, main_file_name + u, &v, &b, &w, *is_running ? TIMEOUT : 0, is_running);
		j = (i >> 2) & 0x03; if (1 < j) j = -1;
		i = i & 0x03; if (1 < i) i = -1;
		if (i != 0) {
			i = 1;
			break;
		}
		if ((j == 0) && (w == 0)) {
			i = 1;
			break;
		}
		i = 0;
	}
	if (i != 0) {
		ret = NULL;
		goto NAME_SEND_FAILURE_EXIT;
	}
	i = espress_stream_flush(*sid, *is_running ? TIMEOUT : 0);
	if (i != 0) {
		ret = NULL;
		goto NAME_SEND_FAILURE_EXIT;
	}

	for (progress = 0 ; *is_running ; ) {
		s = fread(buf, 1, sizeof(buf), file);
		for (u = 0 ; u < s ; u += v) {
			disp(index, main_file_name, file_size, (progress + u) - espress_stream_buffered_send_size(*sid, NULL), i ? -1 : 0,
				espress_stream_buffered_send_size(*sid, NULL),
				espress_stream_rtt(*sid),
				espress_stream_rto(*sid),
				espress_stream_dac(*sid),
				espress_stream_cwnd(*sid),
				espress_stream_send_timeout_count(*sid),
				espress_stream_fast_retransmit_count(*sid));
			v = s - u;
			w = 1;
			i = send_recv((struct file_tx_attr *)param, buf + u, &v, &b, &w, *is_running ? TIMEOUT : 0, is_running);
			j = (i >> 2) & 0x03; if (1 < j) j = -1;
			i = i & 0x03; if (1 < i) i = -1;
			if (i != 0) {
				i = 1;
				break;
			}
			if ((j == 0) && (w == 0)) {
				i = 1;
				break;
			}
		}
		disp(index, main_file_name, file_size, (progress += u) - espress_stream_buffered_send_size(*sid, NULL), i ? -1 : 0,
			espress_stream_buffered_send_size(*sid, NULL),
			espress_stream_rtt(*sid),
			espress_stream_rto(*sid),
			espress_stream_dac(*sid),
			espress_stream_cwnd(*sid),
			espress_stream_send_timeout_count(*sid),
			espress_stream_fast_retransmit_count(*sid));
		if (i != 0) {
			break;
		}
		if (s < sizeof(buf)) {
			break;
		}
	}
	if (!*is_running || (i != 0)) {
		ret = NULL;
		goto DATA_SEND_BREAK_EXIT;
	}

	for (u = espress_stream_buffered_send_size(*sid, NULL), start_time = espress_stream_time(), t = 0 ;
		*is_running ;
		u = v) {
		disp(index, main_file_name, file_size, progress - espress_stream_buffered_send_size(*sid, NULL), 0,
			espress_stream_buffered_send_size(*sid, NULL),
			espress_stream_rtt(*sid),
			espress_stream_rto(*sid),
			espress_stream_dac(*sid),
			espress_stream_cwnd(*sid),
			espress_stream_send_timeout_count(*sid),
			espress_stream_fast_retransmit_count(*sid));
		w = 1;
		i = flush_recv((struct file_tx_attr *)param, &b, &w, 0, is_running);
		j = (i >> 2) & 0x03; if (1 < j) j = -1;
		i = i & 0x03; if (1 < i) i = -1;
		if ((0 <= i) || ((j == 0) && (w == 0))) {
			break;
		}
		v = espress_stream_buffered_send_size(*sid, NULL);
		if (u != v) {
			start_time = espress_stream_time();
		}

		if ((t = (espress_stream_time() - start_time) & 0xffffffff) < TIMEOUT) {
			t = TIMEOUT - t;
			espress_stream_wait_any(*sid, *is_running ? t : 0);
		} else {
			break;
		}
	}
	if (!*is_running || (i != 0) || ((j == 0) && (w == 0))) {
		ret = NULL;
		goto DATA_SEND_TERMINATE_FAILURE_EXIT;
	}

DATA_SEND_TERMINATE_FAILURE_EXIT:
DATA_SEND_BREAK_EXIT:
NAME_SEND_FAILURE_EXIT:
USER_ID_PASS_SEND_FAILURE_EXIT:
	espress_stream_disconnect(*sid);
	espress_stream_disconnect_result(*sid, *is_running ? TIMEOUT : 0);
	if (ret != NULL) {
		for (u = 0 ; u < sizeof(uint64_t) ; u += v) {
			v = sizeof(uint64_t) - u;
			i = espress_stream_recv(*sid, buf + u, &v, *is_running ? TIMEOUT : 0);
			if (i != 0) {
				break;
			}
			if (v == 0) {
				i = 1;
				break;
			}
		}
		if (!*is_running || (i != 0) || (file_size != be64toh(*((uint64_t *)buf)))) {
			ret = NULL;
		} else {
			w = 1;
			espress_stream_recv(*sid, &b, &w, *is_running ? TIMEOUT : 0);
		}
	}
CONNECT_FAILURE_EXIT:
	fclose(file);
FILE_OPEN_FAILURE_EXIT:
FILE_NAME_INVALID_EXIT:
	if (ret == NULL) {
		disp(index, main_file_name, file_size, 0, -1, 0, 0, 0, 0, 0, 0, 0);
	} else {
		disp(index, main_file_name, file_size, file_size, 1, 0, 0, 0, 0, 0, 0, 0);
	}

	*is_running = 0;
	return ret;
}

int app_main()
{
	int ret = 0;
	int i;
	void *p;
	struct sockaddr_storage address[1] = {0};
	struct sockaddr_storage src_address[1] = {0};
	struct file_tx_attr file_tx_attr[1];
	int is_stream_ng = 1;
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
	printf("user id: %s\n", arg_user_id);
	printf("user password:\n");

	ipaddr_aton(arg_address, address->address);
	address->port = atoi(arg_port);

	is_stream_ng = (espress_stream_begin() != 0);
	if (is_stream_ng) {
		ret = 1;
		printf("error: can't begin stream.\n");
		goto EXIT;
	}

	ipaddr_aton("0.0.0.0", src_address->address);
	src_address->port = 0;
	i = espress_stream_begin_interface(&ifd, src_address);
	if (i != 0) {
		ifd = NULL;
		ret = 1;
		fprintf(stderr, "error: can't begin interface.\n");
		goto EXIT;
	}

	file_tx_attr->index = 0;
	file_tx_attr->is_attached = 1;
	file_tx_attr->is_running = 1;
	file_tx_attr->address = address;
	file_tx_attr->file_name = arg_file_name;
	file_tx_attr->ret = 0;
	ex_param->send_connect_payload_param = file_tx_attr;
	ex_param->recv_connect_payload_param = file_tx_attr;
	i = espress_stream_begin_session(&(file_tx_attr->sid), ifd, 0, ex_param);
	if (i != 0) {
		file_tx_attr->is_attached = 0;
		file_tx_attr->is_running = 0;
	} else {
		p = file_tx(file_tx_attr);
	}

	if (file_tx_attr->is_attached) {
		if (p == NULL) {
			file_tx_attr->ret = 1;
		}
		file_tx_attr->is_attached = 0;
		espress_stream_end_session(file_tx_attr->sid);
	} else {
		file_tx_attr->ret = 1;
	}

	disp_fin(1);

	if (file_tx_attr->ret) {
		ret = 1;
		fprintf(stderr, "error: file \"%s\" transmit failure.\n", file_tx_attr->file_name);
	}

EXIT:

	if (ifd != NULL) espress_stream_end_interface(ifd);
	if (!is_stream_ng) espress_stream_end();

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
	if (cyw43_arch_wifi_connect_timeout_ms(WIFI_SSID, WIFI_PASSWORD, CYW43_AUTH_WPA2_AES_PSK, 30000))
	{
		printf("Wi-fi connection failure.\n");
	} else {
		printf("Wi-fi connection success (%s).\n", ip4addr_ntoa(netif_ip4_addr(netif_list)));
		printf("start\n\n");
		u = espress_stream_time();
		app_main();
		u = espress_stream_time() - u;
		printf("\nfinish (elapsed time = %u us)\n", u);
	}
	cyw43_arch_deinit();
EXIT:
	return ret;
}