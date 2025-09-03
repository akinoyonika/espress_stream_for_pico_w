#include "espress_stream/config.h"
#include "espress_stream/depend.h"
#include "espress_stream/uint.h"
#include "espress_stream/tick.h"
#include "espress_stream/mutex.h"
#include "espress_stream/cond.h"
#include "espress_stream/event.h"
#include "espress_stream/timer.h"
#include "espress_stream/hash.h"
#include "espress_stream/mac.h"
#include "espress_stream/rand.h"
#include "espress_stream/block_cipher.h"
#include "espress_stream/ctr_cipher.h"
#include "espress_stream/hash.h"
#include "espress_stream/mac.h"
#include "espress_stream/hkdf.h"
#include "espress_stream/ec.h"
#include "espress_stream/key_ex.h"
#include "espress_stream/key_exp.h"
#include "espress_stream/key.h"
#include "espress_stream/ring.h"
#include "espress_stream/dgram.h"
#include "espress_stream/trans_ring.h"
#include "espress_stream/packet.h"
#include "espress_stream/trans_packet.h"
#include "espress_stream/ring_buffer.h"
#include "espress_stream/session.h"
#include "espress_stream/api.h"

#define uint32_get_u(n, u) do {                         \
	b_t c0;                                             \
	b_t c1;                                             \
	b_t c2;                                             \
	b_t c3;                                             \
	espress_stream_uint32_get((n), &c3, &c2, &c1, &c0); \
	*(u) =                                              \
		((unsigned)c3 << 24) |                          \
		((unsigned)c2 << 16) |                          \
		((unsigned)c1 <<  8) |                          \
		((unsigned)c0 <<  0);                           \
} while (0)

#define uint32_set_u(n, u) do {                     \
	b_t c0;                                         \
	b_t c1;                                         \
	b_t c2;                                         \
	b_t c3;                                         \
	c3 = (b_t)((u) >> 24);                          \
	c2 = (b_t)((u) >> 16);                          \
	c1 = (b_t)((u) >>  8);                          \
	c0 = (b_t)((u) >>  0);                          \
	espress_stream_uint32_set((n), c3, c2, c1, c0); \
} while (0)

struct espress_stream_uint32 /*const*/ espress_stream_connect_timeout_time[1] = {ESPRESS_STREAM_CONNECT_TIMEOUT_TIME};

enum {
	READY = 0,
	SET,
	GO
};

enum {
	INIT = 0,
	CONNECTING,
	CONNECTED,
	DISCONNECTING,
	DISCONNECTED
};

#if ESPRESS_STREAM_API_IS_THREAD_ENABLED
	struct thread_info {
		struct thread thread[1];
		void *routine;
		void *belong;
	};
#endif

struct session {
	int state;
	int src_state;
	int dst_state;
	void *ifd;
	struct espress_stream_session_ex_param ex_param[1];
	i_t (*send_connect_payload_callback)(struct espress_stream_session *session, b_t const *payload, u_t payload_size, i_t is_rejected);
	i_t (*recv_connect_payload_callback)(struct espress_stream_session *session, b_t *payload, u_t *payload_size);
	i_t (*send_disconnect_payload_callback)(struct espress_stream_session *session, b_t const *payload, u_t payload_size);
	i_t (*recv_disconnect_payload_callback)(struct espress_stream_session *session, b_t *payload, u_t *payload_size);
	int is_server;
	struct espress_stream_mutex mutex[1];
	struct espress_stream_event event[1];
	struct espress_stream_event_flags flags[1];
	struct espress_stream_ring_buffer send_ring_buffer[1];
	struct espress_stream_ring_buffer recv_ring_buffer[1];
	b_t send_buffer[ESPRESS_STREAM_SEND_BUFFER_SIZE];
	b_t recv_buffer[ESPRESS_STREAM_RECV_BUFFER_SIZE];
	struct espress_stream_session session[1];
	struct sockaddr_storage address[1];
	struct espress_stream_uint32 start_time[1];
	int is_in_progress;
	volatile int is_broken;
#	if ESPRESS_STREAM_API_IS_THREAD_ENABLED
		struct thread_info yield_thread[ESPRESS_STREAM_SESSION_NUM_YIELD];
#	endif
};

struct interface {
	int state;
	struct espress_stream_trans_packet_interface interface[1];
#	if 0 < ESPRESS_STREAM_MAX_NUM_SESSION
		struct session session[ESPRESS_STREAM_MAX_NUM_SESSION];
#	else
		struct session_list {
			struct session session[1];
			struct session_list *next;
		} *session_list;
#	endif
#	if ESPRESS_STREAM_API_IS_THREAD_ENABLED
		struct thread_info yield_thread[ESPRESS_STREAM_TRANS_PACKET_NUM_YIELD];
#	endif
};

static struct espress_stream_mutex local_mutex[1];

#if 0 < ESPRESS_STREAM_MAX_NUM_INTERFACE
	static struct interface interface[ESPRESS_STREAM_MAX_NUM_INTERFACE];

	static void begin_interface()
	{
		memset(interface, 0, sizeof(interface));
	}

	static void end_interface()
	{
		memset(interface, 0, sizeof(interface));
	}

	static void *new_interface()
	{
		void *ret = NULL;
		int i;
		for (i = 0 ; i < ESPRESS_STREAM_MAX_NUM_INTERFACE ; i++) {
			if (interface[i].state == READY) {
				break;
			}
		}
		if (i < ESPRESS_STREAM_MAX_NUM_INTERFACE) {
			interface[i].state = SET;
			ret = (void *)(interface + i);
		}
		return ret;
	}

	static void delete_interface(void *ifd)
	{
		memset(ifd, 0, sizeof(struct interface));
	}

	static struct interface *get_interface(void *ifd)
	{
		return (struct interface *)ifd;
	}

#	if !ESPRESS_STREAM_API_IS_THREAD_ENABLED
		static void *get_first_interface()
		{
			return (void *)interface;
		}

		static void *get_next_interface(void *ifd)
		{
			void *ret = NULL;
			struct interface *i = (struct interface *)ifd;
			if ((i - get_interface(get_first_interface())) < (ESPRESS_STREAM_MAX_NUM_INTERFACE - 1)) {
				ret = (void *)(i + 1);
			}
			return ret;
		}
#	endif
#else
	struct interface_list {
		struct interface interface[1];
		struct interface_list *next;
	} static *interface_list;

	static void begin_interface()
	{
		interface_list = NULL;
	}

	static void end_interface()
	{
		struct interface_list *p;
		struct interface_list *n;
		for (p = NULL, n = interface_list ; n != NULL ; p = n, n = n->next) {
			if (p != NULL) {
				memset(p, 0, sizeof(*p));
				free(p);
			}
		}
		if (p != NULL) {
			memset(p, 0, sizeof(*p));
			free(p);
		}
	}

	static void *new_interface()
	{
		void *ret = NULL;
		struct interface_list *p;
		struct interface_list *n;
		n = (struct interface_list *)malloc(sizeof(struct interface_list));
		if (n != NULL) {
			for (p = interface_list ; p != NULL ; p = p->next) {
				if (p->next == NULL) {
					p->next = n;
					break;
				}
			}
			if (p == NULL) {
				interface_list = n;
			}
			n->next = NULL;
			n->interface->state = SET;
			ret = (void *)(n->interface);
		}
		return ret;
	}

	static void delete_interface(void *ifd)
	{
		struct interface_list *p;
		struct interface_list *n;
		for (p = NULL, n = interface_list ; n != NULL ; p = n, n = n->next) {
			if ((void *)(n->interface) == ifd) {
				break;
			}
		}
		if (n != NULL) {
			if (p == NULL) {
				interface_list = n->next;
			} else {
				p->next = n->next;
			}
			memset(n, 0, sizeof(*n));
			free(n);
		}
	}

	static struct interface *get_interface(void *ifd)
	{
		return (struct interface *)ifd;
	}

#	if !ESPRESS_STREAM_API_IS_THREAD_ENABLED
		static struct interface *get_first_interface()
		{
			return interface_list->interface;
		}

		static struct interface *get_next_interface(void *ifd)
		{
			struct interface *ret = NULL;
			struct interface_list *n = (struct interface_list *)(ifd - offsetof(struct interface_list, interface));
			if (n->next != NULL) {
				ret = n->next->interface;
			}
			return ret;
		}
#	endif
#endif

#if 0 < ESPRESS_STREAM_MAX_NUM_SESSION
	static void begin_session(void *ifd)
	{
		memset(get_interface(ifd)->session, 0, sizeof(get_interface(ifd)->session));
	}

	static void end_session(void *ifd)
	{
		memset(get_interface(ifd)->session, 0, sizeof(get_interface(ifd)->session));
	}

	static void *new_session(void *ifd)
	{
		void *ret = NULL;
		int i;
		for (i = 0 ; i < ESPRESS_STREAM_MAX_NUM_SESSION ; i++) {
			if (get_interface(ifd)->session[i].state == READY) {
				break;
			}
		}
		if (i < ESPRESS_STREAM_MAX_NUM_SESSION) {
			get_interface(ifd)->session[i].state = SET;
			get_interface(ifd)->session[i].ifd = ifd;
			ret = (void *)(get_interface(ifd)->session + i);
		}
		return ret;
	}

	static void delete_session(void *sid)
	{
		memset(sid, 0, sizeof(struct session));
	}

	static struct session *get_session(void *sid)
	{
		return (struct session *)sid;
	}

#	if !ESPRESS_STREAM_API_IS_THREAD_ENABLED
		static void *get_first_session(void *ifd)
		{
			return (void *)(get_interface(ifd)->session);
		}

		static void *get_next_session(void *ifd, void *sid)
		{
			void *ret = NULL;
			struct session *s = (struct session *)sid;
			if ((s - get_session(get_first_session(ifd))) < (ESPRESS_STREAM_MAX_NUM_SESSION - 1)) {
				ret = (void *)(s + 1);
			}
			return ret;
		}
#	endif

	static void *get_sid(struct session *session)
	{
		return (void *)session;
	}
#else
	static void begin_session(void *ifd)
	{
		get_interface(ifd)->session_list = NULL;
	}

	static void end_session(void *ifd)
	{
		struct session_list *p;
		struct session_list *n;
		for (p = NULL, n = get_interface(ifd)->session_list ; n != NULL ; p = n, n = n->next) {
			if (p != NULL) {
				memset(p, 0, sizeof(*p));
				free(p);
			}
		}
		if (p != NULL) {
			memset(p, 0, sizeof(*p));
			free(p);
		}
	}

	static void *new_session(void *ifd)
	{
		void *ret = NULL;
		struct session_list *p;
		struct session_list *n;
		struct session_list **session_list = &(get_interface(ifd)->session_list);
		n = (struct session_list *)malloc(sizeof(struct session_list));
		if (n != NULL) {
			for (p = *session_list ; p != NULL ; p = p->next) {
				if (p->next == NULL) {
					p->next = n;
					break;
				}
			}
			if (p == NULL) {
				*session_list = n;
			}
			n->next = NULL;
			n->session->state = SET;
			n->session->ifd = ifd;
			ret = (void *)(n->session);
		}
		return ret;
	}

	static void delete_session(void *sid)
	{
		struct session_list *p;
		struct session_list *n;
		struct session_list **session_list = &(get_interface(((struct session *)sid)->ifd)->session_list);
		for (p = NULL, n = *session_list ; n != NULL ; p = n, n = n->next) {
			if ((void *)(n->session) == sid) {
				break;
			}
		}
		if (n != NULL) {
			if (p == NULL) {
				*session_list = n->next;
			} else {
				p->next = n->next;
			}
			memset(n, 0, sizeof(*n));
			free(n);
		}
	}

	static struct session *get_session(void *sid)
	{
		return (struct session *)sid;
	}

#	if !ESPRESS_STREAM_API_IS_THREAD_ENABLED
		static void *get_first_session(void *ifd)
		{
			return (void *)(get_interface(ifd)->session_list->session);
		}

		static void *get_next_session(void *ifd, void *sid)
		{
			struct session *ret = NULL;
			struct session_list *n = (struct session_list *)(sid - offsetof(struct session_list, session));
			(void)ifd;
			if (n->next != NULL) {
				ret = n->next->session;
			}
			return ret;
		}
#	endif

	static void *get_sid(struct session *session)
	{
		return (void *)session;
	}
#endif

#if ESPRESS_STREAM_API_IS_THREAD_ENABLED
#	define event_wait(event, flags, timeout) espress_stream_event->wait(event, flags, timeout)
#	define event_poll(event, num, timeout) espress_stream_event->poll(event, num, timeout)

	static void *interface_yield(void *param)
	{
		struct thread_info *info = (struct thread_info *)param;
		void (*yield)(struct espress_stream_trans_packet_interface *this, struct espress_stream_uint32 const *timeout) =
			(void (*)(struct espress_stream_trans_packet_interface *this, struct espress_stream_uint32 const *timeout))(info->routine);
		struct interface *interface = (struct interface *)(info->belong);
		while (interface->state == GO) {
			yield(interface->interface, espress_stream_uint32_max);
		}
		return param;
	}

	static int create_interface_yield_thread(struct interface *this)
	{
		int ret = 0;
		int i;
		if (this->state != GO) {
			ret = 1;
			goto EXIT;
		}
		for (i = 0 ; i < ESPRESS_STREAM_TRANS_PACKET_NUM_YIELD ; i++) {
			this->yield_thread[i].routine = (void *)espress_stream_trans_packet->yield[i];
			this->yield_thread[i].belong = (void *)this;
			if (0 != thread_create(this->yield_thread[i].thread, interface_yield, this->yield_thread + i)) {
				break;
			}
		}
		if (i < ESPRESS_STREAM_TRANS_PACKET_NUM_YIELD) {
			this->state = SET;
			while (0 <= --i) {
				espress_stream_trans_packet->yield_break[i](this->interface);
				thread_join(this->yield_thread[i].thread);
			}
			ret = 1;
			goto EXIT;
		}
	EXIT:
		return ret;
	}

	static void destroy_interface_yield_thread(struct interface *this)
	{
		int i;
		if (this->state == GO) {
			goto EXIT;
		}
		for (i = 0 ; i < ESPRESS_STREAM_TRANS_PACKET_NUM_YIELD ; i++) {
			espress_stream_trans_packet->yield_break[i](this->interface);
			thread_join(this->yield_thread[i].thread);
		}
	EXIT:
		return;
	}

	static void *session_yield(void *param)
	{
		struct thread_info *info = (struct thread_info *)param;
		void (*yield)(struct espress_stream_session *this, struct espress_stream_uint32 const *timeout) =
			(void (*)(struct espress_stream_session *this, struct espress_stream_uint32 const *timeout))(info->routine);
		struct session *session = (struct session *)(info->belong);
		while (session->state == GO) {
			yield(session->session, espress_stream_uint32_max);
		}
		return param;
	}

	static int create_session_yield_thread(struct session *this)
	{
		int ret = 0;
		int i;
		if (this->state != GO) {
			ret = 1;
			goto EXIT;
		}
		for (i = 0 ; i < ESPRESS_STREAM_SESSION_NUM_YIELD ; i++) {
			this->yield_thread[i].routine = (void *)espress_stream_session->yield[i];
			this->yield_thread[i].belong = (void *)this;
			if (0 != thread_create(this->yield_thread[i].thread, session_yield, this->yield_thread + i)) {
				break;
			}
		}
		if (i < ESPRESS_STREAM_SESSION_NUM_YIELD) {
			this->state = SET;
			while (0 <= --i) {
				espress_stream_session->yield_break[i](this->session);
				thread_join(this->yield_thread[i].thread);
			}
			ret = 1;
			goto EXIT;
		}
	EXIT:
		return ret;
	}

	static void destroy_session_yield_thread(struct session *this)
	{
		int i;
		if (this->state == GO) {
			goto EXIT;
		}
		for (i = 0 ; i < ESPRESS_STREAM_SESSION_NUM_YIELD ; i++) {
			espress_stream_session->yield_break[i](this->session);
			thread_join(this->yield_thread[i].thread);
		}
	EXIT:
		return;
	}
#else
	static void interface_yield()
	{
		void *i;
		int j;
		for (i = get_first_interface() ; i != NULL ; i = get_next_interface(i)) {
			if (get_interface(i)->state == GO) {
				for (j = 0 ; espress_stream_trans_packet->yield[j] != NULL ; j++) {
					espress_stream_trans_packet->yield[j](get_interface(i)->interface, espress_stream_uint32_0);
				}
			}
		}
	}

	static void session_yield()
	{
		void *i;
		void *j;
		int k;
		for (i = get_first_interface() ; i != NULL ; i = get_next_interface(i)) {
			for (j = get_first_session(i) ; j != NULL ; j = get_next_session(i, j)) {
				if (get_session(j)->state == GO) {
					for (k = 0 ; espress_stream_session->yield[k] != NULL ; k++) {
						espress_stream_session->yield[k](get_session(j)->session, espress_stream_uint32_0);
					}
				}
			}
		}
	}

	static void event_wait(struct espress_stream_event *event, struct espress_stream_event_flags *flags, struct espress_stream_uint32 const *timeout)
	{
		struct espress_stream_uint32 u[1];
		struct espress_stream_uint32 start_time[1];
		espress_stream_tick->get_us(start_time);
		do {
			sched_yield();
			espress_stream_mutex->lock(local_mutex);
			interface_yield();
			session_yield();
			espress_stream_mutex->unlock(local_mutex);
			espress_stream_event->wait(event, flags, espress_stream_uint32_0);
			if (espress_stream_event_flags->is_set_any(flags)) {
				break;
			}
			espress_stream_tick->get_us(u);
			espress_stream_uint32_sub(u, start_time);
		} while (espress_stream_uint32_lt(u, timeout));
	}

	static i_t event_poll(struct espress_stream_event * const *event, u_t num, struct espress_stream_uint32 const *timeout)
	{
		i_t ret;
		struct espress_stream_uint32 u[1];
		struct espress_stream_uint32 start_time[1];
		espress_stream_tick->get_us(start_time);
		do {
			sched_yield();
			espress_stream_mutex->lock(local_mutex);
			interface_yield();
			session_yield();
			espress_stream_mutex->unlock(local_mutex);
			ret = espress_stream_event->poll(event, num, espress_stream_uint32_0);
			if (ret != 0) {
				break;
			}
			espress_stream_tick->get_us(u);
			espress_stream_uint32_sub(u, start_time);
		} while (espress_stream_uint32_lt(u, timeout));
		return ret;
	}

#	define create_interface_yield_thread(this) ((void)(this), 0)

#	define destroy_interface_yield_thread(this) ((void)(this))

#	define create_session_yield_thread(this) ((void)(this), 0)

#	define destroy_session_yield_thread(this) ((void)(this))
#endif

int espress_stream_begin()
{
	int ret = 0;
	if (espress_stream_tick->initialize() != 0) {
		ret = 1;
		goto TICK_INIT_FAIL_EXIT;
	}
	if (espress_stream_rand->initialize() != 0) {
		ret = 1;
		goto RAND_INIT_FAIL_EXIT;
	}
	if (espress_stream_mutex->initialize(local_mutex) != 0) {
		ret = 1;
		goto MUTEX_INIT_FAIL_EXIT;
	}
	begin_interface();
MUTEX_INIT_FAIL_EXIT:
	if (0 != ret) espress_stream_rand->finalize();
RAND_INIT_FAIL_EXIT:
	if (0 != ret) espress_stream_tick->finalize();
TICK_INIT_FAIL_EXIT:
	return ret;
}

void espress_stream_end()
{
	end_interface();
	espress_stream_mutex->finalize(local_mutex);
	espress_stream_rand->finalize();
	espress_stream_tick->finalize();
}

char const *espress_stream_copyright()
{
#	define STR(num) #num
#	define NUM_TO_STR(num) STR(num)
	return ESPRESS_STREAM_NAME
		" "
		NUM_TO_STR(ESPRESS_STREAM_MAJOR_VERSION)
		"."
		NUM_TO_STR(ESPRESS_STREAM_MINOR_VERSION)
		"-"
		NUM_TO_STR(ESPRESS_STREAM_PROTOCOL_VERSION)
		" "
		ESPRESS_STREAM_COPYRIGHT;
}

unsigned espress_stream_time(void)
{
	unsigned u;
	struct espress_stream_uint32 time[1];
	espress_stream_tick->get_us(time);
	uint32_get_u(time, &u);
	return u;
}

int espress_stream_begin_interface(void **ifd, struct sockaddr_storage const *address)
{
	int ret = 0;
	void *i;
	int j;
	struct interface *this;
	struct espress_stream_dgram_address dgram_address[1] = {0};

	sched_yield();
	espress_stream_mutex->lock(local_mutex);
#	if !ESPRESS_STREAM_API_IS_THREAD_ENABLED
		interface_yield();
		session_yield();
#	endif

	i = new_interface();
	if (i == NULL) {
		ret = 1;
		goto INTERFACE_FULL_EXIT;
	}
	begin_session(i);
	this = get_interface(i);

	memcpy(dgram_address->storage, address, sizeof(*(dgram_address->storage)));
	j = espress_stream_trans_packet->initialize(this->interface, dgram_address);
	if (j != 0) {
		ret = 1;
		goto INTERFACE_INIT_FAIL_EXIT;
	}

	this->state = GO;

	j = create_interface_yield_thread(this);
	if (j != 0) {
		ret = 1;
		goto CREATE_THREAD_FAIL_EXIT;
	}

	*ifd = i;

CREATE_THREAD_FAIL_EXIT:
	if (ret != 0) this->state = SET;
	if (ret != 0) espress_stream_trans_packet->finalize(this->interface);
INTERFACE_INIT_FAIL_EXIT:
	if (ret != 0) end_session(i);
	if (ret != 0) delete_interface(i);
INTERFACE_FULL_EXIT:

	espress_stream_mutex->unlock(local_mutex);

	return ret;
}

void espress_stream_end_interface(void *ifd)
{
	struct interface *this;

	sched_yield();
	espress_stream_mutex->lock(local_mutex);
#	if !ESPRESS_STREAM_API_IS_THREAD_ENABLED
		interface_yield();
		session_yield();
#	endif

	this = get_interface(ifd);

	this->state = SET;

	destroy_interface_yield_thread(this);

	espress_stream_trans_packet->finalize(this->interface);

	end_session(ifd);
	delete_interface(ifd);

	espress_stream_mutex->unlock(local_mutex);
}

static int send_connect_payload_callback(void *sid, void const *payload, unsigned payload_size, int is_rejected)
{
	i_t ret;
	struct session *this = get_session(sid);
	if (this->send_connect_payload_callback == NULL) {
		ret = 1;
	} else {
		ret = this->send_connect_payload_callback(this->session, (b_t const *)payload, payload_size, is_rejected);
	}
	return ret;
}

static i_t send_connect_payload(struct espress_stream_session *session, i_t (*callback)(struct espress_stream_session *session, b_t const *payload, u_t payload_size, i_t is_rejected), void *param)
{
	i_t ret;
	void *sid = param;
	struct session *this = get_session(sid);
	this->send_connect_payload_callback = callback;
	if (this->ex_param->send_connect_payload == NULL) {
		ret = send_connect_payload_callback(sid, NULL, 0, 0);
	} else {
		ret = this->ex_param->send_connect_payload(sid, send_connect_payload_callback, this->ex_param->send_connect_payload_param);
	}
	return ret;
}

static int recv_connect_payload_callback(void *sid, void *payload, unsigned *payload_size)
{
	i_t ret;
	u_t u;
	struct session *this = get_session(sid);
	if (this->recv_connect_payload_callback == NULL) {
		ret = 1;
	} else {
		ret = this->recv_connect_payload_callback(this->session, (b_t *)payload, (payload_size == NULL) ? NULL : (u = *payload_size, &u));
		if ((ret == 0) && (payload_size != NULL)) {
			*payload_size = u;
		}
	}
	return ret;
}

static i_t recv_connect_payload(struct espress_stream_session *session, i_t (*callback)(struct espress_stream_session *session, b_t *payload, u_t *payload_size), void *param)
{
	i_t ret;
	void *sid = param;
	struct session *this = get_session(sid);
	this->recv_connect_payload_callback = callback;
	if (this->ex_param->recv_connect_payload == NULL) {
		ret = recv_connect_payload_callback(sid, NULL, NULL);
	} else {
		ret = this->ex_param->recv_connect_payload(sid, recv_connect_payload_callback, this->ex_param->recv_connect_payload_param);
	}
	return ret;
}

static int send_disconnect_payload_callback(void *sid, void const *payload, unsigned payload_size)
{
	i_t ret;
	struct session *this = get_session(sid);
	if (this->send_disconnect_payload_callback == NULL) {
		ret = 1;
	} else {
		ret = this->send_disconnect_payload_callback(this->session, (b_t const *)payload, payload_size);
	}
	return ret;
}

static i_t send_disconnect_payload(struct espress_stream_session *session, i_t (*callback)(struct espress_stream_session *session, b_t const *payload, u_t payload_size), void *param)
{
	i_t ret;
	void *sid = param;
	struct session *this = get_session(sid);
	this->send_disconnect_payload_callback = callback;
	if (this->ex_param->send_disconnect_payload == NULL) {
		ret = send_disconnect_payload_callback(sid, NULL, 0);
	} else {
		ret = this->ex_param->send_disconnect_payload(sid, send_disconnect_payload_callback, this->ex_param->send_disconnect_payload_param);
	}
	return ret;
}

static int recv_disconnect_payload_callback(void *sid, void *payload, unsigned *payload_size)
{
	i_t ret;
	u_t u;
	struct session *this = get_session(sid);
	if (this->recv_disconnect_payload_callback == NULL) {
		ret = 1;
	} else {
		ret = this->recv_disconnect_payload_callback(this->session, (b_t *)payload, (payload_size == NULL) ? NULL : (u = *payload_size, &u));
		if ((ret == 0) && (payload_size != NULL)) {
			*payload_size = u;
		}
	}
	return ret;
}

static i_t recv_disconnect_payload(struct espress_stream_session *session, i_t (*callback)(struct espress_stream_session *session, b_t *payload, u_t *payload_size), void *param)
{
	i_t ret;
	void *sid = param;
	struct session *this = get_session(sid);
	this->recv_disconnect_payload_callback = callback;
	if (this->ex_param->recv_disconnect_payload == NULL) {
		ret = recv_disconnect_payload_callback(sid, NULL, NULL);
	} else {
		ret = this->ex_param->recv_disconnect_payload(sid, recv_disconnect_payload_callback, this->ex_param->recv_disconnect_payload_param);
	}
	return ret;
}

static int local_begin_session(struct session *this)
{
	int ret = 0;
	i_t i;

	i = espress_stream_event->initialize(this->event);
	if (0 != i)
	{
		ret = 1;
		goto EVENT_INIT_FAIL_EXIT;
	}
	espress_stream_event_flags->reset_all(this->flags);
	espress_stream_ring_buffer->initialize(this->send_ring_buffer, this->send_buffer, sizeof(this->send_buffer));
	espress_stream_ring_buffer->initialize(this->recv_ring_buffer, this->recv_buffer, sizeof(this->recv_buffer));
	i = espress_stream_session->initialize(this->session,
		get_interface(this->ifd)->interface,
		this->event,
		this->is_server,
		this->send_ring_buffer,
		this->recv_ring_buffer,
		send_connect_payload, get_sid(this),
		recv_connect_payload, get_sid(this),
		send_disconnect_payload, get_sid(this),
		recv_disconnect_payload, get_sid(this));
	if (i != 0) {
		ret = 1;
		goto SESSION_INIT_FAIL_EXIT;
	}
	this->state = GO;
	i = create_session_yield_thread(this);
	if (i != 0) {
		ret = 1;
		goto CREATE_THREAD_FAIL_EXIT;
	}

CREATE_THREAD_FAIL_EXIT:
	if (ret != 0) this->state = SET;
	if (ret != 0) espress_stream_session->finalize(this->session);
SESSION_INIT_FAIL_EXIT:
	if (ret != 0) espress_stream_ring_buffer->finalize(this->recv_ring_buffer);
	if (ret != 0) espress_stream_ring_buffer->finalize(this->send_ring_buffer);
	if (ret != 0) espress_stream_event->finalize(this->event);
EVENT_INIT_FAIL_EXIT:
	return ret;
}

int espress_stream_begin_session(void **sid, void *ifd, int is_server, struct espress_stream_session_ex_param const *ex_param)
{
	int ret = 0;
	void *i;
	int j;
	struct session *this;

	sched_yield();
	espress_stream_mutex->lock(local_mutex);
#	if !ESPRESS_STREAM_API_IS_THREAD_ENABLED
		interface_yield();
		session_yield();
#	endif

	i = new_session(ifd);
	if (i == NULL) {
		ret = 1;
		goto SESSION_FULL_EXIT;
	}
	this = get_session(i);

	this->is_server = is_server;
	if (ex_param == NULL) {
		memset(this->ex_param, 0, sizeof(*(this->ex_param)));
	} else {
		memcpy(this->ex_param, ex_param, sizeof(*(this->ex_param)));
	}
	this->send_connect_payload_callback = NULL;
	this->recv_connect_payload_callback = NULL;
	this->send_disconnect_payload_callback = NULL;
	this->recv_disconnect_payload_callback = NULL;
	j = espress_stream_mutex->initialize(this->mutex);
	if (j != 0) {
		ret = 1;
		goto MUTEX_INIT_FAIL_EXIT;
	}
	j = local_begin_session(this);
	if (j != 0) {
		ret = 1;
		goto SESSION_INIT_FAIL_EXIT;
	}

	this->src_state = INIT;
	this->dst_state = INIT;
	this->is_in_progress = 0;
	this->is_broken = 0;

	*sid = i;

SESSION_INIT_FAIL_EXIT:
	if (ret != 0) espress_stream_mutex->finalize(this->mutex);
MUTEX_INIT_FAIL_EXIT:
	if (ret != 0) delete_session(i);
SESSION_FULL_EXIT:

	espress_stream_mutex->unlock(local_mutex);

	return ret;
}

static void local_end_session(struct session *this)
{
	this->state = SET;
	destroy_session_yield_thread(this);
	espress_stream_session->finalize(this->session);
	espress_stream_ring_buffer->finalize(this->recv_ring_buffer);
	espress_stream_ring_buffer->finalize(this->send_ring_buffer);
	espress_stream_event->finalize(this->event);
}

void espress_stream_end_session(void *sid)
{
	struct session *this;

	sched_yield();
	espress_stream_mutex->lock(local_mutex);
#	if !ESPRESS_STREAM_API_IS_THREAD_ENABLED
		interface_yield();
		session_yield();
#	endif

	this = get_session(sid);

	local_end_session(this);
	espress_stream_mutex->finalize(this->mutex);

	delete_session(sid);

	espress_stream_mutex->unlock(local_mutex);
}

static int local_connect(struct session *this)
{
	int ret;
	struct espress_stream_dgram_address dgram_address[1] = {0};

	espress_stream_tick->get_us(this->start_time);
	memcpy(dgram_address->storage, this->address, sizeof(*(dgram_address->storage)));
	ret = espress_stream_session->connect(this->session, dgram_address);

	return ret;
}

int espress_stream_connect(void *sid, struct sockaddr_storage const *address)
{
	int ret;
	struct session *this = get_session(sid);

	sched_yield();
#	if !ESPRESS_STREAM_API_IS_THREAD_ENABLED
		espress_stream_mutex->lock(local_mutex);
		interface_yield();
		session_yield();
		espress_stream_mutex->unlock(local_mutex);
#	endif

	espress_stream_mutex->lock(this->mutex);

	if ((this->src_state != INIT) || this->is_in_progress) {
		ret = 1;
		goto EXIT;
	}
	this->is_in_progress = 1;

	memcpy(this->address, address, sizeof(*(this->address)));
	ret = local_connect(this);

	this->src_state = CONNECTING;
	this->is_in_progress = 0;

EXIT:

	espress_stream_mutex->unlock(this->mutex);

	return ret;
}

static int retry_connect(struct session *this,
	struct espress_stream_uint32 const *now,
	struct espress_stream_uint32 *timeout)
{
	int ret;
	struct espress_stream_uint32 u[1];

	espress_stream_uint32_assign(u, now);
	espress_stream_uint32_sub(u, this->start_time);
	if (espress_stream_uint32_lt(u, espress_stream_connect_timeout_time)) {
		espress_stream_uint32_assign(timeout, espress_stream_connect_timeout_time);
		espress_stream_uint32_sub(timeout, u);
		ret = -1;
	} else {
		this->state = SET;
		local_end_session(this);
		ret = local_begin_session(this);
		if (ret == 0) {
			this->state = GO;
			ret = local_connect(this);
			if (ret == 0) {
				ret = -1;
			} else {
				ret = 1;
			}
		} else {
			ret = 1;
		}
		espress_stream_uint32_assign(timeout, espress_stream_connect_timeout_time);
	}

	return ret;
}

int espress_stream_connect_result(void *sid, unsigned timeout)
{
	int ret;
	struct espress_stream_event_flags f[1];
	struct espress_stream_uint32 t[1];
	struct espress_stream_uint32 u[1];
	struct espress_stream_uint32 v[1];
	struct session *this = get_session(sid);
	struct espress_stream_uint32 start_time[1];
	struct espress_stream_uint32 timeout_time[1];

	sched_yield();
#	if !ESPRESS_STREAM_API_IS_THREAD_ENABLED
		espress_stream_mutex->lock(local_mutex);
		interface_yield();
		session_yield();
		espress_stream_mutex->unlock(local_mutex);
#	endif

	espress_stream_mutex->lock(this->mutex);

	if ((this->src_state != CONNECTING) || this->is_in_progress) {
		ret = 1;
		goto EXIT;
	}
	this->is_in_progress = 1;

	espress_stream_tick->get_us(start_time);
	uint32_set_u(timeout_time, timeout);
	event_wait(this->event, f, espress_stream_uint32_0);
	espress_stream_event_flags->compo(this->flags, f);
	for ( ; ; ) {
		if (espress_stream_event_flags->is_set(this->flags, ESPRESS_STREAM_EVENT_SESSION_CONNECT_RESULT)) {
			espress_stream_event_flags->reset(this->flags, ESPRESS_STREAM_EVENT_SESSION_CONNECT_RESULT);
			ret = espress_stream_session->connect_result(this->session);
		} else {
			ret = -1;
		}
		if (0 <= ret) {
			break;
		}

		espress_stream_tick->get_us(u);

		ret = retry_connect(this, u, v);
		if (0 <= ret) {
			break;
		}

		espress_stream_uint32_sub(u, start_time);
		if (!this->is_broken && espress_stream_uint32_lt(u, timeout_time)) {
			espress_stream_uint32_assign(t, timeout_time);
			espress_stream_uint32_sub(t, u);
		} else {
			break;
		}

		if (espress_stream_uint32_lt(v, t)) {
			espress_stream_uint32_assign(t, v);
		}

		espress_stream_mutex->unlock(this->mutex);
		event_wait(this->event, f, t);
		espress_stream_mutex->lock(this->mutex);
		espress_stream_event_flags->compo(this->flags, f);
	}

	espress_stream_event_flags->reset(this->flags, ESPRESS_STREAM_EVENT_NONE);
	this->is_broken = 0;

	if (ret == 0) {
		this->src_state = CONNECTED;
		this->dst_state = CONNECTED;
		espress_stream_event_flags->set(this->flags, ESPRESS_STREAM_EVENT_SESSION_SEND);
	} else if (0 < ret) {
		this->src_state = DISCONNECTED;
		this->dst_state = DISCONNECTED;
	}
	this->is_in_progress = 0;

EXIT:

	espress_stream_mutex->unlock(this->mutex);

	return ret;
}

int espress_stream_connect_detect(void *sid, unsigned timeout)
{
	int ret;
	struct espress_stream_uint32 t[1];
	struct espress_stream_uint32 u[1];
	struct espress_stream_event_flags f[1];
	struct session *this = get_session(sid);
	struct espress_stream_uint32 start_time[1];
	struct espress_stream_uint32 timeout_time[1];

	sched_yield();
#	if !ESPRESS_STREAM_API_IS_THREAD_ENABLED
		espress_stream_mutex->lock(local_mutex);
		interface_yield();
		session_yield();
		espress_stream_mutex->unlock(local_mutex);
#	endif

	espress_stream_mutex->lock(this->mutex);

	if ((this->src_state != INIT) || this->is_in_progress) {
		ret = 1;
		goto EXIT;
	}
	this->is_in_progress = 1;

	espress_stream_tick->get_us(start_time);
	uint32_set_u(timeout_time, timeout);
	event_wait(this->event, f, espress_stream_uint32_0);
	espress_stream_event_flags->compo(this->flags, f);
	for ( ; ; ) {
		if (espress_stream_event_flags->is_set(this->flags, ESPRESS_STREAM_EVENT_SESSION_CONNECT_DETECT)) {
			espress_stream_event_flags->reset(this->flags, ESPRESS_STREAM_EVENT_SESSION_CONNECT_DETECT);
			ret = espress_stream_session->connect_detect(this->session);
		} else {
			ret = -1;
		}
		if (0 <= ret) {
			break;
		}

		espress_stream_tick->get_us(u);
		espress_stream_uint32_sub(u, start_time);
		if (!this->is_broken && espress_stream_uint32_lt(u, timeout_time)) {
			espress_stream_uint32_assign(t, timeout_time);
			espress_stream_uint32_sub(t, u);
		} else {
			break;
		}

		espress_stream_mutex->unlock(this->mutex);
		event_wait(this->event, f, t);
		espress_stream_mutex->lock(this->mutex);
		espress_stream_event_flags->compo(this->flags, f);
	}

	espress_stream_event_flags->reset(this->flags, ESPRESS_STREAM_EVENT_NONE);
	this->is_broken = 0;

	if (ret == 0) {
		this->src_state = CONNECTED;
		this->dst_state = CONNECTED;
		espress_stream_event_flags->set(this->flags, ESPRESS_STREAM_EVENT_SESSION_SEND);
	} else if (0 < ret) {
		this->src_state = DISCONNECTED;
		this->dst_state = DISCONNECTED;
	}
	this->is_in_progress = 0;

EXIT:

	espress_stream_mutex->unlock(this->mutex);

	return ret;
}

int espress_stream_disconnect(void *sid)
{
	int ret;
	struct session *this = get_session(sid);

	sched_yield();
#	if !ESPRESS_STREAM_API_IS_THREAD_ENABLED
		espress_stream_mutex->lock(local_mutex);
		interface_yield();
		session_yield();
		espress_stream_mutex->unlock(local_mutex);
#	endif

	espress_stream_mutex->lock(this->mutex);

	if ((this->src_state != CONNECTED) || this->is_in_progress) {
		ret = 1;
		goto EXIT;
	}
	this->is_in_progress = 1;

	ret = espress_stream_session->disconnect(this->session);

	if (ret == 0) {
		this->src_state = DISCONNECTING;
	} else if (0 < ret) {
		this->src_state = DISCONNECTED;
	}
	this->is_in_progress = 0;

EXIT:

	espress_stream_mutex->unlock(this->mutex);

	return ret;
}

int espress_stream_disconnect_result(void *sid, unsigned timeout)
{
	int ret;
	struct espress_stream_uint32 t[1];
	struct espress_stream_uint32 u[1];
	struct espress_stream_event_flags f[1];
	struct session *this = get_session(sid);
	struct espress_stream_uint32 start_time[1];
	struct espress_stream_uint32 timeout_time[1];

	sched_yield();
#	if !ESPRESS_STREAM_API_IS_THREAD_ENABLED
		espress_stream_mutex->lock(local_mutex);
		interface_yield();
		session_yield();
		espress_stream_mutex->unlock(local_mutex);
#	endif

	espress_stream_mutex->lock(this->mutex);

	if ((this->src_state != DISCONNECTING) || this->is_in_progress) {
		ret = 1;
		goto EXIT;
	}
	this->is_in_progress = 1;

	espress_stream_tick->get_us(start_time);
	uint32_set_u(timeout_time, timeout);
	event_wait(this->event, f, espress_stream_uint32_0);
	espress_stream_event_flags->compo(this->flags, f);
	for ( ; ; ) {
		if (espress_stream_event_flags->is_set(this->flags, ESPRESS_STREAM_EVENT_SESSION_DISCONNECT_RESULT)) {
			espress_stream_event_flags->reset(this->flags, ESPRESS_STREAM_EVENT_SESSION_DISCONNECT_RESULT);
			ret = espress_stream_session->disconnect_result(this->session);
		} else {
			ret = -1;
		}
		if (0 <= ret) {
			break;
		}

		espress_stream_tick->get_us(u);
		espress_stream_uint32_sub(u, start_time);
		if (!this->is_broken && espress_stream_uint32_lt(u, timeout_time)) {
			espress_stream_uint32_assign(t, timeout_time);
			espress_stream_uint32_sub(t, u);
		} else {
			break;
		}

		espress_stream_mutex->unlock(this->mutex);
		event_wait(this->event, f, t);
		espress_stream_mutex->lock(this->mutex);
		espress_stream_event_flags->compo(this->flags, f);
	}

	espress_stream_event_flags->reset(this->flags, ESPRESS_STREAM_EVENT_NONE);
	this->is_broken = 0;

	if (ret == 0) {
		this->src_state = DISCONNECTED;
	} else if (0 < ret) {
		this->src_state = DISCONNECTED;
	}
	this->is_in_progress = 0;

EXIT:

	espress_stream_mutex->unlock(this->mutex);

	return ret;
}

static void local_send(struct session *this, void const *data, unsigned *data_size)
{
	u_t u;
	u_t v;
	b_t *d;
	u_t d_size;
	b_t *buffer;
	u_t buffer_size;

	d = espress_stream_ring_buffer->head(this->send_ring_buffer, NULL, 0);
	if (d == NULL) {
		*data_size = 0;
		goto EXIT;
	}
	d_size = espress_stream_ring_buffer->num_invalid(this->send_ring_buffer);
	if (*data_size < d_size) d_size = *data_size;
	buffer = espress_stream_ring_buffer->buffer(this->send_ring_buffer);
	buffer_size = espress_stream_ring_buffer->size(this->send_ring_buffer);
	if ((buffer_size - (d - buffer)) < d_size) {
		u = d_size - (buffer_size - (d - buffer));
	} else {
		u = 0;
	}

	if (data != NULL) {
		v = d_size - u;
		memcpy(d, data, v);
		if (0 < u) {
			memcpy(buffer, data + v, u);
		}

		if (0 < d_size) {
			espress_stream_ring_buffer->en(this->send_ring_buffer, d_size);
			espress_stream_session->send(this->session);
		}
	}

	*data_size = d_size;

EXIT:

	return;
}

int espress_stream_send(void *sid, void const *data, unsigned *data_size, unsigned timeout)
{
	int ret = 0;
	struct espress_stream_uint32 t[1];
	struct espress_stream_uint32 u[1];
	unsigned v;
	unsigned w;
	struct espress_stream_event_flags f[1];
	struct session *this = get_session(sid);
	struct espress_stream_uint32 start_time[1];
	struct espress_stream_uint32 timeout_time[1];

	sched_yield();
#	if !ESPRESS_STREAM_API_IS_THREAD_ENABLED
		espress_stream_mutex->lock(local_mutex);
		interface_yield();
		session_yield();
		espress_stream_mutex->unlock(local_mutex);
#	endif

	espress_stream_mutex->lock(this->mutex);

	if ((this->src_state != CONNECTED) || this->is_in_progress || (*data_size == 0)) {
		ret = 1;
		goto EXIT;
	}
	this->is_in_progress = 1;

	espress_stream_tick->get_us(start_time);
	uint32_set_u(timeout_time, timeout);
	event_wait(this->event, f, espress_stream_uint32_0);
	espress_stream_event_flags->compo(this->flags, f);
	espress_stream_event_flags->reset(this->flags, ESPRESS_STREAM_EVENT_SESSION_SEND);
	v = *data_size;
	local_send(this, data, &v);
	if (v < *data_size) {
		for ( ; ; ) {
			if (espress_stream_event_flags->is_set(this->flags, ESPRESS_STREAM_EVENT_SESSION_ERROR)) {
				ret = 1;
				break;
			}

			if (espress_stream_event_flags->is_set(this->flags, ESPRESS_STREAM_EVENT_SESSION_SEND)) {
				espress_stream_event_flags->reset(this->flags, ESPRESS_STREAM_EVENT_SESSION_SEND);
				if (data == NULL) {
					v = 0;
				}
				w = *data_size - v;
				local_send(this, ((b_t *)data) + v, &w);
				v += w;
				if (v == *data_size) {
					break;
				}
			}

			espress_stream_tick->get_us(u);
			espress_stream_uint32_sub(u, start_time);
			if (!this->is_broken && espress_stream_uint32_lt(u, timeout_time)) {
				espress_stream_uint32_assign(t, timeout_time);
				espress_stream_uint32_sub(t, u);
			} else {
				break;
			}

			espress_stream_mutex->unlock(this->mutex);
			event_wait(this->event, f, t);
			espress_stream_mutex->lock(this->mutex);
			espress_stream_event_flags->compo(this->flags, f);
		}
	}

	if (ret == 0) {
		if (0 < v) {
			*data_size = v;
		} else {
			ret = -1;
		}
	}

	espress_stream_event_flags->reset(this->flags, ESPRESS_STREAM_EVENT_NONE);
	this->is_broken = 0;

	if (ret == 0) {
		if (!espress_stream_ring_buffer->is_full(this->send_ring_buffer)) {
			espress_stream_event_flags->set(this->flags, ESPRESS_STREAM_EVENT_SESSION_SEND);
		}
	} else if (0 < ret) {
		espress_stream_event_flags->reset(this->flags, ESPRESS_STREAM_EVENT_SESSION_ERROR);
		this->src_state = DISCONNECTED;
		if (this->dst_state == CONNECTED) {
			espress_stream_event_flags->set(this->flags, ESPRESS_STREAM_EVENT_SESSION_ERROR);
		}
	}
	this->is_in_progress = 0;

EXIT:

	espress_stream_mutex->unlock(this->mutex);

	return ret;
}

int espress_stream_flush(void *sid, unsigned timeout)
{
	int ret;
	struct espress_stream_uint32 t[1];
	struct espress_stream_uint32 u[1];
	struct espress_stream_event_flags f[1];
	struct session *this = get_session(sid);
	struct espress_stream_uint32 start_time[1];
	struct espress_stream_uint32 timeout_time[1];

	sched_yield();
#	if !ESPRESS_STREAM_API_IS_THREAD_ENABLED
		espress_stream_mutex->lock(local_mutex);
		interface_yield();
		session_yield();
		espress_stream_mutex->unlock(local_mutex);
#	endif

	espress_stream_mutex->lock(this->mutex);

	if ((this->src_state != CONNECTED) || this->is_in_progress) {
		ret = 1;
		goto EXIT;
	}
	this->is_in_progress = 1;

	espress_stream_tick->get_us(start_time);
	uint32_set_u(timeout_time, timeout);
	event_wait(this->event, f, espress_stream_uint32_0);
	espress_stream_event_flags->compo(this->flags, f);
	espress_stream_event_flags->reset(this->flags, ESPRESS_STREAM_EVENT_SESSION_SEND);
	ret = espress_stream_session->flush(this->session);
	if (ret == 0) {
		for ( ; (ret = (espress_stream_ring_buffer->is_empty(this->send_ring_buffer) ? 0 : -1)) == -1 ; ) {
			if (espress_stream_event_flags->is_set(this->flags, ESPRESS_STREAM_EVENT_SESSION_ERROR)) {
				ret = 1;
				break;
			}

			if (espress_stream_event_flags->is_set(this->flags, ESPRESS_STREAM_EVENT_SESSION_SEND)) {
				espress_stream_event_flags->reset(this->flags, ESPRESS_STREAM_EVENT_SESSION_SEND);
			}

			espress_stream_tick->get_us(u);
			espress_stream_uint32_sub(u, start_time);
			if (!this->is_broken && espress_stream_uint32_lt(u, timeout_time)) {
				espress_stream_uint32_assign(t, timeout_time);
				espress_stream_uint32_sub(t, u);
			} else {
				break;
			}

			espress_stream_mutex->unlock(this->mutex);
			event_wait(this->event, f, t);
			espress_stream_mutex->lock(this->mutex);
			espress_stream_event_flags->compo(this->flags, f);
		}
	}

	espress_stream_event_flags->reset(this->flags, ESPRESS_STREAM_EVENT_NONE);
	this->is_broken = 0;

	if (ret <= 0) {
		if (!espress_stream_ring_buffer->is_full(this->send_ring_buffer)) {
			espress_stream_event_flags->set(this->flags, ESPRESS_STREAM_EVENT_SESSION_SEND);
		}
	} else {
		espress_stream_event_flags->reset(this->flags, ESPRESS_STREAM_EVENT_SESSION_ERROR);
		this->src_state = DISCONNECTED;
		if (this->dst_state == CONNECTED) {
			espress_stream_event_flags->set(this->flags, ESPRESS_STREAM_EVENT_SESSION_ERROR);
		}
	}
	this->is_in_progress = 0;

EXIT:

	espress_stream_mutex->unlock(this->mutex);

	return ret;
}

static void local_recv(struct session *this, void *data, unsigned *data_size)
{
	u_t u;
	u_t v;
	b_t *d;
	u_t d_size;
	b_t *buffer;
	u_t buffer_size;

	d = espress_stream_ring_buffer->tail(this->recv_ring_buffer, NULL, 0);
	if (d == NULL) {
		*data_size = 0;
		goto EXIT;
	}
	d_size = espress_stream_ring_buffer->num_valid(this->recv_ring_buffer);
	if (*data_size < d_size) d_size = *data_size;
	buffer = espress_stream_ring_buffer->buffer(this->recv_ring_buffer);
	buffer_size = espress_stream_ring_buffer->size(this->recv_ring_buffer);
	if ((buffer_size - (d - buffer)) < d_size) {
		u = d_size - (buffer_size - (d - buffer));
	} else {
		u = 0;
	}

	if (data != NULL) {
		v = d_size - u;
		memcpy(data, d, v);
		if (0 < u) {
			memcpy(data + v, buffer, u);
		}

		if (0 < d_size) {
			espress_stream_ring_buffer->de(this->recv_ring_buffer, d_size);
			espress_stream_session->recv(this->session);
		}
	}

	*data_size = d_size;

EXIT:

	return;
}

int espress_stream_recv(void *sid, void *data, unsigned *data_size, unsigned timeout)
{
	int ret = 0;
	struct espress_stream_uint32 t[1];
	struct espress_stream_uint32 u[1];
	unsigned v;
	struct espress_stream_event_flags f[1];
	struct session *this = get_session(sid);
	struct espress_stream_uint32 start_time[1];
	struct espress_stream_uint32 timeout_time[1];

	sched_yield();
#	if !ESPRESS_STREAM_API_IS_THREAD_ENABLED
		espress_stream_mutex->lock(local_mutex);
		interface_yield();
		session_yield();
		espress_stream_mutex->unlock(local_mutex);
#	endif

	espress_stream_mutex->lock(this->mutex);

	if ((this->dst_state != CONNECTED) || this->is_in_progress || (*data_size == 0)) {
		ret = 1;
		goto EXIT;
	}
	this->is_in_progress = 1;

	espress_stream_tick->get_us(start_time);
	uint32_set_u(timeout_time, timeout);
	event_wait(this->event, f, espress_stream_uint32_0);
	espress_stream_event_flags->compo(this->flags, f);
	espress_stream_event_flags->reset(this->flags, ESPRESS_STREAM_EVENT_SESSION_RECV);
	v = *data_size;
	local_recv(this, data, &v);
	if (v == 0) {
		for ( ; ; ) {
			if (espress_stream_event_flags->is_set(this->flags, ESPRESS_STREAM_EVENT_SESSION_ERROR)) {
				ret = 1;
				break;
			}

			if (espress_stream_event_flags->is_set(this->flags, ESPRESS_STREAM_EVENT_SESSION_RECV)) {
				espress_stream_event_flags->reset(this->flags, ESPRESS_STREAM_EVENT_SESSION_RECV);
				v = *data_size;
				local_recv(this, data, &v);
				if (0 < v) {
					break;
				}
			} else if (espress_stream_event_flags->is_set(this->flags, ESPRESS_STREAM_EVENT_SESSION_DISCONNECT_DETECT)) {
				espress_stream_event_flags->reset(this->flags, ESPRESS_STREAM_EVENT_SESSION_DISCONNECT_DETECT);
				*data_size = 0;
				ret = 0;
				goto DISCONNECT_DETECT_EXIT;
			}

			espress_stream_tick->get_us(u);
			espress_stream_uint32_sub(u, start_time);
			if (!this->is_broken && espress_stream_uint32_lt(u, timeout_time)) {
				espress_stream_uint32_assign(t, timeout_time);
				espress_stream_uint32_sub(t, u);
			} else {
				break;
			}

			espress_stream_mutex->unlock(this->mutex);
			event_wait(this->event, f, t);
			espress_stream_mutex->lock(this->mutex);
			espress_stream_event_flags->compo(this->flags, f);
		}
	}

	if (ret == 0) {
		if (0 < v) {
			*data_size = v;
			ret = 0;
		} else {
			ret = -1;
		}
	}

DISCONNECT_DETECT_EXIT:

	espress_stream_event_flags->reset(this->flags, ESPRESS_STREAM_EVENT_NONE);
	this->is_broken = 0;

	if (ret == 0) {
		if (*data_size == 0) {
			if (data == NULL) {
				espress_stream_event_flags->set(this->flags, ESPRESS_STREAM_EVENT_SESSION_DISCONNECT_DETECT);
			} else {
				this->dst_state = DISCONNECTED;
			}
		} else if (!espress_stream_ring_buffer->is_empty(this->recv_ring_buffer)) {
			espress_stream_event_flags->set(this->flags, ESPRESS_STREAM_EVENT_SESSION_RECV);
		}
	} else if (0 < ret) {
		this->dst_state = DISCONNECTED;
		if (this->src_state == CONNECTED) {
			espress_stream_event_flags->set(this->flags, ESPRESS_STREAM_EVENT_SESSION_ERROR);
		}
	}
	this->is_in_progress = 0;

EXIT:

	espress_stream_mutex->unlock(this->mutex);

	return ret;
}

int espress_stream_wait_any(void *sid, unsigned event, unsigned timeout)
{
	int ret;
	ret = espress_stream_poll(&sid, &event, 1, timeout) - 1;
	if ((ret < -1) || (1 < ret)) {
		ret = 1;
	}
	return ret;
}

void espress_stream_wait_break(void *sid)
{
	struct session *this = get_session(sid);

	espress_stream_mutex->lock(this->mutex);

	if (!this->is_broken) {
		this->is_broken = 1;
		espress_stream_event->set(this->event, ESPRESS_STREAM_EVENT_NONE);
	}

	espress_stream_mutex->unlock(this->mutex);
}

void espress_stream_wait_cancel_break(void *sid)
{
	struct session *this = get_session(sid);

	espress_stream_mutex->lock(this->mutex);

	if (this->is_broken) {
		this->is_broken = 0;
	}

	espress_stream_mutex->unlock(this->mutex);
}

int espress_stream_poll(void * const *sids, unsigned const *events, unsigned num, unsigned timeout)
{
	int ret;
	int i;
	int is_connecting;
	struct espress_stream_uint32 t[1];
	struct espress_stream_uint32 u[1];
	struct espress_stream_uint32 v[1];
	struct espress_stream_event_flags f[1];
	struct espress_stream_uint32 start_time[1];
	struct espress_stream_uint32 timeout_time[1];
#	define is_event_set(s, e) (                                                                                \
		((((e) & ESPRESS_STREAM_EVENT_CONNECT_RESULT) != 0) &&                                                 \
			espress_stream_event_flags->is_set((s)->flags, ESPRESS_STREAM_EVENT_SESSION_CONNECT_RESULT)) ||    \
		((((e) & ESPRESS_STREAM_EVENT_CONNECT_DETECT) != 0) &&                                                 \
			espress_stream_event_flags->is_set((s)->flags, ESPRESS_STREAM_EVENT_SESSION_CONNECT_DETECT)) ||    \
		((((e) & ESPRESS_STREAM_EVENT_DISCONNECT_RESULT) != 0) &&                                              \
			espress_stream_event_flags->is_set((s)->flags, ESPRESS_STREAM_EVENT_SESSION_DISCONNECT_RESULT)) || \
		((((e) & ESPRESS_STREAM_EVENT_RECV) != 0) &&                                                           \
			espress_stream_event_flags->is_set((s)->flags, ESPRESS_STREAM_EVENT_SESSION_DISCONNECT_DETECT)) || \
		((((e) & ESPRESS_STREAM_EVENT_SEND) != 0) &&                                                           \
			espress_stream_event_flags->is_set((s)->flags, ESPRESS_STREAM_EVENT_SESSION_SEND)) ||              \
		((((e) & ESPRESS_STREAM_EVENT_RECV) != 0) &&                                                           \
			espress_stream_event_flags->is_set((s)->flags, ESPRESS_STREAM_EVENT_SESSION_RECV)) ||              \
		((((e) & ESPRESS_STREAM_EVENT_FLUSH) != 0) &&                                                          \
			espress_stream_ring_buffer->is_empty((s)->send_ring_buffer)) ||                                    \
		((((e) & ESPRESS_STREAM_EVENT_SEND) != 0) &&                                                           \
			espress_stream_event_flags->is_set((s)->flags, ESPRESS_STREAM_EVENT_SESSION_ERROR)) ||             \
		((((e) & ESPRESS_STREAM_EVENT_RECV) != 0) &&                                                           \
			espress_stream_event_flags->is_set((s)->flags, ESPRESS_STREAM_EVENT_SESSION_ERROR)) ||             \
		((((e) & ESPRESS_STREAM_EVENT_FLUSH) != 0) &&                                                          \
			espress_stream_event_flags->is_set((s)->flags, ESPRESS_STREAM_EVENT_SESSION_ERROR)) ||             \
		((s)->is_broken &&                                                                                     \
			espress_stream_event_flags->is_set((s)->flags, ESPRESS_STREAM_EVENT_NONE))                         \
	)

	sched_yield();
#	if !ESPRESS_STREAM_API_IS_THREAD_ENABLED
		espress_stream_mutex->lock(local_mutex);
		interface_yield();
		session_yield();
		espress_stream_mutex->unlock(local_mutex);
#	endif

#	if 0 < ESPRESS_STREAM_MAX_NUM_POLL_SESSION
		struct espress_stream_event *event[ESPRESS_STREAM_MAX_NUM_POLL_SESSION];

		if (ESPRESS_STREAM_MAX_NUM_POLL_SESSION < num) {
			ret = -1;
			goto EXIT;
		}
#	else
		struct espress_stream_event **event;

		event = (struct espress_stream_event **)calloc(num, sizeof(struct espress_stream_event *));
		if (NULL == event) {
			ret = -1;
			goto EXIT;
		}
#	endif

	for (i = 0, ret = 0, is_connecting = 0 ; i < num ; i++) {
		espress_stream_mutex->lock(get_session(sids[i])->mutex);
		if (get_session(sids[i])->is_in_progress) {
			break;
		}
		get_session(sids[i])->is_in_progress = 1;
		if (is_event_set(get_session(sids[i]), events[i])) {
			ret++;
		}
		is_connecting = (is_connecting || (get_session(sids[i])->src_state == CONNECTING));
	}
	if (i < num) {
		for ( ; 0 <= i ; i--) {
			espress_stream_mutex->unlock(get_session(sids[i])->mutex);
		}
		ret = -1;
		goto EXIT;
	}

	if (0 == ret) {
		uint32_set_u(timeout_time, timeout);
		espress_stream_tick->get_us(start_time);
		if (is_connecting) {
			for ( ; ; ) {
				espress_stream_tick->get_us(u);

				espress_stream_uint32_assign(v, espress_stream_uint32_max);
				for (i = 0 ; i < num ; i++) {
					if (get_session(sids[i])->src_state == CONNECTING) {
						retry_connect(get_session(sids[i]), u, t);
						if (espress_stream_uint32_lt(t, v)) {
							espress_stream_uint32_assign(v, t);
						}
					}
				}

				espress_stream_uint32_sub(u, start_time);
				for (i = 0 ; i < num ; i++) {
					if(get_session(sids[i])->is_broken) {
						break;
					}
				}
				if ((i == num) && espress_stream_uint32_lt(u, timeout_time)) {
					espress_stream_uint32_assign(t, timeout_time);
					espress_stream_uint32_sub(t, u);
				} else {
					break;
				}

				if (espress_stream_uint32_lt(v, t)) {
					espress_stream_uint32_assign(t, v);
				}

				for (i = 0 ; i < num ; i++) {
					event[i] = get_session(sids[i])->event;
					espress_stream_mutex->unlock(get_session(sids[i])->mutex);
				}
				ret = event_poll(event, num, t);
				for (i = 0 ; i < num ; i++) {
					espress_stream_mutex->lock(get_session(sids[i])->mutex);
				}
				if (ret != 0) {
					for (i = 0, ret = 0 ; i < num ; i++) {
						event_wait(get_session(sids[i])->event, f, espress_stream_uint32_0);
						espress_stream_event_flags->compo(get_session(sids[i])->flags, f);
						if (is_event_set(get_session(sids[i]), events[i])) {
							ret++;
						}
					}
					if (ret != 0) {
						break;
					}
				}
			}
		} else {
			for ( ; ; ) {
				espress_stream_tick->get_us(u);
				espress_stream_uint32_sub(u, start_time);
				for (i = 0 ; i < num ; i++) {
					if(get_session(sids[i])->is_broken) {
						break;
					}
				}
				if ((i == num) && espress_stream_uint32_lt(u, timeout_time)) {
					espress_stream_uint32_assign(t, timeout_time);
					espress_stream_uint32_sub(t, u);
				} else {
					break;
				}

				for (i = 0 ; i < num ; i++) {
					event[i] = get_session(sids[i])->event;
					espress_stream_mutex->unlock(get_session(sids[i])->mutex);
				}
				ret = event_poll(event, num, t);
				for (i = 0 ; i < num ; i++) {
					espress_stream_mutex->lock(get_session(sids[i])->mutex);
				}
				if (ret != 0) {
					for (i = 0, ret = 0 ; i < num ; i++) {
						event_wait(get_session(sids[i])->event, f, espress_stream_uint32_0);
						espress_stream_event_flags->compo(get_session(sids[i])->flags, f);
						if (is_event_set(get_session(sids[i]), events[i])) {
							ret++;
						}
					}
					if (ret != 0) {
						break;
					}
				}
			}
		}
	}

	for (i = 0 ; i < num ; i++) {
		espress_stream_event_flags->reset(get_session(sids[i])->flags, ESPRESS_STREAM_EVENT_NONE);
		get_session(sids[i])->is_broken = 0;
		get_session(sids[i])->is_in_progress = 0;
		espress_stream_mutex->unlock(get_session(sids[i])->mutex);
	}

EXIT:
#	if 0 == ESPRESS_STREAM_MAX_NUM_POLL_SESSION
		if (NULL != event) free(event);
#	endif

	return ret;
}

void espress_stream_src_address(void *sid, struct sockaddr_storage *address)
{
	struct session *this = get_session(sid);
	struct espress_stream_dgram_address dgram_address[1];

	espress_stream_mutex->lock(this->mutex);

	espress_stream_session->src_address(this->session, dgram_address);
	memcpy(address, dgram_address->storage, sizeof(*address));

	espress_stream_mutex->unlock(this->mutex);
}

void espress_stream_dst_address(void *sid, struct sockaddr_storage *address)
{
	struct session *this = get_session(sid);
	struct espress_stream_dgram_address dgram_address[1];

	espress_stream_mutex->lock(this->mutex);

	espress_stream_session->dst_address(this->session, dgram_address);
	memcpy(address, dgram_address->storage, sizeof(*address));

	espress_stream_mutex->unlock(this->mutex);
}

unsigned espress_stream_connection_id(void *sid)
{
	unsigned u;
	struct espress_stream_uint32 v[1];
	struct session *this = get_session(sid);

	espress_stream_mutex->lock(this->mutex);

	espress_stream_session->connection_id(this->session, v);

	uint32_get_u(v, &u);

	espress_stream_mutex->unlock(this->mutex);

	return u;
}

void espress_stream_session_hash(void *sid, void *session_hash)
{
	struct session *this = get_session(sid);
	b_t shared_key[ESPRESS_STREAM_KEY_EX_KEY_SIZE];
	struct espress_stream_hash hash[1];
#	if ESPRESS_STREAM_SESSION_HASH_SIZE != ESPRESS_STREAM_HASH_DIGEST_SIZE
#		error
#	endif

	espress_stream_mutex->lock(this->mutex);

	espress_stream_session->shared_key(this->session, shared_key);
	espress_stream_hash->initialize(hash);
	espress_stream_hash->update(hash, shared_key, ESPRESS_STREAM_KEY_EX_KEY_SIZE);
	espress_stream_hash->finalize(hash, session_hash);

	espress_stream_mutex->unlock(this->mutex);
}

unsigned espress_stream_buffered_send_size(void *sid, unsigned *max_size)
{
	u_t u;
	struct session *this = get_session(sid);

	espress_stream_mutex->lock(this->mutex);

	u = espress_stream_ring_buffer->num_valid(this->send_ring_buffer);
	if (NULL != max_size) {
		*max_size = espress_stream_ring_buffer->size(this->send_ring_buffer);
	}

	espress_stream_mutex->unlock(this->mutex);

	return u;
}

unsigned espress_stream_buffered_recv_size(void *sid, unsigned *max_size)
{
	u_t u;
	struct session *this = get_session(sid);

	espress_stream_mutex->lock(this->mutex);

	u = espress_stream_ring_buffer->num_valid(this->recv_ring_buffer);
	if (NULL != max_size) {
		*max_size = espress_stream_ring_buffer->size(this->recv_ring_buffer);
	}

	espress_stream_mutex->unlock(this->mutex);

	return u;
}

unsigned espress_stream_rtt(void *sid)
{
	unsigned u;
	struct espress_stream_uint32 v[1];
	struct session *this = get_session(sid);

	espress_stream_mutex->lock(this->mutex);

	if (espress_stream_session->rtt(this->session, v) == 0) {
		uint32_get_u(v, &u);
	} else {
		u = 0;
	}

	espress_stream_mutex->unlock(this->mutex);

	return u;
}

unsigned espress_stream_rto(void *sid)
{
	unsigned u;
	struct espress_stream_uint32 v[1];
	struct session *this = get_session(sid);

	espress_stream_mutex->lock(this->mutex);

	espress_stream_session->rto(this->session, v);
	uint32_get_u(v, &u);

	espress_stream_mutex->unlock(this->mutex);

	return u;
}

unsigned espress_stream_dac(void *sid)
{
	u_t u;
	struct session *this = get_session(sid);

	espress_stream_mutex->lock(this->mutex);

	u = espress_stream_session->dac(this->session);

	espress_stream_mutex->unlock(this->mutex);

	return u;
}

unsigned espress_stream_cwnd(void *sid)
{
	u_t u;
	struct session *this = get_session(sid);

	espress_stream_mutex->lock(this->mutex);

	u = espress_stream_session->cwnd(this->session);

	espress_stream_mutex->unlock(this->mutex);

	return u;
}

unsigned espress_stream_send_timeout_count(void *sid)
{
	u_t u;
	struct session *this = get_session(sid);

	espress_stream_mutex->lock(this->mutex);

	u = espress_stream_session->send_timeout_count(this->session);

	espress_stream_mutex->unlock(this->mutex);

	return u;
}

unsigned espress_stream_fast_retransmit_count(void *sid)
{
	u_t u;
	struct session *this = get_session(sid);

	espress_stream_mutex->lock(this->mutex);

	u = espress_stream_session->fast_retransmit_count(this->session);

	espress_stream_mutex->unlock(this->mutex);

	return u;
}
