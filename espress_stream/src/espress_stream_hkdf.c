#include "espress_stream/config.h"
#include "espress_stream/depend.h"
#include "espress_stream/uint.h"
#include "espress_stream/hash.h"
#include "espress_stream/mac.h"
#include "espress_stream/hkdf.h"

static void _extract(b_t *prk, b_t const *salt, u_t salt_size, b_t const *ikm, u_t ikm_size);
static void _expand(b_t *t1, u_t t1_size, b_t const *prk, b_t const *info, u_t info_size, b_t count);

struct espress_stream_hkdf_ops const espress_stream_hkdf[] =
{{
	_extract,
	_expand
}};

static void _extract(b_t *prk, b_t const *salt, u_t salt_size, b_t const *ikm, u_t ikm_size)
{
	struct espress_stream_mac mac[1];

	espress_stream_mac->initialize(mac, salt, salt_size);
	espress_stream_mac->update(mac, ikm, ikm_size);
	espress_stream_mac->finalize(mac, prk);
}

static void _expand(b_t *t1, u_t t1_size, b_t const *prk, b_t const *info, u_t info_size, b_t count)
{
	struct espress_stream_mac mac[1];

	espress_stream_mac->initialize(mac, prk, ESPRESS_STREAM_HKDF_HASH_SIZE);
	espress_stream_mac->update(mac, t1, t1_size);
	espress_stream_mac->update(mac, info, info_size);
	espress_stream_mac->update(mac, &count, 1);
	espress_stream_mac->finalize(mac, t1);
}
