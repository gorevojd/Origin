#include "bignum_roma.h"

void _BN_mem_copy(void* to, void* from, size_t byte_count) {
	uint8_t* _to = (uint8_t*)to;
	uint8_t* _from = (uint8_t*)from;

	size_t i = 0;

	for (; i < byte_count; i++) {
		*_to++ = *_from++;
	}
}

void _BN_zero_number(BN_t* num, int digit_count) {
	int i;

	for (i = 0; i < digit_count; i++) {
		num[i] = 0;
	}
}

static int _BN_get_ndigits(BN_t* a, int digit_count_alloc) {
	int result = 0;

	for (int i = digit_count_alloc - 1; i >= 0; i--) {
		if (a[i] != 0) {
			result = i + 1;
			break;
		}
	}

	return(result);
}

static int _BN_get_nbits(BN_t* a, int digit_count_alloc) {
	int i;
	int digit;

	int num_digits = _BN_get_ndigits(a, digit_count_alloc);
	if (num_digits == 0) {
		return(0);
	}

	digit = a[num_digits - 1];
	for (i = 0; digit; ++i) {
		digit >>= 1;
	}

	return((num_digits - 1) * BN_SZWORD_BITS + i);
}

static int _BN_testbit(BN_t* a, int bitnum) {

	int Result = (a[bitnum / BN_SZWORD_BITS] & (1 << (bitnum % BN_SZWORD_BITS)));

	return(Result);
}


int BN_is_zero(BN_t* num, int num_digit_count) {
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

void BN_init(BN_t* n, int n_digit_count){
	int i;

	for(i = 0; i < n_digit_count; i++){
		n[i] = 0;
	}
}

void BN_copy(BN_t* dst, BN_t* src, int num_digit_count){
	int i;

	for(i = 0; i < num_digit_count; i++){
		dst[i] = src[i];
	}
}


void BN_from_uint(BN_t* n, BN_utmp_t i){

	BN_init(n, BN_arr_size);

#if (BN_SZWORD == 1)
	n[0] = (i & 0x000000ff);
	n[1] = (i & 0x0000ff00) >> 8;
	n[2] = (i & 0x00ff0000) >> 16;
	n[3] = (i & 0xff000000) >> 24;
#elif (BN_SZWORD == 2)
	n[0] = i;
	n[1] = i >> 16;
#elif (BN_SZWORD == 4)
	n->number[0] = i;
	n->number[1] = i >> 32;
#endif
}

void BN_from_int(BN_t* n, BN_stmp_t i){

	BN_init(n, BN_arr_size);

	i = BN_ABS(i);
	
#if (BN_SZWORD == 1)
	n[0] = (i & 0x000000ff);
	n[1] = (i & 0x0000ff00) >> 8;
	n[2] = (i & 0x00ff0000) >> 16;
	n[3] = (i & 0xff000000) >> 24;

#elif (BN_SZWORD == 2)
	n[0] = i;
	n[1] = i >> 16;
#elif (BN_SZWORD == 4)
	n[0] = i;
	n[1] = i >> 32;
#endif
}

void BN_from_data(BN_t* n, void* data, uint32_t data_size){

	int i;
	int res_ndig = 0;

	uint8_t* at = (uint8_t*)data;
	uint8_t* to = (uint8_t*)n;

	BN_init(n, BN_arr_size);

	for(i = 0; i < data_size; i++){
		*to++ = *at++;
	}
}

void BN_to_data(void* data, uint32_t* written_datasize, uint32_t data_size, BN_t* n){
	int i;

	BN_t* at = n;
	BN_t* to = (BN_t*)data;

	for(i = 0; i < BN_arr_size; i++){
		*to++ = *at++;
	}

	if (written_datasize) {
		*written_datasize = i * BN_SZWORD;
	}
}

static inline int _BN_min_i32(int a, int b) {
	int res = BN_MIN(a, b);

	return(res);
}

static inline int _BN_max_i32(int a, int b) {
	int res = BN_MAX(a, b);

	return(res);
}

#define BN_EVEN(bn) (!(bn[0] & 1))
static inline int _BN_is_even(BN_t* a) {
	int is_even = ((a[0] & 1) == 0);

	return(is_even);
}

void BN_lshift_one_bit(BN_t* a) {
	int i;

	for (i = BN_arr_size - 1; i > 0; --i) {
		a[i] = (a[i] << 1) | (a[i - 1] >> BN_SZWORD_BITS_MINUS_ONE);
	}
	a[0] <<= 1;
}

void BN_rshift_one_bit(BN_t* a) {

	int i;
	for (i = 0; i < (BN_arr_size - 1); i++) {
		a[i] = (a[i] >> 1) | (a[i + 1] << BN_SZWORD_BITS_MINUS_ONE);
	}
	a[BN_arr_size - 1] >>= 1;
}

void BN_lshift_words(BN_t* a, int nwords) {
	int i;
	if (nwords > 0) {
		for (i = (BN_arr_size - 1); i >= nwords; i--) {
			a[i] = a[i - nwords];
		}

		for (; i >= 0; --i) {
			a[i] = 0;
		}
	}
}

void BN_rshift_words(BN_t* a, int nwords) {
	int i;
	if (nwords > 0) {
		for (i = 0; i < nwords; i++) {
			a[i] = a[i + nwords];
		}

		for (; i < BN_arr_size; i++) {
			a[i] = 0;
		}
	}
}

int BN_add(BN_t* r, BN_t* a, BN_t* b) {
	int i;
	BN_t carry = 0;

	for (i = 0; i < BN_arr_size; i++) {
		BN_utmp_t sum = a[i] + b[i] + carry;
		carry = (sum > BN_MAX_VAL);
		r[i] = (sum & BN_MAX_VAL);
	}

	return(carry);
}

int BN_sub(BN_t* r, BN_t* a, BN_t* b) {
	int i;
	int borrow = 0;
	BN_utmp_t res;

	for (i = 0; i < BN_arr_size; i++) {
		BN_utmp_t tmp1 = (BN_utmp_t)a[i] + ((BN_utmp_t)BN_MAX_VAL + 1);
		BN_utmp_t tmp2 = (BN_utmp_t)b[i] + borrow;
		res = (tmp1 - tmp2);
		r[i] = (BN_t)(res & BN_MAX_VAL);
		borrow = (res <= BN_MAX_VAL);
	}

	return(borrow);
}

void BN_mul_word(BN_t* r, BN_t* a, BN_t w) {
	_BN_zero_number(r, BN_arr_size + 1);

	BN_utmp_t uv;
	BN_utmp_t c = 0;
	int j;

	if (w == 0) {
		return;
	}

	for (j = 0; j < BN_arr_size; j++) {
		uv = (BN_utmp_t)r[j] +
			(BN_utmp_t)a[j] * (BN_utmp_t)w + c;
		r[j] = uv & BN_MAX_VAL;
		c = (uv >> BN_SZWORD_BITS) & BN_MAX_VAL;
	}

	r[BN_arr_size] = c;
}

void BN_mul(BN_t* r, BN_t* a, BN_t* b) {
	int i, j;
	int a_ndigits;
	int b_ndigits;

	_BN_zero_number(r, BN_arr_size * 2);

	a_ndigits = _BN_get_ndigits(a, BN_arr_size);
	b_ndigits = _BN_get_ndigits(b, BN_arr_size);

	for (i = 0; i < b_ndigits; i++) {
		BN_utmp_t uv;
		BN_utmp_t c = 0;

		for (j = 0; j < a_ndigits; j++) {
			uv = (BN_utmp_t)r[i + j] + 
				(BN_utmp_t)a[j] * 
				(BN_utmp_t)b[i] + 
				c;
			r[i + j] = uv & BN_MAX_VAL;
			c = (uv >> BN_SZWORD_BITS) & BN_MAX_VAL;
		}

		r[i + a_ndigits] = c;
	}
}

void BN_sqr(BN_t* r, BN_t* a) {
	int i, j;
	int a_ndigits;
	int b_ndigits;

	BN_utmp_t carry = 0;
	BN_utmp_t carry1;

	_BN_zero_number(r, BN_arr_size * 2);

	a_ndigits = _BN_get_ndigits(a, BN_arr_size);

	for (i = 0; i < a_ndigits; i++) {
		for (j = i + 1; j < a_ndigits; j++) {
			BN_utmp_t tmp_mul = a[i] * a[j];
			tmp_mul += carry;
			tmp_mul += r[i + j];
			r[i + j] = (BN_t)tmp_mul;
			carry = (BN_t)(tmp_mul >> BN_SZWORD_BITS);
		}
		r[i + j] = carry;
		carry = 0;
	}

	for (i = 0; i < a_ndigits + a_ndigits; i++) {
		carry1 = r[i] >> (BN_SZWORD_BITS - 1);
		r[i] = (r[i] << 1) | carry;
		carry = carry1;
	}

	for (i = 0; i < a_ndigits; i++) {
		BN_utmp_t tmp_mul = a[i] * a[i];
		tmp_mul += carry;
		tmp_mul += r[i + i];
		r[i + i] = (BN_t)tmp_mul;
		tmp_mul >>= BN_SZWORD_BITS;
		tmp_mul += r[i + i + 1];
		r[i + i + 1] = (BN_t)tmp_mul;
		carry = (BN_t)(tmp_mul >> BN_SZWORD_BITS);
	}
}

void BN_mul_pow2(BN_t* r, BN_t* a, int k) {
	BN_lshift(r, a, k);
}

void BN_div_pow2(BN_t* r, BN_t* a, int k) {
	BN_rshift(r, a, k);
}

void BN_div_word(
	BN_t* q, 
	BN_t* r,
	BN_t* x,
	BN_t w)
{
	BN_t buf[BN_arr_size];
	BN_init(buf, BN_arr_size);

	int i;
	BN_t tmp_r = 0;
	BN_utmp_t divisor = 0;
	for (i = BN_arr_size - 1; i >= 0; i--) {
		divisor = tmp_r;
		divisor <<= BN_SZWORD_BITS;
		divisor |= x[i];
		buf[i] = (BN_t)(divisor / w);
		tmp_r = (BN_t)(divisor % w);
	}
	
	BN_copy(q, buf, BN_arr_size);
	*r = tmp_r;
}

void BN_mod_word(
	BN_t* r, 
	BN_t* n, 
	BN_t w) 
{
	int i;
	BN_t tmp_r = 0;
	BN_utmp_t divisor = 0;
	for (i = BN_arr_size - 1; i >= 0; i--) {
		divisor = tmp_r;
		divisor <<= BN_SZWORD_BITS;
		divisor |= n[i];
		tmp_r = (BN_t)(divisor % w);
	}

	*r = tmp_r;
}

void BN_div(
	BN_t* q,
	BN_t* r,
	BN_t* x, int x_digit_count_alloc,
	BN_t* y, int y_digit_count_alloc)
{
	BN_t a_norm[BN_arr_size * 4];
	BN_t b_norm[BN_arr_size * 4];
	BN_t q_buf[BN_arr_size * 2];
	BN_t r_buf[BN_arr_size * 2];

	BN_init(a_norm, BN_arr_size * 4);
	BN_init(b_norm, BN_arr_size * 4);

	BN_init(q_buf, BN_arr_size * 2);
	BN_init(r_buf, BN_arr_size * 2);

	BN_copy(a_norm, x, x_digit_count_alloc);
	BN_copy(b_norm, y, x_digit_count_alloc);

	int a_ndig = _BN_get_ndigits(x, x_digit_count_alloc);
	int b_ndig = _BN_get_ndigits(y, y_digit_count_alloc);

	int i, j;
	BN_utmp_t mod_bn = (BN_utmp_t)BN_MAX_VAL + 1;
	BN_utmp_t p = 0;
	BN_stmp_t carry = 0;
	BN_stmp_t t;

	int begin_cmp_res = BN_cmp(x, y, BN_arr_size);
	if (begin_cmp_res == BN_CMP_EQUAL) {
		q_buf[0] = 1;
	}
	else if (b_ndig == 1) {

		for (j = a_ndig - 1; j >= 0; j--) {
			q_buf[j] = (BN_t)((carry * mod_bn + x[j]) / y[0]);
			carry = (carry * mod_bn + x[j]) - q_buf[j] * y[0];
		}

		r_buf[0] = (BN_t)carry;
	}
	else {
		for (j = a_ndig - b_ndig; j >= 0; j--) {
			BN_utmp_t c_pred, r_pred;
			BN_utmp_t asd =
				((BN_utmp_t)a_norm[j + b_ndig] * mod_bn) +
				(BN_utmp_t)a_norm[j + b_ndig - 1];

			c_pred = asd / b_norm[b_ndig - 1];
			r_pred = asd - c_pred * b_norm[b_ndig - 1];

			do {
				p = 0;
				if (
					(c_pred >= mod_bn) ||
					((c_pred * (BN_utmp_t)b_norm[b_ndig - 2]) > (mod_bn * r_pred + (BN_utmp_t)a_norm[j + b_ndig - 2])))
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
				t = a_norm[i + j] - carry - (p & BN_MAX_VAL);
				a_norm[i + j] = (BN_t)t;
				carry = (p >> BN_SZWORD_BITS) - (t >> BN_SZWORD_BITS);
			}

			t = a_norm[j + b_ndig] - carry;
			a_norm[j + b_ndig] = (BN_t)t;
			q_buf[j] = (BN_t)c_pred;

			if (t < 0) {
				q_buf[j]--;
				carry = 0;
				for (i = 0; i < b_ndig; i++) {
					t = a_norm[i + j] + b_norm[i] + carry;
					a_norm[i + j] = (BN_t)t;
					carry = t >> BN_SZWORD_BITS;
				}
				a_norm[j + b_ndig] = (BN_t)(a_norm[j + b_ndig] + carry);
			}
		}

		if (begin_cmp_res == BN_CMP_SMALLER) {
			int asdfg = 1;
		}

		BN_copy(r_buf, a_norm, BN_arr_size);
	}

	if (q) {
		BN_copy(q, q_buf, BN_arr_size);
	}

	if (r) {
		BN_copy(r, r_buf, BN_arr_size);
	}
}

void BN_mod(
	BN_t* r, 
	BN_t* a, int a_digit_count_alloc, 
	BN_t* m) 
{
	BN_div(
		0, r, 
		a, a_digit_count_alloc, 
		m, BN_arr_size);
}

void BN_AddM(BN_t* r, BN_t* a, BN_t* b, BN_t* m) {
	int carry = BN_add(r, a, b);
	if (carry || BN_cmp(r, m, BN_arr_size) >= 0) {
		BN_sub(r, r, m);
	}
}

void BN_SubM(BN_t* r, BN_t* a, BN_t* b, BN_t* m) {
	int borrow = BN_sub(r, a, b);
	if (borrow) {
		BN_add(r, r, m);
	}
}

void BN_MulM(BN_t* r, BN_t* a, BN_t* b, BN_t* m) {
	BN_t mul_res[BN_arr_size * 2];
	BN_mul(mul_res, a, b);

	BN_div(0, r, mul_res, BN_arr_size * 2, m, BN_arr_size);
}

void BN_SqrM(BN_t* r, BN_t* a, BN_t* m) {
	BN_t mul_res[BN_arr_size * 2];
	BN_mul(mul_res, a, a);

	BN_div(0, r, mul_res, BN_arr_size * 2, m, BN_arr_size);
}



void BN_InvM(BN_t* result, BN_t *a, BN_t* m) {
	BN_t u[BN_arr_size]; 
	BN_t v[BN_arr_size];
	BN_t r[BN_arr_size];
	BN_t s[BN_arr_size];

	BN_copy(u, m, BN_arr_size);
	BN_copy(v, a, BN_arr_size);
	BN_init(r, BN_arr_size);
	BN_init(s, BN_arr_size);
	s[0] = 1;

	while (!BN_is_zero(v, BN_arr_size)) {
		if (BN_EVEN(u)) {
			if (BN_EVEN(r)) {
				BN_rshift_one_bit(u);
				BN_rshift_one_bit(r);
			}
			else {
				BN_rshift_one_bit(u);
				int carry = BN_add(r, r, m);
				BN_rshift_one_bit(r);
				if (carry) {
					r[BN_arr_size - 1] |= BN_HIGH_BIT_SET;
				}
			}
		}
		else if (BN_EVEN(v)) {
			if (BN_EVEN(s)) {
				BN_rshift_one_bit(v);
				BN_rshift_one_bit(s);
			}
			else {
				BN_rshift_one_bit(v);
				int carry = BN_add(s, s, m);
				BN_rshift_one_bit(s);
				if (carry) {
					s[BN_arr_size - 1] |= BN_HIGH_BIT_SET;
				}
			}
		}
		else {
			if (BN_cmp(u, v, BN_arr_size) > 0) {
				BN_sub(u, u, v);
				int borrow = BN_sub(r, r, s);
				if (borrow) {
					BN_add(r, r, m);
				}
			}
			else {
				BN_sub(v, v, u);
				int borrow = BN_sub(s, s, r);
				if (borrow) {
					BN_add(s, s, m);
				}
			}
		}
	}

	if (BN_cmp(r, m, BN_arr_size) > 0) {
		BN_sub(r, r, m);
	}

	BN_copy(result, r, BN_arr_size);
}

void BN_lshift(BN_t* r, BN_t* a, int nbits) {
	int i;

	BN_copy(r, a, BN_arr_size);

	int words_count = nbits / BN_SZWORD_BITS;
	int bit_offset = nbits & BN_SZWORD_BITS_MINUS_ONE;

	BN_lshift_words(r, words_count);
	for (i = (BN_arr_size - 1); i > 0; --i) {
		r[i] = (r[i] << bit_offset) | (r[i - 1] >> (BN_SZWORD_BITS - bit_offset));
	}
	r[0] <<= bit_offset;
}

void BN_rshift(BN_t* r, BN_t* b, int nbits) {
	int i;

	BN_copy(r, b, BN_arr_size);
	
	int words_count = nbits / BN_SZWORD_BITS;
	int bit_offset = nbits & BN_SZWORD_BITS_MINUS_ONE;

	BN_rshift_words(r, words_count);
	for (i = 0; i < BN_arr_size - 1; i++) {
		r[i] = (r[i] >> bit_offset) | (r[i + 1] << (BN_SZWORD_BITS - bit_offset));
	}
	r[i] >>= bit_offset;
}

int BN_cmp(
	BN_t* a, 
	BN_t* b,
	int num_digits)
{
	int i;

	for(i = num_digits - 1; i >= 0; --i){
		if (a[i] > b[i])
		{
			return(BN_CMP_LARGER);
		}
		else if (a[i] < b[i])
		{
			return(BN_CMP_SMALLER);
		}
	}

	return(BN_CMP_EQUAL);
}

void EC_load_stb128(EC_curve* crv) {

	unsigned char bign_std_curve128_p[32] = {
		0x43, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
		0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
		0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
		0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	};

	unsigned char bign_std_curve128_a[32] = {
		0x40, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
		0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
		0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
		0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	};

	unsigned char bign_std_curve128_b[32] = {
		0xF1, 0x03, 0x9C, 0xD6, 0x6B, 0x7D, 0x2E, 0xB2,
		0x53, 0x92, 0x8B, 0x97, 0x69, 0x50, 0xF5, 0x4C,
		0xBE, 0xFB, 0xD8, 0xE4, 0xAB, 0x3A, 0xC1, 0xD2,
		0xED, 0xA8, 0xF3, 0x15, 0x15, 0x6C, 0xCE, 0x77,
	};

	unsigned char bign_std_curve128_q[32] = {
		0x07, 0x66, 0x3D, 0x26, 0x99, 0xBF, 0x5A, 0x7E,
		0xFC, 0x4D, 0xFB, 0x0D, 0xD6, 0x8E, 0x5C, 0xD9,
		0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
		0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	};

	unsigned char bign_std_curve128_yG[32] = {
		0x93, 0x6A, 0x51, 0x04, 0x18, 0xCF, 0x29, 0x1E,
		0x52, 0xF6, 0x08, 0xC4, 0x66, 0x39, 0x91, 0x78,
		0x5D, 0x83, 0xD6, 0x51, 0xA3, 0xC9, 0xE4, 0x5C,
		0x9F, 0xD6, 0x16, 0xFB, 0x3C, 0xFC, 0xF7, 0x6B,
	};

	unsigned char bign_std_curve128_xG[32] = {
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	};

	BN_from_data(crv->a, (void*)bign_std_curve128_a, sizeof(bign_std_curve128_a));
	BN_from_data(crv->b, (void*)bign_std_curve128_b, sizeof(bign_std_curve128_b));
	BN_from_data(crv->p, (void*)bign_std_curve128_p, sizeof(bign_std_curve128_p));
	BN_from_data(crv->q, (void*)bign_std_curve128_q, sizeof(bign_std_curve128_q));

	BN_from_data(crv->g.x, (void*)bign_std_curve128_xG, sizeof(bign_std_curve128_xG));
	BN_from_data(crv->g.y, (void*)bign_std_curve128_yG, sizeof(bign_std_curve128_yG));
	BN_from_int(crv->g.z, 1);
	crv->g.is_inf = 0;
}

void EC_pt_clear(EC_point* p) {
	BN_init(p->x, BN_arr_size);
	BN_init(p->y, BN_arr_size);
	BN_from_int(p->z, 1);
	p->is_inf = 1;
}

void EC_pt_copy(EC_point* r, EC_point* p) {
	BN_copy(r->x, p->x, BN_arr_size);
	BN_copy(r->y, p->y, BN_arr_size);
	BN_copy(r->z, p->z, BN_arr_size);
	r->is_inf = p->is_inf;
}

void EC_pt_add(EC_point* r, EC_point* a, EC_point* b, EC_curve* crv){
	if (a->is_inf) {
		EC_pt_copy(r, b);
		return;
	}

	if (b->is_inf) {
		EC_pt_copy(r, a);
		return;
	}

	BN_t lambda[BN_arr_size];
	BN_t tmp[BN_arr_size];

	BN_t res_x[BN_arr_size];
	BN_t res_y[BN_arr_size];

	BN_init(lambda, BN_arr_size);
	BN_init(tmp, BN_arr_size);

	/*lambda = (y2 - y1)/(x2 - x1)*/
	BN_SubM(lambda, b->y, a->y, crv->p);
	BN_SubM(tmp, b->x, a->x, crv->p);
	BN_InvM(tmp, tmp, crv->p);
	BN_MulM(lambda, lambda, tmp, crv->p);

	/*x3 = lambda*lambda - x1 - x2*/
	BN_SqrM(res_x, lambda, crv->p);
	BN_SubM(res_x, res_x, a->x, crv->p);
	BN_SubM(res_x, res_x, b->x, crv->p);

	/*y3 = lambda * (x1 - x3) - y1 */
	BN_SubM(res_y, a->x, res_x, crv->p);
	BN_MulM(res_y, lambda, res_y, crv->p);
	BN_SubM(res_y, res_y, a->y, crv->p);

	BN_copy(r->x, res_x, BN_arr_size);
	BN_copy(r->y, res_y, BN_arr_size);
}

void EC_pt_double(EC_point* r, EC_point* a, EC_curve* crv) {
	if (!a->is_inf) {
		BN_t res_x[BN_arr_size];
		BN_t res_y[BN_arr_size];
		
		BN_t lambda[BN_arr_size];
		BN_t tmp[BN_arr_size];

		BN_init(lambda, BN_arr_size);
		BN_init(tmp, BN_arr_size);

		BN_SqrM(lambda, a->x, crv->p);
		tmp[0] = 3;
		BN_MulM(lambda, lambda, tmp, crv->p);
		BN_AddM(lambda, lambda, crv->a, crv->p);
		tmp[0] = 2;
		BN_MulM(tmp, tmp, a->y, crv->p);
		BN_InvM(tmp, tmp, crv->p);
		BN_MulM(lambda, lambda, tmp, crv->p);

		BN_SqrM(res_x, lambda, crv->p);
		BN_SubM(res_x, res_x, a->x, crv->p);
		BN_SubM(res_x, res_x, a->x, crv->p);

		BN_SubM(res_y, a->x, res_x, crv->p);
		BN_MulM(res_y, lambda, res_y, crv->p);
		BN_SubM(res_y, res_y, a->y, crv->p);

		BN_copy(r->x, res_x, BN_arr_size);
		BN_copy(r->y, res_y, BN_arr_size);
	}
}

void EC_apply_final_z_jacobian(EC_point* pt, EC_curve* crv) {
	BN_t temp[BN_arr_size];

	BN_SqrM(temp, pt->z, crv->p);
	BN_InvM(temp, temp, crv->p);
	BN_MulM(pt->x, temp, pt->x, crv->p);

	BN_SqrM(temp, pt->z, crv->p);
	BN_MulM(temp, temp, pt->z, crv->p);
	BN_InvM(temp, temp, crv->p);
	BN_MulM(pt->y, temp, pt->y, crv->p);

	BN_from_int(pt->z, 1);
}

void EC_pt_mul(
	EC_point* p_result,
	EC_point *p_point,
	BN_t *p_scalar,
	EC_curve* crv)
{
	int i;
	int t = _BN_get_nbits(p_scalar, BN_arr_size);

	EC_point res;
	EC_pt_clear(&res);

	for (i = t - 1; i >= 0; i--) {
		EC_pt_double(&res, &res, crv);

		if (_BN_testbit(p_scalar, i)) {
			EC_pt_add(&res, &res, p_point, crv);
		}
	}

	EC_pt_copy(p_result, &res);
}