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
	3) Hashing algorithm
	4) Key expansion
	5) Key transformation
	6) ECB Encryption
	7) ECB Decryption
	8) CBC Encryption
	9) CBC Decryption
	10) Authentification code generation (MAC)
	
HO TO USE:
	If you want to use this library do the following: 
	define GD_BELT_IMPLEMENTATION and include this library in one cpp
	file like this:

	#define GD_CRYPTOS_IMPLEMENTATION
	#include "gd_belt.h"

	if you want to use it in multiple cpp files you need to define 
	#define GD_BELT_STATIC

EXAMPLES:
	

NOTES:
	ECB - Шифрование в режиме простой замены
	CBC - Шифрование в режиме сцепления блоков
	cfb - Шифрование в режиме гаммирования с обратной связью
	ctr - Шифрование в режиме счетчика
	mac - Алгоритм выработки имитовставки
	datawrap - алгоритмы одновременного шифрования и имитозащиты данных
	keywrap - алгоритмы одновременного шифрования и имитозащиты ключа
*/

#ifndef GD_BELT_H
#define GD_BELT_H

#ifndef GDBT_DEF
#ifdef GD_BELT_STATIC
#define GDBT_DEF static
#else
#define GDBT_DEF extern
#endif
#endif

#include <stdint.h>

#if 1
#define GD_BELT_UNPACK_U32(word, str)					\
	*((str) + 3) = (uint8_t)((word) >> 24);				\
	*((str) + 2) = (uint8_t)((word) >> 16);				\
	*((str) + 1) = (uint8_t)((word) >> 8);				\
	*((str) + 0) = (uint8_t)((word));					
#else
#define GD_BELT_UNPACK_U32(word, str)					\
	*(uint32_t*)(str) = (word);
#endif


#if 1
#define GD_BELT_PACK_U32(str, word)					\
	*(word) =											\
		((uint32_t)(*((str) + 3) << 24))	|			\
		((uint32_t)(*((str) + 2) << 16)) 	|			\
		((uint32_t)(*((str) + 1) << 8)) 	|			\
		((uint32_t)(*((str) + 0)));
#else
#define GD_BELT_PACK_U32(str, word)					\
	*(word) = *(uint32_t*)(str);
#endif

#define GD_BELT_ROTLEFT(word, amount)				\
	(((word) << (amount)) | ((word) >> ((sizeof(word) << 3) - (amount))))

#define GD_BELT_ROTRIGHT(word, amount)				\
	(((word) >> (amount)) | ((word) << ((sizeof(word) << 3) - (amount))))

#define GD_BELT_ROTLEFT32(word, amount)				\
	(((word) << (amount)) | ((word) >> (32 - (amount))))

#define GD_BELT_ROTRIGHT32(word, amount)				\
	(((word) >> (amount)) | ((word) << (32 - (amount))))

/*64 chunk macro*/
#define GD_BELT_CHUNK64_COPY(dest, src)								\
	((uint32_t*)(dest))[0] = ((const uint32_t*)(src))[0];			\
	((uint32_t*)(dest))[1] = ((const uint32_t*)(src))[1];			

/*96 chunk macro*/
#define GD_BELT_CHUNK96_COPY(dest, src)								\
	((uint32_t*)(dest))[0] = ((const uint32_t*)(src))[0];			\
	((uint32_t*)(dest))[1] = ((const uint32_t*)(src))[1];			\
	((uint32_t*)(dest))[2] = ((const uint32_t*)(src))[2];			

/*128 chunk macro*/
#define GD_BELT_CHUNK128_SET_ZERO(chunk)			\
	((uint32_t*)(chunk))[0] = 0;					\
	((uint32_t*)(chunk))[1] = 0;					\
	((uint32_t*)(chunk))[2] = 0;					\
	((uint32_t*)(chunk))[3] = 0;					

#define GD_BELT_CHUNK128_SET_ONE(chunk)				\
	((uint32_t*)(chunk))[0] = 0xFFFFFFFF;			\
	((uint32_t*)(chunk))[1] = 0xFFFFFFFF;			\
	((uint32_t*)(chunk))[2] = 0xFFFFFFFF;			\
	((uint32_t*)(chunk))[3] = 0xFFFFFFFF;					

#define GD_BELT_CHUNK128_COPY(dest, src)							\
	((uint32_t*)(dest))[0] = ((const uint32_t*)(src))[0];			\
	((uint32_t*)(dest))[1] = ((const uint32_t*)(src))[1];			\
	((uint32_t*)(dest))[2] = ((const uint32_t*)(src))[2];			\
	((uint32_t*)(dest))[3] = ((const uint32_t*)(src))[3];			

#define GD_BELT_CHUNK128_NEGATE(dest, src)							\
	((uint32_t*)(dest))[0] = ~((const uint32_t*)(src))[0]; 			\
	((uint32_t*)(dest))[1] = ~((const uint32_t*)(src))[1]; 			\
	((uint32_t*)(dest))[2] = ~((const uint32_t*)(src))[2]; 			\
	((uint32_t*)(dest))[3] = ~((const uint32_t*)(src))[3]; 			

#define GD_BELT_CHUNK128_XOR(dest, src1, src2)													\
	((uint32_t*)(dest))[0] = ((const uint32_t*)(src1))[0] ^ ((const uint32_t*)(src2))[0];		\
	((uint32_t*)(dest))[1] = ((const uint32_t*)(src1))[1] ^ ((const uint32_t*)(src2))[1];		\
	((uint32_t*)(dest))[2] = ((const uint32_t*)(src1))[2] ^ ((const uint32_t*)(src2))[2];		\
	((uint32_t*)(dest))[3] = ((const uint32_t*)(src1))[3] ^ ((const uint32_t*)(src2))[3];		

#define GD_BELT_CHUNK128_XOREQ(dest, src)							\
	((uint32_t*)(dest))[0] ^= ((const uint32_t*)(src))[0];			\
	((uint32_t*)(dest))[1] ^= ((const uint32_t*)(src))[1];			\
	((uint32_t*)(dest))[2] ^= ((const uint32_t*)(src))[2];			\
	((uint32_t*)(dest))[3] ^= ((const uint32_t*)(src))[3];			


/*256 chunk macro*/
#define GD_BELT_CHUNK256_SET_ZERO(chunk)			\
	((uint32_t*)(chunk))[0] = 0;					\
	((uint32_t*)(chunk))[1] = 0;					\
	((uint32_t*)(chunk))[2] = 0;					\
	((uint32_t*)(chunk))[3] = 0;					\
	((uint32_t*)(chunk))[4] = 0;					\
	((uint32_t*)(chunk))[5] = 0;					\
	((uint32_t*)(chunk))[6] = 0;					\
	((uint32_t*)(chunk))[7] = 0;					

#define GD_BELT_CHUNK256_SET_ONE(chunk)				\
	((uint32_t*)(chunk))[0] = 0xFFFFFFFF;			\
	((uint32_t*)(chunk))[1] = 0xFFFFFFFF;			\
	((uint32_t*)(chunk))[2] = 0xFFFFFFFF;			\
	((uint32_t*)(chunk))[3] = 0xFFFFFFFF;			\
	((uint32_t*)(chunk))[4] = 0xFFFFFFFF;			\
	((uint32_t*)(chunk))[5] = 0xFFFFFFFF;			\
	((uint32_t*)(chunk))[6] = 0xFFFFFFFF;			\
	((uint32_t*)(chunk))[7] = 0xFFFFFFFF;	

#define GD_BELT_CHUNK256_COPY(dest, src)							\
	((uint32_t*)(dest))[0] = ((const uint32_t*)(src))[0];			\
	((uint32_t*)(dest))[1] = ((const uint32_t*)(src))[1];			\
	((uint32_t*)(dest))[2] = ((const uint32_t*)(src))[2];			\
	((uint32_t*)(dest))[3] = ((const uint32_t*)(src))[3];			\
	((uint32_t*)(dest))[4] = ((const uint32_t*)(src))[4];			\
	((uint32_t*)(dest))[5] = ((const uint32_t*)(src))[5];			\
	((uint32_t*)(dest))[6] = ((const uint32_t*)(src))[6];			\
	((uint32_t*)(dest))[7] = ((const uint32_t*)(src))[7];			

#define GD_BELT_CHUNK256_NEGATE(dest, src)							\
	((uint32_t*)(dest))[0] = ~((const uint32_t*)(src))[0]; 			\
	((uint32_t*)(dest))[1] = ~((const uint32_t*)(src))[1]; 			\
	((uint32_t*)(dest))[2] = ~((const uint32_t*)(src))[2]; 			\
	((uint32_t*)(dest))[3] = ~((const uint32_t*)(src))[3]; 			\
	((uint32_t*)(dest))[4] = ~((const uint32_t*)(src))[4]; 			\
	((uint32_t*)(dest))[5] = ~((const uint32_t*)(src))[5]; 			\
	((uint32_t*)(dest))[6] = ~((const uint32_t*)(src))[6]; 			\
	((uint32_t*)(dest))[7] = ~((const uint32_t*)(src))[7]; 	

#define GD_BELT_CHUNK256_XOR(dest, src1, src2)													\
	((uint32_t*)(dest))[0] = ((const uint32_t*)(src1))[0] ^ ((const uint32_t*)(src2))[0];		\
	((uint32_t*)(dest))[1] = ((const uint32_t*)(src1))[1] ^ ((const uint32_t*)(src2))[1];		\
	((uint32_t*)(dest))[2] = ((const uint32_t*)(src1))[2] ^ ((const uint32_t*)(src2))[2];		\
	((uint32_t*)(dest))[3] = ((const uint32_t*)(src1))[3] ^ ((const uint32_t*)(src2))[3];		\
	((uint32_t*)(dest))[4] = ((const uint32_t*)(src1))[4] ^ ((const uint32_t*)(src2))[4];		\
	((uint32_t*)(dest))[5] = ((const uint32_t*)(src1))[5] ^ ((const uint32_t*)(src2))[5];		\
	((uint32_t*)(dest))[6] = ((const uint32_t*)(src1))[6] ^ ((const uint32_t*)(src2))[6];		\
	((uint32_t*)(dest))[7] = ((const uint32_t*)(src1))[7] ^ ((const uint32_t*)(src2))[7];

#define GD_BELT_CHUNK256_XOREQ(dest, src)							\
	((uint32_t*)(dest))[0] ^= ((const uint32_t*)(src))[0];			\
	((uint32_t*)(dest))[1] ^= ((const uint32_t*)(src))[1];			\
	((uint32_t*)(dest))[2] ^= ((const uint32_t*)(src))[2];			\
	((uint32_t*)(dest))[3] ^= ((const uint32_t*)(src))[3];			\
	((uint32_t*)(dest))[4] ^= ((const uint32_t*)(src))[4];			\
	((uint32_t*)(dest))[5] ^= ((const uint32_t*)(src))[5];			\
	((uint32_t*)(dest))[6] ^= ((const uint32_t*)(src))[6];			\
	((uint32_t*)(dest))[7] ^= ((const uint32_t*)(src))[7];

/*512 chunk macro*/
#define GD_BELT_CHUNK512_SET_ZERO(chunk)			\
	((uint32_t*)(chunk))[0] = 0;					\
	((uint32_t*)(chunk))[1] = 0;					\
	((uint32_t*)(chunk))[2] = 0;					\
	((uint32_t*)(chunk))[3] = 0;					\
	((uint32_t*)(chunk))[4] = 0;					\
	((uint32_t*)(chunk))[5] = 0;					\
	((uint32_t*)(chunk))[6] = 0;					\
	((uint32_t*)(chunk))[7] = 0;					\
	((uint32_t*)(chunk))[8] = 0;					\
	((uint32_t*)(chunk))[9] = 0;					\
	((uint32_t*)(chunk))[10] = 0;					\
	((uint32_t*)(chunk))[11] = 0;					\
	((uint32_t*)(chunk))[12] = 0;					\
	((uint32_t*)(chunk))[13] = 0;					\
	((uint32_t*)(chunk))[14] = 0;					\
	((uint32_t*)(chunk))[15] = 0;

#define GD_BELT_HASH_DIGEST_BYTE_SIZE 32

#ifdef __cplusplus
extern "C" {
#endif

GDBT_DEF void gd_belt_ecb_encrypt(const uint8_t* bytes, int bytes_count, const uint8_t theta[32], uint8_t* dest);
GDBT_DEF void gd_belt_ecb_decrypt(const uint8_t* bytes, int bytes_count, const uint8_t theta[32], uint8_t* dest);

GDBT_DEF void gd_belt_cbc_encrypt(const uint8_t* bytes, int bytes_count, const uint8_t theta[32], const uint8_t s[16], uint8_t* dest);
GDBT_DEF void gd_belt_cbc_decrypt(const uint8_t* bytes, int bytes_count, const uint8_t theta[32], const uint8_t s[16], uint8_t* dest);

GDBT_DEF void gd_belt_cfb_encrypt(const uint8_t* bytes, int bytes_count, const uint8_t theta[32], const uint8_t s[16], uint8_t* dest);
GDBT_DEF void gd_belt_cfb_encrypt(const uint8_t* bytes, int bytes_count, const uint8_t theta[32], const uint8_t s[16], uint8_t* dest);

GDBT_DEF void gd_belt_ctr_encrypt(const uint8_t* bytes, int bytes_count, const uint8_t theta[32], const uint8_t s[16], uint8_t* dest);
GDBT_DEF void gd_belt_ctr_decrypt(const uint8_t* bytes, int bytes_count, const uint8_t theta[32], const uint8_t s[16], uint8_t* dest);

GDBT_DEF void gd_belt_mac(const uint8_t* bytes, int bytes_count, const uint8_t theta[32], uint8_t dest[8]);

GDBT_DEF void gd_belt_hash256(const uint8_t* bytes, int bytes_count, uint8_t dest[32]);

#ifdef __cplusplus
}
#endif

#endif


#if defined(GD_BELT_IMPLEMENTATION) && !defined(GD_BELT_IMPLEMENTATION_DONE)
#define GD_BELT_IMPLEMENTATION_DONE

static void gd_belt_add_one_mod128(uint8_t dest[16]){
	uint32_t s_words[4];
	
	GD_BELT_CHUNK128_COPY(s_words, dest);

	/*Do something about this hell*/
	if (s_words[0] != 0xFFFFFFFF){
		s_words[0] += 1;
	}
	else{
		if (s_words[1] != 0xFFFFFFFF){
			s_words[1] += 1;
		}
		else{
			if (s_words[2] != 0xFFFFFFFF){
				s_words[2] += 1;
			}
			else{
				if (s_words[3] != 0xFFFFFFFF){
					s_words[3] += 1;
				}
				else{
					s_words[3] = 0x00000000;
				}

				s_words[2] = 0x00000000;
			}

			s_words[1] = 0x00000000;
		}

		s_words[0] = 0x00000000;
	}

	GD_BELT_CHUNK128_COPY(dest, s_words);
}

static const uint8_t gd__belt_H[] = {
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

static const uint32_t gd__belt_H5[] = {
	0x00001620, 0x00001280, 0x00001740, 0x00001900, 0x00000140, 0x00000100, 0x00001EA0, 0x00000760,
	0x000006C0, 0x00000DA0, 0x00000000, 0x000011C0, 0x00000B00, 0x00000940, 0x00000BA0, 0x00001C80,
	0x000010A0, 0x00000080, 0x00001F40, 0x000013A0, 0x00000360, 0x000016C0, 0x000018E0, 0x00001580,
	0x000004A0, 0x000005C0, 0x00000E40, 0x00001840, 0x00000040, 0x00001FA0, 0x000019C0, 0x000001A0,
	0x00000B60, 0x00001C60, 0x00001AC0, 0x00000240, 0x000002E0, 0x00001720, 0x00000C20, 0x00001020,
	0x00001FC0, 0x00000CE0, 0x000010C0, 0x000015A0, 0x00000E20, 0x00000D60, 0x00001120, 0x00000160,
	0x00000B80, 0x00001600, 0x00001800, 0x00001FE0, 0x00000660, 0x00001860, 0x00000AC0, 0x00001700,
	0x000006A0, 0x00001880, 0x000000A0, 0x000015C0, 0x00001B00, 0x00001C00, 0x00000FE0, 0x00001320,
	0x00001C20, 0x00000560, 0x00001B80, 0x00000340, 0x00001C40, 0x00001040, 0x00000AE0, 0x00001D80,
	0x00000E00, 0x000007E0, 0x00001980, 0x00001E00, 0x000012A0, 0x00001DC0, 0x000011A0, 0x00001E20,
	0x00001820, 0x00001560, 0x00000EC0, 0x00000700, 0x000013E0, 0x00001CC0, 0x00000F00, 0x00001940,
	0x00001EE0, 0x000018C0, 0x00001F00, 0x00000C00, 0x00001AA0, 0x00001760, 0x00001380, 0x000009E0,
	0x00001E60, 0x00000780, 0x00000CA0, 0x00000F60, 0x00000C60, 0x00000F80, 0x00000600, 0x00000D40,
	0x00001BA0, 0x000009C0, 0x000014E0, 0x00000F20, 0x000013C0, 0x00001640, 0x000007A0, 0x00000620,
	0x000007C0, 0x00001300, 0x000016A0, 0x00000DC0, 0x000004E0, 0x00001A60, 0x00001780, 0x000019E0,
	0x00000B20, 0x000003C0, 0x00000300, 0x000003E0, 0x00000980, 0x00000B40, 0x000016E0, 0x00001260,
	0x00001D20, 0x00001BC0, 0x00001CE0, 0x00000580, 0x000011E0, 0x00000180, 0x000001E0, 0x000014C0,
	0x000005A0, 0x00001B60, 0x00000920, 0x00001E80, 0x00000DE0, 0x00000E60, 0x000012C0, 0x000008E0,
	0x000000C0, 0x000000E0, 0x00000A60, 0x000002C0, 0x00001DA0, 0x00000480, 0x00000F40, 0x000006E0,
	0x00000720, 0x00001960, 0x00001460, 0x00001060, 0x00000060, 0x00001520, 0x00001160, 0x00001EC0,
	0x00001240, 0x000017A0, 0x00001360, 0x00000380, 0x00001CA0, 0x00001A20, 0x00000820, 0x00000020,
	0x00000A80, 0x000008A0, 0x00001F60, 0x00001920, 0x00000BC0, 0x000009A0, 0x000001C0, 0x00001E40,
	0x00000D00, 0x00000400, 0x00001000, 0x00001540, 0x00000440, 0x00000FA0, 0x00000C80, 0x000005E0,
	0x000004C0, 0x000010E0, 0x00001F20, 0x00000680, 0x00001200, 0x00000800, 0x00000AA0, 0x00000220,
	0x000017C0, 0x00000640, 0x000012E0, 0x00000260, 0x00000860, 0x00001F80, 0x00001340, 0x00000900,
	0x00001400, 0x00000540, 0x00001100, 0x00000BE0, 0x00000320, 0x00000960, 0x00000120, 0x00001420,
	0x00000FC0, 0x000019A0, 0x00001480, 0x00001A00, 0x000002A0, 0x00000880, 0x000015E0, 0x00001180,
	0x000014A0, 0x00001080, 0x00000A00, 0x000017E0, 0x00000CC0, 0x00001A40, 0x00001D00, 0x00001140,
	0x00001440, 0x00001AE0, 0x000008C0, 0x00000A40, 0x00000840, 0x00001500, 0x00001BE0, 0x00001660,
	0x00000D20, 0x00000E80, 0x000018A0, 0x00000A20, 0x00001D60, 0x00000460, 0x00000520, 0x00000420,
	0x00001A80, 0x00001DE0, 0x00001B20, 0x00001680, 0x00000740, 0x00000C40, 0x00000500, 0x00000EA0,
	0x00001220, 0x00000280, 0x00000200, 0x00001D40, 0x00000EE0, 0x00000D80, 0x00001B40, 0x000003A0,
};

static const uint32_t gd__belt_H13[] = {
	0x00162000, 0x00128000, 0x00174000, 0x00190000, 0x00014000, 0x00010000, 0x001EA000, 0x00076000,
	0x0006C000, 0x000DA000, 0x00000000, 0x0011C000, 0x000B0000, 0x00094000, 0x000BA000, 0x001C8000,
	0x0010A000, 0x00008000, 0x001F4000, 0x0013A000, 0x00036000, 0x0016C000, 0x0018E000, 0x00158000,
	0x0004A000, 0x0005C000, 0x000E4000, 0x00184000, 0x00004000, 0x001FA000, 0x0019C000, 0x0001A000,
	0x000B6000, 0x001C6000, 0x001AC000, 0x00024000, 0x0002E000, 0x00172000, 0x000C2000, 0x00102000,
	0x001FC000, 0x000CE000, 0x0010C000, 0x0015A000, 0x000E2000, 0x000D6000, 0x00112000, 0x00016000,
	0x000B8000, 0x00160000, 0x00180000, 0x001FE000, 0x00066000, 0x00186000, 0x000AC000, 0x00170000,
	0x0006A000, 0x00188000, 0x0000A000, 0x0015C000, 0x001B0000, 0x001C0000, 0x000FE000, 0x00132000,
	0x001C2000, 0x00056000, 0x001B8000, 0x00034000, 0x001C4000, 0x00104000, 0x000AE000, 0x001D8000,
	0x000E0000, 0x0007E000, 0x00198000, 0x001E0000, 0x0012A000, 0x001DC000, 0x0011A000, 0x001E2000,
	0x00182000, 0x00156000, 0x000EC000, 0x00070000, 0x0013E000, 0x001CC000, 0x000F0000, 0x00194000,
	0x001EE000, 0x0018C000, 0x001F0000, 0x000C0000, 0x001AA000, 0x00176000, 0x00138000, 0x0009E000,
	0x001E6000, 0x00078000, 0x000CA000, 0x000F6000, 0x000C6000, 0x000F8000, 0x00060000, 0x000D4000,
	0x001BA000, 0x0009C000, 0x0014E000, 0x000F2000, 0x0013C000, 0x00164000, 0x0007A000, 0x00062000,
	0x0007C000, 0x00130000, 0x0016A000, 0x000DC000, 0x0004E000, 0x001A6000, 0x00178000, 0x0019E000,
	0x000B2000, 0x0003C000, 0x00030000, 0x0003E000, 0x00098000, 0x000B4000, 0x0016E000, 0x00126000,
	0x001D2000, 0x001BC000, 0x001CE000, 0x00058000, 0x0011E000, 0x00018000, 0x0001E000, 0x0014C000,
	0x0005A000, 0x001B6000, 0x00092000, 0x001E8000, 0x000DE000, 0x000E6000, 0x0012C000, 0x0008E000,
	0x0000C000, 0x0000E000, 0x000A6000, 0x0002C000, 0x001DA000, 0x00048000, 0x000F4000, 0x0006E000,
	0x00072000, 0x00196000, 0x00146000, 0x00106000, 0x00006000, 0x00152000, 0x00116000, 0x001EC000,
	0x00124000, 0x0017A000, 0x00136000, 0x00038000, 0x001CA000, 0x001A2000, 0x00082000, 0x00002000,
	0x000A8000, 0x0008A000, 0x001F6000, 0x00192000, 0x000BC000, 0x0009A000, 0x0001C000, 0x001E4000,
	0x000D0000, 0x00040000, 0x00100000, 0x00154000, 0x00044000, 0x000FA000, 0x000C8000, 0x0005E000,
	0x0004C000, 0x0010E000, 0x001F2000, 0x00068000, 0x00120000, 0x00080000, 0x000AA000, 0x00022000,
	0x0017C000, 0x00064000, 0x0012E000, 0x00026000, 0x00086000, 0x001F8000, 0x00134000, 0x00090000,
	0x00140000, 0x00054000, 0x00110000, 0x000BE000, 0x00032000, 0x00096000, 0x00012000, 0x00142000,
	0x000FC000, 0x0019A000, 0x00148000, 0x001A0000, 0x0002A000, 0x00088000, 0x0015E000, 0x00118000,
	0x0014A000, 0x00108000, 0x000A0000, 0x0017E000, 0x000CC000, 0x001A4000, 0x001D0000, 0x00114000,
	0x00144000, 0x001AE000, 0x0008C000, 0x000A4000, 0x00084000, 0x00150000, 0x001BE000, 0x00166000,
	0x000D2000, 0x000E8000, 0x0018A000, 0x000A2000, 0x001D6000, 0x00046000, 0x00052000, 0x00042000,
	0x001A8000, 0x001DE000, 0x001B2000, 0x00168000, 0x00074000, 0x000C4000, 0x00050000, 0x000EA000,
	0x00122000, 0x00028000, 0x00020000, 0x001D4000, 0x000EE000, 0x000D8000, 0x001B4000, 0x0003A000,
};

static const uint32_t gd__belt_H21[] = {
	0x16200000, 0x12800000, 0x17400000, 0x19000000, 0x01400000, 0x01000000, 0x1EA00000, 0x07600000,
	0x06C00000, 0x0DA00000, 0x00000000, 0x11C00000, 0x0B000000, 0x09400000, 0x0BA00000, 0x1C800000,
	0x10A00000, 0x00800000, 0x1F400000, 0x13A00000, 0x03600000, 0x16C00000, 0x18E00000, 0x15800000,
	0x04A00000, 0x05C00000, 0x0E400000, 0x18400000, 0x00400000, 0x1FA00000, 0x19C00000, 0x01A00000,
	0x0B600000, 0x1C600000, 0x1AC00000, 0x02400000, 0x02E00000, 0x17200000, 0x0C200000, 0x10200000,
	0x1FC00000, 0x0CE00000, 0x10C00000, 0x15A00000, 0x0E200000, 0x0D600000, 0x11200000, 0x01600000,
	0x0B800000, 0x16000000, 0x18000000, 0x1FE00000, 0x06600000, 0x18600000, 0x0AC00000, 0x17000000,
	0x06A00000, 0x18800000, 0x00A00000, 0x15C00000, 0x1B000000, 0x1C000000, 0x0FE00000, 0x13200000,
	0x1C200000, 0x05600000, 0x1B800000, 0x03400000, 0x1C400000, 0x10400000, 0x0AE00000, 0x1D800000,
	0x0E000000, 0x07E00000, 0x19800000, 0x1E000000, 0x12A00000, 0x1DC00000, 0x11A00000, 0x1E200000,
	0x18200000, 0x15600000, 0x0EC00000, 0x07000000, 0x13E00000, 0x1CC00000, 0x0F000000, 0x19400000,
	0x1EE00000, 0x18C00000, 0x1F000000, 0x0C000000, 0x1AA00000, 0x17600000, 0x13800000, 0x09E00000,
	0x1E600000, 0x07800000, 0x0CA00000, 0x0F600000, 0x0C600000, 0x0F800000, 0x06000000, 0x0D400000,
	0x1BA00000, 0x09C00000, 0x14E00000, 0x0F200000, 0x13C00000, 0x16400000, 0x07A00000, 0x06200000,
	0x07C00000, 0x13000000, 0x16A00000, 0x0DC00000, 0x04E00000, 0x1A600000, 0x17800000, 0x19E00000,
	0x0B200000, 0x03C00000, 0x03000000, 0x03E00000, 0x09800000, 0x0B400000, 0x16E00000, 0x12600000,
	0x1D200000, 0x1BC00000, 0x1CE00000, 0x05800000, 0x11E00000, 0x01800000, 0x01E00000, 0x14C00000,
	0x05A00000, 0x1B600000, 0x09200000, 0x1E800000, 0x0DE00000, 0x0E600000, 0x12C00000, 0x08E00000,
	0x00C00000, 0x00E00000, 0x0A600000, 0x02C00000, 0x1DA00000, 0x04800000, 0x0F400000, 0x06E00000,
	0x07200000, 0x19600000, 0x14600000, 0x10600000, 0x00600000, 0x15200000, 0x11600000, 0x1EC00000,
	0x12400000, 0x17A00000, 0x13600000, 0x03800000, 0x1CA00000, 0x1A200000, 0x08200000, 0x00200000,
	0x0A800000, 0x08A00000, 0x1F600000, 0x19200000, 0x0BC00000, 0x09A00000, 0x01C00000, 0x1E400000,
	0x0D000000, 0x04000000, 0x10000000, 0x15400000, 0x04400000, 0x0FA00000, 0x0C800000, 0x05E00000,
	0x04C00000, 0x10E00000, 0x1F200000, 0x06800000, 0x12000000, 0x08000000, 0x0AA00000, 0x02200000,
	0x17C00000, 0x06400000, 0x12E00000, 0x02600000, 0x08600000, 0x1F800000, 0x13400000, 0x09000000,
	0x14000000, 0x05400000, 0x11000000, 0x0BE00000, 0x03200000, 0x09600000, 0x01200000, 0x14200000,
	0x0FC00000, 0x19A00000, 0x14800000, 0x1A000000, 0x02A00000, 0x08800000, 0x15E00000, 0x11800000,
	0x14A00000, 0x10800000, 0x0A000000, 0x17E00000, 0x0CC00000, 0x1A400000, 0x1D000000, 0x11400000,
	0x14400000, 0x1AE00000, 0x08C00000, 0x0A400000, 0x08400000, 0x15000000, 0x1BE00000, 0x16600000,
	0x0D200000, 0x0E800000, 0x18A00000, 0x0A200000, 0x1D600000, 0x04600000, 0x05200000, 0x04200000,
	0x1A800000, 0x1DE00000, 0x1B200000, 0x16800000, 0x07400000, 0x0C400000, 0x05000000, 0x0EA00000,
	0x12200000, 0x02800000, 0x02000000, 0x1D400000, 0x0EE00000, 0x0D800000, 0x1B400000, 0x03A00000,
};

static const uint32_t gd__belt_H29[] = {
	0x20000016, 0x80000012, 0x40000017, 0x00000019, 0x40000001, 0x00000001, 0xA000001E, 0x60000007,
	0xC0000006, 0xA000000D, 0x00000000, 0xC0000011, 0x0000000B, 0x40000009, 0xA000000B, 0x8000001C,
	0xA0000010, 0x80000000, 0x4000001F, 0xA0000013, 0x60000003, 0xC0000016, 0xE0000018, 0x80000015,
	0xA0000004, 0xC0000005, 0x4000000E, 0x40000018, 0x40000000, 0xA000001F, 0xC0000019, 0xA0000001,
	0x6000000B, 0x6000001C, 0xC000001A, 0x40000002, 0xE0000002, 0x20000017, 0x2000000C, 0x20000010,
	0xC000001F, 0xE000000C, 0xC0000010, 0xA0000015, 0x2000000E, 0x6000000D, 0x20000011, 0x60000001,
	0x8000000B, 0x00000016, 0x00000018, 0xE000001F, 0x60000006, 0x60000018, 0xC000000A, 0x00000017,
	0xA0000006, 0x80000018, 0xA0000000, 0xC0000015, 0x0000001B, 0x0000001C, 0xE000000F, 0x20000013,
	0x2000001C, 0x60000005, 0x8000001B, 0x40000003, 0x4000001C, 0x40000010, 0xE000000A, 0x8000001D,
	0x0000000E, 0xE0000007, 0x80000019, 0x0000001E, 0xA0000012, 0xC000001D, 0xA0000011, 0x2000001E,
	0x20000018, 0x60000015, 0xC000000E, 0x00000007, 0xE0000013, 0xC000001C, 0x0000000F, 0x40000019,
	0xE000001E, 0xC0000018, 0x0000001F, 0x0000000C, 0xA000001A, 0x60000017, 0x80000013, 0xE0000009,
	0x6000001E, 0x80000007, 0xA000000C, 0x6000000F, 0x6000000C, 0x8000000F, 0x00000006, 0x4000000D,
	0xA000001B, 0xC0000009, 0xE0000014, 0x2000000F, 0xC0000013, 0x40000016, 0xA0000007, 0x20000006,
	0xC0000007, 0x00000013, 0xA0000016, 0xC000000D, 0xE0000004, 0x6000001A, 0x80000017, 0xE0000019,
	0x2000000B, 0xC0000003, 0x00000003, 0xE0000003, 0x80000009, 0x4000000B, 0xE0000016, 0x60000012,
	0x2000001D, 0xC000001B, 0xE000001C, 0x80000005, 0xE0000011, 0x80000001, 0xE0000001, 0xC0000014,
	0xA0000005, 0x6000001B, 0x20000009, 0x8000001E, 0xE000000D, 0x6000000E, 0xC0000012, 0xE0000008,
	0xC0000000, 0xE0000000, 0x6000000A, 0xC0000002, 0xA000001D, 0x80000004, 0x4000000F, 0xE0000006,
	0x20000007, 0x60000019, 0x60000014, 0x60000010, 0x60000000, 0x20000015, 0x60000011, 0xC000001E,
	0x40000012, 0xA0000017, 0x60000013, 0x80000003, 0xA000001C, 0x2000001A, 0x20000008, 0x20000000,
	0x8000000A, 0xA0000008, 0x6000001F, 0x20000019, 0xC000000B, 0xA0000009, 0xC0000001, 0x4000001E,
	0x0000000D, 0x00000004, 0x00000010, 0x40000015, 0x40000004, 0xA000000F, 0x8000000C, 0xE0000005,
	0xC0000004, 0xE0000010, 0x2000001F, 0x80000006, 0x00000012, 0x00000008, 0xA000000A, 0x20000002,
	0xC0000017, 0x40000006, 0xE0000012, 0x60000002, 0x60000008, 0x8000001F, 0x40000013, 0x00000009,
	0x00000014, 0x40000005, 0x00000011, 0xE000000B, 0x20000003, 0x60000009, 0x20000001, 0x20000014,
	0xC000000F, 0xA0000019, 0x80000014, 0x0000001A, 0xA0000002, 0x80000008, 0xE0000015, 0x80000011,
	0xA0000014, 0x80000010, 0x0000000A, 0xE0000017, 0xC000000C, 0x4000001A, 0x0000001D, 0x40000011,
	0x40000014, 0xE000001A, 0xC0000008, 0x4000000A, 0x40000008, 0x00000015, 0xE000001B, 0x60000016,
	0x2000000D, 0x8000000E, 0xA0000018, 0x2000000A, 0x6000001D, 0x60000004, 0x20000005, 0x20000004,
	0x8000001A, 0xE000001D, 0x2000001B, 0x80000016, 0x40000007, 0x4000000C, 0x00000005, 0xA000000E,
	0x20000012, 0x80000002, 0x00000002, 0x4000001D, 0xE000000E, 0x8000000D, 0x4000001B, 0xA0000003,
};

static uint32_t gd_belt_g(uint32_t u, int r){
	uint32_t res = 0;

#if 1
	uint8_t u1 = (u >> 24) & 0xFF;
	uint8_t u2 = (u >> 16) & 0xFF;
	uint8_t u3 = (u >> 8) & 0xFF;
	uint8_t u4 = (u) & 0xFF;
#else
	uint32_t u1 = (u >> 24) & 0xFF;
	uint32_t u2 = (u >> 16) & 0xFF;
	uint32_t u3 = (u >> 8) & 0xFF;
	uint32_t u4 = (u) & 0xFF;
#endif

	uint32_t to_rot =
		((uint32_t)gd__belt_H[u1] << 24) |
		((uint32_t)gd__belt_H[u2] << 16) |
		((uint32_t)gd__belt_H[u3] << 8) |
		((uint32_t)gd__belt_H[u4]);

	res = GD_BELT_ROTLEFT32(to_rot, r);

	return(res);
}

#define GD_BELT_G5(u)					\
		(gd__belt_H5[u & 0xFF] |		\
		gd__belt_H13[u >> 8 & 0xFF] |	\
		gd__belt_H21[u >> 16 & 0xFF] |	\
		gd__belt_H29[u >> 24 & 0xFF])		

#define GD_BELT_G13(u)					\
		(gd__belt_H13[u & 0xFF] |		\
		gd__belt_H21[u >> 8 & 0xFF] |	\
		gd__belt_H29[u >> 16 & 0xFF] |	\
		gd__belt_H5[u >> 24 & 0xFF])		

#define GD_BELT_G21(u)					\
		(gd__belt_H21[u & 0xFF] |		\
		gd__belt_H29[u >> 8 & 0xFF] |	\
		gd__belt_H5[u >> 16 & 0xFF] |	\
		gd__belt_H13[u >> 24 & 0xFF])		


#if 0
static uint32_t gd_belt_g5(uint32_t u){
	uint32_t res = 0;

#if 1
	uint8_t u1 = (u >> 24) & 0xFF;
	uint8_t u2 = (u >> 16) & 0xFF;
	uint8_t u3 = (u >> 8) & 0xFF;
	uint8_t u4 = (u)& 0xFF;
#else 
	uint8_t u1 = (uint8_t)(u >> 24);
	uint8_t u2 = (uint8_t)(u >> 16);
	uint8_t u3 = (uint8_t)(u >> 8);
	uint8_t u4 = (uint8_t)(u);
#endif

	res =
		gd__belt_H5[u4] |
		gd__belt_H13[u3] |
		gd__belt_H21[u2] |
		gd__belt_H29[u1];

	return(res);
}

static uint32_t gd_belt_g13(uint32_t u){
	uint32_t res = 0;

#if 1
	uint8_t u1 = (u >> 24) & 0xFF;
	uint8_t u2 = (u >> 16) & 0xFF;
	uint8_t u3 = (u >> 8) & 0xFF;
	uint8_t u4 = (u)& 0xFF;
#else 
	uint8_t u1 = (uint8_t)(u >> 24);
	uint8_t u2 = (uint8_t)(u >> 16);
	uint8_t u3 = (uint8_t)(u >> 8);
	uint8_t u4 = (uint8_t)(u);
#endif

	res =
		gd__belt_H13[u4] |
		gd__belt_H21[u3] |
		gd__belt_H29[u2] |
		gd__belt_H5[u1];

	return(res);
}

static uint32_t gd_belt_g21(uint32_t u){
	uint32_t res = 0;

#if 1
	uint8_t u1 = (u >> 24) & 0xFF;
	uint8_t u2 = (u >> 16) & 0xFF;
	uint8_t u3 = (u >> 8) & 0xFF;
	uint8_t u4 = (u) & 0xFF;
#else 
	uint8_t u1 = (uint8_t)(u >> 24);
	uint8_t u2 = (uint8_t)(u >> 16);
	uint8_t u3 = (uint8_t)(u >> 8);
	uint8_t u4 = (uint8_t)(u);
#endif


	res =
		gd__belt_H21[u4] |
		gd__belt_H29[u3] |
		gd__belt_H5[u2] |
		gd__belt_H13[u1];

	return(res);
}
#endif


static void gd__belt_swap32(uint32_t* a, uint32_t* b){
	uint32_t tmp = *a;
	*a = *b;
	*b = tmp;
}

#define GD_BELT_SWAP(a, b, t)	\
	t = a;	\
	a = b;	\
	b = t;

static void gd_belt_transform(const uint8_t x[16], const uint8_t theta_key[32], uint8_t dest[16]){
	int it = 0;
	uint32_t a, b, c, d, e, t;

	uint32_t thetas[8];
	uint32_t k[56];
	
	GD_BELT_PACK_U32(theta_key, &thetas[0]);
	GD_BELT_PACK_U32(theta_key + 4, &thetas[1]);
	GD_BELT_PACK_U32(theta_key + 8, &thetas[2]);
	GD_BELT_PACK_U32(theta_key + 12, &thetas[3]);
	GD_BELT_PACK_U32(theta_key + 16, &thetas[4]);
	GD_BELT_PACK_U32(theta_key + 20, &thetas[5]);
	GD_BELT_PACK_U32(theta_key + 24, &thetas[6]);
	GD_BELT_PACK_U32(theta_key + 28, &thetas[7]);

	GD_BELT_CHUNK256_COPY(k, thetas);
	GD_BELT_CHUNK256_COPY(k + 8, thetas);
	GD_BELT_CHUNK256_COPY(k + 16, thetas);
	GD_BELT_CHUNK256_COPY(k + 24, thetas);
	GD_BELT_CHUNK256_COPY(k + 32, thetas);
	GD_BELT_CHUNK256_COPY(k + 40, thetas);
	GD_BELT_CHUNK256_COPY(k + 48, thetas);

	GD_BELT_PACK_U32(x, &a);
	GD_BELT_PACK_U32(x + 4, &b);
	GD_BELT_PACK_U32(x + 8, &c);
	GD_BELT_PACK_U32(x + 12, &d);	

	/*Original loop was designed to be from 1 to 8 included. 
		so I will use (i + 1) everywhere needed instead (i)*/
	for(it; it < 8; it++){

#if 0
		b = b ^ gd_belt_g(a + k[7 * (it + 1) - 6 - 1], 5);
		c = c ^ gd_belt_g(d + k[7 * (it + 1) - 5 - 1], 21);
		a = a - gd_belt_g(b + k[7 * (it + 1) - 4 - 1], 13);
		e = gd_belt_g(b + c + k[7 * (it + 1) - 3 - 1], 21) ^ (it + 1);
		b = b + e;
		c = c - e;
		d = d + gd_belt_g(c + k[7 * (it + 1) - 2 - 1], 13);
		b = b ^ gd_belt_g(a + k[7 * (it + 1) - 1 - 1], 21);
		c = c ^ gd_belt_g(d + k[7 * (it + 1) - 1], 5);

		GD_BELT_SWAP(a, b, t);
		GD_BELT_SWAP(c, d, t);
		GD_BELT_SWAP(b, c, t);
#else
		b = b ^ GD_BELT_G5(a + k[7 * (it + 1) - 6 - 1]);
		c = c ^ GD_BELT_G21(d + k[7 * (it + 1) - 5 - 1]);
		a = a - GD_BELT_G13(b + k[7 * (it + 1) - 4 - 1]);
		e = GD_BELT_G21(b + c + k[7 * (it + 1) - 3 - 1]) ^ (it + 1);
		b = b + e;
		c = c - e;
		d = d + GD_BELT_G13(c + k[7 * (it + 1) - 2 - 1]);
		b = b ^ GD_BELT_G21(a + k[7 * (it + 1) - 1 - 1]);
		c = c ^ GD_BELT_G5(d + k[7 * (it + 1) - 1]);

		GD_BELT_SWAP(a, b, t);
		GD_BELT_SWAP(c, d, t);
		GD_BELT_SWAP(b, c, t);
#endif
	}

	GD_BELT_UNPACK_U32(b, dest);
	GD_BELT_UNPACK_U32(d, dest + 4);
	GD_BELT_UNPACK_U32(a, dest + 8);
	GD_BELT_UNPACK_U32(c, dest + 12);
}

static void gd_belt_inv_transform(const uint8_t x[16], const uint8_t theta_key[32], uint8_t dest[16]){
	int it = 0;
	uint32_t a, b, c, d, e, t;

	uint32_t thetas[8];
	uint32_t k[56];
	
	GD_BELT_PACK_U32(theta_key, &thetas[0]);
	GD_BELT_PACK_U32(theta_key + 4, &thetas[1]);
	GD_BELT_PACK_U32(theta_key + 8, &thetas[2]);
	GD_BELT_PACK_U32(theta_key + 12, &thetas[3]);
	GD_BELT_PACK_U32(theta_key + 16, &thetas[4]);
	GD_BELT_PACK_U32(theta_key + 20, &thetas[5]);
	GD_BELT_PACK_U32(theta_key + 24, &thetas[6]);
	GD_BELT_PACK_U32(theta_key + 28, &thetas[7]);

	GD_BELT_CHUNK256_COPY(k, thetas);
	GD_BELT_CHUNK256_COPY(k + 8, thetas);
	GD_BELT_CHUNK256_COPY(k + 16, thetas);
	GD_BELT_CHUNK256_COPY(k + 24, thetas);
	GD_BELT_CHUNK256_COPY(k + 32, thetas);
	GD_BELT_CHUNK256_COPY(k + 40, thetas);
	GD_BELT_CHUNK256_COPY(k + 48, thetas);

	GD_BELT_PACK_U32(x, &a);
	GD_BELT_PACK_U32(x + 4, &b);
	GD_BELT_PACK_U32(x + 8, &c);
	GD_BELT_PACK_U32(x + 12, &d);	

	for(it = 8; it >= 1; it--){
#if 0
		b = b ^ gd_belt_g(a + k[7 * it - 1], 5);
		c = c ^ gd_belt_g(d + k[7 * it - 1 - 1], 21);
		a = a - gd_belt_g(b + k[7 * it - 2 - 1], 13);
		e = gd_belt_g(b + c + k[7 * it - 3 - 1], 21) ^ it;
		b = b + e;
		c = c - e;
		d = d + gd_belt_g(c + k[7 * it - 4 - 1], 13);
		b = b ^ gd_belt_g(a + k[7 * it - 5 - 1], 21);
		c = c ^ gd_belt_g(d + k[7 * it - 6 - 1], 5);
		
		GD_BELT_SWAP(a, b, t);
		GD_BELT_SWAP(c, d, t);
		GD_BELT_SWAP(a, d, t);
#else
		b = b ^ GD_BELT_G5(a + k[7 * it - 1]);
		c = c ^ GD_BELT_G21(d + k[7 * it - 1 - 1]);
		a = a - GD_BELT_G13(b + k[7 * it - 2 - 1]);
		e = GD_BELT_G21(b + c + k[7 * it - 3 - 1]) ^ it;
		b = b + e;
		c = c - e;
		d = d + GD_BELT_G13(c + k[7 * it - 4 - 1]);
		b = b ^ GD_BELT_G21(a + k[7 * it - 5 - 1]);
		c = c ^ GD_BELT_G5(d + k[7 * it - 6 - 1]);

		GD_BELT_SWAP(a, b, t);
		GD_BELT_SWAP(c, d, t);
		GD_BELT_SWAP(a, d, t);
#endif
	}

	GD_BELT_UNPACK_U32(c, dest);
	GD_BELT_UNPACK_U32(a, dest + 4);
	GD_BELT_UNPACK_U32(d, dest + 8);
	GD_BELT_UNPACK_U32(b, dest + 12);
}

/*
	ECB
*/

/*
	Params:
		bytes - bytes of the message to encrypt;
		bytes_count - length of the message;
		theta - key;
		dest - destination buffer for encrypted message
*/

void gd_belt_ecb_encrypt(const uint8_t* bytes, int bytes_count, const uint8_t theta[32], uint8_t* dest){

	int it = 0;
	int it2 = 0;
	int xn_size = 0;
	int complete_msg_size = 0;
	
	uint8_t temp16[16];
	uint8_t yn_concat_r[16];

	xn_size = bytes_count & 15;

	if ((uint32_t)(bytes_count & 15) == 0){
		complete_msg_size = bytes_count;
	}
	else{
		complete_msg_size = bytes_count + 16 - (bytes_count & 15);
	}


	if((uint32_t)(bytes_count & 15) == 0){
		for(it = 0; it < bytes_count; it+=16){
			gd_belt_transform(bytes + it, theta, dest + it);
		}
	}
	else{
		for(it = 0; it < complete_msg_size - 32; it+=16){
			gd_belt_transform(bytes + it, theta, dest + it);
		}

		gd_belt_transform(bytes + it, theta, yn_concat_r);

		for (it2 = 0; it2 < xn_size; it2++){
			temp16[it2] = bytes[it2 + it + 16];
		}
		for (it2; it2 < 16; it2++){
			temp16[it2] = yn_concat_r[it2];
		}

		gd_belt_transform(temp16, theta, dest + it);

		for (it2 = 0; it2 < xn_size; it2++){
			*(dest + it + 16 + it2) = yn_concat_r[it2];
		}
	}
}


/*
	Params:
		bytes - bytes of the message to decrypt;
		bytes_count - length of the message;
		theta - key;
		dest - destination buffer for decrypted message
*/
void gd_belt_ecb_decrypt(const uint8_t* bytes, int bytes_count, const uint8_t theta[32], uint8_t* dest){
	int it = 0;
	int it2 = 0;
	int xn_size = 0;
	int complete_msg_size = 0;

	uint8_t temp16[16];
	uint8_t yn_concat_r[16];

	xn_size = bytes_count & 15;

	if ((uint32_t)(bytes_count & 15) == 0){
		complete_msg_size = bytes_count;
	}
	else{
		complete_msg_size = bytes_count + 16 - (bytes_count & 15);
	}

	if((uint32_t)(bytes_count & 15) == 0){
		for(it = 0; it < bytes_count; it+=16){
			gd_belt_inv_transform(bytes + it, theta, dest + it);
		}
	}
	else{
		for(it = 0; it < complete_msg_size - 32; it+=16){
			gd_belt_inv_transform(bytes + it, theta, dest + it);
		}

		gd_belt_inv_transform(bytes + it, theta, yn_concat_r);

		for (it2 = 0; it2 < xn_size; it2++){
			temp16[it2] = bytes[it2 + it + 16];
		}
		for (it2; it2 < 16; it2++){
			temp16[it2] = yn_concat_r[it2];
		}

		gd_belt_inv_transform(temp16, theta, dest + it);

		for (it2 = 0; it2 < xn_size; it2++){
			*(dest + it + 16 + it2) = yn_concat_r[it2];
		}
	}
}


/*
	CBC
*/

void gd_belt_cbc_encrypt(const uint8_t* bytes, int bytes_count, const uint8_t theta[32], const uint8_t s[16], uint8_t* dest){
	uint8_t temp_y[16];
	uint8_t temp16[16];
	uint8_t yn_concat_r[16];

	int it = 0;
	int it2 = 0;
	int cur_chunk_size = 0;
	int xn_size = 0;

	/*First step*/
	GD_BELT_CHUNK128_COPY(temp_y, s);

	/*Second step*/
	if((uint32_t)(bytes_count & 15) == 0){
		/*First substep*/
		for(it = 0; it < bytes_count; it += 16){
			if(it == 0){
				GD_BELT_CHUNK128_XOR(temp16, bytes + it, temp_y);
			}
			else{
				GD_BELT_CHUNK128_XOR(temp16, bytes + it, dest + it - 16);
			}

			gd_belt_transform(temp16, theta, dest + it);
		}

	}else{

		xn_size = bytes_count & 15;

		/*First substep*/
		for(it = 0; it < bytes_count - 32; it += 16){
			if(it == 0){
				GD_BELT_CHUNK128_XOR(temp16, bytes + it, temp_y);
			}
			else{
				GD_BELT_CHUNK128_XOR(temp16, bytes + it, dest + it - 16);
			}

			gd_belt_transform(temp16, theta, dest + it);
		}

		it += 16;

		/*Second substep*/
		GD_BELT_CHUNK128_XOR(temp16, bytes + it - 16, dest + it - 32);
		gd_belt_transform(temp16, theta, yn_concat_r);

		/*Third substep*/
		GD_BELT_CHUNK128_SET_ZERO(temp16);
		for(it2 = 0; it2 < xn_size; it2++){
			temp16[it2] = bytes[it2 + it];
		}
		GD_BELT_CHUNK128_XOREQ(temp16, yn_concat_r);
		gd_belt_transform(temp16, theta, temp16);
		GD_BELT_CHUNK128_COPY(dest + it - 16, temp16);

		for(it2 = 0; it2 < xn_size; it2++){
			*(dest + it + it2) = yn_concat_r[it2];
		}
	}
}
void gd_belt_cbc_decrypt(const uint8_t* bytes, int bytes_count, const uint8_t theta[32], const uint8_t s[16], uint8_t* dest){
	uint8_t temp_x[16];
	uint8_t temp16[16];
	uint8_t yn_concat_r[16];

	int it = 0;
	int it2 = 0;
	int cur_chunk_size = 0;
	int xn_size = 0;

	/*First step*/
	GD_BELT_CHUNK128_COPY(temp_x, s);

	/*Second step*/
	if ((uint32_t)(bytes_count & 15) == 0){
		/*First substep*/
		for (it = 0; it < bytes_count; it += 16){
			gd_belt_inv_transform(bytes + it, theta, temp16);
			if (it == 0){
				GD_BELT_CHUNK128_XOR(dest + it, temp16, temp_x);
			}
			else{
				GD_BELT_CHUNK128_XOR(dest + it, temp16, bytes + it - 16);
			}
		}
	}
	else{
		
		xn_size = bytes_count & 15;

		/*First substep*/
		for (it = 0; it < bytes_count - 32; it += 16){
			gd_belt_inv_transform(bytes + it, theta, temp16);
			if (it == 0){
				GD_BELT_CHUNK128_XOR(dest + it, temp16, temp_x);
			}
			else{
				GD_BELT_CHUNK128_XOR(dest + it, temp16, bytes + it - 16);
			}
		}

		it += 16;

		/*Second substep*/
		gd_belt_inv_transform(bytes + it - 16, theta, temp16);
		for (it2 = 0; it2 < xn_size; it2++){
			temp16[it2] = temp16[it2] ^ *(bytes + it + it2);
		}
		GD_BELT_CHUNK128_COPY(yn_concat_r, temp16);

		/*Third substep*/
		for (it2 = 0; it2 < xn_size; it2++){
			temp16[it2] = *(bytes + it + it2);
		}

		gd_belt_inv_transform(temp16, theta, temp16);
		if (it == 16){
			GD_BELT_CHUNK128_XOR(dest + it - 16, temp16, temp_x);
		}
		else{
			GD_BELT_CHUNK128_XOR(dest + it - 16, temp16, bytes + it - 32);
		}

		for (it2 = 0; it2 < xn_size; it2++){
			*(dest + it + it2) = yn_concat_r[it2];
		}
	}
}


/*
	CFB
*/

void gd_belt_cfb_encrypt(const uint8_t* bytes, int bytes_count, const uint8_t theta[32], const uint8_t s[16], uint8_t* dest){

	int it = 0;
	int it2 = 0;
	int xn_size = 0;

	uint8_t temp16[16];
	uint8_t temp_y0[16];

	GD_BELT_CHUNK128_COPY(temp_y0, s);

	if((uint32_t)(bytes_count & 15) == 0){
		for(it = 0; it < bytes_count; it += 16){
			if(it == 0){
				gd_belt_transform(temp_y0, theta, temp16);
			}
			else{
				gd_belt_transform(dest + it - 16, theta, temp16);
			}

			GD_BELT_CHUNK128_XOR(dest + it, temp16, bytes + it);
		}
	}
	else{
		xn_size = bytes_count & 15;

		for(it = 0; it < bytes_count - 16; it += 16){
			if(it == 0){
				gd_belt_transform(temp_y0, theta, temp16);
			}
			else{
				gd_belt_transform(dest + it - 16, theta, temp16);
			}

			GD_BELT_CHUNK128_XOR(dest + it, temp16, bytes + it);
		}

		if(it == 0){
			gd_belt_transform(temp_y0, theta, temp16);
		}
		else{
			gd_belt_transform(dest + it - 16, theta, temp16);
		}

		for(it2 = 0; it2 < xn_size; it2++){
			*(dest + it + it2) = *(bytes + it + it2) ^ temp16[it2];
		}
	}
}

void gd_belt_cfb_decrypt(const uint8_t* bytes, int bytes_count, const uint8_t theta[32], const uint8_t s[16], uint8_t* dest){
	int it = 0;
	int it2 = 0;
	int xn_size = 0;

	uint8_t temp16[16];
	uint8_t temp_x0[16];

	GD_BELT_CHUNK128_COPY(temp_x0, s);

	if((uint32_t)(bytes_count & 15) == 0){
		for(it = 0; it < bytes_count; it += 16){
			if(it == 0){
				gd_belt_transform(temp_x0, theta, temp16);
			}
			else{
				gd_belt_transform(bytes + it - 16, theta, temp16);
			}

			GD_BELT_CHUNK128_XOR(dest + it, bytes + it, temp16);
		}
	}
	else{
		xn_size = bytes_count & 15;

		for(it = 0; it < bytes_count - 16; it+= 16){
			if(it == 0){
				gd_belt_transform(temp_x0, theta, temp16);
			}
			else{
				gd_belt_transform(bytes + it - 16, theta, temp16);
			}

			GD_BELT_CHUNK128_XOR(dest + it, bytes + it, temp16);
		}

		if(it == 0){
			gd_belt_transform(temp_x0, theta, temp16);
		}
		else{
			gd_belt_transform(bytes + it - 16, theta, temp16);
		}

		for(it2 = 0; it2 < xn_size; it2++){
			*(dest + it + it2) = *(bytes + it + it2) ^ temp16[it2];
		}
	}
}


/*
	CTR
*/

void gd_belt_ctr_encrypt(const uint8_t* bytes, int bytes_count, const uint8_t theta[32], const uint8_t s[16], uint8_t* dest){
	int it = 0;
	int it2 = 0;
	int xn_size = 0;

	uint32_t s_words[4];

	uint8_t temp16[16];
	uint8_t temp_s[16];

	gd_belt_transform(s, theta, temp_s);

	if((uint32_t)(bytes_count & 15) == 0){
		for(it = 0; it < bytes_count; it+= 16){

#if 0
			GD_BELT_CHUNK128_COPY(s_words, temp_s);

			/*Do something about this hell*/
			if(s_words[0] != 0xFFFFFFFF){
				s_words[0] += 1;
			}	
			else{
				if(s_words[1] != 0xFFFFFFFF){
					s_words[1] += 1;
				}
				else{
					if(s_words[2] != 0xFFFFFFFF){
						s_words[2] += 1;
					}
					else{
						if(s_words[3] != 0xFFFFFFFF){
							s_words[3] += 1;
						}
						else{
							s_words[3] = 0x00000000;
						}

						s_words[2] = 0x00000000;
					}

					s_words[1] = 0x00000000;
				}

				s_words[0] = 0x00000000;
			}

			GD_BELT_CHUNK128_COPY(temp_s, s_words);
#else
			gd_belt_add_one_mod128(temp_s);
#endif
			gd_belt_transform(temp_s, theta, temp16);

			if(bytes_count - it >= 16){
				GD_BELT_CHUNK128_XOR(dest + it, temp16, bytes + it);
			}
			else{
				xn_size = bytes_count & 15;

				for(it2 = 0; it2 < xn_size; it2++){
					*(dest + it + it2) = *(bytes + it + it2) ^ temp16[it2];
				}
			}
		}
	}
}

void gd_belt_ctr_decrypt(const uint8_t* bytes, int bytes_count, const uint8_t theta[32], const uint8_t s[16], uint8_t* dest){
	gd_belt_ctr_encrypt(bytes, bytes_count, theta, s, dest);
}


/*
	MAC
*/

static void gd_belt_phi1(const uint8_t src[16], uint8_t dest[16]){
	uint32_t u[4];
	uint32_t temp;

	GD_BELT_PACK_U32(src, &u[0]);
	GD_BELT_PACK_U32(src + 4, &u[1]);
	GD_BELT_PACK_U32(src + 8, &u[2]);
	GD_BELT_PACK_U32(src + 12, &u[3]);

	temp = u[0] ^ u[1];

	GD_BELT_UNPACK_U32(u[1], dest);
	GD_BELT_UNPACK_U32(u[2], dest + 4);
	GD_BELT_UNPACK_U32(u[3], dest + 8);
	GD_BELT_UNPACK_U32(temp, dest + 12);
}

static void gd_belt_phi2(const uint8_t src[16], uint8_t dest[16]){
	uint32_t u[4];
	uint32_t temp;

	GD_BELT_PACK_U32(src, &u[0]);
	GD_BELT_PACK_U32(src + 4, &u[1]);
	GD_BELT_PACK_U32(src + 8, &u[2]);
	GD_BELT_PACK_U32(src + 12, &u[3]);

	temp = u[0] ^ u[3];

	GD_BELT_UNPACK_U32(temp, dest);
	GD_BELT_UNPACK_U32(u[0], dest + 4);
	GD_BELT_UNPACK_U32(u[1], dest + 8);
	GD_BELT_UNPACK_U32(u[2], dest + 12);
}

static void gd_belt_psi(const uint8_t* bytes, int bytes_count, uint8_t dest[16]){
	int zeros_to_write = 0;
	int it = 0;
	int it2 = 0;

	zeros_to_write = 15 - bytes_count;

	for (it = 0; it < bytes_count; it++){
		dest[it] = bytes[it];
	}

	dest[it++] = 0x80;

	for (it2 = 0; it2 < zeros_to_write; it2++){
		dest[it + it2] = 0x00;
	}
}

void gd_belt_mac(const uint8_t* bytes, int bytes_count, const uint8_t theta[32], uint8_t dest[8]){

	uint8_t s[16];
	uint8_t r[16];
	uint8_t temp16[16];
	uint8_t temp16_2[16];

	int it = 0;
	int xn_size = 0;

	GD_BELT_CHUNK128_SET_ZERO(s);

	/*First step*/
	gd_belt_transform(s, theta, r);

	/*Second step*/
	for (it = 0; it < bytes_count - 16; it += 16){
		GD_BELT_CHUNK128_XOR(temp16, s, bytes + it);
		gd_belt_transform(temp16, theta, s);
	}

	/*Third step*/
	if ((uint32_t)(bytes_count & 15) == 0){
		gd_belt_phi1(r, temp16);
		GD_BELT_CHUNK128_XOREQ(temp16, bytes + it);
		GD_BELT_CHUNK128_XOREQ(temp16, s);
		GD_BELT_CHUNK128_COPY(s, temp16);
	}
	else{
		xn_size = bytes_count & 15;

		gd_belt_phi2(r, temp16);
		gd_belt_psi(bytes + it, xn_size, temp16_2);
		GD_BELT_CHUNK128_XOREQ(temp16, temp16_2);
		GD_BELT_CHUNK128_XOREQ(temp16, s);
		GD_BELT_CHUNK128_COPY(s, temp16);
	}

	/*Fourth step*/
	gd_belt_transform(s, theta, temp16);
	GD_BELT_CHUNK64_COPY(dest, temp16);
}


//x_len < 2 ^ 64;
//i_len < 2 ^ 64;
void gd_belt_datawrap_set(uint8_t* x, int x_len, uint8_t* i, int i_len, uint8_t theta[32], uint8_t s[16], uint8_t* dest){

	uint8_t temp_s[16];
	uint8_t temp_r[16];
	uint8_t temp16[16];

	int it = 0;
	int it2 = 0;
	int xn_size = 0;
	int in_size = 0;

	/*Step 1*/
	gd_belt_transform(s, theta, temp_r);
	GD_BELT_CHUNK128_COPY(temp_s, temp_r);

	/*Step 2*/
	for (it = 0; it < x_len; it += 16){
		/*Substep 1*/
		gd_belt_add_one_mod128(temp_s);
	
		/*Substep 2*/
		gd_belt_transform(temp_s, theta, temp16);
		
		if (x_len - it >= 16){
			GD_BELT_CHUNK128_XOR(dest + it, x + it, temp16);
		}
		else{
			xn_size = x_len & 15;

			for (it2 = 0; it2 < xn_size; it2++){
				*(dest + it + it2) = *(x + it + it2) ^ temp16[it2];
			}
		}
	}

	/*Step 3*/
	gd_belt_transform(temp_r, theta, temp_r);

	GD_BELT_CHUNK128_COPY(temp_s, gd__belt_H);

	/*Step 4*/
	for (it = 0; it < i_len; it += 16){
		if (i_len - it >= 16){
			GD_BELT_CHUNK128_COPY(temp16, i + it);
		}
		else{
			in_size = i_len & 15;

			GD_BELT_CHUNK128_SET_ZERO(temp16);

			for (it2 = 0; it2 < in_size; it2++){
				temp16[it2] = *(i + it + it2);
			}
		}

		GD_BELT_CHUNK128_XOREQ(temp_s, temp16);
	}

}

//x_len < 2 ^ 64;
//i_len < 2 ^ 64;
void gd_belt_datawrap_unset(){


}


/*
	Hashing
*/
static void gd_belt_hash_sigma1(uint8_t x[64], uint8_t dest[16]){
	/*x is u1 ## u2 ## u3 ## u4. u is 16 byte (128 bit) word*/

	uint8_t tran_res[16];
	uint8_t temp_res[16];
	uint8_t theta[32];

	/*Theta = u1 ## u2*/
	GD_BELT_CHUNK128_COPY(theta, x);
	GD_BELT_CHUNK128_COPY(theta + 16, x + 16);

	/*temp_res = u3 ^ u4*/
	GD_BELT_CHUNK128_XOR(temp_res, x + 32, x + 48);

	gd_belt_transform(temp_res, theta, tran_res);

	GD_BELT_CHUNK128_XOREQ(tran_res, temp_res);

	/*Storing the result*/
	GD_BELT_CHUNK128_COPY(dest, tran_res);
}

static void gd_belt_hash_sigma2(uint8_t x[64], uint8_t dest[32]){
	/*x is u1 | u2 | u3 | u4. u is 16 byte (128 bit) word*/
	
	uint8_t res1[16];
	uint8_t res2[16];

	uint8_t temp_theta1[16];
	uint8_t temp_theta2[16];
	uint8_t temp_theta2_ones[16];
	uint8_t theta1[32];
	uint8_t theta2[32];

	/*theta1 computation*/
	gd_belt_hash_sigma1(x, temp_theta1);
	GD_BELT_CHUNK128_COPY(theta1, temp_theta1);
	GD_BELT_CHUNK128_COPY(theta1 + 16, x + 48);

	/*theta2 computation*/
	GD_BELT_CHUNK128_SET_ONE(temp_theta2_ones);
	gd_belt_hash_sigma1(x, temp_theta2);
	GD_BELT_CHUNK128_XOREQ(temp_theta2, temp_theta2_ones);
	GD_BELT_CHUNK128_COPY(theta2, temp_theta2);
	GD_BELT_CHUNK128_COPY(theta2 + 16, x + 32);

	/*Computations*/
	gd_belt_transform(x, theta1, res1);
	gd_belt_transform(x + 16, theta2, res2);	

	GD_BELT_CHUNK128_XOREQ(res1, x);
	GD_BELT_CHUNK128_XOREQ(res2, x + 16);

	/*Storing the result*/
	GD_BELT_CHUNK128_COPY(dest, res1);
	GD_BELT_CHUNK128_COPY(dest + 16, res2);
}


/*
	Params:
		bytes - bytes of the message to hash;
		bytes_count - length of the message;
		dest - output pointer where you store the result;
*/
void gd_belt_hash256(const uint8_t* bytes, int bytes_count, uint8_t dest[32]){

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
	GD_BELT_CHUNK128_SET_ZERO(s);

	/*Second step*/
	hash[0] = 0xC8BA94B1;
	hash[1] = 0x3BF5080A;
	hash[2] = 0x8E006D36;
	hash[3] = 0xE45D4A58;
	hash[4] = 0x9DFA0485;
	hash[5] = 0xACC7B61B;
	hash[6] = 0xC2722E25;
	hash[7] = 0x0DCEFD02;
	
	GD_BELT_UNPACK_U32(hash[0], hash_bytes);
	GD_BELT_UNPACK_U32(hash[1], hash_bytes + 4);
	GD_BELT_UNPACK_U32(hash[2], hash_bytes + 8);
	GD_BELT_UNPACK_U32(hash[3], hash_bytes + 12);
	GD_BELT_UNPACK_U32(hash[4], hash_bytes + 16);
	GD_BELT_UNPACK_U32(hash[5], hash_bytes + 20);
	GD_BELT_UNPACK_U32(hash[6], hash_bytes + 24);
	GD_BELT_UNPACK_U32(hash[7], hash_bytes + 28);
	
	/*Third step*/
	for(it = 0; it < new_len; it += 32){
		/*temp64 = Xi ## hash*/
		GD_BELT_CHUNK256_COPY(temp64, new_msg + it);
		GD_BELT_CHUNK256_COPY(temp64 + 32, hash_bytes);
		
		/*Substep 1*/
		gd_belt_hash_sigma1(temp64, temp16);
		GD_BELT_CHUNK128_XOREQ(s, temp16);

		/*Substep 2*/
		gd_belt_hash_sigma2(temp64, hash_bytes);
	}

	free(new_msg);

	/*Fourth step*/
	x_long_mod_128 = (bytes_count << 3);

	GD_BELT_CHUNK128_SET_ZERO(temp16);
	GD_BELT_UNPACK_U32(x_long_mod_128, temp16);

	GD_BELT_CHUNK128_COPY(temp64, temp16);
	GD_BELT_CHUNK128_COPY(temp64 + 16, s);
	GD_BELT_CHUNK256_COPY(temp64 + 32, hash_bytes);
	gd_belt_hash_sigma2(temp64, temp32);

	/*Fifth step*/
	GD_BELT_CHUNK256_COPY(dest, temp32);
}

/*
	Additional algorithms
*/
static void gd_belt_key_expand(uint32_t* thetas, int n, uint8_t dest[32]){
	
	uint32_t thetas_cpy[8];

	switch (n){
		case 4:{
			thetas_cpy[0] = *(thetas);
			thetas_cpy[1] = *(thetas + 1);
			thetas_cpy[2] = *(thetas + 2);
			thetas_cpy[3] = *(thetas + 3);
			
			thetas_cpy[4] = *(thetas);
			thetas_cpy[5] = *(thetas + 1);
			thetas_cpy[6] = *(thetas + 2);
			thetas_cpy[7] = *(thetas + 3);
		}break;

		case 6:{
			thetas_cpy[0] = *(thetas);
			thetas_cpy[1] = *(thetas + 1);
			thetas_cpy[2] = *(thetas + 2);
			thetas_cpy[3] = *(thetas + 3);
			thetas_cpy[4] = *(thetas + 4);
			thetas_cpy[5] = *(thetas + 5);

			thetas_cpy[6] = *(thetas) ^ *(thetas + 1) ^ *(thetas + 2);
			thetas_cpy[7] = *(thetas + 3) ^ *(thetas + 4) ^ *(thetas + 5);
		}break;

		case 8:{
			thetas_cpy[0] = *(thetas);
			thetas_cpy[1] = *(thetas + 1);
			thetas_cpy[2] = *(thetas + 2);
			thetas_cpy[3] = *(thetas + 3);
			thetas_cpy[4] = *(thetas + 4);
			thetas_cpy[5] = *(thetas + 5);
			thetas_cpy[6] = *(thetas + 6);
			thetas_cpy[7] = *(thetas + 7);
		}break;
	}

	GD_BELT_UNPACK_U32(thetas_cpy[0], dest);
	GD_BELT_UNPACK_U32(thetas_cpy[1], dest + 4);
	GD_BELT_UNPACK_U32(thetas_cpy[2], dest + 8);
	GD_BELT_UNPACK_U32(thetas_cpy[3], dest + 12);
	GD_BELT_UNPACK_U32(thetas_cpy[4], dest + 16);
	GD_BELT_UNPACK_U32(thetas_cpy[5], dest + 20);
	GD_BELT_UNPACK_U32(thetas_cpy[6], dest + 24);
	GD_BELT_UNPACK_U32(thetas_cpy[7], dest + 28);
}

/*
	Key transformation.
	Params:
		key - key to transform;
		n_in - size of the key in bytes;
		d - depth of the key;
		i - header;
		m_in - size of the header in bytes;
		dest - destination buffer that has m_in size;
*/
static void gd_belt_key_transform(
	uint8_t* key,
	int n_in,
	uint8_t d[12],
	uint8_t i[16],
	int m_in,
	uint8_t* dest)
{
	uint8_t r[4];
	uint8_t s[32];

	uint8_t temp32[32];
	uint8_t temp64[64];

	int it, m, n;

	n = n_in << 3;
	m = m_in << 3;

	if (n == 128 && m == 128){
		r[0] = 0xb1;
		r[1] = 0x94;
		r[2] = 0xba;
		r[3] = 0xc8;
	}
	else if (n == 192 && m == 128){
		r[0] = 0x5b;
		r[1] = 0xe3;
		r[2] = 0xd6;
		r[3] = 0x12;
	}
	else if (n == 192 && m == 192){
		r[0] = 0x5c;
		r[1] = 0xb0;
		r[2] = 0xc0;
		r[3] = 0xff;
	}
	else if (n == 256 && m == 128){
		r[0] = 0xe1;
		r[1] = 0x2b;
		r[2] = 0xdc;
		r[3] = 0x1a;
	}
	else if (n == 256 && m == 192){
		r[0] = 0xc1;
		r[1] = 0xab;
		r[2] = 0x76;
		r[3] = 0x38;
	}
	else if (n == 256 && m == 256){
		r[0] = 0xf3;
		r[1] = 0x3c;
		r[2] = 0x65;
		r[3] = 0x7b;
	}

	gd_belt_key_expand((uint32_t*)key, (n_in >> 2), s);

	GD_BELT_CHUNK512_SET_ZERO(temp64);

	*((uint32_t*)temp64) = *((uint32_t*)r);
	GD_BELT_CHUNK96_COPY(temp64 + 4, d);
	GD_BELT_CHUNK128_COPY(temp64 + 16, i);
	GD_BELT_CHUNK256_COPY(temp64 + 32, s);

	gd_belt_hash_sigma2(temp64, temp32);

	for (it = 0; it < m_in; it++){
		dest[it] = temp32[it];
	}
}

#endif

