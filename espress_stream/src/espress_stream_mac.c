#include "espress_stream/config.h"
#include "espress_stream/depend.h"
#include "espress_stream/uint.h"
#include "espress_stream/hash.h"
#include "espress_stream/mac.h"

static void _initialize(struct espress_stream_mac *this, b_t const *key, u_t key_size);
static void _finalize(struct espress_stream_mac *this, b_t *mac);
static void _update(struct espress_stream_mac *this, b_t const *message, u_t size);

struct espress_stream_mac_ops const espress_stream_mac[] =
{{
	_initialize,
	_finalize,
	_update
}};

static struct espress_stream_uint512 const _ipad[1] = {ESPRESS_STREAM_UINT512(
	0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36,
	0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36,
	0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36,
	0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36)};
static struct espress_stream_uint512 const _opad[1] = {ESPRESS_STREAM_UINT512(
	0x5c, 0x5c, 0x5c, 0x5c, 0x5c, 0x5c, 0x5c, 0x5c, 0x5c, 0x5c, 0x5c, 0x5c, 0x5c, 0x5c, 0x5c, 0x5c,
	0x5c, 0x5c, 0x5c, 0x5c, 0x5c, 0x5c, 0x5c, 0x5c, 0x5c, 0x5c, 0x5c, 0x5c, 0x5c, 0x5c, 0x5c, 0x5c,
	0x5c, 0x5c, 0x5c, 0x5c, 0x5c, 0x5c, 0x5c, 0x5c, 0x5c, 0x5c, 0x5c, 0x5c, 0x5c, 0x5c, 0x5c, 0x5c,
	0x5c, 0x5c, 0x5c, 0x5c, 0x5c, 0x5c, 0x5c, 0x5c, 0x5c, 0x5c, 0x5c, 0x5c, 0x5c, 0x5c, 0x5c, 0x5c)};

static void _initialize(struct espress_stream_mac *this, b_t const *key, u_t key_size)
{
	struct espress_stream_uint512 kexi[1];

	if (ESPRESS_STREAM_MAC_BLOCK_SIZE < key_size)
	{
		espress_stream_hash->initialize(this->hash);
		espress_stream_hash->update(this->hash, key, key_size);
		espress_stream_hash->finalize(this->hash, this->key);
		key_size = ESPRESS_STREAM_HASH_DIGEST_SIZE;
	}
	else
	{
		memcpy(this->key , key, key_size);
	}
	memset(this->key + key_size, 0, ESPRESS_STREAM_MAC_BLOCK_SIZE - key_size);
	memcpy(kexi, this->key, ESPRESS_STREAM_MAC_BLOCK_SIZE);
	espress_stream_uint512_xor(kexi, _ipad);
	espress_stream_hash->initialize(this->hash);
	espress_stream_hash->update(this->hash, (b_t *)kexi, ESPRESS_STREAM_MAC_BLOCK_SIZE);
}

static void _finalize(struct espress_stream_mac *this, b_t *mac)
{
	struct espress_stream_uint512 kexp[1];

	espress_stream_hash->finalize(this->hash, mac);
	memcpy(kexp, this->key, ESPRESS_STREAM_MAC_BLOCK_SIZE);
	espress_stream_uint512_xor(kexp, _opad);
	espress_stream_hash->initialize(this->hash);
	espress_stream_hash->update(this->hash, (b_t *)kexp, ESPRESS_STREAM_MAC_BLOCK_SIZE);
	espress_stream_hash->update(this->hash, mac, ESPRESS_STREAM_MAC_MAC_SIZE);
	espress_stream_hash->finalize(this->hash, mac);
}

static void _update(struct espress_stream_mac *this, b_t const *message, u_t size)
{
	espress_stream_hash->update(this->hash, message, size);
}
