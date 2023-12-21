#include "espress_stream/config.h"
#include "espress_stream/depend.h"
#include "espress_stream/uint.h"
#include "espress_stream/tick.h"
#include "espress_stream/mutex.h"
#include "espress_stream/cond.h"
#include "espress_stream/event.h"

static void __reset_all(struct espress_stream_event_flags *this);
static void __set(struct espress_stream_event_flags *this, u_t flag);
static void __reset(struct espress_stream_event_flags *this, u_t flag);
static i_t __is_set(struct espress_stream_event_flags const *this, u_t flag);
static i_t __is_set_any(struct espress_stream_event_flags const *this);
static void __compo(struct espress_stream_event_flags *this, struct espress_stream_event_flags const *flags);

struct espress_stream_event_flags_ops const espress_stream_event_flags[] =
{{
	__reset_all,
	__set,
	__reset,
	__is_set,
	__is_set_any,
	__compo
}};

static void __reset_all(struct espress_stream_event_flags *this)
{
	espress_stream_event_flags_uint(_set_u)(this->flags, 0);
}

static void __set(struct espress_stream_event_flags *this, u_t flag)
{
	struct espress_stream_event_flags_uint() u[1] = {ESPRESS_STREAM_EVENT_FLAGS_UINT(_1)};
	espress_stream_event_flags_uint(_shift)(u, flag);
	espress_stream_event_flags_uint(_or)(this->flags, u);
}

static void __reset(struct espress_stream_event_flags *this, u_t flag)
{
	struct espress_stream_event_flags_uint() u[1] = {ESPRESS_STREAM_EVENT_FLAGS_UINT(_1)};
	espress_stream_event_flags_uint(_shift)(u, flag);
	espress_stream_event_flags_uint(_not)(u);
	espress_stream_event_flags_uint(_and)(this->flags, u);
}

static i_t __is_set(struct espress_stream_event_flags const *this, u_t flag)
{
	struct espress_stream_event_flags_uint() u[1] = {ESPRESS_STREAM_EVENT_FLAGS_UINT(_1)};
	espress_stream_event_flags_uint(_shift)(u, flag);
	espress_stream_event_flags_uint(_and)(u, this->flags);
	return !espress_stream_event_flags_uint(_eq)(u, espress_stream_event_flags_uint(_0));
}

static i_t __is_set_any(struct espress_stream_event_flags const *this)
{
	return !espress_stream_event_flags_uint(_eq)(this->flags, espress_stream_event_flags_uint(_0));
}

static void __compo(struct espress_stream_event_flags *this, struct espress_stream_event_flags const *flags)
{
	espress_stream_event_flags_uint(_or)(this->flags, flags->flags);
}

static i_t _initialize(struct espress_stream_event *this);
static void _finalize(struct espress_stream_event *this);
static void _set(struct espress_stream_event *this, u_t flag);
static void _wait(struct espress_stream_event *this, struct espress_stream_event_flags *flags, struct espress_stream_uint32 const *timeout);
static i_t _poll(struct espress_stream_event * const *these, u_t num, struct espress_stream_uint32 const *timeout);

struct espress_stream_event_ops const espress_stream_event[] =
{{
	_initialize,
	_finalize,
	_set,
	_wait,
	_poll
}};

static i_t _initialize(struct espress_stream_event *this)
{
	__reset_all(this->flags);
	return espress_stream_cond->initialize(this->cond);
}

static void _finalize(struct espress_stream_event *this)
{
	espress_stream_cond->finalize(this->cond);
	memset(this, 0, sizeof(*this));
}

static void _set(struct espress_stream_event *this, u_t flag)
{
	espress_stream_cond->signal_lock(this->cond);
	__set(this->flags, flag);
	espress_stream_cond->signal(this->cond);
	espress_stream_cond->signal_unlock(this->cond);
}

static void _wait(struct espress_stream_event *this, struct espress_stream_event_flags *flags, struct espress_stream_uint32 const *timeout)
{
	struct espress_stream_uint32 start[1];
	struct espress_stream_uint32 t[1];
	struct espress_stream_uint32 u[1];
	espress_stream_cond->wait_lock(this->cond);
	espress_stream_tick->get_us(start);
	espress_stream_uint32_assign(t, timeout);
	for ( ; ; )
	{
		espress_stream_cond->wait(this->cond, t);
		if (__is_set_any(this->flags))
		{
			__reset_all(flags);
			__compo(flags, this->flags);
			__reset_all(this->flags);
			break;
		}
		espress_stream_tick->get_us(u);
		espress_stream_uint32_sub(u, start);
		if (espress_stream_uint32_le(timeout, u))
		{
			__reset_all(flags);
			break;
		}
		espress_stream_uint32_assign(t, timeout);
		espress_stream_uint32_sub(t, u);
	}
	espress_stream_cond->wait_unlock(this->cond);
}

static i_t _poll(struct espress_stream_event * const *these, u_t num, struct espress_stream_uint32 const *timeout)
{
	i_t ret;
	i_t i;
#	if 0 < ESPRESS_STREAM_EVENT_MAX_NUM_POLL_EVENT
		struct espress_stream_cond *cond[ESPRESS_STREAM_EVENT_MAX_NUM_POLL_EVENT];

		if (ESPRESS_STREAM_EVENT_MAX_NUM_POLL_EVENT < num)
		{
			ret = -1;
			goto EXIT;
		}
#	else
		struct espress_stream_cond **cond;

		cond = (struct espress_stream_cond **)calloc(num, sizeof(struct espress_stream_cond *));
		if (NULL == cond)
		{
			ret = -1;
			goto EXIT;
		}
#	endif

	for (i = 0, ret = 0 ; i < num ; i++)
	{
		espress_stream_cond->signal_lock(these[i]->cond);
		cond[i] = these[i]->cond;
		if (__is_set_any(these[i]->flags))
		{
			ret++;
		}
		espress_stream_cond->signal_unlock(these[i]->cond);
	}

	if (0 == ret)
	{
		ret = espress_stream_cond->poll(cond, num, timeout);
	}

EXIT:
#	if 0 == ESPRESS_STREAM_EVENT_MAX_NUM_POLL_EVENT
		if (NULL != cond) free(cond);
#	endif

	return ret;
}