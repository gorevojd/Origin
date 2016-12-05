#ifndef GDA_ALGO_H

//    ________  ________  ________          ________  ___       ________  ________     
//   |\   ____\|\   ___ \|\   __  \        |\   __  \|\  \     |\   ____\|\   __  \    
//   \ \  \___|\ \  \_|\ \ \  \|\  \       \ \  \|\  \ \  \    \ \  \___|\ \  \|\  \   
//    \ \  \  __\ \  \ \\ \ \   __  \       \ \   __  \ \  \    \ \  \  __\ \  \\\  \  
//     \ \  \|\  \ \  \_\\ \ \  \ \  \       \ \  \ \  \ \  \____\ \  \|\  \ \  \\\  \ 
//      \ \_______\ \_______\ \__\ \__\       \ \__\ \__\ \_______\ \_______\ \_______\
//       \|_______|\|_______|\|__|\|__|        \|__|\|__|\|_______|\|_______|\|_______|
//                                                                                     
                                                                                  
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

#include <stdlib.h>

#ifndef GDA_TYPES_DEFINED
#define GDA_TYPES_DEFINED

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
#endif

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

GDA_ALGO_DEF void gdaa__swap_s32(s32* a1, s32* a2);
GDA_ALGO_DEF void gdaa__swap_s64(s64* a1, s64* a2);
GDA_ALGO_DEF void gdaa__swap_u32(u32* a1, u32* a2);
GDA_ALGO_DEF void gdaa__swap_u64(u64* a1, u64* a2);
GDA_ALGO_DEF void gdaa__swap_r32(r32* a1, r32* a2);
GDA_ALGO_DEF void gdaa__swap_r64(r64* a1, r64* a2);

/*Numerical algorighms*/
/*Great Common Divisor*/
GDA_ALGO_DEF s32 gda_gcd_s32(s32 a, s32 b);
GDA_ALGO_DEF u32 gda_gcd_u32(u32 a, u32 b);
GDA_ALGO_DEF s64 gda_gcd_s64(s64 a, s64 b);
GDA_ALGO_DEF u64 gda_gcd_u64(u64 a, u64 b);

/*Sorting algorighms*/
/*Bubble sorts*/
GDA_ALGO_DEF void gda_bubble_sort_s32(s32* arr, int size);
GDA_ALGO_DEF void gda_bubble_sort_s64(s64* arr, int size);
GDA_ALGO_DEF void gda_bubble_sort_u32(u32* arr, int size);
GDA_ALGO_DEF void gda_bubble_sort_u64(u64* arr, int size);
GDA_ALGO_DEF void gda_bubble_sort_r32(r32* arr, int size);
GDA_ALGO_DEF void gda_bubble_sort_r64(r64* arr, int size);

GDA_ALGO_DEF void gda_bubble_sort_desc_s32(s32* arr, int size);
GDA_ALGO_DEF void gda_bubble_sort_desc_s64(s64* arr, int size);
GDA_ALGO_DEF void gda_bubble_sort_desc_u32(u32* arr, int size);
GDA_ALGO_DEF void gda_bubble_sort_desc_u64(u64* arr, int size);
GDA_ALGO_DEF void gda_bubble_sort_desc_r32(r32* arr, int size);
GDA_ALGO_DEF void gda_bubble_sort_desc_r64(r64* arr, int size);

/*Insertion sorts*/
GDA_ALGO_DEF void gda_insertion_sort_s32(s32* arr, int size);
GDA_ALGO_DEF void gda_insertion_sort_s64(s64* arr, int size);
GDA_ALGO_DEF void gda_insertion_sort_u32(u32* arr, int size);
GDA_ALGO_DEF void gda_insertion_sort_u64(u64* arr, int size);
GDA_ALGO_DEF void gda_insertion_sort_r32(r32* arr, int size);
GDA_ALGO_DEF void gda_insertion_sort_r64(r64* arr, int size);

GDA_ALGO_DEF void gda_insertion_sort_desc_s32(s32* arr, int size);
GDA_ALGO_DEF void gda_insertion_sort_desc_s64(s64* arr, int size);
GDA_ALGO_DEF void gda_insertion_sort_desc_u32(u32* arr, int size);
GDA_ALGO_DEF void gda_insertion_sort_desc_u64(u64* arr, int size);
GDA_ALGO_DEF void gda_insertion_sort_desc_r32(r32* arr, int size);
GDA_ALGO_DEF void gda_insertion_sort_desc_r64(r64* arr, int size);

/*Selection sort*/
GDA_ALGO_DEF void gda_selection_sort_s32(s32* arr, int size);
GDA_ALGO_DEF void gda_selection_sort_s64(s64* arr, int size);
GDA_ALGO_DEF void gda_selection_sort_u32(u32* arr, int size);
GDA_ALGO_DEF void gda_selection_sort_u64(u64* arr, int size);
GDA_ALGO_DEF void gda_selection_sort_r32(r32* arr, int size);
GDA_ALGO_DEF void gda_selection_sort_r64(r64* arr, int size);

GDA_ALGO_DEF void gda_selection_sort_desc_s32(s32* arr, int size);
GDA_ALGO_DEF void gda_selection_sort_desc_s64(s64* arr, int size);
GDA_ALGO_DEF void gda_selection_sort_desc_u32(u32* arr, int size);
GDA_ALGO_DEF void gda_selection_sort_desc_u64(u64* arr, int size);
GDA_ALGO_DEF void gda_selection_sort_desc_r32(r32* arr, int size);
GDA_ALGO_DEF void gda_selection_sort_desc_r64(r64* arr, int size);

/*Heap sort*/
static void gdaa__heapify_s32(s32* arr, int size);
static void gdaa__heapify_s64(s64* arr, int size);
static void gdaa__heapify_u32(u32* arr, int size);
static void gdaa__heapify_u64(u64* arr, int size);
static void gdaa__heapify_r32(r32* arr, int size);
static void gdaa__heapify_r64(r64* arr, int size);

static s32 gdaa__remove_top_s32(s32* arr, int size);
static s64 gdaa__remove_top_s64(s64* arr, int size);
static u32 gdaa__remove_top_u32(u32* arr, int size);
static u64 gdaa__remove_top_u64(u64* arr, int size);
static r32 gdaa__remove_top_r32(r32* arr, int size);
static r64 gdaa__remove_top_r64(r64* arr, int size);

GDA_ALGO_DEF void gda_heap_sort_s32(s32* arr, int size);
GDA_ALGO_DEF void gda_heap_sort_s64(s64* arr, int size);
GDA_ALGO_DEF void gda_heap_sort_u32(u32* arr, int size);
GDA_ALGO_DEF void gda_heap_sort_u64(u64* arr, int size);
GDA_ALGO_DEF void gda_heap_sort_r32(r32* arr, int size);
GDA_ALGO_DEF void gda_heap_sort_r64(r64* arr, int size);

GDA_ALGO_DEF void gda_heap_sort_desc_s32(s32* arr, int size);
GDA_ALGO_DEF void gda_heap_sort_desc_s64(s64* arr, int size);
GDA_ALGO_DEF void gda_heap_sort_desc_u32(u32* arr, int size);
GDA_ALGO_DEF void gda_heap_sort_desc_u64(u64* arr, int size);
GDA_ALGO_DEF void gda_heap_sort_desc_r32(r32* arr, int size);
GDA_ALGO_DEF void gda_heap_sort_desc_r64(r64* arr, int size);

/*Merge sort*/
static void gda__merge_rek_s32(s32* arr, s32 temp_arr, int left, int right);
static void gda__merge_rek_s64(s64* arr, s64 temp_arr, int left, int right);
static void gda__merge_rek_u32(u32* arr, u32 temp_arr, int left, int right);
static void gda__merge_rek_u64(u64* arr, u64 temp_arr, int left, int right);
static void gda__merge_rek_r32(r32* arr, r32 temp_arr, int left, int right);
static void gda__merge_rek_r64(r64* arr, r64 temp_arr, int left, int right);

static void gda__merge_rek_desc_s32(s32* arr, s32 temp_arr, int left, int right);
static void gda__merge_rek_desc_s64(s64* arr, s64 temp_arr, int left, int right);
static void gda__merge_rek_desc_u32(u32* arr, u32 temp_arr, int left, int right);
static void gda__merge_rek_desc_u64(u64* arr, u64 temp_arr, int left, int right);
static void gda__merge_rek_desc_r32(r32* arr, r32 temp_arr, int left, int right);
static void gda__merge_rek_desc_r64(r64* arr, r64 temp_arr, int left, int right);

GDA_ALGO_DEF void gda_merge_sort_s32(s32* arr, int size);
GDA_ALGO_DEF void gda_merge_sort_s64(s64* arr, int size);
GDA_ALGO_DEF void gda_merge_sort_u32(u32* arr, int size);
GDA_ALGO_DEF void gda_merge_sort_u64(u64* arr, int size);
GDA_ALGO_DEF void gda_merge_sort_r32(r32* arr, int size);
GDA_ALGO_DEF void gda_merge_sort_r64(r64* arr, int size);

GDA_ALGO_DEF void gda_merge_sort_desc_s32(s32* arr, int size);
GDA_ALGO_DEF void gda_merge_sort_desc_s64(s64* arr, int size);
GDA_ALGO_DEF void gda_merge_sort_desc_u32(u32* arr, int size);
GDA_ALGO_DEF void gda_merge_sort_desc_u64(u64* arr, int size);
GDA_ALGO_DEF void gda_merge_sort_desc_r32(r32* arr, int size);
GDA_ALGO_DEF void gda_merge_sort_desc_r64(r64* arr, int size);

#define GDA_ALGO_H
#endif


#ifdef GDA_ALGO_IMPLEMENTATION

#define GDAA_SWAP_MACRO(t, a1, a2) 	\
	t wtf_temp_ptr = *(a1);	\
	*(a1) = *(a2);	\
	*(a2) = wtf_temp_ptr;
void gdaa__swap_s32(s32* a1, s32* a2){ GDAA_SWAP_MACRO(s32, a1, a2) }
void gdaa__swap_s64(s64* a1, s64* a2){ GDAA_SWAP_MACRO(s64, a1, a2) }
void gdaa__swap_u32(u32* a1, u32* a2){ GDAA_SWAP_MACRO(u32, a1, a2) }
void gdaa__swap_u64(u64* a1, u64* a2){ GDAA_SWAP_MACRO(u64, a1, a2) }
void gdaa__swap_r32(r32* a1, r32* a2){ GDAA_SWAP_MACRO(r32, a1, a2) }
void gdaa__swap_r64(r64* a1, r64* a2){ GDAA_SWAP_MACRO(r64, a1, a2) }
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


/*Bubble sort implementation*/
#define GDAA_BUBBLE_SORT_MACRO(arr, sz, t, tn)	\
 	int wtf_iteration = 0;	\
	u32 not_sorted = 1;	\
	while(not_sorted){	\
		not_sorted = 0;	\
		for(int i = 1; i < sz - wtf_iteration; i++){	\
			if(arr[i] < arr[i - 1]){	\
				gdaa__swap_##t(&arr[i], &(arr[i - 1]));	\
				not_sorted = 1;	\
			}	\
		}	\
		wtf_iteration++;	\
	}	
#define GDAA_BUBBLE_SORT_DESC_MACRO(arr, sz, t, tn)	\
 	int wtf_iteration = 0;	\
	u32 not_sorted = 1;	\
	while(not_sorted){	\
		not_sorted = 0;	\
		for(int i = 1; i < sz - wtf_iteration; i++){	\
			if(arr[i] > arr[i - 1]){	\
				gdaa__swap_##t(&arr[i], &(arr[i - 1]));	\
				not_sorted = 1;	\
			}	\
		}	\
		wtf_iteration++;	\
	}	

void gda_bubble_sort_s32(s32* arr, int size){ GDAA_BUBBLE_SORT_MACRO(arr, size, s32) }
void gda_bubble_sort_s64(s64* arr, int size){ GDAA_BUBBLE_SORT_MACRO(arr, size, s64) }
void gda_bubble_sort_u32(u32* arr, int size){ GDAA_BUBBLE_SORT_MACRO(arr, size, u32) }
void gda_bubble_sort_u64(u64* arr, int size){ GDAA_BUBBLE_SORT_MACRO(arr, size, u64) }
void gda_bubble_sort_r32(r32* arr, int size){ GDAA_BUBBLE_SORT_MACRO(arr, size, r32) }
void gda_bubble_sort_r64(r64* arr, int size){ GDAA_BUBBLE_SORT_MACRO(arr, size, r64) }

void gda_bubble_sort_desc_s32(s32* arr, int size){ GDAA_BUBBLE_SORT_DESC_MACRO(arr, size, s32) }
void gda_bubble_sort_desc_s64(s64* arr, int size){ GDAA_BUBBLE_SORT_DESC_MACRO(arr, size, s64) }
void gda_bubble_sort_desc_u32(u32* arr, int size){ GDAA_BUBBLE_SORT_DESC_MACRO(arr, size, u32) }
void gda_bubble_sort_desc_u64(u64* arr, int size){ GDAA_BUBBLE_SORT_DESC_MACRO(arr, size, u64) }
void gda_bubble_sort_desc_r32(r32* arr, int size){ GDAA_BUBBLE_SORT_DESC_MACRO(arr, size, r32) }
void gda_bubble_sort_desc_r64(r64* arr, int size){ GDAA_BUBBLE_SORT_DESC_MACRO(arr, size, r64) }
#undef GDA_BUBBLE_SORT_MACRO
#undef GDA_BUBBLE_SORT_DESC_MACRO


/*Insertion sort implementation*/
#define GDAA_INSERTION_SORT_MACRO(arr, sz, t, tn)	\
	for (int i = 1; i < sz; i++){	\
		int j = i;	\
		while (j > 0 && arr[j] < arr[j - 1]){	\
			gdaa__swap_##t(&arr[j - 1], &arr[j]);	\
			j--;	\
		}	\
	}

#define GDAA_INSERTION_SORT_DESC_MACRO(arr, sz, t, tn)	\
	for (int i = 1; i < sz; i++){	\
		int j = i;	\
		while (j > 0 && arr[j] > arr[j - 1]){	\
			gdaa__swap_##t(&arr[j - 1], &arr[j]);	\
			j--;	\
		}	\
	}

void gda_insertion_sort_s32(s32* arr, int size){ GDAA_INSERTION_SORT_MACRO(arr, size, s32) }
void gda_insertion_sort_s64(s64* arr, int size){ GDAA_INSERTION_SORT_MACRO(arr, size, s64) }
void gda_insertion_sort_u32(u32* arr, int size){ GDAA_INSERTION_SORT_MACRO(arr, size, u32) }
void gda_insertion_sort_u64(u64* arr, int size){ GDAA_INSERTION_SORT_MACRO(arr, size, u64) }
void gda_insertion_sort_r32(r32* arr, int size){ GDAA_INSERTION_SORT_MACRO(arr, size, r32) }
void gda_insertion_sort_r64(r64* arr, int size){ GDAA_INSERTION_SORT_MACRO(arr, size, r64) }

void gda_insertion_sort_desc_s32(s32* arr, int size){GDAA_INSERTION_SORT_DESC_MACRO(arr, size, s32)}
void gda_insertion_sort_desc_s64(s64* arr, int size){GDAA_INSERTION_SORT_DESC_MACRO(arr, size, s64)}
void gda_insertion_sort_desc_u32(u32* arr, int size){GDAA_INSERTION_SORT_DESC_MACRO(arr, size, u32)}
void gda_insertion_sort_desc_u64(u64* arr, int size){GDAA_INSERTION_SORT_DESC_MACRO(arr, size, u64)}
void gda_insertion_sort_desc_r32(r32* arr, int size){GDAA_INSERTION_SORT_DESC_MACRO(arr, size, r32)}
void gda_insertion_sort_desc_r64(r64* arr, int size){GDAA_INSERTION_SORT_DESC_MACRO(arr, size, r64)}
#undef GDAA_INSERTION_SORT_MACRO
#undef GDAA_INSERTION_SORT_DESC_MACRO


/*Selection sort implementation*/
#define GDAA_SELECTION_SORT_MACRO(arr, sz, t, tn)	\
	for(int i = 0; i < sz - 1; i++){	\
		int wtf_min_index = i;	\
		for(int j = i + 1; j < sz; j++){	\
			if(arr[j] < arr[wtf_min_index]){	\
				wtf_min_index = j;	\
			}	\
		}	\
		if(arr[wtf_min_index] < arr[i]){	\
			gdaa__swap_##t(&arr[wtf_min_index], &arr[i]);	\
		}	\
	}

#define GDAA_SELECTION_SORT_DESC_MACRO(arr, sz, t, tn)	\
	for(int i = 0; i < sz - 1; i++){	\
		int wtf_min_index = i;	\
		for(int j = i + 1; j < sz; j++){	\
			if(arr[j] < arr[wtf_min_index]){	\
				wtf_min_index = j;	\
			}	\
		}	\
		if(arr[wtf_min_index] < arr[i]){	\
			gdaa__swap_##t(&arr[wtf_min_index], &arr[i]);	\
		}	\
	}

void gda_selection_sort_s32(s32* arr, int size){ GDAA_SELECTION_SORT_MACRO(arr, size, s32) }
void gda_selection_sort_s64(s64* arr, int size){ GDAA_SELECTION_SORT_MACRO(arr, size, s64) }
void gda_selection_sort_u32(u32* arr, int size){ GDAA_SELECTION_SORT_MACRO(arr, size, u32) }
void gda_selection_sort_u64(u64* arr, int size){ GDAA_SELECTION_SORT_MACRO(arr, size, u64) }
void gda_selection_sort_r32(r32* arr, int size){ GDAA_SELECTION_SORT_MACRO(arr, size, r32) }
void gda_selection_sort_r64(r64* arr, int size){ GDAA_SELECTION_SORT_MACRO(arr, size, r64) }

void gda_selection_sort_desc_s32(s32* arr, int size){ GDAA_SELECTION_SORT_DESC_MACRO(arr, size, s32) }
void gda_selection_sort_desc_s64(s64* arr, int size){ GDAA_SELECTION_SORT_DESC_MACRO(arr, size, s64) }
void gda_selection_sort_desc_u32(u32* arr, int size){ GDAA_SELECTION_SORT_DESC_MACRO(arr, size, u32) }
void gda_selection_sort_desc_u64(u64* arr, int size){ GDAA_SELECTION_SORT_DESC_MACRO(arr, size, u64) }
void gda_selection_sort_desc_r32(r32* arr, int size){ GDAA_SELECTION_SORT_DESC_MACRO(arr, size, r32) }
void gda_selection_sort_desc_r64(r64* arr, int size){ GDAA_SELECTION_SORT_DESC_MACRO(arr, size, r64) }
#undef GDAA_SELECTION_SORT_MACRO
#undef GDAA_SELECTION_SORT_DESC_MACRO


/*Heap sort implementation*/
#define GDAA_HEAPIFY_MACRO(arr, sz, t) 	\
	for(int i = 0; i < sz; i++){	\
		int wtf_index = i;	\
		while(wtf_index != 0){	\
			int parent_index = (wtf_index - 1) / 2;	\
			if(arr[parent_index] >= arr[wtf_index]){ break; }	\
			gdaa__swap_##t(&arr[parent_index], &arr[wtf_index]);	\
			wtf_index = parent_index;	\
		}	\
	}

void gdaa__heapify_s32(s32* arr, int size){ GDAA_HEAPIFY_MACRO(arr, size, s32) }
void gdaa__heapify_s64(s64* arr, int size){ GDAA_HEAPIFY_MACRO(arr, size, s64) }
void gdaa__heapify_u32(u32* arr, int size){ GDAA_HEAPIFY_MACRO(arr, size, u32) }
void gdaa__heapify_u64(u64* arr, int size){ GDAA_HEAPIFY_MACRO(arr, size, u64) }
void gdaa__heapify_r32(r32* arr, int size){ GDAA_HEAPIFY_MACRO(arr, size, r32) }
void gdaa__heapify_r64(r64* arr, int size){ GDAA_HEAPIFY_MACRO(arr, size, r64) }
#undef GDAA_HEAPIFY_MACRO

#define GDAA_REMOVE_TOP_MACRO(arr, sz, t)	\
	t top_element = arr[0];	\
	arr[0] = arr[sz - 1];	\
	int wtf_index = 0;		\
	while(1){				\
		int child1 = 2 * wtf_index + 1;			\
		int child2 = 2 * wtf_index + 2;			\
		if(child1 >= sz){ child1 = wtf_index;}	\
		if(child2 >= sz){ child2 = wtf_index;}	\
		if(arr[wtf_index] >= arr[child1] && arr[wtf_index] >= arr[child2]) {break;}	\
		int child_max_index = arr[child1] > arr[child2] ? child1 : child2;			\
		gdaa__swap_##t(&arr[wtf_index], &arr[child_max_index]);						\
		wtf_index = child_max_index;												\
	}																				\
	return(top_element);

s32 gdaa__remove_top_s32(s32* arr, int size){ GDAA_REMOVE_TOP_MACRO(arr, size, s32) }
s64 gdaa__remove_top_s64(s64* arr, int size){ GDAA_REMOVE_TOP_MACRO(arr, size, s64) }
u32 gdaa__remove_top_u32(u32* arr, int size){ GDAA_REMOVE_TOP_MACRO(arr, size, u32) }
u64 gdaa__remove_top_u64(u64* arr, int size){ GDAA_REMOVE_TOP_MACRO(arr, size, u64) }
r32 gdaa__remove_top_r32(r32* arr, int size){ GDAA_REMOVE_TOP_MACRO(arr, size, r32) }
r64 gdaa__remove_top_r64(r64* arr, int size){ GDAA_REMOVE_TOP_MACRO(arr, size, r64) }
#undef GDAA_REMOVE_TOP_MACRO

#define GDAA_HEAP_SORT_MACRO(arr, sz, t)	\
	int temp_size = sz;	\
	gdaa__heapify_##t(arr, sz);	\
	for(int i = 0; i < sz; i++){	\
		arr[size - 1 - i] = gdaa__remove_top_##t(arr, temp_size);	\
		temp_size = temp_size - 1;	\
	}

void gda_heap_sort_s32(s32* arr, int size){ GDAA_HEAP_SORT_MACRO(arr, size, s32) }
void gda_heap_sort_s64(s64* arr, int size){ GDAA_HEAP_SORT_MACRO(arr, size, s64) }
void gda_heap_sort_u32(u32* arr, int size){ GDAA_HEAP_SORT_MACRO(arr, size, u32) }
void gda_heap_sort_u64(u64* arr, int size){ GDAA_HEAP_SORT_MACRO(arr, size, u64) }
void gda_heap_sort_r32(r32* arr, int size){ GDAA_HEAP_SORT_MACRO(arr, size, r32) }
void gda_heap_sort_r64(r64* arr, int size){ GDAA_HEAP_SORT_MACRO(arr, size, r64) }
#undef GDAA_HEAP_SORT_MACRO

#define GDAA_HEAP_SORT_DESC_MACRO(arr, sz, t)	\
	int temp_size = sz;	\
	t* temp_arr = (t*)malloc(sz * sizeof(t));	\
	gdaa__heapify_##t(arr, sz);	\
	for(int i = 0; i < sz; i++){	\
		temp_arr[i] = gdaa__remove_top_##t(arr, temp_size);	\
		temp_size = temp_size - 1;	\
	}	\
	for(int i = 0; i < sz; i++){	\
		arr[i] = temp_arr[i];	\
	}	\
	free(temp_arr);

void gda_heap_sort_desc_s32(s32* arr, int size){ GDAA_HEAP_SORT_DESC_MACRO(arr, size, s32) }
void gda_heap_sort_desc_s64(s64* arr, int size){ GDAA_HEAP_SORT_DESC_MACRO(arr, size, s64) }
void gda_heap_sort_desc_u32(u32* arr, int size){ GDAA_HEAP_SORT_DESC_MACRO(arr, size, u32) }
void gda_heap_sort_desc_u64(u64* arr, int size){ GDAA_HEAP_SORT_DESC_MACRO(arr, size, u64) }
void gda_heap_sort_desc_r32(r32* arr, int size){ GDAA_HEAP_SORT_DESC_MACRO(arr, size, r32) }
void gda_heap_sort_desc_r64(r64* arr, int size){ GDAA_HEAP_SORT_DESC_MACRO(arr, size, r64) }
#undef GDAA_HEAP_SORT_DESC_MACRO

/*Merge sort implementation*/

#define GDA_MERGE_SORT_REK_MACRO(arr, temp_arr, left, right, t, compare_sign, merge_rek_func_)	\
	if((left) == (right)){return;}	\
	int midpoint = ((right)+(left)) / 2;	\
	merge_rek_func_##t(arr, temp_arr, (left), midpoint);	\
	merge_rek_func_##t(arr, temp_arr, (midpoint + 1), (right));	\
	int left_index = (left);	\
	int right_index = midpoint + 1;	\
	int wtf_index = left_index;	\
	while((left_index <= midpoint) && (right_index <= (right))){	\
		if(arr[left_index] compare_sign arr[right_index]){	\
			temp_arr[wtf_index] = arr[left_index];	\
			left_index = left_index + 1;	\
		}	\
		else{	\
			temp_arr[wtf_index] = arr[right_index];	\
			right_index = right_index + 1;	\
		}	\
		wtf_index = wtf_index + 1;	\
	}	\
	for(int i = left_index; i <= midpoint; i++){	\
		temp_arr[wtf_index] = arr[i];	\
		wtf_index = wtf_index + 1;	\
	}	\
	for(int i = right_index; i <= (right); i++){	\
		temp_arr[wtf_index] = arr[i];	\
		wtf_index = wtf_index + 1;	\
	}	\
	for(int i = (left); i <= (right); i++){	\
		arr[i] = temp_arr[i];	\
	}	

void gda__merge_rek_s32(s32* arr, s32* temp_arr, int left, int right){ GDA_MERGE_SORT_REK_MACRO(arr, temp_arr, left, right, s32, <=, gda__merge_rek_) }
void gda__merge_rek_s64(s64* arr, s64* temp_arr, int left, int right){ GDA_MERGE_SORT_REK_MACRO(arr, temp_arr, left, right, s64, <=, gda__merge_rek_) }
void gda__merge_rek_u32(u32* arr, u32* temp_arr, int left, int right){ GDA_MERGE_SORT_REK_MACRO(arr, temp_arr, left, right, u32, <=, gda__merge_rek_) }
void gda__merge_rek_u64(u64* arr, u64* temp_arr, int left, int right){ GDA_MERGE_SORT_REK_MACRO(arr, temp_arr, left, right, u64, <=, gda__merge_rek_) }
void gda__merge_rek_r32(r32* arr, r32* temp_arr, int left, int right){ GDA_MERGE_SORT_REK_MACRO(arr, temp_arr, left, right, r32, <=, gda__merge_rek_) }
void gda__merge_rek_r64(r64* arr, r64* temp_arr, int left, int right){ GDA_MERGE_SORT_REK_MACRO(arr, temp_arr, left, right, r64, <=, gda__merge_rek_) }

void gda__merge_rek_desc_s32(s32* arr, s32* temp_arr, int left, int right){ GDA_MERGE_SORT_REK_MACRO(arr, temp_arr, left, right, s32, >=, gda__merge_rek_desc_) }
void gda__merge_rek_desc_s64(s64* arr, s64* temp_arr, int left, int right){ GDA_MERGE_SORT_REK_MACRO(arr, temp_arr, left, right, s64, >=, gda__merge_rek_desc_) }
void gda__merge_rek_desc_u32(u32* arr, u32* temp_arr, int left, int right){ GDA_MERGE_SORT_REK_MACRO(arr, temp_arr, left, right, u32, >=, gda__merge_rek_desc_) }
void gda__merge_rek_desc_u64(u64* arr, u64* temp_arr, int left, int right){ GDA_MERGE_SORT_REK_MACRO(arr, temp_arr, left, right, u64, >=, gda__merge_rek_desc_) }
void gda__merge_rek_desc_r32(r32* arr, r32* temp_arr, int left, int right){ GDA_MERGE_SORT_REK_MACRO(arr, temp_arr, left, right, r32, >=, gda__merge_rek_desc_) }
void gda__merge_rek_desc_r64(r64* arr, r64* temp_arr, int left, int right){ GDA_MERGE_SORT_REK_MACRO(arr, temp_arr, left, right, r64, >=, gda__merge_rek_desc_) }
#undef MERGE_SORT_REK_MACRO

#define GDA_MERGE_SORT_MACRO(arr, sz, t)	\
	t* temp_array = (t*)malloc(sz * sizeof(t));	\
	gda__merge_rek_##t(arr, temp_array, 0, (sz - 1));	\
	free(temp_array);	

void gda_merge_sort_s32(s32* arr, int size){ GDA_MERGE_SORT_MACRO(arr, size, s32) }
void gda_merge_sort_s64(s64* arr, int size){ GDA_MERGE_SORT_MACRO(arr, size, s64) }
void gda_merge_sort_u32(u32* arr, int size){ GDA_MERGE_SORT_MACRO(arr, size, u32) }
void gda_merge_sort_u64(u64* arr, int size){ GDA_MERGE_SORT_MACRO(arr, size, u64) }
void gda_merge_sort_r32(r32* arr, int size){ GDA_MERGE_SORT_MACRO(arr, size, r32) }
void gda_merge_sort_r64(r64* arr, int size){ GDA_MERGE_SORT_MACRO(arr, size, r64) }
#undef GDA_MERGE_SORT_MACRO

#define GDA_MERGE_SORT_DESC_MACRO(arr, sz, t)	\
	t* temp_array = (t*)malloc(sz * sizeof(t));	\
	gda__merge_rek_desc_##t(arr, temp_array, 0, (sz - 1));	\
	free(temp_array);	

void gda_merge_sort_desc_s32(s32* arr, int size){ GDA_MERGE_SORT_DESC_MACRO(arr, size, s32) }
void gda_merge_sort_desc_s64(s64* arr, int size){ GDA_MERGE_SORT_DESC_MACRO(arr, size, s64) }
void gda_merge_sort_desc_u32(u32* arr, int size){ GDA_MERGE_SORT_DESC_MACRO(arr, size, u32) }
void gda_merge_sort_desc_u64(u64* arr, int size){ GDA_MERGE_SORT_DESC_MACRO(arr, size, u64) }
void gda_merge_sort_desc_r32(r32* arr, int size){ GDA_MERGE_SORT_DESC_MACRO(arr, size, r32) }
void gda_merge_sort_desc_r64(r64* arr, int size){ GDA_MERGE_SORT_DESC_MACRO(arr, size, r64) }
#undef GDA_MERGE_SORT_DESC_MACRO

#endif