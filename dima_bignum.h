#ifndef DIMA_BIGNUM_H_INCLUDED
#define DIMA_BIGNUM_H_INCLUDED

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

	OTHER:
		Basic code was taken from https://github.com/kokke/tiny-bignum-c
		and then modified.
*/

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <assert.h>

/* This macro defines the word size in bytes of the array that constitues the big-number data structure. */
#ifndef DBN_SZWORD
#define DBN_SZWORD 4
#endif

/* Size of big-numbers in bytes */
#define DBN_SZARR    (512 / DBN_SZWORD)


/* Here comes the compile-time specialization for how large the underlying array size should be. */
/* The choices are 1, 2 and 4 bytes in size with uint32, uint64 for DBN_SZWORD==4, as temporary. */
#ifndef DBN_SZWORD
#error Must define DBN_SZWORD to be 1, 2, 4
#elif (DBN_SZWORD == 1)
  /* Data type of array in structure */
#define DBN_T                    uint8_t
/* bitmask for getting MSB */
#define DBN_T_MSB                ((DBN_T_TMP)(0x80))
/* Data-type larger than DBN_T, for holding intermediate results of calculations */
#define DBN_T_UTMP                uint32_t
#define DBN_T_STMP                int32_t
/* sprintf format string */
#define SPRINTF_FORMAT_STR       "%.02x"
#define SSCANF_FORMAT_STR        "%2hhx"
/* Max value of integer type */
#define DBN_MAX_VAL                  ((DBN_T_TMP)0xFF)
#elif (DBN_SZWORD == 2)
#define DBN_T                    uint16_t
#define DBN_T_STMP               int32_t
#define DBN_T_UTMP               uint32_t
#define DBN_T_MSB                ((DBN_T_TMP)(0x8000))
#define SPRINTF_FORMAT_STR       "%.04x"
#define SSCANF_FORMAT_STR        "%4hx"
#define DBN_MAX_VAL                  ((DBN_T_TMP)0xFFFF)
#elif (DBN_SZWORD == 4)
#define DBN_T                    uint32_t
#define DBN_T_STMP               int64_t
#define DBN_T_UTMP               uint64_t
#define DBN_T_MSB                ((DBN_T_TMP)(0x80000000))
#define SPRINTF_FORMAT_STR       "%.08x"
#define SSCANF_FORMAT_STR        "%8x"
#define DBN_MAX_VAL                  ((DBN_T_UTMP)0xFFFFFFFF)
#endif
#ifndef DBN_T
#error DBN_T must be defined to uint8_t, uint16_t uint32_t or whatever
#endif

/* Custom assert macro - easy to disable */
#define require(p, msg) assert(p && #msg)

/* Custom macro for getting absolute value of the signed integer*/
#define DIMA_BIGNUM_ABS(val) (((val) >= 0) ? (val) : (-(val)))

/*Custom macro for getting the biggest number from two numbers*/
#define DIMA_BIGNUM_MAX(a, b) (((a) > (b)) ? (a) : (b))

/* Data-holding structure: array of DBN_Ts */
struct bn
{
	int32_t sign; /*1 - positive or zero, -1 - negative*/
	DBN_T array[DBN_SZARR];
};



/* Tokens returned by bignum_cmp() for value comparison */
#define DIMA_BIGNUM_CMP_LARGER 1
#define DIMA_BIGNUM_CMP_SMALLER -1
#define DIMA_BIGNUM_CMP_EQUAL 0


#ifdef DIMA_BIGNUM_STATIC
#define DIMA_BIGNUM_DEF static
#else
#define DIMA_BIGNUM_DEF extern
#endif

#ifdef __cplusplus
extern "C" {
#endif

	/* Initialization functions: */
	DIMA_BIGNUM_DEF void bignum_init(struct bn* n);
	DIMA_BIGNUM_DEF void bignum_from_uint(struct bn* n, DBN_T_UTMP i);
	DIMA_BIGNUM_DEF void bignum_from_int(struct bn* n, DBN_T_STMP i);
	DIMA_BIGNUM_DEF int  bignum_to_int(struct bn* n);
	DIMA_BIGNUM_DEF void bignum_from_string(struct bn* n, char* str, int nbytes);
	DIMA_BIGNUM_DEF void bignum_to_string(struct bn* n, char* str, int maxsize);
	DIMA_BIGNUM_DEF void bignum_from_data(struct bn* n, void* data, int datasizeinbytes);
	DIMA_BIGNUM_DEF void bignum_to_data(struct bn* n, void* data, int maxsize);
	DIMA_BIGNUM_DEF void bignum_copy(struct bn* dst, struct bn* src);        /* Copy src into dst -- dst := src */
	DIMA_BIGNUM_DEF void bignum_set_sign(struct bn* dst, int32_t sign);

	/* Basic arithmetic operations: */
	DIMA_BIGNUM_DEF void bignum_add(struct bn* a, struct bn* b, struct bn* c); /* c = a + b */
	DIMA_BIGNUM_DEF void bignum_sub(struct bn* a, struct bn* b, struct bn* c); /* c = a - b */
	DIMA_BIGNUM_DEF void bignum_mul(struct bn* a, struct bn* b, struct bn* c); /* c = a * b */
	DIMA_BIGNUM_DEF void bignum_div(struct bn* a, struct bn* b, struct bn* c); /* c = a / b */
	DIMA_BIGNUM_DEF void bignum_mod(struct bn* a, struct bn* b, struct bn* c); /* c = a % b */

	/* Bitwise operations: */
	DIMA_BIGNUM_DEF void bignum_and(struct bn* a, struct bn* b, struct bn* c); /* c = a & b */
	DIMA_BIGNUM_DEF void bignum_or(struct bn* a, struct bn* b, struct bn* c);  /* c = a | b */
	DIMA_BIGNUM_DEF void bignum_xor(struct bn* a, struct bn* b, struct bn* c); /* c = a ^ b */
	DIMA_BIGNUM_DEF void bignum_lshift(struct bn* a, struct bn* b, int nbits); /* b = a << nbits */
	DIMA_BIGNUM_DEF void bignum_rshift(struct bn* a, struct bn* b, int nbits); /* b = a >> nbits */

	/* Special operators and comparison */
	DIMA_BIGNUM_DEF int  bignum_cmp(struct bn* a, struct bn* b);               /* Compare: returns LARGER, EQUAL or SMALLER */
	DIMA_BIGNUM_DEF int  bignum_is_zero(struct bn* n);                         /* For comparison with zero */
	DIMA_BIGNUM_DEF void bignum_inc(struct bn* n);                             /* Increment: add one to n */
	DIMA_BIGNUM_DEF void bignum_dec(struct bn* n);                             /* Decrement: subtract one from n */
	DIMA_BIGNUM_DEF void bignum_pow(struct bn* a, struct bn* b, struct bn* c); /* Calculate a^b -- e.g. 2^10 => 1024 */

	DIMA_BIGNUM_DEF void bignum_mul_pow2(struct bn* a, int32_t k, struct bn* c); /* Calculate c=a*(2^k) */

#ifdef __cplusplus
}
#endif

#endif

#if defined(DIMA_BIGNUM_IMPLEMENTATION) && !defined(DIMA_BIGNUM_IMPLEMENTATION_DONE)
#define DIMA_BIGNUM_IMPLEMENTATION_DONE

/* Functions for shifting number in-place. */
static void _rshift_word(struct bn* a, int nwords)
{
	/* Naive method: */
	require(a, "a is null");
	require(nwords >= 0, "no negative shifts");

	int i;
	for (i = 0; i < nwords; ++i)
	{
		a->array[i] = a->array[i + 1];
	}
	for (; i < DBN_SZARR; ++i)
	{
		a->array[i] = 0;
	}
}


static void _lshift_word(struct bn* a, int nwords)
{
	require(a, "a is null");
	require(nwords >= 0, "no negative shifts");

	int i;
	/* Shift whole words */
	for (i = (DBN_SZARR - 1); i >= nwords; --i)
	{
		a->array[i] = a->array[i - nwords];
	}
	/* Zero pad shifted words. */
	for (; i >= 0; --i)
	{
		a->array[i] = 0;
	}
}


static void _lshift_one_bit(struct bn* a)
{
	require(a, "a is null");

	int i;
	for (i = (DBN_SZARR - 1); i > 0; --i)
	{
		a->array[i] = (a->array[i] << 1) | (a->array[i - 1] >> ((8 * DBN_SZWORD) - 1));
	}
	a->array[0] <<= 1;
}


static void _rshift_one_bit(struct bn* a)
{
	require(a, "a is null");

	int i;
	for (i = 0; i < (DBN_SZARR - 1); ++i)
	{
		a->array[i] = (a->array[i] >> 1) | (a->array[i + 1] << ((8 * DBN_SZWORD) - 1));
	}
	a->array[DBN_SZARR - 1] >>= 1;
}


static inline int _get_szbytes(struct bn* a) {
	int result = 0;

	for (int i = DBN_SZARR - 1; i >= 0; i--) {
		if (a->array[i] != 0) {
			result = i + 1;
			break;
		}
	}

	return(result);
}


/* Public / Exported functions. */
void bignum_init(struct bn* n)
{
	require(n, "n is null");

	int i;
	for (i = 0; i < DBN_SZARR; ++i)
	{
		n->array[i] = 0;
	}
	n->sign = 1;
}


void bignum_copy(struct bn* dst, struct bn* src)
{
	require(dst, "dst is null");
	require(src, "src is null");

	int i;
	for (i = 0; i < DBN_SZARR; ++i)
	{
		dst->array[i] = src->array[i];
	}
	dst->sign = src->sign;
}


void bignum_from_uint(struct bn* n, DBN_T_UTMP i)
{
	require(n, "n is null");

	bignum_init(n);

	/* Endianness issue if machine is not little-endian? */
#ifdef DBN_SZWORD
#if (DBN_SZWORD == 1)
	n->array[0] = (i & 0x000000ff);
	n->array[1] = (i & 0x0000ff00) >> 8;
	n->array[2] = (i & 0x00ff0000) >> 16;
	n->array[3] = (i & 0xff000000) >> 24;
#elif (DBN_SZWORD == 2)
	n->array[0] = (i & 0x0000ffff);
	n->array[1] = (i & 0xffff0000) >> 16;
#elif (DBN_SZWORD == 4)
	n->array[0] = i;
	uint64_t num_32 = 32;
	uint64_t tmp = i >> num_32; /* bit-shift with U64 operands to force 64-bit results */
	n->array[1] = tmp;
#endif
#endif

	n->sign = 1;
}


void bignum_from_int(struct bn* n, DBN_T_STMP i) {
	require(n, "n is null");

	bignum_init(n);

	/*Setting sign of the new bignum*/
	int32_t result_sign = 1;
	if (i < 0) {
		result_sign = 0;
	}
	n->sign = result_sign;

	DBN_T_STMP abs_si = DIMA_BIGNUM_ABS(i);
	DBN_T_UTMP abs_i = abs_si;

	/* Endianness issue if machine is not little-endian? */
#ifdef DBN_SZWORD
#if (DBN_SZWORD == 1)
	n->array[0] = (abs_i & 0x000000ff);
	n->array[1] = (abs_i & 0x0000ff00) >> 8;
	n->array[2] = (abs_i & 0x00ff0000) >> 16;
	n->array[3] = (abs_i & 0xff000000) >> 24;
#elif (DBN_SZWORD == 2)
	n->array[0] = (abs_i & 0x0000ffff);
	n->array[1] = (abs_i & 0xffff0000) >> 16;
#elif (DBN_SZWORD == 4)
	n->array[0] = abs_i;
	uint64_t tmp = abs_i >> 32; /* bit-shift with U64 operands to force 64-bit results */
	n->array[1] = tmp;
#endif
#endif
}


int bignum_to_int(struct bn* n)
{
	require(n, "n is null");

	int ret = 0;

	/* Endianness issue if machine is not little-endian? */
#if (DBN_SZWORD == 1)
	ret += n->array[0];
	ret += n->array[1] << 8;
	ret += n->array[2] << 16;
	ret += n->array[3] << 24;
#elif (DBN_SZWORD == 2)
	ret += n->array[0];
	ret += n->array[1] << 16;
#elif (DBN_SZWORD == 4)
	ret += n->array[0];
#endif

	return ret;
}


void bignum_from_string(struct bn* n, char* str, int nbytes)
{
	require(n, "n is null");
	require(str, "str is null");
	require(nbytes > 0, "nbytes must be positive");
	require((nbytes & 1) == 0, "string format must be in hex -> equal number of bytes");

	bignum_init(n);

	DBN_T tmp;                        /* DBN_T is defined in bn.h - uint{8,16,32,64}_t */
	int i = nbytes - (2 * DBN_SZWORD); /* index into string */
	int j = 0;                        /* index into array */

	/* reading last hex-byte "MSB" from string first -> big endian */
	/* MSB ~= most significant byte / block ? :) */
	while (i >= 0)
	{
		tmp = 0;
		sscanf(&str[i], SSCANF_FORMAT_STR, &tmp);
		n->array[j] = tmp;
		i -= (2 * DBN_SZWORD); /* step DBN_SZWORD hex-byte(s) back in the string. */
		j += 1;               /* step one element forward in the array. */
	}
}


void bignum_to_string(struct bn* n, char* str, int nbytes)
{
	require(n, "n is null");
	require(str, "str is null");
	require(nbytes > 0, "nbytes must be positive");
	require((nbytes & 1) == 0, "string format must be in hex -> equal number of bytes");

	int j = DBN_SZARR - 1; /* index into array - reading "MSB" first -> big-endian */
	int i = 0;                 /* index into string representation. */

	/* reading last array-element "MSB" first -> big endian */
	while ((j >= 0) && (nbytes > (i + 1)))
	{
		sprintf(&str[i], SPRINTF_FORMAT_STR, n->array[j]);
		i += (2 * DBN_SZWORD); /* step DBN_SZWORD hex-byte(s) forward in the string. */
		j -= 1;               /* step one element back in the array. */
	}

	/* Count leading zeros: */
	j = 0;
	while (str[j] == '0')
	{
		j += 1;
	}

	/* Move string j places ahead, effectively skipping leading zeros */
	for (i = 0; i < (nbytes - j); ++i)
	{
		str[i] = str[i + j];
	}

	/* Zero-terminate string */
	str[i] = 0;
}


void bignum_from_data(struct bn* n, void* data, int datasizeinbytes)
{
	require(n, "n is null");
	require(data, "data is null");
	//require(datasizeinbytes > 0, "data size must be positive");

	int i = 0;

	uint8_t* at = (uint8_t*)data;
	uint8_t* to = (uint8_t*)n->array;

	bignum_init(n);

	for (; i < datasizeinbytes; i++) {
		*to++ = *at++;
	}
}

void bignum_to_data(struct bn* n, void* data, int maxsize)
{
	require(n, "n is null");
	require(data, "data is null");
	require(maxsize > 0, "maxsize must be positive");
	require((DBN_SZARR * DBN_SZWORD) <= maxsize, "maxsize is not big enough to fit array inside");

	DBN_T* at = n->array;
	DBN_T* to = (DBN_T*)data;

	for (int i = 0; i < DBN_SZARR; i++) {
		*to++ = *at++;
	}
}


void bignum_set_sign(struct bn* dst, int32_t sign) {
	dst->sign = sign;
}


void bignum_dec(struct bn* n)
{
	require(n, "n is null");

	DBN_T tmp; /* copy of n */
	DBN_T res;

	int i;
	for (i = 0; i < DBN_SZARR; ++i)
	{
		tmp = n->array[i];
		res = tmp - 1;
		n->array[i] = res;

		if (!(res > tmp))
		{
			break;
		}
	}
}


void bignum_inc(struct bn* n)
{
	require(n, "n is null");

	DBN_T res;
	DBN_T_UTMP tmp; /* copy of n */

	int i;
	for (i = 0; i < DBN_SZARR; ++i)
	{
		tmp = n->array[i];
		res = tmp + 1;
		n->array[i] = res;

		if (res > tmp)
		{
			break;
		}
	}
}


void bignum_add(struct bn* a, struct bn* b, struct bn* c)
{
	require(a, "a is null");
	require(b, "b is null");
	require(c, "c is null");

	DBN_T_UTMP tmp;
	int carry = 0;
	int i;
	for (i = 0; i < DBN_SZARR; ++i)
	{
		tmp = a->array[i] + b->array[i] + carry;
		carry = (tmp > DBN_MAX_VAL);
		c->array[i] = (tmp & DBN_MAX_VAL);
	}
}


void bignum_sub(struct bn* a, struct bn* b, struct bn* c)
{
	require(a, "a is null");
	require(b, "b is null");
	require(c, "c is null");

	DBN_T_UTMP res;
	DBN_T_UTMP tmp1;
	DBN_T_UTMP tmp2;
	int borrow = 0;
	int i;
	for (i = 0; i < DBN_SZARR; ++i)
	{
		tmp1 = (DBN_T_UTMP)a->array[i] + (DBN_MAX_VAL + 1); /* + number_base */
		tmp2 = (DBN_T_UTMP)b->array[i] + borrow;;
		res = (tmp1 - tmp2);
		c->array[i] = (DBN_T)(res & DBN_MAX_VAL); /* "modulo number_base" == "% (number_base - 1)" if number_base is 2^N */
		borrow = (res <= DBN_MAX_VAL);
	}
}


void bignum_mul(struct bn* a, struct bn* b, struct bn* c)
{
	require(a, "a is null");
	require(b, "b is null");
	require(c, "c is null");

	struct bn row;
	struct bn tmp;
	int i, j;

	uint32_t szword_bits = DBN_SZWORD << 3;
	DBN_T_UTMP max_val = DBN_MAX_VAL;

	int a_szbytes = _get_szbytes(a);
	int b_szbytes = _get_szbytes(b);

	bignum_init(c);

	c->sign = a->sign * b->sign;

#if 0
	for (i = 0; i < DBN_SZARR; ++i)
	{
		bignum_init(&row);

		for (j = 0; j < DBN_SZARR; ++j)
		{
			if (i + j < DBN_SZARR)
			{
				DBN_T_UTMP intermediate = ((DBN_T_UTMP)a->array[i] * (DBN_T_UTMP)b->array[j]);
				bignum_from_uint(&tmp, intermediate);
				_lshift_word(&tmp, i + j);
				bignum_add(&tmp, &row, &row);
			}
		}
		bignum_add(c, &row, c);
	}
#else
	for (i = 0; i < a_szbytes; ++i)
	{
		DBN_T_UTMP carry_lo = 0;
		DBN_T_UTMP carry_hi = 0;
		DBN_T_UTMP sum_hi = 0;

		for (j = 0; j < b_szbytes; ++j)
		{
			if (i + j < DBN_SZARR){
				DBN_T_UTMP sum_lo;
				sum_lo =
					(DBN_T_UTMP)a->array[i] *
					(DBN_T_UTMP)b->array[j] + 
					carry_lo;

				sum_hi =
					(DBN_T_UTMP)c->array[i + j] +
					(DBN_T_UTMP)(sum_lo & max_val) +
					carry_hi;

				c->array[i + j] = (DBN_T)sum_hi & max_val;

				carry_lo = (DBN_T)(sum_lo >> szword_bits) & max_val;
				carry_hi = (DBN_T)(sum_hi >> szword_bits) & max_val;
			}
		}
		do {
			if (i + j < DBN_SZARR) {
				sum_hi = (DBN_T_UTMP)c->array[i + j] + (DBN_T_UTMP)(carry_lo)+carry_hi;
				c->array[i + j] = (DBN_T)sum_hi & max_val;
				carry_hi = (DBN_T_UTMP)(sum_hi >> szword_bits) & max_val;
				carry_lo = 0;
			}
			else {
				break;
			}
		} while (carry_hi != 0);
	}
#endif
}

void bignum_mul_karatsuba(struct bn* a, struct bn* b, struct bn* res) {
	/*
		procedure karatsuba(num1, num2)
			if (num1 < 10) or (num2 < 10)
				return num1*num2
			//calculates the size of the numbers
			m = max(size_base10(num1), size_base10(num2))
			m2 = m / 2
			split the digit sequences about the middle
			high1, low1 = split_at(num1, m2)
			high2, low2 = split_at(num2, m2)
			//3 calls made to numbers approximately half the size
			z0 = karatsuba(low1, low2)
			z1 = karatsuba((low1 + high1), (low2 + high2))
			z2 = karatsuba(high1, high2)
			return (z2 * 10 ^ (2 * m2)) + ((z1 - z2 - z0) * 10 ^ (m2)) + (z0)
	*/

	int a_szbytes = _get_szbytes(a);
	int b_szbytes = _get_szbytes(b);

	if ((a_szbytes <= 1) || 
		(b_szbytes <= 1)) 
	{
		return bignum_mul(a, b, res);
	}

	int32_t m = DIMA_BIGNUM_MAX(_get_szbytes(a), _get_szbytes(b));
	int32_t m2 = m / 2;

	struct bn high1, low1;
	struct bn high2, low2;

	struct bn z0, z1, z2;

	int32_t split_at = m2;

	/*Splitting first number*/
	bignum_from_data(&low1, (uint8_t*)a->array, split_at);
	bignum_from_data(&high1, (uint8_t*)a->array + split_at, a_szbytes - split_at);

	/*Splitting second number*/
	bignum_from_data(&low2, (uint8_t*)b->array, split_at);
	bignum_from_data(&high2, (uint8_t*)b->array + split_at, b_szbytes - split_at);

	/*Use of z0 and z1 as a temp variables*/
	bignum_add(&low1, &high1, &z0);
	bignum_add(&low2, &high2, &z2);
	bignum_mul_karatsuba(&z0, &z2, &z1);
	
	/*Reusing them for algo*/
	bignum_mul_karatsuba(&low1, &low2, &z0);
	bignum_mul_karatsuba(&high1, &high2, &z2);
	
	bignum_mul_pow2(&z2, 2 * m2 * (DBN_SZWORD << 3), &high1);

	bignum_sub(&z1, &z2, &low1);
	bignum_sub(&low1, &z0, &high2);
	bignum_mul_pow2(&high2, m2 * (DBN_SZWORD << 3), &low1);

	bignum_add(&high1, &low1, &high2);
}

void bignum_div(struct bn* a, struct bn* b, struct bn* c)
{
	require(a, "a is null");
	require(b, "b is null");
	require(c, "c is null");

	struct bn current;
	struct bn denom;
	struct bn tmp;

	bignum_from_uint(&current, 1);               // int current = 1;
	bignum_copy(&denom, b);                   // denom = b
	bignum_copy(&tmp, a);                     // tmp   = a

	const DBN_T_UTMP half_max = 1 + (DBN_T_UTMP)(DBN_MAX_VAL / 2);
	bool overflow = false;
	while (bignum_cmp(&denom, a) != DIMA_BIGNUM_CMP_LARGER)     // while (denom <= a) {
	{
		if (denom.array[DBN_SZARR - 1] >= half_max)
		{
			overflow = true;
			break;
		}
		_lshift_one_bit(&current);                //   current <<= 1;
		_lshift_one_bit(&denom);                  //   denom <<= 1;
	}
	if (!overflow)
	{
		_rshift_one_bit(&denom);                  // denom >>= 1;
		_rshift_one_bit(&current);                // current >>= 1;
	}
	bignum_init(c);                             // int answer = 0;

	while (!bignum_is_zero(&current))           // while (current != 0)
	{
		if (bignum_cmp(&tmp, &denom) != DIMA_BIGNUM_CMP_SMALLER)  //   if (dividend >= denom)
		{
			bignum_sub(&tmp, &denom, &tmp);         //     dividend -= denom;
			bignum_or(c, &current, c);              //     answer |= current;
		}
		_rshift_one_bit(&current);                //   current >>= 1;
		_rshift_one_bit(&denom);                  //   denom >>= 1;
	}                                           // return answer;
}


void bignum_lshift(struct bn* a, struct bn* b, int nbits)
{
	require(a, "a is null");
	require(b, "b is null");
	require(nbits >= 0, "no negative shifts");

	/* Handle shift in multiples of word-size */
	const int nbits_pr_word = (DBN_SZWORD * 8);
	int nwords = nbits / nbits_pr_word;
	if (nwords != 0)
	{
		_lshift_word(a, nwords);
		nbits -= (nwords * nbits_pr_word);
	}

	if (nbits != 0)
	{
		int i;
		for (i = (DBN_SZARR - 1); i > 0; --i)
		{
			a->array[i] = (a->array[i] << nbits) | (a->array[i - 1] >> ((8 * DBN_SZWORD) - nbits));
		}
		a->array[i] <<= nbits;
	}
	bignum_copy(b, a);
}


void bignum_rshift(struct bn* a, struct bn* b, int nbits)
{
	require(a, "a is null");
	require(b, "b is null");
	require(nbits >= 0, "no negative shifts");

	/* Handle shift in multiples of word-size */
	const int nbits_pr_word = (DBN_SZWORD * 8);
	int nwords = nbits / nbits_pr_word;
	if (nwords != 0)
	{
		_rshift_word(a, nwords);
		nbits -= (nwords * nbits_pr_word);
	}

	if (nbits != 0)
	{
		int i;
		for (i = 0; i < (DBN_SZARR - 1); ++i)
		{
			a->array[i] = (a->array[i] >> nbits) | (a->array[i + 1] << ((8 * DBN_SZWORD) - nbits));
		}
		a->array[i] >>= nbits;
	}
	bignum_copy(b, a);
}


void bignum_mod(struct bn* a, struct bn* b, struct bn* c)
{
	/*
	mod(a,b) = a - ((a / b) * b)

	example:
	mod(8, 3) = 8 - ((8 / 3) * 3) = 2
	*/
	require(a, "a is null");
	require(b, "b is null");
	require(c, "c is null");

	struct bn tmp;

	/* c = (a / b) */
	bignum_div(a, b, c);

	/* tmp = (c * b) */
	bignum_mul(c, b, &tmp);

	/* c = a - tmp */
	bignum_sub(a, &tmp, c);
}


void bignum_and(struct bn* a, struct bn* b, struct bn* c)
{
	require(a, "a is null");
	require(b, "b is null");
	require(c, "c is null");

	int i;
	for (i = 0; i < DBN_SZARR; ++i)
	{
		c->array[i] = (a->array[i] & b->array[i]);
	}
}


void bignum_or(struct bn* a, struct bn* b, struct bn* c)
{
	require(a, "a is null");
	require(b, "b is null");
	require(c, "c is null");

	int i;
	for (i = 0; i < DBN_SZARR; ++i)
	{
		c->array[i] = (a->array[i] | b->array[i]);
	}
}


void bignum_xor(struct bn* a, struct bn* b, struct bn* c)
{
	require(a, "a is null");
	require(b, "b is null");
	require(c, "c is null");

	int i;
	for (i = 0; i < DBN_SZARR; ++i)
	{
		c->array[i] = (a->array[i] ^ b->array[i]);
	}
}


int bignum_cmp(struct bn* a, struct bn* b)
{
	require(a, "a is null");
	require(b, "b is null");

	int i = DBN_SZARR;
	do
	{
		i -= 1; /* Decrement first, to start with last array element */
		if (a->array[i] > b->array[i])
		{
			return DIMA_BIGNUM_CMP_LARGER;
		}
		else if (a->array[i] < b->array[i])
		{
			return DIMA_BIGNUM_CMP_SMALLER;
		}
	} while (i != 0);

	return DIMA_BIGNUM_CMP_EQUAL;
}


int bignum_is_zero(struct bn* n)
{
	require(n, "n is null");

	int i;
	for (i = 0; i < DBN_SZARR; ++i)
	{
		if (n->array[i])
		{
			return 0;
		}
	}

	return 1;
}


void bignum_pow(struct bn* a, struct bn* b, struct bn* c)
{
	require(a, "a is null");
	require(b, "b is null");
	require(c, "c is null");

	struct bn tmp;

	bignum_init(c);

	if (bignum_cmp(b, c) == DIMA_BIGNUM_CMP_EQUAL)
	{
		/* Return 1 when exponent is 0 -- n^0 = 1 */
		bignum_inc(c);
	}
	else
	{
		/* Copy a -> tmp */
		bignum_copy(&tmp, a);

		bignum_dec(b);

		/* Begin summing products: */
		while (!bignum_is_zero(b))
		{

			/* c = tmp * tmp */
			bignum_mul(&tmp, a, c);
			/* Decrement b by one */
			bignum_dec(b);

			bignum_copy(&tmp, c);
		}

		/* c = tmp */
		bignum_copy(c, &tmp);
	}
}

void bignum_mul_pow2(struct bn* a, int32_t k, struct bn* c) 
{
	require(a, "a is null");
	require(c, "c is null");
	require(k >= 0, "k must be greater or equal to zero");

	struct bn pow2;
	int32_t word_index = k / (DBN_SZWORD << 3);
	int32_t bit_offset = k & ((DBN_SZWORD << 3) - 1);

	bignum_init(c);
	bignum_init(&pow2);

	pow2.array[word_index] = 1 << bit_offset;

	bignum_mul(a, &pow2, c);
}


#endif