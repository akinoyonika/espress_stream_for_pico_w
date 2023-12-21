#include "espress_stream/config.h"
#include "espress_stream/depend.h"
#include "espress_stream/uint.h"
#include "espress_stream/mutex.h"
#include "espress_stream/block_cipher.h"
#include "espress_stream/ctr_cipher.h"
#include "espress_stream/hash.h"
#include "espress_stream/mac.h"
#include "espress_stream/hkdf.h"
#include "espress_stream/key_exp.h"
#include "espress_stream/key.h"

#define _fwd_key_idx(i) ((i) = ((i) + 1) % 3)
#define _prv_key_idx(i) (i)
#define _cur_key_idx(i) (((i) + 1) % 3)
#define _nxt_key_idx(i) (((i) + 2) % 3)

static void _initialize(struct espress_stream_key *this, b_t const *key, u_t key_size, b_t const *salt, u_t salt_size);
static void _finalize(struct espress_stream_key *this);
static i_t _get(struct espress_stream_key *this, b_t *mac_key, b_t *cipher_key, struct espress_stream_uint32 const *counter_num);
static i_t _update(struct espress_stream_key *this, struct espress_stream_uint32 const *counter_num);

struct espress_stream_key_ops const espress_stream_key[] =
{{
	_initialize,
	_finalize,
	_get,
	_update
}};

static i_t _uint32_lt(struct espress_stream_uint32 const *this, struct espress_stream_uint32 const *n)
{
	struct espress_stream_uint32 u[1];
	espress_stream_uint32_assign(u, this);
	espress_stream_uint32_sub(u, n);
	espress_stream_uint32_rotate(u, 1);
	espress_stream_uint32_and(u, espress_stream_uint32_1);
	return espress_stream_uint32_eq(u, espress_stream_uint32_1);
}

static void _initialize(struct espress_stream_key *this, b_t const *key, u_t key_size, b_t const *salt, u_t salt_size)
{
	this->is_valid = (0 == espress_stream_mutex->initialize(this->mutex));
	if (!this->is_valid)
	{
		goto EXIT;
	}
	espress_stream_key_exp->initialize(this->key_exp, key, key_size, salt, salt_size);
	this->key_idx = 0;
	espress_stream_key_exp->expand(this->key_exp,
		this->mac_key[_nxt_key_idx(this->key_idx)],
		ESPRESS_STREAM_KEY_MAC_KEY_SIZE,
		(b_t const *)"",
		0);
	espress_stream_key_exp->expand(this->key_exp,
		this->cipher_key[_nxt_key_idx(this->key_idx)],
		ESPRESS_STREAM_KEY_CIPHER_KEY_SIZE,
		(b_t const *)"",
		0);
	espress_stream_key_history_uint_(assign)(this->checks, espress_stream_key_history_uint_(max));
	espress_stream_uint32_assign(this->current, espress_stream_uint32_max);
EXIT:
	return;
}

static void _finalize(struct espress_stream_key *this)
{
	if (!this->is_valid)
	{
		goto EXIT;
	}
	espress_stream_mutex->finalize(this->mutex);
	espress_stream_key_exp->finalize(this->key_exp);
	memset(this, 0, sizeof(*this));
EXIT:
	return;
}

static void _check(struct espress_stream_key *this,
	i_t *is_counter_forward,
	i_t *is_counter_flow,
	i_t *is_counter_valid,
	struct espress_stream_uint32 *distance_counter,
	struct espress_stream_key_history_uint *check,
	struct espress_stream_uint32 const *counter_num)
{
	i_t i;
	struct espress_stream_key_history_uint lu[1];
	i_t is_0_forward;
	struct espress_stream_uint32 distance_0[1];
	struct espress_stream_uint32 const W[1] = {ESPRESS_STREAM_UINT32(0x00, 0x00, 0x00, ESPRESS_STREAM_KEY_HISTORY_SIZE)};

	*is_counter_forward = _uint32_lt(this->current, counter_num);
	if (*is_counter_forward)
	{
		espress_stream_uint32_assign(distance_counter, counter_num);
		espress_stream_uint32_sub(distance_counter, this->current);
	}
	else
	{
		espress_stream_uint32_assign(distance_counter, this->current);
		espress_stream_uint32_sub(distance_counter, counter_num);
	}
	is_0_forward = _uint32_lt(this->current, espress_stream_uint32_0);
	if (is_0_forward)
	{
		espress_stream_uint32_assign(distance_0, espress_stream_uint32_0);
		espress_stream_uint32_sub(distance_0, this->current);
	}
	else
	{
		espress_stream_uint32_assign(distance_0, this->current);
		espress_stream_uint32_sub(distance_0, espress_stream_uint32_0);
	}
	*is_counter_flow =
		(*is_counter_forward && is_0_forward && espress_stream_uint32_le(distance_0, distance_counter)) ||
		(!*is_counter_forward && !is_0_forward && espress_stream_uint32_lt(distance_0, distance_counter));
	if (*is_counter_forward)
	{
		*is_counter_valid = 1;
	}
	else
	{
		if (espress_stream_uint32_lt(distance_counter, W))
		{
			espress_stream_uint32_get_u(distance_counter, (u_t *)&i);
			espress_stream_key_history_uint_(assign)(check, espress_stream_key_history_uint_(1));
			espress_stream_key_history_uint_(shift)(check, i);
			espress_stream_key_history_uint_(assign)(lu, this->checks);
			espress_stream_key_history_uint_(and)(lu, check);
			*is_counter_valid = espress_stream_key_history_uint_(eq)(lu, espress_stream_key_history_uint_(0));
		}
		else
		{
			*is_counter_valid = 0;
		}
	}
}

static i_t _get(struct espress_stream_key *this, b_t *mac_key, b_t *cipher_key, struct espress_stream_uint32 const *counter_num)
{
	i_t is_counter_forward;
	i_t is_counter_flow;
	i_t is_counter_valid;
	struct espress_stream_uint32 distance_counter[1];
	struct espress_stream_key_history_uint check[1];

	if (!this->is_valid)
	{
		is_counter_valid = 0;
		goto EXIT;
	}

	espress_stream_mutex->lock(this->mutex);

	_check(this, &is_counter_forward, &is_counter_flow, &is_counter_valid, distance_counter, check, counter_num);

	if (is_counter_valid)
	{
		if (is_counter_forward)
		{
			if (is_counter_flow)
			{
				memcpy(mac_key, this->mac_key[_nxt_key_idx(this->key_idx)], ESPRESS_STREAM_KEY_MAC_KEY_SIZE);
				memcpy(cipher_key, this->cipher_key[_nxt_key_idx(this->key_idx)], ESPRESS_STREAM_KEY_CIPHER_KEY_SIZE);
			}
			else
			{
				memcpy(mac_key, this->mac_key[_cur_key_idx(this->key_idx)], ESPRESS_STREAM_KEY_MAC_KEY_SIZE);
				memcpy(cipher_key, this->cipher_key[_cur_key_idx(this->key_idx)], ESPRESS_STREAM_KEY_CIPHER_KEY_SIZE);
			}
		}
		else
		{
			if (is_counter_flow)
			{
				memcpy(mac_key, this->mac_key[_prv_key_idx(this->key_idx)], ESPRESS_STREAM_KEY_MAC_KEY_SIZE);
				memcpy(cipher_key, this->cipher_key[_prv_key_idx(this->key_idx)], ESPRESS_STREAM_KEY_CIPHER_KEY_SIZE);
			}
			else
			{
				memcpy(mac_key, this->mac_key[_cur_key_idx(this->key_idx)], ESPRESS_STREAM_KEY_MAC_KEY_SIZE);
				memcpy(cipher_key, this->cipher_key[_cur_key_idx(this->key_idx)], ESPRESS_STREAM_KEY_CIPHER_KEY_SIZE);
			}
		}
	}

	espress_stream_mutex->unlock(this->mutex);

EXIT:

	return is_counter_valid;
}

static i_t _update(struct espress_stream_key *this, struct espress_stream_uint32 const *counter_num)
{
	i_t i;
	i_t is_counter_forward;
	i_t is_counter_flow;
	i_t is_counter_valid;
	struct espress_stream_uint32 distance_counter[1];
	struct espress_stream_key_history_uint check[1];
	struct espress_stream_uint32 const W[1] = {ESPRESS_STREAM_UINT32(0x00, 0x00, 0x00, ESPRESS_STREAM_KEY_HISTORY_SIZE)};

	if (!this->is_valid)
	{
		is_counter_valid = 0;
		goto EXIT;
	}

	espress_stream_mutex->lock(this->mutex);

	_check(this, &is_counter_forward, &is_counter_flow, &is_counter_valid, distance_counter, check, counter_num);

	if (is_counter_valid)
	{
		if (is_counter_forward)
		{
			if (espress_stream_uint32_lt(distance_counter, W))
			{
				espress_stream_uint32_get_u(distance_counter, (u_t *)&i);
				espress_stream_key_history_uint_(shift)(this->checks, i);
			}
			else
			{
				espress_stream_key_history_uint_(assign)(this->checks, espress_stream_key_history_uint_(0));
			}
			espress_stream_uint32_assign(this->current, counter_num);
			espress_stream_key_history_uint_(or)(this->checks, espress_stream_key_history_uint_(1));

			if (is_counter_flow)
			{
				_fwd_key_idx(this->key_idx);
				espress_stream_key_exp->expand(this->key_exp,
					this->mac_key[_nxt_key_idx(this->key_idx)],
					ESPRESS_STREAM_KEY_MAC_KEY_SIZE,
					(b_t const *)"",
					0);
				espress_stream_key_exp->expand(this->key_exp,
					this->cipher_key[_nxt_key_idx(this->key_idx)],
					ESPRESS_STREAM_KEY_CIPHER_KEY_SIZE,
					(b_t const *)"",
					0);
			}
		}
		else
		{
			espress_stream_key_history_uint_(or)(this->checks, check);
		}
	}

	espress_stream_mutex->unlock(this->mutex);

EXIT:

	return is_counter_valid;
}
