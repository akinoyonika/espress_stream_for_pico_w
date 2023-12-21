#include "espress_stream/config.h"
#include "espress_stream/depend.h"
#include "espress_stream/uint.h"
#include "espress_stream/tick.h"
#include "espress_stream/mutex.h"
#include "espress_stream/cond.h"

static i_t _initialize(struct espress_stream_cond *this);
static void _finalize(struct espress_stream_cond *this);
static void _signal_lock(struct espress_stream_cond *this);
static void _signal_unlock(struct espress_stream_cond *this);
static void _signal(struct espress_stream_cond *this);
static void _wait_lock(struct espress_stream_cond *this);
static void _wait_unlock(struct espress_stream_cond *this);
static i_t _wait(struct espress_stream_cond *this, struct espress_stream_uint32 const *timeout);
static i_t _poll(struct espress_stream_cond * const *these, u_t num, struct espress_stream_uint32 const *timeout);

struct espress_stream_cond_ops const espress_stream_cond[] =
{{
	_initialize,
	_finalize,
	_signal_lock,
	_signal_unlock,
	_signal,
	_wait_lock,
	_wait_unlock,
	_wait,
	_poll
}};

static i_t _initialize(struct espress_stream_cond *this)
{
	i_t ret = 0;
	if (0 != espress_stream_mutex->initialize(this->mutex))
	{
		ret = 1;
		goto EXIT;
	}
	this->cond = 0;
	this->is_signaled = 0;
EXIT:
	return ret;
}

static void _finalize(struct espress_stream_cond *this)
{
	espress_stream_mutex->finalize(this->mutex);
	memset(this, 0, sizeof(*this));
}

static void _signal_lock(struct espress_stream_cond *this)
{
	espress_stream_mutex->lock(this->mutex);
}

static void _signal_unlock(struct espress_stream_cond *this)
{
	espress_stream_mutex->unlock(this->mutex);
}

static void _signal(struct espress_stream_cond *this)
{
	this->is_signaled = 1;
	this->cond = 1;
}

static void _wait_lock(struct espress_stream_cond *this)
{
	espress_stream_mutex->lock(this->mutex);
}

static void _wait_unlock(struct espress_stream_cond *this)
{
	espress_stream_mutex->unlock(this->mutex);
}

static i_t _wait(struct espress_stream_cond *this, struct espress_stream_uint32 const *timeout)
{
	i_t ret;
	struct espress_stream_uint32 start[1];
	struct espress_stream_uint32 u[1] = {ESPRESS_STREAM_UINT32_0};

	espress_stream_tick->get_us(start);
	while (!this->is_signaled && espress_stream_uint32_lt(u, timeout))
	{
		espress_stream_mutex->unlock(this->mutex);

		while ((0 == this->cond) && espress_stream_uint32_lt(u, timeout))
		{
			espress_stream_tick->get_us(u);
			espress_stream_uint32_sub(u, start);
		}
		this->cond = 0;

		espress_stream_mutex->lock(this->mutex);
	}

	ret = this->is_signaled;
	this->is_signaled = 0;

	return ret;
}

static i_t _poll(struct espress_stream_cond * const *these, u_t num, struct espress_stream_uint32 const *timeout)
{
	i_t i;
	i_t ret = 0;
	struct espress_stream_uint32 start[1];
	struct espress_stream_uint32 u[1] = {ESPRESS_STREAM_UINT32_0};

	espress_stream_tick->get_us(start);
	while ((0 == ret) && espress_stream_uint32_lt(u, timeout))
	{
		for (i = 0 ; i < num ; i++)
		{
			if (these[i]->is_signaled)
			{
				ret++;
			}
		}
		espress_stream_tick->get_us(u);
		espress_stream_uint32_sub(u, start);
	}

	return ret;
}
