#include "espress_stream/config.h"
#include "espress_stream/depend.h"
#include "espress_stream/uint.h"
#include "espress_stream/tick.h"
#include "espress_stream/mutex.h"
#include "espress_stream/cond.h"
#include "espress_stream/event.h"
#include "espress_stream/timer.h"

static i_t _initialize(struct espress_stream_timer *this);
static void _finalize(struct espress_stream_timer *this);
static i_t _start(struct espress_stream_timer *this,
	void (*callback)(struct espress_stream_timer *this, void *callback_param),
	void *param,
	struct espress_stream_uint32 const *timeout);
static i_t _cancel(struct espress_stream_timer *this);
static void _yield(struct espress_stream_timer *this, struct espress_stream_uint32 const *timeout);
static void _yield_break(struct espress_stream_timer *this);

struct espress_stream_timer_op const espress_stream_timer[] =
{{
	_initialize,
	_finalize,
	_start,
	_cancel,
	_yield,
	_yield_break
}};

static i_t _initialize(struct espress_stream_timer *this)
{
	this->is_active = 0;
	espress_stream_tick->get_us(this->start_time);
	espress_stream_uint32_assign(this->timeout, espress_stream_uint32_0);
	return espress_stream_cond->initialize(this->cond);
}

static void _finalize(struct espress_stream_timer *this)
{
	espress_stream_cond->finalize(this->cond);
	memset(this, 0, sizeof(*this));
}

static i_t _start(struct espress_stream_timer *this,
	void (*callback)(struct espress_stream_timer *this, void *param),
	void *callback_param,
	struct espress_stream_uint32 const *timeout)
{
	i_t ret = 0;

	espress_stream_cond->signal_lock(this->cond);

	if (this->is_active)
	{
		ret = 1;
		goto ALREADY_ACTIVE_EXIT;
	}

	this->callback = callback;
	this->callback_param = callback_param;
	espress_stream_tick->get_us(this->start_time);
	espress_stream_uint32_assign(this->timeout, timeout);
	this->is_active = 1;
	espress_stream_cond->signal(this->cond);

ALREADY_ACTIVE_EXIT:

	espress_stream_cond->signal_unlock(this->cond);

	return ret;
}

static i_t _cancel(struct espress_stream_timer *this)
{
	i_t ret = 0;

	espress_stream_cond->signal_lock(this->cond);

	if (!this->is_active)
	{
		goto NOT_START_EXIT;
	}

	this->is_active = 0;
	espress_stream_cond->signal(this->cond);

NOT_START_EXIT:

	espress_stream_cond->signal_unlock(this->cond);

	return ret;
}

static void _yield(struct espress_stream_timer *this, struct espress_stream_uint32 const *timeout)
{
	struct espress_stream_uint32 t[1];
	struct espress_stream_uint32 u[1];
	void (*callback)(struct espress_stream_timer *this, void *param) = NULL;

	espress_stream_cond->signal_lock(this->cond);

	if (this->is_active)
	{
		espress_stream_tick->get_us(u);
		espress_stream_uint32_sub(u, this->start_time);
		if (espress_stream_uint32_lt(u, this->timeout))
		{
			espress_stream_uint32_assign(t, this->timeout);
			espress_stream_uint32_sub(t, u);
			if (espress_stream_uint32_lt(timeout, t))
			{
				espress_stream_cond->wait(this->cond, timeout);
			}
			else
			{
				if ((0 == espress_stream_cond->wait(this->cond, t)) && this->is_active)
				{
					this->is_active = 0;
					callback = this->callback;
				}
			}
		}
		else
		{
			this->is_active = 0;
			callback = this->callback;
		}
	}
	else
	{
		espress_stream_cond->wait(this->cond, timeout);
	}

	espress_stream_cond->wait_unlock(this->cond);

	if (NULL != callback)
	{
		callback(this, this->callback_param);
	}
}

static void _yield_break(struct espress_stream_timer *this)
{
	espress_stream_cond->signal_lock(this->cond);
	espress_stream_cond->signal(this->cond);
	espress_stream_cond->signal_unlock(this->cond);
}
