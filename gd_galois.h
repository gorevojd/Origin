/*
ABOUT:
	GALOIS FINITE FIELDS MATH

LICENSE:
	This software is dual-licensed to the public domain and under the following
	license: you are granted a perpetual, irrevocable license to copy, modify,
	publish, and distribute this file as you see fit.

CREDITS:
	Written by Gorevoy Dmitry

DONE:

HO TO USE:

EXAMPLES:

NOTES:
	"gf" in function definitions stand for Galois Fields
		
*/

#ifndef GD_GALOIS_H
#define GD_GALOIS_H

#ifndef GDGF_DEF
#ifdef GD_GALOIS_STATIC
#define GDGF_DEF static
#else
#define GDGF_DEF extern
#endif
#endif

#include <stdint.h>

#define GD_PRIME_POLY_8 0x11b
#define GD_PRIME_POLY_32 0x1c0000401

#define GD_MAX_GALOIS_FIELD_8 0x100
#define GD_MAX_GALOIS_FIELD_32 0x100000000

#ifdef __cplusplus
extern "C" {
#endif

GDGF_DEF uint64_t gd_gf_add(uint64_t a, uint64_t b);

GDGF_DEF uint64_t gd_gf_sub(uint64_t a, uint64_t b);

GDGF_DEF uint64_t gd_gf8_mod(uint64_t x);
GDGF_DEF uint64_t gd_gf32_mod(uint64_t x);


GDGF_DEF int gd_gf8_deg(uint64_t x);
GDGF_DEF int gd_gf32_deg(uint64_t x);

GDGF_DEF uint64_t gd_gf8_mul(uint64_t a, uint64_t b);
GDGF_DEF uint64_t gd_gf32_mul(uint64_t a, uint64_t b);

GDGF_DEF uint64_t gd_gf8_pow(uint64_t a, uint64_t b);
GDGF_DEF uint64_t gd_gf32_pow(uint64_t a, uint64_t b);

#ifdef __cplusplus
}
#endif

#endif


#if defined(GD_GALOIS_IMPLEMENTATION) && !defined(GD_GALOIS_IMPLEMENTATION_DONE)
#define GD_GALOIS_IMPLEMENTATION_DONE

uint64_t gd_gf_add(uint64_t a, uint64_t b){
	uint64_t res = a ^ b;
}

uint64_t gd_gf_sub(uint64_t a, uint64_t b){
	uint64_t res = a ^ b;
}

#define GD_GF_MOD(x, num_bits)							\
	uint64_t max = GD_MAX_GALOIS_FIELD_##num_bits;		\
	uint64_t prime_poly = GD_PRIME_POLY_##num_bits;		\
														\
	while(x & max){										\
		x = gd_gf_sub((x), prime_poly);					\
	}													\
														\
	return(x);											


uint64_t gd_gf8_mod(uint64_t x){ GD_GF_MOD(x, 8); }
uint64_t gd_gf32_mod(uint64_t x){ GD_GF_MOD(x, 32); }
#undef GD_GF_MOD

#define GD_GF_DEG(x, num_bits)							\
	int res = 0;										\
														\
	for(int i = 0; i <= num_bits; i++){					\
		if((x) & 1){									\
			res = i;									\
		}												\
														\
		(x) = (x) >> 1;									\
	}													\
														\
	return(res);

int gd_gf8_deg(uint64_t x){ GD_GF_DEG(x, 8);}
int gd_gf32_deg(uint64_t x){ GD_GF_DEG(x, 32);}
#undef GD_GF_DEG

#define GD_GF_MUL(a, b, num_bits)						\
	uint64_t res = 0;									\
														\
	(a) = gd_gf##num_bits##_mod(a);						\
	(b) = gd_gf##num_bits##_mod(b);						\
														\
	for(int i = 0; i < num_bits; i++){					\
		if((b) & 1){									\
			res = gd_gf_add(res, (a));					\
		}												\
														\
		(a) = (a) << 1;									\
		(b) = (b) >> 1;									\
														\
		a = gd_gf##num_bits##_mod(a);					\
		b = gd_gf##num_bits##_mod(b);					\
		res = gd_gf##num_bits##_mod(res);				\
	}													\
														\
	return(res);

uint64_t gd_gf8_mul(uint64_t a, uint64_t b){ GD_GF_MUL(a, b, 8);}
uint64_t gd_gf32_mul(uint64_t a, uint64_t b){ GD_GF_MUL(a, b, 32);}
#undef GD_GF_MUL


#define GD_GF_POW(x, n, num_bits)						\
	uint64_t y = 1;										\
	uint64_t res = 0;									\
														\
	if(n == 0){											\
		return(1);										\
	}													\
														\
	x = gd_gf##num_bits##_mod(x);						\
														\
	while((n) > 1){										\
		if(n % 2 == 0){									\
			x = gd_gf##num_bits##_mul(x, x);			\
			n = (n) / 2;								\
		}												\
		else{											\
			y = gd_gf##num_bits##_mul(x, y);			\
			x = gd_gf##num_bits##_mul(x, x);			\
			n = (n - 1) / 2;							\
		}												\
														\
		x = gd_gf##num_bits##_mod(x);					\
		y = gd_gf##num_bits##_mod(y);					\
	}													\
														\
	res = gd_gf##num_bits##_mul(x, y);					\
														\
	return(res);

uint64_t gd_gf8_pow(uint64_t a, uint64_t b){ GD_GF_POW(a, b, 8);}
uint64_t gd_gf32_pow(uint64_t a, uint64_t b){ GD_GF_POW(a, b, 32);}
#undef GD_GF_POW

#endif