/*
LICENSE
	This software is dual-licensed to the public domain and under the following
	license: you are granted a perpetual, irrevocable license to copy, modify,
	publish, and distribute this file as you see fit.

CREDITS
	Written by Gorevoy Dmitry
*/


#ifndef GD_H
#define GD_H

#ifdef __cplusplus
#define GD_EXTERN extern "C"
#else
#define GD_EXTERN extern
#endif

#ifdef __cplusplus
extern "C"{
#endif


#ifdef _WIN32
#define GD_DLL_EXPORT GD_EXTERN __declspec(dllexport)
#define GD_DLL_IMPORT GD_EXTERN __declspec(dllimport)
#else
#define GD_DLL_EXPORT GD_EXTERN __attribute__((visibility("default")))
#define GD_DLL_IMPORT GD_EXTERN
#endif


#ifndef GD_DEF
#ifdef GD_STATIC
#define GD_DEF static
#else
#define GD_DEF extern
#endif
#endif


#if defined(_WIN64) || defined(__x86_64__) || defined(__64BIT__) || defined(__powerpc64__) || defined(__ppc64__)
#ifndef GD_ARCH_64_BIT
#define GD_ARCH_64_BIT 1
#endif
#else
#ifndef GD_ARCH_32_BIT
#define GD_ARCH_32_BIT 1
#endif
#endif
#if defined(_WIN32) || defined(_WIN64)
#ifndef GD_SYSTEM_WINDOWS
#define GD_SYSTEM_WINDOWS 1
#endif
#elif defined(__APPLE__) && defined(__MACH__)
#ifndef GD_SYSTEM_OSX
#define GD_SYSTEM_OSX 1
#endif
#elif defined(__unix__)
#ifndef GD_SYSTEM_UNIX
#define GD_SYSTEM_UNIX 1
#endif
#ifdef __linux__
#ifndef GD_SYSTEM_LINUX
#define GD_SYSTEM_LINUX 1
#endif
#elif defined(__FreeBSD__) || defined(__FreeBSD_kernel__)
#ifndef GD_SYSTEM_FREEBSD
#define GD_SYSTEM_FREEBSD 1
#endif
#else
#error this UNIX OS is not supported
#endif
#else
#error This operating system is not supported
#endif


#ifdef _MSC_VER
#define GD_COMPILER_MSVC 1
#elif defined(__GNUC__)
#define GD_COMPILER_GCC 1
#elif defined(__clang__)
#define GD_COMPILER_CLANG 1
#else
#error This compiler is not supported
#endif


#if defined(_M_IX86) || defined(_M_X64) || defined(__i386__) || defined(__x86_64__)
#ifndef GD_CPU_X86
#define GD_CPU_X86 1
#endif
#ifndef GD_CACHE_LINE_SIZE
#define GD_CACHE_LINE_SIZE 64
#endif
#elif defined(_M_PPC) || defined(__powerpc__) || defined(__powerpc64__)
#ifndef GD_CPU_PPC
#define GD_CPU_PPC 1
#endif
#ifndef GD_CACHE_LINE_SIZE
#define GD_CACHE_LINE_SIZE 128
#endif
#elif defined(__arm__)
#ifndef GD_CPU_ARM
#define GD_CPU_ARM 1
#endif
#ifndef GD_CACHE_LINE_SIZE
#define GD_CACHE_LINE_SIZE 64
#endif
#elif defined(__MIPSEL__) || defined(__mips_isa_rev)
#ifndef GD_CPU_MIPS
#define GD_CPU_MIPS 1
#endif
#ifndef GD_CACHE_LINE_SIZE
#define GD_CACHE_LINE_SIZE 64
#endif
#else
#error Unknown CPU type
#endif


#if defined(_WIN32) && !defined(__MINGW32__)
#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif
#endif

#ifndef GD_TYPES_DEFINED
#define GD_TYPES_DEFINED

#ifdef GD_COMPILER_MSVC
#if _MSC_VER < 1300
typedef unsigned char uint8;
typedef signed char int8;
typedef unsigned short uint16;
typedef signed short int16;
typedef unsigned int uint32;
typedef signed int int32;
#else
typedef unsigned __int8 uint8;
typedef signed __int8 int8;
typedef unsigned __int16 uint16;
typedef signed __int16 int16;
typedef unsigned __int32 uint32;
typedef signed __int32 int32;
#endif
typedef unsigned __int64 uint64;
typedef signed __int64 int64;
#else
#include <stdint.h>
typedef uint8_t uint8;
typedef int8_t int8;
typedef uint16_t uint16;
typedef int16_t int16;
typedef uint32_t uint32;
typedef int32_t int32;
typedef uint64_t uint64;
typedef int64_t int64;
#endif

typedef float real32;
typedef float real64;


#include <stdio.h>
typedef uintptr_t uintptr;
typedef intptr_t  intptr;

typedef int8 bool8;
typedef int16 bool16;
typedef int32 bool32;
#endif

#ifndef __cplusplus
#if (defined(_MSC_VER) && _MSC_VER <= 1800) || !defined(__STDC_VERSION__)
#ifndef true
#define true (0 == 0)
#endif
#ifndef false
#define false (0 != 0)
#endif
#else
#include <stdbool.h>
#endif
#endif


#ifndef U8_MIN
#define U8_MIN 0u;
#define U8_MAX 0xFFu
#define I8_MIN (-0x7f - 1)
#define I8_MAX 0x7f

#define U16_MIN 0u
#define U16_MAX 0xffffu
#define I16_MIN (-0x7fff - 1)
#define I16_MAX 0x7fff

#define U32_MIN 0u
#define U32_MAX 0xffffffffu
#define I32_MIN (-0x7fffffff - 1)
#define I32_MAX 0x7fffffff

#define U64_MIN 0ull
#define U64_MAX 0xffffffffffffffffull
#define I64_MIN (-0x7fffffffffffffffll - 1)
#define I64_MAX 0x7fffffffffffffffll

#if defined(GD_ARCH_32_BIT)
#define USIZE_MIN U32_MIN
#define USIZE_MAX U32_MAX

#define ISIZE_MIN I32_MIN
#define ISIZE_MAX I32_MAX
#elif defined(GD_ARCH_64_BIT)
#define USIZE_MIN U64_MIN
#define USIZE_MAX U64_MAX

#define ISIZE_MIN I64_MIN
#define ISIZE_MAX I64_MAX
#else
#error Unknown architecture size. This library only supports 32 bit and 64 bit architectures.
#endif

#define F32_MIN 1.175494351e-38F  
#define F32_MAX 3.402823466e+38F

#define F64_MIN 2.2250738585072014e-308
#define F64_MAX 1.7976931348623157e+308
#endif

#ifndef NULL
#ifdef __cplusplus
#if __cplusplus >= 201103L
#define NULL nullptr
#else
#define NULL 0
#endif
#else
#define NULL ((void *)0)
#endif
#endif

#ifdef _WIN64
typedef signed   __int64  intptr;
typedef unsigned __int64 uintptr;
#elif defined(_WIN32)
#ifndef _W64
#if !defined(__midl) && (defined(_X86) || defined(_M_IX86)) && _MSC_VER >= 1300
#define _W64 __w64
#else
#define _W64
#endif
#endif
typedef _W64 signed int intptr;
typedef _W64 unsigned int uintptr;
#else
typedef uintptr_t uintptr;
typedef intptr_t intptr;
#endif

#ifndef GD_GLOBAL_VARIABLE
#define GD_GLOBAL_VARIABLE	static
#endif

#ifndef GD_INTERNAL_FUNCTION
#define GD_INTERNAL_FUNCTION static
#endif

#ifndef GD_LOCAL_PERSIST
#define GD_LOCAL_PERSIST static
#endif

#ifndef GD_BIT
#define GD_BIT(value) (1 << (value))
#endif

#ifndef GD_LERP
#define GD_LERP(a, b, t) ((a) + ((b) - (a)) * (t))
#endif

#ifndef GD_MIN
#define GD_MIN(a, b) ((a) < (b) ? (a) : (b))
#endif

#ifndef GD_MAX
#define GD_MAX(a, b) ((a) > (b) ? (a) : (b))
#endif

#ifndef GD_CLAMP
#define GD_CLAMP(value, lower, upper) (GD_MIN(GD_MAX(value, lower), upper))
#endif

#ifndef GD_CLAMP01
#define GD_CLAMP01(value) (GD_CLAMP(value, 0, 1))
#endif

#ifndef GD_SQUARE
#define GD_SQUARE(value) ((value) * (value))
#endif

#ifndef GD_CUBE
#define GD_CUBE(value) ((value) * (value) * (value))
#endif

#ifndef GD_ABS
#define GD_ABS(value) ((value) >= 0 ? (value) : -(value))
#endif

#ifndef GD_ARRAY_COUNT
#define GD_ARRAY_COUNT(Array) (sizeof(Array) / sizeof(Array[0]))
#endif

#ifndef GD_KILOBYTES
#define GD_KILOBYTES(Value) (Value * 1024)
#define GD_MEGABYTES(Value) (GD_KILOBYTES(1024) * Value)
#define GD_GIGABYTES(Value) (GD_MEGABYTES(1024) * Value)
#define GD_TERABYTES(Value) (GD_GIGABYTES(1024) * Value)
#endif

#ifdef __cplusplus
}
#endif
#endif

#ifdef GD_IMPLEMENTATION



#endif