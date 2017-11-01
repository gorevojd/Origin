/*

ABOUT:
	Implementation of Belarussian standart of hashing algorihms STB 34.101.77
	Description of the standart: http://apmi.bsu.by/assets/files/std/bash-spec10.pdf

LICENSE:
	This software is dual-licensed to the public domain and under the following
	license: you are granted a perpetual, irrevocable license to copy, modify,
	publish, and distribute this file as you see fit.

CREDITS:
	Written by Gorevoy Dmitry

HOW TO USE:
	If you want to use this library do the following: 
	define GD_BASH_IMPLEMENTATION and include this library in one cpp
	file like this:

	#define GD_BASH_IMPLEMENTATION
	#include "gd_bash.h"

	if you want to use it in multiple cpp files you need to define 
	#define GD_BASH_STATIC

EXAMPLES:
	
*/

#ifndef GD_BASH_H

#ifndef GDBSH_DEF
#ifdef GD_BASH_STATIC
#define GDBSH_DEF static
#else
#define GDBSH_DEF extern
#endif
#endif

#include <stdint.h>

#define GD_BASH_ROTLEFT(word, amount)				\
	(((word) << (amount)) | ((word) >> ((sizeof(word) << 3) - (amount))))

#define GD_BASH_ROTLEFT32(word, amount)				\
	(((word) << (amount)) | ((word) >> (32 - (amount))))

#define GD_BASH_ROTLEFT64(word, amount)				\
	(((word) << (amount)) | ((word) >> (64 - (amount))))


#define GD_BASH_ROTRIGHT(word, amount)				\
	(((word) >> (amount)) | ((word) << ((sizeof(word) << 3) - (amount))))

#define GD_BASH_ROTRIGHT32(word, amount)				\
	(((word) >> (amount)) | ((word) << (32 - (amount))))

#define GD_BASH_ROTRIGHT64(word, amount)				\
	(((word) >> (amount)) | ((word) << (64 - (amount))))


/*64 bit macro*/
#if 0
#define GD_BASH_CHUNK64_COPY(dest, src)								\
	((uint32_t*)(dest))[0] = ((const uint32_t*)(src))[0];			\
	((uint32_t*)(dest))[1] = ((const uint32_t*)(src))[1];			
#else
#define GD_BASH_CHUNK64_COPY(dest, src)								\
	*((uint64_t*)(dest)) = *((const uint64_t*)(src));			
#endif


#define GD_BASH_CHUNK64_SET_ZERO(chunk)				\
	((uint32_t*)(chunk))[0] = 0;					\
	((uint32_t*)(chunk))[1] = 0;					


#ifdef __cplusplus
extern "C" {
#endif
	GDBSH_DEF void gd_bash_hash256(uint8_t* bytes, uint64_t bytes_count, uint8_t dest[32]);
	GDBSH_DEF void gd_bash_hash394(uint8_t* bytes, uint64_t bytes_count, uint8_t dest[48]);
	GDBSH_DEF void gd_bash_hash512(uint8_t* bytes, uint64_t bytes_count, uint8_t dest[64]);
#ifdef __cplusplus
}
#endif

#define GD_BASH_H
#endif

#if defined(GD_BASH_IMPLEMENTATION) && !defined(GD_BASH_IMPLEMENTATION_DONE)
#define GD_BASH_IMPLEMENTAION_DONE

#define GD_BASH_S(w0, w1, w2, m1, n1, m2, n2, t0, t1, t2)				\
	t0 = GD_BASH_ROTLEFT64(w0, m1);										\
	w0 = w0 ^ w1 ^ w2;													\
	t1 = w1 ^ GD_BASH_ROTLEFT64(w0, n1);								\
	w1 = t0 ^ t1;														\
	w2 = w2 ^ GD_BASH_ROTLEFT64(w2, m2) ^ GD_BASH_ROTLEFT64(t1, n2);	\
	t0 = ~w2;															\
	t1 = w0 | w2;														\
	t2 = w0 & w1;														\
	t0 = t0 | w1;														\
	w0 = w0 ^ t0;														\
	w1 = w1 ^ t1;														\
	w2 = w2 ^ t2;


static void gd_bash_f(uint8_t bytes[192], uint8_t dest[192]){
	uint64_t c;
	int m1, n1, m2, n2;
	int it = 0;
	int j = 0;

	uint8_t temp[192];
	uint8_t temp8[8];

	c = 0x3BF5080AC8BA94B1;

	for(it = 0; it < 24; it++){
		m1 = 8;
		n1 = 53;
		m2 = 14;
		n2 = 1;

		uint64_t t0, t1, t2;

		uint64_t* w0p = (uint64_t*)(bytes);
		uint64_t* w1p = (uint64_t*)(bytes + 64);
		uint64_t* w2p = (uint64_t*)(bytes + 128);

		for(j = 0; j < 8; j++){

			GD_BASH_S(*w0p, *w1p, *w2p, m1, n1, m2, n2, t0, t1, t2);

			w0p++;
			w1p++;
			w2p++;

#if 1
			m1 = (7 * m1) & 63;
			n1 = (7 * n1) & 63;
			m2 = (7 * m2) & 63;
			n2 = (7 * n2) & 63;
#else
			m1 = (7 * m1) % 64;
			n1 = (7 * n1) % 64;
			m2 = (7 * m2) % 64;
			n2 = (7 * n2) % 64;
#endif
		}

		GD_BASH_CHUNK64_COPY(temp + 0, bytes + 120);
		GD_BASH_CHUNK64_COPY(temp + 8, bytes + 80);
		GD_BASH_CHUNK64_COPY(temp + 16, bytes + 72);
		GD_BASH_CHUNK64_COPY(temp + 24, bytes + 96);
		GD_BASH_CHUNK64_COPY(temp + 32, bytes + 88);
		GD_BASH_CHUNK64_COPY(temp + 40, bytes + 112);
		GD_BASH_CHUNK64_COPY(temp + 48, bytes + 104);
		GD_BASH_CHUNK64_COPY(temp + 56, bytes + 64);
		GD_BASH_CHUNK64_COPY(temp + 64, bytes + 136);
		GD_BASH_CHUNK64_COPY(temp + 72, bytes + 128);
		GD_BASH_CHUNK64_COPY(temp + 80, bytes + 152);
		GD_BASH_CHUNK64_COPY(temp + 88, bytes + 144);
		GD_BASH_CHUNK64_COPY(temp + 96, bytes + 168);
		GD_BASH_CHUNK64_COPY(temp + 104, bytes + 160);
		GD_BASH_CHUNK64_COPY(temp + 112, bytes + 184);
		GD_BASH_CHUNK64_COPY(temp + 120, bytes + 176);
		GD_BASH_CHUNK64_COPY(temp + 128, bytes + 48);
		GD_BASH_CHUNK64_COPY(temp + 136, bytes + 24);
		GD_BASH_CHUNK64_COPY(temp + 144, bytes);
		GD_BASH_CHUNK64_COPY(temp + 152, bytes + 40);
		GD_BASH_CHUNK64_COPY(temp + 160, bytes + 16);
		GD_BASH_CHUNK64_COPY(temp + 168, bytes + 56);
		GD_BASH_CHUNK64_COPY(temp + 176, bytes + 32);
		GD_BASH_CHUNK64_COPY(temp + 184, bytes + 8);

		GD_BASH_CHUNK64_COPY(bytes + 0, temp + 0);
		GD_BASH_CHUNK64_COPY(bytes + 8, temp + 8);
		GD_BASH_CHUNK64_COPY(bytes + 16, temp + 16);
		GD_BASH_CHUNK64_COPY(bytes + 24, temp + 24);
		GD_BASH_CHUNK64_COPY(bytes + 32, temp + 32);
		GD_BASH_CHUNK64_COPY(bytes + 40, temp + 40);
		GD_BASH_CHUNK64_COPY(bytes + 48, temp + 48);
		GD_BASH_CHUNK64_COPY(bytes + 56, temp + 56);
		GD_BASH_CHUNK64_COPY(bytes + 64, temp + 64);
		GD_BASH_CHUNK64_COPY(bytes + 72, temp + 72);
		GD_BASH_CHUNK64_COPY(bytes + 80, temp + 80);
		GD_BASH_CHUNK64_COPY(bytes + 88, temp + 88);
		GD_BASH_CHUNK64_COPY(bytes + 96, temp + 96);
		GD_BASH_CHUNK64_COPY(bytes + 104, temp + 104);
		GD_BASH_CHUNK64_COPY(bytes + 112, temp + 112);
		GD_BASH_CHUNK64_COPY(bytes + 120, temp + 120);
		GD_BASH_CHUNK64_COPY(bytes + 128, temp + 128);
		GD_BASH_CHUNK64_COPY(bytes + 136, temp + 136);
		GD_BASH_CHUNK64_COPY(bytes + 144, temp + 144);
		GD_BASH_CHUNK64_COPY(bytes + 152, temp + 152);
		GD_BASH_CHUNK64_COPY(bytes + 160, temp + 160);
		GD_BASH_CHUNK64_COPY(bytes + 168, temp + 168);
		GD_BASH_CHUNK64_COPY(bytes + 176, temp + 176);
		GD_BASH_CHUNK64_COPY(bytes + 184, temp + 184);

		*((uint64_t*)(bytes + 184)) = *((uint64_t*)(bytes + 184)) ^ c;

		if((uint64_t)(c & 1) == 0){
			c = c >> 1;
		}
		else{
			c = (c >> 1) ^ 0xDC2BE1997FE0D8AE;
		}
	}

	GD_BASH_CHUNK64_COPY(dest + 0, bytes + 0);
	GD_BASH_CHUNK64_COPY(dest + 8, bytes + 8);
	GD_BASH_CHUNK64_COPY(dest + 16, bytes + 16);
	GD_BASH_CHUNK64_COPY(dest + 24, bytes + 24);
	GD_BASH_CHUNK64_COPY(dest + 32, bytes + 32);
	GD_BASH_CHUNK64_COPY(dest + 40, bytes + 40);
	GD_BASH_CHUNK64_COPY(dest + 48, bytes + 48);
	GD_BASH_CHUNK64_COPY(dest + 56, bytes + 56);
	GD_BASH_CHUNK64_COPY(dest + 64, bytes + 64);
	GD_BASH_CHUNK64_COPY(dest + 72, bytes + 72);
	GD_BASH_CHUNK64_COPY(dest + 80, bytes + 80);
	GD_BASH_CHUNK64_COPY(dest + 88, bytes + 88);
	GD_BASH_CHUNK64_COPY(dest + 96, bytes + 96);
	GD_BASH_CHUNK64_COPY(dest + 104, bytes + 104);
	GD_BASH_CHUNK64_COPY(dest + 112, bytes + 112);
	GD_BASH_CHUNK64_COPY(dest + 120, bytes + 120);
	GD_BASH_CHUNK64_COPY(dest + 128, bytes + 128);
	GD_BASH_CHUNK64_COPY(dest + 136, bytes + 136);
	GD_BASH_CHUNK64_COPY(dest + 144, bytes + 144);
	GD_BASH_CHUNK64_COPY(dest + 152, bytes + 152);
	GD_BASH_CHUNK64_COPY(dest + 160, bytes + 160);
	GD_BASH_CHUNK64_COPY(dest + 168, bytes + 168);
	GD_BASH_CHUNK64_COPY(dest + 176, bytes + 176);
	GD_BASH_CHUNK64_COPY(dest + 184, bytes + 184);
}


void gd_bash_hash256(uint8_t* bytes, uint64_t bytes_count, uint8_t dest[32]){
	int l;
	int lmul2;
	int extended_chunk_len;
	int new_msg_size;
	int xn_size;
	int zero_bytes_to_fill;
	int copy_cursor;
	int it;

	uint8_t s[192];
	uint8_t* xn;

	uint64_t at_end_l = 0;

	l = 16;
	lmul2 = l * 2;

	extended_chunk_len = (1536 - 4 * (l << 3)) >> 3;
	xn = (uint8_t*)malloc(extended_chunk_len);

	if((uint64_t)(bytes_count & (extended_chunk_len - 1)) == 0){
		new_msg_size = bytes_count + extended_chunk_len;

		xn_size = 0;
		zero_bytes_to_fill = 0;
		copy_cursor = bytes_count;
	}
	else{
		xn_size = bytes_count % extended_chunk_len;

		zero_bytes_to_fill = extended_chunk_len - 1 - xn_size;

		new_msg_size = bytes_count + 1 + zero_bytes_to_fill;
		copy_cursor = new_msg_size - extended_chunk_len;
	}

	for(it = 0; it < xn_size; it++){
		xn[it] = bytes[copy_cursor + it];
	}

	xn[it++] = 0x40;

	for(it; it < extended_chunk_len; it++){
		xn[it] = 0x00;
	}

	GD_BASH_CHUNK64_SET_ZERO(s + 0);
	GD_BASH_CHUNK64_SET_ZERO(s + 8);
	GD_BASH_CHUNK64_SET_ZERO(s + 16);
	GD_BASH_CHUNK64_SET_ZERO(s + 24);
	GD_BASH_CHUNK64_SET_ZERO(s + 32);
	GD_BASH_CHUNK64_SET_ZERO(s + 40);
	GD_BASH_CHUNK64_SET_ZERO(s + 48);
	GD_BASH_CHUNK64_SET_ZERO(s + 56);
	GD_BASH_CHUNK64_SET_ZERO(s + 64);
	GD_BASH_CHUNK64_SET_ZERO(s + 72);
	GD_BASH_CHUNK64_SET_ZERO(s + 80);
	GD_BASH_CHUNK64_SET_ZERO(s + 88);
	GD_BASH_CHUNK64_SET_ZERO(s + 96);
	GD_BASH_CHUNK64_SET_ZERO(s + 104);
	GD_BASH_CHUNK64_SET_ZERO(s + 112);
	GD_BASH_CHUNK64_SET_ZERO(s + 120);
	GD_BASH_CHUNK64_SET_ZERO(s + 128);
	GD_BASH_CHUNK64_SET_ZERO(s + 136);
	GD_BASH_CHUNK64_SET_ZERO(s + 144);
	GD_BASH_CHUNK64_SET_ZERO(s + 152);
	GD_BASH_CHUNK64_SET_ZERO(s + 160);
	GD_BASH_CHUNK64_SET_ZERO(s + 168);
	GD_BASH_CHUNK64_SET_ZERO(s + 176);


	at_end_l = (uint64_t)((l << 3) >> 2);
	*(uint64_t*)(s + 184) = at_end_l;

	for(
		it = 0;
		it < new_msg_size - extended_chunk_len;
		it += extended_chunk_len)
	{
		GD_BASH_CHUNK64_COPY(s + 0, bytes + it + 0);
		GD_BASH_CHUNK64_COPY(s + 8, bytes + it + 8);
		GD_BASH_CHUNK64_COPY(s + 16, bytes + it + 16);
		GD_BASH_CHUNK64_COPY(s + 24, bytes + it + 24);
		GD_BASH_CHUNK64_COPY(s + 32, bytes + it + 32);
		GD_BASH_CHUNK64_COPY(s + 40, bytes + it + 40);
		GD_BASH_CHUNK64_COPY(s + 48, bytes + it + 48);
		GD_BASH_CHUNK64_COPY(s + 56, bytes + it + 56);
		GD_BASH_CHUNK64_COPY(s + 64, bytes + it + 64);
		GD_BASH_CHUNK64_COPY(s + 72, bytes + it + 72);
		GD_BASH_CHUNK64_COPY(s + 80, bytes + it + 80);
		GD_BASH_CHUNK64_COPY(s + 88, bytes + it + 88);
		GD_BASH_CHUNK64_COPY(s + 96, bytes + it + 96);
		GD_BASH_CHUNK64_COPY(s + 104, bytes + it + 104);
		GD_BASH_CHUNK64_COPY(s + 112, bytes + it + 112);
		GD_BASH_CHUNK64_COPY(s + 120, bytes + it + 120);

		gd_bash_f(s, s);
	}

	GD_BASH_CHUNK64_COPY(s + 0, xn + 0);
	GD_BASH_CHUNK64_COPY(s + 8, xn + 8);
	GD_BASH_CHUNK64_COPY(s + 16, xn + 16);
	GD_BASH_CHUNK64_COPY(s + 24, xn + 24);
	GD_BASH_CHUNK64_COPY(s + 32, xn + 32);
	GD_BASH_CHUNK64_COPY(s + 40, xn + 40);
	GD_BASH_CHUNK64_COPY(s + 48, xn + 48);
	GD_BASH_CHUNK64_COPY(s + 56, xn + 56);
	GD_BASH_CHUNK64_COPY(s + 64, xn + 64);
	GD_BASH_CHUNK64_COPY(s + 72, xn + 72);
	GD_BASH_CHUNK64_COPY(s + 80, xn + 80);
	GD_BASH_CHUNK64_COPY(s + 88, xn + 88);
	GD_BASH_CHUNK64_COPY(s + 96, xn + 96);
	GD_BASH_CHUNK64_COPY(s + 104, xn + 104);
	GD_BASH_CHUNK64_COPY(s + 112, xn + 112);
	GD_BASH_CHUNK64_COPY(s + 120, xn + 120);

	gd_bash_f(s, s);

	for(it = 0; it < lmul2; it++){
		dest[it] = s[it];
	}

	free(xn);
}

void gd_bash_hash394(uint8_t* bytes, uint64_t bytes_count, uint8_t dest[48]){
	int l;
	int lmul2;
	int extended_chunk_len;
	int new_msg_size;
	int xn_size;
	int zero_bytes_to_fill;
	int copy_cursor;
	int it;

	uint8_t s[192];
	uint8_t* xn;

	uint64_t at_end_l = 0;

	l = 24;
	lmul2 = l * 2;

	extended_chunk_len = (1536 - 4 * (l << 3)) >> 3;
	xn = (uint8_t*)malloc(extended_chunk_len);

	if ((uint64_t)(bytes_count & (extended_chunk_len - 1)) == 0){
		new_msg_size = bytes_count + extended_chunk_len;

		xn_size = 0;
		zero_bytes_to_fill = 0;
		copy_cursor = bytes_count;
	}
	else{
		xn_size = bytes_count % extended_chunk_len;

		zero_bytes_to_fill = extended_chunk_len - 1 - xn_size;

		new_msg_size = bytes_count + 1 + zero_bytes_to_fill;
		copy_cursor = new_msg_size - extended_chunk_len;
	}

	for (it = 0; it < xn_size; it++){
		xn[it] = bytes[copy_cursor + it];
	}

	xn[it++] = 0x40;

	for (it; it < extended_chunk_len; it++){
		xn[it] = 0x00;
	}

	GD_BASH_CHUNK64_SET_ZERO(s + 0);
	GD_BASH_CHUNK64_SET_ZERO(s + 8);
	GD_BASH_CHUNK64_SET_ZERO(s + 16);
	GD_BASH_CHUNK64_SET_ZERO(s + 24);
	GD_BASH_CHUNK64_SET_ZERO(s + 32);
	GD_BASH_CHUNK64_SET_ZERO(s + 40);
	GD_BASH_CHUNK64_SET_ZERO(s + 48);
	GD_BASH_CHUNK64_SET_ZERO(s + 56);
	GD_BASH_CHUNK64_SET_ZERO(s + 64);
	GD_BASH_CHUNK64_SET_ZERO(s + 72);
	GD_BASH_CHUNK64_SET_ZERO(s + 80);
	GD_BASH_CHUNK64_SET_ZERO(s + 88);
	GD_BASH_CHUNK64_SET_ZERO(s + 96);
	GD_BASH_CHUNK64_SET_ZERO(s + 104);
	GD_BASH_CHUNK64_SET_ZERO(s + 112);
	GD_BASH_CHUNK64_SET_ZERO(s + 120);
	GD_BASH_CHUNK64_SET_ZERO(s + 128);
	GD_BASH_CHUNK64_SET_ZERO(s + 136);
	GD_BASH_CHUNK64_SET_ZERO(s + 144);
	GD_BASH_CHUNK64_SET_ZERO(s + 152);
	GD_BASH_CHUNK64_SET_ZERO(s + 160);
	GD_BASH_CHUNK64_SET_ZERO(s + 168);
	GD_BASH_CHUNK64_SET_ZERO(s + 176);


	at_end_l = (uint64_t)((l << 3) >> 2);
	*(uint64_t*)(s + 184) = at_end_l;

	for (
		it = 0;
		it < new_msg_size - extended_chunk_len;
	it += extended_chunk_len)
	{
		GD_BASH_CHUNK64_COPY(s + 0, bytes + it + 0);
		GD_BASH_CHUNK64_COPY(s + 8, bytes + it + 8);
		GD_BASH_CHUNK64_COPY(s + 16, bytes + it + 16);
		GD_BASH_CHUNK64_COPY(s + 24, bytes + it + 24);
		GD_BASH_CHUNK64_COPY(s + 32, bytes + it + 32);
		GD_BASH_CHUNK64_COPY(s + 40, bytes + it + 40);
		GD_BASH_CHUNK64_COPY(s + 48, bytes + it + 48);
		GD_BASH_CHUNK64_COPY(s + 56, bytes + it + 56);
		GD_BASH_CHUNK64_COPY(s + 64, bytes + it + 64);
		GD_BASH_CHUNK64_COPY(s + 72, bytes + it + 72);
		GD_BASH_CHUNK64_COPY(s + 80, bytes + it + 80);
		GD_BASH_CHUNK64_COPY(s + 88, bytes + it + 88);

		gd_bash_f(s, s);
	}

	GD_BASH_CHUNK64_COPY(s + 0, xn + 0);
	GD_BASH_CHUNK64_COPY(s + 8, xn + 8);
	GD_BASH_CHUNK64_COPY(s + 16, xn + 16);
	GD_BASH_CHUNK64_COPY(s + 24, xn + 24);
	GD_BASH_CHUNK64_COPY(s + 32, xn + 32);
	GD_BASH_CHUNK64_COPY(s + 40, xn + 40);
	GD_BASH_CHUNK64_COPY(s + 48, xn + 48);
	GD_BASH_CHUNK64_COPY(s + 56, xn + 56);
	GD_BASH_CHUNK64_COPY(s + 64, xn + 64);
	GD_BASH_CHUNK64_COPY(s + 72, xn + 72);
	GD_BASH_CHUNK64_COPY(s + 80, xn + 80);
	GD_BASH_CHUNK64_COPY(s + 88, xn + 88);

	gd_bash_f(s, s);

	for (it = 0; it < lmul2; it++){
		dest[it] = s[it];
	}

	free(xn);
}

void gd_bash_hash512(uint8_t* bytes, uint64_t bytes_count, uint8_t dest[64]){
	int l;
	int lmul2;
	int extended_chunk_len;
	int new_msg_size;
	int xn_size;
	int zero_bytes_to_fill;
	int copy_cursor;
	int it;

	uint8_t s[192];
	uint8_t* xn;

	uint64_t at_end_l = 0;

	l = 32;
	lmul2 = l * 2;

	extended_chunk_len = (1536 - 4 * (l << 3)) >> 3;
	xn = (uint8_t*)malloc(extended_chunk_len);

	if ((uint64_t)(bytes_count & (extended_chunk_len - 1)) == 0){
		new_msg_size = bytes_count + extended_chunk_len;

		xn_size = 0;
		zero_bytes_to_fill = 0;
		copy_cursor = bytes_count;
	}
	else{
		xn_size = bytes_count % extended_chunk_len;

		zero_bytes_to_fill = extended_chunk_len - 1 - xn_size;

		new_msg_size = bytes_count + 1 + zero_bytes_to_fill;
		copy_cursor = new_msg_size - extended_chunk_len;
	}

	for (it = 0; it < xn_size; it++){
		xn[it] = bytes[copy_cursor + it];
	}

	xn[it++] = 0x40;

	for (it; it < extended_chunk_len; it++){
		xn[it] = 0x00;
	}

	GD_BASH_CHUNK64_SET_ZERO(s + 0);
	GD_BASH_CHUNK64_SET_ZERO(s + 8);
	GD_BASH_CHUNK64_SET_ZERO(s + 16);
	GD_BASH_CHUNK64_SET_ZERO(s + 24);
	GD_BASH_CHUNK64_SET_ZERO(s + 32);
	GD_BASH_CHUNK64_SET_ZERO(s + 40);
	GD_BASH_CHUNK64_SET_ZERO(s + 48);
	GD_BASH_CHUNK64_SET_ZERO(s + 56);
	GD_BASH_CHUNK64_SET_ZERO(s + 64);
	GD_BASH_CHUNK64_SET_ZERO(s + 72);
	GD_BASH_CHUNK64_SET_ZERO(s + 80);
	GD_BASH_CHUNK64_SET_ZERO(s + 88);
	GD_BASH_CHUNK64_SET_ZERO(s + 96);
	GD_BASH_CHUNK64_SET_ZERO(s + 104);
	GD_BASH_CHUNK64_SET_ZERO(s + 112);
	GD_BASH_CHUNK64_SET_ZERO(s + 120);
	GD_BASH_CHUNK64_SET_ZERO(s + 128);
	GD_BASH_CHUNK64_SET_ZERO(s + 136);
	GD_BASH_CHUNK64_SET_ZERO(s + 144);
	GD_BASH_CHUNK64_SET_ZERO(s + 152);
	GD_BASH_CHUNK64_SET_ZERO(s + 160);
	GD_BASH_CHUNK64_SET_ZERO(s + 168);
	GD_BASH_CHUNK64_SET_ZERO(s + 176);


	at_end_l = (uint64_t)((l << 3) >> 2);
	*(uint64_t*)(s + 184) = at_end_l;

	for (
		it = 0;
		it < new_msg_size - extended_chunk_len;
	it += extended_chunk_len)
	{
		GD_BASH_CHUNK64_COPY(s + 0, bytes + it + 0);
		GD_BASH_CHUNK64_COPY(s + 8, bytes + it + 8);
		GD_BASH_CHUNK64_COPY(s + 16, bytes + it + 16);
		GD_BASH_CHUNK64_COPY(s + 24, bytes + it + 24);
		GD_BASH_CHUNK64_COPY(s + 32, bytes + it + 32);
		GD_BASH_CHUNK64_COPY(s + 40, bytes + it + 40);
		GD_BASH_CHUNK64_COPY(s + 48, bytes + it + 48);
		GD_BASH_CHUNK64_COPY(s + 56, bytes + it + 56);

		gd_bash_f(s, s);
	}

	GD_BASH_CHUNK64_COPY(s + 0, xn + 0);
	GD_BASH_CHUNK64_COPY(s + 8, xn + 8);
	GD_BASH_CHUNK64_COPY(s + 16, xn + 16);
	GD_BASH_CHUNK64_COPY(s + 24, xn + 24);
	GD_BASH_CHUNK64_COPY(s + 32, xn + 32);
	GD_BASH_CHUNK64_COPY(s + 40, xn + 40);
	GD_BASH_CHUNK64_COPY(s + 48, xn + 48);
	GD_BASH_CHUNK64_COPY(s + 56, xn + 56);

	gd_bash_f(s, s);

	for (it = 0; it < lmul2; it++){
		dest[it] = s[it];
	}

	free(xn);
}

#endif