#include "espress_cipher/depend.h"
#include "espress_cipher/uint.h"
#include "espress_cipher/sha512.h"
#include "espress_cipher/ed25519.h"
#include "cipher_api.h"

/*void cipher_ed25519_get_key(unsigned char *secret_key, unsigned char *public_key)
{
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
	} while(0)

	get_random(secret_key, CIPHER_ED25519_SECRET_KEY_SIZE - CIPHER_ED25519_PUBLIC_KEY_SIZE);
	espress_cipher_ed25519->get_public_key(public_key, secret_key);
	memcpy(secret_key + (CIPHER_ED25519_SECRET_KEY_SIZE - CIPHER_ED25519_PUBLIC_KEY_SIZE), public_key, CIPHER_ED25519_PUBLIC_KEY_SIZE);
}*/

/*void cipher_ed25519_sign(unsigned char *signature, unsigned char const *secret_key, unsigned char const *message, unsigned message_size)
{
	espress_cipher_ed25519->sign(signature, secret_key, secret_key + (CIPHER_ED25519_SECRET_KEY_SIZE - CIPHER_ED25519_PUBLIC_KEY_SIZE), message, message_size);
}*/

int cipher_ed25519_auth(unsigned char *signature, unsigned char const *public_key, unsigned char const *message, unsigned message_size)
{
	return espress_cipher_ed25519->auth(signature, public_key, message, message_size);
}
