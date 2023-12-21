#ifndef __ESPRESS_STREAM_UINT_H__
#define __ESPRESS_STREAM_UINT_H__

#define ESPRESS_STREAM_UINT_MAX_WIDTH (64)

#pragma pack(push, 1)
struct espress_stream_uint8
{
	u8_t u;
};
#pragma pack(pop)

static inline void espress_stream_uint8_set(struct espress_stream_uint8 *this, b_t n0)
{
	this->u = n0;
}
static inline void espress_stream_uint8_get(struct espress_stream_uint8 const *this, b_t *n0)
{
	*n0 = this->u;
}
static inline void espress_stream_uint8_va_set(struct espress_stream_uint8 *this, va_list *np)
{
	b_t const n0 = va_arg(*np, int);
	espress_stream_uint8_set(this, n0);
}
static inline void espress_stream_uint8_va_get(struct espress_stream_uint8 const *this, va_list *np)
{
	b_t * const n0 = va_arg(*np, b_t *);
	espress_stream_uint8_get(this, n0);
}
static inline void espress_stream_uint8_set_u(struct espress_stream_uint8 *this, u_t n){this->u = n;}
static inline void espress_stream_uint8_get_u(struct espress_stream_uint8 const *this, u_t *n){*n = this->u;}
static inline void espress_stream_uint8_assign(struct espress_stream_uint8 *this, struct espress_stream_uint8 const *n){this->u = n->u;}
static inline i_t espress_stream_uint8_add(struct espress_stream_uint8 *this, struct espress_stream_uint8 const *n){this->u = this->u + n->u; return this->u < n->u;}
static inline i_t espress_stream_uint8_sub(struct espress_stream_uint8 *this, struct espress_stream_uint8 const *n){i_t const is_uf = this->u < n ->u; this->u = this->u - n->u; return is_uf;}
static inline void espress_stream_uint8_mul(struct espress_stream_uint8 *this, struct espress_stream_uint8 const *n){this->u *= n->u;}
static inline void espress_stream_uint8_div(struct espress_stream_uint8 *this, struct espress_stream_uint8 const *n){this->u /= n->u;}
static inline void espress_stream_uint8_and(struct espress_stream_uint8 *this, struct espress_stream_uint8 const *n){this->u &= n->u;}
static inline void espress_stream_uint8_or(struct espress_stream_uint8 *this, struct espress_stream_uint8 const *n){this->u |= n->u;}
static inline void espress_stream_uint8_xor(struct espress_stream_uint8 *this, struct espress_stream_uint8 const *n){this->u ^= n->u;}
static inline void espress_stream_uint8_not(struct espress_stream_uint8 *this){this->u = ~(this->u);}
static inline void espress_stream_uint8_shift(struct espress_stream_uint8 *this, i_t n)
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
static inline void espress_stream_uint8_rotate(struct espress_stream_uint8 *this, i_t n)
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
static inline i_t espress_stream_uint8_eq(struct espress_stream_uint8 const *this, struct espress_stream_uint8 const *n){return this->u == n->u;}
static inline i_t espress_stream_uint8_lt(struct espress_stream_uint8 const *this, struct espress_stream_uint8 const *n){return this->u < n->u;}
static inline i_t espress_stream_uint8_le(struct espress_stream_uint8 const *this, struct espress_stream_uint8 const *n){return this->u <= n->u;}
static inline void espress_stream_uint8_bswap(struct espress_stream_uint8 *this){}

#define ESPRESS_STREAM_UINT8(n0) {(u8_t)(n0)}

extern struct espress_stream_uint8 const espress_stream_uint8_0[];
extern struct espress_stream_uint8 const espress_stream_uint8_1[];
extern struct espress_stream_uint8 const espress_stream_uint8_max[];

#define ESPRESS_STREAM_UINT8_0 ESPRESS_STREAM_UINT8(0)
#define ESPRESS_STREAM_UINT8_1 ESPRESS_STREAM_UINT8(1)
#define ESPRESS_STREAM_UINT8_MAX ESPRESS_STREAM_UINT8(0xff)
#define espress_stream_uint8_htol(this) ((void)(this))
#define espress_stream_uint8_htob(this) espress_stream_uint8_bswap(this)
#define espress_stream_uint8_ltoh(this) espress_stream_uint8_htol(this)
#define espress_stream_uint8_btoh(this) espress_stream_uint8_htob(this)
#define espress_stream_uint8_hton(this) espress_stream_uint8_htol(this)
#define espress_stream_uint8_ntoh(this) espress_stream_uint8_hton(this)

#if 2 <= ESPRESS_STREAM_UINT_MAX_WIDTH
#	if 2 <= MAX_U_T_WIDTH
#		pragma pack(push, 1)
		struct espress_stream_uint16
		{
			u16_t u;
		};
#		pragma pack(pop)

		static inline void espress_stream_uint16_set(struct espress_stream_uint16 *this, b_t n1, b_t n0)
		{
			this->u = (u16_t)n0 + ((u16_t)n1 << 8);
		}
		static inline void espress_stream_uint16_get(struct espress_stream_uint16 const *this, b_t *n1, b_t *n0)
		{
			*n0 = this->u;
			*n1 = this->u >> 8;
		}
		static inline void espress_stream_uint16_va_set(struct espress_stream_uint16 *this, va_list *np)
		{
			b_t const n1 = va_arg(*np, int);
			b_t const n0 = va_arg(*np, int);
			espress_stream_uint16_set(this, n1, n0);
		}
		static inline void espress_stream_uint16_va_get(struct espress_stream_uint16 const *this, va_list *np)
		{
			b_t * const n1 = va_arg(*np, b_t *);
			b_t * const n0 = va_arg(*np, b_t *);
			espress_stream_uint16_get(this, n1, n0);
		}
		static inline void espress_stream_uint16_set_u(struct espress_stream_uint16 *this, u_t n){this->u = n;}
		static inline void espress_stream_uint16_get_u(struct espress_stream_uint16 const *this, u_t *n){*n = this->u;}
		static inline void espress_stream_uint16_assign(struct espress_stream_uint16 *this, struct espress_stream_uint16 const *n){this->u = n->u;}
		static inline i_t espress_stream_uint16_add(struct espress_stream_uint16 *this, struct espress_stream_uint16 const *n){this->u = this->u + n->u; return this->u < n->u;}
		static inline i_t espress_stream_uint16_sub(struct espress_stream_uint16 *this, struct espress_stream_uint16 const *n){i_t const is_uf = this->u < n ->u; this->u = this->u - n->u; return is_uf;}
		static inline void espress_stream_uint16_mul(struct espress_stream_uint16 *this, struct espress_stream_uint16 const *n){this->u *= n->u;}
		static inline void espress_stream_uint16_div(struct espress_stream_uint16 *this, struct espress_stream_uint16 const *n){this->u /= n->u;}
		static inline void espress_stream_uint16_and(struct espress_stream_uint16 *this, struct espress_stream_uint16 const *n){this->u &= n->u;}
		static inline void espress_stream_uint16_or(struct espress_stream_uint16 *this, struct espress_stream_uint16 const *n){this->u |= n->u;}
		static inline void espress_stream_uint16_xor(struct espress_stream_uint16 *this, struct espress_stream_uint16 const *n){this->u ^= n->u;}
		static inline void espress_stream_uint16_not(struct espress_stream_uint16 *this){this->u = ~(this->u);}
		static inline void espress_stream_uint16_shift(struct espress_stream_uint16 *this, i_t n)
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
		static inline void espress_stream_uint16_rotate(struct espress_stream_uint16 *this, i_t n)
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
		static inline i_t espress_stream_uint16_eq(struct espress_stream_uint16 const *this, struct espress_stream_uint16 const *n){return this->u == n->u;}
		static inline i_t espress_stream_uint16_lt(struct espress_stream_uint16 const *this, struct espress_stream_uint16 const *n){return this->u < n->u;}
		static inline i_t espress_stream_uint16_le(struct espress_stream_uint16 const *this, struct espress_stream_uint16 const *n){return this->u <= n->u;}
		static inline void espress_stream_uint16_bswap(struct espress_stream_uint16 *this){swap_endian_16(&(this->u));}

#		define ESPRESS_STREAM_UINT16(n1, n0) {(u16_t)(n0) + ((u16_t)(n1) << 8)}
#	else
#		pragma pack(push, 1)
		struct espress_stream_uint16
		{
#		if ESPRESS_STREAM_IS_HOST_BYTE_ORDER_LITTLE
			struct espress_stream_uint8 l;
			struct espress_stream_uint8 h;
#		else
			struct espress_stream_uint8 h;
			struct espress_stream_uint8 l;
#		endif
		};
#		pragma pack(pop)

		void espress_stream_uint16_set(struct espress_stream_uint16 *this, b_t n1, b_t n0);
		void espress_stream_uint16_get(struct espress_stream_uint16 const *this, b_t *n1, b_t *n0);
		void espress_stream_uint16_va_set(struct espress_stream_uint16 *this, va_list *np);
		void espress_stream_uint16_va_get(struct espress_stream_uint16 const *this, va_list *np);
		void espress_stream_uint16_set_u(struct espress_stream_uint16 *this, u_t n);
		void espress_stream_uint16_get_u(struct espress_stream_uint16 const *this, u_t *n);
		void espress_stream_uint16_assign(struct espress_stream_uint16 *this, struct espress_stream_uint16 const *n);
		i_t espress_stream_uint16_add(struct espress_stream_uint16 *this, struct espress_stream_uint16 const *n);
		i_t espress_stream_uint16_sub(struct espress_stream_uint16 *this, struct espress_stream_uint16 const *n);
		void espress_stream_uint16_mul(struct espress_stream_uint16 *this, struct espress_stream_uint16 const *n);
		void espress_stream_uint16_div(struct espress_stream_uint16 *this, struct espress_stream_uint16 const *n);
		void espress_stream_uint16_and(struct espress_stream_uint16 *this, struct espress_stream_uint16 const *n);
		void espress_stream_uint16_or(struct espress_stream_uint16 *this, struct espress_stream_uint16 const *n);
		void espress_stream_uint16_xor(struct espress_stream_uint16 *this, struct espress_stream_uint16 const *n);
		void espress_stream_uint16_not(struct espress_stream_uint16 *this);
		void espress_stream_uint16_shift(struct espress_stream_uint16 *this, i_t n);
		void espress_stream_uint16_rotate(struct espress_stream_uint16 *this, i_t n);
		i_t espress_stream_uint16_eq(struct espress_stream_uint16 const *this, struct espress_stream_uint16 const *n);
		i_t espress_stream_uint16_lt(struct espress_stream_uint16 const *this, struct espress_stream_uint16 const *n);
		i_t espress_stream_uint16_le(struct espress_stream_uint16 const *this, struct espress_stream_uint16 const *n);
		void espress_stream_uint16_bswap(struct espress_stream_uint16 *this);

#		if ESPRESS_STREAM_IS_HOST_BYTE_ORDER_LITTLE
#			define ESPRESS_STREAM_UINT16(n1, n0) {ESPRESS_STREAM_UINT8(n0), ESPRESS_STREAM_UINT8(n1)}
#		else
#			define ESPRESS_STREAM_UINT16(n1, n0) {ESPRESS_STREAM_UINT8(n1), ESPRESS_STREAM_UINT8(n0)}
#		endif
#	endif

	extern struct espress_stream_uint16 const espress_stream_uint16_0[];
	extern struct espress_stream_uint16 const espress_stream_uint16_1[];
	extern struct espress_stream_uint16 const espress_stream_uint16_max[];

#	define ESPRESS_STREAM_UINT16_0 ESPRESS_STREAM_UINT16(0, 0)
#	define ESPRESS_STREAM_UINT16_1 ESPRESS_STREAM_UINT16(0, 1)
#	define ESPRESS_STREAM_UINT16_MAX ESPRESS_STREAM_UINT16(0xff, 0xff)
#	if ESPRESS_STREAM_IS_HOST_BYTE_ORDER_LITTLE
#		define espress_stream_uint16_htol(this) ((void)(this))
#		define espress_stream_uint16_htob(this) espress_stream_uint16_bswap(this)
#	else
#		define espress_stream_uint16_htol(this) espress_stream_uint16_bswap(this)
#		define espress_stream_uint16_htob(this) ((void)(this))
#	endif
#	define espress_stream_uint16_ltoh(this) espress_stream_uint16_htol(this)
#	define espress_stream_uint16_btoh(this) espress_stream_uint16_htob(this)
#	if ESPRESS_STREAM_IS_NETWORK_BYTE_ORDER_LITTLE
#		define espress_stream_uint16_hton(this) espress_stream_uint16_htol(this)
#	else
#		define espress_stream_uint16_hton(this) espress_stream_uint16_htob(this)
#	endif
#	define espress_stream_uint16_ntoh(this) espress_stream_uint16_hton(this)
#endif

#if 4 <= ESPRESS_STREAM_UINT_MAX_WIDTH
#	if 4 <= MAX_U_T_WIDTH
#		pragma pack(push, 1)
		struct espress_stream_uint32
		{
			u32_t u;
		};
#		pragma pack(pop)

		static inline void espress_stream_uint32_set(struct espress_stream_uint32 *this, b_t n3, b_t n2, b_t n1, b_t n0)
		{
			this->u = (u32_t)n0 + ((u32_t)n1 << 8) + ((u32_t)n2 << 16) + ((u32_t)n3 << 24);
		}
		static inline void espress_stream_uint32_get(struct espress_stream_uint32 const *this, b_t *n3, b_t *n2, b_t *n1, b_t *n0)
		{
			*n0 = this->u;
			*n1 = this->u >> 8;
			*n2 = this->u >> 16;
			*n3 = this->u >> 24;
		}
		static inline void espress_stream_uint32_va_set(struct espress_stream_uint32 *this, va_list *np)
		{
			b_t const n3 = va_arg(*np, int);
			b_t const n2 = va_arg(*np, int);
			b_t const n1 = va_arg(*np, int);
			b_t const n0 = va_arg(*np, int);
			espress_stream_uint32_set(this, n3, n2, n1, n0);
		}
		static inline void espress_stream_uint32_va_get(struct espress_stream_uint32 const *this, va_list *np)
		{
			b_t * const n3 = va_arg(*np, b_t *);
			b_t * const n2 = va_arg(*np, b_t *);
			b_t * const n1 = va_arg(*np, b_t *);
			b_t * const n0 = va_arg(*np, b_t *);
			espress_stream_uint32_get(this, n3, n2, n1, n0);
		}
		static inline void espress_stream_uint32_set_u(struct espress_stream_uint32 *this, u_t n){this->u = n;}
		static inline void espress_stream_uint32_get_u(struct espress_stream_uint32 const *this, u_t *n){*n = this->u;}
		static inline void espress_stream_uint32_assign(struct espress_stream_uint32 *this, struct espress_stream_uint32 const *n){this->u = n->u;}
		static inline i_t espress_stream_uint32_add(struct espress_stream_uint32 *this, struct espress_stream_uint32 const *n){this->u = this->u + n->u; return this->u < n->u;}
		static inline i_t espress_stream_uint32_sub(struct espress_stream_uint32 *this, struct espress_stream_uint32 const *n){i_t const is_uf = this->u < n ->u; this->u = this->u - n->u; return is_uf;}
		static inline void espress_stream_uint32_mul(struct espress_stream_uint32 *this, struct espress_stream_uint32 const *n){this->u *= n->u;}
		static inline void espress_stream_uint32_div(struct espress_stream_uint32 *this, struct espress_stream_uint32 const *n){this->u /= n->u;}
		static inline void espress_stream_uint32_and(struct espress_stream_uint32 *this, struct espress_stream_uint32 const *n){this->u &= n->u;}
		static inline void espress_stream_uint32_or(struct espress_stream_uint32 *this, struct espress_stream_uint32 const *n){this->u |= n->u;}
		static inline void espress_stream_uint32_xor(struct espress_stream_uint32 *this, struct espress_stream_uint32 const *n){this->u ^= n->u;}
		static inline void espress_stream_uint32_not(struct espress_stream_uint32 *this){this->u = ~(this->u);}
		static inline void espress_stream_uint32_shift(struct espress_stream_uint32 *this, i_t n)
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
		static inline void espress_stream_uint32_rotate(struct espress_stream_uint32 *this, i_t n)
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
		static inline i_t espress_stream_uint32_eq(struct espress_stream_uint32 const *this, struct espress_stream_uint32 const *n){return this->u == n->u;}
		static inline i_t espress_stream_uint32_lt(struct espress_stream_uint32 const *this, struct espress_stream_uint32 const *n){return this->u < n->u;}
		static inline i_t espress_stream_uint32_le(struct espress_stream_uint32 const *this, struct espress_stream_uint32 const *n){return this->u <= n->u;}
		static inline void espress_stream_uint32_bswap(struct espress_stream_uint32 *this){swap_endian_32(&(this->u));}

#		define ESPRESS_STREAM_UINT32(n3, n2, n1, n0) {(u32_t)(n0) + ((u32_t)(n1) << 8) + ((u32_t)(n2) << 16) + ((u32_t)(n3) << 24)}
#	else
#		pragma pack(push, 1)
		struct espress_stream_uint32
		{
#		if ESPRESS_STREAM_IS_HOST_BYTE_ORDER_LITTLE
			struct espress_stream_uint16 l;
			struct espress_stream_uint16 h;
#		else
			struct espress_stream_uint16 h;
			struct espress_stream_uint16 l;
#		endif
		};
#		pragma pack(pop)

		void espress_stream_uint32_set(struct espress_stream_uint32 *this, b_t n3, b_t n2, b_t n1, b_t n0);
		void espress_stream_uint32_get(struct espress_stream_uint32 const *this, b_t *n3, b_t *n2, b_t *n1, b_t *n0);
		void espress_stream_uint32_va_set(struct espress_stream_uint32 *this, va_list *np);
		void espress_stream_uint32_va_get(struct espress_stream_uint32 const *this, va_list *np);
		void espress_stream_uint32_set_u(struct espress_stream_uint32 *this, u_t n);
		void espress_stream_uint32_get_u(struct espress_stream_uint32 const *this, u_t *n);
		void espress_stream_uint32_assign(struct espress_stream_uint32 *this, struct espress_stream_uint32 const *n);
		i_t espress_stream_uint32_add(struct espress_stream_uint32 *this, struct espress_stream_uint32 const *n);
		i_t espress_stream_uint32_sub(struct espress_stream_uint32 *this, struct espress_stream_uint32 const *n);
		void espress_stream_uint32_mul(struct espress_stream_uint32 *this, struct espress_stream_uint32 const *n);
		void espress_stream_uint32_div(struct espress_stream_uint32 *this, struct espress_stream_uint32 const *n);
		void espress_stream_uint32_and(struct espress_stream_uint32 *this, struct espress_stream_uint32 const *n);
		void espress_stream_uint32_or(struct espress_stream_uint32 *this, struct espress_stream_uint32 const *n);
		void espress_stream_uint32_xor(struct espress_stream_uint32 *this, struct espress_stream_uint32 const *n);
		void espress_stream_uint32_not(struct espress_stream_uint32 *this);
		void espress_stream_uint32_shift(struct espress_stream_uint32 *this, i_t n);
		void espress_stream_uint32_rotate(struct espress_stream_uint32 *this, i_t n);
		i_t espress_stream_uint32_eq(struct espress_stream_uint32 const *this, struct espress_stream_uint32 const *n);
		i_t espress_stream_uint32_lt(struct espress_stream_uint32 const *this, struct espress_stream_uint32 const *n);
		i_t espress_stream_uint32_le(struct espress_stream_uint32 const *this, struct espress_stream_uint32 const *n);
		void espress_stream_uint32_bswap(struct espress_stream_uint32 *this);

#		if ESPRESS_STREAM_IS_HOST_BYTE_ORDER_LITTLE
#			define ESPRESS_STREAM_UINT32(n3, n2, n1, n0) {ESPRESS_STREAM_UINT16(n1, n0), ESPRESS_STREAM_UINT16(n3, n2)}
#		else
#			define ESPRESS_STREAM_UINT32(n3, n2, n1, n0) {ESPRESS_STREAM_UINT16(n3, n2), ESPRESS_STREAM_UINT16(n1, n0)}
#		endif
#	endif

	extern struct espress_stream_uint32 const espress_stream_uint32_0[];
	extern struct espress_stream_uint32 const espress_stream_uint32_1[];
	extern struct espress_stream_uint32 const espress_stream_uint32_max[];

	#define ESPRESS_STREAM_UINT32_0 ESPRESS_STREAM_UINT32(0, 0, 0, 0)
	#define ESPRESS_STREAM_UINT32_1 ESPRESS_STREAM_UINT32(0, 0, 0, 1)
	#define ESPRESS_STREAM_UINT32_MAX ESPRESS_STREAM_UINT32(0xff, 0xff, 0xff, 0xff)
#	if ESPRESS_STREAM_IS_HOST_BYTE_ORDER_LITTLE
#		define espress_stream_uint32_htol(this) ((void)(this))
#		define espress_stream_uint32_htob(this) espress_stream_uint32_bswap(this)
#	else
#		define espress_stream_uint32_htol(this) espress_stream_uint32_bswap(this)
#		define espress_stream_uint32_htob(this) ((void)(this))
#	endif
#	define espress_stream_uint32_ltoh(this) espress_stream_uint32_htol(this)
#	define espress_stream_uint32_btoh(this) espress_stream_uint32_htob(this)
#	if ESPRESS_STREAM_IS_NETWORK_BYTE_ORDER_LITTLE
#		define espress_stream_uint32_hton(this) espress_stream_uint32_htol(this)
#	else
#		define espress_stream_uint32_hton(this) espress_stream_uint32_htob(this)
#	endif
#	define espress_stream_uint32_ntoh(this) espress_stream_uint32_hton(this)
#endif

#if 8 <= ESPRESS_STREAM_UINT_MAX_WIDTH
#	if 8 <= MAX_U_T_WIDTH
#		pragma pack(push, 1)
		struct espress_stream_uint64
		{
			u64_t u;
		};
#		pragma pack(pop)

		static inline void espress_stream_uint64_set(struct espress_stream_uint64 *this, b_t n7, b_t n6, b_t n5, b_t n4, b_t n3, b_t n2, b_t n1, b_t n0)
		{
			this->u =
				(u64_t)n0 + ((u64_t)n1 << 8) + ((u64_t)n2 << 16) + ((u64_t)n3 << 24) +
				((u64_t)n4 << 32) + ((u64_t)n5 << 40) + ((u64_t)n6 << 48) + ((u64_t)n7 << 56);
		}
		static inline void espress_stream_uint64_get(struct espress_stream_uint64 const *this, b_t *n7, b_t *n6, b_t *n5, b_t *n4, b_t *n3, b_t *n2, b_t *n1, b_t *n0)
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
		static inline void espress_stream_uint64_va_set(struct espress_stream_uint64 *this, va_list *np)
		{
			b_t const n7 = va_arg(*np, int);
			b_t const n6 = va_arg(*np, int);
			b_t const n5 = va_arg(*np, int);
			b_t const n4 = va_arg(*np, int);
			b_t const n3 = va_arg(*np, int);
			b_t const n2 = va_arg(*np, int);
			b_t const n1 = va_arg(*np, int);
			b_t const n0 = va_arg(*np, int);
			espress_stream_uint64_set(this, n7, n6, n5, n4, n3, n2, n1, n0);
		}
		static inline void espress_stream_uint64_va_get(struct espress_stream_uint64 const *this, va_list *np)
		{
			b_t * const n7 = va_arg(*np, b_t *);
			b_t * const n6 = va_arg(*np, b_t *);
			b_t * const n5 = va_arg(*np, b_t *);
			b_t * const n4 = va_arg(*np, b_t *);
			b_t * const n3 = va_arg(*np, b_t *);
			b_t * const n2 = va_arg(*np, b_t *);
			b_t * const n1 = va_arg(*np, b_t *);
			b_t * const n0 = va_arg(*np, b_t *);
			espress_stream_uint64_get(this, n7, n6, n5, n4, n3, n2, n1, n0);
		}
		static inline void espress_stream_uint64_set_u(struct espress_stream_uint64 *this, u_t n){this->u = n;}
		static inline void espress_stream_uint64_get_u(struct espress_stream_uint64 const *this, u_t *n){*n = this->u;}
		static inline void espress_stream_uint64_assign(struct espress_stream_uint64 *this, struct espress_stream_uint64 const *n){this->u = n->u;}
		static inline i_t espress_stream_uint64_add(struct espress_stream_uint64 *this, struct espress_stream_uint64 const *n){this->u = this->u + n->u; return this->u < n->u;}
		static inline i_t espress_stream_uint64_sub(struct espress_stream_uint64 *this, struct espress_stream_uint64 const *n){i_t const is_uf = this->u < n ->u; this->u = this->u - n->u; return is_uf;}
		static inline void espress_stream_uint64_mul(struct espress_stream_uint64 *this, struct espress_stream_uint64 const *n){this->u *= n->u;}
		static inline void espress_stream_uint64_div(struct espress_stream_uint64 *this, struct espress_stream_uint64 const *n){this->u /= n->u;}
		static inline void espress_stream_uint64_and(struct espress_stream_uint64 *this, struct espress_stream_uint64 const *n){this->u &= n->u;}
		static inline void espress_stream_uint64_or(struct espress_stream_uint64 *this, struct espress_stream_uint64 const *n){this->u |= n->u;}
		static inline void espress_stream_uint64_xor(struct espress_stream_uint64 *this, struct espress_stream_uint64 const *n){this->u ^= n->u;}
		static inline void espress_stream_uint64_not(struct espress_stream_uint64 *this){this->u = ~(this->u);}
		static inline void espress_stream_uint64_shift(struct espress_stream_uint64 *this, i_t n)
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
		static inline void espress_stream_uint64_rotate(struct espress_stream_uint64 *this, i_t n)
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
		static inline i_t espress_stream_uint64_eq(struct espress_stream_uint64 const *this, struct espress_stream_uint64 const *n){return this->u == n->u;}
		static inline i_t espress_stream_uint64_lt(struct espress_stream_uint64 const *this, struct espress_stream_uint64 const *n){return this->u < n->u;}
		static inline i_t espress_stream_uint64_le(struct espress_stream_uint64 const *this, struct espress_stream_uint64 const *n){return this->u <= n->u;}
		static inline void espress_stream_uint64_bswap(struct espress_stream_uint64 *this){swap_endian_64(&(this->u));}

#		define ESPRESS_STREAM_UINT64(n7, n6, n5, n4, n3, n2, n1, n0) {(u64_t)(n0) + ((u64_t)(n1) << 8) + ((u64_t)(n2) << 16) + ((u64_t)(n3) << 24) + ((u64_t)(n4) << 32) + ((u64_t)(n5) << 40) + ((u64_t)(n6) << 48) + ((u64_t)(n7) << 56)}
#	else
#		pragma pack(push, 1)
		struct espress_stream_uint64
		{
#		if ESPRESS_STREAM_IS_HOST_BYTE_ORDER_LITTLE
			struct espress_stream_uint32 l;
			struct espress_stream_uint32 h;
#		else
			struct espress_stream_uint32 h;
			struct espress_stream_uint32 l;
#		endif
		};
#		pragma pack(pop)

		void espress_stream_uint64_set(struct espress_stream_uint64 *this, b_t n7, b_t n6, b_t n5, b_t n4, b_t n3, b_t n2, b_t n1, b_t n0);
		void espress_stream_uint64_get(struct espress_stream_uint64 const *this, b_t *n7, b_t *n6, b_t *n5, b_t *n4, b_t *n3, b_t *n2, b_t *n1, b_t *n0);
		void espress_stream_uint64_va_set(struct espress_stream_uint64 *this, va_list *np);
		void espress_stream_uint64_va_get(struct espress_stream_uint64 const *this, va_list *np);
		void espress_stream_uint64_set_u(struct espress_stream_uint64 *this, u_t n);
		void espress_stream_uint64_get_u(struct espress_stream_uint64 const *this, u_t *n);
		void espress_stream_uint64_assign(struct espress_stream_uint64 *this, struct espress_stream_uint64 const *n);
		i_t espress_stream_uint64_add(struct espress_stream_uint64 *this, struct espress_stream_uint64 const *n);
		i_t espress_stream_uint64_sub(struct espress_stream_uint64 *this, struct espress_stream_uint64 const *n);
		void espress_stream_uint64_mul(struct espress_stream_uint64 *this, struct espress_stream_uint64 const *n);
		void espress_stream_uint64_div(struct espress_stream_uint64 *this, struct espress_stream_uint64 const *n);
		void espress_stream_uint64_and(struct espress_stream_uint64 *this, struct espress_stream_uint64 const *n);
		void espress_stream_uint64_or(struct espress_stream_uint64 *this, struct espress_stream_uint64 const *n);
		void espress_stream_uint64_xor(struct espress_stream_uint64 *this, struct espress_stream_uint64 const *n);
		void espress_stream_uint64_not(struct espress_stream_uint64 *this);
		void espress_stream_uint64_shift(struct espress_stream_uint64 *this, i_t n);
		void espress_stream_uint64_rotate(struct espress_stream_uint64 *this, i_t n);
		i_t espress_stream_uint64_eq(struct espress_stream_uint64 const *this, struct espress_stream_uint64 const *n);
		i_t espress_stream_uint64_lt(struct espress_stream_uint64 const *this, struct espress_stream_uint64 const *n);
		i_t espress_stream_uint64_le(struct espress_stream_uint64 const *this, struct espress_stream_uint64 const *n);
		void espress_stream_uint64_bswap(struct espress_stream_uint64 *this);

#		if ESPRESS_STREAM_IS_HOST_BYTE_ORDER_LITTLE
#			define ESPRESS_STREAM_UINT64(n7, n6, n5, n4, n3, n2, n1, n0) {ESPRESS_STREAM_UINT32(n3, n2, n1, n0), ESPRESS_STREAM_UINT32(n7, n6, n5, n4)}
#		else
#			define ESPRESS_STREAM_UINT64(n7, n6, n5, n4, n3, n2, n1, n0) {ESPRESS_STREAM_UINT32(n7, n6, n5, n4), ESPRESS_STREAM_UINT32(n3, n2, n1, n0)}
#		endif
#	endif

	extern struct espress_stream_uint64 const espress_stream_uint64_0[];
	extern struct espress_stream_uint64 const espress_stream_uint64_1[];
	extern struct espress_stream_uint64 const espress_stream_uint64_max[];

#	define ESPRESS_STREAM_UINT64_0 ESPRESS_STREAM_UINT64(0, 0, 0, 0, 0, 0, 0, 0)
#	define ESPRESS_STREAM_UINT64_1 ESPRESS_STREAM_UINT64(0, 0, 0, 0, 0, 0, 0, 1)
#	define ESPRESS_STREAM_UINT64_MAX ESPRESS_STREAM_UINT64(0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff)
#	if ESPRESS_STREAM_IS_HOST_BYTE_ORDER_LITTLE
#		define espress_stream_uint64_htol(this) ((void)(this))
#		define espress_stream_uint64_htob(this) espress_stream_uint64_bswap(this)
#	else
#		define espress_stream_uint64_htol(this) espress_stream_uint64_bswap(this)
#		define espress_stream_uint64_htob(this) ((void)(this))
#	endif
#	define espress_stream_uint64_ltoh(this) espress_stream_uint64_htol(this)
#	define espress_stream_uint64_btoh(this) espress_stream_uint64_htob(this)
#	if ESPRESS_STREAM_IS_NETWORK_BYTE_ORDER_LITTLE
#		define espress_stream_uint64_hton(this) espress_stream_uint64_htol(this)
#	else
#		define espress_stream_uint64_hton(this) espress_stream_uint64_htob(this)
#	endif
#	define espress_stream_uint64_ntoh(this) espress_stream_uint64_hton(this)
#endif

#if 16 <= ESPRESS_STREAM_UINT_MAX_WIDTH
#	if 16 <= MAX_U_T_WIDTH
#		pragma pack(push, 1)
		struct espress_stream_uint128
		{
			u128_t u;
		};
#		pragma pack(pop)

		static inline void espress_stream_uint128_set(struct espress_stream_uint128 *this, b_t n15, b_t n14, b_t n13, b_t n12, b_t n11, b_t n10, b_t n9, b_t n8, b_t n7, b_t n6, b_t n5, b_t n4, b_t n3, b_t n2, b_t n1, b_t n0)
		{
			this->u =
				(u128_t)n0 + ((u128_t)n1 << 8) + ((u128_t)n2 << 16) + ((u128_t)n3 << 24) +
				((u128_t)n4 << 32) + ((u128_t)n5 << 40) + ((u128_t)n6 << 48) + ((u128_t)n7 << 56) +
				((u128_t)n5 << 64) + ((u128_t)n5 << 72) + ((u128_t)n6 << 80) + ((u128_t)n7 << 88) +
				((u128_t)n5 << 96) + ((u128_t)n5 << 104) + ((u128_t)n6 << 112) + ((u128_t)n7 << 120);
		}
		static inline void espress_stream_uint128_get(struct espress_stream_uint128 const *this, b_t *n15, b_t *n14, b_t *n13, b_t *n12, b_t *n11, b_t *n10, b_t *n9, b_t *n8, b_t *n7, b_t *n6, b_t *n5, b_t *n4, b_t *n3, b_t *n2, b_t *n1, b_t *n0)
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
		static inline void espress_stream_uint128_va_set(struct espress_stream_uint128 *this, va_list *np)
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
			espress_stream_uint128_set(this, n15, n14, n13, n12, n11, n10, n9, n8, n7, n6, n5, n4, n3, n2, n1, n0);
		}
		static inline void espress_stream_uint128_va_get(struct espress_stream_uint128 const *this, va_list *np)
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
			espress_stream_uint128_get(this, n15, n14, n13, n12, n11, n10, n9, n8, n7, n6, n5, n4, n3, n2, n1, n0);
		}
		static inline void espress_stream_uint128_set_u(struct espress_stream_uint128 *this, u_t n){this->u = n;}
		static inline void espress_stream_uint128_get_u(struct espress_stream_uint128 const *this, u_t *n){*n = this->u;}
		static inline void espress_stream_uint128_assign(struct espress_stream_uint128 *this, struct espress_stream_uint128 const *n){this->u = n->u;}
		static inline i_t espress_stream_uint128_add(struct espress_stream_uint128 *this, struct espress_stream_uint128 const *n){this->u = this->u + n->u; return this->u < n->u;}
		static inline i_t espress_stream_uint128_sub(struct espress_stream_uint128 *this, struct espress_stream_uint128 const *n){i_t const is_uf = this->u < n ->u; this->u = this->u - n->u; return is_uf;}
		static inline void espress_stream_uint128_mul(struct espress_stream_uint128 *this, struct espress_stream_uint128 const *n){this->u *= n->u;}
		static inline void espress_stream_uint128_div(struct espress_stream_uint128 *this, struct espress_stream_uint128 const *n){this->u /= n->u;}
		static inline void espress_stream_uint128_and(struct espress_stream_uint128 *this, struct espress_stream_uint128 const *n){this->u &= n->u;}
		static inline void espress_stream_uint128_or(struct espress_stream_uint128 *this, struct espress_stream_uint128 const *n){this->u |= n->u;}
		static inline void espress_stream_uint128_xor(struct espress_stream_uint128 *this, struct espress_stream_uint128 const *n){this->u ^= n->u;}
		static inline void espress_stream_uint128_not(struct espress_stream_uint128 *this){this->u = ~(this->u);}
		static inline void espress_stream_uint128_shift(struct espress_stream_uint128 *this, i_t n)
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
		static inline void espress_stream_uint128_rotate(struct espress_stream_uint128 *this, i_t n)
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
		static inline i_t espress_stream_uint128_eq(struct espress_stream_uint128 const *this, struct espress_stream_uint128 const *n){return this->u == n->u;}
		static inline i_t espress_stream_uint128_lt(struct espress_stream_uint128 const *this, struct espress_stream_uint128 const *n){return this->u < n->u;}
		static inline i_t espress_stream_uint128_le(struct espress_stream_uint128 const *this, struct espress_stream_uint128 const *n){return this->u <= n->u;}
		static inline void espress_stream_uint128_bswap(struct espress_stream_uint128 *this){swap_endian_128(&(this->u));}

#		define ESPRESS_STREAM_UINT128(n15, n14, n13, n12, n11, n10, n9, n8, n7, n6, n5, n4, n3, n2, n1, n0) \
		{ \
			(u128_t)(n0) + ((u128_t)(n1) << 8) + ((u128_t)(n2) << 16) + ((u128_t)(n3) << 24) + ((u128_t)(n4) << 32) + ((u128_t)(n5) << 40) + ((u128_t)(n6) << 48) + ((u128_t)(n7) << 56) + \
				((u128_t)(n8) << 64) + ((u128_t)(n9) << 72) + ((u128_t)(n10) << 80) + ((u128_t)(n11) << 88) + ((u128_t)(n12) << 96) + ((u128_t)(n13) << 104) + ((u128_t)(n14) << 112) + ((u128_t)(n15) << 120) \
		}
#	else
#		pragma pack(push, 1)
		struct espress_stream_uint128
		{
#		if ESPRESS_STREAM_IS_HOST_BYTE_ORDER_LITTLE
			struct espress_stream_uint64 l;
			struct espress_stream_uint64 h;
#		else
			struct espress_stream_uint64 h;
			struct espress_stream_uint64 l;
#		endif
		};
#		pragma pack(pop)

		void espress_stream_uint128_set(struct espress_stream_uint128 *this,
			b_t n15, b_t n14, b_t n13, b_t n12, b_t n11, b_t n10, b_t n9, b_t n8,
			b_t n7, b_t n6, b_t n5, b_t n4, b_t n3, b_t n2, b_t n1, b_t n0);
		void espress_stream_uint128_get(struct espress_stream_uint128 const *this,
			b_t *n15, b_t *n14, b_t *n13, b_t *n12, b_t *n11, b_t *n10, b_t *n9, b_t *n8,
			b_t *n7, b_t *n6, b_t *n5, b_t *n4, b_t *n3, b_t *n2, b_t *n1, b_t *n0);
		void espress_stream_uint128_va_set(struct espress_stream_uint128 *this, va_list *np);
		void espress_stream_uint128_va_get(struct espress_stream_uint128 const *this, va_list *np);
		void espress_stream_uint128_set_u(struct espress_stream_uint128 *this, u_t n);
		void espress_stream_uint128_get_u(struct espress_stream_uint128 const *this, u_t *n);
		void espress_stream_uint128_assign(struct espress_stream_uint128 *this, struct espress_stream_uint128 const *n);
		i_t espress_stream_uint128_add(struct espress_stream_uint128 *this, struct espress_stream_uint128 const *n);
		i_t espress_stream_uint128_sub(struct espress_stream_uint128 *this, struct espress_stream_uint128 const *n);
		void espress_stream_uint128_mul(struct espress_stream_uint128 *this, struct espress_stream_uint128 const *n);
		void espress_stream_uint128_div(struct espress_stream_uint128 *this, struct espress_stream_uint128 const *n);
		void espress_stream_uint128_and(struct espress_stream_uint128 *this, struct espress_stream_uint128 const *n);
		void espress_stream_uint128_or(struct espress_stream_uint128 *this, struct espress_stream_uint128 const *n);
		void espress_stream_uint128_xor(struct espress_stream_uint128 *this, struct espress_stream_uint128 const *n);
		void espress_stream_uint128_not(struct espress_stream_uint128 *this);
		void espress_stream_uint128_shift(struct espress_stream_uint128 *this, i_t n);
		void espress_stream_uint128_rotate(struct espress_stream_uint128 *this, i_t n);
		i_t espress_stream_uint128_eq(struct espress_stream_uint128 const *this, struct espress_stream_uint128 const *n);
		i_t espress_stream_uint128_lt(struct espress_stream_uint128 const *this, struct espress_stream_uint128 const *n);
		i_t espress_stream_uint128_le(struct espress_stream_uint128 const *this, struct espress_stream_uint128 const *n);
		void espress_stream_uint128_bswap(struct espress_stream_uint128 *this);

#		if ESPRESS_STREAM_IS_HOST_BYTE_ORDER_LITTLE
#			define ESPRESS_STREAM_UINT128(n15, n14, n13, n12, n11, n10, n9, n8, n7, n6, n5, n4, n3, n2, n1, n0) \
				{ESPRESS_STREAM_UINT64(n7, n6, n5, n4, n3, n2, n1, n0), ESPRESS_STREAM_UINT64(n15, n14, n13, n12, n11, n10, n9, n8)}
#		else
#			define ESPRESS_STREAM_UINT128(n15, n14, n13, n12, n11, n10, n9, n8, n7, n6, n5, n4, n3, n2, n1, n0) \
				{ESPRESS_STREAM_UINT64(n15, n14, n13, n12, n11, n10, n9, n8), ESPRESS_STREAM_UINT64(n7, n6, n5, n4, n3, n2, n1, n0)}
#		endif
#	endif

	extern struct espress_stream_uint128 const espress_stream_uint128_0[];
	extern struct espress_stream_uint128 const espress_stream_uint128_1[];
	extern struct espress_stream_uint128 const espress_stream_uint128_max[];
#	define ESPRESS_STREAM_UINT128_0 ESPRESS_STREAM_UINT128(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
#	define ESPRESS_STREAM_UINT128_1 ESPRESS_STREAM_UINT128(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1)
#	define ESPRESS_STREAM_UINT128_MAX ESPRESS_STREAM_UINT128(0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff)
#	if ESPRESS_STREAM_IS_HOST_BYTE_ORDER_LITTLE
#		define espress_stream_uint128_htol(this) ((void)(this))
#		define espress_stream_uint128_htob(this) espress_stream_uint128_bswap(this)
#	else
#		define espress_stream_uint128_htol(this) espress_stream_uint128_bswap(this)
#		define espress_stream_uint128_htob(this) ((void)(this))
#	endif
#	define espress_stream_uint128_ltoh(this) espress_stream_uint128_htol(this)
#	define espress_stream_uint128_btoh(this) espress_stream_uint128_htob(this)
#	if ESPRESS_STREAM_IS_NETWORK_BYTE_ORDER_LITTLE
#		define espress_stream_uint128_hton(this) espress_stream_uint128_htol(this)
#	else
#		define espress_stream_uint128_hton(this) espress_stream_uint128_htob(this)
#	endif
#	define espress_stream_uint128_ntoh(this) espress_stream_uint128_hton(this)
#endif

#if 32 <= ESPRESS_STREAM_UINT_MAX_WIDTH
#	if 32 <= MAX_U_T_WIDTH
#		error
#	else
#		pragma pack(push, 1)
		struct espress_stream_uint256
		{
#		if ESPRESS_STREAM_IS_HOST_BYTE_ORDER_LITTLE
			struct espress_stream_uint128 l;
			struct espress_stream_uint128 h;
#		else
			struct espress_stream_uint128 h;
			struct espress_stream_uint128 l;
#		endif
		};
#		pragma pack(pop)

		void espress_stream_uint256_set(struct espress_stream_uint256 *this,
			b_t n31, b_t n30, b_t n29, b_t n28, b_t n27, b_t n26, b_t n25, b_t n24,
			b_t n23, b_t n22, b_t n21, b_t n20, b_t n19, b_t n18, b_t n17, b_t n16,
			b_t n15, b_t n14, b_t n13, b_t n12, b_t n11, b_t n10, b_t n9, b_t n8,
			b_t n7, b_t n6, b_t n5, b_t n4, b_t n3, b_t n2, b_t n1, b_t n0);
		void espress_stream_uint256_get(struct espress_stream_uint256 const *this,
			b_t *n31, b_t *n30, b_t *n29, b_t *n28, b_t *n27, b_t *n26, b_t *n25, b_t *n24,
			b_t *n23, b_t *n22, b_t *n21, b_t *n20, b_t *n19, b_t *n18, b_t *n17, b_t *n16,
			b_t *n15, b_t *n14, b_t *n13, b_t *n12, b_t *n11, b_t *n10, b_t *n9, b_t *n8,
			b_t *n7, b_t *n6, b_t *n5, b_t *n4, b_t *n3, b_t *n2, b_t *n1, b_t *n0);
		void espress_stream_uint256_va_set(struct espress_stream_uint256 *this, va_list *np);
		void espress_stream_uint256_va_get(struct espress_stream_uint256 const *this, va_list *np);
		void espress_stream_uint256_set_u(struct espress_stream_uint256 *this, u_t n);
		void espress_stream_uint256_get_u(struct espress_stream_uint256 const *this, u_t *n);
		void espress_stream_uint256_assign(struct espress_stream_uint256 *this, struct espress_stream_uint256 const *n);
		i_t espress_stream_uint256_add(struct espress_stream_uint256 *this, struct espress_stream_uint256 const *n);
		i_t espress_stream_uint256_sub(struct espress_stream_uint256 *this, struct espress_stream_uint256 const *n);
		void espress_stream_uint256_mul(struct espress_stream_uint256 *this, struct espress_stream_uint256 const *n);
		void espress_stream_uint256_div(struct espress_stream_uint256 *this, struct espress_stream_uint256 const *n);
		void espress_stream_uint256_and(struct espress_stream_uint256 *this, struct espress_stream_uint256 const *n);
		void espress_stream_uint256_or(struct espress_stream_uint256 *this, struct espress_stream_uint256 const *n);
		void espress_stream_uint256_xor(struct espress_stream_uint256 *this, struct espress_stream_uint256 const *n);
		void espress_stream_uint256_not(struct espress_stream_uint256 *this);
		void espress_stream_uint256_shift(struct espress_stream_uint256 *this, i_t n);
		void espress_stream_uint256_rotate(struct espress_stream_uint256 *this, i_t n);
		i_t espress_stream_uint256_eq(struct espress_stream_uint256 const *this, struct espress_stream_uint256 const *n);
		i_t espress_stream_uint256_lt(struct espress_stream_uint256 const *this, struct espress_stream_uint256 const *n);
		i_t espress_stream_uint256_le(struct espress_stream_uint256 const *this, struct espress_stream_uint256 const *n);
		void espress_stream_uint256_bswap(struct espress_stream_uint256 *this);

#		if ESPRESS_STREAM_IS_HOST_BYTE_ORDER_LITTLE
#			define ESPRESS_STREAM_UINT256(n31, n30, n29, n28, n27, n26, n25, n24, n23, n22, n21, n20, n19, n18, n17, n16, n15, n14, n13, n12, n11, n10, n9, n8, n7, n6, n5, n4, n3, n2, n1, n0) \
				{ESPRESS_STREAM_UINT128(n15, n14, n13, n12, n11, n10, n9, n8, n7, n6, n5, n4, n3, n2, n1, n0), ESPRESS_STREAM_UINT128(n31, n30, n29, n28, n27, n26, n25, n24, n23, n22, n21, n20, n19, n18, n17, n16)}
#		else
#			define ESPRESS_STREAM_UINT256(n31, n30, n29, n28, n27, n26, n25, n24, n23, n22, n21, n20, n19, n18, n17, n16, n15, n14, n13, n12, n11, n10, n9, n8, n7, n6, n5, n4, n3, n2, n1, n0) \
				{ESPRESS_STREAM_UINT128(n31, n30, n29, n28, n27, n26, n25, n24, n23, n22, n21, n20, n19, n18, n17, n16), ESPRESS_STREAM_UINT128(n15, n14, n13, n12, n11, n10, n9, n8, n7, n6, n5, n4, n3, n2, n1, n0)}
#		endif
#	endif

	extern struct espress_stream_uint256 const espress_stream_uint256_0[];
	extern struct espress_stream_uint256 const espress_stream_uint256_1[];
	extern struct espress_stream_uint256 const espress_stream_uint256_max[];

#	define ESPRESS_STREAM_UINT256_0 ESPRESS_STREAM_UINT256(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
#	define ESPRESS_STREAM_UINT256_1 ESPRESS_STREAM_UINT256(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1)
#	define ESPRESS_STREAM_UINT256_MAX ESPRESS_STREAM_UINT256(0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff)
#	if ESPRESS_STREAM_IS_HOST_BYTE_ORDER_LITTLE
#		define espress_stream_uint256_htol(this) ((void)(this))
#		define espress_stream_uint256_htob(this) espress_stream_uint256_bswap(this)
#	else
#		define espress_stream_uint256_htol(this) espress_stream_uint256_bswap(this)
#		define espress_stream_uint256_htob(this) ((void)(this))
#	endif
#	define espress_stream_uint256_ltoh(this) espress_stream_uint256_htol(this)
#	define espress_stream_uint256_btoh(this) espress_stream_uint256_htob(this)
#	if ESPRESS_STREAM_IS_NETWORK_BYTE_ORDER_LITTLE
#		define espress_stream_uint256_hton(this) espress_stream_uint256_htol(this)
#	else
#		define espress_stream_uint256_hton(this) espress_stream_uint256_htob(this)
#	endif
#	define espress_stream_uint256_ntoh(this) espress_stream_uint256_hton(this)
#endif

#if 64 <= ESPRESS_STREAM_UINT_MAX_WIDTH
#	if 64 <= MAX_U_T_WIDTH
#		error
#	else
#		pragma pack(push, 1)
		struct espress_stream_uint512
		{
#		if ESPRESS_STREAM_IS_HOST_BYTE_ORDER_LITTLE
			struct espress_stream_uint256 l;
			struct espress_stream_uint256 h;
#		else
			struct espress_stream_uint256 h;
			struct espress_stream_uint256 l;
#		endif
		};
#		pragma pack(pop)

		void espress_stream_uint512_set(struct espress_stream_uint512 *this,
			b_t n63, b_t n62, b_t n61, b_t n60, b_t n59, b_t n58, b_t n57, b_t n56,
			b_t n55, b_t n54, b_t n53, b_t n52, b_t n51, b_t n50, b_t n49, b_t n48,
			b_t n47, b_t n46, b_t n45, b_t n44, b_t n43, b_t n42, b_t n41, b_t n40,
			b_t n39, b_t n38, b_t n37, b_t n36, b_t n35, b_t n34, b_t n33, b_t n32,
			b_t n31, b_t n30, b_t n29, b_t n28, b_t n27, b_t n26, b_t n25, b_t n24,
			b_t n23, b_t n22, b_t n21, b_t n20, b_t n19, b_t n18, b_t n17, b_t n16,
			b_t n15, b_t n14, b_t n13, b_t n12, b_t n11, b_t n10, b_t n9, b_t n8,
			b_t n7, b_t n6, b_t n5, b_t n4, b_t n3, b_t n2, b_t n1, b_t n0);
		void espress_stream_uint512_get(struct espress_stream_uint512 const *this,
			b_t *n63, b_t *n62, b_t *n61, b_t *n60, b_t *n59, b_t *n58, b_t *n57, b_t *n56,
			b_t *n55, b_t *n54, b_t *n53, b_t *n52, b_t *n51, b_t *n50, b_t *n49, b_t *n48,
			b_t *n47, b_t *n46, b_t *n45, b_t *n44, b_t *n43, b_t *n42, b_t *n41, b_t *n40,
			b_t *n39, b_t *n38, b_t *n37, b_t *n36, b_t *n35, b_t *n34, b_t *n33, b_t *n32,
			b_t *n31, b_t *n30, b_t *n29, b_t *n28, b_t *n27, b_t *n26, b_t *n25, b_t *n24,
			b_t *n23, b_t *n22, b_t *n21, b_t *n20, b_t *n19, b_t *n18, b_t *n17, b_t *n16,
			b_t *n15, b_t *n14, b_t *n13, b_t *n12, b_t *n11, b_t *n10, b_t *n9, b_t *n8,
			b_t *n7, b_t *n6, b_t *n5, b_t *n4, b_t *n3, b_t *n2, b_t *n1, b_t *n0);
		void espress_stream_uint512_va_set(struct espress_stream_uint512 *this, va_list *np);
		void espress_stream_uint512_va_get(struct espress_stream_uint512 const *this, va_list *np);
		void espress_stream_uint512_set_u(struct espress_stream_uint512 *this, u_t n);
		void espress_stream_uint512_get_u(struct espress_stream_uint512 const *this, u_t *n);
		void espress_stream_uint512_assign(struct espress_stream_uint512 *this, struct espress_stream_uint512 const *n);
		i_t espress_stream_uint512_add(struct espress_stream_uint512 *this, struct espress_stream_uint512 const *n);
		i_t espress_stream_uint512_sub(struct espress_stream_uint512 *this, struct espress_stream_uint512 const *n);
		void espress_stream_uint512_mul(struct espress_stream_uint512 *this, struct espress_stream_uint512 const *n);
		void espress_stream_uint512_div(struct espress_stream_uint512 *this, struct espress_stream_uint512 const *n);
		void espress_stream_uint512_and(struct espress_stream_uint512 *this, struct espress_stream_uint512 const *n);
		void espress_stream_uint512_or(struct espress_stream_uint512 *this, struct espress_stream_uint512 const *n);
		void espress_stream_uint512_xor(struct espress_stream_uint512 *this, struct espress_stream_uint512 const *n);
		void espress_stream_uint512_not(struct espress_stream_uint512 *this);
		void espress_stream_uint512_shift(struct espress_stream_uint512 *this, i_t n);
		void espress_stream_uint512_rotate(struct espress_stream_uint512 *this, i_t n);
		i_t espress_stream_uint512_eq(struct espress_stream_uint512 const *this, struct espress_stream_uint512 const *n);
		i_t espress_stream_uint512_lt(struct espress_stream_uint512 const *this, struct espress_stream_uint512 const *n);
		i_t espress_stream_uint512_le(struct espress_stream_uint512 const *this, struct espress_stream_uint512 const *n);
		void espress_stream_uint512_bswap(struct espress_stream_uint512 *this);

#		if ESPRESS_STREAM_IS_HOST_BYTE_ORDER_LITTLE
#			define ESPRESS_STREAM_UINT512(n63, n62, n61, n60, n59, n58, n57, n56, n55, n54, n53, n52, n51, n50, n49, n48, n47, n46, n45, n44, n43, n42, n41, n40, n39, n38, n37, n36, n35, n34, n33, n32, \
				n31, n30, n29, n28, n27, n26, n25, n24, n23, n22, n21, n20, n19, n18, n17, n16, n15, n14, n13, n12, n11, n10, n9, n8, n7, n6, n5, n4, n3, n2, n1, n0) \
				{ \
					ESPRESS_STREAM_UINT256(n31, n30, n29, n28, n27, n26, n25, n24, n23, n22, n21, n20, n19, n18, n17, n16, n15, n14, n13, n12, n11, n10, n9, n8, n7, n6, n5, n4, n3, n2, n1, n0), \
					ESPRESS_STREAM_UINT256(n63, n62, n61, n60, n59, n58, n57, n56, n55, n54, n53, n52, n51, n50, n49, n48, n47, n46, n45, n44, n43, n42, n41, n40, n39, n38, n37, n36, n35, n34, n33, n32) \
				}
#		else
#			define ESPRESS_STREAM_UINT512(n63, n62, n61, n60, n59, n58, n57, n56, n55, n54, n53, n52, n51, n50, n49, n48, n47, n46, n45, n44, n43, n42, n41, n40, n39, n38, n37, n36, n35, n34, n33, n32, \
				n31, n30, n29, n28, n27, n26, n25, n24, n23, n22, n21, n20, n19, n18, n17, n16, n15, n14, n13, n12, n11, n10, n9, n8, n7, n6, n5, n4, n3, n2, n1, n0) \
				{ \
					ESPRESS_STREAM_UINT256(n63, n62, n61, n60, n59, n58, n57, n56, n55, n54, n53, n52, n51, n50, n49, n48, n47, n46, n45, n44, n43, n42, n41, n40, n39, n38, n37, n36, n35, n34, n33, n32), \
					ESPRESS_STREAM_UINT256(n31, n30, n29, n28, n27, n26, n25, n24, n23, n22, n21, n20, n19, n18, n17, n16, n15, n14, n13, n12, n11, n10, n9, n8, n7, n6, n5, n4, n3, n2, n1, n0) \
				}
#		endif
#	endif

	extern struct espress_stream_uint512 const espress_stream_uint512_0[];
	extern struct espress_stream_uint512 const espress_stream_uint512_1[];
	extern struct espress_stream_uint512 const espress_stream_uint512_max[];

#	define ESPRESS_STREAM_UINT512_0 ESPRESS_STREAM_UINT512(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
#	define ESPRESS_STREAM_UINT512_1 ESPRESS_STREAM_UINT512(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1)
#	define ESPRESS_STREAM_UINT512_MAX ESPRESS_STREAM_UINT512( \
		0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, \
		0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff \
		)
#	if ESPRESS_STREAM_IS_HOST_BYTE_ORDER_LITTLE
#		define espress_stream_uint512_htol(this) ((void)(this))
#		define espress_stream_uint512_htob(this) espress_stream_uint512_bswap(this)
#	else
#		define espress_stream_uint512_htol(this) espress_stream_uint512_bswap(this)
#		define espress_stream_uint512_htob(this) ((void)(this))
#	endif
#	define espress_stream_uint512_ltoh(this) espress_stream_uint512_htol(this)
#	define espress_stream_uint512_btoh(this) espress_stream_uint512_htob(this)
#	if ESPRESS_STREAM_IS_NETWORK_BYTE_ORDER_LITTLE
#		define espress_stream_uint512_hton(this) espress_stream_uint512_htol(this)
#	else
#		define espress_stream_uint512_hton(this) espress_stream_uint512_htob(this)
#	endif
#	define espress_stream_uint512_ntoh(this) espress_stream_uint512_hton(this)
#endif

#endif /* __ESPRESS_STREAM_UINT_H__ */
