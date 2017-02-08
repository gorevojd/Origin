/*
LICENSE:
	This software is dual-licensed to the public domain and under the following
	license: you are granted a perpetual, irrevocable license to copy, modify,
	publish, and distribute this file as you see fit.

CREDITS:
	Written by Gorevoy Dmitry

TODO:
	Finish AES encryption

DONE:
	MD5 hash computation
	SHA 224, 256, 384, 512 hash computation

HO TO USE:
	If you want to use this library do the folowing: 
	define GD_CRYPTOS_IMPLEMENTATION and include this library in one cpp
	file like this:

	#define GD_CRYPTOS_IMPLEMENTATION
	#include "gd_cryptos.h"

	if you want to use it in multiple cpp files you need to define 
	#define GD_CRYPTOS_STATIC

EXAMPLES:
	MD5: 
		unsigned char Buffer[16];
		char StringToHash[] = "Hello";
		gd_hash_md5((const unsigned char*)StringToHash, strlen(StringToHash), Buffer);

		Result: Buffer now contains folowing bytes 8b1a9953 c4611296 a827abf8 c47804d7

	SHA 224:
		const char* Message1 = "Hello";
		unsigned char DigestSHA224[GDCR_SHA224_DIGEST_SIZE];
		gd_hash_sha224((const unsigned char*)Message1, strlen(Message1), DigestSHA224);
		Result: 4149da18aa8bfc2b1e382c6c26556d01a92c261b6436dad5e3be3fcc

	SHA 256
		const char* Message2 = "Hello";
		unsigned char DigestSHA256[GDCR_SHA256_DIGEST_SIZE];
		gd_hash_sha256((const unsigned char*)Message2, strlen(Message2), DigestSHA256);
		Result: 185f8db32271fe25f561a6fc938b2e264306ec304eda518007d1764826381969

	SHA 384
		const char* Message3 = "Hello";
		unsigned char DigestSHA384[GDCR_SHA384_DIGEST_SIZE];
		gd_hash_sha384((const unsigned char*)Message3, strlen(Message3), DigestSHA384);
		Result: 3519fe5ad2c596efe3e276a6f351b8fc0b03db861782490d45f7598ebd0ab5fd5520ed102f38c4a5ec834e98668035fc

	SHA 512
		const char* Message4 = "Hello";
		unsigned char DigestSHA512[GDCR_SHA512_DIGEST_SIZE];
		gd_hash_sha512((const unsigned char*)Message4, strlen(Message4), DigestSHA512);
		Result: 3615f80c9d293ed7402687f94b22d58e529b8cc7916f8fac7fddf7fbd5af4cf777d3d795a7a00a16bf7e7f3fb9561ee9baae480da9fe7a18769e71886b03f315
*/

#ifndef GD_CRYPTOS_HEADER

#include <stdlib.h>

#ifndef GDCR_DEF
#ifdef GD_CRYPTOS_STATIC
#define GDCR_DEF static
#else
#define GDCR_DEF extern
#endif
#endif

#ifndef GD_ARRAY_COUNT
#define GD_ARRAY_COUNT(Array) (sizeof(Array) / sizeof(Array[0]))
#endif

typedef unsigned long long gdcr_u64;
typedef unsigned int gdcr_u32;
typedef unsigned short gdcr_u16;
typedef unsigned char gdcr_u8;

/*AES Macroses*/
#define GDCR_AES_NB 4

/*MD5 Macroses*/
#define GDCR_WORD 32
#define GDCR_MASK_FF 0xFFFFFFFF

#define GDCR_MD5_S11 7
#define GDCR_MD5_S12 12
#define GDCR_MD5_S13 17
#define GDCR_MD5_S14 22
#define GDCR_MD5_S21 5
#define GDCR_MD5_S22 9
#define GDCR_MD5_S23 14
#define GDCR_MD5_S24 20
#define GDCR_MD5_S31 4
#define GDCR_MD5_S32 11
#define GDCR_MD5_S33 16
#define GDCR_MD5_S34 23
#define GDCR_MD5_S41 6
#define GDCR_MD5_S42 10
#define GDCR_MD5_S43 15
#define GDCR_MD5_S44 21

#define GDCR_MD5_F(B, C, D) (((B) & (C)) | ((~(B)) & (D))) 
#define GDCR_MD5_G(B, C, D) (((B) & (D)) | ((C) & (~(D))))
#define GDCR_MD5_H(B, C, D) ((B) ^ (C) ^ (D))
#define GDCR_MD5_I(B, C, D) ((C) ^ ((B) | (~(D))))

#define GDCR_ROTATE_LEFT_U32(Value, Shifts) (((Value) << (Shifts)) | (Value >> (32 - (Shifts))))

#define GDCR_MD5_FF(a, b, c, d, x, s, ac)	\
	(a) += GDCR_MD5_F((b), (c), (d)) + (x) + (gdcr_u32)(ac);	\
	(a) = GDCR_ROTATE_LEFT_U32((a), (s));	\
	(a) += (b);		

#define GDCR_MD5_GG(a, b, c, d, x, s, ac)	\
	(a) += GDCR_MD5_G((b), (c), (d)) + (x) + (gdcr_u32)(ac);	\
	(a) = GDCR_ROTATE_LEFT_U32((a), (s));	\
	(a) += (b);	

#define GDCR_MD5_HH(a, b, c, d, x, s, ac)	\
	(a) += GDCR_MD5_H((b), (c), (d)) + (x) + (gdcr_u32)(ac);	\
	(a) = GDCR_ROTATE_LEFT_U32((a), (s));	\
	(a) += (b);	

#define GDCR_MD5_II(a, b, c, d, x, s, ac)	\
	(a) += GDCR_MD5_I((b), (c), (d)) + (x) + (gdcr_u32)(ac);	\
	(a) = GDCR_ROTATE_LEFT_U32((a), (s));	\
	(a) += (b);	


/*SHA Macroses*/
#define GDCR_SHA_UNROLL_LOOPS

#define GDCR_SHA224_DIGEST_SIZE 28
#define GDCR_SHA256_DIGEST_SIZE 32
#define GDCR_SHA384_DIGEST_SIZE 48
#define GDCR_SHA512_DIGEST_SIZE 64

#define GDCR_SHA256_BLOCK_SIZE 64
#define GDCR_SHA512_BLOCK_SIZE 128
#define GDCR_SHA384_BLOCK_SIZE GDCR_SHA512_BLOCK_SIZE
#define GDCR_SHA224_BLOCK_SIZE GDCR_SHA256_BLOCK_SIZE

#define GDCR_SHIFT_RIGHT(x, n)    (x >> n)
#define GDCR_ROTATE_RIGHT(x, n)   ((x >> n) | (x << ((sizeof(x) << 3) - n)))
#define GDCR_ROTATE_LEFT(x, n)   ((x << n) | (x >> ((sizeof(x) << 3) - n)))

#define GDCR_SHA_CH(x, y, z)  ((x & y) ^ (~x & z))
#define GDCR_SHA_MAJ(x, y, z) ((x & y) ^ (x & z) ^ (y & z))

#define GDCR_SHA256_F1(x) (GDCR_ROTATE_RIGHT(x,  2) ^ GDCR_ROTATE_RIGHT(x, 13) ^ GDCR_ROTATE_RIGHT(x, 22))
#define GDCR_SHA256_F2(x) (GDCR_ROTATE_RIGHT(x,  6) ^ GDCR_ROTATE_RIGHT(x, 11) ^ GDCR_ROTATE_RIGHT(x, 25))
#define GDCR_SHA256_F3(x) (GDCR_ROTATE_RIGHT(x,  7) ^ GDCR_ROTATE_RIGHT(x, 18) ^ GDCR_SHIFT_RIGHT(x,  3))
#define GDCR_SHA256_F4(x) (GDCR_ROTATE_RIGHT(x, 17) ^ GDCR_ROTATE_RIGHT(x, 19) ^ GDCR_SHIFT_RIGHT(x, 10))

#define GDCR_SHA512_F1(x) (GDCR_ROTATE_RIGHT(x, 28) ^ GDCR_ROTATE_RIGHT(x, 34) ^ GDCR_ROTATE_RIGHT(x, 39))
#define GDCR_SHA512_F2(x) (GDCR_ROTATE_RIGHT(x, 14) ^ GDCR_ROTATE_RIGHT(x, 18) ^ GDCR_ROTATE_RIGHT(x, 41))
#define GDCR_SHA512_F3(x) (GDCR_ROTATE_RIGHT(x,  1) ^ GDCR_ROTATE_RIGHT(x,  8) ^ GDCR_SHIFT_RIGHT(x,  7))
#define GDCR_SHA512_F4(x) (GDCR_ROTATE_RIGHT(x, 19) ^ GDCR_ROTATE_RIGHT(x, 61) ^ GDCR_SHIFT_RIGHT(x,  6))

#define GDCR_UNPACK32(val, str)	\
{                                             \
    *((str) + 3) = (unsigned char) ((val)      );       \
    *((str) + 2) = (unsigned char) ((val) >>  8);       \
    *((str) + 1) = (unsigned char) ((val) >> 16);       \
    *((str) + 0) = (unsigned char) ((val) >> 24);       \
}

#define GDCR_PACK32(str, val)	\
{                                             \
    *(val) =   ((unsigned int) *((str) + 3)      )    \
           | ((unsigned int) *((str) + 2) <<  8)    \
           | ((unsigned int) *((str) + 1) << 16)    \
           | ((unsigned int) *((str) + 0) << 24);   \
}

#define GDCR_UNPACK64(val, str)                      \
{													    \
    *((str) + 7) = (unsigned char) ((val)      );       \
    *((str) + 6) = (unsigned char) ((val) >>  8);       \
    *((str) + 5) = (unsigned char) ((val) >> 16);       \
    *((str) + 4) = (unsigned char) ((val) >> 24);       \
    *((str) + 3) = (unsigned char) ((val) >> 32);       \
    *((str) + 2) = (unsigned char) ((val) >> 40);       \
    *((str) + 1) = (unsigned char) ((val) >> 48);       \
    *((str) + 0) = (unsigned char) ((val) >> 56);       \
}

#define GDCR_PACK64(str, val)                       \
{														  \
    *(val) =   ((unsigned long long) *((str) + 7))		  \
           | ((unsigned long long) *((str) + 6) <<  8)    \
           | ((unsigned long long) *((str) + 5) << 16)    \
           | ((unsigned long long) *((str) + 4) << 24)    \
           | ((unsigned long long) *((str) + 3) << 32)    \
           | ((unsigned long long) *((str) + 2) << 40)    \
           | ((unsigned long long) *((str) + 1) << 48)    \
           | ((unsigned long long) *((str) + 0) << 56);   \
}

/* Macros for unrolling loop in SHA*/
#define GDCR_SHA256_SCR(i)							\
{													\
    w[i] =  GDCR_SHA256_F4(w[i -  2]) + w[i -  7]	\
          + GDCR_SHA256_F3(w[i - 15]) + w[i - 16];	\
}

#define GDCR_SHA512_SCR(i)							\
{													\
    w[i] =  GDCR_SHA512_F4(w[i -  2]) + w[i -  7]	\
          + GDCR_SHA512_F3(w[i - 15]) + w[i - 16];	\
}

#define GDCR_SHA256_EXP(a, b, c, d, e, f, g, h, j)							\
{																			\
    t1 = wv[h] + GDCR_SHA256_F2(wv[e]) + GDCR_SHA_CH(wv[e], wv[f], wv[g])	\
         + gdcr__sha256_const_table[j] + w[j];                              \
    t2 = GDCR_SHA256_F1(wv[a]) + GDCR_SHA_MAJ(wv[a], wv[b], wv[c]);			\
    wv[d] += t1;															\
    wv[h] = t1 + t2;														\
}

#define GDCR_SHA512_EXP(a, b, c, d, e, f, g ,h, j)							\
{																			\
    t1 = wv[h] + GDCR_SHA512_F2(wv[e]) + GDCR_SHA_CH(wv[e], wv[f], wv[g])	\
         + gdcr__sha512_const_table[j] + w[j];                              \
    t2 = GDCR_SHA512_F1(wv[a]) + GDCR_SHA_MAJ(wv[a], wv[b], wv[c]);			\
    wv[d] += t1;															\
    wv[h] = t1 + t2;														\
}



typedef enum gdcr_aes_type{
	GDCR_AES_128,
	GDCR_AES_192,
	GDCR_AES_256,

	GDCR_AES_COUNT,
};

typedef struct gdcr_sha256_context{
	gdcr_u32 Hash[8];
	unsigned char Block[2 * GDCR_SHA256_BLOCK_SIZE];
	unsigned int Len;
	unsigned int TotalLen;
}gdcr_sha256_context, gdcr_sha224_context;

typedef struct gdcr_sha512_context{
	gdcr_u64 Hash[8];
	unsigned char Block[2 * GDCR_SHA512_BLOCK_SIZE];
	unsigned int Len;
	unsigned int TotalLen;
}gdcr_sha512_context, gdcr_sha384_context;


#ifdef __cplusplus
extern "C" {
#endif

/*Hash calculation for strings*/
GDCR_DEF unsigned int gd_hash_h37(const char* Str);
GDCR_DEF unsigned int gd_hash_faq6(const char* Str);
GDCR_DEF unsigned int gd_hash_rot13(const char* Str);
GDCR_DEF unsigned int gd_hash_ly(const char* Str);
GDCR_DEF unsigned int gd_hash_rs(const char* Str);
GDCR_DEF unsigned int gd_hash_pjw(const char* Str);

/*Finite field arithmetic*/
GDCR_DEF unsigned char gd_gadd_2_8(unsigned char a, unsigned char b);
GDCR_DEF unsigned char gd_gsub_2_8(unsigned char a, unsigned char b);
GDCR_DEF unsigned char gd_gmul_2_8(unsigned char a, unsigned char b);

/*MD5 hash calculation*/
GDCR_DEF void gd_hash_md5(const unsigned char* init_str, int str_len, unsigned char* out_str);

/*SHA hash calculation*/
GDCR_DEF void gd_hash_sha224(const unsigned char *message, unsigned int len, unsigned char *digest);
GDCR_DEF void gd_hash_sha256(const unsigned char *message, unsigned int len, unsigned char *digest);
GDCR_DEF void gd_hash_sha384(const unsigned char *message, unsigned int len, unsigned char *digest);
GDCR_DEF void gd_hash_sha512(const unsigned char *message, unsigned int len, unsigned char *digest);

/*AES algo*/

#ifdef __cplusplus
}
#endif

#define GD_CRYPTOS_HEADER
#endif /*GD_CRYPTOS_HEADER*/

#if defined(GD_CRYPTOS_IMPLEMENTATION) && !defined(GD_CRYPTOS_IMPLEMENTATION_DONE)
#define GD_CRYPTOS_IMPLEMENTATION_DONE

unsigned int gd_hash_h37(const char* Str){
	unsigned int Hash = 2139062143;
	const char* StrCopy = Str;
	
	for(const char* StrCopy = Str; *StrCopy; StrCopy++){
		Hash = 37 * Hash + *StrCopy;
	}

	return(Hash);
}

unsigned int gd_hash_faq6(const char* Str){
	unsigned int Hash = 0;

	for(const char* StrCopy = Str; *StrCopy; StrCopy++){
		Hash += (unsigned char)(*StrCopy);
		Hash += (Hash << 10);
		Hash = Hash ^ (Hash >> 6);
	}

	Hash += (Hash << 3);
	Hash ^= (Hash >> 11);
	Hash += (Hash << 15);

	return(Hash);
}

unsigned int gd_hash_rot13(const char* Str){
	unsigned int Hash = 0;

	for(const char* StrCopy = Str;
		*StrCopy;
		StrCopy++)
	{
		Hash += (unsigned char)(*StrCopy);
		Hash -= (Hash << 13) | (Hash >> 19);
	}

	return(Hash);
}

unsigned int gd_hash_ly(const char* Str){
	unsigned int Hash = 0;

	for (const char* StrCopy = Str; *StrCopy; StrCopy++){
		Hash = (Hash * 1664525) + (unsigned char)(*StrCopy) + 1013904223;
	}

	return(Hash);
}

unsigned int gd_hash_rs(const char* Str){
	unsigned int a = 63689;
	unsigned int Hash = 0;

	for(const char* StrCopy; *StrCopy; StrCopy++){
		Hash = Hash * a + (unsigned char)(*StrCopy);
		a *= 378551;
	}

	return(Hash);
}

unsigned int gd_hash_pjw(const char* Str){
	unsigned int Hash = 0;
	unsigned int High;

	for(const char* StrCopy = Str; *Str; Str++){
		Hash = (Hash << 4) + (unsigned char)(*StrCopy);
		if(High = Hash & 0xF0000000){
			Hash ^= High >> 24;
		}
		Hash = Hash & ~High;
	}

	return(Hash);
}

unsigned char gd_gadd_2_8(unsigned char a, unsigned char b){
	return(a ^ b);
}

unsigned char gd_gsub_2_8(unsigned char a, unsigned char b){
	return(a ^ b);
}

unsigned char gd_gmul_2_8(unsigned char a, unsigned char b){
	unsigned char Result = 0;
	
	while(b){
		if(b & 1){
			Result ^= a;
		}
		if(a & 0x80){
			a = (a << 1) ^ 0x11b;
		}
		else{
			a = a << 1;
		}
		b = b >> 1;
	}

	return(Result);
}

/*SBox transformation table*/
static unsigned char gdcr__aes_s_box[] = {
 	0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76, 
    0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0, 
    0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15, 
    0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75, 
    0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84, 
    0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf, 
    0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8, 
    0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2, 
    0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73, 
    0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb, 
    0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79, 
    0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08, 
    0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a, 
    0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e, 
    0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf, 
    0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16
};

/*Inverse SBox transformation table*/
static unsigned char gdcr__aes_inv_s_box[] = {
    0x52, 0x09, 0x6a, 0xd5, 0x30, 0x36, 0xa5, 0x38, 0xbf, 0x40, 0xa3, 0x9e, 0x81, 0xf3, 0xd7, 0xfb,
    0x7c, 0xe3, 0x39, 0x82, 0x9b, 0x2f, 0xff, 0x87, 0x34, 0x8e, 0x43, 0x44, 0xc4, 0xde, 0xe9, 0xcb,
    0x54, 0x7b, 0x94, 0x32, 0xa6, 0xc2, 0x23, 0x3d, 0xee, 0x4c, 0x95, 0x0b, 0x42, 0xfa, 0xc3, 0x4e,
    0x08, 0x2e, 0xa1, 0x66, 0x28, 0xd9, 0x24, 0xb2, 0x76, 0x5b, 0xa2, 0x49, 0x6d, 0x8b, 0xd1, 0x25,
    0x72, 0xf8, 0xf6, 0x64, 0x86, 0x68, 0x98, 0x16, 0xd4, 0xa4, 0x5c, 0xcc, 0x5d, 0x65, 0xb6, 0x92,
    0x6c, 0x70, 0x48, 0x50, 0xfd, 0xed, 0xb9, 0xda, 0x5e, 0x15, 0x46, 0x57, 0xa7, 0x8d, 0x9d, 0x84,
    0x90, 0xd8, 0xab, 0x00, 0x8c, 0xbc, 0xd3, 0x0a, 0xf7, 0xe4, 0x58, 0x05, 0xb8, 0xb3, 0x45, 0x06,
    0xd0, 0x2c, 0x1e, 0x8f, 0xca, 0x3f, 0x0f, 0x02, 0xc1, 0xaf, 0xbd, 0x03, 0x01, 0x13, 0x8a, 0x6b,
    0x3a, 0x91, 0x11, 0x41, 0x4f, 0x67, 0xdc, 0xea, 0x97, 0xf2, 0xcf, 0xce, 0xf0, 0xb4, 0xe6, 0x73,
    0x96, 0xac, 0x74, 0x22, 0xe7, 0xad, 0x35, 0x85, 0xe2, 0xf9, 0x37, 0xe8, 0x1c, 0x75, 0xdf, 0x6e,
    0x47, 0xf1, 0x1a, 0x71, 0x1d, 0x29, 0xc5, 0x89, 0x6f, 0xb7, 0x62, 0x0e, 0xaa, 0x18, 0xbe, 0x1b,
    0xfc, 0x56, 0x3e, 0x4b, 0xc6, 0xd2, 0x79, 0x20, 0x9a, 0xdb, 0xc0, 0xfe, 0x78, 0xcd, 0x5a, 0xf4,
    0x1f, 0xdd, 0xa8, 0x33, 0x88, 0x07, 0xc7, 0x31, 0xb1, 0x12, 0x10, 0x59, 0x27, 0x80, 0xec, 0x5f,
    0x60, 0x51, 0x7f, 0xa9, 0x19, 0xb5, 0x4a, 0x0d, 0x2d, 0xe5, 0x7a, 0x9f, 0x93, 0xc9, 0x9c, 0xef,
    0xa0, 0xe0, 0x3b, 0x4d, 0xae, 0x2a, 0xf5, 0xb0, 0xc8, 0xeb, 0xbb, 0x3c, 0x83, 0x53, 0x99, 0x61,
    0x17, 0x2b, 0x04, 0x7e, 0xba, 0x77, 0xd6, 0x26, 0xe1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0c, 0x7d
};

/*Round constants array precalculated*/
static unsigned char gdcr__aes_rcon[] = {
	0x00, 0x00, 0x00, 0x00,
	0x01, 0x00, 0x00, 0x00,
	0x02, 0x00, 0x00, 0x00,
	0x04, 0x00, 0x00, 0x00,
	0x08, 0x00, 0x00, 0x00,
	0x10, 0x00, 0x00, 0x00,
	0x20, 0x00, 0x00, 0x00,
	0x40, 0x00, 0x00, 0x00,
	0x80, 0x00, 0x00, 0x00,
	0x1b, 0x00, 0x00, 0x00,
	0x36, 0x00, 0x00, 0x00,
	0x6c, 0x00, 0x00, 0x00,
	0xd8, 0x00, 0x00, 0x00,
	0xab, 0x00, 0x00, 0x00,
	0x4d, 0x00, 0x00, 0x00,
	0x9a, 0x00, 0x00, 0x00,
	0x2f, 0x00, 0x00, 0x00,
	0x5e, 0x00, 0x00, 0x00
};

/*
 Function used in the Key Expansion routine that takes a four-byte
 input word and applies an S-box to each of the four bytes to
 produce an output word.
*/
static void gdcr__aes_sub_word(unsigned char* w){
	w[0] = gdcr__aes_s_box[16 * ((w[0] & 0xf0) >> 4) + (w[0] & 0x0f)];
	w[1] = gdcr__aes_s_box[16 * ((w[1] & 0xf0) >> 4) + (w[1] & 0x0f)];
	w[2] = gdcr__aes_s_box[16 * ((w[2] & 0xf0) >> 4) + (w[2] & 0x0f)];
	w[3] = gdcr__aes_s_box[16 * ((w[3] & 0xf0) >> 4) + (w[3] & 0x0f)];
}

/*
 Transformation in the Cipher that processes the State using a non­
 linear byte substitution table (S-box) that operates on each of the
 State bytes independently.
*/
static void gdcr__aes_sub_bytes(unsigned char* State){
	int i, j;
	unsigned int Row, Col;

	for (i = 0; i < 4; i++){
		for (j = 0; j < GDCR_AES_NB; i++){
			Row = (State[GDCR_AES_NB * i + j] & 0xf0) >> 4;
			Col = State[GDCR_AES_NB * i + j] & 0x0f;
			State[GDCR_AES_NB * i + j] = gdcr__aes_s_box[16 * Row + Col];
		}
	}
}

/*
 Transformation in the Inverse Cipher that is the inverse of
 SubBytes(). Uses Inverse S-Box.
*/
static void gdcr__aes_inv_sub_bytes(unsigned char* State){
	int i, j;
	unsigned int Row, Col;

	for (i = 0; i < 4; i++){
		for (j = 0; j < GDCR_AES_NB; i++){
			Row = (State[GDCR_AES_NB * i + j] & 0xf0) >> 4;
			Col = State[GDCR_AES_NB * i + j] & 0x0f;
			State[GDCR_AES_NB * i + j] = gdcr__aes_inv_s_box[16 * Row + Col];
		}
	}
}


/*
 Function used in the Key Expansion routine that takes a four-byte
 word and performs a cyclic permutation.
*/
static void gdcr__aes_rot_word(unsigned char* w){
	unsigned char Temp;
	int i;

	Temp = w[0];

	for (i = 0; i < 3; i++){
		w[i] = w[i + 1];
	}

	w[3] = Temp;
}

static void gdcr__aes_coef_add(unsigned char* a, unsigned char* b, unsigned char* d){
	d[0] = a[0] ^ b[0];
	d[1] = a[1] ^ b[1];
	d[2] = a[2] ^ b[2];
	d[3] = a[3] ^ b[3];
}

static void gdcr__aes_coef_mul(unsigned char* a, unsigned char* b, unsigned char* d){
	d[0] = gd_gmul_2_8(a[0], b[0]) ^ gd_gmul_2_8(a[0], b[1]) ^ gd_gmul_2_8(a[0], b[2]) ^ gd_gmul_2_8(a[0], b[3]);
	d[1] = gd_gmul_2_8(a[1], b[0]) ^ gd_gmul_2_8(a[1], b[1]) ^ gd_gmul_2_8(a[1], b[2]) ^ gd_gmul_2_8(a[1], b[3]);
	d[2] = gd_gmul_2_8(a[2], b[0]) ^ gd_gmul_2_8(a[2], b[1]) ^ gd_gmul_2_8(a[2], b[2]) ^ gd_gmul_2_8(a[2], b[3]);
	d[3] = gd_gmul_2_8(a[3], b[0]) ^ gd_gmul_2_8(a[3], b[1]) ^ gd_gmul_2_8(a[3], b[2]) ^ gd_gmul_2_8(a[3], b[3]);
}



/*
 Transformation in the Cipher that takes all of the columns of the
 State and mixes their data (independently of one another) to
 produce new columns.
*/
static void gdcr__aes_mix_columns(unsigned char* State){
	unsigned char a[] = { 0x02, 0x01, 0x01, 0x03 };
	int j;
	unsigned char Col[4], Res[4];

	for (j = 0; j < GDCR_AES_NB; j++){
		Col[0] = State[GDCR_AES_NB * 0 + j];
		Col[1] = State[GDCR_AES_NB * 1 + j];
		Col[2] = State[GDCR_AES_NB * 2 + j];
		Col[3] = State[GDCR_AES_NB * 3 + j];

		gdcr__aes_coef_mul(a, Col, Res);

		State[GDCR_AES_NB * 0 + j] = Res[0];
		State[GDCR_AES_NB * 1 + j] = Res[1];
		State[GDCR_AES_NB * 2 + j] = Res[2];
		State[GDCR_AES_NB * 3 + j] = Res[3];

	}
}

/*
 Transformation in the Inverse Cipher that is the inverse of
 MixColumns().
*/
static void gdcr__aes_inv_mix_columns(unsigned char* State){
	unsigned char a[] = { 0x0e, 0x09, 0x0d, 0x0b };
	int j;
	unsigned char Col[4], Res[4];

	for (j = 0; j < GDCR_AES_NB; j++){
		Col[0] = State[GDCR_AES_NB * 0 + j];
		Col[1] = State[GDCR_AES_NB * 1 + j];
		Col[2] = State[GDCR_AES_NB * 2 + j];
		Col[3] = State[GDCR_AES_NB * 3 + j];

		gdcr__aes_coef_mul(a, Col, Res);

		State[GDCR_AES_NB * 0 + j] = Res[0];
		State[GDCR_AES_NB * 1 + j] = Res[1];
		State[GDCR_AES_NB * 2 + j] = Res[2];
		State[GDCR_AES_NB * 3 + j] = Res[3];
	}
}

/*
 Transformation in the Cipher that processes the State by cyclically
 shifting the last three rows of the State by different offsets.
*/
static void gdcr__aes_shift_rows(unsigned char* State){
	int i, k, s, Temp;

	for (i = 1; i < 4; i++){
		s = 0;
		while (s < i){
			Temp = State[GDCR_AES_NB * i];

			for (k = 1; k < GDCR_AES_NB; k++){
				State[GDCR_AES_NB * i + k - 1] = State[GDCR_AES_NB * i + k];
			}

			State[GDCR_AES_NB * (i + 1) - 1] = Temp;
			s++;
		}
	}
}

/*
 Transformation in the Inverse Cipher that is the inverse of
 ShiftRows().
*/
static void gdcr__aes_inv_shift_rows(unsigned char* State){
	int i, k, s, Temp;

	for (i = 1; i < 4; i++){
		s = 0;
		while (s < i){
			Temp = State[GDCR_AES_NB * (i + 1) - 1];

			for (k = GDCR_AES_NB - 1; k > 0; k--){
				State[GDCR_AES_NB * i + k] = State[GDCR_AES_NB * (i + 1) - 1];
			}

			State[GDCR_AES_NB * i] = Temp;
			s++;
		}
	}
}


/*
 Transformation in the Cipher and Inverse Cipher in which a Round
 Key is added to the State using an XOR operation. The length of a
 Round Key equals the size of the State (i.e., for Nb = 4, the Round
 Key length equals 128 bits/16 bytes).
*/
static void gdcr__aes_add_round_key(unsigned char* State, unsigned char* w, unsigned char r){
	unsigned char i;

	for (i = 0; i < GDCR_AES_NB; i++){
		State[GDCR_AES_NB * 0 + i] = State[GDCR_AES_NB * 0 + i] ^ w[4 * GDCR_AES_NB * r + 4 * i + 0];
		State[GDCR_AES_NB * 1 + i] = State[GDCR_AES_NB * 1 + i] ^ w[4 * GDCR_AES_NB * r + 4 * i + 1];
		State[GDCR_AES_NB * 2 + i] = State[GDCR_AES_NB * 2 + i] ^ w[4 * GDCR_AES_NB * r + 4 * i + 2];
		State[GDCR_AES_NB * 3 + i] = State[GDCR_AES_NB * 3 + i] ^ w[4 * GDCR_AES_NB * r + 4 * i + 3];
	}
}

static void gdcr__aes_key_expansion(unsigned char* key, unsigned char* w, int Nk){
	unsigned char Temp[4];

	int Nr = -1;;
	switch (Nk){
		case 4:{
			Nr = 10;
		}break;
		case 6:{
			Nr = 12;
		}break;
		case 8:{
			Nr = 14;
		}break;
	}

	int Len = GDCR_AES_NB * (Nr + 1);
	
	for (int i = 0; i < Nk; i++){
		w[4 * i + 0] = (unsigned short)key[4 * i + 0];
		w[4 * i + 1] = (unsigned short)key[4 * i + 1];
		w[4 * i + 2] = (unsigned short)key[4 * i + 2];
		w[4 * i + 3] = (unsigned short)key[4 * i + 3];
	}

	for (int i = Nk; i < Len; i++){
		Temp[0] = w[4 * (i - 1) + 0];
		Temp[1] = w[4 * (i - 1) + 1];
		Temp[2] = w[4 * (i - 1) + 2];
		Temp[3] = w[4 * (i - 1) + 3];

		if (i % Nk == 0){
			gdcr__aes_rot_word(Temp);
			gdcr__aes_sub_word(Temp);
			gdcr__aes_coef_add(Temp, &gdcr__aes_rcon[(i / Nk) * 4], Temp);
		}
		else if (Nk > 6 && (i % Nk == 4)){
			gdcr__aes_sub_word(Temp);
		}
		
		w[4 * i + 0] = w[4 * (i - Nk) + 0] ^ Temp[0];
		w[4 * i + 1] = w[4 * (i - Nk) + 1] ^ Temp[1];
		w[4 * i + 2] = w[4 * (i - Nk) + 2] ^ Temp[2];
		w[4 * i + 3] = w[4 * (i - Nk) + 3] ^ Temp[3];
	}
}

static void gdcr__aes_cipher(unsigned char* In, unsigned char* Out, unsigned char* w, int Nr){
	unsigned char State[4 * GDCR_AES_NB];
	int r, i, j;

	for (i = 0; i < 4; i++){
		for (j = 0; j < GDCR_AES_NB; j++){
			State[GDCR_AES_NB * i + j] = In[i + 4 * j];
		}
	}

	gdcr__aes_add_round_key(State, w, 0);

	for (r = 1; r < Nr; r++){
		gdcr__aes_sub_bytes(State);
		gdcr__aes_shift_rows(State);
		gdcr__aes_mix_columns(State);
		gdcr__aes_add_round_key(State, w, r);
	}

	gdcr__aes_sub_bytes(State);
	gdcr__aes_shift_rows(State);
	gdcr__aes_add_round_key(State, w, Nr);

	for (i = 0; i < 4; i++){
		for (j = 0; j < GDCR_AES_NB; j++){
			Out[i + 4 * j] = State[GDCR_AES_NB * i + j];
		}
	}
}

static void gdcr__aes_inv_cipher(unsigned char* In, unsigned char* Out, unsigned char* w, int Nr){
	unsigned char State[4 * GDCR_AES_NB];
	int r, i, j;

	for (i = 0; i < 4; i++){
		for (j = 0; j < GDCR_AES_NB; j++){
			State[GDCR_AES_NB * i + j] = In[i + 4 * j];
		}
	}

	gdcr__aes_add_round_key(State, w, Nr);

	for (r = Nr - 1; r >= 1; r--){
		gdcr__aes_inv_shift_rows(State);
		gdcr__aes_inv_sub_bytes(State);
		gdcr__aes_add_round_key(State, w, r);
		gdcr__aes_inv_mix_columns(State);
	}

	gdcr__aes_inv_shift_rows(State);
	gdcr__aes_inv_sub_bytes(State);
	gdcr__aes_add_round_key(State, w, 0);

	for (i = 0; i < 4; i++){
		for (j = 0; j < GDCR_AES_NB; j++){
			Out[i + 4 * j] = State[GDCR_AES_NB * i + j];
		}
	}
}

void gd_aes_encode(
	unsigned char* Data,
	unsigned char* Key, int KeyBytesCount,
	unsigned char* Out,
	gdcr_aes_type Type)
{
	int Nk = -1;
	int Nr = -1;

	switch (Type){
		case GDCR_AES_128: Nk = 4; Nr = 10; break;
		case GDCR_AES_192: Nk = 6; Nr = 12; break;
		case GDCR_AES_256: Nk = 8; Nr = 14; break;
	}

	/*Expanded key memory allocation*/
	unsigned char* w = (unsigned char*)malloc(GDCR_AES_NB * (Nr + 1) * 4);

	//unsigned char calculated_md5_hash[16];
	//gd_hash_md5(Key, KeyBytesCount, calculated_md5_hash);

	gdcr__aes_key_expansion(Key, w, Nr);

	gdcr__aes_cipher(Data, Out, w, Nr);

	free(w);
}

void gd_aes_decode(
	unsigned char* Data,
	unsigned char* Key, int KeyBytesCount,
	unsigned char* Out,
	gdcr_aes_type Type)
{
	int Nk = -1;
	int Nr = -1;

	switch (Type){
		case GDCR_AES_128: Nk = 4; Nr = 10; break;
		case GDCR_AES_192: Nk = 6; Nr = 12; break;
		case GDCR_AES_256: Nk = 8; Nr = 14; break;
	}

	/*Expanded key memory allocation*/
	unsigned char* w = (unsigned char*)malloc(GDCR_AES_NB * (Nr + 1) * 4);

	//unsigned char calculated_md5_hash[16];
	//gd_hash_md5(Key, KeyBytesCount, calculated_md5_hash);

	gdcr__aes_key_expansion(Key, w, Nr);

	gdcr__aes_inv_cipher(Data, Out, w, Nr);

	free(w);
}


/*
 *
 *
 *
 *
 *	
 *
 *
 *
 *
 *	MD5
 */

/* 
	Decodes input (unsigned char) into output (unsigned 32 bit int).
	Assumes len is a multiple of 4.
*/
static void gdcr__md5_bytes_to_words(gdcr_u32* Out, unsigned char* In, int Len){
	int i, j;
	for (i = 0, j = 0; j < Len; i++, j += 4){
		Out[i] = ((gdcr_u32)In[j]) |
			(((gdcr_u32)In[j + 1]) << 8) |
			(((gdcr_u32)In[j + 2]) << 16) |
			(((gdcr_u32)In[j + 3]) << 24);
	}
}


/* 
	Encodes input (unsigned 32 bit int) into output (unsigned char).
	Assumes len is a multiple of 4.
*/
static void gdcr__md5_words_to_bytes(unsigned char* Output, gdcr_u32* Input, int Len){
	int i, j;
	for (i = 0, j = 0; j < Len; i++, j += 4){
		Output[j] = (unsigned char)(Input[i] & 0xFF);
		Output[j + 1] = (unsigned char)((Input[i] >> 8) & 0xFF);
		Output[j + 2] = (unsigned char)((Input[i] >> 16) & 0xFF);
		Output[j + 3] = (unsigned char)((Input[i] >> 24) & 0xFF);
	}
}

static void gdcr__md5_word_to_bytes(gdcr_u32 value, gdcr_u8* bytes){
#if 1
	bytes[0] = (gdcr_u8)value;
	bytes[1] = (gdcr_u8)(value >> 8);
	bytes[2] = (gdcr_u8)(value >> 16);
	bytes[3] = (gdcr_u8)(value >> 24);
#else
	bytes[0] = (gdcr_u8)(value & 0xFF);
	bytes[1] = (gdcr_u8)((value >> 8) & 0xFF);
	bytes[2] = (gdcr_u8)((value >> 16) & 0xFF);
	bytes[3] = (gdcr_u8)((value >> 24) & 0xFF);
#endif
}

static gdcr_u32 gdcr__md5_bytes_to_word(const gdcr_u8* bytes){
	gdcr_u32 res = 
		(gdcr_u32)bytes[0]
		| ((gdcr_u32)bytes[1] << 8)
		| ((gdcr_u32)bytes[2] << 16)
		| ((gdcr_u32)bytes[3] << 24);
	
	return(res);
}

static void gdcr__md5_memset(unsigned char* out_ptr, int value, int len){
	int i;
	for (i = 0; i < len; i++){
		((unsigned char*)out_ptr)[i] = value;
	}
}

static void gdcr__md5_memcpy(unsigned char* out_ptr, const unsigned char* in_ptr, int len){
	int i;

	for (i = 0; i < len; i++){
		out_ptr[i] = in_ptr[i];
	}
}

static void gdcr__md5_transform(gdcr_u32 State[4], unsigned char Block[64]){
	gdcr_u32 a = State[0];
	gdcr_u32 b = State[1];
	gdcr_u32 c = State[2];
	gdcr_u32 d = State[3];

	gdcr_u32 x[16];
	gdcr__md5_bytes_to_words(x, Block, 64);

	/*Round 1*/
	GDCR_MD5_FF(a, b, c, d, x[0], GDCR_MD5_S11, 0xd76aa478); /* 1 */
	GDCR_MD5_FF(d, a, b, c, x[1], GDCR_MD5_S12, 0xe8c7b756); /* 2 */
	GDCR_MD5_FF(c, d, a, b, x[2], GDCR_MD5_S13, 0x242070db); /* 3 */
	GDCR_MD5_FF(b, c, d, a, x[3], GDCR_MD5_S14, 0xc1bdceee); /* 4 */
	GDCR_MD5_FF(a, b, c, d, x[4], GDCR_MD5_S11, 0xf57c0faf); /* 5 */
	GDCR_MD5_FF(d, a, b, c, x[5], GDCR_MD5_S12, 0x4787c62a); /* 6 */
	GDCR_MD5_FF(c, d, a, b, x[6], GDCR_MD5_S13, 0xa8304613); /* 7 */
	GDCR_MD5_FF(b, c, d, a, x[7], GDCR_MD5_S14, 0xfd469501); /* 8 */
	GDCR_MD5_FF(a, b, c, d, x[8], GDCR_MD5_S11, 0x698098d8); /* 9 */
	GDCR_MD5_FF(d, a, b, c, x[9], GDCR_MD5_S12, 0x8b44f7af); /* 10 */
	GDCR_MD5_FF(c, d, a, b, x[10], GDCR_MD5_S13, 0xffff5bb1); /* 11 */
	GDCR_MD5_FF(b, c, d, a, x[11], GDCR_MD5_S14, 0x895cd7be); /* 12 */
	GDCR_MD5_FF(a, b, c, d, x[12], GDCR_MD5_S11, 0x6b901122); /* 13 */
	GDCR_MD5_FF(d, a, b, c, x[13], GDCR_MD5_S12, 0xfd987193); /* 14 */
	GDCR_MD5_FF(c, d, a, b, x[14], GDCR_MD5_S13, 0xa679438e); /* 15 */
	GDCR_MD5_FF(b, c, d, a, x[15], GDCR_MD5_S14, 0x49b40821); /* 16 */

	/*Round 2*/
	GDCR_MD5_GG(a, b, c, d, x[1], GDCR_MD5_S21, 0xf61e2562); /* 17 */
	GDCR_MD5_GG(d, a, b, c, x[6], GDCR_MD5_S22, 0xc040b340); /* 18 */
	GDCR_MD5_GG(c, d, a, b, x[11], GDCR_MD5_S23, 0x265e5a51); /* 19 */
	GDCR_MD5_GG(b, c, d, a, x[0], GDCR_MD5_S24, 0xe9b6c7aa); /* 20 */
	GDCR_MD5_GG(a, b, c, d, x[5], GDCR_MD5_S21, 0xd62f105d); /* 21 */
	GDCR_MD5_GG(d, a, b, c, x[10], GDCR_MD5_S22, 0x2441453); /* 22 */
	GDCR_MD5_GG(c, d, a, b, x[15], GDCR_MD5_S23, 0xd8a1e681); /* 23 */
	GDCR_MD5_GG(b, c, d, a, x[4], GDCR_MD5_S24, 0xe7d3fbc8); /* 24 */
	GDCR_MD5_GG(a, b, c, d, x[9], GDCR_MD5_S21, 0x21e1cde6); /* 25 */
	GDCR_MD5_GG(d, a, b, c, x[14], GDCR_MD5_S22, 0xc33707d6); /* 26 */
	GDCR_MD5_GG(c, d, a, b, x[3], GDCR_MD5_S23, 0xf4d50d87); /* 27 */
	GDCR_MD5_GG(b, c, d, a, x[8], GDCR_MD5_S24, 0x455a14ed); /* 28 */
	GDCR_MD5_GG(a, b, c, d, x[13], GDCR_MD5_S21, 0xa9e3e905); /* 29 */
	GDCR_MD5_GG(d, a, b, c, x[2], GDCR_MD5_S22, 0xfcefa3f8); /* 30 */
	GDCR_MD5_GG(c, d, a, b, x[7], GDCR_MD5_S23, 0x676f02d9); /* 31 */
	GDCR_MD5_GG(b, c, d, a, x[12], GDCR_MD5_S24, 0x8d2a4c8a); /* 32 */

	/* Round 3 */
	GDCR_MD5_HH(a, b, c, d, x[5], GDCR_MD5_S31, 0xfffa3942); /* 33 */
	GDCR_MD5_HH(d, a, b, c, x[8], GDCR_MD5_S32, 0x8771f681); /* 34 */
	GDCR_MD5_HH(c, d, a, b, x[11], GDCR_MD5_S33, 0x6d9d6122); /* 35 */
	GDCR_MD5_HH(b, c, d, a, x[14], GDCR_MD5_S34, 0xfde5380c); /* 36 */
	GDCR_MD5_HH(a, b, c, d, x[1], GDCR_MD5_S31, 0xa4beea44); /* 37 */
	GDCR_MD5_HH(d, a, b, c, x[4], GDCR_MD5_S32, 0x4bdecfa9); /* 38 */
	GDCR_MD5_HH(c, d, a, b, x[7], GDCR_MD5_S33, 0xf6bb4b60); /* 39 */
	GDCR_MD5_HH(b, c, d, a, x[10], GDCR_MD5_S34, 0xbebfbc70); /* 40 */
	GDCR_MD5_HH(a, b, c, d, x[13], GDCR_MD5_S31, 0x289b7ec6); /* 41 */
	GDCR_MD5_HH(d, a, b, c, x[0], GDCR_MD5_S32, 0xeaa127fa); /* 42 */
	GDCR_MD5_HH(c, d, a, b, x[3], GDCR_MD5_S33, 0xd4ef3085); /* 43 */
	GDCR_MD5_HH(b, c, d, a, x[6], GDCR_MD5_S34, 0x4881d05); /* 44 */
	GDCR_MD5_HH(a, b, c, d, x[9], GDCR_MD5_S31, 0xd9d4d039); /* 45 */
	GDCR_MD5_HH(d, a, b, c, x[12], GDCR_MD5_S32, 0xe6db99e5); /* 46 */
	GDCR_MD5_HH(c, d, a, b, x[15], GDCR_MD5_S33, 0x1fa27cf8); /* 47 */
	GDCR_MD5_HH(b, c, d, a, x[2], GDCR_MD5_S34, 0xc4ac5665); /* 48 */

	/* Round 4 */
	GDCR_MD5_II(a, b, c, d, x[0], GDCR_MD5_S41, 0xf4292244); /* 49 */
	GDCR_MD5_II(d, a, b, c, x[7], GDCR_MD5_S42, 0x432aff97); /* 50 */
	GDCR_MD5_II(c, d, a, b, x[14], GDCR_MD5_S43, 0xab9423a7); /* 51 */
	GDCR_MD5_II(b, c, d, a, x[5], GDCR_MD5_S44, 0xfc93a039); /* 52 */
	GDCR_MD5_II(a, b, c, d, x[12], GDCR_MD5_S41, 0x655b59c3); /* 53 */
	GDCR_MD5_II(d, a, b, c, x[3], GDCR_MD5_S42, 0x8f0ccc92); /* 54 */
	GDCR_MD5_II(c, d, a, b, x[10], GDCR_MD5_S43, 0xffeff47d); /* 55 */
	GDCR_MD5_II(b, c, d, a, x[1], GDCR_MD5_S44, 0x85845dd1); /* 56 */
	GDCR_MD5_II(a, b, c, d, x[8], GDCR_MD5_S41, 0x6fa87e4f); /* 57 */
	GDCR_MD5_II(d, a, b, c, x[15], GDCR_MD5_S42, 0xfe2ce6e0); /* 58 */
	GDCR_MD5_II(c, d, a, b, x[6], GDCR_MD5_S43, 0xa3014314); /* 59 */
	GDCR_MD5_II(b, c, d, a, x[13], GDCR_MD5_S44, 0x4e0811a1); /* 60 */
	GDCR_MD5_II(a, b, c, d, x[4], GDCR_MD5_S41, 0xf7537e82); /* 61 */
	GDCR_MD5_II(d, a, b, c, x[11], GDCR_MD5_S42, 0xbd3af235); /* 62 */
	GDCR_MD5_II(c, d, a, b, x[2], GDCR_MD5_S43, 0x2ad7d2bb); /* 63 */
	GDCR_MD5_II(b, c, d, a, x[9], GDCR_MD5_S44, 0xeb86d391); /* 64 */

	gdcr__md5_memset((unsigned char*)x, 0, sizeof(x));

	State[0] += a;
	State[1] += b;
	State[2] += c;
	State[3] += d;
}

void gd_hash_md5(const unsigned char* init_str, int init_len, unsigned char* out_str){
	/*
		Algorigth was taken from:
		https://ru.wikipedia.org/wiki/MD5

		Thanks to:
		https://github.com/pod32g
		https://github.com/keplerproject
		https://github.com/leimingshan
	*/
	
	/*This is future hash values*/
	gdcr_u32 init_hash[4];
	
	int new_len;
	int it;
	unsigned char* new_message = 0;

	/*Step 1*/

	//64 = 512 / 8
	//56 = 448 / 8
	for (new_len = init_len + 1; new_len % 64 != 56; new_len++);

	/*Step 2*/
	new_message = (unsigned char*)malloc(new_len + 8);
	gdcr__md5_memcpy(new_message, init_str, init_len);
	new_message[init_len] = 0x80;
	for (it = init_len + 1; it < new_len; it++){
		new_message[it] = 0;
	}
	
	// Theese to lines append (init_len * 8) to the new_message at new_len
	gdcr__md5_word_to_bytes(init_len * 8, new_message + new_len);
	gdcr__md5_word_to_bytes(init_len >> 29, new_message + new_len + 4);

	/*Step 3*/
	init_hash[0] = 0x67452301;
	init_hash[1] = 0xefcdab89;
	init_hash[2] = 0x98badcfe;
	init_hash[3] = 0x10325476;

	/*Step 4*/

	//64 = 512 / 8
	for (it = 0; it < new_len; it += 64){
		gdcr__md5_transform(init_hash, new_message + it);
	}

	free(new_message);
	
	/*Step 5*/
	gdcr__md5_word_to_bytes(init_hash[0], out_str);
	gdcr__md5_word_to_bytes(init_hash[1], out_str + 4);
	gdcr__md5_word_to_bytes(init_hash[2], out_str + 8);
	gdcr__md5_word_to_bytes(init_hash[3], out_str + 12);
}


/*
*
*
*
*
*
*
*
*
*
*	SHA
*/

static unsigned int gdcr__sha224_init_hash[8] = {
	0xc1059ed8, 0x367cd507, 0x3070dd17, 0xf70e5939,
	0xffc00b31, 0x68581511, 0x64f98fa7, 0xbefa4fa4 
};

static unsigned int gdcr__sha256_init_hash[8] = {
	0x6a09e667, 0xbb67ae85, 0x3c6ef372, 0xa54ff53a,
	0x510e527f, 0x9b05688c, 0x1f83d9ab, 0x5be0cd19 
};

static unsigned long long gdcr__sha384_init_hash[8] = {
	0xcbbb9d5dc1059ed8ULL, 0x629a292a367cd507ULL,
	0x9159015a3070dd17ULL, 0x152fecd8f70e5939ULL,
	0x67332667ffc00b31ULL, 0x8eb44a8768581511ULL,
	0xdb0c2e0d64f98fa7ULL, 0x47b5481dbefa4fa4ULL
};

static unsigned long long gdcr__sha512_init_hash[8] ={
	0x6a09e667f3bcc908ULL, 0xbb67ae8584caa73bULL,
	0x3c6ef372fe94f82bULL, 0xa54ff53a5f1d36f1ULL,
	0x510e527fade682d1ULL, 0x9b05688c2b3e6c1fULL,
	0x1f83d9abfb41bd6bULL, 0x5be0cd19137e2179ULL
};

static unsigned int gdcr__sha256_const_table[64] =	{ 
	0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5,
	0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
	0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3,
	0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
	0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc,
	0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
	0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7,
	0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
	0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13,
	0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
	0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3,
	0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
	0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5,
	0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
	0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208,
	0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
};

static unsigned long long gdcr__sha512_const_table[80] = {
	0x428a2f98d728ae22ULL, 0x7137449123ef65cdULL,
	0xb5c0fbcfec4d3b2fULL, 0xe9b5dba58189dbbcULL,
	0x3956c25bf348b538ULL, 0x59f111f1b605d019ULL,
	0x923f82a4af194f9bULL, 0xab1c5ed5da6d8118ULL,
	0xd807aa98a3030242ULL, 0x12835b0145706fbeULL,
	0x243185be4ee4b28cULL, 0x550c7dc3d5ffb4e2ULL,
	0x72be5d74f27b896fULL, 0x80deb1fe3b1696b1ULL,
	0x9bdc06a725c71235ULL, 0xc19bf174cf692694ULL,
	0xe49b69c19ef14ad2ULL, 0xefbe4786384f25e3ULL,
	0x0fc19dc68b8cd5b5ULL, 0x240ca1cc77ac9c65ULL,
	0x2de92c6f592b0275ULL, 0x4a7484aa6ea6e483ULL,
	0x5cb0a9dcbd41fbd4ULL, 0x76f988da831153b5ULL,
	0x983e5152ee66dfabULL, 0xa831c66d2db43210ULL,
	0xb00327c898fb213fULL, 0xbf597fc7beef0ee4ULL,
	0xc6e00bf33da88fc2ULL, 0xd5a79147930aa725ULL,
	0x06ca6351e003826fULL, 0x142929670a0e6e70ULL,
	0x27b70a8546d22ffcULL, 0x2e1b21385c26c926ULL,
	0x4d2c6dfc5ac42aedULL, 0x53380d139d95b3dfULL,
	0x650a73548baf63deULL, 0x766a0abb3c77b2a8ULL,
	0x81c2c92e47edaee6ULL, 0x92722c851482353bULL,
	0xa2bfe8a14cf10364ULL, 0xa81a664bbc423001ULL,
	0xc24b8b70d0f89791ULL, 0xc76c51a30654be30ULL,
	0xd192e819d6ef5218ULL, 0xd69906245565a910ULL,
	0xf40e35855771202aULL, 0x106aa07032bbd1b8ULL,
	0x19a4c116b8d2d0c8ULL, 0x1e376c085141ab53ULL,
	0x2748774cdf8eeb99ULL, 0x34b0bcb5e19b48a8ULL,
	0x391c0cb3c5c95a63ULL, 0x4ed8aa4ae3418acbULL,
	0x5b9cca4f7763e373ULL, 0x682e6ff3d6b2b8a3ULL,
	0x748f82ee5defb2fcULL, 0x78a5636f43172f60ULL,
	0x84c87814a1f0ab72ULL, 0x8cc702081a6439ecULL,
	0x90befffa23631e28ULL, 0xa4506cebde82bde9ULL,
	0xbef9a3f7b2c67915ULL, 0xc67178f2e372532bULL,
	0xca273eceea26619cULL, 0xd186b8c721c0c207ULL,
	0xeada7dd6cde0eb1eULL, 0xf57d4f7fee6ed178ULL,
	0x06f067aa72176fbaULL, 0x0a637dc5a2c898a6ULL,
	0x113f9804bef90daeULL, 0x1b710b35131c471bULL,
	0x28db77f523047d84ULL, 0x32caab7b40c72493ULL,
	0x3c9ebe0a15c9bebcULL, 0x431d67c49c100d4cULL,
	0x4cc5d4becb3e42b6ULL, 0x597f299cfc657e2aULL,
	0x5fcb6fab3ad6faecULL, 0x6c44198c4a475817ULL
};

static void gdcr__sha256_transform(
	gdcr_sha256_context* context,
	const unsigned char* message,
	unsigned int block_number)
{
	uint32 w[64];
	uint32 wv[8];
	uint32 t1, t2;
	const unsigned char *sub_block;
	int i;


#ifndef GDCR_SHA_UNROLL_LOOPS
	int j;
#endif


	for (i = 0; i < (int)block_number; i++) {
		sub_block = message + (i << 6);

#ifndef GDCR_SHA_UNROLL_LOOPS
		for (j = 0; j < 16; j++) {
			GDCR_PACK32(&sub_block[j << 2], &w[j]);
		}

		for (j = 16; j < 64; j++) {
			GDCR_SHA256_SCR(j);
		}

		for (j = 0; j < 8; j++) {
			wv[j] = context->Hash[j];
		}

		for (j = 0; j < 64; j++) {
			t1 = wv[7] + GDCR_SHA256_F2(wv[4]) + GDCR_SHA_CH(wv[4], wv[5], wv[6])
				+ gdcr__sha256_const_table[j] + w[j];
			t2 = GDCR_SHA256_F1(wv[0]) + GDCR_SHA_MAJ(wv[0], wv[1], wv[2]);
			wv[7] = wv[6];
			wv[6] = wv[5];
			wv[5] = wv[4];
			wv[4] = wv[3] + t1;
			wv[3] = wv[2];
			wv[2] = wv[1];
			wv[1] = wv[0];
			wv[0] = t1 + t2;
		}

		for (j = 0; j < 8; j++) {
			context->Hash[j] += wv[j];
		}
#else
		GDCR_PACK32(&sub_block[0], &w[0]); GDCR_PACK32(&sub_block[4], &w[1]);
		GDCR_PACK32(&sub_block[8], &w[2]); GDCR_PACK32(&sub_block[12], &w[3]);
		GDCR_PACK32(&sub_block[16], &w[4]); GDCR_PACK32(&sub_block[20], &w[5]);
		GDCR_PACK32(&sub_block[24], &w[6]); GDCR_PACK32(&sub_block[28], &w[7]);
		GDCR_PACK32(&sub_block[32], &w[8]); GDCR_PACK32(&sub_block[36], &w[9]);
		GDCR_PACK32(&sub_block[40], &w[10]); GDCR_PACK32(&sub_block[44], &w[11]);
		GDCR_PACK32(&sub_block[48], &w[12]); GDCR_PACK32(&sub_block[52], &w[13]);
		GDCR_PACK32(&sub_block[56], &w[14]); GDCR_PACK32(&sub_block[60], &w[15]);

		GDCR_SHA256_SCR(16); GDCR_SHA256_SCR(17); GDCR_SHA256_SCR(18); GDCR_SHA256_SCR(19);
		GDCR_SHA256_SCR(20); GDCR_SHA256_SCR(21); GDCR_SHA256_SCR(22); GDCR_SHA256_SCR(23);
		GDCR_SHA256_SCR(24); GDCR_SHA256_SCR(25); GDCR_SHA256_SCR(26); GDCR_SHA256_SCR(27);
		GDCR_SHA256_SCR(28); GDCR_SHA256_SCR(29); GDCR_SHA256_SCR(30); GDCR_SHA256_SCR(31);
		GDCR_SHA256_SCR(32); GDCR_SHA256_SCR(33); GDCR_SHA256_SCR(34); GDCR_SHA256_SCR(35);
		GDCR_SHA256_SCR(36); GDCR_SHA256_SCR(37); GDCR_SHA256_SCR(38); GDCR_SHA256_SCR(39);
		GDCR_SHA256_SCR(40); GDCR_SHA256_SCR(41); GDCR_SHA256_SCR(42); GDCR_SHA256_SCR(43);
		GDCR_SHA256_SCR(44); GDCR_SHA256_SCR(45); GDCR_SHA256_SCR(46); GDCR_SHA256_SCR(47);
		GDCR_SHA256_SCR(48); GDCR_SHA256_SCR(49); GDCR_SHA256_SCR(50); GDCR_SHA256_SCR(51);
		GDCR_SHA256_SCR(52); GDCR_SHA256_SCR(53); GDCR_SHA256_SCR(54); GDCR_SHA256_SCR(55);
		GDCR_SHA256_SCR(56); GDCR_SHA256_SCR(57); GDCR_SHA256_SCR(58); GDCR_SHA256_SCR(59);
		GDCR_SHA256_SCR(60); GDCR_SHA256_SCR(61); GDCR_SHA256_SCR(62); GDCR_SHA256_SCR(63);

		wv[0] = context->Hash[0]; wv[1] = context->Hash[1];
		wv[2] = context->Hash[2]; wv[3] = context->Hash[3];
		wv[4] = context->Hash[4]; wv[5] = context->Hash[5];
		wv[6] = context->Hash[6]; wv[7] = context->Hash[7];

		GDCR_SHA256_EXP(0, 1, 2, 3, 4, 5, 6, 7, 0); GDCR_SHA256_EXP(7, 0, 1, 2, 3, 4, 5, 6, 1);
		GDCR_SHA256_EXP(6, 7, 0, 1, 2, 3, 4, 5, 2); GDCR_SHA256_EXP(5, 6, 7, 0, 1, 2, 3, 4, 3);
		GDCR_SHA256_EXP(4, 5, 6, 7, 0, 1, 2, 3, 4); GDCR_SHA256_EXP(3, 4, 5, 6, 7, 0, 1, 2, 5);
		GDCR_SHA256_EXP(2, 3, 4, 5, 6, 7, 0, 1, 6); GDCR_SHA256_EXP(1, 2, 3, 4, 5, 6, 7, 0, 7);
		GDCR_SHA256_EXP(0, 1, 2, 3, 4, 5, 6, 7, 8); GDCR_SHA256_EXP(7, 0, 1, 2, 3, 4, 5, 6, 9);
		GDCR_SHA256_EXP(6, 7, 0, 1, 2, 3, 4, 5, 10); GDCR_SHA256_EXP(5, 6, 7, 0, 1, 2, 3, 4, 11);
		GDCR_SHA256_EXP(4, 5, 6, 7, 0, 1, 2, 3, 12); GDCR_SHA256_EXP(3, 4, 5, 6, 7, 0, 1, 2, 13);
		GDCR_SHA256_EXP(2, 3, 4, 5, 6, 7, 0, 1, 14); GDCR_SHA256_EXP(1, 2, 3, 4, 5, 6, 7, 0, 15);
		GDCR_SHA256_EXP(0, 1, 2, 3, 4, 5, 6, 7, 16); GDCR_SHA256_EXP(7, 0, 1, 2, 3, 4, 5, 6, 17);
		GDCR_SHA256_EXP(6, 7, 0, 1, 2, 3, 4, 5, 18); GDCR_SHA256_EXP(5, 6, 7, 0, 1, 2, 3, 4, 19);
		GDCR_SHA256_EXP(4, 5, 6, 7, 0, 1, 2, 3, 20); GDCR_SHA256_EXP(3, 4, 5, 6, 7, 0, 1, 2, 21);
		GDCR_SHA256_EXP(2, 3, 4, 5, 6, 7, 0, 1, 22); GDCR_SHA256_EXP(1, 2, 3, 4, 5, 6, 7, 0, 23);
		GDCR_SHA256_EXP(0, 1, 2, 3, 4, 5, 6, 7, 24); GDCR_SHA256_EXP(7, 0, 1, 2, 3, 4, 5, 6, 25);
		GDCR_SHA256_EXP(6, 7, 0, 1, 2, 3, 4, 5, 26); GDCR_SHA256_EXP(5, 6, 7, 0, 1, 2, 3, 4, 27);
		GDCR_SHA256_EXP(4, 5, 6, 7, 0, 1, 2, 3, 28); GDCR_SHA256_EXP(3, 4, 5, 6, 7, 0, 1, 2, 29);
		GDCR_SHA256_EXP(2, 3, 4, 5, 6, 7, 0, 1, 30); GDCR_SHA256_EXP(1, 2, 3, 4, 5, 6, 7, 0, 31);
		GDCR_SHA256_EXP(0, 1, 2, 3, 4, 5, 6, 7, 32); GDCR_SHA256_EXP(7, 0, 1, 2, 3, 4, 5, 6, 33);
		GDCR_SHA256_EXP(6, 7, 0, 1, 2, 3, 4, 5, 34); GDCR_SHA256_EXP(5, 6, 7, 0, 1, 2, 3, 4, 35);
		GDCR_SHA256_EXP(4, 5, 6, 7, 0, 1, 2, 3, 36); GDCR_SHA256_EXP(3, 4, 5, 6, 7, 0, 1, 2, 37);
		GDCR_SHA256_EXP(2, 3, 4, 5, 6, 7, 0, 1, 38); GDCR_SHA256_EXP(1, 2, 3, 4, 5, 6, 7, 0, 39);
		GDCR_SHA256_EXP(0, 1, 2, 3, 4, 5, 6, 7, 40); GDCR_SHA256_EXP(7, 0, 1, 2, 3, 4, 5, 6, 41);
		GDCR_SHA256_EXP(6, 7, 0, 1, 2, 3, 4, 5, 42); GDCR_SHA256_EXP(5, 6, 7, 0, 1, 2, 3, 4, 43);
		GDCR_SHA256_EXP(4, 5, 6, 7, 0, 1, 2, 3, 44); GDCR_SHA256_EXP(3, 4, 5, 6, 7, 0, 1, 2, 45);
		GDCR_SHA256_EXP(2, 3, 4, 5, 6, 7, 0, 1, 46); GDCR_SHA256_EXP(1, 2, 3, 4, 5, 6, 7, 0, 47);
		GDCR_SHA256_EXP(0, 1, 2, 3, 4, 5, 6, 7, 48); GDCR_SHA256_EXP(7, 0, 1, 2, 3, 4, 5, 6, 49);
		GDCR_SHA256_EXP(6, 7, 0, 1, 2, 3, 4, 5, 50); GDCR_SHA256_EXP(5, 6, 7, 0, 1, 2, 3, 4, 51);
		GDCR_SHA256_EXP(4, 5, 6, 7, 0, 1, 2, 3, 52); GDCR_SHA256_EXP(3, 4, 5, 6, 7, 0, 1, 2, 53);
		GDCR_SHA256_EXP(2, 3, 4, 5, 6, 7, 0, 1, 54); GDCR_SHA256_EXP(1, 2, 3, 4, 5, 6, 7, 0, 55);
		GDCR_SHA256_EXP(0, 1, 2, 3, 4, 5, 6, 7, 56); GDCR_SHA256_EXP(7, 0, 1, 2, 3, 4, 5, 6, 57);
		GDCR_SHA256_EXP(6, 7, 0, 1, 2, 3, 4, 5, 58); GDCR_SHA256_EXP(5, 6, 7, 0, 1, 2, 3, 4, 59);
		GDCR_SHA256_EXP(4, 5, 6, 7, 0, 1, 2, 3, 60); GDCR_SHA256_EXP(3, 4, 5, 6, 7, 0, 1, 2, 61);
		GDCR_SHA256_EXP(2, 3, 4, 5, 6, 7, 0, 1, 62); GDCR_SHA256_EXP(1, 2, 3, 4, 5, 6, 7, 0, 63);

		context->Hash[0] += wv[0]; context->Hash[1] += wv[1];
		context->Hash[2] += wv[2]; context->Hash[3] += wv[3];
		context->Hash[4] += wv[4]; context->Hash[5] += wv[5];
		context->Hash[6] += wv[6]; context->Hash[7] += wv[7];
#endif 
	}
}


static void gdcr__sha256_init(gdcr_sha256_context *ctx)
{
#ifndef GDCR_SHA_UNROLL_LOOPS
	int i;
	for (i = 0; i < 8; i++) {
		ctx->Hash[i] = gdcr__sha256_init_hash[i];
	}
#else
	ctx->Hash[0] = gdcr__sha256_init_hash[0]; ctx->Hash[1] = gdcr__sha256_init_hash[1];
	ctx->Hash[2] = gdcr__sha256_init_hash[2]; ctx->Hash[3] = gdcr__sha256_init_hash[3];
	ctx->Hash[4] = gdcr__sha256_init_hash[4]; ctx->Hash[5] = gdcr__sha256_init_hash[5];
	ctx->Hash[6] = gdcr__sha256_init_hash[6]; ctx->Hash[7] = gdcr__sha256_init_hash[7];
#endif

	ctx->Len = 0;
	ctx->TotalLen = 0;
}

void gdcr__sha256_update(
	gdcr_sha256_context *ctx,
	const unsigned char *message,
	unsigned int len)
{
	unsigned int block_nb;
	unsigned int new_len, rem_len, tmp_len;
	const unsigned char *shifted_message;

	tmp_len = GDCR_SHA256_BLOCK_SIZE - ctx->Len;
	rem_len = len < tmp_len ? len : tmp_len;

	memcpy(&ctx->Block[ctx->Len], message, rem_len);

	if (ctx->Len + len < GDCR_SHA256_BLOCK_SIZE) {
		ctx->Len += len;
		return;
	}

	new_len = len - rem_len;
	block_nb = new_len / GDCR_SHA256_BLOCK_SIZE;

	shifted_message = message + rem_len;

	gdcr__sha256_transform(ctx, ctx->Block, 1);
	gdcr__sha256_transform(ctx, shifted_message, block_nb);

	rem_len = new_len % GDCR_SHA256_BLOCK_SIZE;

	memcpy(ctx->Block, &shifted_message[block_nb << 6],
		rem_len);

	ctx->Len = rem_len;
	ctx->TotalLen += (block_nb + 1) << 6;
}

void gdcr__sha256_final(gdcr_sha256_context *ctx, unsigned char *digest)
{
	unsigned int block_nb;
	unsigned int pm_len;
	unsigned int len_b;

#ifndef GDCR_SHA_UNROLL_LOOPS
	int i;
#endif

	block_nb = (1 + ((GDCR_SHA256_BLOCK_SIZE - 9)
		< (ctx->Len % GDCR_SHA256_BLOCK_SIZE)));

	len_b = (ctx->TotalLen + ctx->Len) << 3;
	pm_len = block_nb << 6;

	memset(ctx->Block + ctx->Len, 0, pm_len - ctx->Len);
	ctx->Block[ctx->Len] = 0x80;
	GDCR_UNPACK32(len_b, ctx->Block + pm_len - 4);

	gdcr__sha256_transform(ctx, ctx->Block, block_nb);

#ifndef GDCR_SHA_UNROLL_LOOPS
	for (i = 0; i < 8; i++) {
		GDCR_UNPACK32(ctx->Hash[i], &digest[i << 2]);
	}
#else
	GDCR_UNPACK32(ctx->Hash[0], &digest[0]);
	GDCR_UNPACK32(ctx->Hash[1], &digest[4]);
	GDCR_UNPACK32(ctx->Hash[2], &digest[8]);
	GDCR_UNPACK32(ctx->Hash[3], &digest[12]);
	GDCR_UNPACK32(ctx->Hash[4], &digest[16]);
	GDCR_UNPACK32(ctx->Hash[5], &digest[20]);
	GDCR_UNPACK32(ctx->Hash[6], &digest[24]);
	GDCR_UNPACK32(ctx->Hash[7], &digest[28]);
#endif
}


void gd_hash_sha256(const unsigned char *message, unsigned int len, unsigned char *digest)
{
	gdcr_sha256_context ctx;

	gdcr__sha256_init(&ctx);
	gdcr__sha256_update(&ctx, message, len);
	gdcr__sha256_final(&ctx, digest);
}

static void gdcr__sha512_transform(
	gdcr_sha512_context *ctx,
	const unsigned char *message,
	unsigned int block_nb)
{
	uint64 w[80];
	uint64 wv[8];
	uint64 t1, t2;
	const unsigned char *sub_block;
	int i, j;

	for (i = 0; i < (int)block_nb; i++) {
		sub_block = message + (i << 7);

#ifndef GDCR_SHA_UNROLL_LOOPS
		for (j = 0; j < 16; j++) {
			GDCR_PACK64(&sub_block[j << 3], &w[j]);
		}

		for (j = 16; j < 80; j++) {
			GDCR_SHA512_SCR(j);
		}

		for (j = 0; j < 8; j++) {
			wv[j] = ctx->Hash[j];
		}

		for (j = 0; j < 80; j++) {
			t1 = wv[7] + GDCR_SHA512_F2(wv[4]) + GDCR_SHA_CH(wv[4], wv[5], wv[6])
				+ gdcr__sha512_const_table[j] + w[j];
			t2 = GDCR_SHA512_F1(wv[0]) + GDCR_SHA_MAJ(wv[0], wv[1], wv[2]);
			wv[7] = wv[6];
			wv[6] = wv[5];
			wv[5] = wv[4];
			wv[4] = wv[3] + t1;
			wv[3] = wv[2];
			wv[2] = wv[1];
			wv[1] = wv[0];
			wv[0] = t1 + t2;
		}

		for (j = 0; j < 8; j++) {
			ctx->Hash[j] += wv[j];
		}
#else
		GDCR_PACK64(&sub_block[0], &w[0]); GDCR_PACK64(&sub_block[8], &w[1]);
		GDCR_PACK64(&sub_block[16], &w[2]); GDCR_PACK64(&sub_block[24], &w[3]);
		GDCR_PACK64(&sub_block[32], &w[4]); GDCR_PACK64(&sub_block[40], &w[5]);
		GDCR_PACK64(&sub_block[48], &w[6]); GDCR_PACK64(&sub_block[56], &w[7]);
		GDCR_PACK64(&sub_block[64], &w[8]); GDCR_PACK64(&sub_block[72], &w[9]);
		GDCR_PACK64(&sub_block[80], &w[10]); GDCR_PACK64(&sub_block[88], &w[11]);
		GDCR_PACK64(&sub_block[96], &w[12]); GDCR_PACK64(&sub_block[104], &w[13]);
		GDCR_PACK64(&sub_block[112], &w[14]); GDCR_PACK64(&sub_block[120], &w[15]);

		GDCR_SHA512_SCR(16); GDCR_SHA512_SCR(17); GDCR_SHA512_SCR(18); GDCR_SHA512_SCR(19);
		GDCR_SHA512_SCR(20); GDCR_SHA512_SCR(21); GDCR_SHA512_SCR(22); GDCR_SHA512_SCR(23);
		GDCR_SHA512_SCR(24); GDCR_SHA512_SCR(25); GDCR_SHA512_SCR(26); GDCR_SHA512_SCR(27);
		GDCR_SHA512_SCR(28); GDCR_SHA512_SCR(29); GDCR_SHA512_SCR(30); GDCR_SHA512_SCR(31);
		GDCR_SHA512_SCR(32); GDCR_SHA512_SCR(33); GDCR_SHA512_SCR(34); GDCR_SHA512_SCR(35);
		GDCR_SHA512_SCR(36); GDCR_SHA512_SCR(37); GDCR_SHA512_SCR(38); GDCR_SHA512_SCR(39);
		GDCR_SHA512_SCR(40); GDCR_SHA512_SCR(41); GDCR_SHA512_SCR(42); GDCR_SHA512_SCR(43);
		GDCR_SHA512_SCR(44); GDCR_SHA512_SCR(45); GDCR_SHA512_SCR(46); GDCR_SHA512_SCR(47);
		GDCR_SHA512_SCR(48); GDCR_SHA512_SCR(49); GDCR_SHA512_SCR(50); GDCR_SHA512_SCR(51);
		GDCR_SHA512_SCR(52); GDCR_SHA512_SCR(53); GDCR_SHA512_SCR(54); GDCR_SHA512_SCR(55);
		GDCR_SHA512_SCR(56); GDCR_SHA512_SCR(57); GDCR_SHA512_SCR(58); GDCR_SHA512_SCR(59);
		GDCR_SHA512_SCR(60); GDCR_SHA512_SCR(61); GDCR_SHA512_SCR(62); GDCR_SHA512_SCR(63);
		GDCR_SHA512_SCR(64); GDCR_SHA512_SCR(65); GDCR_SHA512_SCR(66); GDCR_SHA512_SCR(67);
		GDCR_SHA512_SCR(68); GDCR_SHA512_SCR(69); GDCR_SHA512_SCR(70); GDCR_SHA512_SCR(71);
		GDCR_SHA512_SCR(72); GDCR_SHA512_SCR(73); GDCR_SHA512_SCR(74); GDCR_SHA512_SCR(75);
		GDCR_SHA512_SCR(76); GDCR_SHA512_SCR(77); GDCR_SHA512_SCR(78); GDCR_SHA512_SCR(79);

		wv[0] = ctx->Hash[0]; wv[1] = ctx->Hash[1];
		wv[2] = ctx->Hash[2]; wv[3] = ctx->Hash[3];
		wv[4] = ctx->Hash[4]; wv[5] = ctx->Hash[5];
		wv[6] = ctx->Hash[6]; wv[7] = ctx->Hash[7];

		j = 0;

		do {
			GDCR_SHA512_EXP(0, 1, 2, 3, 4, 5, 6, 7, j); j++;
			GDCR_SHA512_EXP(7, 0, 1, 2, 3, 4, 5, 6, j); j++;
			GDCR_SHA512_EXP(6, 7, 0, 1, 2, 3, 4, 5, j); j++;
			GDCR_SHA512_EXP(5, 6, 7, 0, 1, 2, 3, 4, j); j++;
			GDCR_SHA512_EXP(4, 5, 6, 7, 0, 1, 2, 3, j); j++;
			GDCR_SHA512_EXP(3, 4, 5, 6, 7, 0, 1, 2, j); j++;
			GDCR_SHA512_EXP(2, 3, 4, 5, 6, 7, 0, 1, j); j++;
			GDCR_SHA512_EXP(1, 2, 3, 4, 5, 6, 7, 0, j); j++;
		} while (j < 80);

		ctx->Hash[0] += wv[0]; ctx->Hash[1] += wv[1];
		ctx->Hash[2] += wv[2]; ctx->Hash[3] += wv[3];
		ctx->Hash[4] += wv[4]; ctx->Hash[5] += wv[5];
		ctx->Hash[6] += wv[6]; ctx->Hash[7] += wv[7];
#endif
	}
}


static void gdcr__sha512_init(gdcr_sha512_context *ctx)
{
#ifndef GDCR_SHA_UNROLL_LOOPS
	int i;
	for (i = 0; i < 8; i++) {
		ctx->Hash[i] = gdcr__sha512_init_hash[i];
	}
#else
	ctx->Hash[0] = gdcr__sha512_init_hash[0]; ctx->Hash[1] = gdcr__sha512_init_hash[1];
	ctx->Hash[2] = gdcr__sha512_init_hash[2]; ctx->Hash[3] = gdcr__sha512_init_hash[3];
	ctx->Hash[4] = gdcr__sha512_init_hash[4]; ctx->Hash[5] = gdcr__sha512_init_hash[5];
	ctx->Hash[6] = gdcr__sha512_init_hash[6]; ctx->Hash[7] = gdcr__sha512_init_hash[7];
#endif

	ctx->Len = 0;
	ctx->TotalLen = 0;
}

static void gdcr__sha512_update(
	gdcr_sha512_context *ctx,
	const unsigned char *message,
	unsigned int len)
{
	unsigned int block_nb;
	unsigned int new_len, rem_len, tmp_len;
	const unsigned char *shifted_message;

	tmp_len = GDCR_SHA512_BLOCK_SIZE - ctx->Len;
	rem_len = len < tmp_len ? len : tmp_len;

	memcpy(&ctx->Block[ctx->Len], message, rem_len);

	if (ctx->Len + len < GDCR_SHA512_BLOCK_SIZE) {
		ctx->Len += len;
		return;
	}

	new_len = len - rem_len;
	block_nb = new_len / GDCR_SHA512_BLOCK_SIZE;

	shifted_message = message + rem_len;

	gdcr__sha512_transform(ctx, ctx->Block, 1);
	gdcr__sha512_transform(ctx, shifted_message, block_nb);

	rem_len = new_len % GDCR_SHA512_BLOCK_SIZE;

	memcpy(ctx->Block, &shifted_message[block_nb << 7],
		rem_len);

	ctx->Len = rem_len;
	ctx->TotalLen += (block_nb + 1) << 7;
}

static void gdcr__sha512_final(gdcr_sha512_context *ctx, unsigned char *digest)
{
	unsigned int block_nb;
	unsigned int pm_len;
	unsigned int len_b;

#ifndef GDCR_SHA_UNROLL_LOOPS
	int i;
#endif

	block_nb = 1 + ((GDCR_SHA512_BLOCK_SIZE - 17)
		< (ctx->Len % GDCR_SHA512_BLOCK_SIZE));

	len_b = (ctx->TotalLen + ctx->Len) << 3;
	pm_len = block_nb << 7;

	memset(ctx->Block + ctx->Len, 0, pm_len - ctx->Len);
	ctx->Block[ctx->Len] = 0x80;
	GDCR_UNPACK32(len_b, ctx->Block + pm_len - 4);

	gdcr__sha512_transform(ctx, ctx->Block, block_nb);

#ifndef GDCR_SHA_UNROLL_LOOPS
	for (i = 0; i < 8; i++) {
		GDCR_UNPACK64(ctx->Hash[i], &digest[i << 3]);
	}
#else
	GDCR_UNPACK64(ctx->Hash[0], &digest[0]);
	GDCR_UNPACK64(ctx->Hash[1], &digest[8]);
	GDCR_UNPACK64(ctx->Hash[2], &digest[16]);
	GDCR_UNPACK64(ctx->Hash[3], &digest[24]);
	GDCR_UNPACK64(ctx->Hash[4], &digest[32]);
	GDCR_UNPACK64(ctx->Hash[5], &digest[40]);
	GDCR_UNPACK64(ctx->Hash[6], &digest[48]);
	GDCR_UNPACK64(ctx->Hash[7], &digest[56]);
#endif
}

void gd_hash_sha512(
	const unsigned char *message,
	unsigned int len,
	unsigned char *digest)
{
	gdcr_sha512_context ctx;

	gdcr__sha512_init(&ctx);
	gdcr__sha512_update(&ctx, message, len);
	gdcr__sha512_final(&ctx, digest);
}


static void gdcr__sha384_init(gdcr_sha384_context *ctx)
{
#ifndef GDCR_SHA_UNROLL_LOOPS
	int i;
	for (i = 0; i < 8; i++) {
		ctx->Hash[i] = gdcr__sha384_init_hash[i];
	}
#else
	ctx->Hash[0] = gdcr__sha384_init_hash[0]; ctx->Hash[1] = gdcr__sha384_init_hash[1];
	ctx->Hash[2] = gdcr__sha384_init_hash[2]; ctx->Hash[3] = gdcr__sha384_init_hash[3];
	ctx->Hash[4] = gdcr__sha384_init_hash[4]; ctx->Hash[5] = gdcr__sha384_init_hash[5];
	ctx->Hash[6] = gdcr__sha384_init_hash[6]; ctx->Hash[7] = gdcr__sha384_init_hash[7];
#endif

	ctx->Len = 0;
	ctx->TotalLen = 0;
}

static void gdcr__sha384_update(
	gdcr_sha384_context *ctx,
	const unsigned char *message,
	unsigned int len)
{
	unsigned int block_nb;
	unsigned int new_len, rem_len, tmp_len;
	const unsigned char *shifted_message;

	tmp_len = GDCR_SHA384_BLOCK_SIZE - ctx->Len;
	rem_len = len < tmp_len ? len : tmp_len;

	memcpy(&ctx->Block[ctx->Len], message, rem_len);

	if (ctx->Len + len < GDCR_SHA384_BLOCK_SIZE) {
		ctx->Len += len;
		return;
	}

	new_len = len - rem_len;
	block_nb = new_len / GDCR_SHA384_BLOCK_SIZE;

	shifted_message = message + rem_len;

	gdcr__sha512_transform(ctx, ctx->Block, 1);
	gdcr__sha512_transform(ctx, shifted_message, block_nb);

	rem_len = new_len % GDCR_SHA384_BLOCK_SIZE;

	memcpy(ctx->Block, &shifted_message[block_nb << 7],
		rem_len);

	ctx->Len = rem_len;
	ctx->TotalLen += (block_nb + 1) << 7;
}

static void gdcr__sha384_final(gdcr_sha384_context *ctx, unsigned char *digest)
{
	unsigned int block_nb;
	unsigned int pm_len;
	unsigned int len_b;

#ifndef GDCR_SHA_UNROLL_LOOPS
	int i;
#endif

	block_nb = (1 + ((GDCR_SHA384_BLOCK_SIZE - 17)
		< (ctx->Len % GDCR_SHA384_BLOCK_SIZE)));

	len_b = (ctx->TotalLen + ctx->Len) << 3;
	pm_len = block_nb << 7;

	memset(ctx->Block + ctx->Len, 0, pm_len - ctx->Len);
	ctx->Block[ctx->Len] = 0x80;
	GDCR_UNPACK32(len_b, ctx->Block + pm_len - 4);

	gdcr__sha512_transform(ctx, ctx->Block, block_nb);

#ifndef GDCR_SHA_UNROLL_LOOPS
	for (i = 0; i < 6; i++) {
		GDCR_UNPACK64(ctx->Hash[i], &digest[i << 3]);
	}
#else
	GDCR_UNPACK64(ctx->Hash[0], &digest[0]);
	GDCR_UNPACK64(ctx->Hash[1], &digest[8]);
	GDCR_UNPACK64(ctx->Hash[2], &digest[16]);
	GDCR_UNPACK64(ctx->Hash[3], &digest[24]);
	GDCR_UNPACK64(ctx->Hash[4], &digest[32]);
	GDCR_UNPACK64(ctx->Hash[5], &digest[40]);
#endif
}

void gd_hash_sha384(
	const unsigned char *message,
	unsigned int len,
	unsigned char *digest)
{
	gdcr_sha384_context ctx;

	gdcr__sha384_init(&ctx);
	gdcr__sha384_update(&ctx, message, len);
	gdcr__sha384_final(&ctx, digest);
}


static void gdcr__sha224_init(gdcr_sha224_context *ctx)
{
#ifndef GDCR_SHA_UNROLL_LOOPS
	int i;
	for (i = 0; i < 8; i++) {
		ctx->Hash[i] = gdcr__sha224_init_hash[i];
	}
#else
	ctx->Hash[0] = gdcr__sha224_init_hash[0]; ctx->Hash[1] = gdcr__sha224_init_hash[1];
	ctx->Hash[2] = gdcr__sha224_init_hash[2]; ctx->Hash[3] = gdcr__sha224_init_hash[3];
	ctx->Hash[4] = gdcr__sha224_init_hash[4]; ctx->Hash[5] = gdcr__sha224_init_hash[5];
	ctx->Hash[6] = gdcr__sha224_init_hash[6]; ctx->Hash[7] = gdcr__sha224_init_hash[7];
#endif /* !UNROLL_LOOPS */

	ctx->Len = 0;
	ctx->TotalLen = 0;
}

static void gdcr__sha224_update(
	gdcr_sha224_context *ctx,
	const unsigned char *message,
	unsigned int len)
{
	unsigned int block_nb;
	unsigned int new_len, rem_len, tmp_len;
	const unsigned char *shifted_message;

	tmp_len = GDCR_SHA224_BLOCK_SIZE - ctx->Len;
	rem_len = len < tmp_len ? len : tmp_len;

	memcpy(&ctx->Block[ctx->Len], message, rem_len);

	if (ctx->Len + len < GDCR_SHA224_BLOCK_SIZE) {
		ctx->Len += len;
		return;
	}

	new_len = len - rem_len;
	block_nb = new_len / GDCR_SHA224_BLOCK_SIZE;

	shifted_message = message + rem_len;

	gdcr__sha256_transform(ctx, ctx->Block, 1);
	gdcr__sha256_transform(ctx, shifted_message, block_nb);

	rem_len = new_len % GDCR_SHA224_BLOCK_SIZE;

	memcpy(ctx->Block, &shifted_message[block_nb << 6],
		rem_len);

	ctx->Len = rem_len;
	ctx->TotalLen += (block_nb + 1) << 6;
}

static void gdcr__sha224_final(gdcr_sha224_context *ctx, unsigned char *digest)
{
	unsigned int block_nb;
	unsigned int pm_len;
	unsigned int len_b;

#ifndef GDCR_SHA_UNROLL_LOOPS
	int i;
#endif

	block_nb = (1 + ((GDCR_SHA224_BLOCK_SIZE - 9)
		< (ctx->Len % GDCR_SHA224_BLOCK_SIZE)));

	len_b = (ctx->TotalLen + ctx->Len) << 3;
	pm_len = block_nb << 6;

	memset(ctx->Block + ctx->Len, 0, pm_len - ctx->Len);
	ctx->Block[ctx->Len] = 0x80;
	GDCR_UNPACK32(len_b, ctx->Block + pm_len - 4);

	gdcr__sha256_transform(ctx, ctx->Block, block_nb);

#ifndef GDCR_SHA_UNROLL_LOOPS
	for (i = 0; i < 7; i++) {
		GDCR_UNPACK32(ctx->Hash[i], &digest[i << 2]);
	}
#else
	GDCR_UNPACK32(ctx->Hash[0], &digest[0]);
	GDCR_UNPACK32(ctx->Hash[1], &digest[4]);
	GDCR_UNPACK32(ctx->Hash[2], &digest[8]);
	GDCR_UNPACK32(ctx->Hash[3], &digest[12]);
	GDCR_UNPACK32(ctx->Hash[4], &digest[16]);
	GDCR_UNPACK32(ctx->Hash[5], &digest[20]);
	GDCR_UNPACK32(ctx->Hash[6], &digest[24]);
#endif
}

void gd_hash_sha224(
	const unsigned char *message,
	unsigned int len,
	unsigned char *digest)
{
	gdcr_sha224_context ctx;

	gdcr__sha224_init(&ctx);
	gdcr__sha224_update(&ctx, message, len);
	gdcr__sha224_final(&ctx, digest);
}


#endif