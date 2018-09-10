#ifndef ECURVA_H
#define ECURVA_H

#include "stdint.h"

typedef uint16_t word;

/*!	\brief Булев тип */
typedef int bool_t;

#ifndef TRUE
	#define TRUE ((bool_t)1)
#endif

#ifndef FALSE
	#define FALSE ((bool_t)0)
#endif

typedef unsigned char octet;

#ifndef LITTLE_ENDIAN
	#define LITTLE_ENDIAN 1234
#endif

#ifndef BIG_ENDIAN
	#define BIG_ENDIAN 4321
#endif

#if defined(__M_IX86) || defined(_X86_) || defined(i386) ||\
	defined(__i386__) || defined(_M_I86)  || defined(_M_IX86) ||\
	defined(_M_IA64) || defined(__ia64__) || defined(_M_X64) ||\
	defined(_M_AMD64) || defined(__amd64__) || defined(__amd64) ||\
	defined(__x86_64__) ||\
	defined(_M_ALPHA) || defined(__alpha__) || defined(__alpha) ||\
    defined(__arm__) ||\
    defined(__MIPS__) || defined(__mips__) || defined(__mips) ||\
	defined(__OS2__) || defined(sun386) || defined(__TURBOC__) ||\
	defined(vax) || defined(vms) || defined(VMS) || defined(__VMS)
	#define OCTET_ORDER LITTLE_ENDIAN
#elif defined(__powerpc__) || defined(__ppc__) || defined(__PPC__) ||\
	defined(__powerpc64__) || defined(__ppc64__) || defined(__PPC64__) ||\
	defined(AMIGA) || defined(applec) || defined(__AS400__) ||\
	defined(_CRAY) || defined(__hppa) || defined(__hp9000) ||\
	defined(ibm370) || defined(mc68000) || defined(m68k) ||\
	defined(__MRC__) || defined(__MVS__) || defined(__MWERKS__) ||\
	defined(sparc) || defined(__sparc) || defined(SYMANTEC_C) ||\
	defined(__VOS__) || defined(__TIGCC__) || defined(__TANDEM) ||\
	defined(THINK_C) || defined(__VMCMS__) || defined(_AIX)
	#define OCTET_ORDER BIG_ENDIAN
#else
	#error "Platform undefined"
#endif


#if defined(_WIN32) || defined(_WIN64) || defined(_WINNT) ||\
	defined(__WIN32__) || defined(__WIN64__)  || defined(__WINNT__)
	#define OS_WIN
	#undef OS_UNIX
	#undef OS_LINUX
	#undef OS_APPLE
#elif defined(unix) || defined(_unix_) || defined(__unix__) ||\
	defined(__APPLE__)
	#undef OS_WIN
	#define OS_UNIX
	#if defined(__unix__)
		#define OS_LINUX
		#undef OS_APPLE
	#elif defined(__APPLE__)
		#undef OS_LINUX
		#define OS_APPLE
	#endif
#else
	#undef OS_WIN
	#undef OS_UNIX
	#undef OS_LINUX
	#undef OS_APPLE
#endif



#undef U8_SUPPORT
#undef U16_SUPPORT
#undef U32_SUPPORT
#undef U64_SUPPORT
#undef U128_SUPPORT

#if (UCHAR_MAX == 255)
	typedef unsigned char u8;
	typedef signed char i8;
	typedef u8 octet;
	#define U8_SUPPORT
#else
	#error "Unsupported char size"
#endif

#if (USHRT_MAX == 65535)
	typedef unsigned short u16;
	typedef signed short i16;
	#define U16_SUPPORT
#else
	#error "Unsupported short size"
#endif

#if (UINT_MAX == 65535u)
	#if (ULONG_MAX == 4294967295ul)
		typedef unsigned long u32;
		typedef signed long i32;
		#define U32_SUPPORT
	#else
		#error "Unsupported long size"
	#endif
#elif (UINT_MAX == 4294967295u)
	typedef unsigned int u32;
	typedef signed int i32;
	#define U32_SUPPORT
	#if (ULONG_MAX == 4294967295ul)
		#if !defined(ULLONG_MAX) || (ULLONG_MAX == 4294967295ull)
			#error "Unsupported int/long/long long configuration"
		#elif (ULLONG_MAX == 18446744073709551615ull)
			typedef unsigned long long u64;
			typedef signed long long i64;
			#define U64_SUPPORT
		#else
			#error "Unsupported int/long/long long configuration"
		#endif
	#elif (ULONG_MAX == 18446744073709551615ul)
		typedef unsigned long u64;
		typedef signed long i64;
		#define U64_SUPPORT
		#if defined(__GNUC__) && (__WORDSIZE == 64)
			typedef __int128 i128;
			typedef unsigned __int128 u128;
			#define U128_SUPPORT
		#endif
	#else
		#error "Unsupported int/long configuration"
	#endif
#elif (UINT_MAX == 18446744073709551615u)
	#if (ULONG_MAX == 18446744073709551615ul)
		#if !defined(ULLONG_MAX) || (ULLONG_MAX == 18446744073709551615ull)
			#error "Unsupported int/long/long long configuration"
		#elif (ULLONG_MAX == 340282366920938463463374607431768211455ull)
			typedef unsigned long long u128;
			typedef signed long long i128;
			#define U128_SUPPORT
		#else
			#error "Unsupported int/long/long long configuration"
		#endif
	#elif (ULONG_MAX == 340282366920938463463374607431768211455ul)
		typedef unsigned long u128;
		typedef signed long i128;
		#define U128_SUPPORT
	#else
		#error "Unsupported long size"
	#endif
#else
	#error "Unsupported int size"
#endif

#if !defined(U8_SUPPORT) || !defined(U16_SUPPORT) || !defined(U32_SUPPORT)
	#error "One of the base types is not supported"
#endif


#if defined(__WORDSIZE)
	#if (__WORDSIZE == 16)
		#define B_PER_W 16
		typedef u16 word;
		typedef u32 dword;
	#elif (__WORDSIZE == 32)
		#define B_PER_W 32
		typedef u32 word;
		typedef u64 dword;
	#elif (__WORDSIZE == 64)
		#define B_PER_W 64
		typedef u64 word;
		typedef u128 dword;
	#else
		#error "Unsupported word size"
	#endif
#else
	#if (UINT_MAX == 65535u)
		#define B_PER_W 16
		typedef u16 word;
		typedef u32 dword;
	#elif (UINT_MAX == 4294967295u)
		#define B_PER_W 32
		typedef u32 word;
		typedef u64 dword;
	#elif (UINT_MAX == 18446744073709551615u)
		#define B_PER_W 64
		typedef u64 word;
		typedef u128 dword;
	#else
		#error "Unsupported word size"
	#endif
#endif

#if (B_PER_W != 16 && B_PER_W != 32 && B_PER_W != 64)
	#error "Unsupported word size"
#endif

#define O_PER_W (B_PER_W / 8)
#define O_PER_S (B_PER_S / 8)

#define SIZE_0 ((size_t)0)
#define SIZE_1 ((size_t)1)
#ifndef SIZE_MAX
	#define SIZE_MAX ((size_t)(SIZE_0 - SIZE_1))
#endif

#if (SIZE_MAX == 65535u)
	#define B_PER_S 16
#elif (SIZE_MAX == 4294967295u)
	#define B_PER_S 32
#elif (SIZE_MAX == 18446744073709551615u)
	#define B_PER_S 64
#else
	#error "Unsupported size_t size"
#endif


/*
*******************************************************************************
Ошибки
*******************************************************************************
*/

/*!	\brief Коды ошибок
	\remark Высокоуровневые функции возвращают значения типа err_t.
	Возврат ERR_OK означает, что функция завершена успешно. Код ERR_MAX
	зарезервирован для описания специальных особых ситуаций.
	Возврат других значений означает ошибку при выполнении функции.
*/
typedef u32 err_t;

/*!	\brief Код успешного завершения */
#define ERR_OK	((err_t)0)

/*!	\brief Максимальный код ошибки */
#define ERR_MAX	(ERR_OK - (err_t)1)




/*!	\brief Число октетов для размещения nb битов */
#define O_OF_B(nb) (((nb) + 7) / 8)

/*!	\brief Число машинных слов для размещения nb битов */
#define W_OF_B(nb) (((nb) + B_PER_W - 1) / B_PER_W)

/*!	\brief Число битов для размещения no октетов */
#define B_OF_O(no) ((no) * 8)

/*!	\brief Число машинных слов для размещения no октетов */
#define W_OF_O(no) (((no) + O_PER_W - 1) / O_PER_W)

/*!	\brief Число октетов для размещения nw машинных слов */
#define O_OF_W(nw) ((nw) * O_PER_W)

/*!	\brief Число битов для размещения nw машинных слов */
#define B_OF_W(nw) ((nw) * B_PER_W)


#ifdef SAFE_FAST
	#define SAFE(tag) tag##_safe
	#define FAST(tag) tag
#else
	#define SAFE(tag) tag
	#define FAST(tag) tag##_fast
#endif

#define WORD_0 ((word)0)
#define WORD_1 ((word)1)
#define WORD_MAX ((word)(WORD_0 - WORD_1))

#define WORD_BIT_POS(pos) (WORD_1 << (pos))
#define WORD_BIT_HI WORD_BIT_POS(B_PER_W - 1)
#define WORD_BIT_HALF WORD_BIT_POS(B_PER_W / 2)

#if (B_PER_W == 16)
/*!
*******************************************************************************
\file u16.h
Реализованы операции над 16-разрядными словами и массивами таких слов.
\pre В функции передаются корректные буферы памяти.
*******************************************************************************
*/

#define U16_0 ((u16)0)
#define U16_1 ((u16)1)
#define U16_MAX ((u16)(U16_0 - U16_1))

/*!	\def u16RotHi
	\brief Циклический сдвиг слова u16 на d позиций в сторону старших разрядов
	\pre 0 < d < 16.
*/
#define u16RotHi(w, d)\
	((u16)((w) << d | (w) >> (16 - d)))

/*!	\def u16RotLo
	\brief Циклический сдвиг слова u16 на d позиций в сторону младших разрядов
	\pre 0 < d < 16.
*/
#define u16RotLo(w, d)\
	((u16)((w) >> d | (w) << (16 - d)))

/*!	\def u16Rev
	\brief Реверс октетов слова u16
*/
#define u16Rev(w)\
	((u16)((w) << 8 | (w) >> 8))

/*!	\brief Реверс октетов
	Выполняется реверс октетов слов u16 массива [count]buf.
*/
void u16Rev2(
	u16 buf[],			/*!< [in/out] приемник */
	size_t count		/*!< [in] число элементов */
);

/*!	\brief Загрузка из буфера памяти
	Буфер [count]src преобразуется в массив [(count + 1) / 2]dest слов u16.
*/
void u16From(
	u16 dest[],			/*!< [out] приемник */
	const void* src,	/*!< [in] источник */
	size_t count		/*!< [in] число октетов */
);

/*!	\brief Выгрузка в буфер памяти
	Буфер [count]dest формируется по массиву [(count + 1) / 2]src слов u16.
*/
void u16To(
	void* dest,			/*!< [out] приемник */
	size_t count,		/*!< [in] число октетов */
	const u16 src[]		/*!< [in] источник */
);
	#define wordRev u16Rev
#elif (B_PER_W == 32)

/*!
*******************************************************************************
\file u32.h
Реализованы операции над 32-разрядными словами и массивами таких слов.
\pre В функции передаются корректные буферы памяти.
*******************************************************************************
*/

#define U32_0 ((u32)0)
#define U32_1 ((u32)1)
#define U32_MAX ((u32)(U32_0 - U32_1))

/*!	\def u32RotHi
	\brief Циклический сдвиг слова u32 на d позиций в сторону старших разрядов
	\pre 0 < d < 32.
*/
#define u32RotHi(w, d)\
	((u32)((w) << d | (w) >> (32 - d)))

/*!	\def u32RotLo
	\brief Циклический сдвиг слова u32 на d позиций в сторону младших разрядов
	\pre 0 < d < 32.
*/
#define u32RotLo(w, d)\
	((u32)((w) >> d | (w) << (32 - d)))

/*!	\def u32Rev
	\brief Реверс октетов слова u32
*/
#define u32Rev(w)\
	((u32)((w) << 24 | ((w) & 0xFF00) << 8 | ((w) >> 8 & 0xFF00) | (w) >> 24))

/*!	\brief Реверс октетов
	Выполняется реверс октетов слов u32 массива [count]buf.
*/
void u32Rev2(
	u32 buf[],			/*!< [in/out] приемник */
	size_t count		/*!< [in] число элементов */
);

/*!	\brief Загрузка из буфера памяти
	Буфер [count]src преобразуется в массив [(count + 3) / 4]dest слов u32.
*/
void u32From(
	u32 dest[],			/*!< [out] приемник */
	const void* src,	/*!< [in] источник */
	size_t count		/*!< [in] число октетов */
);

/*!	\brief Выгрузка в буфер памяти
	Буфер [count]dest формируется по массиву [(count + 3) / 4]src слов u32.
*/
void u32To(
	void* dest,			/*!< [out] приемник */
	size_t count,		/*!< [in] число октетов */
	const u32 src[]		/*!< [in] источник */
);

	#define wordRev u32Rev
#elif (B_PER_W == 64)

/*!
*******************************************************************************
\file u64.h
Реализованы операции над 64-разрядными словами и массивами таких слов.
\pre В функции передаются корректные буферы памяти.
*******************************************************************************
*/

#define U64_0 ((u64)0)
#define U64_1 ((u64)1)
#define U64_MAX ((u64)(U64_0 - U64_1))

/*!	\def u64RotHi
	\brief Циклический сдвиг слова u64 на d позиций в сторону старших разрядов
	\pre 0 < d < 64.
*/
#define u64RotHi(w, d)\
	((u64)((w) << d | (w) >> (64 - d)))

/*!	\def u64RotLo
	\brief Циклический сдвиг слова u32 на d позиций в сторону младших разрядов
	\pre 0 < d < 64.
*/
#define u64RotLo(w, d)\
	((u64)((w) >> d | (w) << (64 - d)))

/*!	\def u64Rev
	\brief Реверс октетов слова u64
*/
#define u64Rev(w)\
	((u64)((w) << 56 | ((w) & 0xFF00) << 40 | ((w) & 0xFF0000) << 24 |\
	((w) & 0xFF000000) << 8 | ((w) >> 8 & 0xFF000000) |\
	((w) >> 24 & 0xFF0000) | ((w) >> 40 & 0xFF00) | (w) >> 56))

/*!	\brief Реверс октетов
	Выполняется реверс октетов слов u64 массива [count]buf.
*/
void u64Rev2(
	u64 buf[],			/*!< [in/out] приемник */
	size_t count		/*!< [in] число элементов */
);

/*!	\brief Загрузка из буфера памяти
	Буфер [count]src преобразуется в массив [(count + 7) / 8]dest слов u64.
*/
void u64From(
	u64 dest[],			/*!< [out] приемник */
	const void* src,	/*!< [in] источник */
	size_t count		/*!< [in] число октетов */
);

/*!	\brief Выгрузка в буфер памяти
	Буфер [count]dest формируется по массиву [(count + 7) / 8]src слов u64.
*/
void u64To(
	void* dest,			/*!< [out] приемник */
	size_t count,		/*!< [in] число октетов */
	const u64 src[]		/*!< [in] источник */
);
	#define wordRev u64Rev
#else
	#error "Unsupported word size"
#endif /* B_PER_W */

#define wordEq(a, b) ((word)(a) == (word)(b))
#define wordNeq(a, b) ((word)(a) != (word)(b))
#define wordLess(a, b) ((word)(a) < (word)(b))
#define wordLeq(a, b) ((word)(a) <= (word)(b))
#define wordGreater(a, b) wordLess(b, a)
#define wordGeq(a, b) wordLeq(b, a)

#define wordEq01(a, b) ((word)wordEq(a, b))
#define wordNeq01(a, b) ((word)wordNeq(a, b))
#define wordLess01(a, b) ((word)wordLess(a, b))
#define wordLeq01(a, b) ((word)wordLeq(a, b))
#define wordGreater01(a, b) ((word)wordGreater(a, b))
#define wordGeq01(a, b) ((word)wordGeq(a, b))

#define wordEq0M(a, b) ((word)(wordNeq01(a, b) - WORD_1))
#define wordNeq0M(a, b) ((word)(wordEq01(a, b) - WORD_1))
#define wordLess0M(a, b) ((word)(wordGeq01(a, b) - WORD_1))
#define wordLeq0M(a, b) ((word)(wordGreater01(a, b) - WORD_1))
#define wordGreater0M(a, b) ((word)(wordLeq01(a, b) - WORD_1))
#define wordGeq0M(a, b) ((word)(wordLess01(a, b) - WORD_1))

/*
*******************************************************************************
Стандартные функции
*******************************************************************************
*/

/*!	\brief Копировать буфер памяти
	Октеты буфера [count]src переписываются в буфер [count]dest.
	\pre Буферы src и dest не пересекаются.
*/
void memCopy( 
	void* dest,			/*< [out] буфер-назначение */
	const void* src,	/*< [in] буфер-источник */
	size_t count		/*< [in] число октетов */
);

/*!	\brief Переместить буфер памяти
	Октеты буфера [count]src перемещаются в буфер [count]dest.
	\pre Буферы src и dest могут пересекаться.
*/
void memMove( 
	void* dest,			/*< [out] буфер-назначение */
	const void* src,	/*< [in] буфер-источник */
	size_t count		/*< [in] число октетов */
);

/*!	\brief Заполнить буфер памяти
	Буфер [count]buf заполняется октетом c.
*/
void memSet( 
	void* buf,			/*< [out] буфер */
	octet c,			/*< [in] октет-значение */
	size_t count		/*< [in] число октетов */
);

/*!	Буфер [count]buf обнуляется. */
#define memSetZero(buf, count) memSet(buf, 0, count)

/*!	\brief Выделение блока памяти
	Выделяется блок динамической памяти из count октетов.
	\return Указатель на блок памяти или 0, если памяти не хватает.
	\remark Блок выделяется, даже если count == 0.
*/
void* memAlloc(
	size_t count		/*!< [in] размер блока */
);

/*!	\brief Изменение размера блока памяти
	Размер блока динамической памяти buf устанавливается равным count. 
	При необходимости блок перемещается в памяти. Содержимое блока 
	максимально сохраняется. 
	\return Указатель на блок памяти с новым размером 
	или 0, если count == 0 или памяти не хватает.
	\remark memRealloc(buf, 0) равносильно memFree(buf).
*/
void* memRealloc(
	void* buf,		/*!< [in] блок памяти */
	size_t count	/*!< [in] размер блока */
);

/*!	\brief Освобождение блока памяти
	Освобождается блок динамической памяти buf.
	\pre buf выделен с помощью memAlloc() или memRealloc().
*/
void memFree(
	void* buf		/*!< [in] буфер */
);

/*
*******************************************************************************
Дополнительные функции
*******************************************************************************
*/

/*!	\brief Корректный буфер памяти?
	Проверяется, что [count]buf является корректным буфером.
	\return Проверяемый признак.
	\remark Нулевой указатель buf является корректным, если count == 0.
*/
bool_t memIsValid(
	const void* buf,	/*!< [in] буфер */
	size_t count		/*!< [in] размер буфера */
);

/*!	\def memIsNullOrValid
	\brief Нулевой указатель или корректный буфер памяти? 
*/
#define memIsNullOrValid(buf, count)\
	((buf) == 0 || memIsValid(buf, count))

/*!	\brief Проверка совпадения
	Проверяется, что содержимое буферов [count]buf1 и [count]buf2 совпадает. 
	\return Признак совпадения.
	\safe Имеется ускоренная нерегулярная редакция.
*/
bool_t memEq(
	const void* buf1,	/*!< [in] первый буфер */
	const void* buf2,	/*!< [in] второй буфер */
	size_t count		/*!< [in] размер буферов */
);

bool_t FAST(memEq)(const void* buf1, const void* buf2, size_t count);

/*!	\brief Сравнение
	Буферы [count]buf1 и [count]buf2 сравниваются обратно-лексикографически.
	\return < 0, если [count]buf1 < [count]buf2, 
	0, если [count]buf1 == [count]buf2, 
	> 0, если [count]buf1 > [count]buf2.
	\remark Октеты буферов сравниваются последовательно, от последнего 
	к первому. Первое несовпадение задает соотношение между буферами.
	\warning Стандартная функция memcmp() сравнивает октеты от первого 
	к последнему.
	\safe Имеется ускоренная нерегулярная редакция.
*/
int memCmp(
	const void* buf1,	/*!< [in] первый буфер */
	const void* buf2,	/*!< [in] второй буфер */
	size_t count		/*!< [in] размер буферов */
);

int FAST(memCmp)(const void* buf1, const void* buf2, size_t count);

/*!	\brief Очистить буфер памяти
	Буфер [count]buf очищается -- в него записываются произвольные октеты.
	\remark Запись выполняется всегда, даже если buf в дальнейшем не
	используется и включена оптимизация компиляции.
*/
void memWipe(
	void* buf,	        /*!< [out] буфер */
	size_t count		/*!< [in] размер буфера */
);

/*!	\brief Нулевой буфер памяти?
	Проверяется, что буфер [count]buf является нулевым.
	\return Проверяемый признак.
	\safe Имеется ускоренная нерегулярная редакция.
*/
bool_t memIsZero(
	const void* buf,	/*!< [out] буфер */
	size_t count		/*!< [in] размер буфера */
);

bool_t FAST(memIsZero)(const void* buf, size_t count);

/*!	\brief Размер значащей части буфера
	Определяется размер значащей части буфера [count]buf.
	Незначащими считаются последние нулевые октеты буфера вплоть до первого
	ненулевого.
	\return Размер значащей части в октетах.
	\safe Функция нерегулярна: время выполнения зависит от заполнения buf.
*/
size_t memNonZeroSize(
	const void* buf,	/*!< [out] буфер */
	size_t count		/*!< [in] размер буфера */
);

/*!	\brief Повтор октета?
	Проверяется, что [count]buf заполнен октетом o.
	\remark Считается, что в пустом буфере (count == 0) повторяется значение 0.
	\return Признак успеха.
	\safe Имеется ускоренная нерегулярная редакция.
*/
bool_t memIsRep(
	const void* buf,	/*!< [in] буфер */
	size_t count,		/*!< [in] размер буфера */
	octet o				/*!< [in] значение */
);

bool_t FAST(memIsRep)(const void* buf, size_t count, octet o);

/*!	\brief Объединение двух буферов
	В dest записывается блок [count1]src1 || [count2]src2.
	\pre По адресам src1, src2, dest зарезервировано count1, count2 и
	count1 +  count2 октетов памяти соответственно.
	\remark Буферы src1, src2 и dest могут пересекаться.
*/
void memJoin(
	void* dest,			/*!< [out] назначение */
	const void* src1,	/*!< [in] первый источник */
	size_t count1,		/*!< [in] число октетов src1 */
	const void* src2,	/*!< [in] второй источник */
	size_t count2		/*!< [in] число октетов src2 */
);

/*!	\brief Буферы одинакового размера не пересекаются?
	Проверяется, что буфер [count]buf1 не пересекается с буфером [count]buf2.
	\return Проверяемый признак.
	\pre Буферы buf1 и buf2 корректны.
*/
bool_t memIsDisjoint(
	const void* buf1,	/*!< [out] первый буфер */
	const void* buf2,	/*!< [out] второй буфер */
	size_t count		/*!< [in] размер буферов */
);

/*!	\brief Буферы совпадают или не пересекаются?
	Проверяется, что буфер [count]buf1 совпадает или не пересекается с буфером 
	[count]buf2.
	\return Проверяемый признак.
	\pre Буферы buf1 и buf2 корректны.
*/
bool_t memIsSameOrDisjoint(
	const void* buf1,	/*!< [out] первый буфер */
	const void* buf2,	/*!< [out] второй буфер */
	size_t count		/*!< [in] размер буферов */
);

/*!	\brief Два буфера не пересекаются?
	Проверяется, что буфер [count1]buf1 не пересекается с буфером [count2]buf2.
	\return Проверяемый признак.
	\pre Буферы buf1 и buf2 корректны.
*/
bool_t memIsDisjoint2(
	const void* buf1,	/*!< [out] первый буфер */
	size_t count1,		/*!< [in] размер buf1 */
	const void* buf2,	/*!< [out] второй буфер */
	size_t count2		/*!< [in] размер buf2 */
);

/*!	\brief Три буфера не пересекаются?
	Проверяется, что буферы [count1]buf1, [count2]buf2 и [count3]buf3 
	попарно не пересекаются.
	\return Проверяемый признак.
	\pre Буферы buf1, buf2 и buf3 корректны.
*/
bool_t memIsDisjoint3(
	const void* buf1,	/*!< [out] первый буфер */
	size_t count1,		/*!< [in] размер buf1 */
	const void* buf2,	/*!< [out] второй буфер */
	size_t count2,		/*!< [in] размер buf2 */
	const void* buf3,	/*!< [out] третий буфер */
	size_t count3		/*!< [in] размер buf3 */
);

/*!	\brief Четыре буфера не пересекаются?
	Проверяется, что буферы [count1]buf1, [count2]buf2, [count3]buf3 
	и [count4]buf4 попарно не пересекаются.
	\return Проверяемый признак.
	\pre Буферы buf1, buf2, buf3 и buf4 корректны.
*/
bool_t memIsDisjoint4(
	const void* buf1,	/*!< [out] первый буфер */
	size_t count1,		/*!< [in] размер buf1 */
	const void* buf2,	/*!< [out] второй буфер */
	size_t count2,		/*!< [in] размер buf2 */
	const void* buf3,	/*!< [out] третий буфер */
	size_t count3,		/*!< [in] размер buf3 */
	const void* buf4,	/*!< [out] четвертый буфер */
	size_t count4		/*!< [in] размер buf4 */
);

/*!	\brief Cложение октетов памяти по модулю 2
	В буфер [count]dest записывается поразрядная по модулю 2 сумма октетов
	октетов буферов [count]src1 и [count]src2.
	\pre Буфер dest либо не пересекается, либо совпадает с каждым из
	буферов src1, src2.
*/
void memXor(
	void* dest,			/*!< [out] сумма */
	const void* src1,	/*!< [in] первое слагаемое */
	const void* src2,	/*!< [in] второе слагаемое */
	size_t count		/*!< [in] число октетов */
);

/*!	\brief Добавление октетов памяти по модулю 2
	К октетам буфера [count]dest добавляются октеты буфера [count]src. 
	Сложение выполняется поразрядно по модулю 2.
	\pre Буфер dest либо не пересекается, либо совпадает с буфером src.
*/
void memXor2(
	void* dest,			/*!< [in/out] второе слагаемое / сумма */
	const void* src,	/*!< [in] первое слагаемое */
	size_t count		/*!< [in] число октетов */
);

/*!	\brief Перестановка октетов памяти
	Октеты буферов [count]buf1 и [count]buf2 меняются местами. 
	\pre Буферы buf1 и buf2 не пересекаются.
*/
void memSwap(
	void* buf1,		/*!< [in/out] первый буфер */
	void* buf2,		/*!< [in/out] второй буфер */
	size_t count	/*!< [in] число октетов */
);

/*
*******************************************************************************
Реверс октетов
*******************************************************************************
*/

/*!	\brief Реверс октетов
	Октеты буфера [count]buf записываются в обратном порядке.
*/
void memRev(
	void* buf,		/*!< [out] буфер */
	size_t count	/*!< [in] размер буфера */
);


/*
*******************************************************************************
Вспомогательные макросы
*******************************************************************************
*/

/*!	\brief Число элементов в массиве a */
#define COUNT_OF(a) (sizeof(a) / sizeof(*(a)))

/*!	\brief Число элементов в массиве a
	\pre Массив непустой.
*/
#define LAST_OF(a) ((a)[COUNT_OF(a) - 1])

/*!	\brief Компиляция с проверкой условия
	
	Для отладочной версии вычислить e и завершить компиляцию, если e == 0.
*/
#ifdef NDEBUG
	#define CASSERT(e) ((void)0)
#else
	#define CASSERT(e) ((void)sizeof(char[1 - 2 * !(e)]))
#endif

/*!	\brief Предполагается выполнение условия
	
	Для отладочной версии вычислить e и завершить выполнение, если e == 0.
	\remark Используется собственная редакция макроса assert(). В стандартной 
	редакции есть проверка условия. При правильной работе программы условие
	никогда не выполняется, и анализаторы покрытия тестами могут показывать 
	только частичное покрытие.
*/
#ifdef NDEBUG
	#define ASSERT(e) ((void)0)
#else
	extern void utilAssert(int e, const char* file, int line);
	#define ASSERT(e) utilAssert(!!(e), __FILE__, __LINE__)
#endif

/*!	\brief Проверяется выполнение условия
	Вычислить e (всегда) и завершить выполнение, если a == 0 (при отладке). 
*/
#define VERIFY(e) {if (!(e)) ASSERT(0);}

/*!	\brief Ожидается выполнение условия
	Ожидать выполнения a, ничего не предпринимая. 
	\remark Макрос EXPECT указывает на условия, которые ожидаются, 
	но все таки могут быть нарушены. Примеры условий: простота числа, 
	неприводимость многочлена, корректность эллиптической кривой. 
	\remark Ожидаемые условия могут быть труднопроверяемыми. Поэтому
	программы не могут полагаться на безусловное выполнение этих условий 
	и должны устойчиво работать даже при их нарушении. Например, 
	программа сложения точек эллиптической кривой над простым полем GF(p) 
	должна завершать сложение даже если p -- составное.
	\remark Следует четко разграничивать ASSERT (как правило, самоконтроль 
	программиста) и EXPECT (как правило, контроль входных данных).
	\remark Некоторые ожидаемые условия могут частично проверяться. 
	Например, EXPECT(p -- нечетное простое) может быть поддержано проверкой 
	ASSERT(p -- нечетное). Безусловные проверки, поддерживающие ожидаемые 
	условия, должны по возможности документироваться.
*/
#define EXPECT(a) 



/*!	\brief Импорт из аффинной точки

	По аффинной точке [2 * ec->f->n]a эллиптической кривой ec строится 
	точка [ec->d * ec->f->n]b.
	\pre Описание ec работоспособно.
	\pre Буферы a и b либо не пересекаются, либо указатели a и b совпадают.
	\pre Координаты a лежат в базовом поле.
	\expect Описание ec корректно.
	\expect Точка a лежит на кривой.
	\return TRUE, если преобразование успешно выполнено, и FALSE в противном 
	случае.
*/
typedef bool_t (*ec_froma_i)(
	word b[],				/*!< [out] выходная точка */
	const word a[],			/*!< [in] аффинная точка */
	const struct ec_o* ec,	/*!< [in] описание эллиптической кривой */
	void* stack				/*!< [in] вспомогательная память */
);

/*!	\brief Экспорт в аффинную точку

	По точке [ec->d * ec->f->n]a эллиптической кривой ec строится 
	аффинная точка [2 * ec->f->n]b.
	\pre Описание ec работоспособно.
	\pre Буферы a и b либо не пересекаются, либо указатели a и b совпадают.
	\pre Координаты a лежат в базовом поле.
	\expect Описание ec корректно.
	\expect Точка a лежит на кривой.
	\return TRUE, если аффинная точка построена, и FALSE, если точке a 
	соответствует бесконечно удаленная точка.
*/
typedef bool_t (*ec_toa_i)(
	word b[],				/*!< [out] аффинная точка */
	const word a[],			/*!< [in] входная точка */
	const struct ec_o* ec,	/*!< [in] описание эллиптической кривой */
	void* stack				/*!< [in] вспомогательная память */
);

/*!	\brief Обратная точка

	На эллиптической кривой ec определяется точка [ec->d * ec->f->n]b,
	обратная к точке [ec->d * ec->f->n]a:
	\code
		b <- -a.
	\endcode
	\pre Описание ec работоспособно.
	\pre Буфер b либо не пересекается, либо совпадает с буфером a.
	\pre Координаты a лежат в базовом поле.
	\expect Описание ec корректно.
	\expect Точка a лежит на кривой.
*/
typedef void (*ec_neg_i)(
	word b[],				/*!< [out] обратная точка */
	const word a[],			/*!< [in] обращаемая точка */
	const struct ec_o* ec,	/*!< [in] описание эллиптической кривой */
	void* stack				/*!< [in] вспомогательная память */
);

/*!	\brief Сложение точек

	На эллиптической кривой ec определяется сумма [ec->d * ec->f->n]c точек 
	[ec->d * ec->f->n]a и [ec->d * ec->f->n]b:
	\code
		c <- a + b.
	\endcode
	\pre Описание ec работоспособно.
	\pre Буфер с либо не пересекается с каждым из буферов a и b, либо 
	совпадает с некоторым из них.
	\pre Координаты a и b лежат в базовом поле.
	\expect Описание ec корректно.
	\expect Точки a и b лежат на кривой.
*/
typedef void (*ec_add_i)(
	word c[],				/*!< [out] сумма */
	const word a[],			/*!< [in] первое слагаемое */
	const word b[],			/*!< [in] второе слагаемое */
	const struct ec_o* ec,	/*!< [in] описание эллиптической кривой */
	void* stack				/*!< [in] вспомогательная память */
);

/*!	\brief Сложение с аффинной точкой

	На эллиптической кривой ec определяется сумма [ec->d * ec->f->n]c 
	проективной точки [ec->d * ec->f->n]a и аффинной точки [2 * ec->f->n]b:
	\code
		c <- a + b.
	\endcode
	\pre Описание ec работоспособно.
	\pre Буфер с либо не пересекается с каждым из буферов a и b, либо 
	указатель c совпадает с некоторым из указателей a или b.
	\pre Координаты a и b лежат в базовом поле.
	\expect Описание ec корректно.
	\expect Точки a и b лежат на кривой.
*/
typedef void (*ec_adda_i)(
	word c[],				/*!< [out] сумма */
	const word a[],			/*!< [in] первое слагаемое */
	const word b[],			/*!< [in] второе слагаемое */
	const struct ec_o* ec,	/*!< [in] описание эллиптической кривой */
	void* stack				/*!< [in] вспомогательная память */
);

/*!	\brief Вычитание точек

	На эллиптической кривой ec определяется разность [ec->d * ec->f->n]c точек 
	[ec->d * ec->f->n]a и [ec->d * ec->f->n]b:
	\code
		c <- a - b.
	\endcode
	\pre Описание ec работоспособно.
	\pre Буфер с либо не пересекается с каждым из буферов a и b, либо 
	совпадает с некоторым из них.
	\pre Координаты a и b лежат в базовом поле.
	\expect Описание ec корректно.
	\expect Точки a и b лежат на кривой.
*/
typedef void (*ec_sub_i)(
	word c[],				/*!< [out] разность */
	const word a[],			/*!< [in] уменьшаемое */
	const word b[],			/*!< [in] вычитаемое */
	const struct ec_o* ec,	/*!< [in] описание эллиптической кривой */
	void* stack				/*!< [in] вспомогательная память */
);

/*!	\brief Вычитание аффинной точки точек

	На эллиптической кривой ec определяется разность [ec->d * ec->f->n]c 
	проективной точки [ec->d * ec->f->n]a и аффинной точки [2 * ec->f->n]b:
	\code
		c <- a - b.
	\endcode
	\pre Описание ec работоспособно.
	\pre Буфер с либо не пересекается с каждым из буферов a и b, либо 
	указатель c совпадает с некоторым из указателей a или b.
	\pre Координаты a и b лежат в базовом поле.
	\expect Описание ec корректно.
	\expect Точки a и b лежат на кривой.
*/
typedef void (*ec_suba_i)(
	word c[],				/*!< [out] разность */
	const word a[],			/*!< [in] уменьшаемое */
	const word b[],			/*!< [in] вычитаемое */
	const struct ec_o* ec,	/*!< [in] описание эллиптической кривой */
	void* stack				/*!< [in] вспомогательная память */
);

/*!	\brief Удвоение точки

	На эллиптической кривой ec определяется точка [ec->d * ec->f->n]b, 
	полученная удвоением точки [ec->d * ec->f->n]a:
	\code
		b <- 2 a.
	\endcode
	\pre Описание ec работоспособно.
	\pre Буфер b либо не пересекается, либо совпадает с буфером a.
	\pre Координаты a лежат в базовом поле.
	\expect Описание ec корректно.
	\expect Точка a лежит на кривой.
*/
typedef void (*ec_dbl_i)(
	word b[],				/*!< [out] удвоенная точка */
	const word a[],			/*!< [in] первоначальная точка */
	const struct ec_o* ec,	/*!< [in] описание эллиптической кривой */
	void* stack				/*!< [in] вспомогательная память */
);

/*!	\brief Удвоение аффинной точки

	На эллиптической кривой ec определяется проективная 
	точка [ec->d * ec->f->n]b, полученная удвоением аффинной 
	точки [2 * ec->f->n]a:
	\code
		b <- 2 a.
	\endcode
	\pre Описание ec работоспособно.
	\pre Буферы a и b либо не пересекаются, либо указатели a и b совпадают.
	\pre Координаты a лежат в базовом поле.
	\expect Описание ec корректно.
	\expect Точка a лежит на кривой.
*/
typedef void (*ec_dbla_i)(
	word b[],				/*!< [out] удвоенная точка */
	const word a[],			/*!< [in] первоначальная точка */
	const struct ec_o* ec,	/*!< [in] описание эллиптической кривой */
	void* stack				/*!< [in] вспомогательная память */
);

/*!	\brief Утроение точки

	На эллиптической кривой ec определяется точка [ec->d * ec->f->n]b, 
	полученная утроением точки [ec->d * ec->f->n]a:
	\code
		b <- 3 a.
	\endcode
	\pre Описание ec работоспособно.
	\pre Буфер b либо не пересекается, либо совпадает с буфером a.
	\pre Координаты a лежат в базовом поле.
	\expect Описание ec корректно.
	\expect Точка a лежит на кривой.
*/
typedef void (*ec_tpl_i)(
	word b[],				/*!< [out] утроенная точка */
	const word a[],			/*!< [in] первоначальная точка */
	const struct ec_o* ec,	/*!< [in] описание эллиптической кривой */
	void* stack				/*!< [in] вспомогательная память */
);

struct ec_o;


/*!	\brief Описание эллиптической кривой

	Описывается эллиптическая кривая, правила представления ее элементов, 
	группа точек и функции, реализующие операции в группе.
	\remark В таблицу указателей описания кривой как объекта входят поля 
	f, A, B, base, order, params. Поле f является указателем на объект.
*/
typedef struct ec_o
{
	obj_hdr_t hdr;			/*!< заголовок */
// ptr_table {
	const qr_o* f;			/*!< базовое поле */
	word* A;				/*!< коэффициент A */
	word* B;				/*!< коэффициент B */
	word* base;				/*!< базовая точка */
	word* order;			/*!< порядок группы точек */
	void* params;			/*!< дополнительные параметры */
// }
	size_t d;				/*!< размерность */
	word cofactor;			/*!< кофактор группы точек */
	ec_froma_i froma;		/*!< функция импорта из аффинной точки */
	ec_toa_i toa;			/*!< функция экспорта в аффинную точку */
	ec_neg_i neg;			/*!< функция обращения */
	ec_add_i add;			/*!< функция сложения */
	ec_adda_i adda;			/*!< функция сложения с аффинной точкой */
	ec_sub_i sub;			/*!< функция вычитания */
	ec_suba_i suba;			/*!< функция вычитания аффинной точки */
	ec_dbl_i dbl;			/*!< функция удвоения */
	ec_dbla_i dbla;			/*!< функция удвоения аффинной точки */
	ec_tpl_i tpl;			/*!< функция утроения */
	size_t deep;			/*!< максимальная глубина стека функций */
	octet descr[];			/*!< память для размещения данных */
} ec_o;

/*
*******************************************************************************
Псевдонимы
*******************************************************************************
*/

#define ecX(pt)\
	(pt)

#define ecY(pt, n)\
	((pt) + (n))

#define ecZ(pt, n)\
	((pt) + (n) + (n))

#define ecFromA(b, a, ec, stack)\
	(ec)->froma(b, a, ec, stack)

#define ecToA(b, a, ec, stack)\
	(ec)->toa(b, a, ec, stack)

#define ecFrom(b, a, ec, stack)\
	(qrFrom(ecX(b), a, (ec)->f, stack) &&\
		qrFrom(ecY(b, (ec)->f->n), (a) + (ec)->f->no, (ec)->f, stack) &&\
			ecFromA(b, b, ec, stack))

#define ecTo(b, a, ec, stack)\
	(ecToA(b, a, ec, stack) ?\
		qrTo((b), ecX(b), (ec)->f, stack),\
			qrTo((b) + (ec)->f->no, ecY(b, (ec)->f->n), (ec)->f, stack),\
		TRUE : FALSE)

#define ecNeg(b, a, ec, stack)\
	(ec)->neg(b, a, ec, stack)

#define ecAdd(c, a, b, ec, stack)\
	(ec)->add(c, a, b, ec, stack)

#define ecAddA(c, a, b, ec, stack)\
	(ec)->adda(c, a, b, ec, stack)

#define ecSub(c, a, b, ec, stack)\
	(ec)->sub(c, a, b, ec, stack)

#define ecSubA(c, a, b, ec, stack)\
	(ec)->suba(c, a, b, ec, stack)

#define ecDbl(b, a, ec, stack)\
	(ec)->dbl(b, a, ec, stack)

#define ecDblA(b, a, ec, stack)\
	(ec)->dbla(b, a, ec, stack)

#define ecSetO(a, ec)\
	wwSetZero(ecZ(a, (ec)->f->n), (ec)->f->n)

#define ecIsO(a, ec)\
	wwIsZero(ecZ(a, (ec)->f->n), (ec)->f->n)

#endif



#ifndef OS_APPLE
	#include <malloc.h>
#else
	#include <stdlib.h>
#endif
#ifdef OS_WIN
	#include <windows.h>
#endif

/*
*******************************************************************************
Проверка
\todo Реализовать полноценную проверку корректности памяти.
*******************************************************************************
*/

bool_t memIsValid(const void* buf, size_t count)
{
	return count == 0 || buf != 0;
}

/*
*******************************************************************************
Стандартные функции
\remark Перед вызовом memcpy(), memmove(), memset() проверяется, 
что count != 0: при count == 0 поведение стандартных функций непредсказуемо
(см. https://www.imperialviolet.org/2016/06/26/nonnull.html).
\remark Прямое обращение к функции ядра HeapAlloc() решает проблему 
с освобождением памяти в плагине bee2evp, связывающем bee2 с OpenSSL (1.1.0).
*******************************************************************************
*/

void memCopy(void* dest, const void* src, size_t count)
{
	ASSERT(memIsDisjoint(src, dest, count));
	if (count)
		memcpy(dest, src, count);
}

void memMove(void* dest, const void* src, size_t count)
{
	ASSERT(memIsValid(src, count));
	ASSERT(memIsValid(dest, count));
	if (count)
		memmove(dest, src, count);
}

void memSet(void* buf, octet c, size_t count)
{
	ASSERT(memIsValid(buf, count));
	if (count)
		memset(buf, c, count);
}

void* memAlloc(size_t count)
{
#ifdef OS_WIN
	return HeapAlloc(GetProcessHeap(), 0, count);
#else
	return malloc(count);
#endif
}

void* memRealloc(void* buf, size_t count)
{
	if (count == 0)
	{
		memFree(buf);
		return 0;
	}
#ifdef OS_WIN
	if (!buf)
		return HeapAlloc(GetProcessHeap(), 0, count);
	return HeapReAlloc(GetProcessHeap(), 0, buf, count);
#else
	return realloc(buf, count);
#endif
}

void memFree(void* buf)
{
#ifdef OS_WIN
	HeapFree(GetProcessHeap(), 0, buf);
#else
	free(buf);
#endif
}

/*
*******************************************************************************
Дополнительные функции
\remark Функция memWipe() повторяет функцию OPENSSL_cleanse()
из библиотеки OpenSSL:
\code
	unsigned char cleanse_ctr = 0;
	void OPENSSL_cleanse(void *ptr, size_t len)
	{
		unsigned char *p = ptr;
		size_t loop = len, ctr = cleanse_ctr;
		while(loop--)
		{
			*(p++) = (unsigned char)ctr;
			ctr += (17 + ((size_t)p & 0xF));
		}
		p=memchr(ptr, (unsigned char)ctr, len);
		if(p)
			ctr += (63 + (size_t)p);
		cleanse_ctr = (unsigned char)ctr;
	}
\endcode
\remark На платформе Windows есть функции SecureZeroMemory()
и RtlSecureZeroMemory(), которые, как и memWipe(), выполняют
гарантированную очистку памяти.
*******************************************************************************
*/

bool_t SAFE(memEq)(const void* buf1, const void* buf2, size_t count)
{
	register word diff = 0;
	ASSERT(memIsValid(buf1, count));
	ASSERT(memIsValid(buf2, count));
	for (; count >= O_PER_W; count -= O_PER_W)
	{
		diff |= *(const word*)buf1 ^ *(const word*)buf2;
		buf1 = (const word*)buf1 + 1;
		buf2 = (const word*)buf2 + 1;
	}
	while (count--)
	{
		diff |= *(const octet*)buf1 ^ *(const octet*)buf2;
		buf1 = (const octet*)buf1 + 1;
		buf2 = (const octet*)buf2 + 1;
	}
	return wordEq(diff, 0);
}

bool_t FAST(memEq)(const void* buf1, const void* buf2, size_t count)
{
	ASSERT(memIsValid(buf1, count));
	ASSERT(memIsValid(buf2, count));
	return memcmp(buf1, buf2, count) == 0;
}

int SAFE(memCmp)(const void* buf1, const void* buf2, size_t count)
{
	register word less = 0;
	register word greater = 0;
	register word w1;
	register word w2;
	ASSERT(memIsValid(buf1, count));
	ASSERT(memIsValid(buf2, count));
	if (count % O_PER_W)
	{
		w1 = w2 = 0;
		while (count % O_PER_W)
		{
			w1 = w1 << 8 | ((const octet*)buf1)[--count];
			w2 = w2 << 8 | ((const octet*)buf2)[count];
		}
		less |= ~greater & wordLess01(w1, w2);
		greater |= ~less & wordGreater01(w1, w2);
	}
	count /= O_PER_W;
	while (count--)
	{
		w1 = ((const word*)buf1)[count];
		w2 = ((const word*)buf2)[count];
#if (OCTET_ORDER == BIG_ENDIAN)
		w1 = wordRev(w1);
		w2 = wordRev(w2);
#endif
		less |= ~greater & wordLess(w1, w2);
		greater |= ~less & wordGreater(w1, w2);
	}
	w1 = w2 = 0;
	return (wordEq(less, 0) - 1) | wordNeq(greater, 0);
}

int FAST(memCmp)(const void* buf1, const void* buf2, size_t count)
{
	const octet* b1 = (const octet*)buf1 + count;
	const octet* b2 = (const octet*)buf2 + count;
	ASSERT(memIsValid(buf1, count));
	ASSERT(memIsValid(buf2, count));
	while (count--)
		if (*--b1 > *--b2)
			return 1;
		else if (*b1 < *b2)
			return -1;
	return 0;
}

void memWipe(void* buf, size_t count)
{
	static octet wipe_ctr = 0;
	volatile octet* p = (octet*)buf;
	size_t ctr = wipe_ctr;
	size_t i = count;
	ASSERT(memIsValid(buf, count));
	// вычисления, которые должны показаться полезными оптимизатору
	while (i--)
		*(p++) = (octet)ctr, ctr += 17 + ((size_t)p & 15);
	p = memchr(buf, (octet)ctr, count);
	if (p)
		ctr += (63 + (size_t)p);
	wipe_ctr = (octet)ctr;
}

bool_t SAFE(memIsZero)(const void* buf, size_t count)
{
	register word diff = 0;
	ASSERT(memIsValid(buf, count));
	for (; count >= O_PER_W; count -= O_PER_W)
	{
		diff |= *(const word*)buf;
		buf = (const word*)buf + 1;
	}
	while (count--)
	{
		diff |= *(const octet*)buf;
		buf = (const octet*)buf + 1;
	}
	return (bool_t)wordEq(diff, 0);
}

bool_t FAST(memIsZero)(const void* buf, size_t count)
{
	ASSERT(memIsValid(buf, count));
	for (; count >= O_PER_W; count -= O_PER_W, buf = (const word*)buf + 1)
		if (*(const word*)buf)
			return FALSE;
	for (; count--; buf = (const octet*)buf + 1)
		if (*(const octet*)buf)
			return FALSE;
	return TRUE;
}

size_t memNonZeroSize(const void* buf, size_t count)
{
	ASSERT(memIsValid(buf, count));
	while (count--)
		if (*((const octet*)buf + count))
			return count + 1;
	return 0;
}

bool_t SAFE(memIsRep)(const void* buf, size_t count, octet o)
{
	register word diff = 0;
	ASSERT(memIsValid(buf, count));
	for (; count--; buf = (const octet*)buf + 1)
		diff |= *(const octet*)buf ^ o;
	return wordEq(diff, 0);
}

bool_t FAST(memIsRep)(const void* buf, size_t count, octet o)
{
	ASSERT(memIsValid(buf, count));
	for (; count--; buf = (const octet*)buf + 1)
		if (*(const octet*)buf != o)
			return FALSE;
	return TRUE;
}

bool_t memIsDisjoint(const void* buf1, const void* buf2, size_t count)
{
	ASSERT(memIsValid(buf1, count));
	ASSERT(memIsValid(buf2, count));
	return count == 0 || (const octet*)buf1 + count <= (const octet*)buf2 ||
		(const octet*)buf1 >= (const octet*)buf2 + count;
}

bool_t memIsSameOrDisjoint(const void* buf1, const void* buf2, size_t count)
{
	ASSERT(memIsValid(buf1, count));
	ASSERT(memIsValid(buf2, count));
	return buf1 == buf2 || count == 0 ||
		(const octet*)buf1 + count <= (const octet*)buf2 ||
		(const octet*)buf1 >= (const octet*)buf2 + count;
}

bool_t memIsDisjoint2(const void* buf1, size_t count1,
	const void* buf2, size_t count2)
{
	ASSERT(memIsValid(buf1, count1));
	ASSERT(memIsValid(buf2, count2));
	return count1 == 0 || count2 == 0 ||
		(const octet*)buf1 + count1 <= (const octet*)buf2 ||
		(const octet*)buf1 >= (const octet*)buf2 + count2;
}

bool_t memIsDisjoint3(const void* buf1, size_t count1,
	const void* buf2, size_t count2,
	const void* buf3, size_t count3)
{
	return memIsDisjoint2(buf1, count1, buf2, count2) &&
		memIsDisjoint2(buf1, count1, buf3, count3) &&
		memIsDisjoint2(buf2, count2, buf3, count3);
}

bool_t memIsDisjoint4(const void* buf1, size_t count1,
	const void* buf2, size_t count2,
	const void* buf3, size_t count3,
	const void* buf4, size_t count4)
{
	return memIsDisjoint2(buf1, count1, buf2, count2) &&
		memIsDisjoint2(buf1, count1, buf3, count3) &&
		memIsDisjoint2(buf1, count1, buf4, count4) &&
		memIsDisjoint3(buf2, count2, buf3, count3, buf4, count4);
}

void memJoin(void* dest, const void* src1, size_t count1, const void* src2,
	size_t count2)
{
	register octet o;
	size_t i;
	ASSERT(memIsValid(src1, count1));
	ASSERT(memIsValid(src2, count2));
	ASSERT(memIsValid(dest, count1 + count2));
repeat:
	if (memIsDisjoint2(dest, count1, src2, count2))
	{
		memMove(dest, src1, count1);
		memMove((octet*)dest + count1, src2, count2);
	}
	else if (memIsDisjoint2((octet*)dest + count1, count2, src1, count1))
	{
		memMove((octet*)dest + count1, src2, count2);
		memMove(dest, src1, count1);
	}
	else if (memIsDisjoint2(dest, count2, src1, count1))
	{
		// dest <- src2 || src1
		memMove(dest, src2, count2);
		memMove((octet*)dest + count2, src1, count1);
		// dest <- dest <<< count2
		for (i = 0; i < count2; ++i)
		{
			o = ((octet*)dest)[0];
			memMove(dest, (octet*)dest + 1, count1 + count2 - 1);
			((octet*)dest)[count1 + count2 - 1] = o;
		}
	}
	else if (memIsDisjoint2((octet*)dest + count2, count1, src2, count2))
	{
		// dest <- src2 || src1
		memMove((octet*)dest + count2, src1, count1);
		memMove(dest, src2, count2);
		// dest <- dest <<< count2
		for (i = 0; i < count2; ++i)
		{
			o = ((octet*)dest)[0];
			memMove(dest, (octet*)dest + 1, count1 + count2 - 1);
			((octet*)dest)[count1 + count2 - 1] = o;
		}
	}
	else
	{
		// src1 (src2) пересекается и с префиксом, и с суффиксом dest
		// длины count2 (count1) => и первый, и последний октет dest
		// не входят не входят ни в src1, ни в src2
		((octet*)dest)[0] = ((const octet*)src1)[0];
		((octet*)dest)[count1 + count2 - 1] = ((const octet*)src2)[count2 - 1];
		VERIFY(count1--);
		VERIFY(count2--);
		src1 = (const octet*)src1 + 1;
		dest = (octet*)dest + 1;
		goto repeat;
	}
}

void memXor(void* dest, const void* src1, const void* src2, size_t count)
{
	ASSERT(memIsSameOrDisjoint(src1, dest, count));
	ASSERT(memIsSameOrDisjoint(src2, dest, count));
	for (; count >= O_PER_W; count -= O_PER_W)
	{
		*(word*)dest = *(const word*)src1 ^ *(const word*)src2;
		src1 = (const word*)src1 + 1;
		src2 = (const word*)src2 + 1;
		dest = (word*)dest + 1;
	}
	while (count--)
	{
		*(octet*)dest = *(const octet*)src1 ^ *(const octet*)src2;
		src1 = (const octet*)src1 + 1;
		src2 = (const octet*)src2 + 1;
		dest = (octet*)dest + 1;
	}
}

void memXor2(void* dest, const void* src, size_t count)
{
	ASSERT(memIsSameOrDisjoint(src, dest, count));
	for (; count >= O_PER_W; count -= O_PER_W)
	{
		*(word*)dest ^= *(const word*)src;
		src = (const word*)src + 1;
		dest = (word*)dest + 1;
	}
	while (count--)
	{
		*(octet*)dest ^= *(const octet*)src;
		src = (const octet*)src + 1;
		dest = (octet*)dest + 1;
	}
}

void memSwap(void* buf1, void* buf2, size_t count)
{
	ASSERT(memIsDisjoint(buf1, buf2, count));
	for (; count >= O_PER_W; count -= O_PER_W)
	{
		SWAP(*(word*)buf1, *(word*)buf2);
		buf1 = (word*)buf1 + 1;
		buf2 = (word*)buf2 + 1;
	}
	while (count--)
	{
		SWAP(*(octet*)buf1, *(octet*)buf2);
		buf1 = (octet*)buf1 + 1;
		buf2 = (octet*)buf2 + 1;
	}
}

/*
*******************************************************************************
Реверс октетов
*******************************************************************************
*/

void memRev(void* buf, size_t count)
{
	register size_t i = count / 2;
	ASSERT(memIsValid(buf, count));
	while (i--)
	{
		((octet*)buf)[i] ^= ((octet*)buf)[count - 1 - i];
		((octet*)buf)[count - 1 - i] ^= ((octet*)buf)[i];
		((octet*)buf)[i] ^= ((octet*)buf)[count - 1 - i];
	}
}


bool_t ecpCreateJ(ec_o* ec, const qr_o* f, const octet A[], const octet B[], 
	void* stack)
{
	register bool_t bA3;
	word* t;
	// pre
	ASSERT(memIsValid(ec, sizeof(ec_o)));
	ASSERT(gfpIsOperable(f));
	ASSERT(memIsValid(A, f->no)); 
	ASSERT(memIsValid(B, f->no)); 
	// обнулить
	memSetZero(ec, sizeof(ec_o));
	// зафикисровать размерности
	ec->d = 3;
	// запомнить базовое поле
	ec->f = f;
	// сохранить коэффициенты
	ec->A = (word*)ec->descr;
	ec->B = ec->A + f->n;
	if (!qrFrom(ec->A, A, ec->f, stack) || !qrFrom(ec->B, B, ec->f, stack))
		return FALSE;
	// t <- -3
	t = (word*)stack;
	gfpDouble(t, f->unity, f);
	zmAdd(t, t, f->unity, f);
	zmNeg(t, t, f);
	// bA3 <- A == -3?
	bA3 = qrCmp(t, ec->A, f) == 0;
	// подготовить буферы для описания группы точек
	ec->base = ec->B + f->n;
	ec->order = ec->base + 2 * f->n;
	// настроить интерфейсы
	ec->froma = ecpFromAJ;
	ec->toa = ecpToAJ;
	ec->neg = ecpNegJ;
	ec->add = ecpAddJ;
	ec->adda = ecpAddAJ;
	ec->sub = ecpSubJ;
	ec->suba = ecpSubAJ;
	ec->dbl = bA3 ? ecpDblJA3 : ecpDblJ;
	ec->dbla = ecpDblAJ;
	ec->tpl = bA3 ? ecpTplJA3 : ecpTplJ;
	ec->deep = utilMax(9,
		ecpToAJ_deep(f->n, f->deep),
		ecpAddJ_deep(f->n, f->deep),
		ecpAddAJ_deep(f->n, f->deep),
		ecpSubJ_deep(f->n, f->deep),
		ecpSubAJ_deep(f->n, f->deep),
		bA3 ? ecpDblJA3_deep(f->n, f->deep) : ecpDblJ_deep(f->n, f->deep),
		ecpDblAJ_deep(f->n, f->deep),
		bA3 ? ecpTplJA3_deep(f->n, f->deep) : ecpTplJ_deep(f->n, f->deep));
	// настроить
	ec->hdr.keep = sizeof(ec_o) + O_OF_W(5 * f->n + 1);
	ec->hdr.p_count = 6;
	ec->hdr.o_count = 1;
	// все нормально
	bA3 = 0;
	return TRUE;
}

size_t ecpCreateJ_keep(size_t n)
{
	return sizeof(ec_o) + O_OF_W(5 * n + 1);
}

size_t ecpCreateJ_deep(size_t n, size_t f_deep)
{
	return utilMax(11,
		O_OF_W(n),
		ecpToAJ_deep(n, f_deep),
		ecpAddJ_deep(n, f_deep),
		ecpAddAJ_deep(n, f_deep),
		ecpSubJ_deep(n, f_deep),
		ecpSubAJ_deep(n, f_deep),
		ecpDblJ_deep(n, f_deep),
		ecpDblJA3_deep(n, f_deep),
		ecpDblAJ_deep(n, f_deep),
		ecpTplJ_deep(n, f_deep),
		ecpTplJA3_deep(n, f_deep));
}


/*
*******************************************************************************
Создание / закрытие эллиптической кривой
*******************************************************************************
*/

err_t bignStart(void* state, const bign_params* params)
{
	// размерности
	size_t no, n;
	size_t f_keep;
	size_t ec_keep;
	// состояние
	qr_o* f;		/* поле */
	ec_o* ec;		/* кривая */
	void* stack;	/* вложенный стек */
	// pre
	ASSERT(memIsValid(params, sizeof(bign_params)));
	ASSERT(params->l == 128 || params->l == 192  || params->l == 256);
	ASSERT(memIsValid(state, bignStart_keep(params->l, 0)));
	// определить размерности
	no = O_OF_B(2 * params->l);
	n = W_OF_B(2 * params->l);
	f_keep = gfpCreate_keep(no);
	ec_keep = ecpCreateJ_keep(n);
	// создать поле и выполнить минимальные проверки p
	f = (qr_o*)((octet*)state + ec_keep);
	stack = (octet*)f + f_keep;
	if (!gfpCreate(f, params->p, no, stack) ||
		wwBitSize(f->mod, n) != params->l * 2 ||
		wwGetBits(f->mod, 0, 2) != 3)
		return ERR_BAD_PARAMS;
	// создать кривую и группу, выполнить минимальную проверку order
	ec = (ec_o*)state;
	if (!ecpCreateJ(ec, f, params->a, params->b, stack) ||
		!ecCreateGroup(ec, 0, params->yG, params->q, no, 1, stack) ||
		wwBitSize(ec->order, n) != params->l * 2 ||
		zzIsEven(ec->order, n))
		return ERR_BAD_PARAMS;
	// присоединить f к ec
	objAppend(ec, f, 0);
	// все нормально
	return ERR_OK;
}

size_t bignStart_keep(size_t l, bign_deep_i deep)
{
	// размерности
	size_t no = O_OF_B(2 * l);
	size_t n = W_OF_B(2 * l);
	size_t f_keep = gfpCreate_keep(no);
	size_t f_deep = gfpCreate_deep(no);
	size_t ec_d = 3;
	size_t ec_keep = ecpCreateJ_keep(n);
	size_t ec_deep = ecpCreateJ_deep(n, f_deep);
	// расчет
	return f_keep + ec_keep +
		utilMax(3,
			ec_deep,
			ecCreateGroup_deep(f_deep),
			deep ? deep(n, f_deep, ec_d, ec_deep) : 0);
}

int ecMulA(word b[], word a[], ec_o* ec, word d[], size_t m, void* stack)
{
	const size_t n = ec->f->n;
	const size_t naf_width = ecNAFWidth(B_OF_W(m));
	const size_t naf_count = SIZE_1 << (naf_width - 2);
	const word naf_hi = WORD_1 << (naf_width - 1);
	register size_t naf_size;
	register size_t i;
	register word w;
	// declaring stack variables
	word* naf;			/* NAF */
	word* t;			/* help point */
	word* pre;			/* pre[i] = (2i + 1)a (naf_count elements) */
						// pre
	ASSERT(ecIsOperable(ec));
	// stack fill
	naf = (word*)stack;
	t = naf + 2 * m + 1;
	pre = t + ec->d * n;
	stack = pre + naf_count * ec->d * n;
	// calculating NAF
	ASSERT(naf_width >= 3);
	naf_size = wwNAF(naf, d, m, naf_width);
	// d == O => b <- O
	if (naf_size == 0)
		return FALSE;
	// pre[0] <- a
	ecFromA(pre, a, ec, stack);
	// calculating pre[i]: t <- 2a, pre[i] <- t + pre[i - 1]
	ASSERT(naf_count > 1);
	ecDblA(t, pre, ec, stack);
	ecAddA(pre + ec->d * n, t, pre, ec, stack);
	for (i = 2; i < naf_count; ++i)
		ecAdd(pre + i * ec->d * n, t, pre + (i - 1) * ec->d * n, ec, stack);
	// t <- a[naf[l - 1]]
	w = wwGetBits(naf, 0, naf_width);
	ASSERT((w & 1) == 1 && (w & naf_hi) == 0);
	wwCopy(t, pre + (w >> 1) * ec->d * n, ec->d * n);
	// loop through NAF symbols
	i = naf_width;
	while (--naf_size)
	{
		w = wwGetBits(naf, i, naf_width);
		if (w & 1)
		{
			// t <- 2 t
			ecDbl(t, t, ec, stack);
			// t <- t \pm pre[naf[w]]
			if (w == 1)
				ecAddA(t, t, pre, ec, stack);
			else if (w == (naf_hi ^ 1))
				ecSubA(t, t, pre, ec, stack);
			else if (w & naf_hi)
				ecSub(t, t, pre + ((w ^ naf_hi) >> 1) * ec->d * n, ec, stack);
			else
				ecAdd(t, t, pre + (w >> 1) * ec->d * n, ec, stack);
			// next naf slot
			i += naf_width;
		}
		else
			ecDbl(t, t, ec, stack), ++i;
	}
	// clearing
	w = 0;
	i = 0;
	// to affine coordinates
	return ecToA(b, t, ec, stack);
}

size_t ecMulA_deep(size_t n, size_t ec_d, size_t ec_deep, size_t m)
{
	const size_t naf_width = ecNAFWidth(B_OF_W(m));
	const size_t naf_count = SIZE_1 << (naf_width - 2);
	return O_OF_W(2 * m + 1) +
		O_OF_W(ec_d * n) +
		O_OF_W(ec_d * n * naf_count) +
		ec_deep;
}