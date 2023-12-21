#ifndef __ESPRESS_CIPHER_ED25519_H__
#define __ESPRESS_CIPHER_ED25519_H__

#define ESPRESS_CIPHER_ED25519_KEY_SIZE (32)
#define ESPRESS_CIPHER_ED25519_SIGNATURE_SIZE (64)

struct espress_cipher_ed25519_ops
{
	void (*get_public_key)(b_t *public_key, b_t const *secret_key);
	void (*sign)(b_t *signature, b_t const *secret_key, b_t const *public_key, b_t const *message, u_t message_size);
	i_t (*auth)(b_t const *signature, b_t const *public_key, b_t const *message, u_t message_size);
};

extern struct espress_cipher_ed25519_ops const espress_cipher_ed25519[];

#endif /* __ESPRESS_CIPHER_ED25519_H__ */
