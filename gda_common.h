#ifndef GDA_COMMON

#ifndef COMPILER_MSVC
#define COMPILER_MSVC 0
#endif

#ifndef COMPILER_GCC
#define COMPILER_GCC 0
#endif

#if !COMPILER_MSVC
#if _MSC_VER
#undef COMPILER_MSVC
#define COMPILER_MSVC 1
#endif

#elif
#if __GNUC__
#undef COMPILER_GCC
#define COMPILER_GCC 1
#endif
#endif

#ifdef STDINT_TYPES
#include <stdint.h>
typedef uint64_t uint64;
typedef uint32_t uint32;
typedef uint16_t uint16;
typedef uint8_t uint8;

typedef int64_t int64;
typedef int32_t int32;
typedef int16_t int16;
typedef int8_t int8;

typedef uint64_t u64;
typedef uint32_t u32;
typedef uint16_t u16;
typedef uint8_t u8;

typedef int64_t s64;
typedef int32_t s32;
typedef int16_t s16;
typedef int8_t s8;
#else
typedef unsigned long long uint64;
typedef unsigned int uint32;
typedef unsigned short uint16;
typedef unsigned char uint8;

typedef unsigned long long u64;
typedef unsigned int u32;
typedef unsigned short u16;
typedef unsigned char u8;

typedef long long int64;
typedef signed int int32;
typedef signed short int16;
typedef signed char int8;

typedef long long s64;
typedef signed int s32;
typedef signed short s16;
typedef signed char s8;
#endif

typedef float real32;
typedef double real64;

#define ArrayCount(Array) (sizeof((Array)) / sizeof((Array[0])))

#define GDA_PI 3.14159265359

#define GLOBAL_VARIABLE static
#define LOCAL_PERSIST static
#define INTERNAL_FUNCTION static

#include <math.h>

int32 gda_StrLen(const char* Str1){
	int32 Result = 0;
	while (*Str1++ != 0){
		Result = Result + 1;
	}
	return(Result);
}

//TODO(Dima): Maybe optimize this with SIMD;
int32 gda_CompareStrings(const char* Str1, const char* Str2){
	while (*Str1++ == *Str2++){
		if (*Str1 == '\0'){
			return 0;
		}
	}
	return(*(unsigned char*)Str1 > *(unsigned char*)Str2);
}


char* gda_CopyString(char* Dest, char* Source, int SourceLen){
	for (int i = 0; i < SourceLen; i++){
		Dest[i] = Source[i];
	}
	Dest[SourceLen] = '\0';
	return(Dest);
}

char* gda_UppercaseString(char* Str1){
	char* Save = Str1;
	int Diff = 'A' - 'a';
	while (*Str1++ != '\0'){
		if (*Str1 < 'A' && *Str1 >= 'a'){
			*Str1 += Diff;
		}
	}
	return Save;
}

#define GDA_COMMON
#endif