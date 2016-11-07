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

#define GDA_ALGO_H
#endif



#ifdef GDA_ALGO_IMPLEMENTATION

#endif