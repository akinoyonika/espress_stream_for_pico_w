#include "espress_stream/config.h"
#include "espress_stream/depend.h"
#include "espress_stream/uint.h"
#include "espress_stream/ec.h"
#include "espress_stream/key_ex.h"

static void _get_public_key(b_t *public_key, b_t const *private_key);
static void _get_shared_key(b_t *shared_key, b_t const *private_key, b_t const *public_key);

struct espress_stream_key_ex_ops const espress_stream_key_ex[] =
{{
	_get_public_key,
	_get_shared_key
}};

static void _get_public_key(b_t *public_key, b_t const *private_key)
{
	b_t b[ESPRESS_STREAM_KEY_EX_KEY_SIZE];
	memcpy(b, private_key, ESPRESS_STREAM_KEY_EX_KEY_SIZE);
	memset(public_key, 0, ESPRESS_STREAM_KEY_EX_KEY_SIZE);
	public_key[0] = 9;
	espress_stream_ec->prepare_key(b);
	espress_stream_ec->smul(public_key, b);
}

static void _get_shared_key(b_t *shared_key, b_t const *private_key, b_t const *public_key)
{
	b_t b[ESPRESS_STREAM_KEY_EX_KEY_SIZE];
	memcpy(b, private_key, ESPRESS_STREAM_KEY_EX_KEY_SIZE);
	espress_stream_ec->prepare_key(b);
	memcpy(shared_key, public_key, ESPRESS_STREAM_KEY_EX_KEY_SIZE);
	espress_stream_ec->smul(shared_key, b);
}
