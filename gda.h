#ifndef GDA_H

#ifdef __cplusplus
extern "C"{
#endif

#ifdef __cplusplus
#define GDA_EXTERN extern "C"
#else
#define GDA_EXTERN extern
#endif

#ifdef _WIN32
#define GDA_DLL_EXPORT GDA_EXTERN __declspec(dllexport)
#define GDA_DLL_IMPORT GDA_EXTERN __declspec(dllimport)
#else
#define GDA_DLL_EXPORT GDA_EXTERN __attribute__((visibility("default")))
#define GDA_DLL_IMPORT GDA_EXTERN
#endif


#ifndef GDA_DEF
#ifdef GDA_STATIC
#define GDA_DEF static
#else
#define GDA_DEF extern
#endif
#endif


#if defined(_WIN64) || defined(__x86_64__) || defined(__64BIT__) || defined(__powerpc64__) || defined(__ppc64__)
#ifndef GDA_ARCH_64_BIT
#define GDA_ARCH_64_BIT 1
#endif
#else
#ifndef GDA_ARCH_32_BIT
#define GDA_ARCH_32_BIT 1
#endif
#endif
#if defined(_WIN32) || defined(_WIN64)
#ifndef GDA_SYSTEM_WINDOWS
#define GDA_SYSTEM_WINDOWS 1
#endif
#elif defined(__APPLE__) && defined(__MACH__)
#ifndef GDA_SYSTEM_OSX
#define GDA_SYSTEM_OSX 1
#endif
#elif defined(__unix__)
#ifndef GDA_SYSTEM_UNIX
#define GDA_SYSTEM_UNIX 1
#endif
#ifdef __linux__
#ifndef GDA_SYSTEM_LINUX
#define GDA_SYSTEM_LINUX 1
#endif
#elif defined(__FreeBSD__) || defined(__FreeBSD_kernel__)
#ifndef GDA_SYSTEM_FREEBSD
#define GDA_SYSTEM_FREEBSD 1
#endif
#else
#error this UNIX OS is not supported
#endif
#else
#error This operating system is not supported
#endif


#ifdef _MSC_VER
#define GDA_COMPILER_MSVC 1
#elif defined(__GNUC__)
#define GDA_COMPILER_GCC 1
#elif defined(__clang__)
#define GDA_COMPILER_CLANG 1
#else
#error This compiler is not supported
#endif


#if defined(_M_IX86) || defined(_M_X64) || defined(__i386__) || defined(__x86_64__)
#ifndef GDA_CPU_X86
#define GDA_CPU_X86 1
#endif
#ifndef GDA_CACHE_LINE_SIZE
#define GDA_CACHE_LINE_SIZE 64
#endif
#elif defined(_M_PPC) || defined(__powerpc__) || defined(__powerpc64__)
#ifndef GDA_CPU_PPC
#define GDA_CPU_PPC 1
#endif
#ifndef GDA_CACHE_LINE_SIZE
#define GDA_CACHE_LINE_SIZE 128
#endif
#elif defined(__arm__)
#ifndef GDA_CPU_ARM
#define GDA_CPU_ARM 1
#endif
#ifndef GDA_CACHE_LINE_SIZE
#define GDA_CACHE_LINE_SIZE 64
#endif
#elif defined(__MIPSEL__) || defined(__mips_isa_rev)
#ifndef GDA_CPU_MIPS
#define GDA_CPU_MIPS 1
#endif
#ifndef GDA_CACHE_LINE_SIZE
#define GDA_CACHE_LINE_SIZE 64
#endif
#else
#error Unknown CPU type
#endif


#if defined(_WIN32) && !defined(__MINGW32__)
#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif
#endif

#ifndef GDA_TYPES_DEFINED
#define GDA_TYPES_DEFINED

#ifdef GDA_COMPILER_MSVC
#if _MSC_VER < 1300
typedef unsigned char u8;
typedef unsigned char uint8;
typedef signed char s8;
typedef signed char int8;
typedef unsigned short u16;
typedef unsigned short uint16;
typedef signed short s16;
typedef signed short int16;
typedef unsigned int u32;
typedef unsigned int uint32;
typedef signed int s32;
typedef signed int int32;
#else
typedef unsigned __int8 u8;
typedef unsigned __int8 uint8;
typedef signed __int8 s8;
typedef signed __int8 int8;
typedef unsigned __int16 u16;
typedef unsigned __int16 uint16;
typedef signed __int16 s16;
typedef signed __int16 int16;
typedef unsigned __int32 u32;
typedef unsigned __int32 uint32;
typedef signed __int32 s32;
typedef signed __int32 int32;
#endif
typedef unsigned __int64 u64;
typedef unsigned __int64 uint64;
typedef signed __int64 s64;
typedef signed __int64 int64;
#else
#include <stdint.h>
typedef uint8_t   u8;
typedef uint8_t uint8;
typedef  int8_t   s8;
typedef int8_t int8;
typedef uint16_t u16;
typedef uint16_t uint16;
typedef  int16_t s16;
typedef int16_t int16;
typedef uint32_t u32;
typedef uint32_t uint32;
typedef  int32_t s32;
typedef int32_t int32;
typedef uint64_t u64;
typedef uint64_t uint64;
typedef  int64_t s64;
typedef int64_t int64;
#endif

typedef float  r32;
typedef float real32;
typedef double r64;
typedef float real64;

typedef s8 b8;
typedef s8 bool8;
typedef s16 b16;
typedef s16 bool16;
typedef s32 b32;
typedef s32 bool32;
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

#if defined(GDA_ARCH_32_BIT)
#define USIZE_MIN U32_MIN
#define USIZE_MAX U32_MAX

#define ISIZE_MIN I32_MIN
#define ISIZE_MAX I32_MAX
#elif defined(GDA_ARCH_64_BIT)
#define USIZE_MIN U64_MIN
#define USIZE_MAX U64_MAX

#define ISIZE_MIN I64_MIN
#define ISIZE_MAX I64_MAX
#else
#error Unknown architecture size. This library only supports 32 bit and 64 bit architectures.
#endif

#define F32_MIN 1.17549435e-38f
#define F32_MAX 3.40282347e+38f

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

#ifndef GDA_GLOBAL_VARIABLE
#define GDA_GLOBAL_VARIABLE	static
#endif

#ifndef GDA_INTERNAL_FUNCTION
#define GDA_INTERNAL_FUNCTION static
#endif

#ifndef GDA_LOCAL_PERSIST
#define GDA_LOCAL_PERSIST static
#endif

#ifndef GDA_BIT
#define GDA_BIT(value) (1 << (value))
#endif

#ifndef GDA_LERP
#define GDA_LERP(a, b, t) ((a) + ((b) - (a)) * (t))
#endif

#ifndef GDA_MIN
#define GDA_MIN(a, b) (a) < (b) ? (a) : (b)
#endif

#ifndef GDA_MAX
#define GDA_MAX(a, b) (a) > (b) ? (a) : (b)
#endif

#ifndef GDA_CLAMP
#define GDA_CLAMP(value, lower, upper) (GDA_MIN(GDA_MAX(value, lower), upper))
#endif

#ifndef GDA_CLAMP01
#define GDA_CLAMP01(value) (GDA_CLAMP(value, 0, 1))
#endif

#ifndef GDA_SQUARE
#define GDA_SQUARE(value) ((value) * (value))
#endif

#ifndef GDA_CUBE
#define GDA_CUBE(value) ((value) * (value) * (value))
#endif

#ifndef GDA_ABS
#define GDA_ABS(value) (value) >= 0 ? (value) : -(value
#endif

#ifndef GDA_ARRAY_COUNT
#define GDA_ARRAY_COUNT(Array) (sizeof(Array) / sizeof(Array[0]))
#endif

#ifndef GDA_KILOBYTES
#define GDA_KILOBYTES(Value) (Value * 1024)
#define GDA_MEGABYTES(Value) (GDA_KILOBYTES(1024) * Value)
#define GDA_GIGABYTES(Value) (GDA_MEGABYTES(1024) * Value)
#define GDA_TERABYTES(Value) (GDA_GIGABYTES(1024) * Value)
#endif

#ifdef __cplusplus
}
#endif

#define GDA_H
#endif

#ifdef GDA_IMPLEMENTATION



#endif