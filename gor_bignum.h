#ifndef GOR_BIGNUM_H_INCLUDED
#define GOR_BIGNUM_H_INCLUDED

/*
	ABOUT:
		Big number library - arithmetic on multiple-precision unsigned integers.

		This library is an implementation of arithmetic on arbitrarily large integers.

		The difference between this and other implementations, is that the data structure
		has optimal memory utilization (i.e. a 1024 bit integer takes up 128 bytes RAM),
		and all memory is allocated statically: no dynamic allocation for better or worse.

		Primary goals are correctness, clarity of code and clean, portable implementation.
		Secondary goal is a memory footprint small enough to make it suitable for use in
		embedded applications.

		The current state is correct functionality and adequate performance.
		There may well be room for performance-optimizations and improvements.

	USAGE:
		

	LICENCE:
		This is free and unencumbered software released into the public domain.

		Anyone is free to copy, modify, publish, use, compile, sell, or
		distribute this software, either in source code form or as a compiled
		binary, for any purpose, commercial or non-commercial, and by any
		means.

		In jurisdictions that recognize copyright laws, the author or authors
		of this software dedicate any and all copyright interest in the
		software to the public domain. We make this dedication for the benefit
		of the public at large and to the detriment of our heirs and
		successors. We intend this dedication to be an overt act of
		relinquishment in perpetuity of all present and future rights to this
		software under copyright law.

		THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
		EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
		MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
		IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
		OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
		ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
		OTHER DEALINGS IN THE SOFTWARE.

		For more information, please refer to <https://unlicense.org>

	AUTHOR:
		Gorevoy Dmitry - github.com/gorevojd

	OTHER:
		Basic code was taken from https://github.com/kokke/tiny-bignum-c
		and then modified.
*/

#include <stdint.h>

#define GORBN_SZWORD 4
#define GORBN_SZARR (128 / GORBN_SZWORD)

#ifndef GORBN_SZWORD
#error GORBN_SZWORD must be defined
#elif (GORBN_SZWORD == 1)
#define gorbn_t uint8_t
#define gorbn_utmp_t uint32_t
#define gorbn_stmp_t int32_t
#define GORBN_MAX_VAL 0xFF

#elif (GORBN_SZWORD == 2)
#define gorbn_t uint16_t
#define gorbn_utmp_t uint32_t
#define gorbn_stmp_t int32_t
#define GORBN_MAX_VAL 0xFFFF

#elif (GORBN_SZWORD == 4)
#define gorbn_t uint32_t
#define gorbn_utmp_t uint64_t
#define gorbn_stmp_t int64_t
#define GORBN_MAX_VAL 0xFFFFFFFF

#elif
#error GORBN_SZWORD must be defined to 1, 2 or 4
#endif

typedef struct gorbn_bignum{
	int sign;

	gorbn_t number[GORBN_SZARR];
} gorbn_bignum;

/* Custom macro for getting absolute value of the signed integer*/
#define GORBN_ABS(val) (((val) >= 0) ? (val) : (-(val)))

/*Custom macro for getting the biggest number from two numbers*/
#define GORBN_MAX(a, b) (((a) > (b)) ? (a) : (b))

/* Custom assert macro */
#ifdef GOR_BIGNUM_ENABLE_ASSERTS
#define GORBN_ASSERT(cond) if(!(cond)){*(int*)0 = 0;}
#else
#define GORBN_ASSERT(...)
#endif

/*Tokens returned by gorbn_cmp() for value comparison*/
#define GORBN_CMP_LARGER 1
#define GORBN_CMP_SMALLER -1
#define GORBN_CMP_EQUAL 0

#ifdef GOR_BIGNUM_STATIC
#define GORBN_DEF static
#else
#define GORBN_DEF extern
#endif

#ifdef __cplusplus
extern "C" {
#endif

/*Initialization functions*/
GORBN_DEF void gorbn_init(gorbn_bignum* n);
GORBN_DEF void gorbn_copy(gorbn_bignum* dst, gorbn_bignum* src);
GORBN_DEF void gorbn_from_uint(gorbn_bignum* n, gorbn_utmp_t i);
GORBN_DEF void gorbn_from_int(gorbn_bignum* n, gorbn_stmp_t i);
GORBN_DEF void gorbn_from_data(gorbn_bignum* n, void* data, uint32_t data_size);
GORBN_DEF void gorbn_to_data(void* data, uint32_t data_size, gorbn_bignum* n);

/* Arithmetic operations: */
GORBN_DEF void gorbn_add(gorbn_bignum* r, gorbn_bignum* a, gorbn_bignum* b); /* r = a + b */
GORBN_DEF void gorbn_sub(gorbn_bignum* r, gorbn_bignum* a, gorbn_bignum* b); /* r = a - b */
GORBN_DEF void gorbn_mul(gorbn_bignum* r, gorbn_bignum* a, gorbn_bignum* b); /* r = a * b */
GORBN_DEF void gorbn_div(gorbn_bignum* r, gorbn_bignum* a, gorbn_bignum* b); /* r = a / b */
GORBN_DEF void gorbn_mod(gorbn_bignum* r, gorbn_bignum* a, gorbn_bignum* b); /* r = a % b */
GORBN_DEF void gorbn_pow(gorbn_bignum* r, gorbn_bignum* a, gorbn_bignum* b);
GORBN_DEF void gorbn_mul_pow2(gorbn_bignum* r, gorbn_bignum* a, int k); /* r = a * (2 ^ k) */

/* Bitwise operations: */
GORBN_DEF void gorbn_and(gorbn_bignum* r, gorbn_bignum* a, gorbn_bignum* b); /* r = a & b */
GORBN_DEF void gorbn_or(gorbn_bignum* r, gorbn_bignum* a, gorbn_bignum* b); /* r = a | b */
GORBN_DEF void gorbn_xor(gorbn_bignum* r, gorbn_bignum* a, gorbn_bignum* b); /* r = a ^ b */
GORBN_DEF void gorbn_lshift(gorbn_bignum* r, gorbn_bignum* a, int nbits); /* r = a << nbits */
GORBN_DEF void gorbn_rshift(gorbn_bignum* r, gorbn_bignum* b, int nbits); /* r = a >> nbits */

/* Special operators and comparison */
GORBN_DEF int gorbn_cmp(gorbn_bignum* a, gorbn_bignum* b);
GORBN_DEF int gorbn_cmp_mod(gorbn_bignum* a, gorbn_bignum* b);
GORBN_DEF int gorbn_cmp_zero(gorbn_bignum* a);
GORBN_DEF int gorbn_is_zero(gorbn_bignum* a, gorbn_bignum* b);


#ifdef __cplusplus
}
#endif

#endif

#if defined(GOR_BIGNUM_IMPLEMENTATION) && !defined(GOR_BIGNUM_IMPLEMENTATION_DONE)
#define GOR_BIGNUM_IMPLEMENTATION_DONE

void gorbn_init(gorbn_bignum* n){
	GORBN_ASSERT(n);

	int i = 0;
	for(i = 0; i < GORBN_SZARR; i++){
		n->number[i] = 0;
	}
	n->sign = 1;
}

void gorbn_copy(gorbn_bignum* dst, gorbn_bignum* src){
	GORBN_ASSERT(dst);
	GORBN_ASSERT(src);

	int i;
	for(i = 0; i < GORBN_SZARR; i++){
		dst->number[i] = src->number[i];
	}
	dst->sign = src->sign;
}

void gorbn_from_uint(gorbn_bignum* n, gorbn_utmp_t i){
	GORBN_ASSERT(n);

	gorbn_init(n);

#if (GORBN_SZWORD == 1)
	n->number[0] = (i & 0x000000ff);
	n->number[1] = (i & 0x0000ff00) >> 8;
	n->number[2] = (i & 0x00ff0000) >> 16;
	n->number[3] = (i & 0xff000000) >> 24;
#elif (GORBN_SZWORD == 2)
	n->number[0] = i;
	n->number[1] = i >> 16;
#elif (GORBN_SZWORD == 4)
	n->number[0] = i;
	n->number[1] = i >> 32;
#endif

	n->sign = 1;
}

void gorbn_from_int(gorbn_bignum* n, gorbn_stmp_t i){
	GORBN_ASSERT(n);

	gorbn_init(n);

	n->sign = 1;
	if(i < 0){
		n->sign = -1;
	}

	i = GORBN_ABS(i);
	
#if (GORBN_SZWORD == 1)
	n->number[0] = (i & 0x000000ff);
	n->number[1] = (i & 0x0000ff00) >> 8;
	n->number[2] = (i & 0x00ff0000) >> 16;
	n->number[3] = (i & 0xff000000) >> 24;
#elif (GORBN_SZWORD == 2)
	n->number[0] = i;
	n->number[1] = i >> 16;
#elif (GORBN_SZWORD == 4)
	n->number[0] = i;
	n->number[1] = i >> 32;
#endif
}

void gorbn_from_data(gorbn_bignum* n, void* data, uint32_t data_size){
	GORBN_ASSERT(n);
	GORBN_ASSERT(data);

	int i;

	uint8_t* at = (uint8_t*)data;
	uint8_t* to = (uint8_t*)n->number;

	gorbn_init(n);

	for(i = 0; i < data_size; i++){
		*to++ = *at++;
	}
}

void gorbn_to_data(void* data, uint32_t data_size, gorbn_bignum* n){
	GORBN_ASSERT(data);
	GORBN_ASSERT(n);
	GORBN_ASSERT((GORBN_SZARR * GORBN_SZWORD) <= data_size); /*data must be big enough to hold big number inside*/

	int i;

	gorbn_t* at = n->number;
	gorbn_t* to = (gorbn_t*)data;

	for(i = 0; i < GORBN_SZARR; i++){
		*to++ = *at++;
	}
}

static inline void _gorbn_internal_addition(
	gorbn_bignum* dst,
	gorbn_bignum* a, int a_sign,
	gorbn_bignum* b, int b_sign)
{
	gorbn_utmp_t res;
	gorbn_utmp_t tmp1;
	gorbn_utmp_t tmp2;
	int borrow = 0;
	int carry = 0;
	int i;

	gorbn_bignum* big;
	gorbn_bignum* low;

	if(a_sign == b_sign){
		/* Just sum those 2 values */
		for(i = 0; i < GORBN_SZARR; i++){
			tmp1 = a->number[i] + b->number[i] + carry;
			carry = (tmp1 > GORBN_MAX_VAL);
			dst->number[i] = (tmp1 & GORBN_MAX_VAL);
		}

		/* Set the initial sign */
		dst->sign = a_sign;
	}
	else{
		/* Detecting which number is bigger */
		int comp_res = gorbn_cmp_mod(a, b);
		if(comp_res >= 0){
			big = a;
			low = b;
		}
		else{
			big = b;
			low = a;
		}

		/* Subtracting: dst = big - small */
		for (i = 0; i < GORBN_SZARR; ++i) {
			tmp1 = (gorbn_utmp_t)big->number[i] + ((gorbn_utmp_t)GORBN_MAX_VAL + 1);
			tmp2 = (gorbn_utmp_t)low->number[i] + borrow;
			res = (tmp1 - tmp2);
			dst->number[i] = (gorbn_t)(res & GORBN_MAX_VAL);
			borrow = (res <= GORBN_MAX_VAL);
		}

		/* Detecting the result sign */
		if(a_sign > 0 && comp_res >= 0){
			dst->sign = 1;
		}
		else{
			dst->sign = -1;
		}
	}
}

void gorbn_add(gorbn_bignum* r, gorbn_bignum* a, gorbn_bignum* b){
	GORBN_ASSERT(r);
	GORBN_ASSERT(a);
	GORBN_ASSERT(b);

	_gorbn_internal_addition(r, a, a->sign, b, b->sign);
}

void gorbn_sub(gorbn_bignum* r, gorbn_bignum* a, gorbn_bignum* b){
	GORBN_ASSERT(r);
	GORBN_ASSERT(a);
	GORBN_ASSERT(b);

	_gorbn_internal_addition(r, a, a->sign, b, b->sign * -1);
}

int gorbn_cmp(gorbn_bignum* a, gorbn_bignum* b){
	GORBN_ASSERT(a);
	GORBN_ASSERT(b);

	int i;
	int res = GORBN_CMP_EQUAL;
	int mod_res = 0;

	if(a->sign != b->sign){
		res = (a->sign > b->sign) ? GORBN_CMP_LARGER : GORBN_CMP_SMALLER;
	}
	else{
		i = GORBN_SZARR;
		do{
			i -= 1; /* Decrement first, to start with last array element */
			if (a->number[i] > b->number[i])
			{
				mod_res = GORBN_CMP_LARGER;
				break;
			}
			else if (a->number[i] < b->number[i])
			{
				mod_res = GORBN_CMP_SMALLER;
				break;
			}
		} while(i != 0);

		res = mod_res * a->sign;
	}

	return(res);
}

int gorbn_cmp_mod(gorbn_bignum* a, gorbn_bignum* b){
	GORBN_ASSERT(a);
	GORBN_ASSERT(b);

	int i = GORBN_SZARR;
	do{
		i -= 1; /* Decrement first, to start with last array element */
		if (a->number[i] > b->number[i])
		{
			return GORBN_CMP_LARGER;
		}
		else if (a->number[i] < b->number[i])
		{
			return GORBN_CMP_SMALLER;
		}
	} while(i != 0);

	return(GORBN_CMP_EQUAL);
}

#endif