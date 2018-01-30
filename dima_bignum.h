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
#ifndef DIMA_BIGNUM_WORD_SIZE
#define DIMA_BIGNUM_WORD_SIZE 4
#endif

/* Size of big-numbers in bytes */
#define DIMA_BIGNUM_ARRAY_SIZE    (512 / DIMA_BIGNUM_WORD_SIZE)


/* Here comes the compile-time specialization for how large the underlying array size should be. */
/* The choices are 1, 2 and 4 bytes in size with uint32, uint64 for DIMA_BIGNUM_WORD_SIZE==4, as temporary. */
#ifndef DIMA_BIGNUM_WORD_SIZE
#error Must define DIMA_BIGNUM_WORD_SIZE to be 1, 2, 4
#elif (DIMA_BIGNUM_WORD_SIZE == 1)
  /* Data type of array in structure */
#define DIMA_BIGNUM_DTYPE                    uint8_t
/* bitmask for getting MSB */
#define DIMA_BIGNUM_DTYPE_MSB                ((DIMA_BIGNUM_DTYPE_TMP)(0x80))
/* Data-type larger than DIMA_BIGNUM_DTYPE, for holding intermediate results of calculations */
#define DIMA_BIGNUM_DTYPE_TMP                uint32_t
/* sprintf format string */
#define SPRINTF_FORMAT_STR       "%.02x"
#define SSCANF_FORMAT_STR        "%2hhx"
/* Max value of integer type */
#define DIMA_BIGNUM_MAX_VAL                  ((DIMA_BIGNUM_DTYPE_TMP)0xFF)
#elif (DIMA_BIGNUM_WORD_SIZE == 2)
#define DIMA_BIGNUM_DTYPE                    uint16_t
#define DIMA_BIGNUM_DTYPE_TMP                uint32_t
#define DIMA_BIGNUM_DTYPE_MSB                ((DIMA_BIGNUM_DTYPE_TMP)(0x8000))
#define SPRINTF_FORMAT_STR       "%.04x"
#define SSCANF_FORMAT_STR        "%4hx"
#define DIMA_BIGNUM_MAX_VAL                  ((DIMA_BIGNUM_DTYPE_TMP)0xFFFF)
#elif (DIMA_BIGNUM_WORD_SIZE == 4)
#define DIMA_BIGNUM_DTYPE                    uint32_t
#define DIMA_BIGNUM_DTYPE_TMP                uint64_t
#define DIMA_BIGNUM_DTYPE_MSB                ((DIMA_BIGNUM_DTYPE_TMP)(0x80000000))
#define SPRINTF_FORMAT_STR       "%.08x"
#define SSCANF_FORMAT_STR        "%8x"
#define DIMA_BIGNUM_MAX_VAL                  ((DIMA_BIGNUM_DTYPE_TMP)0xFFFFFFFF)
#endif
#ifndef DIMA_BIGNUM_DTYPE
#error DIMA_BIGNUM_DTYPE must be defined to uint8_t, uint16_t uint32_t or whatever
#endif


/* Custom assert macro - easy to disable */
#define require(p, msg) assert(p && #msg)

/* Data-holding structure: array of DIMA_BIGNUM_DTYPEs */
struct bn
{
	DIMA_BIGNUM_DTYPE array[DIMA_BIGNUM_ARRAY_SIZE];
};



/* Tokens returned by bignum_cmp() for value comparison */
enum { SMALLER = -1, EQUAL = 0, LARGER = 1 };

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
	DIMA_BIGNUM_DEF void bignum_from_int(struct bn* n, DIMA_BIGNUM_DTYPE_TMP i);
	DIMA_BIGNUM_DEF int  bignum_to_int(struct bn* n);
	DIMA_BIGNUM_DEF void bignum_from_string(struct bn* n, char* str, int nbytes);
	DIMA_BIGNUM_DEF void bignum_to_string(struct bn* n, char* str, int maxsize);
	DIMA_BIGNUM_DEF void bignum_from_data(struct bn* n, void* data, int datasizeinbytes);
	DIMA_BIGNUM_DEF void bignum_to_data(struct bn* n, void* data, int maxsize);
	DIMA_BIGNUM_DEF void bignum_copy(struct bn* dst, struct bn* src);        /* Copy src into dst -- dst := src */

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
	for (; i < DIMA_BIGNUM_ARRAY_SIZE; ++i)
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
	for (i = (DIMA_BIGNUM_ARRAY_SIZE - 1); i >= nwords; --i)
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
	for (i = (DIMA_BIGNUM_ARRAY_SIZE - 1); i > 0; --i)
	{
		a->array[i] = (a->array[i] << 1) | (a->array[i - 1] >> ((8 * DIMA_BIGNUM_WORD_SIZE) - 1));
	}
	a->array[0] <<= 1;
}


static void _rshift_one_bit(struct bn* a)
{
	require(a, "a is null");

	int i;
	for (i = 0; i < (DIMA_BIGNUM_ARRAY_SIZE - 1); ++i)
	{
		a->array[i] = (a->array[i] >> 1) | (a->array[i + 1] << ((8 * DIMA_BIGNUM_WORD_SIZE) - 1));
	}
	a->array[DIMA_BIGNUM_ARRAY_SIZE - 1] >>= 1;
}


static inline int _get_msb(struct bn* a) {
	int result = 0;

	for (int i = DIMA_BIGNUM_ARRAY_SIZE - 1; i >= 0; i--) {
		if (a->array[i] != 0) {
			result = i;
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
	for (i = 0; i < DIMA_BIGNUM_ARRAY_SIZE; ++i)
	{
		n->array[i] = 0;
	}
}


void bignum_copy(struct bn* dst, struct bn* src)
{
	require(dst, "dst is null");
	require(src, "src is null");

	int i;
	for (i = 0; i < DIMA_BIGNUM_ARRAY_SIZE; ++i)
	{
		dst->array[i] = src->array[i];
	}
}


void bignum_from_int(struct bn* n, DIMA_BIGNUM_DTYPE_TMP i)
{
	require(n, "n is null");

	bignum_init(n);

	/* Endianness issue if machine is not little-endian? */
#ifdef DIMA_BIGNUM_WORD_SIZE
#if (DIMA_BIGNUM_WORD_SIZE == 1)
	n->array[0] = (i & 0x000000ff);
	n->array[1] = (i & 0x0000ff00) >> 8;
	n->array[2] = (i & 0x00ff0000) >> 16;
	n->array[3] = (i & 0xff000000) >> 24;
#elif (DIMA_BIGNUM_WORD_SIZE == 2)
	n->array[0] = (i & 0x0000ffff);
	n->array[1] = (i & 0xffff0000) >> 16;
#elif (DIMA_BIGNUM_WORD_SIZE == 4)
	n->array[0] = i;
	DIMA_BIGNUM_DTYPE_TMP num_32 = 32;
	DIMA_BIGNUM_DTYPE_TMP tmp = i >> num_32; /* bit-shift with U64 operands to force 64-bit results */
	n->array[1] = tmp;
#endif
#endif
}


int bignum_to_int(struct bn* n)
{
	require(n, "n is null");

	int ret = 0;

	/* Endianness issue if machine is not little-endian? */
#if (DIMA_BIGNUM_WORD_SIZE == 1)
	ret += n->array[0];
	ret += n->array[1] << 8;
	ret += n->array[2] << 16;
	ret += n->array[3] << 24;
#elif (DIMA_BIGNUM_WORD_SIZE == 2)
	ret += n->array[0];
	ret += n->array[1] << 16;
#elif (DIMA_BIGNUM_WORD_SIZE == 4)
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

	DIMA_BIGNUM_DTYPE tmp;                        /* DIMA_BIGNUM_DTYPE is defined in bn.h - uint{8,16,32,64}_t */
	int i = nbytes - (2 * DIMA_BIGNUM_WORD_SIZE); /* index into string */
	int j = 0;                        /* index into array */

	/* reading last hex-byte "MSB" from string first -> big endian */
	/* MSB ~= most significant byte / block ? :) */
	while (i >= 0)
	{
		tmp = 0;
		sscanf(&str[i], SSCANF_FORMAT_STR, &tmp);
		n->array[j] = tmp;
		i -= (2 * DIMA_BIGNUM_WORD_SIZE); /* step DIMA_BIGNUM_WORD_SIZE hex-byte(s) back in the string. */
		j += 1;               /* step one element forward in the array. */
	}
}


void bignum_to_string(struct bn* n, char* str, int nbytes)
{
	require(n, "n is null");
	require(str, "str is null");
	require(nbytes > 0, "nbytes must be positive");
	require((nbytes & 1) == 0, "string format must be in hex -> equal number of bytes");

	int j = DIMA_BIGNUM_ARRAY_SIZE - 1; /* index into array - reading "MSB" first -> big-endian */
	int i = 0;                 /* index into string representation. */

	/* reading last array-element "MSB" first -> big endian */
	while ((j >= 0) && (nbytes > (i + 1)))
	{
		sprintf(&str[i], SPRINTF_FORMAT_STR, n->array[j]);
		i += (2 * DIMA_BIGNUM_WORD_SIZE); /* step DIMA_BIGNUM_WORD_SIZE hex-byte(s) forward in the string. */
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
	require(datasizeinbytes > 0, "data size must be positive");

	int i = 0;

	uint8_t* at = (uint8_t*)data;
	uint8_t* to = (uint8_t*)n->array;

	for (; i < datasizeinbytes; i++) {
		*to++ = *at++;
	}
}

void bignum_to_data(struct bn* n, void* data, int maxsize)
{
	require(n, "n is null");
	require(data, "data is null");
	require(maxsize > 0, "maxsize must be positive");
	require((DIMA_BIGNUM_ARRAY_SIZE * DIMA_BIGNUM_WORD_SIZE) <= maxsize, "maxsize is not big enough to fit array inside");

	DIMA_BIGNUM_DTYPE* at = n->array;
	DIMA_BIGNUM_DTYPE* to = (DIMA_BIGNUM_DTYPE*)data;

	for (int i = 0; i < DIMA_BIGNUM_ARRAY_SIZE; i++) {
		*to++ = *at++;
	}
}

void bignum_dec(struct bn* n)
{
	require(n, "n is null");

	DIMA_BIGNUM_DTYPE tmp; /* copy of n */
	DIMA_BIGNUM_DTYPE res;

	int i;
	for (i = 0; i < DIMA_BIGNUM_ARRAY_SIZE; ++i)
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

	DIMA_BIGNUM_DTYPE res;
	DIMA_BIGNUM_DTYPE_TMP tmp; /* copy of n */

	int i;
	for (i = 0; i < DIMA_BIGNUM_ARRAY_SIZE; ++i)
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

	DIMA_BIGNUM_DTYPE_TMP tmp;
	int carry = 0;
	int i;
	for (i = 0; i < DIMA_BIGNUM_ARRAY_SIZE; ++i)
	{
		tmp = a->array[i] + b->array[i] + carry;
		carry = (tmp > DIMA_BIGNUM_MAX_VAL);
		c->array[i] = (tmp & DIMA_BIGNUM_MAX_VAL);
	}
}


void bignum_sub(struct bn* a, struct bn* b, struct bn* c)
{
	require(a, "a is null");
	require(b, "b is null");
	require(c, "c is null");

	DIMA_BIGNUM_DTYPE_TMP res;
	DIMA_BIGNUM_DTYPE_TMP tmp1;
	DIMA_BIGNUM_DTYPE_TMP tmp2;
	int borrow = 0;
	int i;
	for (i = 0; i < DIMA_BIGNUM_ARRAY_SIZE; ++i)
	{
		tmp1 = (DIMA_BIGNUM_DTYPE_TMP)a->array[i] + (DIMA_BIGNUM_MAX_VAL + 1); /* + number_base */
		tmp2 = (DIMA_BIGNUM_DTYPE_TMP)b->array[i] + borrow;;
		res = (tmp1 - tmp2);
		c->array[i] = (DIMA_BIGNUM_DTYPE)(res & DIMA_BIGNUM_MAX_VAL); /* "modulo number_base" == "% (number_base - 1)" if number_base is 2^N */
		borrow = (res <= DIMA_BIGNUM_MAX_VAL);
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

	bignum_init(c);

	for (i = 0; i < DIMA_BIGNUM_ARRAY_SIZE; ++i)
	{
		bignum_init(&row);

		for (j = 0; j < DIMA_BIGNUM_ARRAY_SIZE; ++j)
		{
			if (i + j < DIMA_BIGNUM_ARRAY_SIZE)
			{
				bignum_init(&tmp);
				DIMA_BIGNUM_DTYPE_TMP intermediate = ((DIMA_BIGNUM_DTYPE_TMP)a->array[i] * (DIMA_BIGNUM_DTYPE_TMP)b->array[j]);
				bignum_from_int(&tmp, intermediate);
				_lshift_word(&tmp, i + j);
				bignum_add(&tmp, &row, &row);
			}
		}
		bignum_add(c, &row, c);
	}
}

void bignum_mul_karatsuba(struct bn* a, struct bn* b, struct bn* res) {
	int a_msb = _get_msb(a);
	int b_msb = _get_msb(b);

	if (a_msb < 1 || 
		b_msb < 1) 
	{
		return bignum_mul(a, b, res);
	}


}

void bignum_div(struct bn* a, struct bn* b, struct bn* c)
{
	require(a, "a is null");
	require(b, "b is null");
	require(c, "c is null");

	struct bn current;
	struct bn denom;
	struct bn tmp;

	bignum_from_int(&current, 1);               // int current = 1;
	bignum_copy(&denom, b);                   // denom = b
	bignum_copy(&tmp, a);                     // tmp   = a

	const DIMA_BIGNUM_DTYPE_TMP half_max = 1 + (DIMA_BIGNUM_DTYPE_TMP)(DIMA_BIGNUM_MAX_VAL / 2);
	bool overflow = false;
	while (bignum_cmp(&denom, a) != LARGER)     // while (denom <= a) {
	{
		if (denom.array[DIMA_BIGNUM_ARRAY_SIZE - 1] >= half_max)
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
		if (bignum_cmp(&tmp, &denom) != SMALLER)  //   if (dividend >= denom)
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
	const int nbits_pr_word = (DIMA_BIGNUM_WORD_SIZE * 8);
	int nwords = nbits / nbits_pr_word;
	if (nwords != 0)
	{
		_lshift_word(a, nwords);
		nbits -= (nwords * nbits_pr_word);
	}

	if (nbits != 0)
	{
		int i;
		for (i = (DIMA_BIGNUM_ARRAY_SIZE - 1); i > 0; --i)
		{
			a->array[i] = (a->array[i] << nbits) | (a->array[i - 1] >> ((8 * DIMA_BIGNUM_WORD_SIZE) - nbits));
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
	const int nbits_pr_word = (DIMA_BIGNUM_WORD_SIZE * 8);
	int nwords = nbits / nbits_pr_word;
	if (nwords != 0)
	{
		_rshift_word(a, nwords);
		nbits -= (nwords * nbits_pr_word);
	}

	if (nbits != 0)
	{
		int i;
		for (i = 0; i < (DIMA_BIGNUM_ARRAY_SIZE - 1); ++i)
		{
			a->array[i] = (a->array[i] >> nbits) | (a->array[i + 1] << ((8 * DIMA_BIGNUM_WORD_SIZE) - nbits));
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
	for (i = 0; i < DIMA_BIGNUM_ARRAY_SIZE; ++i)
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
	for (i = 0; i < DIMA_BIGNUM_ARRAY_SIZE; ++i)
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
	for (i = 0; i < DIMA_BIGNUM_ARRAY_SIZE; ++i)
	{
		c->array[i] = (a->array[i] ^ b->array[i]);
	}
}


int bignum_cmp(struct bn* a, struct bn* b)
{
	require(a, "a is null");
	require(b, "b is null");

	int i = DIMA_BIGNUM_ARRAY_SIZE;
	do
	{
		i -= 1; /* Decrement first, to start with last array element */
		if (a->array[i] > b->array[i])
		{
			return LARGER;
		}
		else if (a->array[i] < b->array[i])
		{
			return SMALLER;
		}
	} while (i != 0);

	return EQUAL;
}


int bignum_is_zero(struct bn* n)
{
	require(n, "n is null");

	int i;
	for (i = 0; i < DIMA_BIGNUM_ARRAY_SIZE; ++i)
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

	if (bignum_cmp(b, c) == EQUAL)
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


#endif