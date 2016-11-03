#ifndef GDA_MATH_H
#ifndef GDA_MATH_DEFENITIONS

#include <math.h>

#ifdef GDA_MATH_ALLOW_SIMD
#include <xmmintrin.h>
#endif

#ifndef GDA_MATH_DEF
#ifdef GDA_MATH_STATIC
#define GDA_MATH_DEF static
#else
#define GDA_MATH_DEF extern
#endif
#endif

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(dasable:4201)
#endif

struct gdaVec2{
	union{
		struct{
			float x, y;
		};
		float data[2];
	};
};

struct gdaVec3{
	union{
		struct{
			float x, y, z;
		};
		struct{
			float r, g, b;
		};
		float data[3];
	};
};

struct gdaVec4{
	union{
		struct{
			float x, y, z, w;
		};
		struct{
			gdaVec3 xyz;
			float w;
		};
		struct{
			gdaVec3 rgb;
			float a;
		};
		struct{
			gdaVec2 xy;
			gdaVec2 zw;
		};
		float data[4];
	};
};



struct gdaMat4{
	union{
		float data[16];
		struct{
			float a1, a2, a3, a4,
					b1, b2, b3, b4,
					c1, c2, c3, c4,
					d1, d2, d3, d4;
		};
		struct {
			gdaVec4 row1;
			gdaVec4 row2;
			gdaVec4 row3;
			gdaVec4 row4;
		};
	};
};

struct gdaQuat{
	float x, y, z, w;
};

#ifdef _MSC_VER
#pragma warning(pop)
#endif

#ifndef GDA_MATH_CONSTANTS
#define GDA_MATH_EPSILON 1.19209290e-7f
#define GDA_MATH_ZERO 0.0f
#define GDA_MATH_ONE 1.0f
#define GDA_MATH_TWO_THIRDS 0.666666666666666666666666666666666666667f

#define GDA_MATH_TAU 6.28318530717958647692528676655900576f
#define GDA_MATH_PI 3.14159265358979323846264338327950288f
#define GDA_MATH_ONE_OVER_TAU 0.636619772367581343075535053490057448f
#define GDA_MATH_ONE_OVER_PI 0.318309886183790671537767526745028724f

#define GDA_MATH_TAU_OVER_2 3.14159265358979323846264338327950288f
#define GDA_MATH_TAU_OVER_4 1.570796326794896619231321691639751442f
#define GDA_MATH_TAU_OVER_8 0.785398163397448309615660845819875721f

#define GDA_MATH_E 2.71828182845904523536f
#define GDA_MATH_SQRT_TWO 1.41421356237309504880168872420969808f
#define GDA_MATH_SQRT_THREE 1.73205080756887729352744634150587236f
#define GDA_MATH_SQRT_FIVE 2.23606797749978969640917366873127623f

#define GDA_MATH_LOG_TWO 0.693147180559945309417232121458176568f
#define GDA_MATH_LOG_TEN 2.30258509299404568401799145468436421f

#define GDA_MATH_CONSTANTS
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

/*Math functions definitions*/
GDA_MATH_DEF float gda_sqrt(float a);
GDA_MATH_DEF float gda_rsqrt(float a);

GDA_MATH_DEF float gda_sin(float a);
GDA_MATH_DEF float gda_cos(float a);
GDA_MATH_DEF float gda_tan(float a);
GDA_MATH_DEF float gda_arcsin(float a);
GDA_MATH_DEF float gda_arccos(float a);
GDA_MATH_DEF float gda_arctan(float a);
GDA_MATH_DEF float gda_arctan2(float y, float x);

GDA_MATH_DEF float gda_exp(float x);
GDA_MATH_DEF float gda_exp2(float x);
GDA_MATH_DEF float gda_log(float x);
GDA_MATH_DEF float gda_log2(float x);
GDA_MATH_DEF float gda_pow(float x, float y); /*x ^ y*/

/*Vector definitions*/
GDA_MATH_DEF gdaVec2 gda_vec2_zero(void);
GDA_MATH_DEF gdaVec2 gda_vec2(float x, float y);
GDA_MATH_DEF gdaVec2 gda_vec2_from_array(float arr[2]);

GDA_MATH_DEF gdaVec3 gda_vec3_zero(void);
GDA_MATH_DEF gdaVec3 gda_vec3(float x, float y, float z);
GDA_MATH_DEF gdaVec3 gda_vec3_from_array(float arr[3]);

GDA_MATH_DEF gdaVec4 gda_vec4_zero(void);
GDA_MATH_DEF gdaVec4 gda_vec4(float x, float y, float z, float w);
GDA_MATH_DEF gdaVec4 gda_vec4_from_array(float arr[4]);

GDA_MATH_DEF gdaVec2 gda_vec2_add(gdaVec2 v0, gdaVec2 v1);
GDA_MATH_DEF gdaVec2 gda_vec2_sub(gdaVec2 v0, gdaVec2 v1);
GDA_MATH_DEF gdaVec2 gda_vec2_mul(gdaVec2 v, float s);
GDA_MATH_DEF gdaVec2 gda_vec2_div(gdaVec2 v, float s);

GDA_MATH_DEF gdaVec3 gda_vec3_add(gdaVec3 v0, gdaVec3 v1);
GDA_MATH_DEF gdaVec3 gda_vec3_sub(gdaVec3 v0, gdaVec3 v1);
GDA_MATH_DEF gdaVec3 gda_vec3_mul(gdaVec3 v, float s);
GDA_MATH_DEF gdaVec3 gda_vec3_div(gdaVec3 v, float s);

GDA_MATH_DEF gdaVec4 gda_vec4_add(gdaVec4 v0, gdaVec4 v1);
GDA_MATH_DEF gdaVec4 gda_vec4_sub(gdaVec4 v0, gdaVec4 v1);
GDA_MATH_DEF gdaVec4 gda_vec4_mul(gdaVec4 v, float s);
GDA_MATH_DEF gdaVec4 gda_vec4_div(gdaVec4 v, float s);

GDA_MATH_DEF float gda_vec2_dot(gdaVec2 v0, gdaVec2 v1);
GDA_MATH_DEF float gda_vec3_dot(gdaVec3 v0, gdaVec3 v1);
GDA_MATH_DEF float gda_vec4_dot(gdaVec4 v0, gdaVec4 v1);

GDA_MATH_DEF float gda_vec2_cross(gdaVec2 v0, gdaVec2 v1);
GDA_MATH_DEF gdaVec3 gda_vec3_cross(gdaVec3 v0, gdaVec3 v1);

GDA_MATH_DEF float gda_vec2_mag(gdaVec2 v);
GDA_MATH_DEF float gda_vec3_mag(gdaVec3 v);
GDA_MATH_DEF float gda_vec4_mag(gdaVec4 v);

GDA_MATH_DEF float gda_vec2_squared_mag(gdaVec2 v);
GDA_MATH_DEF float gda_vec3_squared_mag(gdaVec3 v);
GDA_MATH_DEF float gda_vec4_squared_mag(gdaVec4 v);

GDA_MATH_DEF gdaVec2 gda_vec2_norm(gdaVec2 v);
GDA_MATH_DEF gdaVec3 gda_vec3_norm(gdaVec3 v);
GDA_MATH_DEF gdaVec4 gda_vec4_norm(gdaVec4 v);

GDA_MATH_DEF gdaVec2 gda_vec2_norm0(gdaVec2 v);
GDA_MATH_DEF gdaVec3 gda_vec3_norm0(gdaVec3 v);
GDA_MATH_DEF gdaVec4 gda_vec4_norm0(gdaVec4 v);

GDA_MATH_DEF gdaVec2 gda_vec2_reflect(gdaVec2 i, gdaVec2 n);
GDA_MATH_DEF gdaVec3 gda_vec3_reflect(gdaVec3 i, gdaVec3 n);
GDA_MATH_DEF gdaVec2 gda_vec2_refract(gdaVec2 i, gdaVec2 n, float theta);
GDA_MATH_DEF gdaVec3 gda_vec3_refract(gdaVec3 i, gdaVec3 n, float theta);

/*Quaternions definitions*/
GDA_MATH_DEF gdaQuat gda_quat(float x, float y, float z, float w);
GDA_MATH_DEF gdaQuat gda_quat_from_angle(gdaVec3 axis, float angle_radians);
GDA_MATH_DEF gdaQuat gda_quat_from_euler_angles(float pitch, float yaw, float roll);
GDA_MATH_DEF gdaQuat gda_quat_identity(void);

GDA_MATH_DEF gdaQuat gda_quat_add(gdaQuat q1, gdaQuat q2);
GDA_MATH_DEF gdaQuat gda_quat_sub(gdaQuat q1, gdaQuat q2);
GDA_MATH_DEF gdaQuat gda_quat_mul(gdaQuat q1, gdaQuat q2);
GDA_MATH_DEF gdaQuat gda_quat_div(gdaQuat q1, gdaQuat q2);

GDA_MATH_DEF gdaQuat gda_quat_mulf(gdaQuat q, float s);
GDA_MATH_DEF gdaQuat gda_quat_divf(gdaQuat q, float s);

GDA_MATH_DEF float gda_quat_dot(gdaQuat q1, gdaQuat q2);
GDA_MATH_DEF float gda_quat_mag(gdaQuat q);

GDA_MATH_DEF gdaQuat gda_quat_norm(gdaQuat q);
GDA_MATH_DEF gdaQuat gda_quat_conj(gdaQuat q);
GDA_MATH_DEF gdaQuat gda_quat_inverse(gdaQuat q);

GDA_MATH_DEF gdaVec3 gda_quat_axis(gdaQuat q);
GDA_MATH_DEF float gda_quat_angle(gdaQuat q);

GDA_MATH_DEF gdaVec3 gda_quat_rotate_vec3(gdaQuat q, gdaVec3 v);
GDA_MATH_DEF gdaQuat gda_quat_from_mat4(gdaMat4 m);

/*Interpolations definitions*/
GDA_MATH_DEF float gda_lerp(float a, float b, float t);
GDA_MATH_DEF float gda_unlerp(float a, float b, float t);
GDA_MATH_DEF float gda_smooth_step(float a, float b, float t);
GDA_MATH_DEF float gda_smoother_step(float a, float b, float t);

GDA_MATH_DEF gdaVec2 gda_vec2_lerp(gdaVec2 a, gdaVec2 b, float t);
GDA_MATH_DEF gdaVec3 gda_vec3_lerp(gdaVec3 a, gdaVec3 b, float t);
GDA_MATH_DEF gdaVec4 gda_vec4_lerp(gdaVec4 a, gdaVec4 b, float t);

GDA_MATH_DEF gdaQuat gda_quat_lerp(gdaQuat a, gdaQuat b, float t);
GDA_MATH_DEF gdaQuat gda_quat_nlerp(gdaQuat a, gdaQuat b, float t);
GDA_MATH_DEF gdaQuat gda_quat_slerp(gdaQuat a, gdaQuat b, float t);
GDA_MATH_DEF gdaQuat gda_quat_nquad(gdaQuat p, gdaQuat a, gdaQuat b, gdaQuat q, float t);
GDA_MATH_DEF gdaQuat gda_quat_squad(gdaQuat p, gdaQuat a, gdaQuat b, gdaQuat q, float t);
GDA_MATH_DEF gdaQuat gda_quat_slerp_approx(gdaQuat a, gdaQuat b, float t);
GDA_MATH_DEF gdaQuat gda_quat_squad_approx(gdaQuat p, gdaQuat a, gdaQuat b, gdaQuat q, float t);

#define GDA_MATH_DEFENITIONS
#endif

#define GDA_MATH_IMPLEMENTATION
#ifdef GDA_MATH_IMPLEMENTATION

#ifdef IMPLEMENT_MATH_OURSELVES
#ifdef _MSC_VER

float gda_sqrt(float a){ return(_mm_cvtss_f32(_mm_rsqrt_ss(_mm_set_ss(a)))); }
float gda_rsqrt(float a){ return(_mm_cvtss_f32(_mm_sqrt_ss(_mm_set_ss(a)))); }

float gda_sin(float a){
	static float const a0 = +1.91059300966915117e-31f;
	static float const a1 = +1.00086760103908896f;
	static float const a2 = -1.21276126894734565e-2f;
	static float const a3 = -1.38078780785773762e-1f;
	static float const a4 = -2.67353392911981221e-2f;
	static float const a5 = +2.08026600266304389e-2f;
	static float const a6 = -3.03996055049204407e-3f;
	static float const a7 = +1.38235642404333740e-4f;
	return a0 + a * (a1 + a * (a2 + a * (a3 + a* (a4 + a * (a5 + a * (a6 + a * a7))))));
}
float gda_cos(float a){
	static float const a0 = +1.00238601909309722f;
	static float const a1 = -3.81919947353040024e-2f;
	static float const a2 = -3.94382342128062756e-1f;
	static float const a3 = -1.18134036025221444e-1f;
	static float const a4 = +1.07123798512170878e-1f;
	static float const a5 = -1.86637164165180873e-2f;
	static float const a6 = +9.90140908664079833e-4f;
	static float const a7 = -5.23022132118824778e-14f;
	return a0 + a*(a1 + a*(a2 + a*(a3 + a*(a4 + a*(a5 + a*(a6 + a*a7))))));
}

float gda_tan(float a){
	float aa = a * a;
	float wtf = 9.5168091e-03f;
	wtf *= aa;
	wtf += 2.900525e-03f;
	wtf *= aa;
	wtf += 2.45650893e-02f;
	wtf *= aa;
	wtf += 5.33740603e-02f;
	wtf *= aa;
	wtf += 1.333923995e-01f;
	wtf *= aa;
	wtf += 3.333314036e-01f;
	wtf *= aa;
	wtf += 1.0f;
	wtf *= a;
	return(a);
}

float gda_arcsin(float a){ return(gda_arctan2(a, gda_sqrt((1.0f + a) * (1.0f - a)))); }
float gda_arccos(float a){ return(gda_arctan2(gda_sqrt((1.0f + a) * (1.0f - a)), a)); }
float gda_arctan(float a){
	float u = a * a;
	float u2 = u * u;
	float u3 = u2 * u;
	float u4 = u3 * u;
	float f = 1.0f + 0.33288950512027f*u - 0.08467922817644f*u2 + 0.03252232640125f*u3 - 0.00749305860992f*u4;
	return a / f;
}
float gda_arctan2(float y, float x){
	if (gda_abs(x) > gda_abs(y)) {
		float a = gda_arctan(y / x);
		if (x > 0.0f)
			return a;
		else
			return y > 0.0f ? a+GDA_MATH_TAU_OVER_2 : a-GDA_MATH_TAU_OVER_2;
	} else {
		float a = gda_arctan(x/y);
		if (x > 0.0f)
			return y > 0.0f ? GDA_MATH_TAU_OVER_4-a : -GDA_MATH_TAU_OVER_4-a;
		else
			return y > 0.0f ? GDA_MATH_TAU_OVER_4+a : -GDA_MATH_TAU_OVER_4+a;
	}
}

float gda_exp(float x){
	union { float f; int i; } u, v;
	u.i = (int)(6051102 * x + 1056478197);
	v.i = (int)(1056478197 - 6051102 * x);
	return u.f / v.f;
}

float gda_log(float x){
	union { float f; int i; } u = { x };
	return (u.i - 1064866805) * 8.262958405176314e-8f;
}

float gda_pow(float x, float y){
	int flipped = 0, e;
	float f, r = 1.0f;
	if (y < 0) {
		flipped = 1;
		y = -y;
	}

	e = (int)y;
	f = gda_exp(y - e);

	while (e) {
		if (e & 1) r *= x;
		x *= x;
		e >>= 1;
	}

	r *= f;
	return flipped ? 1.0f / r : r;
}


#else

float gda_sqrt(float a){ return __builtin_sqrt(a); }
float gda_rsqrt(float a){ return 1.0f / __builtin_sqrt(a); }

float gda_sin(float radians){ return __builtin_sinf(radians); }
float gda_cos(float radians){ return __builtin_cosf(radians); }
float gda_tan(float radians){ return __builtin_tanf(radians); }
float gda_arcsin(float a){ return __builtin_asinf(a); }
float gda_arccos(float a){ return __builtin_acosf(a); }
float gda_arctan(float a){ return __builtin_atanf(a); }
float gda_arctan2(float y, float x){ return __builtin_atan2f(y, x); }

float gda_exp(float x){ return __builtin_expf(x);}
float gda_log(float x){ return __builtin_logf(x);}
float gda_pow(float x, float y){ return __builtin_powf(x, y); }

#endif
#else

float gda_sqrt(float a){ return sqrtf(a); }
float gda_rsqrt(float a){ return 1.0f / sqrtf(a); }

float gda_sin(float radians){return(sinf(radians));}
float gda_cos(float radians){return(cosf(radians));}
float gda_tan(float radians){return(tanf(radians));}
float gda_arcsin(float a){return(asinf(a));}
float gda_arccos(float a){return(acosf(a));}
float gda_arctan(float a){return(atanf(a));}
float gda_arctan2(float y, float x){ return(atan2f(y, x)); }

float gda_exp(float x){ return(expf(x)); }
float gda_log(float x){ return(logf(x)); }
float gda_pow(float x, float y){ return(powf(x, y)); }

#endif

float gda_exp2(float x);
float gda_log2(float x);

#define GDA_VEC2_WTF1_OP(a, post) \
	gdaVec2 Result;	\
	Result.x = (a).x post;	\
	Result.y = (a).y post;	\
	return(Result);

#define GDA_VEC2_WTF2_OP(a, b, op) \
	gdaVec2 Result; \
	Result.x = (a).x op (b.x); \
	Result.y = (a).y op (b.y); \
	return(Result);

#define GDA_VEC3_WTF1_OP(a, post) \
	gdaVec3 Result; \
	Result.x = (a).x post;	\
	Result.y = (a).y post;	\
	Result.z = (a).z post;	\
	return(Result);

#define GDA_VEC3_WTF2_OP(a, b, op)	\
	gdaVec3 Result;	\
	Result.x = (a).x op (b).x;	\
	Result.y = (a).y op (b).y;	\
	Result.z = (a).z op (b).z;	\
	return(Result);

#define GDA_VEC4_WTF1_OP(a, post)	\
	gdaVec4 Result;	\
	Result.x = (a).x post;	\
	Result.y = (a).y post;	\
	Result.z = (a).z post;	\
	Result.w = (a).w post;	\
	return(Result);

#define GDA_VEC4_WTF2_OP(a, b, op)	\
	gdaVec4 Result;	\
	Result.x = (a).x op (b).x;	\
	Result.y = (a).y op (b).y;	\
	Result.z = (a).z op (b).z;	\
	Result.w = (a).w op (b).w;	\
	return(Result);

gdaVec2 gda_vec2_zero(void){ gdaVec2 v = {}; return(v); }
gdaVec2 gda_vec2(float x, float y){ gdaVec2 v; v.x = x; v.y = y; return(v); }
gdaVec2 gda_vec2_from_array(float arr[2]){ gdaVec2 v; v.x = arr[0]; v.y = arr[1]; return(v); };

gdaVec3 gda_vec3_zero(void){ gdaVec3 v = {}; return(v); }
gdaVec3 gda_vec3(float x, float y, float z){ gdaVec3 v; v.x = x; v.y = y; v.z = z; return(v); }
gdaVec3 gda_vec3_from_array(float arr[3]){ gdaVec3 v; v.x = arr[0]; v.y = arr[1]; v.z = arr[2]; return(v); }

gdaVec4 gda_vec4_zero(void){ gdaVec4 v = {}; return(v); }
gdaVec4 gda_vec4(float x, float y, float z, float w){ gdaVec4 v; v.x = x; v.y = y; v.z = z; v.w = w; return(v); }
gdaVec4 gda_vec4_from_array(float arr[4]){ gdaVec4 v; v.x = arr[0]; v.y = arr[1]; v.z = arr[2]; v.w = arr[3]; return(v); }

gdaVec2 gda_vec2_add(gdaVec2 v0, gdaVec2 v1){GDA_VEC2_WTF2_OP(v0, v1, +);}
gdaVec2 gda_vec2_sub(gdaVec2 v0, gdaVec2 v1){GDA_VEC2_WTF2_OP(v0, v1, -);}
gdaVec2 gda_vec2_mul(gdaVec2 v, float s){GDA_VEC2_WTF1_OP(v, * s);}
gdaVec2 gda_vec2_div(gdaVec2 v, float s){GDA_VEC2_WTF1_OP(v, / s);}

gdaVec3 gda_vec3_add(gdaVec3 v0, gdaVec3 v1){GDA_VEC3_WTF2_OP(v0, v1, +);}
gdaVec3 gda_vec3_sub(gdaVec3 v0, gdaVec3 v1){GDA_VEC3_WTF2_OP(v0, v1, -);}
gdaVec3 gda_vec3_mul(gdaVec3 v, float s){GDA_VEC3_WTF1_OP(v, * s);}
gdaVec3 gda_vec3_div(gdaVec3 v, float s){GDA_VEC3_WTF1_OP(v, / s);}

gdaVec4 gda_vec4_add(gdaVec4 v0, gdaVec4 v1){GDA_VEC4_WTF2_OP(v0, v1, +);}
gdaVec4 gda_vec4_sub(gdaVec4 v0, gdaVec4 v1){GDA_VEC4_WTF2_OP(v0, v1, -);}
gdaVec4 gda_vec4_mul(gdaVec4 v, float s){GDA_VEC4_WTF1_OP(v, * s);}
gdaVec4 gda_vec4_div(gdaVec4 v, float s){GDA_VEC4_WTF1_OP(v, / s);}

#undef GDA_VEC2_UNARY_OPERATION
#undef GDA_VEC2_BINARY_OPERATION
#undef GDA_VEC3_UNARY_OPERATION
#undef GDA_VEC3_BINARY_OPERATION
#undef GDA_VEC4_UNARY_OPERATION
#undef GDA_VEC4_BINARY_OPERATION

float gda_vec2_dot(gdaVec2 v0, gdaVec2 v1){ return v0.x * v1.x + v0.y * v1.y; }
float gda_vec3_dot(gdaVec3 v0, gdaVec3 v1){ return v0.x * v1.x + v0.y * v1.y + v0.z * v1.z; }
float gda_vec4_dot(gdaVec4 v0, gdaVec4 v1){ return v0.x * v1.x + v0.y * v1.y + v0.z * v1.z + v0.w * v1.w; }

float gda_vec2_cross(gdaVec2 v0, gdaVec2 v1){ return v0.x * v1.y - v1.x * v0.y; }
gdaVec3 gda_vec3_cross(gdaVec3 v0, gdaVec3 v1){
	gdaVec3 v;
	v.x = v0.y * v1.z - v1.y * v0.z;
	v.y = v0.x * v1.z - v1.x * v0.z;
	v.z = v0.x * v1.y - v1.x * v0.y;
	return(v);
}

float gda_vec2_mag(gdaVec2 v){ return(gda_sqrt(gda_vec2_dot(v, v)));}
float gda_vec3_mag(gdaVec3 v){ return(gda_sqrt(gda_vec3_dot(v, v)));}
float gda_vec4_mag(gdaVec4 v){ return(gda_sqrt(gda_vec4_dot(v, v)));}

float gda_vec2_squared_mag(gdaVec2 v){ return(gda_vec2_dot(v, v)); }
float gda_vec3_squared_mag(gdaVec3 v){ return(gda_vec3_dot(v, v)); }
float gda_vec4_squared_mag(gdaVec4 v){ return(gda_vec4_dot(v, v)); }

gdaVec2 gda_vec2_norm(gdaVec2 v){ return(gda_vec2_mul(v, gda_rsqrt(gda_vec2_dot(v, v)))); }
gdaVec3 gda_vec3_norm(gdaVec3 v){ return(gda_vec3_mul(v, gda_rsqrt(gda_vec3_dot(v, v)))); }
gdaVec4 gda_vec4_norm(gdaVec4 v){ return(gda_vec4_mul(v, gda_rsqrt(gda_vec4_dot(v, v)))); }

gdaVec2 gda_vec2_norm0(gdaVec2 v){ float sqmag = gda_vec2_dot(v, v); return((sqmag) == 0.0f ? gda_vec2_zero() : gda_vec2_mul(v, gda_rsqrt(sqmag))); }
gdaVec3 gda_vec3_norm0(gdaVec3 v){ float sqmag = gda_vec3_dot(v, v); return((sqmag) == 0.0f ? gda_vec3_zero() : gda_vec3_mul(v, gda_rsqrt(sqmag))); }
gdaVec4 gda_vec4_norm0(gdaVec4 v){ float sqmag = gda_vec4_dot(v, v); return((sqmag) == 0.0f ? gda_vec4_zero() : gda_vec4_mul(v, gda_rsqrt(sqmag))); }

gdaVec2 gda_vec2_reflect(gdaVec2 i, gdaVec2 n);
gdaVec3 gda_vec3_reflect(gdaVec3 i, gdaVec3 n);
gdaVec2 gda_vec2_refract(gdaVec2 i, gdaVec2 n, float theta);
gdaVec3 gda_vec3_refract(gdaVec3 i, gdaVec3 n, float theta);




/*
inline gdaVec2 gda_vec2(){
	gdaVec2 res;
	res.x = 0.0f;
	res.y = 0.0f;
	return res;
}

inline gdaVec2 gda_vec2(float x, float y){
	gdaVec2 res;
	res.x = 0.0f;
	res.y = 0.0f;
	return res;
}

inline gdaVec3 gda_vec3(){
	gdaVec3 res;
	res.x = 0.0f;
	res.y = 0.0f;
	res.z = 0.0f;
	return res;
}

inline gdaVec3 gda_vec3(float value){
	gdaVec3 res;
	res.x = value;
	res.y = value;
	res.z = value;
	return res;
}

inline gdaVec3 gda_vec3(float x, float y, float z){
	gdaVec3 res;
	res.x = x;
	res.y = y;
	res.z = z;
	return res;
}

inline gdaVec4 gda_vec4(){
	gdaVec4 res;
	res.x = 0.0f;
	res.y = 0.0f;
	res.z = 0.0f;
	res.w = 0.0f;
	return res;
}

inline gdaVec4 gda_vec4(float x, float y, float z, float w){
	gdaVec4 res;
	res.x = x;
	res.y = y;
	res.z = z;
	res.w = w;
	return res;
}

inline gdaVec4 gda_vec4(gdaVec3 v, float w){
	gdaVec4 res;
	res.x = v.x;
	res.y = v.y;
	res.z = v.z;
	res.w = w;
}

inline gdaQuat gda_quat(){
	gdaQuat Result;
	Result.x = 0.0f;
	Result.y = 0.0f;
	Result.z = 0.0f;
	Result.w = 1.0f;
	return(Result);
}

inline gdaQuat gda_quat(float x, float y, float z, float w){
	gdaQuat res;
	res.x = x;
	res.y = y;
	res.z = z;
	res.w = w;
	return res;
}

inline gdaMat4 gda_mat4(){
	gdaMat4 Result = {};
	return(Result);
}

inline gdaMat4 gda_mat4(float Value){
	gdaMat4 Result = {};
	*(Result.data + 0) = Value;
	*(Result.data + 5) = Value;
	*(Result.data + 10) = Value;
	*(Result.data + 15) = Value;
	return(Result);
}

//gdaVec2 OPERATIONS
inline float Length(gdaVec2 vec){
	return sqrtf(vec.x * vec.x + vec.y * vec.y);
}

inline gdaVec2 Normalize(gdaVec2 vec){
	float len = Length(vec);
	gdaVec2 res;
	res.x = vec.x / len;
	res.y = vec.y / len;
	return res;
}

inline float DotProduct(gdaVec2 v1, gdaVec2 v2){
	return (v1.x * v2.x + v1.y * v2.y);
}

//gdaVec3 OPERATIONS
inline float Length(gdaVec3 vec){
	return sqrtf(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
}

inline gdaVec3 Normalize(gdaVec3 vec){
	float len = Length(vec);
	gdaVec3 res;
	res.x = vec.x / len;
	res.y = vec.y / len;
	res.z = vec.z / len;
	return res;
}

inline float DotProduct(gdaVec3 v1, gdaVec3 v2){
	return (v1.x * v2.x + v1.y * v2.y + v1.z * v2.z);
}

inline gdaVec3 CrossProduct(gdaVec3 p1, gdaVec3 p2){
	gdaVec3 Result;

	Result.x = p1.y * p2.z - p2.y * p1.z;
	Result.y = p1.x * p2.z - p2.x * p1.z;
	Result.z = p1.x * p2.y - p2.x * p1.y;

	return(Result);
}



//gdaVec2 OPERATOR OVERLOADING
inline gdaVec2 operator+(gdaVec2 v1, gdaVec2 v2){
	return gda_vec2(v1.x + v2.x, v1.y + v2.y);
}

inline gdaVec2 operator-(gdaVec2 v1, gdaVec2 v2){
	return gda_vec2(v1.x - v2.x, v1.y - v2.y);
}

inline gdaVec2 operator*(gdaVec2 v, float f){
	return gda_vec2(v.x * f, v.y * f);
}

inline gdaVec2 operator/(gdaVec2 v, float f){
	return gda_vec2(v.x / f, v.y / f);
}

inline gdaVec2& operator*=(gdaVec2& v, float Value){
	v = v * Value;
	return(v);
}

inline gdaVec2& operator/=(gdaVec2& v, float Value){
	v = v / Value;
	return(v);
}

//gdaVec3 OPERATOR OVERLOADING
inline gdaVec3 operator+(gdaVec3 v1, gdaVec3 v2){
	return gda_vec3(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z);
}

inline gdaVec3 operator-(gdaVec3 v1, gdaVec3 v2){
	return gda_vec3(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z);
}

inline gdaVec3 operator*(gdaVec3 v, float f){
	return gda_vec3(v.x * f, v.y * f, v.z * f);
}

inline gdaVec3 operator/(gdaVec3 v, float f){
	return gda_vec3(v.x / f, v.y / f, v.z / f);
}

inline gdaVec3& operator*=(gdaVec3& v, float f){
	v = v * f;
	return(v);
}

inline gdaVec3& operator/=(gdaVec3& v, float f){
	v = v / f;
	return(v);
}

//gdaVec4 OPERATIONS
inline float DotProduct(gdaVec4 v1, gdaVec4 v2){

	float Result;

#ifndef GDA_MATH_ALLOW_SIMD
	Result = (v1.x * v2.x + v1.y * v2.y + v1.z * v2.z + v1.w * v2.w);
#else
	Result = (v1.x * v2.x + v1.y * v2.y + v1.z * v2.z + v1.w * v2.w);
#endif

	return(Result);
}


//gdaQuat OPERATIONS
inline float Length(gdaQuat q) {
	return sqrtf(q.x * q.x + q.y * q.y + q.z * q.z + q.w * q.w);
}

inline gdaQuat NormalizegdaQuat(gdaQuat quat){
	gdaQuat Result;

	float length = Length(quat);
	float x = quat.x / length;
	float y = quat.y / length;
	float z = quat.z / length;
	float w = quat.w / length;

	return(Result);
}

inline gdaQuat ConjugategdaQuat(gdaQuat quat){
	return gda_quat(-quat.x, -quat.y, -quat.z, quat.w);
}

inline gdaQuat MultiplygdaQuatBygdaQuat(gdaQuat l, gdaQuat r){
#if 0 
	float w = l.w * r.w - l.x * r.x - l.y * r.y - l.z * r.z;
	float x = l.x * r.w + l.w * r.x + l.y * r.z - l.z * r.y;
	float y = l.y * r.w + l.w * r.y + l.z * r.x - l.x * r.z;
	float z = l.z * r.w + l.w * r.z + l.x * r.y - l.y * r.x;

#else
	float w = l.w*r.w - l.x*r.x - l.y*r.y - l.z*r.z;
	float x = l.w*r.x + l.x*r.w + l.y*r.z - l.z*r.y;
	float y = l.w*r.y - l.x*r.z + l.y*r.w + l.z*r.x;
	float z = l.w*r.z + l.x*r.y - l.y*r.x + l.z*r.w;
#endif
	return gda_quat(x, y, z, w);
}

inline gdaQuat MultiplygdaQuatByVector(gdaQuat q, gdaVec3 v){
	gdaQuat Result;
	Result.w = -q.x * v.x - q.y * v.y - q.z * v.z;
	Result.x = q.w * v.x + q.y * v.z - q.z * v.y;
	Result.y = q.w * v.y + q.z * v.x - q.x * v.z;
	Result.z = q.w * v.z + q.x * v.y - q.y * v.x;
	return(Result);
}

//MATRIX FUNCTIONS
inline gdaMat4 TransposeMatrix(gdaMat4 mat){
	gdaMat4 res;
	for (int i = 0; i < 4; i++){
		for (int j = 0; j < 4; j++){
			*(res.data + j * 4 + i) = *(mat.data + i * 4 + j);
		}
	}
}


//Maybe make SIMD version of those two
inline gdaMat4 MultiplyMatrices(gdaMat4 m1, gdaMat4 m2){
	gdaMat4 Result = {};
#ifndef GDA_MATH_ALLOW_SIMD
	for (int j = 0; j < 4; j++){
		for (int i = 0; i < 4; i++){

			gdaVec4 SourceV = gda_vec4(
				*(m1.data + j * 4 + 0),
				*(m1.data + j * 4 + 1),
				*(m1.data + j * 4 + 2),
				*(m1.data + j * 4 + 3));
			gdaVec4 DestV = gda_vec4(
				*(m2.data + 0 * 4 + i),
				*(m2.data + 1 * 4 + i),
				*(m2.data + 2 * 4 + i),
				*(m2.data + 3 * 4 + i) );

			*(Result.data + j * 4 + i) = DotProduct(SourceV, DestV);
		}
	}
#else
	__m128 Row1 = _mm_set_ps(m2.data[0], m2.data[1], m2.data[2], m2.data[3]);
	__m128 Row2 = _mm_set_ps(m2.data[4], m2.data[5], m2.data[6], m2.data[7]);
	__m128 Row3 = _mm_set_ps(m2.data[8], m2.data[9], m2.data[10], m2.data[11]);
	__m128 Row4 = _mm_set_ps(m2.data[12], m2.data[13], m2.data[14], m2.data[15]);

	for (int i = 0; i < 4; i++){
		__m128 Broad1 = _mm_set1_ps(*(m1.data + i * 4 + 0));
		__m128 Broad2 = _mm_set1_ps(*(m1.data + i * 4 + 1));
		__m128 Broad3 = _mm_set1_ps(*(m1.data + i * 4 + 2));
		__m128 Broad4 = _mm_set1_ps(*(m1.data + i * 4 + 3));
		__m128 Row = _mm_add_ps(
			_mm_add_ps(
				_mm_mul_ps(Row1, Broad1),
				_mm_mul_ps(Row2, Broad2)),
			_mm_add_ps(
				_mm_mul_ps(Row3, Broad3),
				_mm_mul_ps(Row4, Broad4)));

		__declspec(align(16)) float ResultRowPointer[4];
		_mm_store_ps(ResultRowPointer, Row);
		Result.data[i * 4 + 0] = ResultRowPointer[0];
		Result.data[i * 4 + 1] = ResultRowPointer[1];
		Result.data[i * 4 + 2] = ResultRowPointer[2];
		Result.data[i * 4 + 3] = ResultRowPointer[3];
	}
#endif
	return(Result);
}

inline gdaVec4 MultiplyMatrixBygdaVec4(gdaMat4 mat, gdaVec4 vec){
	gdaVec4 Result;
#ifndef GDA_MATH_ALLOW_SIMD
	for (int j = 0; j < 4; j++){
		gdaVec4 SourceV = gda_vec4(
			mat.data[j * 4 + 0],
			mat.data[j * 4 + 1],
			mat.data[j * 4 + 2],
			mat.data[j * 4 + 3]);

		Result.data[j] = DotProduct(SourceV, vec);
	}
#else
#if 0
	__m128 Mem1 = _mm_set1_ps(vec.data[0]);
	__m128 Mem2 = _mm_set1_ps(vec.data[1]);
	__m128 Mem3 = _mm_set1_ps(vec.data[2]);
	__m128 Mem4 = _mm_set1_ps(vec.data[3]);

	__m128 Column1 = _mm_set_ps(mat.d1, mat.c1, mat.b1, mat.a1);
	__m128 Column2 = _mm_set_ps(mat.d2, mat.c2, mat.b2, mat.a2);
	__m128 Column3 = _mm_set_ps(mat.d3, mat.c3, mat.b3, mat.a3);
	__m128 Column4 = _mm_set_ps(mat.d4, mat.c4, mat.b4, mat.a4);

	__m128 Res = _mm_add_ps(
		_mm_add_ps(
			_mm_mul_ps(Mem1, Column1),
			_mm_mul_ps(Mem2, Column2)),
		_mm_add_ps(
			_mm_mul_ps(Mem3, Column3),
			_mm_mul_ps(Mem4, Column4)));

#else
	__m128 OurVec = _mm_set_ps(vec.w, vec.z, vec.y, vec.x);

	__m128 Row1 = _mm_load_ps(&mat.a1);
	__m128 Row2 = _mm_load_ps(&mat.b1);
	__m128 Row3 = _mm_load_ps(&mat.c1);
	__m128 Row4 = _mm_load_ps(&mat.d1);

	__m128 Res = _mm_add_ps(
		_mm_add_ps(
			_mm_mul_ps(OurVec, Row1),
			_mm_mul_ps(OurVec, Row2)),
		_mm_add_ps(
			_mm_mul_ps(OurVec, Row3),
			_mm_mul_ps(OurVec, Row4)));
#endif

	__declspec(align(16)) float TempArray[4];
	_mm_store_ps(TempArray, Res);
	Result.data[0] = TempArray[0];
	Result.data[1] = TempArray[1];
	Result.data[2] = TempArray[2];
	Result.data[3] = TempArray[3];

#endif
	return(Result);
}

inline gdaMat4 RotationMatrix(gdaQuat qu){
	gdaMat4 result;

	*(result.data + 1)  = 1.0 - 2 * qu.y * qu.y - 2 * qu.z * qu.z;
	*(result.data + 2)  = 2 * qu.x * qu.y + 2 * qu.z * qu.w;
	*(result.data + 3)  = 2 * qu.x * qu.z - 2 * qu.y * qu.w;
	*(result.data + 4)  = 0.0f;
					    
	*(result.data + 4)  = 2 * qu.x * qu.y - 2 * qu.z * qu.w;
	*(result.data + 5)  = 1.0 - 2 * qu.x * qu.x - 2 * qu.z * qu.z;
	*(result.data + 6)  = 2 * qu.y * qu.z + 2 * qu.x * qu.w;
	*(result.data + 7)  = 0.0f;
					    
	*(result.data + 8)  = 2 * qu.x * qu.z + 2 * qu.y * qu.w;
	*(result.data + 9)  = 2 * qu.y * qu.z - 2 * qu.x * qu.w;
	*(result.data + 10) = 1.0 - 2 * qu.x * qu.x - 2 * qu.y * qu.y;
	*(result.data + 11) = 0.0f;

	*(result.data + 12) = 0.0f;
	*(result.data + 13) = 0.0f;
	*(result.data + 14) = 0.0f;
	*(result.data + 15) = 1.0f;

	return result;
}

inline gdaMat4 OrthograficProjectionMatrix(int Left, int Right, int Bottom, int Top){
	gdaMat4 Result = {};
	*(Result.data) = 2.0f / (Right - Left);
	*(Result.data + 3) = (Right + Left) / (Right - Left);

	*(Result.data + 5) = 2.0f / (Top - Bottom);
	*(Result.data + 7) = (Top + Bottom) / (Top - Bottom);

	*(Result.data + 10) = 1.0f;
	*(Result.data + 15) = 1.0f;
	return(Result);
}

inline gdaMat4 OrthograficProjectionMatrix(int Left, int Right, int Bottom, int Top, int Near, int Far){
	gdaMat4 Result = {};

	*(Result.data) = 2.0f / (Right - Left);
	*(Result.data + 3) = (Right + Left) / (Right - Left);

	*(Result.data + 5) = 2.0f / (Top - Bottom);
	*(Result.data + 7) = (Top + Bottom) / (Top - Bottom);

	*(Result.data + 10) = -2.0f / (Far - Near);
	*(Result.data + 11) = (Far + Near) / (Far - Near);

	*(Result.data + 15) = 1.0f;

	return(Result);
}

inline gdaMat4 PerspectiveProjectionMatrix(int Left, int Right, int Bottom, int Top, int Near, int Far){
	gdaMat4 Result;
	*(Result.data + 1) = 2.0f * Near / (Right - Left);
	*(Result.data + 2) = 0.0f;
	*(Result.data + 3) = (Right + Left) / (Right - Left);
	*(Result.data + 4) = 0.0f;

	*(Result.data + 4) = 0.0f;
	*(Result.data + 5) = 2 * Near / (Top - Bottom);
	*(Result.data + 6) = (Top + Bottom) / (Top - Bottom);
	*(Result.data + 7) = 0.0f;

	*(Result.data + 8) = 0.0f;
	*(Result.data + 9) = 0.0f;
	*(Result.data + 10) = -1.0f * (Far + Near) / (Far - Near);
	*(Result.data + 11) = -2.0f * Far * Near / (Far - Near);

	*(Result.data + 12) = 0.0f;
	*(Result.data + 13) = 0.0f;
	*(Result.data + 14) = -1.0f;
	*(Result.data + 15) = 0;
	return(Result);
}

inline gdaMat4 operator *(gdaMat4 m1, gdaMat4 m2){
	return MultiplyMatrices(m1, m2);
}

inline gdaVec4 operator*(gdaMat4 mat, gdaVec4 vec){
	return MultiplyMatrixBygdaVec4(mat, vec);
}
*/

#endif



#define GDA_MATH_H
#endif