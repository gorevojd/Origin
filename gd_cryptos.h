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

		Result: Buffer now contains folowing bytes 8b1a9953c4611296a827abf8c47804d7
*/

#ifndef GD_CRYPTOS_HEADER

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
	unsigned char* Out)
{
	int Nk = -1;
	int Nr = -1;

	switch (KeyBytesCount){
		case 16: Nk = 4; Nr = 10; break;
		case 24: Nk = 6; Nr = 12; break;
		case 32: Nk = 8; Nr = 14; break;
	}

	/*Expanded key memory allocation*/
	unsigned char* w = (unsigned char*)malloc(GDCR_AES_NB * (Nr + 1) * 4);

	gdcr__aes_key_expansion(Key, w, Nr);

	gdcr__aes_cipher(Data, Out, w, Nr);

	free(w);
}

void gd_aes_decode(
	unsigned char* Data,
	unsigned char* Key, int KeyBytesCount,
	unsigned char* Out)
{
	int Nk = -1;
	int Nr = -1;

	switch (KeyBytesCount){
		case 16: Nk = 4; Nr = 10; break;
		case 24: Nk = 6; Nr = 12; break;
		case 32: Nk = 8; Nr = 14; break;
	}

	/*Expanded key memory allocation*/
	unsigned char* w = (unsigned char*)malloc(GDCR_AES_NB * (Nr + 1) * 4);

	gdcr__aes_key_expansion(Key, w, Nr);

	gdcr__aes_inv_cipher(Data, Out, w, Nr);

	free(w);
}


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

#endif