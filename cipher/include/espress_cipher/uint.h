#ifndef __ESPRESS_CIPHER_UINT_H__
#define __ESPRESS_CIPHER_UINT_H__

#define ESPRESS_CIPHER_UINT_MAX_WIDTH (128)

#pragma pack(push, 1)
struct espress_cipher_uint8
{
	u8_t u;
};
#pragma pack(pop)

static inline void espress_cipher_uint8_set(struct espress_cipher_uint8 *this, b_t n0)
{
	this->u = n0;
}
static inline void espress_cipher_uint8_get(struct espress_cipher_uint8 const *this, b_t *n0)
{
	*n0 = this->u;
}
static inline void espress_cipher_uint8_va_set(struct espress_cipher_uint8 *this, va_list *np)
{
	b_t const n0 = va_arg(*np, int);
	espress_cipher_uint8_set(this, n0);
}
static inline void espress_cipher_uint8_va_get(struct espress_cipher_uint8 const *this, va_list *np)
{
	b_t * const n0 = va_arg(*np, b_t *);
	espress_cipher_uint8_get(this, n0);
}
static inline void espress_cipher_uint8_set_u(struct espress_cipher_uint8 *this, u_t n){this->u = n;}
static inline void espress_cipher_uint8_get_u(struct espress_cipher_uint8 const *this, u_t *n){*n = this->u;}
static inline void espress_cipher_uint8_assign(struct espress_cipher_uint8 *this, struct espress_cipher_uint8 const *n){this->u = n->u;}
static inline i_t espress_cipher_uint8_add(struct espress_cipher_uint8 *this, struct espress_cipher_uint8 const *n){this->u = this->u + n->u; return this->u < n->u;}
static inline i_t espress_cipher_uint8_sub(struct espress_cipher_uint8 *this, struct espress_cipher_uint8 const *n){i_t const is_uf = this->u < n ->u; this->u = this->u - n->u; return is_uf;}
static inline void espress_cipher_uint8_mul(struct espress_cipher_uint8 *this, struct espress_cipher_uint8 const *n){this->u *= n->u;}
static inline void espress_cipher_uint8_div(struct espress_cipher_uint8 *this, struct espress_cipher_uint8 const *n){this->u /= n->u;}
static inline void espress_cipher_uint8_and(struct espress_cipher_uint8 *this, struct espress_cipher_uint8 const *n){this->u &= n->u;}
static inline void espress_cipher_uint8_or(struct espress_cipher_uint8 *this, struct espress_cipher_uint8 const *n){this->u |= n->u;}
static inline void espress_cipher_uint8_xor(struct espress_cipher_uint8 *this, struct espress_cipher_uint8 const *n){this->u ^= n->u;}
static inline void espress_cipher_uint8_not(struct espress_cipher_uint8 *this){this->u = ~(this->u);}
static inline void espress_cipher_uint8_shift(struct espress_cipher_uint8 *this, i_t n)
{
	if (n < 0)
	{
		this->u >>=  -n;
	}
	else if (0 < n)
	{
		this->u <<= n;
	}
}
static inline void espress_cipher_uint8_rotate(struct espress_cipher_uint8 *this, i_t n)
{
	i_t m = n % 8;
	u8_t u;
	if (m < 0)
	{
		u = this->u << (8 + m);
		this->u = (this->u >> -m) + u;
	}
	else if (0 < m)
	{
		u = this->u >> (8 - m);
		this->u = (this->u << m) + u;
	}
}
static inline i_t espress_cipher_uint8_eq(struct espress_cipher_uint8 const *this, struct espress_cipher_uint8 const *n){return this->u == n->u;}
static inline i_t espress_cipher_uint8_lt(struct espress_cipher_uint8 const *this, struct espress_cipher_uint8 const *n){return this->u < n->u;}
static inline i_t espress_cipher_uint8_le(struct espress_cipher_uint8 const *this, struct espress_cipher_uint8 const *n){return this->u <= n->u;}
static inline void espress_cipher_uint8_bswap(struct espress_cipher_uint8 *this){}

#define ESPRESS_CIPHER_UINT8(n0) {(u8_t)(n0)}

extern struct espress_cipher_uint8 const espress_cipher_uint8_0[];
extern struct espress_cipher_uint8 const espress_cipher_uint8_1[];
extern struct espress_cipher_uint8 const espress_cipher_uint8_max[];

#define ESPRESS_CIPHER_UINT8_0 ESPRESS_CIPHER_UINT8(0)
#define ESPRESS_CIPHER_UINT8_1 ESPRESS_CIPHER_UINT8(1)
#define ESPRESS_CIPHER_UINT8_MAX ESPRESS_CIPHER_UINT8(0xff)
#define espress_cipher_uint8_htol(this) ((void)(this))
#define espress_cipher_uint8_htob(this) espress_cipher_uint8_bswap(this)
#define espress_cipher_uint8_ltoh(this) espress_cipher_uint8_htol(this)
#define espress_cipher_uint8_btoh(this) espress_cipher_uint8_htob(this)
#define espress_cipher_uint8_hton(this) espress_cipher_uint8_htol(this)
#define espress_cipher_uint8_ntoh(this) espress_cipher_uint8_hton(this)

#if 2 <= ESPRESS_CIPHER_UINT_MAX_WIDTH
#	if 2 <= MAX_U_T_WIDTH
#		pragma pack(push, 1)
		struct espress_cipher_uint16
		{
			u16_t u;
		};
#		pragma pack(pop)

		static inline void espress_cipher_uint16_set(struct espress_cipher_uint16 *this, b_t n1, b_t n0)
		{
			this->u = (u16_t)n0 + ((u16_t)n1 << 8);
		}
		static inline void espress_cipher_uint16_get(struct espress_cipher_uint16 const *this, b_t *n1, b_t *n0)
		{
			*n0 = this->u;
			*n1 = this->u >> 8;
		}
		static inline void espress_cipher_uint16_va_set(struct espress_cipher_uint16 *this, va_list *np)
		{
			b_t const n1 = va_arg(*np, int);
			b_t const n0 = va_arg(*np, int);
			espress_cipher_uint16_set(this, n1, n0);
		}
		static inline void espress_cipher_uint16_va_get(struct espress_cipher_uint16 const *this, va_list *np)
		{
			b_t * const n1 = va_arg(*np, b_t *);
			b_t * const n0 = va_arg(*np, b_t *);
			espress_cipher_uint16_get(this, n1, n0);
		}
		static inline void espress_cipher_uint16_set_u(struct espress_cipher_uint16 *this, u_t n){this->u = n;}
		static inline void espress_cipher_uint16_get_u(struct espress_cipher_uint16 const *this, u_t *n){*n = this->u;}
		static inline void espress_cipher_uint16_assign(struct espress_cipher_uint16 *this, struct espress_cipher_uint16 const *n){this->u = n->u;}
		static inline i_t espress_cipher_uint16_add(struct espress_cipher_uint16 *this, struct espress_cipher_uint16 const *n){this->u = this->u + n->u; return this->u < n->u;}
		static inline i_t espress_cipher_uint16_sub(struct espress_cipher_uint16 *this, struct espress_cipher_uint16 const *n){i_t const is_uf = this->u < n ->u; this->u = this->u - n->u; return is_uf;}
		static inline void espress_cipher_uint16_mul(struct espress_cipher_uint16 *this, struct espress_cipher_uint16 const *n){this->u *= n->u;}
		static inline void espress_cipher_uint16_div(struct espress_cipher_uint16 *this, struct espress_cipher_uint16 const *n){this->u /= n->u;}
		static inline void espress_cipher_uint16_and(struct espress_cipher_uint16 *this, struct espress_cipher_uint16 const *n){this->u &= n->u;}
		static inline void espress_cipher_uint16_or(struct espress_cipher_uint16 *this, struct espress_cipher_uint16 const *n){this->u |= n->u;}
		static inline void espress_cipher_uint16_xor(struct espress_cipher_uint16 *this, struct espress_cipher_uint16 const *n){this->u ^= n->u;}
		static inline void espress_cipher_uint16_not(struct espress_cipher_uint16 *this){this->u = ~(this->u);}
		static inline void espress_cipher_uint16_shift(struct espress_cipher_uint16 *this, i_t n)
		{
			if (n < 0)
			{
				this->u >>= -n;
			}
			else if (0 < n)
			{
				this->u <<= n;
			}
		}
		static inline void espress_cipher_uint16_rotate(struct espress_cipher_uint16 *this, i_t n)
		{
			i_t m = n % 16;
			u16_t u;
			if (m < 0)
			{
				u = this->u << (16 + m);
				this->u = (this->u >> -m) + u;
			}
			else if (0 < m)
			{
				u = this->u >> (16 - m);
				this->u = (this->u << m) + u;
			}
		}
		static inline i_t espress_cipher_uint16_eq(struct espress_cipher_uint16 const *this, struct espress_cipher_uint16 const *n){return this->u == n->u;}
		static inline i_t espress_cipher_uint16_lt(struct espress_cipher_uint16 const *this, struct espress_cipher_uint16 const *n){return this->u < n->u;}
		static inline i_t espress_cipher_uint16_le(struct espress_cipher_uint16 const *this, struct espress_cipher_uint16 const *n){return this->u <= n->u;}
		static inline void espress_cipher_uint16_bswap(struct espress_cipher_uint16 *this){swap_endian_16(&(this->u));}

#		define ESPRESS_CIPHER_UINT16(n1, n0) {(u16_t)(n0) + ((u16_t)(n1) << 8)}
#	else
#		pragma pack(push, 1)
		struct espress_cipher_uint16
		{
#		if ESPRESS_CIPHER_IS_HOST_BYTE_ORDER_LITTLE
			struct espress_cipher_uint8 l;
			struct espress_cipher_uint8 h;
#		else
			struct espress_cipher_uint8 h;
			struct espress_cipher_uint8 l;
#		endif
		};
#		pragma pack(pop)

		void espress_cipher_uint16_set(struct espress_cipher_uint16 *this, b_t n1, b_t n0);
		void espress_cipher_uint16_get(struct espress_cipher_uint16 const *this, b_t *n1, b_t *n0);
		void espress_cipher_uint16_va_set(struct espress_cipher_uint16 *this, va_list *np);
		void espress_cipher_uint16_va_get(struct espress_cipher_uint16 const *this, va_list *np);
		void espress_cipher_uint16_set_u(struct espress_cipher_uint16 *this, u_t n);
		void espress_cipher_uint16_get_u(struct espress_cipher_uint16 const *this, u_t *n);
		void espress_cipher_uint16_assign(struct espress_cipher_uint16 *this, struct espress_cipher_uint16 const *n);
		i_t espress_cipher_uint16_add(struct espress_cipher_uint16 *this, struct espress_cipher_uint16 const *n);
		i_t espress_cipher_uint16_sub(struct espress_cipher_uint16 *this, struct espress_cipher_uint16 const *n);
		void espress_cipher_uint16_mul(struct espress_cipher_uint16 *this, struct espress_cipher_uint16 const *n);
		void espress_cipher_uint16_div(struct espress_cipher_uint16 *this, struct espress_cipher_uint16 const *n);
		void espress_cipher_uint16_and(struct espress_cipher_uint16 *this, struct espress_cipher_uint16 const *n);
		void espress_cipher_uint16_or(struct espress_cipher_uint16 *this, struct espress_cipher_uint16 const *n);
		void espress_cipher_uint16_xor(struct espress_cipher_uint16 *this, struct espress_cipher_uint16 const *n);
		void espress_cipher_uint16_not(struct espress_cipher_uint16 *this);
		void espress_cipher_uint16_shift(struct espress_cipher_uint16 *this, i_t n);
		void espress_cipher_uint16_rotate(struct espress_cipher_uint16 *this, i_t n);
		i_t espress_cipher_uint16_eq(struct espress_cipher_uint16 const *this, struct espress_cipher_uint16 const *n);
		i_t espress_cipher_uint16_lt(struct espress_cipher_uint16 const *this, struct espress_cipher_uint16 const *n);
		i_t espress_cipher_uint16_le(struct espress_cipher_uint16 const *this, struct espress_cipher_uint16 const *n);
		void espress_cipher_uint16_bswap(struct espress_cipher_uint16 *this);

#		if ESPRESS_CIPHER_IS_HOST_BYTE_ORDER_LITTLE
#			define ESPRESS_CIPHER_UINT16(n1, n0) {ESPRESS_CIPHER_UINT8(n0), ESPRESS_CIPHER_UINT8(n1)}
#		else
#			define ESPRESS_CIPHER_UINT16(n1, n0) {ESPRESS_CIPHER_UINT8(n1), ESPRESS_CIPHER_UINT8(n0)}
#		endif
#	endif

	extern struct espress_cipher_uint16 const espress_cipher_uint16_0[];
	extern struct espress_cipher_uint16 const espress_cipher_uint16_1[];
	extern struct espress_cipher_uint16 const espress_cipher_uint16_max[];

#	define ESPRESS_CIPHER_UINT16_0 ESPRESS_CIPHER_UINT16(0, 0)
#	define ESPRESS_CIPHER_UINT16_1 ESPRESS_CIPHER_UINT16(0, 1)
#	define ESPRESS_CIPHER_UINT16_MAX ESPRESS_CIPHER_UINT16(0xff, 0xff)
#	if ESPRESS_CIPHER_IS_HOST_BYTE_ORDER_LITTLE
#		define espress_cipher_uint16_htol(this) ((void)(this))
#		define espress_cipher_uint16_htob(this) espress_cipher_uint16_bswap(this)
#	else
#		define espress_cipher_uint16_htol(this) espress_cipher_uint16_bswap(this)
#		define espress_cipher_uint16_htob(this) ((void)(this))
#	endif
#	define espress_cipher_uint16_ltoh(this) espress_cipher_uint16_htol(this)
#	define espress_cipher_uint16_btoh(this) espress_cipher_uint16_htob(this)
#	if ESPRESS_CIPHER_IS_NETWORK_BYTE_ORDER_LITTLE
#		define espress_cipher_uint16_hton(this) espress_cipher_uint16_htol(this)
#	else
#		define espress_cipher_uint16_hton(this) espress_cipher_uint16_htob(this)
#	endif
#	define espress_cipher_uint16_ntoh(this) espress_cipher_uint16_hton(this)
#endif

#if 4 <= ESPRESS_CIPHER_UINT_MAX_WIDTH
#	if 4 <= MAX_U_T_WIDTH
#		pragma pack(push, 1)
		struct espress_cipher_uint32
		{
			u32_t u;
		};
#		pragma pack(pop)

		static inline void espress_cipher_uint32_set(struct espress_cipher_uint32 *this, b_t n3, b_t n2, b_t n1, b_t n0)
		{
			this->u = (u32_t)n0 + ((u32_t)n1 << 8) + ((u32_t)n2 << 16) + ((u32_t)n3 << 24);
		}
		static inline void espress_cipher_uint32_get(struct espress_cipher_uint32 const *this, b_t *n3, b_t *n2, b_t *n1, b_t *n0)
		{
			*n0 = this->u;
			*n1 = this->u >> 8;
			*n2 = this->u >> 16;
			*n3 = this->u >> 24;
		}
		static inline void espress_cipher_uint32_va_set(struct espress_cipher_uint32 *this, va_list *np)
		{
			b_t const n3 = va_arg(*np, int);
			b_t const n2 = va_arg(*np, int);
			b_t const n1 = va_arg(*np, int);
			b_t const n0 = va_arg(*np, int);
			espress_cipher_uint32_set(this, n3, n2, n1, n0);
		}
		static inline void espress_cipher_uint32_va_get(struct espress_cipher_uint32 const *this, va_list *np)
		{
			b_t * const n3 = va_arg(*np, b_t *);
			b_t * const n2 = va_arg(*np, b_t *);
			b_t * const n1 = va_arg(*np, b_t *);
			b_t * const n0 = va_arg(*np, b_t *);
			espress_cipher_uint32_get(this, n3, n2, n1, n0);
		}
		static inline void espress_cipher_uint32_set_u(struct espress_cipher_uint32 *this, u_t n){this->u = n;}
		static inline void espress_cipher_uint32_get_u(struct espress_cipher_uint32 const *this, u_t *n){*n = this->u;}
		static inline void espress_cipher_uint32_assign(struct espress_cipher_uint32 *this, struct espress_cipher_uint32 const *n){this->u = n->u;}
		static inline i_t espress_cipher_uint32_add(struct espress_cipher_uint32 *this, struct espress_cipher_uint32 const *n){this->u = this->u + n->u; return this->u < n->u;}
		static inline i_t espress_cipher_uint32_sub(struct espress_cipher_uint32 *this, struct espress_cipher_uint32 const *n){i_t const is_uf = this->u < n ->u; this->u = this->u - n->u; return is_uf;}
		static inline void espress_cipher_uint32_mul(struct espress_cipher_uint32 *this, struct espress_cipher_uint32 const *n){this->u *= n->u;}
		static inline void espress_cipher_uint32_div(struct espress_cipher_uint32 *this, struct espress_cipher_uint32 const *n){this->u /= n->u;}
		static inline void espress_cipher_uint32_and(struct espress_cipher_uint32 *this, struct espress_cipher_uint32 const *n){this->u &= n->u;}
		static inline void espress_cipher_uint32_or(struct espress_cipher_uint32 *this, struct espress_cipher_uint32 const *n){this->u |= n->u;}
		static inline void espress_cipher_uint32_xor(struct espress_cipher_uint32 *this, struct espress_cipher_uint32 const *n){this->u ^= n->u;}
		static inline void espress_cipher_uint32_not(struct espress_cipher_uint32 *this){this->u = ~(this->u);}
		static inline void espress_cipher_uint32_shift(struct espress_cipher_uint32 *this, i_t n)
		{
			if (n < 0)
			{
				this->u >>= -n;
			}
			else if (0 < n)
			{
				this->u <<= n;
			}
		}
		static inline void espress_cipher_uint32_rotate(struct espress_cipher_uint32 *this, i_t n)
		{
			i_t m = n % 32;
			u_t u;
			if (m < 0)
			{
				u = this->u << (32 + m);
				this->u = (this->u >> -m) + u;
			}
			else if (0 < m)
			{
				u = this->u >> (32 - m);
				this->u = (this->u << m) + u;
			}
		}
		static inline i_t espress_cipher_uint32_eq(struct espress_cipher_uint32 const *this, struct espress_cipher_uint32 const *n){return this->u == n->u;}
		static inline i_t espress_cipher_uint32_lt(struct espress_cipher_uint32 const *this, struct espress_cipher_uint32 const *n){return this->u < n->u;}
		static inline i_t espress_cipher_uint32_le(struct espress_cipher_uint32 const *this, struct espress_cipher_uint32 const *n){return this->u <= n->u;}
		static inline void espress_cipher_uint32_bswap(struct espress_cipher_uint32 *this){swap_endian_32(&(this->u));}

#		define ESPRESS_CIPHER_UINT32(n3, n2, n1, n0) {(u32_t)(n0) + ((u32_t)(n1) << 8) + ((u32_t)(n2) << 16) + ((u32_t)(n3) << 24)}
#	else
#		pragma pack(push, 1)
		struct espress_cipher_uint32
		{
#		if ESPRESS_CIPHER_IS_HOST_BYTE_ORDER_LITTLE
			struct espress_cipher_uint16 l;
			struct espress_cipher_uint16 h;
#		else
			struct espress_cipher_uint16 h;
			struct espress_cipher_uint16 l;
#		endif
		};
#		pragma pack(pop)

		void espress_cipher_uint32_set(struct espress_cipher_uint32 *this, b_t n3, b_t n2, b_t n1, b_t n0);
		void espress_cipher_uint32_get(struct espress_cipher_uint32 const *this, b_t *n3, b_t *n2, b_t *n1, b_t *n0);
		void espress_cipher_uint32_va_set(struct espress_cipher_uint32 *this, va_list *np);
		void espress_cipher_uint32_va_get(struct espress_cipher_uint32 const *this, va_list *np);
		void espress_cipher_uint32_set_u(struct espress_cipher_uint32 *this, u_t n);
		void espress_cipher_uint32_get_u(struct espress_cipher_uint32 const *this, u_t *n);
		void espress_cipher_uint32_assign(struct espress_cipher_uint32 *this, struct espress_cipher_uint32 const *n);
		i_t espress_cipher_uint32_add(struct espress_cipher_uint32 *this, struct espress_cipher_uint32 const *n);
		i_t espress_cipher_uint32_sub(struct espress_cipher_uint32 *this, struct espress_cipher_uint32 const *n);
		void espress_cipher_uint32_mul(struct espress_cipher_uint32 *this, struct espress_cipher_uint32 const *n);
		void espress_cipher_uint32_div(struct espress_cipher_uint32 *this, struct espress_cipher_uint32 const *n);
		void espress_cipher_uint32_and(struct espress_cipher_uint32 *this, struct espress_cipher_uint32 const *n);
		void espress_cipher_uint32_or(struct espress_cipher_uint32 *this, struct espress_cipher_uint32 const *n);
		void espress_cipher_uint32_xor(struct espress_cipher_uint32 *this, struct espress_cipher_uint32 const *n);
		void espress_cipher_uint32_not(struct espress_cipher_uint32 *this);
		void espress_cipher_uint32_shift(struct espress_cipher_uint32 *this, i_t n);
		void espress_cipher_uint32_rotate(struct espress_cipher_uint32 *this, i_t n);
		i_t espress_cipher_uint32_eq(struct espress_cipher_uint32 const *this, struct espress_cipher_uint32 const *n);
		i_t espress_cipher_uint32_lt(struct espress_cipher_uint32 const *this, struct espress_cipher_uint32 const *n);
		i_t espress_cipher_uint32_le(struct espress_cipher_uint32 const *this, struct espress_cipher_uint32 const *n);
		void espress_cipher_uint32_bswap(struct espress_cipher_uint32 *this);

#		if ESPRESS_CIPHER_IS_HOST_BYTE_ORDER_LITTLE
#			define ESPRESS_CIPHER_UINT32(n3, n2, n1, n0) {ESPRESS_CIPHER_UINT16(n1, n0), ESPRESS_CIPHER_UINT16(n3, n2)}
#		else
#			define ESPRESS_CIPHER_UINT32(n3, n2, n1, n0) {ESPRESS_CIPHER_UINT16(n3, n2), ESPRESS_CIPHER_UINT16(n1, n0)}
#		endif
#	endif

	extern struct espress_cipher_uint32 const espress_cipher_uint32_0[];
	extern struct espress_cipher_uint32 const espress_cipher_uint32_1[];
	extern struct espress_cipher_uint32 const espress_cipher_uint32_max[];

	#define ESPRESS_CIPHER_UINT32_0 ESPRESS_CIPHER_UINT32(0, 0, 0, 0)
	#define ESPRESS_CIPHER_UINT32_1 ESPRESS_CIPHER_UINT32(0, 0, 0, 1)
	#define ESPRESS_CIPHER_UINT32_MAX ESPRESS_CIPHER_UINT32(0xff, 0xff, 0xff, 0xff)
#	if ESPRESS_CIPHER_IS_HOST_BYTE_ORDER_LITTLE
#		define espress_cipher_uint32_htol(this) ((void)(this))
#		define espress_cipher_uint32_htob(this) espress_cipher_uint32_bswap(this)
#	else
#		define espress_cipher_uint32_htol(this) espress_cipher_uint32_bswap(this)
#		define espress_cipher_uint32_htob(this) ((void)(this))
#	endif
#	define espress_cipher_uint32_ltoh(this) espress_cipher_uint32_htol(this)
#	define espress_cipher_uint32_btoh(this) espress_cipher_uint32_htob(this)
#	if ESPRESS_CIPHER_IS_NETWORK_BYTE_ORDER_LITTLE
#		define espress_cipher_uint32_hton(this) espress_cipher_uint32_htol(this)
#	else
#		define espress_cipher_uint32_hton(this) espress_cipher_uint32_htob(this)
#	endif
#	define espress_cipher_uint32_ntoh(this) espress_cipher_uint32_hton(this)
#endif

#if 8 <= ESPRESS_CIPHER_UINT_MAX_WIDTH
#	if 8 <= MAX_U_T_WIDTH
#		pragma pack(push, 1)
		struct espress_cipher_uint64
		{
			u64_t u;
		};
#		pragma pack(pop)

		static inline void espress_cipher_uint64_set(struct espress_cipher_uint64 *this, b_t n7, b_t n6, b_t n5, b_t n4, b_t n3, b_t n2, b_t n1, b_t n0)
		{
			this->u =
				(u64_t)n0 + ((u64_t)n1 << 8) + ((u64_t)n2 << 16) + ((u64_t)n3 << 24) +
				((u64_t)n4 << 32) + ((u64_t)n5 << 40) + ((u64_t)n6 << 48) + ((u64_t)n7 << 56);
		}
		static inline void espress_cipher_uint64_get(struct espress_cipher_uint64 const *this, b_t *n7, b_t *n6, b_t *n5, b_t *n4, b_t *n3, b_t *n2, b_t *n1, b_t *n0)
		{
			*n0 = this->u;
			*n1 = this->u >> 8;
			*n2 = this->u >> 16;
			*n3 = this->u >> 24;
			*n4 = this->u >> 32;
			*n5 = this->u >> 40;
			*n6 = this->u >> 48;
			*n7 = this->u >> 56;
		}
		static inline void espress_cipher_uint64_va_set(struct espress_cipher_uint64 *this, va_list *np)
		{
			b_t const n7 = va_arg(*np, int);
			b_t const n6 = va_arg(*np, int);
			b_t const n5 = va_arg(*np, int);
			b_t const n4 = va_arg(*np, int);
			b_t const n3 = va_arg(*np, int);
			b_t const n2 = va_arg(*np, int);
			b_t const n1 = va_arg(*np, int);
			b_t const n0 = va_arg(*np, int);
			espress_cipher_uint64_set(this, n7, n6, n5, n4, n3, n2, n1, n0);
		}
		static inline void espress_cipher_uint64_va_get(struct espress_cipher_uint64 const *this, va_list *np)
		{
			b_t * const n7 = va_arg(*np, b_t *);
			b_t * const n6 = va_arg(*np, b_t *);
			b_t * const n5 = va_arg(*np, b_t *);
			b_t * const n4 = va_arg(*np, b_t *);
			b_t * const n3 = va_arg(*np, b_t *);
			b_t * const n2 = va_arg(*np, b_t *);
			b_t * const n1 = va_arg(*np, b_t *);
			b_t * const n0 = va_arg(*np, b_t *);
			espress_cipher_uint64_get(this, n7, n6, n5, n4, n3, n2, n1, n0);
		}
		static inline void espress_cipher_uint64_set_u(struct espress_cipher_uint64 *this, u_t n){this->u = n;}
		static inline void espress_cipher_uint64_get_u(struct espress_cipher_uint64 const *this, u_t *n){*n = this->u;}
		static inline void espress_cipher_uint64_assign(struct espress_cipher_uint64 *this, struct espress_cipher_uint64 const *n){this->u = n->u;}
		static inline i_t espress_cipher_uint64_add(struct espress_cipher_uint64 *this, struct espress_cipher_uint64 const *n){this->u = this->u + n->u; return this->u < n->u;}
		static inline i_t espress_cipher_uint64_sub(struct espress_cipher_uint64 *this, struct espress_cipher_uint64 const *n){i_t const is_uf = this->u < n ->u; this->u = this->u - n->u; return is_uf;}
		static inline void espress_cipher_uint64_mul(struct espress_cipher_uint64 *this, struct espress_cipher_uint64 const *n){this->u *= n->u;}
		static inline void espress_cipher_uint64_div(struct espress_cipher_uint64 *this, struct espress_cipher_uint64 const *n){this->u /= n->u;}
		static inline void espress_cipher_uint64_and(struct espress_cipher_uint64 *this, struct espress_cipher_uint64 const *n){this->u &= n->u;}
		static inline void espress_cipher_uint64_or(struct espress_cipher_uint64 *this, struct espress_cipher_uint64 const *n){this->u |= n->u;}
		static inline void espress_cipher_uint64_xor(struct espress_cipher_uint64 *this, struct espress_cipher_uint64 const *n){this->u ^= n->u;}
		static inline void espress_cipher_uint64_not(struct espress_cipher_uint64 *this){this->u = ~(this->u);}
		static inline void espress_cipher_uint64_shift(struct espress_cipher_uint64 *this, i_t n)
		{
			if (n < 0)
			{
				this->u >>= -n;
			}
			else if (0 < n)
			{
				this->u <<= n;
			}
		}
		static inline void espress_cipher_uint64_rotate(struct espress_cipher_uint64 *this, i_t n)
		{
			i_t m = n % 64;
			u64_t u;
			if (m < 0)
			{
				u = this->u << (64 + m);
				this->u = (this->u >> -m) + u;
			}
			else if (0 < m)
			{
				u = this->u >> (64 - m);
				this->u = (this->u << m) + u;
			}
		}
		static inline i_t espress_cipher_uint64_eq(struct espress_cipher_uint64 const *this, struct espress_cipher_uint64 const *n){return this->u == n->u;}
		static inline i_t espress_cipher_uint64_lt(struct espress_cipher_uint64 const *this, struct espress_cipher_uint64 const *n){return this->u < n->u;}
		static inline i_t espress_cipher_uint64_le(struct espress_cipher_uint64 const *this, struct espress_cipher_uint64 const *n){return this->u <= n->u;}
		static inline void espress_cipher_uint64_bswap(struct espress_cipher_uint64 *this){swap_endian_64(&(this->u));}

#		define ESPRESS_CIPHER_UINT64(n7, n6, n5, n4, n3, n2, n1, n0) {(u64_t)(n0) + ((u64_t)(n1) << 8) + ((u64_t)(n2) << 16) + ((u64_t)(n3) << 24) + ((u64_t)(n4) << 32) + ((u64_t)(n5) << 40) + ((u64_t)(n6) << 48) + ((u64_t)(n7) << 56)}
#	else
#		pragma pack(push, 1)
		struct espress_cipher_uint64
		{
#		if ESPRESS_CIPHER_IS_HOST_BYTE_ORDER_LITTLE
			struct espress_cipher_uint32 l;
			struct espress_cipher_uint32 h;
#		else
			struct espress_cipher_uint32 h;
			struct espress_cipher_uint32 l;
#		endif
		};
#		pragma pack(pop)

		void espress_cipher_uint64_set(struct espress_cipher_uint64 *this, b_t n7, b_t n6, b_t n5, b_t n4, b_t n3, b_t n2, b_t n1, b_t n0);
		void espress_cipher_uint64_get(struct espress_cipher_uint64 const *this, b_t *n7, b_t *n6, b_t *n5, b_t *n4, b_t *n3, b_t *n2, b_t *n1, b_t *n0);
		void espress_cipher_uint64_va_set(struct espress_cipher_uint64 *this, va_list *np);
		void espress_cipher_uint64_va_get(struct espress_cipher_uint64 const *this, va_list *np);
		void espress_cipher_uint64_set_u(struct espress_cipher_uint64 *this, u_t n);
		void espress_cipher_uint64_get_u(struct espress_cipher_uint64 const *this, u_t *n);
		void espress_cipher_uint64_assign(struct espress_cipher_uint64 *this, struct espress_cipher_uint64 const *n);
		i_t espress_cipher_uint64_add(struct espress_cipher_uint64 *this, struct espress_cipher_uint64 const *n);
		i_t espress_cipher_uint64_sub(struct espress_cipher_uint64 *this, struct espress_cipher_uint64 const *n);
		void espress_cipher_uint64_mul(struct espress_cipher_uint64 *this, struct espress_cipher_uint64 const *n);
		void espress_cipher_uint64_div(struct espress_cipher_uint64 *this, struct espress_cipher_uint64 const *n);
		void espress_cipher_uint64_and(struct espress_cipher_uint64 *this, struct espress_cipher_uint64 const *n);
		void espress_cipher_uint64_or(struct espress_cipher_uint64 *this, struct espress_cipher_uint64 const *n);
		void espress_cipher_uint64_xor(struct espress_cipher_uint64 *this, struct espress_cipher_uint64 const *n);
		void espress_cipher_uint64_not(struct espress_cipher_uint64 *this);
		void espress_cipher_uint64_shift(struct espress_cipher_uint64 *this, i_t n);
		void espress_cipher_uint64_rotate(struct espress_cipher_uint64 *this, i_t n);
		i_t espress_cipher_uint64_eq(struct espress_cipher_uint64 const *this, struct espress_cipher_uint64 const *n);
		i_t espress_cipher_uint64_lt(struct espress_cipher_uint64 const *this, struct espress_cipher_uint64 const *n);
		i_t espress_cipher_uint64_le(struct espress_cipher_uint64 const *this, struct espress_cipher_uint64 const *n);
		void espress_cipher_uint64_bswap(struct espress_cipher_uint64 *this);

#		if ESPRESS_CIPHER_IS_HOST_BYTE_ORDER_LITTLE
#			define ESPRESS_CIPHER_UINT64(n7, n6, n5, n4, n3, n2, n1, n0) {ESPRESS_CIPHER_UINT32(n3, n2, n1, n0), ESPRESS_CIPHER_UINT32(n7, n6, n5, n4)}
#		else
#			define ESPRESS_CIPHER_UINT64(n7, n6, n5, n4, n3, n2, n1, n0) {ESPRESS_CIPHER_UINT32(n7, n6, n5, n4), ESPRESS_CIPHER_UINT32(n3, n2, n1, n0)}
#		endif
#	endif

	extern struct espress_cipher_uint64 const espress_cipher_uint64_0[];
	extern struct espress_cipher_uint64 const espress_cipher_uint64_1[];
	extern struct espress_cipher_uint64 const espress_cipher_uint64_max[];

#	define ESPRESS_CIPHER_UINT64_0 ESPRESS_CIPHER_UINT64(0, 0, 0, 0, 0, 0, 0, 0)
#	define ESPRESS_CIPHER_UINT64_1 ESPRESS_CIPHER_UINT64(0, 0, 0, 0, 0, 0, 0, 1)
#	define ESPRESS_CIPHER_UINT64_MAX ESPRESS_CIPHER_UINT64(0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff)
#	if ESPRESS_CIPHER_IS_HOST_BYTE_ORDER_LITTLE
#		define espress_cipher_uint64_htol(this) ((void)(this))
#		define espress_cipher_uint64_htob(this) espress_cipher_uint64_bswap(this)
#	else
#		define espress_cipher_uint64_htol(this) espress_cipher_uint64_bswap(this)
#		define espress_cipher_uint64_htob(this) ((void)(this))
#	endif
#	define espress_cipher_uint64_ltoh(this) espress_cipher_uint64_htol(this)
#	define espress_cipher_uint64_btoh(this) espress_cipher_uint64_htob(this)
#	if ESPRESS_CIPHER_IS_NETWORK_BYTE_ORDER_LITTLE
#		define espress_cipher_uint64_hton(this) espress_cipher_uint64_htol(this)
#	else
#		define espress_cipher_uint64_hton(this) espress_cipher_uint64_htob(this)
#	endif
#	define espress_cipher_uint64_ntoh(this) espress_cipher_uint64_hton(this)
#endif

#if 16 <= ESPRESS_CIPHER_UINT_MAX_WIDTH
#	if 16 <= MAX_U_T_WIDTH
#		pragma pack(push, 1)
		struct espress_cipher_uint128
		{
			u128_t u;
		};
#		pragma pack(pop)

		static inline void espress_cipher_uint128_set(struct espress_cipher_uint128 *this, b_t n15, b_t n14, b_t n13, b_t n12, b_t n11, b_t n10, b_t n9, b_t n8, b_t n7, b_t n6, b_t n5, b_t n4, b_t n3, b_t n2, b_t n1, b_t n0)
		{
			this->u =
				(u128_t)n0 + ((u128_t)n1 << 8) + ((u128_t)n2 << 16) + ((u128_t)n3 << 24) +
				((u128_t)n4 << 32) + ((u128_t)n5 << 40) + ((u128_t)n6 << 48) + ((u128_t)n7 << 56) +
				((u128_t)n5 << 64) + ((u128_t)n5 << 72) + ((u128_t)n6 << 80) + ((u128_t)n7 << 88) +
				((u128_t)n5 << 96) + ((u128_t)n5 << 104) + ((u128_t)n6 << 112) + ((u128_t)n7 << 120);
		}
		static inline void espress_cipher_uint128_get(struct espress_cipher_uint128 const *this, b_t *n15, b_t *n14, b_t *n13, b_t *n12, b_t *n11, b_t *n10, b_t *n9, b_t *n8, b_t *n7, b_t *n6, b_t *n5, b_t *n4, b_t *n3, b_t *n2, b_t *n1, b_t *n0)
		{
			 *n0 = this->u;
			 *n1 = this->u >> 8;
			 *n2 = this->u >> 16;
			 *n3 = this->u >> 24;
			 *n4 = this->u >> 32;
			 *n5 = this->u >> 40;
			 *n6 = this->u >> 48;
			 *n7 = this->u >> 56;
			 *n8 = this->u >> 64;
			 *n9 = this->u >> 72;
			*n10 = this->u >> 80;
			*n11 = this->u >> 88;
			*n12 = this->u >> 96;
			*n13 = this->u >> 104;
			*n14 = this->u >> 112;
			*n15 = this->u >> 120;
		}
		static inline void espress_cipher_uint128_va_set(struct espress_cipher_uint128 *this, va_list *np)
		{
			b_t const n15 = va_arg(*np, int);
			b_t const n14 = va_arg(*np, int);
			b_t const n13 = va_arg(*np, int);
			b_t const n12 = va_arg(*np, int);
			b_t const n11 = va_arg(*np, int);
			b_t const n10 = va_arg(*np, int);
			b_t const n9 = va_arg(*np, int);
			b_t const n8 = va_arg(*np, int);
			b_t const n7 = va_arg(*np, int);
			b_t const n6 = va_arg(*np, int);
			b_t const n5 = va_arg(*np, int);
			b_t const n4 = va_arg(*np, int);
			b_t const n3 = va_arg(*np, int);
			b_t const n2 = va_arg(*np, int);
			b_t const n1 = va_arg(*np, int);
			b_t const n0 = va_arg(*np, int);
			espress_cipher_uint128_set(this, n15, n14, n13, n12, n11, n10, n9, n8, n7, n6, n5, n4, n3, n2, n1, n0);
		}
		static inline void espress_cipher_uint128_va_get(struct espress_cipher_uint128 const *this, va_list *np)
		{
			b_t * const n15 = va_arg(*np, b_t *);
			b_t * const n14 = va_arg(*np, b_t *);
			b_t * const n13 = va_arg(*np, b_t *);
			b_t * const n12 = va_arg(*np, b_t *);
			b_t * const n11 = va_arg(*np, b_t *);
			b_t * const n10 = va_arg(*np, b_t *);
			b_t * const n9 = va_arg(*np, b_t *);
			b_t * const n8 = va_arg(*np, b_t *);
			b_t * const n7 = va_arg(*np, b_t *);
			b_t * const n6 = va_arg(*np, b_t *);
			b_t * const n5 = va_arg(*np, b_t *);
			b_t * const n4 = va_arg(*np, b_t *);
			b_t * const n3 = va_arg(*np, b_t *);
			b_t * const n2 = va_arg(*np, b_t *);
			b_t * const n1 = va_arg(*np, b_t *);
			b_t * const n0 = va_arg(*np, b_t *);
			espress_cipher_uint128_get(this, n15, n14, n13, n12, n11, n10, n9, n8, n7, n6, n5, n4, n3, n2, n1, n0);
		}
		static inline void espress_cipher_uint128_set_u(struct espress_cipher_uint128 *this, u_t n){this->u = n;}
		static inline void espress_cipher_uint128_get_u(struct espress_cipher_uint128 const *this, u_t *n){*n = this->u;}
		static inline void espress_cipher_uint128_assign(struct espress_cipher_uint128 *this, struct espress_cipher_uint128 const *n){this->u = n->u;}
		static inline i_t espress_cipher_uint128_add(struct espress_cipher_uint128 *this, struct espress_cipher_uint128 const *n){this->u = this->u + n->u; return this->u < n->u;}
		static inline i_t espress_cipher_uint128_sub(struct espress_cipher_uint128 *this, struct espress_cipher_uint128 const *n){i_t const is_uf = this->u < n ->u; this->u = this->u - n->u; return is_uf;}
		static inline void espress_cipher_uint128_mul(struct espress_cipher_uint128 *this, struct espress_cipher_uint128 const *n){this->u *= n->u;}
		static inline void espress_cipher_uint128_div(struct espress_cipher_uint128 *this, struct espress_cipher_uint128 const *n){this->u /= n->u;}
		static inline void espress_cipher_uint128_and(struct espress_cipher_uint128 *this, struct espress_cipher_uint128 const *n){this->u &= n->u;}
		static inline void espress_cipher_uint128_or(struct espress_cipher_uint128 *this, struct espress_cipher_uint128 const *n){this->u |= n->u;}
		static inline void espress_cipher_uint128_xor(struct espress_cipher_uint128 *this, struct espress_cipher_uint128 const *n){this->u ^= n->u;}
		static inline void espress_cipher_uint128_not(struct espress_cipher_uint128 *this){this->u = ~(this->u);}
		static inline void espress_cipher_uint128_shift(struct espress_cipher_uint128 *this, i_t n)
		{
			if (n < 0)
			{
				this->u >>= -n;
			}
			else if (0 < n)
			{
				this->u <<= n;
			}
		}
		static inline void espress_cipher_uint128_rotate(struct espress_cipher_uint128 *this, i_t n)
		{
			i_t m = n % 128;
			u128_t u;
			if (m < 0)
			{
				u = this->u << (128 + m);
				this->u = (this->u >> -m) + u;
			}
			else if (0 < m)
			{
				u = this->u >> (128 - m);
				this->u = (this->u << m) + u;
			}
		}
		static inline i_t espress_cipher_uint128_eq(struct espress_cipher_uint128 const *this, struct espress_cipher_uint128 const *n){return this->u == n->u;}
		static inline i_t espress_cipher_uint128_lt(struct espress_cipher_uint128 const *this, struct espress_cipher_uint128 const *n){return this->u < n->u;}
		static inline i_t espress_cipher_uint128_le(struct espress_cipher_uint128 const *this, struct espress_cipher_uint128 const *n){return this->u <= n->u;}
		static inline void espress_cipher_uint128_bswap(struct espress_cipher_uint128 *this){swap_endian_128(&(this->u));}

#		define ESPRESS_CIPHER_UINT128(n15, n14, n13, n12, n11, n10, n9, n8, n7, n6, n5, n4, n3, n2, n1, n0) \
		{ \
			(u128_t)(n0) + ((u128_t)(n1) << 8) + ((u128_t)(n2) << 16) + ((u128_t)(n3) << 24) + ((u128_t)(n4) << 32) + ((u128_t)(n5) << 40) + ((u128_t)(n6) << 48) + ((u128_t)(n7) << 56) + \
				((u128_t)(n8) << 64) + ((u128_t)(n9) << 72) + ((u128_t)(n10) << 80) + ((u128_t)(n11) << 88) + ((u128_t)(n12) << 96) + ((u128_t)(n13) << 104) + ((u128_t)(n14) << 112) + ((u128_t)(n15) << 120) \
		}
#	else
#		pragma pack(push, 1)
		struct espress_cipher_uint128
		{
#		if ESPRESS_CIPHER_IS_HOST_BYTE_ORDER_LITTLE
			struct espress_cipher_uint64 l;
			struct espress_cipher_uint64 h;
#		else
			struct espress_cipher_uint64 h;
			struct espress_cipher_uint64 l;
#		endif
		};
#		pragma pack(pop)

		void espress_cipher_uint128_set(struct espress_cipher_uint128 *this,
			b_t n15, b_t n14, b_t n13, b_t n12, b_t n11, b_t n10, b_t n9, b_t n8,
			b_t n7, b_t n6, b_t n5, b_t n4, b_t n3, b_t n2, b_t n1, b_t n0);
		void espress_cipher_uint128_get(struct espress_cipher_uint128 const *this,
			b_t *n15, b_t *n14, b_t *n13, b_t *n12, b_t *n11, b_t *n10, b_t *n9, b_t *n8,
			b_t *n7, b_t *n6, b_t *n5, b_t *n4, b_t *n3, b_t *n2, b_t *n1, b_t *n0);
		void espress_cipher_uint128_va_set(struct espress_cipher_uint128 *this, va_list *np);
		void espress_cipher_uint128_va_get(struct espress_cipher_uint128 const *this, va_list *np);
		void espress_cipher_uint128_set_u(struct espress_cipher_uint128 *this, u_t n);
		void espress_cipher_uint128_get_u(struct espress_cipher_uint128 const *this, u_t *n);
		void espress_cipher_uint128_assign(struct espress_cipher_uint128 *this, struct espress_cipher_uint128 const *n);
		i_t espress_cipher_uint128_add(struct espress_cipher_uint128 *this, struct espress_cipher_uint128 const *n);
		i_t espress_cipher_uint128_sub(struct espress_cipher_uint128 *this, struct espress_cipher_uint128 const *n);
		void espress_cipher_uint128_mul(struct espress_cipher_uint128 *this, struct espress_cipher_uint128 const *n);
		void espress_cipher_uint128_div(struct espress_cipher_uint128 *this, struct espress_cipher_uint128 const *n);
		void espress_cipher_uint128_and(struct espress_cipher_uint128 *this, struct espress_cipher_uint128 const *n);
		void espress_cipher_uint128_or(struct espress_cipher_uint128 *this, struct espress_cipher_uint128 const *n);
		void espress_cipher_uint128_xor(struct espress_cipher_uint128 *this, struct espress_cipher_uint128 const *n);
		void espress_cipher_uint128_not(struct espress_cipher_uint128 *this);
		void espress_cipher_uint128_shift(struct espress_cipher_uint128 *this, i_t n);
		void espress_cipher_uint128_rotate(struct espress_cipher_uint128 *this, i_t n);
		i_t espress_cipher_uint128_eq(struct espress_cipher_uint128 const *this, struct espress_cipher_uint128 const *n);
		i_t espress_cipher_uint128_lt(struct espress_cipher_uint128 const *this, struct espress_cipher_uint128 const *n);
		i_t espress_cipher_uint128_le(struct espress_cipher_uint128 const *this, struct espress_cipher_uint128 const *n);
		void espress_cipher_uint128_bswap(struct espress_cipher_uint128 *this);

#		if ESPRESS_CIPHER_IS_HOST_BYTE_ORDER_LITTLE
#			define ESPRESS_CIPHER_UINT128(n15, n14, n13, n12, n11, n10, n9, n8, n7, n6, n5, n4, n3, n2, n1, n0) \
				{ESPRESS_CIPHER_UINT64(n7, n6, n5, n4, n3, n2, n1, n0), ESPRESS_CIPHER_UINT64(n15, n14, n13, n12, n11, n10, n9, n8)}
#		else
#			define ESPRESS_CIPHER_UINT128(n15, n14, n13, n12, n11, n10, n9, n8, n7, n6, n5, n4, n3, n2, n1, n0) \
				{ESPRESS_CIPHER_UINT64(n15, n14, n13, n12, n11, n10, n9, n8), ESPRESS_CIPHER_UINT64(n7, n6, n5, n4, n3, n2, n1, n0)}
#		endif
#	endif

	extern struct espress_cipher_uint128 const espress_cipher_uint128_0[];
	extern struct espress_cipher_uint128 const espress_cipher_uint128_1[];
	extern struct espress_cipher_uint128 const espress_cipher_uint128_max[];
#	define ESPRESS_CIPHER_UINT128_0 ESPRESS_CIPHER_UINT128(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
#	define ESPRESS_CIPHER_UINT128_1 ESPRESS_CIPHER_UINT128(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1)
#	define ESPRESS_CIPHER_UINT128_MAX ESPRESS_CIPHER_UINT128(0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff)
#	if ESPRESS_CIPHER_IS_HOST_BYTE_ORDER_LITTLE
#		define espress_cipher_uint128_htol(this) ((void)(this))
#		define espress_cipher_uint128_htob(this) espress_cipher_uint128_bswap(this)
#	else
#		define espress_cipher_uint128_htol(this) espress_cipher_uint128_bswap(this)
#		define espress_cipher_uint128_htob(this) ((void)(this))
#	endif
#	define espress_cipher_uint128_ltoh(this) espress_cipher_uint128_htol(this)
#	define espress_cipher_uint128_btoh(this) espress_cipher_uint128_htob(this)
#	if ESPRESS_CIPHER_IS_NETWORK_BYTE_ORDER_LITTLE
#		define espress_cipher_uint128_hton(this) espress_cipher_uint128_htol(this)
#	else
#		define espress_cipher_uint128_hton(this) espress_cipher_uint128_htob(this)
#	endif
#	define espress_cipher_uint128_ntoh(this) espress_cipher_uint128_hton(this)
#endif

#if 32 <= ESPRESS_CIPHER_UINT_MAX_WIDTH
#	if 32 <= MAX_U_T_WIDTH
#		error
#	else
#		pragma pack(push, 1)
		struct espress_cipher_uint256
		{
#		if ESPRESS_CIPHER_IS_HOST_BYTE_ORDER_LITTLE
			struct espress_cipher_uint128 l;
			struct espress_cipher_uint128 h;
#		else
			struct espress_cipher_uint128 h;
			struct espress_cipher_uint128 l;
#		endif
		};
#		pragma pack(pop)

		void espress_cipher_uint256_set(struct espress_cipher_uint256 *this,
			b_t n31, b_t n30, b_t n29, b_t n28, b_t n27, b_t n26, b_t n25, b_t n24,
			b_t n23, b_t n22, b_t n21, b_t n20, b_t n19, b_t n18, b_t n17, b_t n16,
			b_t n15, b_t n14, b_t n13, b_t n12, b_t n11, b_t n10, b_t n9, b_t n8,
			b_t n7, b_t n6, b_t n5, b_t n4, b_t n3, b_t n2, b_t n1, b_t n0);
		void espress_cipher_uint256_get(struct espress_cipher_uint256 const *this,
			b_t *n31, b_t *n30, b_t *n29, b_t *n28, b_t *n27, b_t *n26, b_t *n25, b_t *n24,
			b_t *n23, b_t *n22, b_t *n21, b_t *n20, b_t *n19, b_t *n18, b_t *n17, b_t *n16,
			b_t *n15, b_t *n14, b_t *n13, b_t *n12, b_t *n11, b_t *n10, b_t *n9, b_t *n8,
			b_t *n7, b_t *n6, b_t *n5, b_t *n4, b_t *n3, b_t *n2, b_t *n1, b_t *n0);
		void espress_cipher_uint256_va_set(struct espress_cipher_uint256 *this, va_list *np);
		void espress_cipher_uint256_va_get(struct espress_cipher_uint256 const *this, va_list *np);
		void espress_cipher_uint256_set_u(struct espress_cipher_uint256 *this, u_t n);
		void espress_cipher_uint256_get_u(struct espress_cipher_uint256 const *this, u_t *n);
		void espress_cipher_uint256_assign(struct espress_cipher_uint256 *this, struct espress_cipher_uint256 const *n);
		i_t espress_cipher_uint256_add(struct espress_cipher_uint256 *this, struct espress_cipher_uint256 const *n);
		i_t espress_cipher_uint256_sub(struct espress_cipher_uint256 *this, struct espress_cipher_uint256 const *n);
		void espress_cipher_uint256_mul(struct espress_cipher_uint256 *this, struct espress_cipher_uint256 const *n);
		void espress_cipher_uint256_div(struct espress_cipher_uint256 *this, struct espress_cipher_uint256 const *n);
		void espress_cipher_uint256_and(struct espress_cipher_uint256 *this, struct espress_cipher_uint256 const *n);
		void espress_cipher_uint256_or(struct espress_cipher_uint256 *this, struct espress_cipher_uint256 const *n);
		void espress_cipher_uint256_xor(struct espress_cipher_uint256 *this, struct espress_cipher_uint256 const *n);
		void espress_cipher_uint256_not(struct espress_cipher_uint256 *this);
		void espress_cipher_uint256_shift(struct espress_cipher_uint256 *this, i_t n);
		void espress_cipher_uint256_rotate(struct espress_cipher_uint256 *this, i_t n);
		i_t espress_cipher_uint256_eq(struct espress_cipher_uint256 const *this, struct espress_cipher_uint256 const *n);
		i_t espress_cipher_uint256_lt(struct espress_cipher_uint256 const *this, struct espress_cipher_uint256 const *n);
		i_t espress_cipher_uint256_le(struct espress_cipher_uint256 const *this, struct espress_cipher_uint256 const *n);
		void espress_cipher_uint256_bswap(struct espress_cipher_uint256 *this);

#		if ESPRESS_CIPHER_IS_HOST_BYTE_ORDER_LITTLE
#			define ESPRESS_CIPHER_UINT256(n31, n30, n29, n28, n27, n26, n25, n24, n23, n22, n21, n20, n19, n18, n17, n16, n15, n14, n13, n12, n11, n10, n9, n8, n7, n6, n5, n4, n3, n2, n1, n0) \
				{ESPRESS_CIPHER_UINT128(n15, n14, n13, n12, n11, n10, n9, n8, n7, n6, n5, n4, n3, n2, n1, n0), ESPRESS_CIPHER_UINT128(n31, n30, n29, n28, n27, n26, n25, n24, n23, n22, n21, n20, n19, n18, n17, n16)}
#		else
#			define ESPRESS_CIPHER_UINT256(n31, n30, n29, n28, n27, n26, n25, n24, n23, n22, n21, n20, n19, n18, n17, n16, n15, n14, n13, n12, n11, n10, n9, n8, n7, n6, n5, n4, n3, n2, n1, n0) \
				{ESPRESS_CIPHER_UINT128(n31, n30, n29, n28, n27, n26, n25, n24, n23, n22, n21, n20, n19, n18, n17, n16), ESPRESS_CIPHER_UINT128(n15, n14, n13, n12, n11, n10, n9, n8, n7, n6, n5, n4, n3, n2, n1, n0)}
#		endif
#	endif

	extern struct espress_cipher_uint256 const espress_cipher_uint256_0[];
	extern struct espress_cipher_uint256 const espress_cipher_uint256_1[];
	extern struct espress_cipher_uint256 const espress_cipher_uint256_max[];

#	define ESPRESS_CIPHER_UINT256_0 ESPRESS_CIPHER_UINT256(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
#	define ESPRESS_CIPHER_UINT256_1 ESPRESS_CIPHER_UINT256(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1)
#	define ESPRESS_CIPHER_UINT256_MAX ESPRESS_CIPHER_UINT256(0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff)
#	if ESPRESS_CIPHER_IS_HOST_BYTE_ORDER_LITTLE
#		define espress_cipher_uint256_htol(this) ((void)(this))
#		define espress_cipher_uint256_htob(this) espress_cipher_uint256_bswap(this)
#	else
#		define espress_cipher_uint256_htol(this) espress_cipher_uint256_bswap(this)
#		define espress_cipher_uint256_htob(this) ((void)(this))
#	endif
#	define espress_cipher_uint256_ltoh(this) espress_cipher_uint256_htol(this)
#	define espress_cipher_uint256_btoh(this) espress_cipher_uint256_htob(this)
#	if ESPRESS_CIPHER_IS_NETWORK_BYTE_ORDER_LITTLE
#		define espress_cipher_uint256_hton(this) espress_cipher_uint256_htol(this)
#	else
#		define espress_cipher_uint256_hton(this) espress_cipher_uint256_htob(this)
#	endif
#	define espress_cipher_uint256_ntoh(this) espress_cipher_uint256_hton(this)
#endif

#if 64 <= ESPRESS_CIPHER_UINT_MAX_WIDTH
#	if 64 <= MAX_U_T_WIDTH
#		error
#	else
#		pragma pack(push, 1)
		struct espress_cipher_uint512
		{
#		if ESPRESS_CIPHER_IS_HOST_BYTE_ORDER_LITTLE
			struct espress_cipher_uint256 l;
			struct espress_cipher_uint256 h;
#		else
			struct espress_cipher_uint256 h;
			struct espress_cipher_uint256 l;
#		endif
		};
#		pragma pack(pop)

		void espress_cipher_uint512_set(struct espress_cipher_uint512 *this,
			b_t n63, b_t n62, b_t n61, b_t n60, b_t n59, b_t n58, b_t n57, b_t n56,
			b_t n55, b_t n54, b_t n53, b_t n52, b_t n51, b_t n50, b_t n49, b_t n48,
			b_t n47, b_t n46, b_t n45, b_t n44, b_t n43, b_t n42, b_t n41, b_t n40,
			b_t n39, b_t n38, b_t n37, b_t n36, b_t n35, b_t n34, b_t n33, b_t n32,
			b_t n31, b_t n30, b_t n29, b_t n28, b_t n27, b_t n26, b_t n25, b_t n24,
			b_t n23, b_t n22, b_t n21, b_t n20, b_t n19, b_t n18, b_t n17, b_t n16,
			b_t n15, b_t n14, b_t n13, b_t n12, b_t n11, b_t n10, b_t n9, b_t n8,
			b_t n7, b_t n6, b_t n5, b_t n4, b_t n3, b_t n2, b_t n1, b_t n0);
		void espress_cipher_uint512_get(struct espress_cipher_uint512 const *this,
			b_t *n63, b_t *n62, b_t *n61, b_t *n60, b_t *n59, b_t *n58, b_t *n57, b_t *n56,
			b_t *n55, b_t *n54, b_t *n53, b_t *n52, b_t *n51, b_t *n50, b_t *n49, b_t *n48,
			b_t *n47, b_t *n46, b_t *n45, b_t *n44, b_t *n43, b_t *n42, b_t *n41, b_t *n40,
			b_t *n39, b_t *n38, b_t *n37, b_t *n36, b_t *n35, b_t *n34, b_t *n33, b_t *n32,
			b_t *n31, b_t *n30, b_t *n29, b_t *n28, b_t *n27, b_t *n26, b_t *n25, b_t *n24,
			b_t *n23, b_t *n22, b_t *n21, b_t *n20, b_t *n19, b_t *n18, b_t *n17, b_t *n16,
			b_t *n15, b_t *n14, b_t *n13, b_t *n12, b_t *n11, b_t *n10, b_t *n9, b_t *n8,
			b_t *n7, b_t *n6, b_t *n5, b_t *n4, b_t *n3, b_t *n2, b_t *n1, b_t *n0);
		void espress_cipher_uint512_va_set(struct espress_cipher_uint512 *this, va_list *np);
		void espress_cipher_uint512_va_get(struct espress_cipher_uint512 const *this, va_list *np);
		void espress_cipher_uint512_set_u(struct espress_cipher_uint512 *this, u_t n);
		void espress_cipher_uint512_get_u(struct espress_cipher_uint512 const *this, u_t *n);
		void espress_cipher_uint512_assign(struct espress_cipher_uint512 *this, struct espress_cipher_uint512 const *n);
		i_t espress_cipher_uint512_add(struct espress_cipher_uint512 *this, struct espress_cipher_uint512 const *n);
		i_t espress_cipher_uint512_sub(struct espress_cipher_uint512 *this, struct espress_cipher_uint512 const *n);
		void espress_cipher_uint512_mul(struct espress_cipher_uint512 *this, struct espress_cipher_uint512 const *n);
		void espress_cipher_uint512_div(struct espress_cipher_uint512 *this, struct espress_cipher_uint512 const *n);
		void espress_cipher_uint512_and(struct espress_cipher_uint512 *this, struct espress_cipher_uint512 const *n);
		void espress_cipher_uint512_or(struct espress_cipher_uint512 *this, struct espress_cipher_uint512 const *n);
		void espress_cipher_uint512_xor(struct espress_cipher_uint512 *this, struct espress_cipher_uint512 const *n);
		void espress_cipher_uint512_not(struct espress_cipher_uint512 *this);
		void espress_cipher_uint512_shift(struct espress_cipher_uint512 *this, i_t n);
		void espress_cipher_uint512_rotate(struct espress_cipher_uint512 *this, i_t n);
		i_t espress_cipher_uint512_eq(struct espress_cipher_uint512 const *this, struct espress_cipher_uint512 const *n);
		i_t espress_cipher_uint512_lt(struct espress_cipher_uint512 const *this, struct espress_cipher_uint512 const *n);
		i_t espress_cipher_uint512_le(struct espress_cipher_uint512 const *this, struct espress_cipher_uint512 const *n);
		void espress_cipher_uint512_bswap(struct espress_cipher_uint512 *this);

#		if ESPRESS_CIPHER_IS_HOST_BYTE_ORDER_LITTLE
#			define ESPRESS_CIPHER_UINT512(n63, n62, n61, n60, n59, n58, n57, n56, n55, n54, n53, n52, n51, n50, n49, n48, n47, n46, n45, n44, n43, n42, n41, n40, n39, n38, n37, n36, n35, n34, n33, n32, \
				n31, n30, n29, n28, n27, n26, n25, n24, n23, n22, n21, n20, n19, n18, n17, n16, n15, n14, n13, n12, n11, n10, n9, n8, n7, n6, n5, n4, n3, n2, n1, n0) \
				{ \
					ESPRESS_CIPHER_UINT256(n31, n30, n29, n28, n27, n26, n25, n24, n23, n22, n21, n20, n19, n18, n17, n16, n15, n14, n13, n12, n11, n10, n9, n8, n7, n6, n5, n4, n3, n2, n1, n0), \
					ESPRESS_CIPHER_UINT256(n63, n62, n61, n60, n59, n58, n57, n56, n55, n54, n53, n52, n51, n50, n49, n48, n47, n46, n45, n44, n43, n42, n41, n40, n39, n38, n37, n36, n35, n34, n33, n32) \
				}
#		else
#			define ESPRESS_CIPHER_UINT512(n63, n62, n61, n60, n59, n58, n57, n56, n55, n54, n53, n52, n51, n50, n49, n48, n47, n46, n45, n44, n43, n42, n41, n40, n39, n38, n37, n36, n35, n34, n33, n32, \
				n31, n30, n29, n28, n27, n26, n25, n24, n23, n22, n21, n20, n19, n18, n17, n16, n15, n14, n13, n12, n11, n10, n9, n8, n7, n6, n5, n4, n3, n2, n1, n0) \
				{ \
					ESPRESS_CIPHER_UINT256(n63, n62, n61, n60, n59, n58, n57, n56, n55, n54, n53, n52, n51, n50, n49, n48, n47, n46, n45, n44, n43, n42, n41, n40, n39, n38, n37, n36, n35, n34, n33, n32), \
					ESPRESS_CIPHER_UINT256(n31, n30, n29, n28, n27, n26, n25, n24, n23, n22, n21, n20, n19, n18, n17, n16, n15, n14, n13, n12, n11, n10, n9, n8, n7, n6, n5, n4, n3, n2, n1, n0) \
				}
#		endif
#	endif

	extern struct espress_cipher_uint512 const espress_cipher_uint512_0[];
	extern struct espress_cipher_uint512 const espress_cipher_uint512_1[];
	extern struct espress_cipher_uint512 const espress_cipher_uint512_max[];

#	define ESPRESS_CIPHER_UINT512_0 ESPRESS_CIPHER_UINT512(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
#	define ESPRESS_CIPHER_UINT512_1 ESPRESS_CIPHER_UINT512(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1)
#	define ESPRESS_CIPHER_UINT512_MAX ESPRESS_CIPHER_UINT512( \
		0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, \
		0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff \
		)
#	if ESPRESS_CIPHER_IS_HOST_BYTE_ORDER_LITTLE
#		define espress_cipher_uint512_htol(this) ((void)(this))
#		define espress_cipher_uint512_htob(this) espress_cipher_uint512_bswap(this)
#	else
#		define espress_cipher_uint512_htol(this) espress_cipher_uint512_bswap(this)
#		define espress_cipher_uint512_htob(this) ((void)(this))
#	endif
#	define espress_cipher_uint512_ltoh(this) espress_cipher_uint512_htol(this)
#	define espress_cipher_uint512_btoh(this) espress_cipher_uint512_htob(this)
#	if ESPRESS_CIPHER_IS_NETWORK_BYTE_ORDER_LITTLE
#		define espress_cipher_uint512_hton(this) espress_cipher_uint512_htol(this)
#	else
#		define espress_cipher_uint512_hton(this) espress_cipher_uint512_htob(this)
#	endif
#	define espress_cipher_uint512_ntoh(this) espress_cipher_uint512_hton(this)
#endif

#if 128 <= ESPRESS_CIPHER_UINT_MAX_WIDTH
#	if 128 <= MAX_U_T_WIDTH
#		error
#	else
#		pragma pack(push, 1)
		struct espress_cipher_uint1024
		{
#		if ESPRESS_CIPHER_IS_HOST_BYTE_ORDER_LITTLE
			struct espress_cipher_uint512 l;
			struct espress_cipher_uint512 h;
#		else
			struct espress_cipher_uint512 h;
			struct espress_cipher_uint512 l;
#		endif
		};
#		pragma pack(pop)

		void espress_cipher_uint1024_set(struct espress_cipher_uint1024 *this,
			b_t n127, b_t n126, b_t n125, b_t n124, b_t n123, b_t n122, b_t n121, b_t n120,
			b_t n119, b_t n118, b_t n117, b_t n116, b_t n115, b_t n114, b_t n113, b_t n112,
			b_t n111, b_t n110, b_t n109, b_t n108, b_t n107, b_t n106, b_t n105, b_t n104,
			b_t n103, b_t n102, b_t n101, b_t n100, b_t n99, b_t n98, b_t n97, b_t n96,
			b_t n95, b_t n94, b_t n93, b_t n92, b_t n91, b_t n90, b_t n89, b_t n88,
			b_t n87, b_t n86, b_t n85, b_t n84, b_t n83, b_t n82, b_t n81, b_t n80,
			b_t n79, b_t n78, b_t n77, b_t n76, b_t n75, b_t n74, b_t n73, b_t n72,
			b_t n71, b_t n70, b_t n69, b_t n68, b_t n67, b_t n66, b_t n65, b_t n64,
			b_t n63, b_t n62, b_t n61, b_t n60, b_t n59, b_t n58, b_t n57, b_t n56,
			b_t n55, b_t n54, b_t n53, b_t n52, b_t n51, b_t n50, b_t n49, b_t n48,
			b_t n47, b_t n46, b_t n45, b_t n44, b_t n43, b_t n42, b_t n41, b_t n40,
			b_t n39, b_t n38, b_t n37, b_t n36, b_t n35, b_t n34, b_t n33, b_t n32,
			b_t n31, b_t n30, b_t n29, b_t n28, b_t n27, b_t n26, b_t n25, b_t n24,
			b_t n23, b_t n22, b_t n21, b_t n20, b_t n19, b_t n18, b_t n17, b_t n16,
			b_t n15, b_t n14, b_t n13, b_t n12, b_t n11, b_t n10, b_t n9, b_t n8,
			b_t n7, b_t n6, b_t n5, b_t n4, b_t n3, b_t n2, b_t n1, b_t n0);
		void espress_cipher_uint1024_get(struct espress_cipher_uint1024 const *this,
			b_t *n127, b_t *n126, b_t *n125, b_t *n124, b_t *n123, b_t *n122, b_t *n121, b_t *n120,
			b_t *n119, b_t *n118, b_t *n117, b_t *n116, b_t *n115, b_t *n114, b_t *n113, b_t *n112,
			b_t *n111, b_t *n110, b_t *n109, b_t *n108, b_t *n107, b_t *n106, b_t *n105, b_t *n104,
			b_t *n103, b_t *n102, b_t *n101, b_t *n100, b_t *n99, b_t *n98, b_t *n97, b_t *n96,
			b_t *n95, b_t *n94, b_t *n93, b_t *n92, b_t *n91, b_t *n90, b_t *n89, b_t *n88,
			b_t *n87, b_t *n86, b_t *n85, b_t *n84, b_t *n83, b_t *n82, b_t *n81, b_t *n80,
			b_t *n79, b_t *n78, b_t *n77, b_t *n76, b_t *n75, b_t *n74, b_t *n73, b_t *n72,
			b_t *n71, b_t *n70, b_t *n69, b_t *n68, b_t *n67, b_t *n66, b_t *n65, b_t *n64,
			b_t *n63, b_t *n62, b_t *n61, b_t *n60, b_t *n59, b_t *n58, b_t *n57, b_t *n56,
			b_t *n55, b_t *n54, b_t *n53, b_t *n52, b_t *n51, b_t *n50, b_t *n49, b_t *n48,
			b_t *n47, b_t *n46, b_t *n45, b_t *n44, b_t *n43, b_t *n42, b_t *n41, b_t *n40,
			b_t *n39, b_t *n38, b_t *n37, b_t *n36, b_t *n35, b_t *n34, b_t *n33, b_t *n32,
			b_t *n31, b_t *n30, b_t *n29, b_t *n28, b_t *n27, b_t *n26, b_t *n25, b_t *n24,
			b_t *n23, b_t *n22, b_t *n21, b_t *n20, b_t *n19, b_t *n18, b_t *n17, b_t *n16,
			b_t *n15, b_t *n14, b_t *n13, b_t *n12, b_t *n11, b_t *n10, b_t *n9, b_t *n8,
			b_t *n7, b_t *n6, b_t *n5, b_t *n4, b_t *n3, b_t *n2, b_t *n1, b_t *n0);
		void espress_cipher_uint1024_va_set(struct espress_cipher_uint1024 *this, va_list *np);
		void espress_cipher_uint1024_va_get(struct espress_cipher_uint1024 const *this, va_list *np);
		void espress_cipher_uint1024_set_u(struct espress_cipher_uint1024 *this, u_t n);
		void espress_cipher_uint1024_get_u(struct espress_cipher_uint1024 const *this, u_t *n);
		void espress_cipher_uint1024_assign(struct espress_cipher_uint1024 *this, struct espress_cipher_uint1024 const *n);
		i_t espress_cipher_uint1024_add(struct espress_cipher_uint1024 *this, struct espress_cipher_uint1024 const *n);
		i_t espress_cipher_uint1024_sub(struct espress_cipher_uint1024 *this, struct espress_cipher_uint1024 const *n);
		void espress_cipher_uint1024_mul(struct espress_cipher_uint1024 *this, struct espress_cipher_uint1024 const *n);
		void espress_cipher_uint1024_div(struct espress_cipher_uint1024 *this, struct espress_cipher_uint1024 const *n);
		void espress_cipher_uint1024_and(struct espress_cipher_uint1024 *this, struct espress_cipher_uint1024 const *n);
		void espress_cipher_uint1024_or(struct espress_cipher_uint1024 *this, struct espress_cipher_uint1024 const *n);
		void espress_cipher_uint1024_xor(struct espress_cipher_uint1024 *this, struct espress_cipher_uint1024 const *n);
		void espress_cipher_uint1024_not(struct espress_cipher_uint1024 *this);
		void espress_cipher_uint1024_shift(struct espress_cipher_uint1024 *this, i_t n);
		void espress_cipher_uint1024_rotate(struct espress_cipher_uint1024 *this, i_t n);
		i_t espress_cipher_uint1024_eq(struct espress_cipher_uint1024 const *this, struct espress_cipher_uint1024 const *n);
		i_t espress_cipher_uint1024_lt(struct espress_cipher_uint1024 const *this, struct espress_cipher_uint1024 const *n);
		i_t espress_cipher_uint1024_le(struct espress_cipher_uint1024 const *this, struct espress_cipher_uint1024 const *n);
		void espress_cipher_uint1024_bswap(struct espress_cipher_uint1024 *this);

#		if ESPRESS_CIPHER_IS_HOST_BYTE_ORDER_LITTLE
#			define ESPRESS_CIPHER_UINT1024(n127, n126, n125, n124, n123, n122, n121, n120, n119, n118, n117, n116, n115, n114, n113, n112, n111, n110, n109, n108, n107, n106, n105, n104, n103, n102, n101, n100, n99, n98, n97, n96, \
				n95, n94, n93, n92, n91, n90, n89, n88, n87, n86, n85, n84, n83, n82, n81, n80, n79, n78, n77, n76, n75, n74, n73, n72, n71, n70, n69, n68, n67, n66, n65, n64, \
				n63, n62, n61, n60, n59, n58, n57, n56, n55, n54, n53, n52, n51, n50, n49, n48, n47, n46, n45, n44, n43, n42, n41, n40, n39, n38, n37, n36, n35, n34, n33, n32, \
				n31, n30, n29, n28, n27, n26, n25, n24, n23, n22, n21, n20, n19, n18, n17, n16, n15, n14, n13, n12, n11, n10, n9, n8, n7, n6, n5, n4, n3, n2, n1, n0) \
				{ \
					ESPRESS_CIPHER_UINT512(n63, n62, n61, n60, n59, n58, n57, n56, n55, n54, n53, n52, n51, n50, n49, n48, n47, n46, n45, n44, n43, n42, n41, n40, n39, n38, n37, n36, n35, n34, n33, n32, \
						n31, n30, n29, n28, n27, n26, n25, n24, n23, n22, n21, n20, n19, n18, n17, n16, n15, n14, n13, n12, n11, n10, n9, n8, n7, n6, n5, n4, n3, n2, n1, n0), \
					ESPRESS_CIPHER_UINT512(n127, n126, n125, n124, n123, n122, n121, n120, n119, n118, n117, n116, n115, n114, n113, n112, n111, n110, n109, n108, n107, n106, n105, n104, n103, n102, n101, n100, n99, n98, n97, n96, \
						n95, n94, n93, n92, n91, n90, n89, n88, n87, n86, n85, n84, n83, n82, n81, n80, n79, n78, n77, n76, n75, n74, n73, n72, n71, n70, n69, n68, n67, n66, n65, n64) \
				}
#		else
#			define ESPRESS_CIPHER_UINT1024(n127, n126, n125, n124, n123, n122, n121, n120, n119, n118, n117, n116, n115, n114, n113, n112, n111, n110, n109, n108, n107, n106, n105, n104, n103, n102, n101, n100, n99, n98, n97, n96, \
				n95, n94, n93, n92, n91, n90, n89, n88, n87, n86, n85, n84, n83, n82, n81, n80, n79, n78, n77, n76, n75, n74, n73, n72, n71, n70, n69, n68, n67, n66, n65, n64, \
				n63, n62, n61, n60, n59, n58, n57, n56, n55, n54, n53, n52, n51, n50, n49, n48, n47, n46, n45, n44, n43, n42, n41, n40, n39, n38, n37, n36, n35, n34, n33, n32, \
				n31, n30, n29, n28, n27, n26, n25, n24, n23, n22, n21, n20, n19, n18, n17, n16, n15, n14, n13, n12, n11, n10, n9, n8, n7, n6, n5, n4, n3, n2, n1, n0) \
				{ \
					ESPRESS_CIPHER_UINT512(n127, n126, n125, n124, n123, n122, n121, n120, n119, n118, n117, n116, n115, n114, n113, n112, n111, n110, n109, n108, n107, n106, n105, n104, n103, n102, n101, n100, n99, n98, n97, n96, \
						n95, n94, n93, n92, n91, n90, n89, n88, n87, n86, n85, n84, n83, n82, n81, n80, n79, n78, n77, n76, n75, n74, n73, n72, n71, n70, n69, n68, n67, n66, n65, n64) \
					ESPRESS_CIPHER_UINT512(n63, n62, n61, n60, n59, n58, n57, n56, n55, n54, n53, n52, n51, n50, n49, n48, n47, n46, n45, n44, n43, n42, n41, n40, n39, n38, n37, n36, n35, n34, n33, n32, \
						n31, n30, n29, n28, n27, n26, n25, n24, n23, n22, n21, n20, n19, n18, n17, n16, n15, n14, n13, n12, n11, n10, n9, n8, n7, n6, n5, n4, n3, n2, n1, n0) \
				}
#		endif
#	endif

	extern struct espress_cipher_uint1024 const espress_cipher_uint1024_0[];
	extern struct espress_cipher_uint1024 const espress_cipher_uint1024_1[];
	extern struct espress_cipher_uint1024 const espress_cipher_uint1024_max[];

#	define ESPRESS_CIPHER_UINT1024_0 ESPRESS_CIPHER_UINT1024(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, \
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
#	define ESPRESS_CIPHER_UINT1024_1 ESPRESS_CIPHER_UINT1024(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, \
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1)
#	define ESPRESS_CIPHER_UINT1024_MAX ESPRESS_CIPHER_UINT1024( \
		0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, \
		0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, \
		0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, \
		0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff \
		)
#	if ESPRESS_CIPHER_IS_HOST_BYTE_ORDER_LITTLE
#		define espress_cipher_uint1024_htol(this) ((void)(this))
#		define espress_cipher_uint1024_htob(this) espress_cipher_uint1024_bswap(this)
#	else
#		define espress_cipher_uint1024_htol(this) espress_cipher_uint1024_bswap(this)
#		define espress_cipher_uint1024_htob(this) ((void)(this))
#	endif
#	define espress_cipher_uint1024_ltoh(this) espress_cipher_uint1024_htol(this)
#	define espress_cipher_uint1024_btoh(this) espress_cipher_uint1024_htob(this)
#	if ESPRESS_CIPHER_IS_NETWORK_BYTE_ORDER_LITTLE
#		define espress_cipher_uint1024_hton(this) espress_cipher_uint1024_htol(this)
#	else
#		define espress_cipher_uint1024_hton(this) espress_cipher_uint1024_htob(this)
#	endif
#	define espress_cipher_uint1024_ntoh(this) espress_cipher_uint1024_hton(this)
#endif

#if 256 <= ESPRESS_CIPHER_UINT_MAX_WIDTH
#	if 256 <= MAX_U_T_WIDTH
#		error
#	else
#		pragma pack(push, 1)
		struct espress_cipher_uint2048
		{
#		if ESPRESS_CIPHER_IS_HOST_BYTE_ORDER_LITTLE
			struct espress_cipher_uint1024 l;
			struct espress_cipher_uint1024 h;
#		else
			struct espress_cipher_uint1024 h;
			struct espress_cipher_uint1024 l;
#		endif
		};
#		pragma pack(pop)

		void espress_cipher_uint2048_set(struct espress_cipher_uint2048 *this,
			b_t n255, b_t n254, b_t n253, b_t n252, b_t n251, b_t n250, b_t n249, b_t n248,
			b_t n247, b_t n246, b_t n245, b_t n244, b_t n243, b_t n242, b_t n241, b_t n240,
			b_t n239, b_t n238, b_t n237, b_t n236, b_t n235, b_t n234, b_t n233, b_t n232,
			b_t n231, b_t n230, b_t n229, b_t n228, b_t n227, b_t n226, b_t n225, b_t n224,
			b_t n223, b_t n222, b_t n221, b_t n220, b_t n219, b_t n218, b_t n217, b_t n216,
			b_t n215, b_t n214, b_t n213, b_t n212, b_t n211, b_t n210, b_t n209, b_t n208,
			b_t n207, b_t n206, b_t n205, b_t n204, b_t n203, b_t n202, b_t n201, b_t n200,
			b_t n199, b_t n198, b_t n197, b_t n196, b_t n195, b_t n194, b_t n193, b_t n192,
			b_t n191, b_t n190, b_t n189, b_t n188, b_t n187, b_t n186, b_t n185, b_t n184,
			b_t n183, b_t n182, b_t n181, b_t n180, b_t n179, b_t n178, b_t n177, b_t n176,
			b_t n175, b_t n174, b_t n173, b_t n172, b_t n171, b_t n170, b_t n169, b_t n168,
			b_t n167, b_t n166, b_t n165, b_t n164, b_t n163, b_t n162, b_t n161, b_t n160,
			b_t n159, b_t n158, b_t n157, b_t n156, b_t n155, b_t n154, b_t n153, b_t n152,
			b_t n151, b_t n150, b_t n149, b_t n148, b_t n147, b_t n146, b_t n145, b_t n144,
			b_t n143, b_t n142, b_t n141, b_t n140, b_t n139, b_t n138, b_t n137, b_t n136,
			b_t n135, b_t n134, b_t n133, b_t n132, b_t n131, b_t n130, b_t n129, b_t n128, 
			b_t n127, b_t n126, b_t n125, b_t n124, b_t n123, b_t n122, b_t n121, b_t n120,
			b_t n119, b_t n118, b_t n117, b_t n116, b_t n115, b_t n114, b_t n113, b_t n112,
			b_t n111, b_t n110, b_t n109, b_t n108, b_t n107, b_t n106, b_t n105, b_t n104,
			b_t n103, b_t n102, b_t n101, b_t n100, b_t n99, b_t n98, b_t n97, b_t n96,
			b_t n95, b_t n94, b_t n93, b_t n92, b_t n91, b_t n90, b_t n89, b_t n88,
			b_t n87, b_t n86, b_t n85, b_t n84, b_t n83, b_t n82, b_t n81, b_t n80,
			b_t n79, b_t n78, b_t n77, b_t n76, b_t n75, b_t n74, b_t n73, b_t n72,
			b_t n71, b_t n70, b_t n69, b_t n68, b_t n67, b_t n66, b_t n65, b_t n64,
			b_t n63, b_t n62, b_t n61, b_t n60, b_t n59, b_t n58, b_t n57, b_t n56,
			b_t n55, b_t n54, b_t n53, b_t n52, b_t n51, b_t n50, b_t n49, b_t n48,
			b_t n47, b_t n46, b_t n45, b_t n44, b_t n43, b_t n42, b_t n41, b_t n40,
			b_t n39, b_t n38, b_t n37, b_t n36, b_t n35, b_t n34, b_t n33, b_t n32,
			b_t n31, b_t n30, b_t n29, b_t n28, b_t n27, b_t n26, b_t n25, b_t n24,
			b_t n23, b_t n22, b_t n21, b_t n20, b_t n19, b_t n18, b_t n17, b_t n16,
			b_t n15, b_t n14, b_t n13, b_t n12, b_t n11, b_t n10, b_t n9, b_t n8,
			b_t n7, b_t n6, b_t n5, b_t n4, b_t n3, b_t n2, b_t n1, b_t n0);
		void espress_cipher_uint2048_get(struct espress_cipher_uint2048 const *this,
			b_t *n255, b_t *n254, b_t *n253, b_t *n252, b_t *n251, b_t *n250, b_t *n249, b_t *n248,
			b_t *n247, b_t *n246, b_t *n245, b_t *n244, b_t *n243, b_t *n242, b_t *n241, b_t *n240,
			b_t *n239, b_t *n238, b_t *n237, b_t *n236, b_t *n235, b_t *n234, b_t *n233, b_t *n232,
			b_t *n231, b_t *n230, b_t *n229, b_t *n228, b_t *n227, b_t *n226, b_t *n225, b_t *n224,
			b_t *n223, b_t *n222, b_t *n221, b_t *n220, b_t *n219, b_t *n218, b_t *n217, b_t *n216,
			b_t *n215, b_t *n214, b_t *n213, b_t *n212, b_t *n211, b_t *n210, b_t *n209, b_t *n208,
			b_t *n207, b_t *n206, b_t *n205, b_t *n204, b_t *n203, b_t *n202, b_t *n201, b_t *n200,
			b_t *n199, b_t *n198, b_t *n197, b_t *n196, b_t *n195, b_t *n194, b_t *n193, b_t *n192,
			b_t *n191, b_t *n190, b_t *n189, b_t *n188, b_t *n187, b_t *n186, b_t *n185, b_t *n184,
			b_t *n183, b_t *n182, b_t *n181, b_t *n180, b_t *n179, b_t *n178, b_t *n177, b_t *n176,
			b_t *n175, b_t *n174, b_t *n173, b_t *n172, b_t *n171, b_t *n170, b_t *n169, b_t *n168,
			b_t *n167, b_t *n166, b_t *n165, b_t *n164, b_t *n163, b_t *n162, b_t *n161, b_t *n160,
			b_t *n159, b_t *n158, b_t *n157, b_t *n156, b_t *n155, b_t *n154, b_t *n153, b_t *n152,
			b_t *n151, b_t *n150, b_t *n149, b_t *n148, b_t *n147, b_t *n146, b_t *n145, b_t *n144,
			b_t *n143, b_t *n142, b_t *n141, b_t *n140, b_t *n139, b_t *n138, b_t *n137, b_t *n136,
			b_t *n135, b_t *n134, b_t *n133, b_t *n132, b_t *n131, b_t *n130, b_t *n129, b_t *n128, 
			b_t *n127, b_t *n126, b_t *n125, b_t *n124, b_t *n123, b_t *n122, b_t *n121, b_t *n120,
			b_t *n119, b_t *n118, b_t *n117, b_t *n116, b_t *n115, b_t *n114, b_t *n113, b_t *n112,
			b_t *n111, b_t *n110, b_t *n109, b_t *n108, b_t *n107, b_t *n106, b_t *n105, b_t *n104,
			b_t *n103, b_t *n102, b_t *n101, b_t *n100, b_t *n99, b_t *n98, b_t *n97, b_t *n96,
			b_t *n95, b_t *n94, b_t *n93, b_t *n92, b_t *n91, b_t *n90, b_t *n89, b_t *n88,
			b_t *n87, b_t *n86, b_t *n85, b_t *n84, b_t *n83, b_t *n82, b_t *n81, b_t *n80,
			b_t *n79, b_t *n78, b_t *n77, b_t *n76, b_t *n75, b_t *n74, b_t *n73, b_t *n72,
			b_t *n71, b_t *n70, b_t *n69, b_t *n68, b_t *n67, b_t *n66, b_t *n65, b_t *n64,
			b_t *n63, b_t *n62, b_t *n61, b_t *n60, b_t *n59, b_t *n58, b_t *n57, b_t *n56,
			b_t *n55, b_t *n54, b_t *n53, b_t *n52, b_t *n51, b_t *n50, b_t *n49, b_t *n48,
			b_t *n47, b_t *n46, b_t *n45, b_t *n44, b_t *n43, b_t *n42, b_t *n41, b_t *n40,
			b_t *n39, b_t *n38, b_t *n37, b_t *n36, b_t *n35, b_t *n34, b_t *n33, b_t *n32,
			b_t *n31, b_t *n30, b_t *n29, b_t *n28, b_t *n27, b_t *n26, b_t *n25, b_t *n24,
			b_t *n23, b_t *n22, b_t *n21, b_t *n20, b_t *n19, b_t *n18, b_t *n17, b_t *n16,
			b_t *n15, b_t *n14, b_t *n13, b_t *n12, b_t *n11, b_t *n10, b_t *n9, b_t *n8,
			b_t *n7, b_t *n6, b_t *n5, b_t *n4, b_t *n3, b_t *n2, b_t *n1, b_t *n0);
		void espress_cipher_uint2048_va_set(struct espress_cipher_uint2048 *this, va_list *np);
		void espress_cipher_uint2048_va_get(struct espress_cipher_uint2048 const *this, va_list *np);
		void espress_cipher_uint2048_set_u(struct espress_cipher_uint2048 *this, u_t n);
		void espress_cipher_uint2048_get_u(struct espress_cipher_uint2048 const *this, u_t *n);
		void espress_cipher_uint2048_assign(struct espress_cipher_uint2048 *this, struct espress_cipher_uint2048 const *n);
		i_t espress_cipher_uint2048_add(struct espress_cipher_uint2048 *this, struct espress_cipher_uint2048 const *n);
		i_t espress_cipher_uint2048_sub(struct espress_cipher_uint2048 *this, struct espress_cipher_uint2048 const *n);
		void espress_cipher_uint2048_mul(struct espress_cipher_uint2048 *this, struct espress_cipher_uint2048 const *n);
		void espress_cipher_uint2048_div(struct espress_cipher_uint2048 *this, struct espress_cipher_uint2048 const *n);
		void espress_cipher_uint2048_and(struct espress_cipher_uint2048 *this, struct espress_cipher_uint2048 const *n);
		void espress_cipher_uint2048_or(struct espress_cipher_uint2048 *this, struct espress_cipher_uint2048 const *n);
		void espress_cipher_uint2048_xor(struct espress_cipher_uint2048 *this, struct espress_cipher_uint2048 const *n);
		void espress_cipher_uint2048_not(struct espress_cipher_uint2048 *this);
		void espress_cipher_uint2048_shift(struct espress_cipher_uint2048 *this, i_t n);
		void espress_cipher_uint2048_rotate(struct espress_cipher_uint2048 *this, i_t n);
		i_t espress_cipher_uint2048_eq(struct espress_cipher_uint2048 const *this, struct espress_cipher_uint2048 const *n);
		i_t espress_cipher_uint2048_lt(struct espress_cipher_uint2048 const *this, struct espress_cipher_uint2048 const *n);
		i_t espress_cipher_uint2048_le(struct espress_cipher_uint2048 const *this, struct espress_cipher_uint2048 const *n);
		void espress_cipher_uint2048_bswap(struct espress_cipher_uint2048 *this);

#		if ESPRESS_CIPHER_IS_HOST_BYTE_ORDER_LITTLE
#			define ESPRESS_CIPHER_UINT2048(n255, n254, n253, n252, n251, n250, n249, n248, n247, n246, n245, n244, n243, n242, n241, n240, n239, n238, n237, n236, n235, n234, n233, n232, n231, n230, n229, n228, n227, n226, n225, n224, \
				n223, n222, n221, n220, n219, n218, n217, n216, n215, n214, n213, n212, n211, n210, n209, n208, n207, n206, n205, n204, n203, n202, n201, n200, n199, n198, n197, n196, n195, n194, n193, n192, \
				n191, n190, n189, n188, n187, n186, n185, n184, n183, n182, n181, n180, n179, n178, n177, n176, n175, n174, n173, n172, n171, n170, n169, n168, n167, n166, n165, n164, n163, n162, n161, n160, \
				n159, n158, n157, n156, n155, n154, n153, n152, n151, n150, n149, n148, n147, n146, n145, n144, n143, n142, n141, n140, n139, n138, n137, n136, n135, n134, n133, n132, n131, n130, n129, n128, \
				n127, n126, n125, n124, n123, n122, n121, n120, n119, n118, n117, n116, n115, n114, n113, n112, n111, n110, n109, n108, n107, n106, n105, n104, n103, n102, n101, n100, n99, n98, n97, n96, \
				n95, n94, n93, n92, n91, n90, n89, n88, n87, n86, n85, n84, n83, n82, n81, n80, n79, n78, n77, n76, n75, n74, n73, n72, n71, n70, n69, n68, n67, n66, n65, n64, \
				n63, n62, n61, n60, n59, n58, n57, n56, n55, n54, n53, n52, n51, n50, n49, n48, n47, n46, n45, n44, n43, n42, n41, n40, n39, n38, n37, n36, n35, n34, n33, n32, \
				n31, n30, n29, n28, n27, n26, n25, n24, n23, n22, n21, n20, n19, n18, n17, n16, n15, n14, n13, n12, n11, n10, n9, n8, n7, n6, n5, n4, n3, n2, n1, n0) \
				{ \
					ESPRESS_CIPHER_UINT1024(n127, n126, n125, n124, n123, n122, n121, n120, n119, n118, n117, n116, n115, n114, n113, n112, n111, n110, n109, n108, n107, n106, n105, n104, n103, n102, n101, n100, n99, n98, n97, n96, \
						n95, n94, n93, n92, n91, n90, n89, n88, n87, n86, n85, n84, n83, n82, n81, n80, n79, n78, n77, n76, n75, n74, n73, n72, n71, n70, n69, n68, n67, n66, n65, n64, \
						n63, n62, n61, n60, n59, n58, n57, n56, n55, n54, n53, n52, n51, n50, n49, n48, n47, n46, n45, n44, n43, n42, n41, n40, n39, n38, n37, n36, n35, n34, n33, n32, \
						n31, n30, n29, n28, n27, n26, n25, n24, n23, n22, n21, n20, n19, n18, n17, n16, n15, n14, n13, n12, n11, n10, n9, n8, n7, n6, n5, n4, n3, n2, n1, n0), \
					ESPRESS_CIPHER_UINT1024(n255, n254, n253, n252, n251, n250, n249, n248, n247, n246, n245, n244, n243, n242, n241, n240, n239, n238, n237, n236, n235, n234, n233, n232, n231, n230, n229, n228, n227, n226, n225, n224, \
						n223, n222, n221, n220, n219, n218, n217, n216, n215, n214, n213, n212, n211, n210, n209, n208, n207, n206, n205, n204, n203, n202, n201, n200, n199, n198, n197, n196, n195, n194, n193, n192, \
						n191, n190, n189, n188, n187, n186, n185, n184, n183, n182, n181, n180, n179, n178, n177, n176, n175, n174, n173, n172, n171, n170, n169, n168, n167, n166, n165, n164, n163, n162, n161, n160, \
						n159, n158, n157, n156, n155, n154, n153, n152, n151, n150, n149, n148, n147, n146, n145, n144, n143, n142, n141, n140, n139, n138, n137, n136, n135, n134, n133, n132, n131, n130, n129, n128) \
				}
#		else
#			define ESPRESS_CIPHER_UINT2048(n255, n254, n253, n252, n251, n250, n249, n248, n247, n246, n245, n244, n243, n242, n241, n240, n239, n238, n237, n236, n235, n234, n233, n232, n231, n230, n229, n228, n227, n226, n225, n224, \
				n223, n222, n221, n220, n219, n218, n217, n216, n215, n214, n213, n212, n211, n210, n209, n208, n207, n206, n205, n204, n203, n202, n201, n200, n199, n198, n197, n196, n195, n194, n193, n192, \
				n191, n190, n189, n188, n187, n186, n185, n184, n183, n182, n181, n180, n179, n178, n177, n176, n175, n174, n173, n172, n171, n170, n169, n168, n167, n166, n165, n164, n163, n162, n161, n160, \
				n159, n158, n157, n156, n155, n154, n153, n152, n151, n150, n149, n148, n147, n146, n145, n144, n143, n142, n141, n140, n139, n138, n137, n136, n135, n134, n133, n132, n131, n130, n129, n128, \
				n127, n126, n125, n124, n123, n122, n121, n120, n119, n118, n117, n116, n115, n114, n113, n112, n111, n110, n109, n108, n107, n106, n105, n104, n103, n102, n101, n100, n99, n98, n97, n96, \
				n95, n94, n93, n92, n91, n90, n89, n88, n87, n86, n85, n84, n83, n82, n81, n80, n79, n78, n77, n76, n75, n74, n73, n72, n71, n70, n69, n68, n67, n66, n65, n64, \
				n63, n62, n61, n60, n59, n58, n57, n56, n55, n54, n53, n52, n51, n50, n49, n48, n47, n46, n45, n44, n43, n42, n41, n40, n39, n38, n37, n36, n35, n34, n33, n32, \
				n31, n30, n29, n28, n27, n26, n25, n24, n23, n22, n21, n20, n19, n18, n17, n16, n15, n14, n13, n12, n11, n10, n9, n8, n7, n6, n5, n4, n3, n2, n1, n0) \
				{ \
					ESPRESS_CIPHER_UINT1024(n255, n254, n253, n252, n251, n250, n249, n248, n247, n246, n245, n244, n243, n242, n241, n240, n239, n238, n237, n236, n235, n234, n233, n232, n231, n230, n229, n228, n227, n226, n225, n224, \
						n223, n222, n221, n220, n219, n218, n217, n216, n215, n214, n213, n212, n211, n210, n209, n208, n207, n206, n205, n204, n203, n202, n201, n200, n199, n198, n197, n196, n195, n194, n193, n192, \
						n191, n190, n189, n188, n187, n186, n185, n184, n183, n182, n181, n180, n179, n178, n177, n176, n175, n174, n173, n172, n171, n170, n169, n168, n167, n166, n165, n164, n163, n162, n161, n160, \
						n159, n158, n157, n156, n155, n154, n153, n152, n151, n150, n149, n148, n147, n146, n145, n144, n143, n142, n141, n140, n139, n138, n137, n136, n135, n134, n133, n132, n131, n130, n129, n128), \
					ESPRESS_CIPHER_UINT1024(n127, n126, n125, n124, n123, n122, n121, n120, n119, n118, n117, n116, n115, n114, n113, n112, n111, n110, n109, n108, n107, n106, n105, n104, n103, n102, n101, n100, n99, n98, n97, n96, \
						n95, n94, n93, n92, n91, n90, n89, n88, n87, n86, n85, n84, n83, n82, n81, n80, n79, n78, n77, n76, n75, n74, n73, n72, n71, n70, n69, n68, n67, n66, n65, n64, \
						n63, n62, n61, n60, n59, n58, n57, n56, n55, n54, n53, n52, n51, n50, n49, n48, n47, n46, n45, n44, n43, n42, n41, n40, n39, n38, n37, n36, n35, n34, n33, n32, \
						n31, n30, n29, n28, n27, n26, n25, n24, n23, n22, n21, n20, n19, n18, n17, n16, n15, n14, n13, n12, n11, n10, n9, n8, n7, n6, n5, n4, n3, n2, n1, n0) \
				}
#		endif
#	endif

	extern struct espress_cipher_uint2048 const espress_cipher_uint2048_0[];
	extern struct espress_cipher_uint2048 const espress_cipher_uint2048_1[];
	extern struct espress_cipher_uint2048 const espress_cipher_uint2048_max[];

#	define ESPRESS_CIPHER_UINT2048_0 ESPRESS_CIPHER_UINT2048(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, \
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, \
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, \
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
#	define ESPRESS_CIPHER_UINT2048_1 ESPRESS_CIPHER_UINT2048(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, \
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, \
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, \
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1)
#	define ESPRESS_CIPHER_UINT2048_MAX ESPRESS_CIPHER_UINT2048( \
		0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, \
		0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, \
		0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, \
		0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, \
		0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, \
		0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, \
		0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, \
		0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff \
		)
#	if ESPRESS_CIPHER_IS_HOST_BYTE_ORDER_LITTLE
#		define espress_cipher_uint2048_htol(this) ((void)(this))
#		define espress_cipher_uint2048_htob(this) espress_cipher_uint2048_bswap(this)
#	else
#		define espress_cipher_uint2048_htol(this) espress_cipher_uint2048_bswap(this)
#		define espress_cipher_uint2048_htob(this) ((void)(this))
#	endif
#	define espress_cipher_uint2048_ltoh(this) espress_cipher_uint2048_htol(this)
#	define espress_cipher_uint2048_btoh(this) espress_cipher_uint2048_htob(this)
#	if ESPRESS_CIPHER_IS_NETWORK_BYTE_ORDER_LITTLE
#		define espress_cipher_uint2048_hton(this) espress_cipher_uint2048_htol(this)
#	else
#		define espress_cipher_uint2048_hton(this) espress_cipher_uint2048_htob(this)
#	endif
#	define espress_cipher_uint2048_ntoh(this) espress_cipher_uint2048_hton(this)
#endif

#if 512 <= ESPRESS_CIPHER_UINT_MAX_WIDTH
#	if 512 <= MAX_U_T_WIDTH
#		error
#	else
#		pragma pack(push, 1)
		struct espress_cipher_uint4096
		{
#		if ESPRESS_CIPHER_IS_HOST_BYTE_ORDER_LITTLE
			struct espress_cipher_uint2048 l;
			struct espress_cipher_uint2048 h;
#		else
			struct espress_cipher_uint2048 h;
			struct espress_cipher_uint2048 l;
#		endif
		};
#		pragma pack(pop)

		void espress_cipher_uint4096_set(struct espress_cipher_uint4096 *this,
			b_t n511, b_t n510, b_t n509, b_t n508, b_t n507, b_t n506, b_t n505, b_t n504,
			b_t n503, b_t n502, b_t n501, b_t n500, b_t n499, b_t n498, b_t n497, b_t n496,
			b_t n495, b_t n494, b_t n493, b_t n492, b_t n491, b_t n490, b_t n489, b_t n488,
			b_t n487, b_t n486, b_t n485, b_t n484, b_t n483, b_t n482, b_t n481, b_t n480,
			b_t n479, b_t n478, b_t n477, b_t n476, b_t n475, b_t n474, b_t n473, b_t n472,
			b_t n471, b_t n470, b_t n469, b_t n468, b_t n467, b_t n466, b_t n465, b_t n464,
			b_t n463, b_t n462, b_t n461, b_t n460, b_t n459, b_t n458, b_t n457, b_t n456,
			b_t n455, b_t n454, b_t n453, b_t n452, b_t n451, b_t n450, b_t n449, b_t n448,
			b_t n447, b_t n446, b_t n445, b_t n444, b_t n443, b_t n442, b_t n441, b_t n440,
			b_t n439, b_t n438, b_t n437, b_t n436, b_t n435, b_t n434, b_t n433, b_t n432,
			b_t n431, b_t n430, b_t n429, b_t n428, b_t n427, b_t n426, b_t n425, b_t n424,
			b_t n423, b_t n422, b_t n421, b_t n420, b_t n419, b_t n418, b_t n417, b_t n416,
			b_t n415, b_t n414, b_t n413, b_t n412, b_t n411, b_t n410, b_t n409, b_t n408,
			b_t n407, b_t n406, b_t n405, b_t n404, b_t n403, b_t n402, b_t n401, b_t n400,
			b_t n399, b_t n398, b_t n397, b_t n396, b_t n395, b_t n394, b_t n393, b_t n392,
			b_t n391, b_t n390, b_t n389, b_t n388, b_t n387, b_t n386, b_t n385, b_t n384,
			b_t n383, b_t n382, b_t n381, b_t n380, b_t n379, b_t n378, b_t n377, b_t n376,
			b_t n375, b_t n374, b_t n373, b_t n372, b_t n371, b_t n370, b_t n369, b_t n368,
			b_t n367, b_t n366, b_t n365, b_t n364, b_t n363, b_t n362, b_t n361, b_t n360,
			b_t n359, b_t n358, b_t n357, b_t n356, b_t n355, b_t n354, b_t n353, b_t n352,
			b_t n351, b_t n350, b_t n349, b_t n348, b_t n347, b_t n346, b_t n345, b_t n344,
			b_t n343, b_t n342, b_t n341, b_t n340, b_t n339, b_t n338, b_t n337, b_t n336,
			b_t n335, b_t n334, b_t n333, b_t n332, b_t n331, b_t n330, b_t n329, b_t n328,
			b_t n327, b_t n326, b_t n325, b_t n324, b_t n323, b_t n322, b_t n321, b_t n320,
			b_t n319, b_t n318, b_t n317, b_t n316, b_t n315, b_t n314, b_t n313, b_t n312,
			b_t n311, b_t n310, b_t n309, b_t n308, b_t n307, b_t n306, b_t n305, b_t n304,
			b_t n303, b_t n302, b_t n301, b_t n300, b_t n299, b_t n298, b_t n297, b_t n296,
			b_t n295, b_t n294, b_t n293, b_t n292, b_t n291, b_t n290, b_t n289, b_t n288,
			b_t n287, b_t n286, b_t n285, b_t n284, b_t n283, b_t n282, b_t n281, b_t n280,
			b_t n279, b_t n278, b_t n277, b_t n276, b_t n275, b_t n274, b_t n273, b_t n272,
			b_t n271, b_t n270, b_t n269, b_t n268, b_t n267, b_t n266, b_t n265, b_t n264,
			b_t n263, b_t n262, b_t n261, b_t n260, b_t n259, b_t n258, b_t n257, b_t n256,
			b_t n255, b_t n254, b_t n253, b_t n252, b_t n251, b_t n250, b_t n249, b_t n248,
			b_t n247, b_t n246, b_t n245, b_t n244, b_t n243, b_t n242, b_t n241, b_t n240,
			b_t n239, b_t n238, b_t n237, b_t n236, b_t n235, b_t n234, b_t n233, b_t n232,
			b_t n231, b_t n230, b_t n229, b_t n228, b_t n227, b_t n226, b_t n225, b_t n224,
			b_t n223, b_t n222, b_t n221, b_t n220, b_t n219, b_t n218, b_t n217, b_t n216,
			b_t n215, b_t n214, b_t n213, b_t n212, b_t n211, b_t n210, b_t n209, b_t n208,
			b_t n207, b_t n206, b_t n205, b_t n204, b_t n203, b_t n202, b_t n201, b_t n200,
			b_t n199, b_t n198, b_t n197, b_t n196, b_t n195, b_t n194, b_t n193, b_t n192,
			b_t n191, b_t n190, b_t n189, b_t n188, b_t n187, b_t n186, b_t n185, b_t n184,
			b_t n183, b_t n182, b_t n181, b_t n180, b_t n179, b_t n178, b_t n177, b_t n176,
			b_t n175, b_t n174, b_t n173, b_t n172, b_t n171, b_t n170, b_t n169, b_t n168,
			b_t n167, b_t n166, b_t n165, b_t n164, b_t n163, b_t n162, b_t n161, b_t n160,
			b_t n159, b_t n158, b_t n157, b_t n156, b_t n155, b_t n154, b_t n153, b_t n152,
			b_t n151, b_t n150, b_t n149, b_t n148, b_t n147, b_t n146, b_t n145, b_t n144,
			b_t n143, b_t n142, b_t n141, b_t n140, b_t n139, b_t n138, b_t n137, b_t n136,
			b_t n135, b_t n134, b_t n133, b_t n132, b_t n131, b_t n130, b_t n129, b_t n128, 
			b_t n127, b_t n126, b_t n125, b_t n124, b_t n123, b_t n122, b_t n121, b_t n120,
			b_t n119, b_t n118, b_t n117, b_t n116, b_t n115, b_t n114, b_t n113, b_t n112,
			b_t n111, b_t n110, b_t n109, b_t n108, b_t n107, b_t n106, b_t n105, b_t n104,
			b_t n103, b_t n102, b_t n101, b_t n100, b_t n99, b_t n98, b_t n97, b_t n96,
			b_t n95, b_t n94, b_t n93, b_t n92, b_t n91, b_t n90, b_t n89, b_t n88,
			b_t n87, b_t n86, b_t n85, b_t n84, b_t n83, b_t n82, b_t n81, b_t n80,
			b_t n79, b_t n78, b_t n77, b_t n76, b_t n75, b_t n74, b_t n73, b_t n72,
			b_t n71, b_t n70, b_t n69, b_t n68, b_t n67, b_t n66, b_t n65, b_t n64,
			b_t n63, b_t n62, b_t n61, b_t n60, b_t n59, b_t n58, b_t n57, b_t n56,
			b_t n55, b_t n54, b_t n53, b_t n52, b_t n51, b_t n50, b_t n49, b_t n48,
			b_t n47, b_t n46, b_t n45, b_t n44, b_t n43, b_t n42, b_t n41, b_t n40,
			b_t n39, b_t n38, b_t n37, b_t n36, b_t n35, b_t n34, b_t n33, b_t n32,
			b_t n31, b_t n30, b_t n29, b_t n28, b_t n27, b_t n26, b_t n25, b_t n24,
			b_t n23, b_t n22, b_t n21, b_t n20, b_t n19, b_t n18, b_t n17, b_t n16,
			b_t n15, b_t n14, b_t n13, b_t n12, b_t n11, b_t n10, b_t n9, b_t n8,
			b_t n7, b_t n6, b_t n5, b_t n4, b_t n3, b_t n2, b_t n1, b_t n0);
		void espress_cipher_uint4096_get(struct espress_cipher_uint4096 const *this,
			b_t *n511, b_t *n510, b_t *n509, b_t *n508, b_t *n507, b_t *n506, b_t *n505, b_t *n504,
			b_t *n503, b_t *n502, b_t *n501, b_t *n500, b_t *n499, b_t *n498, b_t *n497, b_t *n496,
			b_t *n495, b_t *n494, b_t *n493, b_t *n492, b_t *n491, b_t *n490, b_t *n489, b_t *n488,
			b_t *n487, b_t *n486, b_t *n485, b_t *n484, b_t *n483, b_t *n482, b_t *n481, b_t *n480,
			b_t *n479, b_t *n478, b_t *n477, b_t *n476, b_t *n475, b_t *n474, b_t *n473, b_t *n472,
			b_t *n471, b_t *n470, b_t *n469, b_t *n468, b_t *n467, b_t *n466, b_t *n465, b_t *n464,
			b_t *n463, b_t *n462, b_t *n461, b_t *n460, b_t *n459, b_t *n458, b_t *n457, b_t *n456,
			b_t *n455, b_t *n454, b_t *n453, b_t *n452, b_t *n451, b_t *n450, b_t *n449, b_t *n448,
			b_t *n447, b_t *n446, b_t *n445, b_t *n444, b_t *n443, b_t *n442, b_t *n441, b_t *n440,
			b_t *n439, b_t *n438, b_t *n437, b_t *n436, b_t *n435, b_t *n434, b_t *n433, b_t *n432,
			b_t *n431, b_t *n430, b_t *n429, b_t *n428, b_t *n427, b_t *n426, b_t *n425, b_t *n424,
			b_t *n423, b_t *n422, b_t *n421, b_t *n420, b_t *n419, b_t *n418, b_t *n417, b_t *n416,
			b_t *n415, b_t *n414, b_t *n413, b_t *n412, b_t *n411, b_t *n410, b_t *n409, b_t *n408,
			b_t *n407, b_t *n406, b_t *n405, b_t *n404, b_t *n403, b_t *n402, b_t *n401, b_t *n400,
			b_t *n399, b_t *n398, b_t *n397, b_t *n396, b_t *n395, b_t *n394, b_t *n393, b_t *n392,
			b_t *n391, b_t *n390, b_t *n389, b_t *n388, b_t *n387, b_t *n386, b_t *n385, b_t *n384,
			b_t *n383, b_t *n382, b_t *n381, b_t *n380, b_t *n379, b_t *n378, b_t *n377, b_t *n376,
			b_t *n375, b_t *n374, b_t *n373, b_t *n372, b_t *n371, b_t *n370, b_t *n369, b_t *n368,
			b_t *n367, b_t *n366, b_t *n365, b_t *n364, b_t *n363, b_t *n362, b_t *n361, b_t *n360,
			b_t *n359, b_t *n358, b_t *n357, b_t *n356, b_t *n355, b_t *n354, b_t *n353, b_t *n352,
			b_t *n351, b_t *n350, b_t *n349, b_t *n348, b_t *n347, b_t *n346, b_t *n345, b_t *n344,
			b_t *n343, b_t *n342, b_t *n341, b_t *n340, b_t *n339, b_t *n338, b_t *n337, b_t *n336,
			b_t *n335, b_t *n334, b_t *n333, b_t *n332, b_t *n331, b_t *n330, b_t *n329, b_t *n328,
			b_t *n327, b_t *n326, b_t *n325, b_t *n324, b_t *n323, b_t *n322, b_t *n321, b_t *n320,
			b_t *n319, b_t *n318, b_t *n317, b_t *n316, b_t *n315, b_t *n314, b_t *n313, b_t *n312,
			b_t *n311, b_t *n310, b_t *n309, b_t *n308, b_t *n307, b_t *n306, b_t *n305, b_t *n304,
			b_t *n303, b_t *n302, b_t *n301, b_t *n300, b_t *n299, b_t *n298, b_t *n297, b_t *n296,
			b_t *n295, b_t *n294, b_t *n293, b_t *n292, b_t *n291, b_t *n290, b_t *n289, b_t *n288,
			b_t *n287, b_t *n286, b_t *n285, b_t *n284, b_t *n283, b_t *n282, b_t *n281, b_t *n280,
			b_t *n279, b_t *n278, b_t *n277, b_t *n276, b_t *n275, b_t *n274, b_t *n273, b_t *n272,
			b_t *n271, b_t *n270, b_t *n269, b_t *n268, b_t *n267, b_t *n266, b_t *n265, b_t *n264,
			b_t *n263, b_t *n262, b_t *n261, b_t *n260, b_t *n259, b_t *n258, b_t *n257, b_t *n256, 
			b_t *n255, b_t *n254, b_t *n253, b_t *n252, b_t *n251, b_t *n250, b_t *n249, b_t *n248,
			b_t *n247, b_t *n246, b_t *n245, b_t *n244, b_t *n243, b_t *n242, b_t *n241, b_t *n240,
			b_t *n239, b_t *n238, b_t *n237, b_t *n236, b_t *n235, b_t *n234, b_t *n233, b_t *n232,
			b_t *n231, b_t *n230, b_t *n229, b_t *n228, b_t *n227, b_t *n226, b_t *n225, b_t *n224,
			b_t *n223, b_t *n222, b_t *n221, b_t *n220, b_t *n219, b_t *n218, b_t *n217, b_t *n216,
			b_t *n215, b_t *n214, b_t *n213, b_t *n212, b_t *n211, b_t *n210, b_t *n209, b_t *n208,
			b_t *n207, b_t *n206, b_t *n205, b_t *n204, b_t *n203, b_t *n202, b_t *n201, b_t *n200,
			b_t *n199, b_t *n198, b_t *n197, b_t *n196, b_t *n195, b_t *n194, b_t *n193, b_t *n192,
			b_t *n191, b_t *n190, b_t *n189, b_t *n188, b_t *n187, b_t *n186, b_t *n185, b_t *n184,
			b_t *n183, b_t *n182, b_t *n181, b_t *n180, b_t *n179, b_t *n178, b_t *n177, b_t *n176,
			b_t *n175, b_t *n174, b_t *n173, b_t *n172, b_t *n171, b_t *n170, b_t *n169, b_t *n168,
			b_t *n167, b_t *n166, b_t *n165, b_t *n164, b_t *n163, b_t *n162, b_t *n161, b_t *n160,
			b_t *n159, b_t *n158, b_t *n157, b_t *n156, b_t *n155, b_t *n154, b_t *n153, b_t *n152,
			b_t *n151, b_t *n150, b_t *n149, b_t *n148, b_t *n147, b_t *n146, b_t *n145, b_t *n144,
			b_t *n143, b_t *n142, b_t *n141, b_t *n140, b_t *n139, b_t *n138, b_t *n137, b_t *n136,
			b_t *n135, b_t *n134, b_t *n133, b_t *n132, b_t *n131, b_t *n130, b_t *n129, b_t *n128, 
			b_t *n127, b_t *n126, b_t *n125, b_t *n124, b_t *n123, b_t *n122, b_t *n121, b_t *n120,
			b_t *n119, b_t *n118, b_t *n117, b_t *n116, b_t *n115, b_t *n114, b_t *n113, b_t *n112,
			b_t *n111, b_t *n110, b_t *n109, b_t *n108, b_t *n107, b_t *n106, b_t *n105, b_t *n104,
			b_t *n103, b_t *n102, b_t *n101, b_t *n100, b_t *n99, b_t *n98, b_t *n97, b_t *n96,
			b_t *n95, b_t *n94, b_t *n93, b_t *n92, b_t *n91, b_t *n90, b_t *n89, b_t *n88,
			b_t *n87, b_t *n86, b_t *n85, b_t *n84, b_t *n83, b_t *n82, b_t *n81, b_t *n80,
			b_t *n79, b_t *n78, b_t *n77, b_t *n76, b_t *n75, b_t *n74, b_t *n73, b_t *n72,
			b_t *n71, b_t *n70, b_t *n69, b_t *n68, b_t *n67, b_t *n66, b_t *n65, b_t *n64,
			b_t *n63, b_t *n62, b_t *n61, b_t *n60, b_t *n59, b_t *n58, b_t *n57, b_t *n56,
			b_t *n55, b_t *n54, b_t *n53, b_t *n52, b_t *n51, b_t *n50, b_t *n49, b_t *n48,
			b_t *n47, b_t *n46, b_t *n45, b_t *n44, b_t *n43, b_t *n42, b_t *n41, b_t *n40,
			b_t *n39, b_t *n38, b_t *n37, b_t *n36, b_t *n35, b_t *n34, b_t *n33, b_t *n32,
			b_t *n31, b_t *n30, b_t *n29, b_t *n28, b_t *n27, b_t *n26, b_t *n25, b_t *n24,
			b_t *n23, b_t *n22, b_t *n21, b_t *n20, b_t *n19, b_t *n18, b_t *n17, b_t *n16,
			b_t *n15, b_t *n14, b_t *n13, b_t *n12, b_t *n11, b_t *n10, b_t *n9, b_t *n8,
			b_t *n7, b_t *n6, b_t *n5, b_t *n4, b_t *n3, b_t *n2, b_t *n1, b_t *n0);
		void espress_cipher_uint4096_va_set(struct espress_cipher_uint4096 *this, va_list *np);
		void espress_cipher_uint4096_va_get(struct espress_cipher_uint4096 const *this, va_list *np);
		void espress_cipher_uint4096_set_u(struct espress_cipher_uint4096 *this, u_t n);
		void espress_cipher_uint4096_get_u(struct espress_cipher_uint4096 const *this, u_t *n);
		void espress_cipher_uint4096_assign(struct espress_cipher_uint4096 *this, struct espress_cipher_uint4096 const *n);
		i_t espress_cipher_uint4096_add(struct espress_cipher_uint4096 *this, struct espress_cipher_uint4096 const *n);
		i_t espress_cipher_uint4096_sub(struct espress_cipher_uint4096 *this, struct espress_cipher_uint4096 const *n);
		void espress_cipher_uint4096_mul(struct espress_cipher_uint4096 *this, struct espress_cipher_uint4096 const *n);
		void espress_cipher_uint4096_div(struct espress_cipher_uint4096 *this, struct espress_cipher_uint4096 const *n);
		void espress_cipher_uint4096_and(struct espress_cipher_uint4096 *this, struct espress_cipher_uint4096 const *n);
		void espress_cipher_uint4096_or(struct espress_cipher_uint4096 *this, struct espress_cipher_uint4096 const *n);
		void espress_cipher_uint4096_xor(struct espress_cipher_uint4096 *this, struct espress_cipher_uint4096 const *n);
		void espress_cipher_uint4096_not(struct espress_cipher_uint4096 *this);
		void espress_cipher_uint4096_shift(struct espress_cipher_uint4096 *this, i_t n);
		void espress_cipher_uint4096_rotate(struct espress_cipher_uint4096 *this, i_t n);
		i_t espress_cipher_uint4096_eq(struct espress_cipher_uint4096 const *this, struct espress_cipher_uint4096 const *n);
		i_t espress_cipher_uint4096_lt(struct espress_cipher_uint4096 const *this, struct espress_cipher_uint4096 const *n);
		i_t espress_cipher_uint4096_le(struct espress_cipher_uint4096 const *this, struct espress_cipher_uint4096 const *n);
		void espress_cipher_uint4096_bswap(struct espress_cipher_uint4096 *this);

#		if ESPRESS_CIPHER_IS_HOST_BYTE_ORDER_LITTLE
#			define ESPRESS_CIPHER_UINT4096(n511, n510, n509, n508, n507, n506, n505, n504, n503, n502, n501, n500, n499, n498, n497, n496, n495, n494, n493, n492, n491, n490, n489, n488, n487, n486, n485, n484, n483, n482, n481, n480, \
				n479, n478, n477, n476, n475, n474, n473, n472, n471, n470, n469, n468, n467, n466, n465, n464, n463, n462, n461, n460, n459, n458, n457, n456, n455, n454, n453, n452, n451, n450, n449, n448, \
				n447, n446, n445, n444, n443, n442, n441, n440, n439, n438, n437, n436, n435, n434, n433, n432, n431, n430, n429, n428, n427, n426, n425, n424, n423, n422, n421, n420, n419, n418, n417, n416, \
				n415, n414, n413, n412, n411, n410, n409, n408, n407, n406, n405, n404, n403, n402, n401, n400, n399, n398, n397, n396, n395, n394, n393, n392, n391, n390, n389, n388, n387, n386, n385, n384, \
				n383, n382, n381, n380, n379, n378, n377, n376, n375, n374, n373, n372, n371, n370, n369, n368, n367, n366, n365, n364, n363, n362, n361, n360, n359, n358, n357, n356, n355, n354, n353, n352, \
				n351, n350, n349, n348, n347, n346, n345, n344, n343, n342, n341, n340, n339, n338, n337, n336, n335, n334, n333, n332, n331, n330, n329, n328, n327, n326, n325, n324, n323, n322, n321, n320, \
				n319, n318, n317, n316, n315, n314, n313, n312, n311, n310, n309, n308, n307, n306, n305, n304, n303, n302, n301, n300, n299, n298, n297, n296, n295, n294, n293, n292, n291, n290, n289, n288, \
				n287, n286, n285, n284, n283, n282, n281, n280, n279, n278, n277, n276, n275, n274, n273, n272, n271, n270, n269, n268, n267, n266, n265, n264, n263, n262, n261, n260, n259, n258, n257, n256, \
				n255, n254, n253, n252, n251, n250, n249, n248, n247, n246, n245, n244, n243, n242, n241, n240, n239, n238, n237, n236, n235, n234, n233, n232, n231, n230, n229, n228, n227, n226, n225, n224, \
				n223, n222, n221, n220, n219, n218, n217, n216, n215, n214, n213, n212, n211, n210, n209, n208, n207, n206, n205, n204, n203, n202, n201, n200, n199, n198, n197, n196, n195, n194, n193, n192, \
				n191, n190, n189, n188, n187, n186, n185, n184, n183, n182, n181, n180, n179, n178, n177, n176, n175, n174, n173, n172, n171, n170, n169, n168, n167, n166, n165, n164, n163, n162, n161, n160, \
				n159, n158, n157, n156, n155, n154, n153, n152, n151, n150, n149, n148, n147, n146, n145, n144, n143, n142, n141, n140, n139, n138, n137, n136, n135, n134, n133, n132, n131, n130, n129, n128, \
				n127, n126, n125, n124, n123, n122, n121, n120, n119, n118, n117, n116, n115, n114, n113, n112, n111, n110, n109, n108, n107, n106, n105, n104, n103, n102, n101, n100, n99, n98, n97, n96, \
				n95, n94, n93, n92, n91, n90, n89, n88, n87, n86, n85, n84, n83, n82, n81, n80, n79, n78, n77, n76, n75, n74, n73, n72, n71, n70, n69, n68, n67, n66, n65, n64, \
				n63, n62, n61, n60, n59, n58, n57, n56, n55, n54, n53, n52, n51, n50, n49, n48, n47, n46, n45, n44, n43, n42, n41, n40, n39, n38, n37, n36, n35, n34, n33, n32, \
				n31, n30, n29, n28, n27, n26, n25, n24, n23, n22, n21, n20, n19, n18, n17, n16, n15, n14, n13, n12, n11, n10, n9, n8, n7, n6, n5, n4, n3, n2, n1, n0) \
				{ \
					ESPRESS_CIPHER_UINT2048(n255, n254, n253, n252, n251, n250, n249, n248, n247, n246, n245, n244, n243, n242, n241, n240, n239, n238, n237, n236, n235, n234, n233, n232, n231, n230, n229, n228, n227, n226, n225, n224, \
						n223, n222, n221, n220, n219, n218, n217, n216, n215, n214, n213, n212, n211, n210, n209, n208, n207, n206, n205, n204, n203, n202, n201, n200, n199, n198, n197, n196, n195, n194, n193, n192, \
						n191, n190, n189, n188, n187, n186, n185, n184, n183, n182, n181, n180, n179, n178, n177, n176, n175, n174, n173, n172, n171, n170, n169, n168, n167, n166, n165, n164, n163, n162, n161, n160, \
						n159, n158, n157, n156, n155, n154, n153, n152, n151, n150, n149, n148, n147, n146, n145, n144, n143, n142, n141, n140, n139, n138, n137, n136, n135, n134, n133, n132, n131, n130, n129, n128, \
						n127, n126, n125, n124, n123, n122, n121, n120, n119, n118, n117, n116, n115, n114, n113, n112, n111, n110, n109, n108, n107, n106, n105, n104, n103, n102, n101, n100, n99, n98, n97, n96, \
						n95, n94, n93, n92, n91, n90, n89, n88, n87, n86, n85, n84, n83, n82, n81, n80, n79, n78, n77, n76, n75, n74, n73, n72, n71, n70, n69, n68, n67, n66, n65, n64, \
						n63, n62, n61, n60, n59, n58, n57, n56, n55, n54, n53, n52, n51, n50, n49, n48, n47, n46, n45, n44, n43, n42, n41, n40, n39, n38, n37, n36, n35, n34, n33, n32, \
						n31, n30, n29, n28, n27, n26, n25, n24, n23, n22, n21, n20, n19, n18, n17, n16, n15, n14, n13, n12, n11, n10, n9, n8, n7, n6, n5, n4, n3, n2, n1, n0), \
					ESPRESS_CIPHER_UINT2048(n511, n510, n509, n508, n507, n506, n505, n504, n503, n502, n501, n500, n499, n498, n497, n496, n495, n494, n493, n492, n491, n490, n489, n488, n487, n486, n485, n484, n483, n482, n481, n480, \
						n479, n478, n477, n476, n475, n474, n473, n472, n471, n470, n469, n468, n467, n466, n465, n464, n463, n462, n461, n460, n459, n458, n457, n456, n455, n454, n453, n452, n451, n450, n449, n448, \
						n447, n446, n445, n444, n443, n442, n441, n440, n439, n438, n437, n436, n435, n434, n433, n432, n431, n430, n429, n428, n427, n426, n425, n424, n423, n422, n421, n420, n419, n418, n417, n416, \
						n415, n414, n413, n412, n411, n410, n409, n408, n407, n406, n405, n404, n403, n402, n401, n400, n399, n398, n397, n396, n395, n394, n393, n392, n391, n390, n389, n388, n387, n386, n385, n384, \
						n383, n382, n381, n380, n379, n378, n377, n376, n375, n374, n373, n372, n371, n370, n369, n368, n367, n366, n365, n364, n363, n362, n361, n360, n359, n358, n357, n356, n355, n354, n353, n352, \
						n351, n350, n349, n348, n347, n346, n345, n344, n343, n342, n341, n340, n339, n338, n337, n336, n335, n334, n333, n332, n331, n330, n329, n328, n327, n326, n325, n324, n323, n322, n321, n320, \
						n319, n318, n317, n316, n315, n314, n313, n312, n311, n310, n309, n308, n307, n306, n305, n304, n303, n302, n301, n300, n299, n298, n297, n296, n295, n294, n293, n292, n291, n290, n289, n288, \
						n287, n286, n285, n284, n283, n282, n281, n280, n279, n278, n277, n276, n275, n274, n273, n272, n271, n270, n269, n268, n267, n266, n265, n264, n263, n262, n261, n260, n259, n258, n257, n256) \
				}
#		else
#			define ESPRESS_CIPHER_UINT4096(n511, n510, n509, n508, n507, n506, n505, n504, n503, n502, n501, n500, n499, n498, n497, n496, n495, n494, n493, n492, n491, n490, n489, n488, n487, n486, n485, n484, n483, n482, n481, n480, \
				n479, n478, n477, n476, n475, n474, n473, n472, n471, n470, n469, n468, n467, n466, n465, n464, n463, n462, n461, n460, n459, n458, n457, n456, n455, n454, n453, n452, n451, n450, n449, n448, \
				n447, n446, n445, n444, n443, n442, n441, n440, n439, n438, n437, n436, n435, n434, n433, n432, n431, n430, n429, n428, n427, n426, n425, n424, n423, n422, n421, n420, n419, n418, n417, n416, \
				n415, n414, n413, n412, n411, n410, n409, n408, n407, n406, n405, n404, n403, n402, n401, n400, n399, n398, n397, n396, n395, n394, n393, n392, n391, n390, n389, n388, n387, n386, n385, n384, \
				n383, n382, n381, n380, n379, n378, n377, n376, n375, n374, n373, n372, n371, n370, n369, n368, n367, n366, n365, n364, n363, n362, n361, n360, n359, n358, n357, n356, n355, n354, n353, n352, \
				n351, n350, n349, n348, n347, n346, n345, n344, n343, n342, n341, n340, n339, n338, n337, n336, n335, n334, n333, n332, n331, n330, n329, n328, n327, n326, n325, n324, n323, n322, n321, n320, \
				n319, n318, n317, n316, n315, n314, n313, n312, n311, n310, n309, n308, n307, n306, n305, n304, n303, n302, n301, n300, n299, n298, n297, n296, n295, n294, n293, n292, n291, n290, n289, n288, \
				n287, n286, n285, n284, n283, n282, n281, n280, n279, n278, n277, n276, n275, n274, n273, n272, n271, n270, n269, n268, n267, n266, n265, n264, n263, n262, n261, n260, n259, n258, n257, n256, \
				n255, n254, n253, n252, n251, n250, n249, n248, n247, n246, n245, n244, n243, n242, n241, n240, n239, n238, n237, n236, n235, n234, n233, n232, n231, n230, n229, n228, n227, n226, n225, n224, \
				n223, n222, n221, n220, n219, n218, n217, n216, n215, n214, n213, n212, n211, n210, n209, n208, n207, n206, n205, n204, n203, n202, n201, n200, n199, n198, n197, n196, n195, n194, n193, n192, \
				n191, n190, n189, n188, n187, n186, n185, n184, n183, n182, n181, n180, n179, n178, n177, n176, n175, n174, n173, n172, n171, n170, n169, n168, n167, n166, n165, n164, n163, n162, n161, n160, \
				n159, n158, n157, n156, n155, n154, n153, n152, n151, n150, n149, n148, n147, n146, n145, n144, n143, n142, n141, n140, n139, n138, n137, n136, n135, n134, n133, n132, n131, n130, n129, n128, \
				n127, n126, n125, n124, n123, n122, n121, n120, n119, n118, n117, n116, n115, n114, n113, n112, n111, n110, n109, n108, n107, n106, n105, n104, n103, n102, n101, n100, n99, n98, n97, n96, \
				n95, n94, n93, n92, n91, n90, n89, n88, n87, n86, n85, n84, n83, n82, n81, n80, n79, n78, n77, n76, n75, n74, n73, n72, n71, n70, n69, n68, n67, n66, n65, n64, \
				n63, n62, n61, n60, n59, n58, n57, n56, n55, n54, n53, n52, n51, n50, n49, n48, n47, n46, n45, n44, n43, n42, n41, n40, n39, n38, n37, n36, n35, n34, n33, n32, \
				n31, n30, n29, n28, n27, n26, n25, n24, n23, n22, n21, n20, n19, n18, n17, n16, n15, n14, n13, n12, n11, n10, n9, n8, n7, n6, n5, n4, n3, n2, n1, n0) \
				{ \
					ESPRESS_CIPHER_UINT2048(n511, n510, n509, n508, n507, n506, n505, n504, n503, n502, n501, n500, n499, n498, n497, n496, n495, n494, n493, n492, n491, n490, n489, n488, n487, n486, n485, n484, n483, n482, n481, n480, \
						n479, n478, n477, n476, n475, n474, n473, n472, n471, n470, n469, n468, n467, n466, n465, n464, n463, n462, n461, n460, n459, n458, n457, n456, n455, n454, n453, n452, n451, n450, n449, n448, \
						n447, n446, n445, n444, n443, n442, n441, n440, n439, n438, n437, n436, n435, n434, n433, n432, n431, n430, n429, n428, n427, n426, n425, n424, n423, n422, n421, n420, n419, n418, n417, n416, \
						n415, n414, n413, n412, n411, n410, n409, n408, n407, n406, n405, n404, n403, n402, n401, n400, n399, n398, n397, n396, n395, n394, n393, n392, n391, n390, n389, n388, n387, n386, n385, n384, \
						n383, n382, n381, n380, n379, n378, n377, n376, n375, n374, n373, n372, n371, n370, n369, n368, n367, n366, n365, n364, n363, n362, n361, n360, n359, n358, n357, n356, n355, n354, n353, n352, \
						n351, n350, n349, n348, n347, n346, n345, n344, n343, n342, n341, n340, n339, n338, n337, n336, n335, n334, n333, n332, n331, n330, n329, n328, n327, n326, n325, n324, n323, n322, n321, n320, \
						n319, n318, n317, n316, n315, n314, n313, n312, n311, n310, n309, n308, n307, n306, n305, n304, n303, n302, n301, n300, n299, n298, n297, n296, n295, n294, n293, n292, n291, n290, n289, n288, \
						n287, n286, n285, n284, n283, n282, n281, n280, n279, n278, n277, n276, n275, n274, n273, n272, n271, n270, n269, n268, n267, n266, n265, n264, n263, n262, n261, n260, n259, n258, n257, n256), \
					ESPRESS_CIPHER_UINT2048(n255, n254, n253, n252, n251, n250, n249, n248, n247, n246, n245, n244, n243, n242, n241, n240, n239, n238, n237, n236, n235, n234, n233, n232, n231, n230, n229, n228, n227, n226, n225, n224, \
						n223, n222, n221, n220, n219, n218, n217, n216, n215, n214, n213, n212, n211, n210, n209, n208, n207, n206, n205, n204, n203, n202, n201, n200, n199, n198, n197, n196, n195, n194, n193, n192, \
						n191, n190, n189, n188, n187, n186, n185, n184, n183, n182, n181, n180, n179, n178, n177, n176, n175, n174, n173, n172, n171, n170, n169, n168, n167, n166, n165, n164, n163, n162, n161, n160, \
						n159, n158, n157, n156, n155, n154, n153, n152, n151, n150, n149, n148, n147, n146, n145, n144, n143, n142, n141, n140, n139, n138, n137, n136, n135, n134, n133, n132, n131, n130, n129, n128, \
						n127, n126, n125, n124, n123, n122, n121, n120, n119, n118, n117, n116, n115, n114, n113, n112, n111, n110, n109, n108, n107, n106, n105, n104, n103, n102, n101, n100, n99, n98, n97, n96, \
						n95, n94, n93, n92, n91, n90, n89, n88, n87, n86, n85, n84, n83, n82, n81, n80, n79, n78, n77, n76, n75, n74, n73, n72, n71, n70, n69, n68, n67, n66, n65, n64, \
						n63, n62, n61, n60, n59, n58, n57, n56, n55, n54, n53, n52, n51, n50, n49, n48, n47, n46, n45, n44, n43, n42, n41, n40, n39, n38, n37, n36, n35, n34, n33, n32, \
						n31, n30, n29, n28, n27, n26, n25, n24, n23, n22, n21, n20, n19, n18, n17, n16, n15, n14, n13, n12, n11, n10, n9, n8, n7, n6, n5, n4, n3, n2, n1, n0) \
				}
#		endif
#	endif

	extern struct espress_cipher_uint4096 const espress_cipher_uint4096_0[];
	extern struct espress_cipher_uint4096 const espress_cipher_uint4096_1[];
	extern struct espress_cipher_uint4096 const espress_cipher_uint4096_max[];

#	define ESPRESS_CIPHER_UINT4096_0 ESPRESS_CIPHER_UINT4096(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, \
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, \
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, \
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, \
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, \
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, \
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, \
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
#	define ESPRESS_CIPHER_UINT4096_1 ESPRESS_CIPHER_UINT4096(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, \
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, \
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, \
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, \
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, \
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, \
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, \
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1)
#	define ESPRESS_CIPHER_UINT4096_MAX ESPRESS_CIPHER_UINT4096( \
		0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, \
		0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, \
		0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, \
		0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, \
		0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, \
		0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, \
		0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, \
		0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, \
		0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, \
		0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, \
		0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, \
		0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, \
		0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, \
		0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, \
		0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, \
		0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff \
		)
#	if ESPRESS_CIPHER_IS_HOST_BYTE_ORDER_LITTLE
#		define espress_cipher_uint4096_htol(this) ((void)(this))
#		define espress_cipher_uint4096_htob(this) espress_cipher_uint4096_bswap(this)
#	else
#		define espress_cipher_uint4096_htol(this) espress_cipher_uint4096_bswap(this)
#		define espress_cipher_uint4096_htob(this) ((void)(this))
#	endif
#	define espress_cipher_uint4096_ltoh(this) espress_cipher_uint4096_htol(this)
#	define espress_cipher_uint4096_btoh(this) espress_cipher_uint4096_htob(this)
#	if ESPRESS_CIPHER_IS_NETWORK_BYTE_ORDER_LITTLE
#		define espress_cipher_uint4096_hton(this) espress_cipher_uint4096_htol(this)
#	else
#		define espress_cipher_uint4096_hton(this) espress_cipher_uint4096_htob(this)
#	endif
#	define espress_cipher_uint4096_ntoh(this) espress_cipher_uint4096_hton(this)
#endif

#endif /* __ESPRESS_CIPHER_UINT_H__ */
