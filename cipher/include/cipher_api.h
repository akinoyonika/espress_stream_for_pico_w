#define CIPHER_ED25519_SECRET_KEY_SIZE (64)
#define CIPHER_ED25519_PUBLIC_KEY_SIZE (32)
#define CIPHER_ED25519_SIGNATURE_SIZE (64)

//void cipher_ed25519_get_key(unsigned char *secret_key, unsigned char *public_key);
//void cipher_ed25519_sign(unsigned char *signature, unsigned char const *secret_key, unsigned char const *message, unsigned message_size);
int cipher_ed25519_auth(unsigned char *signature, unsigned char const *public_key, unsigned char const *message, unsigned message_size);
