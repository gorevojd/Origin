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
*/

#define GORBN_SZWORD 2
#define GORBN_SZARR (32 / GORBN_SZWORD)

#ifndef GORBN_SZWORD
#error GORBN_SZWORD must be defined
#elif (GORBN_SZWORD == 1)
#define gorbn_t unsigned char
#define gorbn_utmp_t unsigned int
#define gorbn_stmp_t signed int
#define GORBN_MAX_VAL 0xFF
#define GORBN_SZWORD_BITS 8
#define GORBN_SZWORD_BITS_MINUS_ONE 7
#define GORBN_HIGH_BIT_SET 0x80

#elif (GORBN_SZWORD == 2)
#define gorbn_t unsigned short
#define gorbn_utmp_t unsigned int
#define gorbn_stmp_t signed int
#define GORBN_MAX_VAL 0xFFFF
#define GORBN_SZWORD_BITS 16
#define GORBN_SZWORD_BITS_MINUS_ONE 15
#define GORBN_HIGH_BIT_SET 0x8000

#elif (GORBN_SZWORD == 4)
#define gorbn_t unsigned int
#define gorbn_utmp_t unsigned long long
#define gorbn_stmp_t signed long long
#define GORBN_MAX_VAL 0xFFFFFFFF
#define GORBN_SZWORD_BITS 32
#define GORBN_SZWORD_BITS_MINUS_ONE 31
#define GORBN_HIGH_BIT_SET 0x80000000

#elif
#error GORBN_SZWORD must be defined to 1, 2 or 4
#endif

#define GORBN_SZARR_BITS_TOTAL (GORBN_SZARR * GORBN_SZWORD_BITS)

typedef struct gorec_point {
	gorbn_t x[GORBN_SZARR];
	gorbn_t y[GORBN_SZARR];
	gorbn_t z[GORBN_SZARR];

	int is_inf;
} gorec_point;

typedef struct gorec_curve {
	gorbn_t a[GORBN_SZARR];
	gorbn_t b[GORBN_SZARR];

	gorbn_t p[GORBN_SZARR];
	gorbn_t q[GORBN_SZARR];

	gorec_point g;
} gorec_curve;

/* Custom macro for getting absolute value of the signed integer*/
#define GORBN_ABS(val) (((val) >= 0) ? (val) : (-(val)))

/*Custom macro for getting the biggest number from two numbers*/
#define GORBN_MAX(a, b) (((a) > (b)) ? (a) : (b))

/*Custom macro for getting minimum number from two numbers*/
#define GORBN_MIN(a, b) (((a) < (b)) ? (a) : (b))

/*Custom macro for clamping*/
#define GORBN_CLAMP(a, min, max) (GORBN_MAX(min, GORBN_MIN(max, a)))

#ifdef GOR_BIGNUM_SAFE
#ifndef GOR_BIGNUM_ENABLE_ASSERTS
#define GOR_BIGNUM_ENABLE_ASSERTS
#endif
#else
#define GOR_BIGNUM_FAST
#endif

#ifndef GOR_BIGNUM_FAST
#define GORBN_SAFE_CONDITION_BEGIN(cond) if(cond) {
#define GORBN_SAFE_CONDITION_END() }
#else
#define GORBN_SAFE_CONDITION_BEGIN(...)
#define GORBN_SAFE_CONDITION_END()
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

#define GOREC_PT_ADD(name) void name(gorec_point* r, gorec_point* a, gorec_point* b, gorec_curve* crv)
typedef GOREC_PT_ADD(gorec_pt_add_type);

#define GOREC_PT_DOUBLE(name) void name(gorec_point* r, gorec_point* a, gorec_curve* crv)
typedef GOREC_PT_DOUBLE(gorec_pt_double_type);

#ifdef __cplusplus
extern "C" {
#endif

	/*Initialization functions*/
	/*
		Fills n with n_digit_count zeros
	*/
	GORBN_DEF void gorbn_init(gorbn_t* n, int n_digit_count);
	GORBN_DEF void gorbn_copy(gorbn_t* dst, gorbn_t* src);
	//GORBN_DEF void gorbn_from_uint(gorbn_t* n, gorbn_utmp_t i);
	//GORBN_DEF void gorbn_from_int(gorbn_t* n, gorbn_stmp_t i);
	GORBN_DEF void gorbn_from_data(gorbn_t* n, void* data, uint32_t data_size);
	GORBN_DEF void gorbn_to_data(void* data, uint32_t data_size, gorbn_t* n);

	/* Arithmetic operations: */
	GORBN_DEF int gorbn_add(
		gorbn_t* r,
		gorbn_t* a,
		gorbn_t* b); /* r = a + b */

	GORBN_DEF int gorbn_sub(
		gorbn_t* r,
		gorbn_t* a,
		gorbn_t* b); /* r = a - b */

	GORBN_DEF void gorbn_mul(
		gorbn_t* r,
		gorbn_t* a,
		gorbn_t* b); /* r = a * b */

	GORBN_DEF void gorbn_div(
		gorbn_t* q,
		gorbn_t* r,
		gorbn_t* x, int x_digit_count_alloc,
		gorbn_t* y, int y_digit_count_alloc); /* q = a / b; a = q * b + r*/

	GORBN_DEF void gorbn_div_word(
		gorbn_t* q, gorbn_t* r,
		gorbn_t* x, int x_digit_count_alloc,
		gorbn_t w);

	GORBN_DEF void gorbn_sqr(gorbn_t* r, gorbn_t* a);                  /* r = a ^ 2 */
	GORBN_DEF void gorbn_mul_word(gorbn_t* r, gorbn_t* a, gorbn_t w);
	GORBN_DEF void gorbn_pow(gorbn_t* r, gorbn_t* a, gorbn_t* b);
	GORBN_DEF void gorbn_mul_pow2(gorbn_t* r, gorbn_t* a, int k); /* r = a * (2 ^ k) */
	GORBN_DEF void gorbn_div_pow2(gorbn_t* r, gorbn_t* a, int k); /* r = a / (2 ^ k) */
	//GORBN_DEF void gorbn_gcd(gorbn_t* r, gorbn_t* a, gorbn_t* b);
	//GORBN_DEF void gorbn_gcd_ext(gorbn_t* r, gorbn_t* a, gorbn_t* b, gorbn_t* x, gorbn_t* y);

	GORBN_DEF void gorbn_mod(gorbn_t* r, gorbn_t* a, int a_digit_count, gorbn_t* m);

	GORBN_DEF void gorbn_inv_mod(gorbn_t* r, gorbn_t *a, gorbn_t* m); /* r = (a ^ -1) mod m */
	//GORBN_DEF void gorbn_mul_inv_mod(gorbn_t* r, gorbn_t* a, gorbn_t* b, gorbn_t* m); /* r = a * (b ^ (-1)) mod m */
	GORBN_DEF void gorbn_mod_pow2(gorbn_t* r, gorbn_t* a, int k);
	//GORBN_DEF void gorbn_mod_word(gorbn_t* r, gorbn_t* n, gorbn_t w);

	GORBN_DEF void gorbn_sub_mod(gorbn_t* r, gorbn_t* a, gorbn_t* b, gorbn_t* m);
	GORBN_DEF void gorbn_add_mod(gorbn_t* r, gorbn_t* a, gorbn_t* b, gorbn_t* m);
	GORBN_DEF void gorbn_mul_mod(gorbn_t* r, gorbn_t* a, gorbn_t* b, gorbn_t* m);

	/* Bitwise operations: */
	GORBN_DEF void gorbn_and(gorbn_t* r, gorbn_t* a, gorbn_t* b); /* r = a & b */
	GORBN_DEF void gorbn_or(gorbn_t* r, gorbn_t* a, gorbn_t* b); /* r = a | b */
	GORBN_DEF void gorbn_xor(gorbn_t* r, gorbn_t* a, gorbn_t* b); /* r = a ^ b */
	GORBN_DEF void gorbn_lshift(gorbn_t* r, gorbn_t* a, int nbits); /* r = a << nbits */
	GORBN_DEF void gorbn_rshift(gorbn_t* r, gorbn_t* b, int nbits); /* r = a >> nbits */

	/* Special operators and comparison */
	GORBN_DEF int gorbn_cmp(gorbn_t* a, gorbn_t* b);
	GORBN_DEF int gorbn_cmp_word(gorbn_t* a, gorbn_t w);
	GORBN_DEF int gorbn_cmp_mod(gorbn_t* a, gorbn_t* b);

	GORBN_DEF int gorbn_is_zero(gorbn_t* num);
	GORBN_DEF int gorbn_is_one(gorbn_t* num);

	/* Eliptic curve algorithms */
	GORBN_DEF void gorec_load_stb128(gorec_curve* crv);

	GORBN_DEF void gorec_pt_mul(
		gorec_point* p_result,
		gorec_point *p_point,
		gorbn_t *p_scalar,
		gorec_curve* crv);

	GORBN_DEF void gorec_pt_mul_monty(
		gorec_point* p_result,
		gorec_point *p_point,
		gorbn_t *p_scalar,
		gorec_curve* crv);

	GORBN_DEF void gorec_pt_mul_wnaf_jacobian(
		gorec_point* p_result,
		gorec_point *p_point,
		gorbn_t *p_scalar,
		gorec_curve* crv);

#ifdef __cplusplus
}
#endif

#endif

#if defined(GOR_BIGNUM_IMPLEMENTATION) && !defined(GOR_BIGNUM_IMPLEMENTATION_DONE)
#define GOR_BIGNUM_IMPLEMENTATION_DONE

void _gorbn_mem_copy(void* to, void* from, size_t byte_count) {
	uint8_t* _to = (uint8_t*)to;
	uint8_t* _from = (uint8_t*)from;

	size_t i = 0;

	for (; i < byte_count; i++) {
		*_to++ = *_from++;
	}
}

void _gorbn_zero_number(gorbn_t* num, int digit_count) {
	int i;

	for (i = 0; i < digit_count; i++) {
		num[i] = 0;
	}
}

static int _gorbn_get_ndigits(gorbn_t* a, int digit_count_alloc) {
	int result = 0;

	for (int i = digit_count_alloc - 1; i >= 0; i--) {
		if (a[i] != 0) {
			result = i + 1;
			break;
		}
	}

	return(result);
}

static int _gorbn_get_nbits(gorbn_t* a, int digit_count_alloc) {
	int i;
	int digit;

	int num_digits = _gorbn_get_ndigits(a, digit_count_alloc);
	if (num_digits == 0) {
		return(0);
	}

	digit = a[num_digits - 1];
	for (i = 0; digit; ++i) {
		digit >>= 1;
	}

	return((num_digits - 1) * GORBN_SZWORD_BITS + i);
}

static int _gorbn_testbit(gorbn_t* a, int bitnum) {

	int Result = (a[bitnum / GORBN_SZWORD_BITS] & (1 << (bitnum % GORBN_SZWORD_BITS)));

	return(Result);
}


static int _gorbn_is_zero_internal(gorbn_t* num, int num_digit_count) {
	int i, r;

	r = 1;

	for (i = 0; i < num_digit_count; i++) {
		if (num[i]) {
			r = 0;
			break;
		}
	}

	return(r);
}

int gorbn_is_zero(gorbn_t* num) {
	return(_gorbn_is_zero_internal(num, GORBN_SZARR));
}

static int _gorbn_is_one_internal(gorbn_t* num, int num_digit_count) {
	int i;

	if (num[0] != 1) {
		return(0);
	}

	for (i = 1; i < num_digit_count; i++) {
		if (num[i]) {
			return(0);
		}
	}

	return(1);
}

int gorbn_is_one(gorbn_t* num) {
	return(_gorbn_is_one_internal(num, GORBN_SZARR));
}

void gorbn_init(gorbn_t* n, int n_digit_count){
	int i;

	for(i = 0; i < n_digit_count; i++){
		n[i] = 0;
	}
}

static void gorbn_copy_internal(gorbn_t* dst, gorbn_t* src, int num_digit_count) {
	int i;

	for (i = 0; i < num_digit_count; i++) {
		dst[i] = src[i];
	}
}

void gorbn_copy(gorbn_t* dst, gorbn_t* src){
	gorbn_copy_internal(dst, src, GORBN_SZARR);
}


void gorbn_from_uint(gorbn_t* n, gorbn_utmp_t i){
	gorbn_init(n, GORBN_SZARR);

#if (GORBN_SZWORD == 1)
	n[0] = (i & 0x000000ff);
	n[1] = (i & 0x0000ff00) >> 8;
	n[2] = (i & 0x00ff0000) >> 16;
	n[3] = (i & 0xff000000) >> 24;
#elif (GORBN_SZWORD == 2)
	n[0] = i;
	n[1] = i >> 16;
#elif (GORBN_SZWORD == 4)
	n->number[0] = i;
	n->number[1] = i >> 32;
#endif
}

void gorbn_from_int(gorbn_t* n, gorbn_stmp_t i){
	gorbn_init(n, GORBN_SZARR);

	i = GORBN_ABS(i);
	
#if (GORBN_SZWORD == 1)
	n[0] = (i & 0x000000ff);
	n[1] = (i & 0x0000ff00) >> 8;
	n[2] = (i & 0x00ff0000) >> 16;
	n[3] = (i & 0xff000000) >> 24;

#elif (GORBN_SZWORD == 2)
	n[0] = i;
	n[1] = i >> 16;
#elif (GORBN_SZWORD == 4)
	n[0] = i;
	n[1] = i >> 32;
#endif
}

void gorbn_from_data(gorbn_t* n, void* data, uint32_t data_size){
	int i;

	unsigned char* at = (unsigned char*)data;
	unsigned char* to = (unsigned char*)n;

	gorbn_init(n, GORBN_SZARR);

	for(i = 0; i < data_size; i++){
		*to++ = *at++;
	}
}

void gorbn_to_data(void* data, uint32_t* written_datasize, uint32_t data_size, gorbn_t* n){
	int i;

	gorbn_t* at = n;
	gorbn_t* to = (gorbn_t*)data;

	for(i = 0; i < GORBN_SZARR; i++){
		*to++ = *at++;
	}

	if (written_datasize) {
		*written_datasize = i * GORBN_SZWORD;
	}
}

//static inline int _gorbn_min_i32(int a, int b) {
//	int res = GORBN_MIN(a, b);
//
//	return(res);
//}
//
//static inline int _gorbn_max_i32(int a, int b) {
//	int res = GORBN_MAX(a, b);
//
//	return(res);
//}

#define GORBN_EVEN(bn) (!(bn[0] & 1))
static inline int _gorbn_is_even(gorbn_t* a) {
	int is_even = ((a[0] & 1) == 0);

	return(is_even);
}

void gorbn_lshift_one_bit(gorbn_t* a) {
	int i;

	for (i = GORBN_SZARR - 1; i > 0; --i) {
		a[i] = (a[i] << 1) | (a[i - 1] >> GORBN_SZWORD_BITS_MINUS_ONE);
	}
	a[0] <<= 1;
}

void gorbn_rshift_one_bit(gorbn_t* a) {

	int i;
	for (i = 0; i < (GORBN_SZARR - 1); i++) {
		a[i] = (a[i] >> 1) | (a[i + 1] << GORBN_SZWORD_BITS_MINUS_ONE);
	}
	a[GORBN_SZARR - 1] >>= 1;
}

void gorbn_lshift_words(gorbn_t* a, int nwords) {
	int i;
	if (nwords > 0) {
		for (i = (GORBN_SZARR - 1); i >= nwords; i--) {
			a[i] = a[i - nwords];
		}

		for (; i >= 0; --i) {
			a[i] = 0;
		}
	}
}

void gorbn_rshift_words(gorbn_t* a, int nwords) {
	int i;
	if (nwords > 0) {
		for (i = 0; i < nwords; i++) {
			a[i] = a[i + nwords];
		}

		for (; i < GORBN_SZARR; i++) {
			a[i] = 0;
		}
	}
}

//NOTE(dima): Computes r = a + b, returning carry.
int gorbn_add(gorbn_t* r, gorbn_t* a, gorbn_t* b) {
	int i;
	gorbn_t carry = 0;

	for (i = 0; i < GORBN_SZARR; i++) {
#if 1
		gorbn_utmp_t sum = a[i] + b[i] + carry;
		carry = (sum > GORBN_MAX_VAL);
		r[i] = (sum & GORBN_MAX_VAL);
#else
		gorbn_t sum = a[i] + b[i] + carry;
		if (sum != a[i]) {
			carry = (sum < a[i]);
		}
		r[i] = sum;
#endif
	}

	return(carry);
}

int gorbn_sub(gorbn_t* r, gorbn_t* a, gorbn_t* b) {
	int i;
	int borrow = 0;
	gorbn_utmp_t res;

	for (i = 0; i < GORBN_SZARR; i++) {
		gorbn_utmp_t tmp1 = (gorbn_utmp_t)a[i] + ((gorbn_utmp_t)GORBN_MAX_VAL + 1);
		gorbn_utmp_t tmp2 = (gorbn_utmp_t)b[i] + borrow;
		res = (tmp1 - tmp2);
		r[i] = (gorbn_t)(res & GORBN_MAX_VAL);
		borrow = (res <= GORBN_MAX_VAL);
	}

	return(borrow);
}

void gorbn_mul_word(gorbn_t* r, gorbn_t* a, gorbn_t w) {
	_gorbn_zero_number(r, GORBN_SZARR + 1);

	gorbn_utmp_t uv;
	gorbn_utmp_t c = 0;
	int j;

	if (w == 0) {
		return;
	}

	for (j = 0; j < GORBN_SZARR; j++) {
		uv = (gorbn_utmp_t)r[j] +
			(gorbn_utmp_t)a[j] * (gorbn_utmp_t)w + c;
		r[j] = uv & GORBN_MAX_VAL;
		c = (uv >> GORBN_SZWORD_BITS) & GORBN_MAX_VAL;
	}

	r[GORBN_SZARR] = c;
}

void gorbn_mul(gorbn_t* r, gorbn_t* a, gorbn_t* b) {
	int i, j;
	int a_ndigits;
	int b_ndigits;

	_gorbn_zero_number(r, GORBN_SZARR * 2);

	a_ndigits = _gorbn_get_ndigits(a, GORBN_SZARR);
	b_ndigits = _gorbn_get_ndigits(b, GORBN_SZARR);

	for (i = 0; i < b_ndigits; i++) {
		gorbn_utmp_t uv;
		gorbn_utmp_t c = 0;

		for (j = 0; j < a_ndigits; j++) {
			uv = (gorbn_utmp_t)r[i + j] + 
				(gorbn_utmp_t)a[j] * 
				(gorbn_utmp_t)b[i] + 
				c;
			r[i + j] = uv & GORBN_MAX_VAL;
			c = (uv >> GORBN_SZWORD_BITS) & GORBN_MAX_VAL;
		}

		r[i + a_ndigits] = c;
	}
}

void gorbn_sqr(gorbn_t* r, gorbn_t* a) {
	int i, j;
	int a_ndigits;
	int b_ndigits;

	gorbn_utmp_t carry = 0;
	gorbn_utmp_t carry1;

	_gorbn_zero_number(r, GORBN_SZARR * 2);

	a_ndigits = _gorbn_get_ndigits(a, GORBN_SZARR);

	for (i = 0; i < a_ndigits; i++) {
		for (j = i + 1; j < a_ndigits; j++) {
			gorbn_utmp_t tmp_mul = a[i] * a[j];
			tmp_mul += carry;
			tmp_mul += r[i + j];
			r[i + j] = (gorbn_t)tmp_mul;
			carry = (gorbn_t)(tmp_mul >> GORBN_SZWORD_BITS);
		}
		r[i + j] = carry;
		carry = 0;
	}

	for (i = 0; i < a_ndigits + a_ndigits; i++) {
		carry1 = r[i] >> (GORBN_SZWORD_BITS - 1);
		r[i] = (r[i] << 1) | carry;
		carry = carry1;
	}

	for (i = 0; i < a_ndigits; i++) {
		gorbn_utmp_t tmp_mul = a[i] * a[i];
		tmp_mul += carry;
		tmp_mul += r[i + i];
		r[i + i] = (gorbn_t)tmp_mul;
		tmp_mul >>= GORBN_SZWORD_BITS;
		tmp_mul += r[i + i + 1];
		r[i + i + 1] = (gorbn_t)tmp_mul;
		carry = (gorbn_t)(tmp_mul >> GORBN_SZWORD_BITS);
	}
}

void gorbn_mul_pow2(gorbn_t* r, gorbn_t* a, int k) {
	gorbn_lshift(r, a, k);
}

void gorbn_div_pow2(gorbn_t* r, gorbn_t* a, int k) {
	gorbn_rshift(r, a, k);
}

void gorbn_div_word(
	gorbn_t* q, 
	gorbn_t* r,
	gorbn_t* x,
	gorbn_t w)
{
	gorbn_t buf[GORBN_SZARR];
	gorbn_init(buf, GORBN_SZARR);

	int i;
	gorbn_t tmp_r = 0;
	gorbn_utmp_t divisor = 0;
	for (i = GORBN_SZARR - 1; i >= 0; i--) {
		divisor = tmp_r;
		divisor <<= GORBN_SZWORD_BITS;
		divisor |= x[i];
		buf[i] = (gorbn_t)(divisor / w);
		tmp_r = (gorbn_t)(divisor % w);
	}
	
	gorbn_copy(q, buf);
	*r = tmp_r;
}

//TODO: make correct sign when n is negative
void gorbn_mod_word(
	gorbn_t* r, 
	gorbn_t* n, 
	gorbn_t w) 
{
	int i;
	gorbn_t tmp_r = 0;
	gorbn_utmp_t divisor = 0;
	for (i = GORBN_SZARR - 1; i >= 0; i--) {
		divisor = tmp_r;
		divisor <<= GORBN_SZWORD_BITS;
		divisor |= n[i];
		tmp_r = (gorbn_t)(divisor % w);
	}

	*r = tmp_r;
}


/*
	NOTE(Dima):
		q - the quotient output param. Can be NULL.
		r - the remainder output param. Can be NULL.
		x - the divident input param.
		y - the divisor input param.
*/
void gorbn_div(
	gorbn_t* q,
	gorbn_t* r,
	gorbn_t* x, int x_digit_count_alloc,
	gorbn_t* y, int y_digit_count_alloc)
{
	gorbn_utmp_t c_pred, r_pred;

	gorbn_t a_norm[GORBN_SZARR * 4];
	gorbn_t b_norm[GORBN_SZARR * 4];
	gorbn_t q_buf[GORBN_SZARR * 2];
	gorbn_t r_buf[GORBN_SZARR * 2];

	gorbn_init(a_norm, GORBN_SZARR * 4);
	gorbn_init(b_norm, GORBN_SZARR * 4);

	gorbn_init(q_buf, GORBN_SZARR * 2);
	gorbn_init(r_buf, GORBN_SZARR * 2);

	gorbn_copy_internal(a_norm, x, x_digit_count_alloc);
	gorbn_copy_internal(b_norm, y, y_digit_count_alloc);

	int a_ndig = _gorbn_get_ndigits(x, x_digit_count_alloc);
	int b_ndig = _gorbn_get_ndigits(y, y_digit_count_alloc);

	int i, j;
	gorbn_utmp_t mod_bn = (gorbn_utmp_t)GORBN_MAX_VAL + 1;
	gorbn_utmp_t p = 0;
	gorbn_stmp_t carry = 0;
	gorbn_stmp_t t;

	int begin_cmp_res = gorbn_cmp(x, y);
	if (begin_cmp_res == GORBN_CMP_EQUAL) {
		//NOTE(DIMA): If divisor is equal to divident than return 1
		q_buf[0] = 1;
	}
	else if (b_ndig == 1) {
		//NOTE(DIMA): If divisor is small number (== 1 word)
		
		for (j = a_ndig - 1; j >= 0; j--) {
			q_buf[j] = (gorbn_t)((carry * mod_bn + x[j]) / y[0]);
			carry = (carry * mod_bn + x[j]) - q_buf[j] * y[0];
		}

		r_buf[0] = (gorbn_t)carry;
	}
	else {

		//int b_nbits = _gorbn_get_nbits(y, y_digit_count_alloc);
		//int norm_val = (GORBN_SZWORD_BITS - b_nbits & GORBN_SZWORD_BITS_MINUS_ONE) & GORBN_SZWORD_BITS_MINUS_ONE;
		//gorbn_mul_pow2(a_norm, x, norm_val);

		for (j = a_ndig - b_ndig; j >= 0; j--) {
			int j_plus_b_ndig = j + b_ndig;
			gorbn_utmp_t asd =
				((gorbn_utmp_t)a_norm[j_plus_b_ndig] * mod_bn) +
				(gorbn_utmp_t)a_norm[j_plus_b_ndig - 1];

			c_pred = asd / b_norm[b_ndig - 1];
			r_pred = asd - c_pred * b_norm[b_ndig - 1];

			do {
				p = 0;
				if (
					(c_pred >= mod_bn) ||
					((c_pred * (gorbn_utmp_t)b_norm[b_ndig - 2]) > (mod_bn * r_pred + (gorbn_utmp_t)a_norm[j + b_ndig - 2])))
				{
					c_pred--;
					r_pred += b_norm[b_ndig - 1];
					if (r_pred < mod_bn) {
						p = 1;
					}
				}
			} while (p);

			carry = 0;
			for (i = 0; i < b_ndig; i++) {
				p = c_pred * b_norm[i];
				t = a_norm[i + j] - carry - (p & GORBN_MAX_VAL);
				a_norm[i + j] = (gorbn_t)t;
				carry = (p >> GORBN_SZWORD_BITS) - (t >> GORBN_SZWORD_BITS);
			}

			t = a_norm[j + b_ndig] - carry;
			a_norm[j + b_ndig] = (gorbn_t)t;
			q_buf[j] = (gorbn_t)c_pred;

			if (t < 0) {
				q_buf[j]--;
				carry = 0;
				for (i = 0; i < b_ndig; i++) {
					t = a_norm[i + j] + b_norm[i] + carry;
					a_norm[i + j] = (gorbn_t)t;
					carry = t >> GORBN_SZWORD_BITS;
				}
				a_norm[j + b_ndig] = (gorbn_t)(a_norm[j + b_ndig] + carry);
			}
		}

		if (begin_cmp_res == GORBN_CMP_SMALLER) {
			int asdfg = 1;
		}

		gorbn_copy(r_buf, a_norm);
	}

	if (q) {
		gorbn_copy(q, q_buf);
	}

	if (r) {
		gorbn_copy(r, r_buf);
	}
}

#if 0
void gorbn_div1420(
	gorbn_t* q,
	gorbn_t* r,
	gorbn_t* x, int x_digit_count_alloc,
	gorbn_t* y, int y_digit_count_alloc)
{
	int i, j;

	gorbn_t tmp_x[GORBN_SZARR * 2];
	gorbn_t tmp_y[GORBN_SZARR * 2];

	gorbn_t tmp32[GORBN_SZARR];

	int n = _gorbn_get_ndigits(x, x_digit_count_alloc);
	int t = _gorbn_get_ndigits(y, y_digit_count_alloc);

	gorbn_init(q, GORBN_SZARR);
	gorbn_init(r, GORBN_SZARR);
	gorbn_init(tmp32, GORBN_SZARR);

	_gorbn_zero_number(tmp_x, GORBN_SZARR * 2);
	_gorbn_zero_number(tmp_y, GORBN_SZARR * 2);

	while(gorbn_cmp(tmp_x, ))
}
#endif

void gorbn_mod(
	gorbn_t* r, 
	gorbn_t* a, int a_digit_count_alloc, 
	gorbn_t* m) 
{
	gorbn_div(
		0, r, 
		a, a_digit_count_alloc, 
		m, GORBN_SZARR);
}

void gorbn_mod_pow2(gorbn_t* r, gorbn_t* a, int k) {
	gorbn_t r_buf[GORBN_SZARR];

	int i;
	int j;

	int nwords_masked = k / GORBN_SZWORD_BITS;
	int nbits_rest = k & GORBN_SZWORD_BITS_MINUS_ONE;

	gorbn_t last_word_mask = 0;

	gorbn_init(r_buf, GORBN_SZARR);

	for (i = 0; i < nwords_masked; i++) {
		r_buf[i] = a[i] & GORBN_MAX_VAL;
	}

	for (j = 0; j < nbits_rest; j++) {
		last_word_mask |= (1 << i);
	}
	
	r_buf[i] = a[i] & last_word_mask;

	gorbn_copy(r, r_buf);
}

void gorbn_add_mod(gorbn_t* r, gorbn_t* a, gorbn_t* b, gorbn_t* m) {
	int carry = gorbn_add(r, a, b);
	if (carry || gorbn_cmp(r, m) >= 0) {
		gorbn_sub(r, r, m);
	}
}

void gorbn_sub_mod(gorbn_t* r, gorbn_t* a, gorbn_t* b, gorbn_t* m) {
	int borrow = gorbn_sub(r, a, b);
	if (borrow) {
		gorbn_add(r, r, m);
	}
}

void gorbn_mul_mod(gorbn_t* r, gorbn_t* a, gorbn_t* b, gorbn_t* m) {
	gorbn_t mul_res[GORBN_SZARR * 2];
	gorbn_mul(mul_res, a, b);

	gorbn_div(0, r, mul_res, GORBN_SZARR * 2, m, GORBN_SZARR);
}

void gorbn_mulword_mod(gorbn_t* r, gorbn_t* a, gorbn_t w, gorbn_t* m) {
	gorbn_t mul_res[GORBN_SZARR + 1];
	gorbn_mul_word(mul_res, a, w);

	gorbn_div(0, r, mul_res, GORBN_SZARR + 1, m, GORBN_SZARR);
}

void gorbn_sqr_mod(gorbn_t* r, gorbn_t* a, gorbn_t* m) {
	gorbn_t mul_res[GORBN_SZARR * 2];
	//gorbn_mul(mul_res, a, a);
	gorbn_sqr(mul_res, a);

	gorbn_div(0, r, mul_res, GORBN_SZARR * 2, m, GORBN_SZARR);
}

/* Getting inverse by modulo*/
void gorbn_inv_mod(gorbn_t* result, gorbn_t *a, gorbn_t* m) {
	gorbn_t u[GORBN_SZARR]; 
	gorbn_t v[GORBN_SZARR];
	gorbn_t r[GORBN_SZARR];
	gorbn_t s[GORBN_SZARR];

	gorbn_copy(u, m);
	gorbn_copy(v, a);
	gorbn_init(r, GORBN_SZARR);
	gorbn_init(s, GORBN_SZARR);
	s[0] = 1;

	while (!gorbn_is_zero(v)) {
		if (GORBN_EVEN(u)) {
			if (GORBN_EVEN(r)) {
				gorbn_rshift_one_bit(u);
				gorbn_rshift_one_bit(r);
			}
			else {
				gorbn_rshift_one_bit(u);
				int carry = gorbn_add(r, r, m);
				gorbn_rshift_one_bit(r);
				if (carry) {
					r[GORBN_SZARR - 1] |= GORBN_HIGH_BIT_SET;
				}
			}
		}
		else if (GORBN_EVEN(v)) {
			if (GORBN_EVEN(s)) {
				gorbn_rshift_one_bit(v);
				gorbn_rshift_one_bit(s);
			}
			else {
				gorbn_rshift_one_bit(v);
				int carry = gorbn_add(s, s, m);
				gorbn_rshift_one_bit(s);
				if (carry) {
					s[GORBN_SZARR - 1] |= GORBN_HIGH_BIT_SET;
				}
			}
		}
		else {
			if (gorbn_cmp(u, v) > 0) {
				gorbn_sub(u, u, v);
				int borrow = gorbn_sub(r, r, s);
				if (borrow) {
					gorbn_add(r, r, m);
				}
			}
			else {
				gorbn_sub(v, v, u);
				int borrow = gorbn_sub(s, s, r);
				if (borrow) {
					gorbn_add(s, s, m);
				}
			}
		}
	}

	if (gorbn_cmp(r, m) > 0) {
		gorbn_sub(r, r, m);
	}

	gorbn_copy(result, r);
}

/* logical AND */
void gorbn_and(gorbn_t* r, gorbn_t* a, gorbn_t* b, int digit_count)
{
	int walk_count = digit_count;

	int i;
	for (i = 0; i < walk_count; i++) {
		r[i] = a[i] & b[i];
	}
}

/* logical OR */
void gorbn_or(gorbn_t* r, gorbn_t* a, gorbn_t* b, int digit_count) {
	int walk_count = digit_count;

	int i;
	for (i = 0; i < walk_count; i++) {
		r[i] = a[i] | b[i];
	}
}

/* logical XOR */
void gorbn_xor(gorbn_t* r, gorbn_t* a, gorbn_t* b, int digit_count) {
	int walk_count = digit_count;

	int i;
	for (i = 0; i < walk_count; i++) {
		r[i] = a[i] ^ b[i];
	}
}

/* Shifting big number to left with by nbits*/
void gorbn_lshift(gorbn_t* r, gorbn_t* a, int nbits) {
	int i;
	gorbn_copy(r, a);

	int words_count = nbits / GORBN_SZWORD_BITS;
	int bit_offset = nbits & GORBN_SZWORD_BITS_MINUS_ONE;

	gorbn_lshift_words(r, words_count);
	for (i = (GORBN_SZARR - 1); i > 0; --i) {
		r[i] = (r[i] << bit_offset) | (r[i - 1] >> (GORBN_SZWORD_BITS - bit_offset));
	}
	r[0] <<= bit_offset;
}

/* Shifting big number to right by nbits */
void gorbn_rshift(gorbn_t* r, gorbn_t* b, int nbits) {
	int i;
	gorbn_copy(r, b);
	
	int words_count = nbits / GORBN_SZWORD_BITS;
	int bit_offset = nbits & GORBN_SZWORD_BITS_MINUS_ONE;

	gorbn_rshift_words(r, words_count);
	for (i = 0; i < GORBN_SZARR - 1; i++) {
		r[i] = (r[i] >> bit_offset) | (r[i + 1] << (GORBN_SZWORD_BITS - bit_offset));
	}
	r[i] >>= bit_offset;
}

/* Comparing two big numbers internal function */
static int _gorbn_cmp_internal(gorbn_t* a, gorbn_t* b, int num_digits) {
	int i;

	for (i = num_digits - 1; i >= 0; --i) {
		if (a[i] > b[i])
		{
			return(GORBN_CMP_LARGER);
		}
		else if (a[i] < b[i])
		{
			return(GORBN_CMP_SMALLER);
		}
	}

	return(GORBN_CMP_EQUAL);
}

/* Comparing two big numbers */
int gorbn_cmp(
	gorbn_t* a, 
	gorbn_t* b)
{
	return(_gorbn_cmp_internal(a, b, GORBN_SZARR));
}

/* Comparing big number with word */
int gorbn_cmp_word(
	gorbn_t* a,
	gorbn_t w)
{
	int i;

	for (i = 1; i < GORBN_SZARR; i++) {
		if (a[i]) {
			return(GORBN_CMP_LARGER);
		}
	}

	if (a[i] > w) {
		return(GORBN_CMP_LARGER);
	}
	else if (a[i] < w) {
		return(GORBN_CMP_SMALLER);
	}
	else {
		return(GORBN_CMP_EQUAL);
	}
}

/* Loading standard belarussian parameters*/
void gorec_load_stb128(gorec_curve* crv) {
	unsigned char lwo_bign_std_curve128_p[32] = {
		0x43, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
		0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
		0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
		0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	};

	unsigned char lwo_bign_std_curve128_a[32] = {
		0x40, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
		0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
		0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
		0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	};

	unsigned char lwo_bign_std_curve128_b[32] = {
		0xF1, 0x03, 0x9C, 0xD6, 0x6B, 0x7D, 0x2E, 0xB2,
		0x53, 0x92, 0x8B, 0x97, 0x69, 0x50, 0xF5, 0x4C,
		0xBE, 0xFB, 0xD8, 0xE4, 0xAB, 0x3A, 0xC1, 0xD2,
		0xED, 0xA8, 0xF3, 0x15, 0x15, 0x6C, 0xCE, 0x77,
	};

	unsigned char lwo_bign_std_curve128_q[32] = {
		0x07, 0x66, 0x3D, 0x26, 0x99, 0xBF, 0x5A, 0x7E,
		0xFC, 0x4D, 0xFB, 0x0D, 0xD6, 0x8E, 0x5C, 0xD9,
		0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
		0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	};

	unsigned char lwo_bign_std_curve128_yG[32] = {
		0x93, 0x6A, 0x51, 0x04, 0x18, 0xCF, 0x29, 0x1E,
		0x52, 0xF6, 0x08, 0xC4, 0x66, 0x39, 0x91, 0x78,
		0x5D, 0x83, 0xD6, 0x51, 0xA3, 0xC9, 0xE4, 0x5C,
		0x9F, 0xD6, 0x16, 0xFB, 0x3C, 0xFC, 0xF7, 0x6B,
	};

	unsigned char lwo_bign_std_curve128_xG[32] = {
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	};

	gorbn_from_data(crv->a, (void*)lwo_bign_std_curve128_a, sizeof(lwo_bign_std_curve128_a));
	gorbn_from_data(crv->b, (void*)lwo_bign_std_curve128_b, sizeof(lwo_bign_std_curve128_b));
	gorbn_from_data(crv->p, (void*)lwo_bign_std_curve128_p, sizeof(lwo_bign_std_curve128_p));
	gorbn_from_data(crv->q, (void*)lwo_bign_std_curve128_q, sizeof(lwo_bign_std_curve128_q));

	gorbn_from_data(crv->g.x, (void*)lwo_bign_std_curve128_xG, sizeof(lwo_bign_std_curve128_xG));
	gorbn_from_data(crv->g.y, (void*)lwo_bign_std_curve128_yG, sizeof(lwo_bign_std_curve128_yG));
	gorbn_from_int(crv->g.z, 1);
	crv->g.is_inf = 0;
}

/* point clearing */
void gorec_pt_clear(gorec_point* p) {
	gorbn_init(p->x, GORBN_SZARR);
	gorbn_init(p->y, GORBN_SZARR);
	gorbn_from_int(p->z, 1);
	p->is_inf = 1;
}

/* point copying */
void gorec_pt_copy(gorec_point* r, gorec_point* p) {
	gorbn_copy(r->x, p->x);
	gorbn_copy(r->y, p->y);
	gorbn_copy(r->z, p->z);
	r->is_inf = p->is_inf;
}

/* Point addition in affine coordinates */
void gorec_pt_add(gorec_point* r, gorec_point* a, gorec_point* b, gorec_curve* crv) {
	if (a->is_inf) {
		gorec_pt_copy(r, b);
		return;
	}

	if (b->is_inf) {
		gorec_pt_copy(r, a);
		return;
	}

	gorbn_t lambda[GORBN_SZARR];
	gorbn_t tmp[GORBN_SZARR];

	gorbn_t res_x[GORBN_SZARR];
	gorbn_t res_y[GORBN_SZARR];

	gorbn_init(lambda, GORBN_SZARR);
	gorbn_init(tmp, GORBN_SZARR);

	/*lambda = (y2 - y1)/(x2 - x1)*/
	gorbn_sub_mod(lambda, b->y, a->y, crv->p);
	gorbn_sub_mod(tmp, b->x, a->x, crv->p);
	gorbn_inv_mod(tmp, tmp, crv->p);
	gorbn_mul_mod(lambda, lambda, tmp, crv->p);

	/*x3 = lambda*lambda - x1 - x2*/
	gorbn_sqr_mod(res_x, lambda, crv->p);
	gorbn_sub_mod(res_x, res_x, a->x, crv->p);
	gorbn_sub_mod(res_x, res_x, b->x, crv->p);

	/*y3 = lambda * (x1 - x3) - y1 */
	gorbn_sub_mod(res_y, a->x, res_x, crv->p);
	gorbn_mul_mod(res_y, lambda, res_y, crv->p);
	gorbn_sub_mod(res_y, res_y, a->y, crv->p);

	gorbn_copy(r->x, res_x);
	gorbn_copy(r->y, res_y);
}

/* Point doubling in affine coordinates */
void gorec_pt_double(gorec_point* r, gorec_point* a, gorec_curve* crv) {
	if (!a->is_inf) {
		gorbn_t res_x[GORBN_SZARR];
		gorbn_t res_y[GORBN_SZARR];
		
		gorbn_t lambda[GORBN_SZARR];
		gorbn_t tmp[GORBN_SZARR];

		gorbn_init(lambda, GORBN_SZARR);
		gorbn_init(tmp, GORBN_SZARR);

		/*lambda = (3x1x1 + a)/(2y1) */
		gorbn_sqr_mod(lambda, a->x, crv->p);
		tmp[0] = 3;
		gorbn_mul_mod(lambda, lambda, tmp, crv->p);
		gorbn_add_mod(lambda, lambda, crv->a, crv->p);
		tmp[0] = 2;
		gorbn_mul_mod(tmp, tmp, a->y, crv->p);
		gorbn_inv_mod(tmp, tmp, crv->p);
		gorbn_mul_mod(lambda, lambda, tmp, crv->p);

		/*x3 = lambda*lambda - x1 - x2*/
		gorbn_sqr_mod(res_x, lambda, crv->p);
		gorbn_sub_mod(res_x, res_x, a->x, crv->p);
		gorbn_sub_mod(res_x, res_x, a->x, crv->p);

		/*y3 = lambda * (x1 - x3) - y1 */
		gorbn_sub_mod(res_y, a->x, res_x, crv->p);
		gorbn_mul_mod(res_y, lambda, res_y, crv->p);
		gorbn_sub_mod(res_y, res_y, a->y, crv->p);

		gorbn_copy(r->x, res_x);
		gorbn_copy(r->y, res_y);
	}
}

/* Point doubling in Jacobian coordinates */
void gorec_pt_double_jacobian(gorec_point* r, gorec_point* a, gorec_curve* crv) {
	gorbn_t S[GORBN_SZARR];
	gorbn_t M[GORBN_SZARR];
	gorbn_t TMP[GORBN_SZARR];
	gorbn_t YSQ[GORBN_SZARR];

	gorec_point rp;
	gorec_pt_clear(&rp);

	if (a->is_inf) {
		return;
	}

	// S = 4*X*Y^2
	gorbn_sqr_mod(YSQ, a->y, crv->p);
	gorbn_mul_mod(S, a->x, YSQ, crv->p);
	gorbn_mulword_mod(S, S, 4, crv->p);

	// M = 3*X^2 + a*Z^4
	gorbn_sqr_mod(M, a->x, crv->p);
	gorbn_mulword_mod(M, M, 3, crv->p);
	gorbn_sqr_mod(TMP, a->z, crv->p);
	gorbn_sqr_mod(TMP, TMP, crv->p);
	gorbn_mul_mod(TMP, TMP, crv->a, crv->p);
	gorbn_add_mod(M, M, TMP, crv->p);

	// X' = M^2 - 2*S
	gorbn_sqr_mod(rp.x, M, crv->p);
	gorbn_mulword_mod(TMP, S, 2, crv->p);
	gorbn_sub_mod(rp.x, rp.x, TMP, crv->p);

	// Y' = M*(S - X') - 8 * Y ^ 4
	gorbn_sub_mod(rp.y, S, rp.x, crv->p);
	gorbn_mul_mod(rp.y, M, rp.y, crv->p);
	gorbn_sqr_mod(TMP, YSQ, crv->p);
	gorbn_mulword_mod(TMP, TMP, 8, crv->p);
	gorbn_sub_mod(rp.y, rp.y, TMP, crv->p);
	
	// Z' = 2*Y*Z
	gorbn_mul_mod(rp.z, a->y, a->z, crv->p);
	gorbn_mulword_mod(rp.z, rp.z, 2, crv->p);

	rp.is_inf = 0;
	gorec_pt_copy(r, &rp);
}

/*Point addition in Jacobian projective coordinates*/
void gorec_pt_add_jacobian(gorec_point* r, gorec_point* a, gorec_point* b, gorec_curve* crv){
	gorbn_t U1[GORBN_SZARR];
	gorbn_t U2[GORBN_SZARR];
	gorbn_t S1[GORBN_SZARR];
	gorbn_t S2[GORBN_SZARR];
	gorbn_t TMP[GORBN_SZARR];
	gorbn_t H[GORBN_SZARR];
	gorbn_t R[GORBN_SZARR];

	if (a->is_inf) {
		gorec_pt_copy(r, b);
		return;
	}

	if (b->is_inf) {
		gorec_pt_copy(r, a);
		return;
	}

	// U1 = X1*Z2^2
	// U2 = X2*Z1^2
	// S1 = Y1*Z2^3
	// S2 = Y2*Z1^3
	gorbn_sqr_mod(TMP, b->z, crv->p);
	gorbn_mul_mod(U1, a->x, TMP, crv->p);
	gorbn_mul_mod(S1, TMP, a->y, crv->p);
	gorbn_mul_mod(S1, S1, b->z, crv->p);
	gorbn_sqr_mod(TMP, a->z, crv->p);
	gorbn_mul_mod(U2, TMP, b->x, crv->p);
	gorbn_mul_mod(S2, TMP, b->y, crv->p);
	gorbn_mul_mod(S2, S2, a->z, crv->p);

	if (gorbn_cmp(U1, U2) == GORBN_CMP_EQUAL) {
		if (gorbn_cmp(S1, S2) != GORBN_CMP_EQUAL) {
			//NOTE(dima): Return POINT_AT_INFINITY
			int a = 1;
		}
		else {
			//NOTE(dima):
			return gorec_pt_double_jacobian(r, a, crv);
		}
	}

	// H = U2 - U1
	gorbn_sub_mod(H, U2, U1, crv->p);

	// R = S2 - S1
	gorbn_sub_mod(R, S2, S1, crv->p);

	// X3 = R^2 - H^3 - 2*U1*H^2
	gorbn_sqr_mod(r->x, R, crv->p);
	gorbn_sqr_mod(U2, H, crv->p);
	gorbn_mul_mod(U2, U2, H, crv->p);
	gorbn_sub_mod(r->x, r->x, U2, crv->p);
	gorbn_sqr_mod(TMP, H, crv->p);
	gorbn_mul_mod(TMP, TMP, U1, crv->p);
	gorbn_mulword_mod(TMP, TMP, 2, crv->p);
	gorbn_sub_mod(r->x, r->x, TMP, crv->p);

	// Y3 = R*(U1*H^2 - X3) - S1*H^3
	gorbn_sqr_mod(TMP, H, crv->p);
	gorbn_mul_mod(TMP, TMP, U1, crv->p);
	gorbn_sub_mod(TMP, TMP, r->x, crv->p);
	gorbn_mul_mod(TMP, TMP, R, crv->p);
	gorbn_mul_mod(U2, U2, S1, crv->p);
	gorbn_sub_mod(r->y, TMP, U2, crv->p);

	// Z3 = H*Z1*Z2
	gorbn_mul_mod(r->z, a->z, b->z, crv->p);
	gorbn_mul_mod(r->z, r->z, H, crv->p);
}

/*Point subtraction in Jacobian projective coordinates*/
void gorec_pt_sub_jacobian(gorec_point* r, gorec_point* a, gorec_point* b, gorec_curve* crv) {
	gorbn_t SaveY[GORBN_SZARR];

	//NOTE(dima): Save b->y for later reassigning
	gorbn_copy(SaveY, b->y);

	//NOTE(dima): 0 - b->y
	gorbn_init(b->y, GORBN_SZARR);

	gorbn_sub_mod(b->y, b->y, SaveY, crv->p);

	//NOTE(dima): Adding initial A and negated B
	gorec_pt_add_jacobian(r, a, b, crv);

	//NOTE(dima): Returning old saved b->y value
	gorbn_copy(b->y, SaveY);
}

//NOTE(dima): window width w should not be greater than 7 (<=7)

static void gorec_compute_naf(char* NAF, int* NAFLength, gorbn_t k[GORBN_SZARR], int w) {
	gorbn_t kk[GORBN_SZARR];
	gorbn_t temp[GORBN_SZARR];
	int i;

	gorbn_copy(kk, k);
	gorbn_init(temp, GORBN_SZARR);

	int two_pow_w = 1 << w;
	int half_pow_w = two_pow_w >> 1;

	i = 0;
	while (!gorbn_is_zero(kk)) {
		int ExitOnTheEndOfIteration = 0;
		if (gorbn_is_one(kk)) {
			ExitOnTheEndOfIteration = 1;
		}

		if (!GORBN_EVEN(kk)) {
			int word_mod_res = kk[0] & (two_pow_w - 1);

			if (word_mod_res >= half_pow_w) {
				NAF[i] = word_mod_res - two_pow_w;
			}
			else {
				NAF[i] = word_mod_res;
			}

			if (NAF[i] > 0) {
				gorbn_from_int(temp, NAF[i]);
				int borrow = gorbn_sub(kk, kk, temp);

				if (borrow) {
					ExitOnTheEndOfIteration = 1;
				}
			}
			else {
				gorbn_from_int(temp, -NAF[i]);
				gorbn_add(kk, kk, temp);

				if (ExitOnTheEndOfIteration) {
					ExitOnTheEndOfIteration = 0;
				}
			}
		}
		else {
			NAF[i] = 0;
		}

		i++;
		gorbn_rshift_one_bit(kk);

		if (ExitOnTheEndOfIteration) {
			break;
		}
	}

	*NAFLength = i;
}

#define GOREC_WINDOW_W 4
#define GOREC_PRECOMPUTE_ARRAYS_COUNT (1 << (GOREC_WINDOW_W - 2))
void gorec_pt_mul_wnaf_jacobian(
	gorec_point* p_result,
	gorec_point *p_point,
	gorbn_t *p_scalar,
	gorec_curve* crv) 
{
	int PrecomputeIndex;
	int i;

	//NOTE(dima): I think that this is the maximum possible size of this thing
	char NAF[GORBN_SZARR_BITS_TOTAL + 1];
	int NAFLength;

	int w = GOREC_WINDOW_W;
	gorec_point PrecomputePoints[GOREC_PRECOMPUTE_ARRAYS_COUNT];

	gorec_point result;
	gorbn_t temp_for_exit[GORBN_SZARR];

	//NOTE(dima): Step1 - Computing Non-Adjacent Form (NAF)
	gorec_compute_naf(NAF, &NAFLength, p_scalar, w);

	//NOTE(dima): Using p_result as temp variable
	gorec_pt_copy(&result, p_point);
	gorec_pt_copy(&PrecomputePoints[0], &result);
	//NOTE(dima): Step2 - Precomputing points
	for (PrecomputeIndex = 0;
		PrecomputeIndex < GOREC_PRECOMPUTE_ARRAYS_COUNT;
		PrecomputeIndex++)
	{
		if (PrecomputeIndex != 0) {
			//NOTE(dima): Incrementing 2 times by p_point to save odd'ness
			gorec_pt_add_jacobian(&result, &result, p_point, crv);
			gorec_pt_add_jacobian(&result, &result, p_point, crv);

			gorec_pt_copy(&PrecomputePoints[PrecomputeIndex], &result);
		}
	}

	gorec_pt_clear(&result);
	//NOTE(dima): Step 3 - Compute result using precomputed values
	for (i = NAFLength - 1; i >= 0; i--) {
		gorec_pt_double_jacobian(&result, &result, crv);
		if (NAF[i] != 0) {
			if (NAF[i] > 0) {
				gorec_pt_add_jacobian(&result, &result, &PrecomputePoints[NAF[i] >> 1], crv);
			}
			else {
				gorec_pt_sub_jacobian(&result, &result, &PrecomputePoints[(-NAF[i]) >> 1], crv);
			}
		}
	}

	//NOTE(dima): Exit from Jacobian coordinates
	gorbn_sqr_mod(temp_for_exit, result.z, crv->p);
	gorbn_inv_mod(temp_for_exit, temp_for_exit, crv->p);
	gorbn_mul_mod(result.x, temp_for_exit, result.x, crv->p);

	gorbn_sqr_mod(temp_for_exit, result.z, crv->p);
	gorbn_mul_mod(temp_for_exit, temp_for_exit, result.z, crv->p);
	gorbn_inv_mod(temp_for_exit, temp_for_exit, crv->p);
	gorbn_mul_mod(result.y, temp_for_exit, result.y, crv->p);

	gorbn_from_int(result.z, 1);

	gorec_pt_copy(p_result, &result);
}

/*
	Умножение точки эллиптической кривой на скаляр

	p_result - указатель на результат - точку эллиптической кривой
	p_point - указатель на точку, которую будем умножать
	p_scalar - указатель на скалял, на который будем умножать p_point
	crv - указатель на структуру с параметрами эллиптической кривой
*/

void gorec_pt_mul(
	gorec_point* p_result,
	gorec_point *p_point,
	gorbn_t *p_scalar,
	gorec_curve* crv)
{
	int i;
	int t = _gorbn_get_nbits(p_scalar, GORBN_SZARR);

	gorec_pt_clear(p_result);

	for (i = t - 1; i >= 0; i--) {
		gorec_pt_double(p_result, p_result, crv);

		if (_gorbn_testbit(p_scalar, i)) {
			gorec_pt_add(p_result, p_result, p_point, crv);
		}
	}
}

void gorec_pt_mul_jacobian(
	gorec_point* r,
	gorec_point* p,
	gorbn_t* s,
	gorec_curve* crv)
{
	int i;
	int t = _gorbn_get_nbits(s, GORBN_SZARR);

	gorec_point result;
	gorbn_t temp[GORBN_SZARR];
	gorec_pt_clear(&result);

	for (i = t - 1; i >= 0; i--) {
		gorec_pt_double_jacobian(&result, &result, crv);

		if (_gorbn_testbit(s, i)) {
			gorec_pt_add_jacobian(&result, &result, p, crv);
		}
	}

	//NOTE(dima): Exit from Jacobian coordinates
	gorbn_sqr_mod(temp, result.z, crv->p);
	gorbn_inv_mod(temp, temp, crv->p);
	gorbn_mul_mod(result.x, temp, result.x, crv->p);

	gorbn_sqr_mod(temp, result.z, crv->p);
	gorbn_mul_mod(temp, temp, result.z, crv->p);
	gorbn_inv_mod(temp, temp, crv->p);
	gorbn_mul_mod(result.y, temp, result.y, crv->p);

	gorbn_from_int(result.z, 1);

	gorec_pt_copy(r, &result);
}

void gorec_pt_mul_monty(
	gorec_point* p_result,
	gorec_point *p_point,
	gorbn_t *p_scalar,
	gorec_curve* crv)
{
	int t = _gorbn_get_nbits(p_scalar, GORBN_SZARR);

	gorbn_t X1[GORBN_SZARR];
	gorbn_t Z1[GORBN_SZARR];
	gorbn_t X2[GORBN_SZARR];
	gorbn_t Z2[GORBN_SZARR];
	gorbn_t T[GORBN_SZARR];

	gorbn_t TMP[GORBN_SZARR];
	gorbn_t TMP2[GORBN_SZARR];

	gorbn_init(X1, GORBN_SZARR);
	gorbn_init(X2, GORBN_SZARR);
	gorbn_init(Z1, GORBN_SZARR);
	gorbn_init(Z2, GORBN_SZARR);
	gorbn_init(T, GORBN_SZARR);
	gorbn_init(TMP, GORBN_SZARR);
	gorbn_init(TMP2, GORBN_SZARR);

	/*X1 <- x*/
	gorbn_copy(X1, p_point->x);
	/*Z1 <- 1*/
	gorbn_from_int(Z1, 1);
	/*X2 <- x^4 + b*/
	gorbn_sqr_mod(X2, p_point->x, crv->p);
	gorbn_sqr_mod(X2, X2, crv->p);
	gorbn_add_mod(X2, X2, crv->b, crv->p);
	/*Z2<-x^2*/
	//gorbn_copy(X2, p_point->x, GORBN_SZARR);
	gorbn_sqr_mod(Z2, p_point->x, crv->p);

	for (int i = t - 2; i >= 0; i--) {
		int bit_is_set = _gorbn_testbit(p_scalar, i);
		if (bit_is_set) {
			/*T<-Z1*/
			gorbn_copy(T, Z1);
			/*Z1 <- (X1Z2 + X2Z1)^2 */
			gorbn_mul_mod(Z1, X2, Z1, crv->p);
			gorbn_mul_mod(TMP, X1, Z2, crv->p);
			gorbn_add_mod(Z1, TMP, Z1, crv->p);
			gorbn_sqr_mod(Z1, Z1, crv->p);
			/*X1 <- xZ1 + X1X2TZ2*/
			gorbn_mul_mod(X1, X1, X2, crv->p);
			gorbn_mul_mod(X1, X1, T, crv->p);
			gorbn_mul_mod(X1, X1, Z2, crv->p);
			gorbn_mul_mod(TMP, p_point->x, Z1, crv->p);
			gorbn_add_mod(X1, X1, TMP, crv->p);
			/*T <- X2*/
			gorbn_copy(T, X2);
			/*X2 <- X2^4 + b*Z2^4 */
			gorbn_sqr_mod(X2, X2, crv->p);
			gorbn_sqr_mod(X2, X2, crv->p);
			gorbn_sqr_mod(TMP2, Z2, crv->p);
			gorbn_sqr_mod(TMP, TMP2, crv->p);
			gorbn_mul_mod(TMP, TMP, crv->b, crv->p);
			gorbn_add_mod(X2, X2, TMP, crv->p);
			/*Z2 <- (T^2)(Z2^2)*/
			gorbn_sqr_mod(TMP, T, crv->p);
			gorbn_mul_mod(Z2, TMP2, TMP, crv->p);
		}
		else {
			/*T <- Z2*/
			gorbn_copy(T, Z2);
			/*Z2 <- (X1Z2 + X2Z1)^2 */
			gorbn_mul_mod(Z2, X1, Z2, crv->p);
			gorbn_mul_mod(TMP, X2, Z1, crv->p);
			gorbn_add_mod(Z2, TMP, Z2, crv->p);
			gorbn_sqr_mod(Z2, Z2, crv->p);
			/*X2 <- xZ2 + X1X2Z1T*/
			gorbn_mul_mod(X2, X1, X2, crv->p);
			gorbn_mul_mod(X2, X2, Z1, crv->p);
			gorbn_mul_mod(X2, X2, T, crv->p);
			gorbn_mul_mod(TMP, Z2, p_point->x, crv->p);
			gorbn_add_mod(X2, X2, TMP, crv->p);
			/*T <- X1*/
			gorbn_copy(T, X1);
			/*X1 <- (X1^4) + (b * Z1^4) */
			gorbn_sqr_mod(X1, X1, crv->p);
			gorbn_sqr_mod(X1, X1, crv->p);
			gorbn_sqr_mod(TMP2, Z1, crv->p);
			gorbn_sqr_mod(TMP, TMP2, crv->p);
			gorbn_mul_mod(TMP, TMP, crv->b, crv->p);
			gorbn_add_mod(X1, X1, TMP, crv->p);
			/*Z1 <- T^2 * Z1^2 */
			gorbn_sqr_mod(TMP, T, crv->p);
			gorbn_mul_mod(Z1, TMP2, TMP, crv->p);
		}
	}

	/*x3 <- X1/Z1*/
	gorbn_inv_mod(TMP, Z1, crv->p);
	gorbn_mul_mod(p_result->x, X1, TMP, crv->p);

	/*y3 <- (x + X1/Z1)[(X1 + xZ1)(X2 + xZ2) + (XX + y)(Z1Z2)](xZ1Z2)^-1 + y */
	gorbn_add_mod(p_result->y, p_point->x, p_result->x, crv->p);
	gorbn_mul_mod(TMP2, Z1, Z2, crv->p); /*TMP2 = Z1 * Z2;*/

	gorbn_mul_mod(Z1, p_point->x, Z1, crv->p);/*Z1 no longer needed after this operation*/
	gorbn_add_mod(Z1, Z1, X1, crv->p);/*X1 no longer needed after this operation*/

	gorbn_mul_mod(Z2, p_point->x, Z2, crv->p);/*Z2 no longer needed after this operation*/
	gorbn_add_mod(Z2, Z2, X2, crv->p);/*X2 no longer needed after this operation*/

	gorbn_mul_mod(Z1, Z1, Z2, crv->p);

	gorbn_sqr_mod(TMP, p_point->x, crv->p);
	gorbn_add_mod(TMP, TMP, p_point->y, crv->p);
	gorbn_mul_mod(TMP, TMP, TMP2, crv->p);
	gorbn_add_mod(TMP, TMP, Z1, crv->p);

	gorbn_mul_mod(TMP2, TMP2, p_point->x, crv->p);
	gorbn_inv_mod(TMP2, TMP2, crv->p);
	gorbn_mul_mod(TMP, TMP, TMP2, crv->p);
	gorbn_mul_mod(p_result->y, p_result->y, TMP, crv->p);
	gorbn_add_mod(p_result->y, p_result->y, p_point->y, crv->p);
}

#endif