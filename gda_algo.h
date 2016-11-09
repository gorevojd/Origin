#ifndef GDA_ALGO_H

#ifndef GDA_ALGO_DEF
#ifdef GDA_AGLO_STATIC
#define GDA_ALGO_DEF static
#else
#define GDA_ALGO_DEF extern
#endif
#endif

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable:4201)
#endif

#ifdef _MSC_VER
#pragma warning(pop)
#endif

#ifdef _MSC_VER
typedef char s8;
typedef short s16;
typedef int s32;
typedef long long s64;

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long long u64;
#else
#include <stdint.h>
typedef int8_t s8;
typedef int16_t s16;
typedef int32_t s32
typedef int64_t s64;

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;
#endif

#ifndef gda_min
#define gda_min(a, b) ((a) < (b) ? (a) : (b))
#endif

#ifndef gda_max
#define gda_max(a, b) ((a) > (b) ? (a) : (b))
#endif

#ifndef gda_clamp
#define gda_clamp(x, lower, upper) (gda_min(gda_max(x, (lower)), (upper))) 
#endif

#ifndef gda_clamp01
#define gda_clamp01(x) dga_clamp(x, 0, 1)
#endif

#ifndef gda_square
#define gda_square(x) ((x) * (x))
#endif

#ifndef gda_cube
#define gda_cube(x) ((x) * (x) * (x))
#endif

#ifndef gda_abs
#define gda_abs(x) ((x) > 0 ? (x) : -(x))
#endif

#ifndef gda_sign
#define gda_sign(x) ((x) >= 0 ? 1 : -1)
#endif

/*Numerical algorighms*/
/*Great Common Divisor*/
GDA_ALGO_DEF s32 gda_gcd_s32(s32 a, s32 b);
GDA_ALGO_DEF u32 gda_gcd_u32(u32 a, u32 b);
GDA_ALGO_DEF s64 gda_gcd_s64(s64 a, s64 b);
GDA_ALGO_DEF u64 gda_gcd_u64(u64 a, u64 b);
#define GDA_ALGO_H
#endif


#define GDA_ALGO_IMPLEMENTATION
#ifdef GDA_ALGO_IMPLEMENTATION

#define GDA_GCD_DEF(t, a, b) t ta = a; t tb = b; \
	while(tb != 0){		\
		t remainder = ta % tb;	\
		ta = tb;	\
		tb = remainder;	\
	} \
	return(ta);

s32 gda_gcd_s32(s32 a, s32 b){GDA_GCD_DEF(s32, a, b)}
u32 gda_gcd_u32(u32 a, u32 b){GDA_GCD_DEF(u32, a, b)}
s64 gda_gcd_s64(s64 a, s64 b){GDA_GCD_DEF(s64, a, b)}
u64 gda_gcd_u64(u64 a, u64 b){GDA_GCD_DEF(u64, a, b)}

#undef GDA_GCD_DEF

#endif