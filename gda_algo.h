#ifndef GDA_ALGO_H

#ifndef GDA_ALGO_DEF
#ifdef GDA_AGLO_STATIC
#define GDA_ALGO_DEF static
#else
#define GDA_ALGO_DEF extern
#endif
#endif

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable:4201)
#endif

#ifdef _MSC_VER
#pragma warning(pop)
#endif

#ifdef _MSC_VER
typedef char s8;
typedef short s16;
typedef int s32;
typedef long long s64;

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long long u64;
#else
#include <stdint.h>
typedef int8_t s8;
typedef int16_t s16;
typedef int32_t s32
typedef int64_t s64;

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;
#endif

typedef float r32;
typedef double r64;

#ifndef gda_min
#define gda_min(a, b) ((a) < (b) ? (a) : (b))
#endif

#ifndef gda_max
#define gda_max(a, b) ((a) > (b) ? (a) : (b))
#endif

#ifndef gda_clamp
#define gda_clamp(x, lower, upper) (gda_min(gda_max(x, (lower)), (upper))) 
#endif

#ifndef gda_clamp01
#define gda_clamp01(x) dga_clamp(x, 0, 1)
#endif

#ifndef gda_square
#define gda_square(x) ((x) * (x))
#endif

#ifndef gda_cube
#define gda_cube(x) ((x) * (x) * (x))
#endif

#ifndef gda_abs
#define gda_abs(x) ((x) > 0 ? (x) : -(x))
#endif

#ifndef gda_sign
#define gda_sign(x) ((x) >= 0 ? 1 : -1)
#endif

GDA_ALGO_DEF void gdaa__swap_i(s32* a1, s32* a2);
GDA_ALGO_DEF void gdaa__swap_u(u32* a1, u32* a2);
GDA_ALGO_DEF void gdaa__swap_f(r32* a1, r32* a2);
GDA_ALGO_DEF void gdaa__swap_d(r64* a1, r64* a2);

/*Numerical algorighms*/
/*Great Common Divisor*/
GDA_ALGO_DEF s32 gda_gcd_s32(s32 a, s32 b);
GDA_ALGO_DEF u32 gda_gcd_u32(u32 a, u32 b);
GDA_ALGO_DEF s64 gda_gcd_s64(s64 a, s64 b);
GDA_ALGO_DEF u64 gda_gcd_u64(u64 a, u64 b);

/*Sorting algorighms*/
GDA_ALGO_DEF void gda_bubble_sort_i(s32* arr, int size);
GDA_ALGO_DEF void gda_bubble_sort_u(u32* arr, int size);
GDA_ALGO_DEF void gda_bubble_sort_f(r32* arr, int size);
GDA_ALGO_DEF void gda_bubble_sort_d(r64* arr, int size);

GDA_ALGO_DEF void gda_insertion_sort_i(s32* arr, int size);
GDA_ALGO_DEF void gda_insertion_sort_u(u32* arr, int size);
GDA_ALGO_DEF void gda_insertion_sort_f(r32* arr, int size);
GDA_ALGO_DEF void gda_insertion_sort_d(r64* arr, int size);

GDA_ALGO_DEF void gda_selection_sort_i(s32* arr, int size);
GDA_ALGO_DEF void gda_selection_sort_u(u32* arr, int size);
GDA_ALGO_DEF void gda_selection_sort_f(r32* arr, int size);
GDA_ALGO_DEF void gda_selection_sort_d(r64* arr, int size);

#define GDA_ALGO_H
#endif


#define GDA_ALGO_IMPLEMENTATION
#ifdef GDA_ALGO_IMPLEMENTATION

#define GDAA_SWAP_MACRO(t, a1, a2) 	\
	t wtf_temp_ptr = *(a1);	\
	*(a1) = *(a2);	\
	*(a2) = wtf_temp_ptr;
void gdaa__swap_i(s32* a1, s32* a2){GDAA_SWAP_MACRO(s32, a1, a2)}
void gdaa__swap_u(u32* a1, u32* a2){GDAA_SWAP_MACRO(u32, a1, a2)}
void gdaa__swap_f(r32* a1, r32* a2){GDAA_SWAP_MACRO(r32, a1, a2)}
void gdaa__swap_d(r64* a1, r64* a2){GDAA_SWAP_MACRO(r64, a1, a2)}
#undef GDAA_SWAP_MACRO

#define GDA_GCD_DEF(t, a, b) t ta = a; t tb = b; \
	while(tb != 0){		\
		t remainder = ta % tb;	\
		ta = tb;	\
		tb = remainder;	\
	} \
	return(ta);

s32 gda_gcd_s32(s32 a, s32 b){GDA_GCD_DEF(s32, a, b)}
u32 gda_gcd_u32(u32 a, u32 b){GDA_GCD_DEF(u32, a, b)}
s64 gda_gcd_s64(s64 a, s64 b){GDA_GCD_DEF(s64, a, b)}
u64 gda_gcd_u64(u64 a, u64 b){GDA_GCD_DEF(u64, a, b)}
#undef GDA_GCD_DEF


#define GDAA_BUBBLE_SORT_MACRO(arr, sz, t, tn)	\
 	int wtf_iteration = 0;	\
	u32 not_sorted = 1;	\
	while(not_sorted){	\
		not_sorted = 0;	\
		for(int i = 1; i < sz - wtf_iteration; i++){	\
			if(arr[i] < arr[i - 1]){	\
				gdaa__swap_##tn(&arr[i], &(arr[i - 1]));	\
				not_sorted = 1;	\
			}	\
		}	\
		wtf_iteration++;	\
	}	\

void gda_bubble_sort_i(s32* arr, int size){GDAA_BUBBLE_SORT_MACRO(arr, size, s32, i)}
void gda_bubble_sort_u(u32* arr, int size){GDAA_BUBBLE_SORT_MACRO(arr, size, u32, u)}
void gda_bubble_sort_f(r32* arr, int size){GDAA_BUBBLE_SORT_MACRO(arr, size, r32, f)}
void gda_bubble_sort_d(r64* arr, int size){GDAA_BUBBLE_SORT_MACRO(arr, size, r64, d)}
#undef GDA_BUBBLE_SORT_MACRO



#define GDAA_INSERTION_SORT_MACRO(arr, sz, t, tn)	\
	for (int i = 1; i < sz; i++){	\
		int j = i;	\
		while (j > 0 && arr[j] < arr[j - 1]){	\
			gdaa__swap_##tn(&arr[j - 1], &arr[j]);	\
			j--;	\
		}	\
	}

void gda_insertion_sort_i(s32* arr, int size){ GDAA_INSERTION_SORT_MACRO(arr, size, s32, i) }
void gda_insertion_sort_u(u32* arr, int size){ GDAA_INSERTION_SORT_MACRO(arr, size, u32, u) }
void gda_insertion_sort_f(r32* arr, int size){ GDAA_INSERTION_SORT_MACRO(arr, size, r32, f) }
void gda_insertion_sort_d(r64* arr, int size){ GDAA_INSERTION_SORT_MACRO(arr, size, r64, d) }
#undef GDAA_INSERTION_SORT_MACRO


#define GDAA_SELECTION_SORT_MACRO(arr, sz, t, tn)	\
	for(int i = 0; i < sz - 1; i++){	\
		int wtf_min_index = i;	\
		for(int j = i + 1; j < sz; j++){	\
			if(arr[j] < arr[wtf_min_index]){	\
				wtf_min_index = j;	\
						}	\
				}	\
		if(arr[wtf_min_index] < arr[i]){	\
			gdaa__swap_##tn(&arr[wtf_min_index], &arr[i]);	\
		}	\
	}

void gda_selection_sort_i(s32* arr, int size){ GDAA_SELECTION_SORT_MACRO(arr, size, s32, i) }
void gda_selection_sort_u(u32* arr, int size){ GDAA_SELECTION_SORT_MACRO(arr, size, u32, u) }
void gda_selection_sort_f(r32* arr, int size){ GDAA_SELECTION_SORT_MACRO(arr, size, r32, f) }
void gda_selection_sort_d(r64* arr, int size){ GDAA_SELECTION_SORT_MACRO(arr, size, r64, d) }
#undef GDAA_SELECTION_SORT_MACRO

#endif