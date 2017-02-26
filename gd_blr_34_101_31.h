/*
ABOUT:
	IN DEVELOPMENT
	Implementation of Belarussian cryptological standart STB 34.101.31 of 2011 year.
	Description of the standart: http://apmi.bsu.by/assets/files/std/belt-spec27.pdf

LICENSE:
	This software is dual-licensed to the public domain and under the following
	license: you are granted a perpetual, irrevocable license to copy, modify,
	publish, and distribute this file as you see fit.

CREDITS:
	Written by Gorevoy Dmitry

TODO:
	1) Encryption
	2) Decryption
	3) Provide sUm examples

DONE:
	1) Chunk encryption
	2) Chunk decryption
	2) Hashing algorithm

HO TO USE:
	If you want to use this library do the folowing: 
	define GD_BLR_34_101_31_IMPLEMENTATION and include this library in one cpp
	file like this:

	#define GD_CRYPTOS_IMPLEMENTATION
	#include "gd_blr_34_101_31.h"

	if you want to use it in multiple cpp files you need to define 
	#define GD_BLR_34_101_31_STATIC

EXAMPLES:
	
*/

#ifndef GD_BLR_34_101_31_H
#define GD_BLR_34_101_31_H

#ifndef GDBLR31_DEF
#ifdef GD_BLR_34_101_31_STATIC
#define GDBLR31_DEF static
#else
#define GDBLR31_DEF extern
#endif
#endif

#include <stdint.h>

#define GD_BLR31_UNPACK_U32(word, str)					\
	*((str) + 3) = (uint8_t)((word) >> 24);				\
	*((str) + 2) = (uint8_t)((word) >> 16);				\
	*((str) + 1) = (uint8_t)((word) >> 8);				\
	*((str) + 0) = (uint8_t)((word));					

#define GD_BLR31_PACK_U32(str, word)					\
	*(word) =											\
		((uint32_t)(*((str) + 3) << 24))	|			\
		((uint32_t)(*((str) + 2) << 16)) 	|			\
		((uint32_t)(*((str) + 1) << 8)) 	|			\
		((uint32_t)(*((str) + 0)));


#if 1
#define GD_BLR31_ROTLEFT(word, amount)				\
	(((word) << (amount)) | ((word) >> ((sizeof(word) << 3) - (amount))))

#define GD_BLR31_ROTRIGHT(word, amount)				\
	(((word) >> (amount)) | ((word) << ((sizeof(word) << 3) - (amount))))
#else
#define GD_BLR31_ROTLEFT(word, amount)				\
	(((word) << (amount)) ^ (word >> ((sizeof(word) << 3) - (amount))))

#define GD_BLR31_ROTRIGHT(word, amount)				\
	(((word) >> (amount)) ^ (word << ((sizeof(word) << 3) - (amount))))
#endif

/*128 chunk macro*/
#define GD_BLR31_CHUNK128_SET_ZERO(chunk)			\
	((uint32_t*)(chunk))[0] = 0;					\
	((uint32_t*)(chunk))[1] = 0;					\
	((uint32_t*)(chunk))[2] = 0;					\
	((uint32_t*)(chunk))[3] = 0;					

#define GD_BLR31_CHUNK128_SET_ONE(chunk)			\
	((uint32_t*)(chunk))[0] = 0xFFFFFFFF;			\
	((uint32_t*)(chunk))[1] = 0xFFFFFFFF;			\
	((uint32_t*)(chunk))[2] = 0xFFFFFFFF;			\
	((uint32_t*)(chunk))[3] = 0xFFFFFFFF;					

#define GD_BLR31_CHUNK128_COPY(dest, src)							\
	((uint32_t*)(dest))[0] = ((const uint32_t*)(src))[0];			\
	((uint32_t*)(dest))[1] = ((const uint32_t*)(src))[1];			\
	((uint32_t*)(dest))[2] = ((const uint32_t*)(src))[2];			\
	((uint32_t*)(dest))[3] = ((const uint32_t*)(src))[3];			

#define GD_BLR31_CHUNK128_NEGATE(dest, src)							\
	((uint32_t*)(dest))[0] = ~((const uint32_t*)(src))[0]; 			\
	((uint32_t*)(dest))[1] = ~((const uint32_t*)(src))[1]; 			\
	((uint32_t*)(dest))[2] = ~((const uint32_t*)(src))[2]; 			\
	((uint32_t*)(dest))[3] = ~((const uint32_t*)(src))[3]; 			

#define GD_BLR31_CHUNK128_XOR(dest, src1, src2)													\
	((uint32_t*)(dest))[0] = ((const uint32_t*)(src1))[0] ^ ((const uint32_t*)(src2))[0];		\
	((uint32_t*)(dest))[1] = ((const uint32_t*)(src1))[1] ^ ((const uint32_t*)(src2))[1];		\
	((uint32_t*)(dest))[2] = ((const uint32_t*)(src1))[2] ^ ((const uint32_t*)(src2))[2];		\
	((uint32_t*)(dest))[3] = ((const uint32_t*)(src1))[3] ^ ((const uint32_t*)(src2))[3];		

#define GD_BLR31_CHUNK128_XOREQ(dest, src)							\
	((uint32_t*)(dest))[0] ^= ((const uint32_t*)(src))[0];			\
	((uint32_t*)(dest))[1] ^= ((const uint32_t*)(src))[1];			\
	((uint32_t*)(dest))[2] ^= ((const uint32_t*)(src))[2];			\
	((uint32_t*)(dest))[3] ^= ((const uint32_t*)(src))[3];			


/*256 chunk macro*/
#define GD_BLR31_CHUNK256_SET_ZERO(chunk)			\
	((uint32_t*)(chunk))[0] = 0;					\
	((uint32_t*)(chunk))[1] = 0;					\
	((uint32_t*)(chunk))[2] = 0;					\
	((uint32_t*)(chunk))[3] = 0;					\
	((uint32_t*)(chunk))[4] = 0;					\
	((uint32_t*)(chunk))[5] = 0;					\
	((uint32_t*)(chunk))[6] = 0;					\
	((uint32_t*)(chunk))[7] = 0;					

#define GD_BLR31_CHUNK256_SET_ONE(chunk)			\
	((uint32_t*)(chunk))[0] = 0xFFFFFFFF;			\
	((uint32_t*)(chunk))[1] = 0xFFFFFFFF;			\
	((uint32_t*)(chunk))[2] = 0xFFFFFFFF;			\
	((uint32_t*)(chunk))[3] = 0xFFFFFFFF;			\
	((uint32_t*)(chunk))[4] = 0xFFFFFFFF;			\
	((uint32_t*)(chunk))[5] = 0xFFFFFFFF;			\
	((uint32_t*)(chunk))[6] = 0xFFFFFFFF;			\
	((uint32_t*)(chunk))[7] = 0xFFFFFFFF;	

#define GD_BLR31_CHUNK256_COPY(dest, src)							\
	((uint32_t*)(dest))[0] = ((const uint32_t*)(src))[0];			\
	((uint32_t*)(dest))[1] = ((const uint32_t*)(src))[1];			\
	((uint32_t*)(dest))[2] = ((const uint32_t*)(src))[2];			\
	((uint32_t*)(dest))[3] = ((const uint32_t*)(src))[3];			\
	((uint32_t*)(dest))[4] = ((const uint32_t*)(src))[4];			\
	((uint32_t*)(dest))[5] = ((const uint32_t*)(src))[5];			\
	((uint32_t*)(dest))[6] = ((const uint32_t*)(src))[6];			\
	((uint32_t*)(dest))[7] = ((const uint32_t*)(src))[7];			

#define GD_BLR31_CHUNK256_NEGATE(dest, src)							\
	((uint32_t*)(dest))[0] = ~((const uint32_t*)(src))[0]; 			\
	((uint32_t*)(dest))[1] = ~((const uint32_t*)(src))[1]; 			\
	((uint32_t*)(dest))[2] = ~((const uint32_t*)(src))[2]; 			\
	((uint32_t*)(dest))[3] = ~((const uint32_t*)(src))[3]; 			\
	((uint32_t*)(dest))[4] = ~((const uint32_t*)(src))[4]; 			\
	((uint32_t*)(dest))[5] = ~((const uint32_t*)(src))[5]; 			\
	((uint32_t*)(dest))[6] = ~((const uint32_t*)(src))[6]; 			\
	((uint32_t*)(dest))[7] = ~((const uint32_t*)(src))[7]; 	

#define GD_BLR31_CHUNK256_XOR(dest, src1, src2)													\
	((uint32_t*)(dest))[0] = ((const uint32_t*)(src1))[0] ^ ((const uint32_t*)(src2))[0];		\
	((uint32_t*)(dest))[1] = ((const uint32_t*)(src1))[1] ^ ((const uint32_t*)(src2))[1];		\
	((uint32_t*)(dest))[2] = ((const uint32_t*)(src1))[2] ^ ((const uint32_t*)(src2))[2];		\
	((uint32_t*)(dest))[3] = ((const uint32_t*)(src1))[3] ^ ((const uint32_t*)(src2))[3];		\
	((uint32_t*)(dest))[4] = ((const uint32_t*)(src1))[4] ^ ((const uint32_t*)(src2))[4];		\
	((uint32_t*)(dest))[5] = ((const uint32_t*)(src1))[5] ^ ((const uint32_t*)(src2))[5];		\
	((uint32_t*)(dest))[6] = ((const uint32_t*)(src1))[6] ^ ((const uint32_t*)(src2))[6];		\
	((uint32_t*)(dest))[7] = ((const uint32_t*)(src1))[7] ^ ((const uint32_t*)(src2))[7];

#define GD_BLR31_CHUNK256_XOREQ(dest, src)							\
	((uint32_t*)(dest))[0] ^= ((const uint32_t*)(src))[0];			\
	((uint32_t*)(dest))[1] ^= ((const uint32_t*)(src))[1];			\
	((uint32_t*)(dest))[2] ^= ((const uint32_t*)(src))[2];			\
	((uint32_t*)(dest))[3] ^= ((const uint32_t*)(src))[3];			\
	((uint32_t*)(dest))[4] ^= ((const uint32_t*)(src))[4];			\
	((uint32_t*)(dest))[5] ^= ((const uint32_t*)(src))[5];			\
	((uint32_t*)(dest))[6] ^= ((const uint32_t*)(src))[6];			\
	((uint32_t*)(dest))[7] ^= ((const uint32_t*)(src))[7];


#define GD_BLR31_HASH_DIGEST_BYTE_SIZE 32

#ifdef __cplusplus
extern "C" {
#endif

GDBLR31_DEF void gd_blr31_hash256(const uint8_t* bytes, int bytes_count, uint8_t dest[32]);

#ifdef __cplusplus
}
#endif

#endif


#if defined(GD_BLR_34_101_31_IMPLEMENTATION) && !defined(GD_BLR_34_101_31_IMPLEMENTATION_DONE)
#define GD_BLR_34_101_31_IMPLEMENTATION_DONE

static const uint8_t H[] = {
	0xB1, 0x94, 0xBA, 0xC8, 0x0A, 0x08, 0xF5, 0x3B, 0x36, 0x6D, 0x00, 0x8E, 0x58, 0x4A, 0x5D, 0xE4,
	0x85, 0x04, 0xFA, 0x9D, 0x1B, 0xB6, 0xC7, 0xAC, 0x25, 0x2E, 0x72, 0xC2, 0x02, 0xFD, 0xCE, 0x0D,
	0x5B, 0xE3, 0xD6, 0x12, 0x17, 0xB9, 0x61, 0x81, 0xFE, 0x67, 0x86, 0xAD, 0x71, 0x6B, 0x89, 0x0B,
	0x5C, 0xB0, 0xC0, 0xFF, 0x33, 0xC3, 0x56, 0xB8, 0x35, 0xC4, 0x05, 0xAE, 0xD8, 0xE0, 0x7F, 0x99,
	0xE1, 0x2B, 0xDC, 0x1A, 0xE2, 0x82, 0x57, 0xEC, 0x70, 0x3F, 0xCC, 0xF0, 0x95, 0xEE, 0x8D, 0xF1,
	0xC1, 0xAB, 0x76, 0x38, 0x9F, 0xE6, 0x78, 0xCA, 0xF7, 0xC6, 0xF8, 0x60, 0xD5, 0xBB, 0x9C, 0x4F,
	0xF3, 0x3C, 0x65, 0x7B, 0x63, 0x7C, 0x30, 0x6A, 0xDD, 0x4E, 0xA7, 0x79, 0x9E, 0xB2, 0x3D, 0x31,
	0x3E, 0x98, 0xB5, 0x6E, 0x27, 0xD3, 0xBC, 0xCF, 0x59, 0x1E, 0x18, 0x1F, 0x4C, 0x5A, 0xB7, 0x93,
	0xE9, 0xDE, 0xE7, 0x2C, 0x8F, 0x0C, 0x0F, 0xA6, 0x2D, 0xDB, 0x49, 0xF4, 0x6F, 0x73, 0x96, 0x47,
	0x06, 0x07, 0x53, 0x16, 0xED, 0x24, 0x7A, 0x37, 0x39, 0xCB, 0xA3, 0x83, 0x03, 0xA9, 0x8B, 0xF6,
	0x92, 0xBD, 0x9B, 0x1C, 0xE5, 0xD1, 0x41, 0x01, 0x54, 0x45, 0xFB, 0xC9, 0x5E, 0x4D, 0x0E, 0xF2,
	0x68, 0x20, 0x80, 0xAA, 0x22, 0x7D, 0x64, 0x2F, 0x26, 0x87, 0xF9, 0x34, 0x90, 0x40, 0x55, 0x11,
	0xBE, 0x32, 0x97, 0x13, 0x43, 0xFC, 0x9A, 0x48, 0xA0, 0x2A, 0x88, 0x5F, 0x19, 0x4B, 0x09, 0xA1,
	0x7E, 0xCD, 0xA4, 0xD0, 0x15, 0x44, 0xAF, 0x8C, 0xA5, 0x84, 0x50, 0xBF, 0x66, 0xD2, 0xE8, 0x8A,
	0xA2, 0xD7, 0x46, 0x52, 0x42, 0xA8, 0xDF, 0xB3, 0x69, 0x74, 0xC5, 0x51, 0xEB, 0x23, 0x29, 0x21,
	0xD4, 0xEF, 0xD9, 0xB4, 0x3A, 0x62, 0x28, 0x75, 0x91, 0x14, 0x10, 0xEA, 0x77, 0x6C, 0xDA, 0x1D
};

static uint32_t gd_blr31_g(uint32_t u, int r){
	uint32_t res = 0;

	uint8_t u1 = (uint8_t)(u >> 24);
	uint8_t u2 = (uint8_t)(u >> 16);
	uint8_t u3 = (uint8_t)(u >> 8);
	uint8_t u4 = (uint8_t)(u);

#if 1
	uint32_t to_rot =
		((uint32_t)H[u1] << 24) |
		((uint32_t)H[u2] << 16) |
		((uint32_t)H[u3] << 8) 	|
		((uint32_t)H[u4]);
#else
	uint32_t to_rot =
		((uint32_t)H[u4] << 24) |
		((uint32_t)H[u3] << 16) |
		((uint32_t)H[u2] << 8) |
		((uint32_t)H[u1]);
#endif

	res = GD_BLR31_ROTLEFT(to_rot, r);

	return(res);
}

static void gd__blr31_swap32(uint32_t* a, uint32_t* b){
	uint32_t tmp = *a;
	*a = *b;
	*b = tmp;
}

static void gd_blr31_transform(uint8_t x[16], uint8_t theta_key[32], uint8_t dest[16]){
	int it = 0;
	uint32_t a, b, c, d, e;

	uint32_t thetas[8];
	uint32_t k[56];
	
	GD_BLR31_PACK_U32(theta_key, &thetas[0]);
	GD_BLR31_PACK_U32(theta_key + 4, &thetas[1]);
	GD_BLR31_PACK_U32(theta_key + 8, &thetas[2]);
	GD_BLR31_PACK_U32(theta_key + 12, &thetas[3]);
	GD_BLR31_PACK_U32(theta_key + 16, &thetas[4]);
	GD_BLR31_PACK_U32(theta_key + 20, &thetas[5]);
	GD_BLR31_PACK_U32(theta_key + 24, &thetas[6]);
	GD_BLR31_PACK_U32(theta_key + 28, &thetas[7]);

	GD_BLR31_CHUNK256_COPY(k, thetas);
	GD_BLR31_CHUNK256_COPY(k + 8, thetas);
	GD_BLR31_CHUNK256_COPY(k + 16, thetas);
	GD_BLR31_CHUNK256_COPY(k + 24, thetas);
	GD_BLR31_CHUNK256_COPY(k + 32, thetas);
	GD_BLR31_CHUNK256_COPY(k + 40, thetas);
	GD_BLR31_CHUNK256_COPY(k + 48, thetas);

	GD_BLR31_PACK_U32(x, &a);
	GD_BLR31_PACK_U32(x + 4, &b);
	GD_BLR31_PACK_U32(x + 8, &c);
	GD_BLR31_PACK_U32(x + 12, &d);	

	/*Original loop was designed to be from 1 to 8 included. 
		so I will use (i + 1) everywhere needed instead (i)*/
	for(it; it < 8; it++){

		b = b ^ gd_blr31_g(a + k[7 * (it + 1) - 6 - 1], 5);
		c = c ^ gd_blr31_g(d + k[7 * (it + 1) - 5 - 1], 21);
		a = a - gd_blr31_g(b + k[7 * (it + 1) - 4 - 1], 13);
		e = gd_blr31_g(b + c + k[7 * (it + 1) - 3 - 1], 21) ^ (it + 1);
		b = b + e;
		c = c - e;
		d = d + gd_blr31_g(c + k[7 * (it + 1) - 2 - 1], 13);
		b = b ^ gd_blr31_g(a + k[7 * (it + 1) - 1 - 1], 21);
		c = c ^ gd_blr31_g(d + k[7 * (it + 1) - 1], 5);

		gd__blr31_swap32(&a, &b);
		gd__blr31_swap32(&c, &d);
		gd__blr31_swap32(&b, &c);
	}

	GD_BLR31_UNPACK_U32(b, dest);
	GD_BLR31_UNPACK_U32(d, dest + 4);
	GD_BLR31_UNPACK_U32(a, dest + 8);
	GD_BLR31_UNPACK_U32(c, dest + 12);
}

static void gd_blr31_inv_transform(uint8_t x[16], uint8_t theta_key[32], uint8_t dest[16]){
	int it = 0;
	uint32_t a, b, c, d, e;

	uint32_t thetas[8];
	uint32_t k[56];
	
	GD_BLR31_PACK_U32(theta_key, &thetas[0]);
	GD_BLR31_PACK_U32(theta_key + 4, &thetas[1]);
	GD_BLR31_PACK_U32(theta_key + 8, &thetas[2]);
	GD_BLR31_PACK_U32(theta_key + 12, &thetas[3]);
	GD_BLR31_PACK_U32(theta_key + 16, &thetas[4]);
	GD_BLR31_PACK_U32(theta_key + 20, &thetas[5]);
	GD_BLR31_PACK_U32(theta_key + 24, &thetas[6]);
	GD_BLR31_PACK_U32(theta_key + 28, &thetas[7]);

	GD_BLR31_CHUNK256_COPY(k, thetas);
	GD_BLR31_CHUNK256_COPY(k + 8, thetas);
	GD_BLR31_CHUNK256_COPY(k + 16, thetas);
	GD_BLR31_CHUNK256_COPY(k + 24, thetas);
	GD_BLR31_CHUNK256_COPY(k + 32, thetas);
	GD_BLR31_CHUNK256_COPY(k + 40, thetas);
	GD_BLR31_CHUNK256_COPY(k + 48, thetas);

	GD_BLR31_PACK_U32(x, &a);
	GD_BLR31_PACK_U32(x + 4, &b);
	GD_BLR31_PACK_U32(x + 8, &c);
	GD_BLR31_PACK_U32(x + 12, &d);	

	for(it = 8; it >= 1; it--){
		
		b = b ^ gd_blr31_g(a + k[7 * it - 1], 5);
		c = c ^ gd_blr31_g(d + k[7 * it - 1 - 1], 21);
		a = a - gd_blr31_g(b + k[7 * it - 2 - 1], 13);
		e = gd_blr31_g(b + c + k[7 * it - 3 - 1], 21) ^ it;
		b = b + e;
		c = c - e;
		d = d + gd_blr31_g(c + k[7 * it - 4 - 1], 13);
		b = b ^ gd_blr31_g(a + k[7 * it - 5 - 1], 21);
		c = c ^ gd_blr31_g(d + k[7 * it - 6 - 1], 5);
		
		gd__blr31_swap32(&a, &b);
		gd__blr31_swap32(&c, &d);
		gd__blr31_swap32(&a, &d);
	}

	GD_BLR31_UNPACK_U32(c, dest);
	GD_BLR31_UNPACK_U32(a, dest + 4);
	GD_BLR31_UNPACK_U32(d, dest + 8);
	GD_BLR31_UNPACK_U32(b, dest + 12);
}

static void gd_blr31_hash_sigma1(uint8_t x[64], uint8_t dest[16]){
	/*x is u1 ## u2 ## u3 ## u4. u is 16 byte (128 bit) word*/

	uint8_t tran_res[16];
	uint8_t temp_res[16];
	uint8_t theta[32];

	/*Theta = u1 ## u2*/
	GD_BLR31_CHUNK128_COPY(theta, x);
	GD_BLR31_CHUNK128_COPY(theta + 16, x + 16);

	/*temp_res = u3 ^ u4*/
	GD_BLR31_CHUNK128_XOR(temp_res, x + 32, x + 48);

	gd_blr31_transform(temp_res, theta, tran_res);

	GD_BLR31_CHUNK128_XOREQ(tran_res, temp_res);

	/*Storing the result*/
	GD_BLR31_CHUNK128_COPY(dest, tran_res);
}

static void gd_blr31_hash_sigma2(uint8_t x[64], uint8_t dest[32]){
	/*x is u1 | u2 | u3 | u4. u is 16 byte (128 bit) word*/
	
	uint8_t res1[16];
	uint8_t res2[16];

	uint8_t temp_theta1[16];
	uint8_t temp_theta2[16];
	uint8_t temp_theta2_ones[16];
	uint8_t theta1[32];
	uint8_t theta2[32];

	/*theta1 computation*/
	gd_blr31_hash_sigma1(x, temp_theta1);
	GD_BLR31_CHUNK128_COPY(theta1, temp_theta1);
	GD_BLR31_CHUNK128_COPY(theta1 + 16, x + 48);

	/*theta2 computation*/
	GD_BLR31_CHUNK128_SET_ONE(temp_theta2_ones);
	gd_blr31_hash_sigma1(x, temp_theta2);
	GD_BLR31_CHUNK128_XOREQ(temp_theta2, temp_theta2_ones);
	GD_BLR31_CHUNK128_COPY(theta2, temp_theta2);
	GD_BLR31_CHUNK128_COPY(theta2 + 16, x + 32);

	/*Computations*/
	gd_blr31_transform(x, theta1, res1);
	gd_blr31_transform(x + 16, theta2, res2);	

	GD_BLR31_CHUNK128_XOREQ(res1, x);
	GD_BLR31_CHUNK128_XOREQ(res2, x + 16);

	/*Storing the result*/
	GD_BLR31_CHUNK128_COPY(dest, res1);
	GD_BLR31_CHUNK128_COPY(dest + 16, res2);
}

void gd_blr31_hash256(const uint8_t* bytes, int bytes_count, uint8_t dest[32]){

	int zeros_to_write = 0;
	int new_len;
	int it = 0;
	uint32_t x_long_mod_128 = 0;

	uint8_t* new_msg;

	uint8_t s[16];
	uint32_t hash[8];
	uint8_t hash_bytes[32];

	uint8_t temp16[16];
	uint8_t temp32[32];
	uint8_t temp64[64];

	/*Beginning*/
	new_len = bytes_count;

	if (bytes_count & 31){
		zeros_to_write = 32 - bytes_count % 32;
		new_len += zeros_to_write;
	}

	new_msg = (uint8_t*)malloc(new_len);

	for (it = 0; it < bytes_count; it++){
		new_msg[it] = bytes[it];
	}

	for (it = bytes_count; it < new_len; it++){
		new_msg[it] = 0;
	}

	/*First step*/
	GD_BLR31_CHUNK128_SET_ZERO(s);

	/*Second step*/
	hash[0] = 0xC8BA94B1;
	hash[1] = 0x3BF5080A;
	hash[2] = 0x8E006D36;
	hash[3] = 0xE45D4A58;
	hash[4] = 0x9DFA0485;
	hash[5] = 0xACC7B61B;
	hash[6] = 0xC2722E25;
	hash[7] = 0x0DCEFD02;
	
	GD_BLR31_UNPACK_U32(hash[0], hash_bytes);
	GD_BLR31_UNPACK_U32(hash[1], hash_bytes + 4);
	GD_BLR31_UNPACK_U32(hash[2], hash_bytes + 8);
	GD_BLR31_UNPACK_U32(hash[3], hash_bytes + 12);
	GD_BLR31_UNPACK_U32(hash[4], hash_bytes + 16);
	GD_BLR31_UNPACK_U32(hash[5], hash_bytes + 20);
	GD_BLR31_UNPACK_U32(hash[6], hash_bytes + 24);
	GD_BLR31_UNPACK_U32(hash[7], hash_bytes + 28);
	
	/*Third step*/
	for(it = 0; it < new_len; it += 32){
		/*temp64 = Xi ## hash*/
		GD_BLR31_CHUNK256_COPY(temp64, new_msg + it);
		GD_BLR31_CHUNK256_COPY(temp64 + 32, hash_bytes);
		
		/*Substep 1*/
		gd_blr31_hash_sigma1(temp64, temp16);
		GD_BLR31_CHUNK128_XOREQ(s, temp16);

		/*Substep 2*/
		gd_blr31_hash_sigma2(temp64, temp32);
		GD_BLR31_CHUNK256_COPY(hash_bytes, temp32);
	}

	free(new_msg);

	/*Fourth step*/
	x_long_mod_128 = (bytes_count << 3);

	GD_BLR31_CHUNK128_SET_ZERO(temp16);
	GD_BLR31_UNPACK_U32(x_long_mod_128, temp16);

	GD_BLR31_CHUNK128_COPY(temp64, temp16);
	GD_BLR31_CHUNK128_COPY(temp64 + 16, s);
	GD_BLR31_CHUNK256_COPY(temp64 + 32, hash_bytes);
	gd_blr31_hash_sigma2(temp64, temp32);

	/*Fifth step*/
	GD_BLR31_CHUNK256_COPY(dest, temp32);
}

#endif

