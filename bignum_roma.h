#ifndef BN_H_INCLUDED
#define BN_H_INCLUDED

#define BN_SZWORD 1
#define BN_arr_size 32

#define BN_t unsigned char
#define BN_utmp_t unsigned int
#define BN_stmp_t int
#define BN_MAX_VAL 0xFF
#define BN_SZWORD_BITS 8
#define BN_SZWORD_BITS_MINUS_ONE 7
#define BN_HIGH_BIT_SET 0x80


typedef struct EC_point {
	BN_t x[BN_arr_size];
	BN_t y[BN_arr_size];
	BN_t z[BN_arr_size];
	int is_inf;
} EC_point;

typedef struct EC_curve {
	BN_t a[BN_arr_size];
	BN_t b[BN_arr_size];

	BN_t p[BN_arr_size];
	BN_t q[BN_arr_size];

	EC_point g;
} EC_curve;

#define BN_ABS(val) (((val) >= 0) ? (val) : (-(val)))
#define BN_MAX(a, b) (((a) > (b)) ? (a) : (b))
#define BN_MIN(a, b) (((a) < (b)) ? (a) : (b))

#define BN_CLAMP(a, min, max) (BN_MAX(min, BN_MIN(max, a)))


#define BN_CMP_LARGER 1
#define BN_CMP_SMALLER -1
#define BN_CMP_EQUAL 0


#ifdef __cplusplus
extern "C" {
#endif

	void BN_init(BN_t* n, int n_digit_count);
	void BN_copy(BN_t* dst, BN_t* src, int num_digit_count);
	void BN_from_data(BN_t* n, void* data, uint32_t data_size);
	void BN_to_data(void* data, uint32_t data_size, BN_t* n);

	int BN_add(BN_t* r, BN_t* a, BN_t* b);
	int BN_sub(BN_t* r, BN_t* a, BN_t* b);
	void BN_mul(BN_t* r,BN_t* a,BN_t* b);

	void BN_div( BN_t* q, BN_t* r, BN_t* x, int x_digit_count_alloc, BN_t* y, int y_digit_count_alloc);
	void BN_div_word( BN_t* q, BN_t* r, BN_t* x, int x_digit_count_alloc, BN_t w);

	void BN_sqr(BN_t* r, BN_t* a);
	void BN_mul_word(BN_t* r, BN_t* a, BN_t w);
	void BN_pow(BN_t* r, BN_t* a, BN_t* b);
	void BN_mul_pow2(BN_t* r, BN_t* a, int k);
	void BN_div_pow2(BN_t* r, BN_t* a, int k);
	void BN_mod(BN_t* r, BN_t* a, int a_digit_count, BN_t* m);

	void BN_InvM(BN_t* r, BN_t *a, BN_t* m);
	void BN_SubM(BN_t* r, BN_t* a, BN_t* b, BN_t* m);
	void BN_AddM(BN_t* r, BN_t* a, BN_t* b, BN_t* m);
	void BN_MulM(BN_t* r, BN_t* a, BN_t* b, BN_t* m);
	void BN_and(BN_t* r, BN_t* a, BN_t* b);
	void BN_or(BN_t* r, BN_t* a, BN_t* b);
	void BN_xor(BN_t* r, BN_t* a, BN_t* b);
	void BN_lshift(BN_t* r, BN_t* a, int nbits);
	void BN_rshift(BN_t* r, BN_t* b, int nbits);
	int BN_cmp(BN_t* a, BN_t* b, int num_digits);

	int BN_cmp_mod(BN_t* a, BN_t* b);
	int BN_is_zero(BN_t* num, int num_digit_count);

	void EC_load_stb128(EC_curve* crv);
	void EC_pt_mul( EC_point* p_result, EC_point *p_point, BN_t *p_scalar, EC_curve* crv);
	void EC_pt_mul_jacobian( EC_point* r, EC_point* p, BN_t* s, EC_curve* crv);
	void EC_pt_mul_monty( EC_point* p_result, EC_point *p_point, BN_t *p_scalar, EC_curve* crv);


	void EC_pt_add(EC_point* r, EC_point* a, EC_point* b, EC_curve* crv);
	void EC_pt_double(EC_point* r, EC_point* a, EC_curve* crv);

#ifdef __cplusplus
}
#endif

#endif

