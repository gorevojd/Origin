#ifndef GDA_MATH_H

#ifdef GDA_MATH_ALLOW_SIMD
#include <xmmintrin.h>
#endif

#ifdef IMPLEMENT_MATH_OURSELVES
#include <intrin.h>
#else
#include <math.h>
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
		struct{ float x, y, z, w;};
		struct{ gdaVec3 xyz; float w; };
		struct{ gdaVec3 rgb; float a; };
		struct{ gdaVec2 xy; gdaVec2 zw; };
		float data[4];
	};
};

typedef union gdaMat2{
	struct{ gdaVec2 x, y; };
	gdaVec2 row[2];
	float data[4];
};

typedef union gdaMat3{
	struct{ gdaVec3 x, y, z; };
	gdaVec3 row[3];
	float data[9];
};

typedef union gdaMat4{
	struct{ gdaVec4 x, y, z, w; };
	gdaVec4 row[4];
	float data[16];
};

struct gdaQuat{
	union{
		struct {
			float x, y, z, w;
		};
		gdaVec3 xyz;
		gdaVec4 xyzw;
	};
};

typedef float gdaFloat2[2];
typedef float gdaFloat3[3];
typedef float gdaFloat4[4];

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

#define GDA_DEG_TO_RAD 0.0174533
#define GDA_RAD_TO_DEG 57.2958

#define GDA_MATH_CONSTANTS
#endif

#ifdef __cplusplus
extern "C" {
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
	GDA_MATH_DEF float gda_to_radians(float degrees);
	GDA_MATH_DEF float gda_to_degrees(float radians);
	
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

	GDA_MATH_DEF float gda_vec2_aspect_ratio(gdaVec2 v);

	/*Matrix definitions*/
	GDA_MATH_DEF void gda_mat2_identity(gdaMat2* m);
	GDA_MATH_DEF void gda_mat2_transpose(gdaMat2* m);
	GDA_MATH_DEF void gda_mat2_mul(gdaMat2* out, gdaMat2* m1, gdaMat2* m2);
	GDA_MATH_DEF gdaVec2 gda_mat2_mul_vec2(gdaMat2* m, gdaVec2 v);
	GDA_MATH_DEF void gda_mat2_inverse(gdaMat2* out, gdaMat2* m);
	GDA_MATH_DEF float gda_mat2_determinant(gdaMat2* m);

	GDA_MATH_DEF void gda_mat3_identity(gdaMat3* m);
	GDA_MATH_DEF void gda_mat3_transpose(gdaMat3* m);
	GDA_MATH_DEF void gda_mat3_mul(gdaMat3* out, gdaMat3* m1, gdaMat3* m2);
	GDA_MATH_DEF gdaVec3 gda_mat3_mul_vec3(gdaMat3* m, gdaVec3 v);
	GDA_MATH_DEF void gda_mat3_inverse(gdaMat3* out, gdaMat3* m);
	GDA_MATH_DEF float gda_mat3_determinant(gdaMat3* m);

	GDA_MATH_DEF void gda_mat4_identity(gdaMat4* m);
	GDA_MATH_DEF void gda_mat4_transpose(gdaMat4* m);
	GDA_MATH_DEF void gda_mat4_mul(gdaMat4* out, gdaMat4* m1, gdaMat4* m2);
	GDA_MATH_DEF gdaVec4 gda_mat4_mul_vec4(gdaMat4* m, gdaVec4 v);
	GDA_MATH_DEF void gda_mat4_inverse(gdaMat4* out, gdaMat4* in);

	GDA_MATH_DEF void gda_mat4_translate(gdaMat4* out, gdaVec3 v);
	GDA_MATH_DEF void gda_mat4_rotate(gdaMat4* out, gdaVec3 v, float angle_radians);
	GDA_MATH_DEF void gda_mat4_scale(gdaMat4* out, gdaVec3 v);
	GDA_MATH_DEF void gda_mat4_scalef(gdaMat4* out, float s);
	GDA_MATH_DEF void gda_mat4_ortho2d(gdaMat4* out, float left, float right, float bottom, float top);
	GDA_MATH_DEF void gda_mat4_ortho3d(gdaMat4* out, float left, float right, float bottom, float top, float z_near, float z_far);
	GDA_MATH_DEF void gda_mat4_perspective(gdaMat4* out, float fovy, float aspect, float z_near, float z_far);
	GDA_MATH_DEF void gda_mat4_infinite_perspective(gdaMat4* out, float fovy, float aspect, float z_near);

	GDA_MATH_DEF gdaMat4 gda_mat4_look_at(gdaVec3 eye, gdaVec3 centre, gdaVec3 up);

	GDA_MATH_DEF void gda_float22_identity(float m[2][2]);
	GDA_MATH_DEF gdaFloat2* gda_float22_m(gdaMat2* m);
	GDA_MATH_DEF gdaFloat2* gda_float22_v(gdaVec2 m[2]);
	GDA_MATH_DEF gdaFloat2* gda_float22_4(float m[4]);
	GDA_MATH_DEF void gda_float22_transpose(float(*vec)[2]);
	GDA_MATH_DEF void gda_float22_mul(float(*out)[2], float(*mat1)[2], float(*mat2)[2]);
	GDA_MATH_DEF gdaVec2 gda_float22_mul_vec2(float m[2][2], gdaVec2 v1);

	GDA_MATH_DEF void gda_float33_identity(float m[3][3]);
	GDA_MATH_DEF gdaFloat3* gda_float33_m(gdaMat3* m);
	GDA_MATH_DEF gdaFloat3* gda_float33_v(gdaVec3 m[3]);
	GDA_MATH_DEF gdaFloat3* gda_float33_9(float m[9]);
	GDA_MATH_DEF void gda_float33_transpose(float(*vec)[3]);
	GDA_MATH_DEF void gda_float33_mul(float(*out)[3], float(*mat1)[3], float(*mat2)[3]);
	GDA_MATH_DEF gdaVec3 gda_float33_mul_vec3(float m[3][3], gdaVec3 v1);

	GDA_MATH_DEF void gda_float44_identity(float m[4][4]);
	GDA_MATH_DEF gdaFloat4* gda_float44_m(gdaMat4* m);
	GDA_MATH_DEF gdaFloat4* gda_float44_v(gdaVec4 m[4]);
	GDA_MATH_DEF gdaFloat4* gda_float44_16(float m[16]);
	GDA_MATH_DEF void gda_float44_transpose(float(*vec)[4]);
	GDA_MATH_DEF void gda_float44_mul(float(*out)[4], float(*mat1)[4], float(*mat2)[4]);
	GDA_MATH_DEF gdaVec4 gda_float44_mul_vec4(float m[4][4], gdaVec4 v1);

	/*Quaternions definitions*/
	GDA_MATH_DEF gdaQuat gda_quat(float x, float y, float z, float w);
	GDA_MATH_DEF gdaQuat gda_quat_from_array(float arr[4]);
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
	GDA_MATH_DEF void gda_mat4_from_quat(gdaMat4* out, gdaQuat q);
	GDA_MATH_DEF gdaQuat gda_quat_from_mat4(gdaMat4* m);

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

#ifdef __cplusplus
}
#endif

#define GDA_MATH_H
#endif

#define GDA_MATH_IMPLEMENTATION
#ifdef GDA_MATH_IMPLEMENTATION

static void gda__memcpy_4byte(void *dest, void const *src, size_t size){
	size_t i;
	unsigned int *d, *s;
	d = (unsigned int *)dest;
	s = (unsigned int *)src;
	for (int i = 0; i < size / 4; i++){
		*d++ = *s++;
	}
}

float gda_to_radians(float degrees){ return(degrees * GDA_DEG_TO_RAD); }
float gda_to_degrees(float radians){ return(radians * GDA_RAD_TO_DEG); }

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

gdaVec2 gda_vec2_reflect(gdaVec2 i, gdaVec2 n){return(gda_vec2_sub(i, gda_vec2_mul(n, 2.0f * gda_vec2_dot(n, i))));}
gdaVec3 gda_vec3_reflect(gdaVec3 i, gdaVec3 n){return(gda_vec3_sub(i, gda_vec3_mul(n, 2.0f * gda_vec3_dot(n, i))));}
gdaVec2 gda_vec2_refract(gdaVec2 i, gdaVec2 n, float theta){
	gdaVec2 r, a, b;
	float dv, k;
	dv = gda_vec2_dot(n, i);
	k = 1.0f - theta * theta * (1.0f - dv * dv);
	a = gda_vec2_mul(i, theta);
	b = gda_vec2_mul(n, theta * dv * gda_sqrt(k));
	r = gda_vec2_sub(a, b);
	r = gda_vec2_mul(r, (float)(k >= 0.0f));
	return(r);
}
gdaVec3 gda_vec3_refract(gdaVec3 i, gdaVec3 n, float theta){
	gdaVec3 r, a, b;
	float dv, k;
	dv = gda_vec3_dot(n, i);
	k = 1.0f - theta * theta * (1.0f - dv * dv);
	a = gda_vec3_mul(i, theta);
	b = gda_vec3_mul(n, theta * dv * gda_sqrt(k));
	r = gda_vec3_sub(a, b);
	r = gda_vec3_mul(r, (float)(k >= 0.0f));
	return(r);
}

float gda_vec2_aspect_ratio(gdaVec2 v){ return ((v.y < 0.0001f) ? 0.0f : v.x / v.y); }

/*Matrices implementation*/
void gda_mat2_identity(gdaMat2* m){ gda_float22_identity(gda_float22_m(m)); }
void gda_mat2_transpose(gdaMat2* m){ gda_float22_transpose(gda_float22_m(m)); }
void gda_mat2_mul(gdaMat2* out, gdaMat2* m1, gdaMat2* m2){ gda_float22_mul(gda_float22_m(out), gda_float22_m(m1), gda_float22_m(m2)); }
gdaVec2 gda_mat2_mul_vec2(gdaMat2* m, gdaVec2 v){ return(gda_float22_mul_vec2(gda_float22_m(m), v));}

void gda_mat2_inverse(gdaMat2* out, gdaMat2* m){
	gdaFloat2 *o = gda_float22_m(out);
	gdaFloat2 *i = gda_float22_m(m);
	float ood = 1.0f / gda_mat2_determinant(m);

	o[0][0] = +i[1][1] * ood;
	o[0][1] = -i[0][1] * ood;
	o[1][0] = -i[1][0] * ood;
	o[1][1] = +i[0][0] * ood;
}

float gda_mat2_determinant(gdaMat2* m){
	gdaFloat2* e = gda_float22_m(m);
	return (e[0][0] * e[1][1] - e[1][0] * e[0][1]);
}

void gda_mat3_identity(gdaMat3* m){ gda_float33_identity(gda_float33_m(m)); }
void gda_mat3_transpose(gdaMat3* m){ gda_float33_transpose(gda_float33_m(m)); }
void gda_mat3_mul(gdaMat3* out, gdaMat3* m1, gdaMat3* m2) { gda_float33_mul(gda_float33_m(out), gda_float33_m(m1), gda_float33_m(m2)); }
gdaVec3 gda_mat3_mul_vec3(gdaMat3* m, gdaVec3 v){ return(gda_float33_mul_vec3(gda_float33_m(m), v)); }

void gda_mat3_inverse(gdaMat3* out, gdaMat3* m){
	gdaFloat3 *o = gda_float33_m(out);
	gdaFloat3 *i = gda_float33_m(m);

	float ood = 1.0f / gda_mat3_determinant(m);

	o[0][0] = +(i[1][1] * i[2][2] - i[2][1] * i[1][2]) * ood;
	o[0][1] = -(i[1][0] * i[2][2] - i[2][0] * i[1][2]) * ood;
	o[0][2] = +(i[1][0] * i[2][1] - i[2][0] * i[1][1]) * ood;
	o[1][0] = -(i[0][1] * i[2][2] - i[2][1] * i[0][2]) * ood;
	o[1][1] = +(i[0][0] * i[2][2] - i[2][0] * i[0][2]) * ood;
	o[1][2] = -(i[0][0] * i[2][1] - i[2][0] * i[0][1]) * ood;
	o[2][0] = +(i[0][1] * i[1][2] - i[1][1] * i[0][2]) * ood;
	o[2][1] = -(i[0][0] * i[1][2] - i[1][0] * i[0][2]) * ood;
	o[2][2] = +(i[0][0] * i[1][1] - i[1][0] * i[0][1]) * ood;
}

float gda_mat3_determinant(gdaMat3* m){
	gdaFloat3 *e = gda_float33_m(m);
	return(+e[0][0] * (e[1][1] * e[2][2] - e[1][2] * e[2][1])
		- e[0][1] * (e[1][0] * e[2][2] - e[1][2] * e[2][0])
		+ e[0][2] * (e[1][0] * e[2][1] - e[1][1] * e[2][0]));
}

void gda_mat4_identity(gdaMat4* m){ gda_float44_identity(gda_float44_m(m)); }
void gda_mat4_transpose(gdaMat4* m){ gda_float44_transpose(gda_float44_m(m)); }
void gda_mat4_mul(gdaMat4* out, gdaMat4* m1, gdaMat4* m2){ gda_float44_mul(gda_float44_m(out), gda_float44_m(m1), gda_float44_m(m2)); }
gdaVec4 gda_mat4_mul_vec4(gdaMat4* m, gdaVec4 v){ return(gda_float44_mul_vec4(gda_float44_m(m), v)); }

void gda_mat4_inverse(gdaMat4* out, gdaMat4* in){
	gdaFloat4 *o = gda_float44_m(out);
	gdaFloat4 *m = gda_float44_m(in);

	float ood;

	float sf00 = m[2][2] * m[3][3] - m[3][2] * m[2][3];
	float sf01 = m[2][1] * m[3][3] - m[3][1] * m[2][3];
	float sf02 = m[2][1] * m[3][2] - m[3][1] * m[2][2];
	float sf03 = m[2][0] * m[3][3] - m[3][0] * m[2][3];
	float sf04 = m[2][0] * m[3][2] - m[3][0] * m[2][2];
	float sf05 = m[2][0] * m[3][1] - m[3][0] * m[2][1];
	float sf06 = m[1][2] * m[3][3] - m[3][2] * m[1][3];
	float sf07 = m[1][1] * m[3][3] - m[3][1] * m[1][3];
	float sf08 = m[1][1] * m[3][2] - m[3][1] * m[1][2];
	float sf09 = m[1][0] * m[3][3] - m[3][0] * m[1][3];
	float sf10 = m[1][0] * m[3][2] - m[3][0] * m[1][2];
	float sf11 = m[1][1] * m[3][3] - m[3][1] * m[1][3];
	float sf12 = m[1][0] * m[3][1] - m[3][0] * m[1][1];
	float sf13 = m[1][2] * m[2][3] - m[2][2] * m[1][3];
	float sf14 = m[1][1] * m[2][3] - m[2][1] * m[1][3];
	float sf15 = m[1][1] * m[2][2] - m[2][1] * m[1][2];
	float sf16 = m[1][0] * m[2][3] - m[2][0] * m[1][3];
	float sf17 = m[1][0] * m[2][2] - m[2][0] * m[1][2];
	float sf18 = m[1][0] * m[2][1] - m[2][0] * m[1][1];

	o[0][0] = +(m[1][1] * sf00 - m[1][2] * sf01 + m[1][3] * sf02);
	o[0][1] = -(m[1][0] * sf00 - m[1][2] * sf03 + m[1][3] * sf04);
	o[0][2] = +(m[1][0] * sf01 - m[1][1] * sf03 + m[1][3] * sf05);
	o[0][3] = -(m[1][0] * sf02 - m[1][1] * sf04 + m[1][2] * sf05);

	o[1][0] = -(m[0][1] * sf00 - m[0][2] * sf01 + m[0][3] * sf02);
	o[1][1] = +(m[0][0] * sf00 - m[0][2] * sf03 + m[0][3] * sf04);
	o[1][2] = -(m[0][0] * sf01 - m[0][1] * sf03 + m[0][3] * sf05);
	o[1][3] = +(m[0][0] * sf02 - m[0][1] * sf04 + m[0][2] * sf05);

	o[2][0] = +(m[0][1] * sf06 - m[0][2] * sf07 + m[0][3] * sf08);
	o[2][1] = -(m[0][0] * sf06 - m[0][2] * sf09 + m[0][3] * sf10);
	o[2][2] = +(m[0][0] * sf11 - m[0][1] * sf09 + m[0][3] * sf12);
	o[2][3] = -(m[0][0] * sf08 - m[0][1] * sf10 + m[0][2] * sf12);

	o[3][0] = -(m[0][1] * sf13 - m[0][2] * sf14 + m[0][3] * sf15);
	o[3][1] = +(m[0][0] * sf13 - m[0][2] * sf16 + m[0][3] * sf17);
	o[3][2] = -(m[0][0] * sf14 - m[0][1] * sf16 + m[0][3] * sf18);
	o[3][3] = +(m[0][0] * sf15 - m[0][1] * sf17 + m[0][2] * sf18);

	ood = 1.0f / (
		m[0][0] * o[0][0] +
		m[0][1] * o[0][1] +
		m[0][2] * o[0][2] +
		m[0][3] * o[0][3]);

	o[0][0] *= ood;
	o[0][1] *= ood;
	o[0][2] *= ood;
	o[0][3] *= ood;
	o[1][0] *= ood;
	o[1][1] *= ood;
	o[1][2] *= ood;
	o[1][3] *= ood;
	o[2][0] *= ood;
	o[2][1] *= ood;
	o[2][2] *= ood;
	o[2][3] *= ood;
	o[3][0] *= ood;
	o[3][1] *= ood;
	o[3][2] *= ood;
	o[3][3] *= ood;
}

void gda_mat4_translate(gdaMat4* out, gdaVec3 v){
	gda_mat4_identity(out);
	out->x.data[3] = v.x;
	out->y.data[3] = v.y;
	out->z.data[3] = v.y;
	out->data[15] = 1;
}

void gda_mat4_rotate(gdaMat4* out, gdaVec3 v, float angle_radians){
	float c, s;
	gdaVec3 axis, t;
	gdaFloat4* rot;

	c = gda_cos(angle_radians);
	s = gda_sin(angle_radians);

	axis = gda_vec3_norm(v);
	t = gda_vec3_mul(axis, 1.0f - c);

	gda_mat4_identity(out);
	rot = gda_float44_m(out);

	rot[0][0] = c + t.x*axis.x;
	rot[0][1] = 0 + t.x*axis.y + s*axis.z;
	rot[0][2] = 0 + t.x*axis.z - s*axis.y;
	rot[0][3] = 0;

	rot[1][0] = 0 + t.y*axis.x - s*axis.z;
	rot[1][1] = c + t.y*axis.y;
	rot[1][2] = 0 + t.y*axis.z + s*axis.x;
	rot[1][3] = 0;

	rot[2][0] = 0 + t.z*axis.x + s*axis.y;
	rot[2][1] = 0 + t.z*axis.y - s*axis.x;
	rot[2][2] = c + t.z*axis.z;
	rot[2][3] = 0;
}

void gda_mat4_scale(gdaMat4* out, gdaVec3 v){
	gda_mat4_identity(out);
	out->data[0] = v.x;
	out->data[5] = v.y;
	out->data[10] = v.z;
}

void gda_mat4_scalef(gdaMat4* out, float s){
	gda_mat4_identity(out);
	out->data[0] = s;
	out->data[5] = s;
	out->data[10] = s;
}

void gda_mat4_ortho2d(gdaMat4* out, float left, float right, float bottom, float top){
	gdaFloat4* m;
	gda_mat4_identity(out);
	m = gda_float44_m(out);

	m[0][0] = 2.0f / (right - left);
	m[1][1] = 2.0f / (top - bottom);
	m[2][2] = -1.0f;
	m[3][0] = -(right + left) / (right - left);
	m[3][1] = -(top + bottom) / (top - bottom);
}

void gda_mat4_ortho3d(gdaMat4* out, float left, float right, float bottom, float top, float z_near, float z_far){
	gdaFloat4 *m;
	gda_mat4_identity(out);
	m = gda_float44_m(out);

	m[0][0] = +2.0f / (right - left);
	m[1][1] = +2.0f / (top - bottom);
	m[2][2] = -2.0f / (z_far - z_near);
	m[3][0] = -(right + left) / (right - left);
	m[3][1] = -(top + bottom) / (top - bottom);
	m[3][2] = -(z_far + z_near) / (z_far - z_near);
}

void gda_mat4_perspective(gdaMat4* out, float fovy, float aspect, float z_near, float z_far){
	float tan_half_fovy = gda_tan(0.5f * fovy);
	gdaMat4 zero_mat = { 0 };
	gdaFloat4 *m = gda_float44_m(out);
	*out = zero_mat;

	m[0][0] = 1.0f / (aspect*tan_half_fovy);
	m[1][1] = 1.0f / (tan_half_fovy);
	m[2][2] = -(z_far + z_near) / (z_far - z_near);
	m[2][3] = -1.0f;
	m[3][2] = -2.0f*z_far*z_near / (z_far - z_near);
}

void gda_mat4_infinite_perspective(gdaMat4* out, float fovy, float aspect, float z_near){
	float range = gda_tan(0.5f * fovy) * z_near;
	float left = -range * aspect;
	float right = range * aspect;
	float bottom = -range;
	float top = range;
	gdaMat4 zero_mat = { 0 };
	gdaFloat4 *m = gda_float44_m(out);
	*out = zero_mat;

	m[0][0] = (2.0f*z_near) / (right - left);
	m[1][1] = (2.0f*z_near) / (top - bottom);
	m[2][2] = -1.0f;
	m[2][3] = -1.0f;
	m[3][2] = -2.0f*z_near;
}

void gda_mat4_look_at(gdaMat4* out, gdaVec3 eye, gdaVec3 centre, gdaVec3 up){
	gdaVec3 f, s, u;
	gdaFloat4 *m;

	f = gda_vec3_sub(centre, eye);
	f = gda_vec3_norm(f);

	s = gda_vec3_cross(f, up);
	s = gda_vec3_norm(s);

	u = gda_vec3_cross(s, f);

	gda_mat4_identity(out);
	m = gda_float44_m(out);

	m[0][0] = +s.x;
	m[1][0] = +s.y;
	m[2][0] = +s.z;

	m[0][1] = +u.x;
	m[1][1] = +u.y;
	m[2][1] = +u.z;

	m[0][2] = -f.x;
	m[1][2] = -f.y;
	m[2][2] = -f.z;

	m[3][0] = gda_vec3_dot(s, eye);
	m[3][1] = gda_vec3_dot(u, eye);
	m[3][2] = gda_vec3_dot(f, eye);
}

void gda_float22_identity(float m[2][2]){
	m[0][0] = 1; m[0][1] = 0;
	m[1][0] = 0; m[1][1];
}
gdaFloat2* gda_float22_m(gdaMat2* m){ return ((gdaFloat2*)m); }
gdaFloat2* gda_float22_v(gdaVec2 m[2]){ return ((gdaFloat2*)m); }
gdaFloat2* gda_float22_4(float m[4]){ return ((gdaFloat2*)m); }
void gda_float22_transpose(float(*vec)[2]){
	int i, j;
	for (j = 0; j < 2; j++){
		for (i = j + 1; i < 2; i++){
			float t = vec[i][j];
			vec[i][j] = vec[j][i];
			vec[j][i] = t;
		}
	}
}
void gda_float22_mul(float(*out)[2], float(*mat1)[2], float(*mat2)[2]){
	int i, j;
	float temp1[2][2], temp2[2][2];
	if (mat1 == out){ gda__memcpy_4byte(temp1, mat1, sizeof(temp1)); mat1 = temp1; }
	if (mat2 == out){ gda__memcpy_4byte(temp2, mat2, sizeof(temp2)); mat2 = temp2; }
	for (j = 0; j < 2; j++){
		for (int i = 0; i < 2; i++){
			out[j][i] =
				mat1[0][i] * mat2[j][0] +
				mat1[1][i] * mat2[j][i];
		}
	}
}
gdaVec2 gda_float22_mul_vec2(float m[2][2], gdaVec2 v1){
	gdaVec2 r;
	r.x = m[0][0] * v1.x + m[0][1] * v1.y;
	r.y = m[1][0] * v1.x + m[1][1] * v1.y;
	return(r);
}

void gda_float33_identity(float m[3][3]){
	m[0][0] = 1; m[0][1] = 0; m[0][2] = 0;
	m[1][0] = 0; m[1][1] = 1; m[1][2] = 0;
	m[2][0] = 0; m[2][1] = 0; m[2][2] = 1;
}
gdaFloat3* gda_float33_m(gdaMat3* m){ return((gdaFloat3*)m); }
gdaFloat3* gda_float33_v(gdaVec3 m[3]){ return((gdaFloat3*)m); }
gdaFloat3* gda_float33_9(float m[9]){ return((gdaFloat3*)m); }
void gda_float33_transpose(float(*vec)[3]){
	int i, j;
	for (j = 0; j < 3; j++) {
		for (i = j + 1; i < 3; i++) {
			float t = vec[i][j];
			vec[i][j] = vec[j][i];
			vec[j][i] = t;
		}
	}
}
void gda_float33_mul(float(*out)[3], float(*mat1)[3], float(*mat2)[3]){
	int i, j;
	float temp1[3][3], temp2[3][3];
	if (mat1 == out) { gda__memcpy_4byte(temp1, mat1, sizeof(temp1)); mat1 = temp1; }
	if (mat2 == out) { gda__memcpy_4byte(temp2, mat2, sizeof(temp2)); mat2 = temp2; }
	for (j = 0; j < 3; j++) {
		for (i = 0; i < 3; i++) {
			out[j][i] =
				mat1[0][i] * mat2[j][0] + 
				mat1[1][i] * mat2[j][1] +
				mat1[2][i] * mat2[j][2];
		}
	}
}
gdaVec3 gda_float33_mul_vec3(float m[3][3], gdaVec3 v1){
	gdaVec3 r;
	r.x = m[0][0] * v1.x + m[0][1] * v1.y + m[0][2] * v1.z;
	r.y = m[1][0] * v1.x + m[1][1] * v1.y + m[1][2] * v1.z;
	r.z = m[2][0] * v1.x + m[2][1] * v1.y + m[2][2] * v1.z;
	return(r);
}

void gda_float44_identity(float m[4][4]){
	m[0][0] = 1; m[0][1] = 0; m[0][2] = 0; m[0][3] = 0;
	m[1][0] = 0; m[1][1] = 1; m[1][2] = 0; m[1][3] = 0;
	m[2][0] = 0; m[2][1] = 0; m[2][2] = 1; m[2][3] = 0;
	m[3][0] = 0; m[3][1] = 0; m[3][2] = 0; m[3][3] = 1;
}

gdaFloat4* gda_float44_m(gdaMat4* m){ return ((gdaFloat4*)m); }
gdaFloat4* gda_float44_v(gdaVec4 m[4]){ return ((gdaFloat4*)m); }
gdaFloat4* gda_float44_16(float m[16]){ return ((gdaFloat4*)m); }

void gda_float44_transpose(float(*vec)[4]){
	int i, j;
	for (j = 0; j < 4; j++) {
		for (i = j + 1; i < 4; i++) {
			float t = vec[i][j];
			vec[i][j] = vec[j][i];
			vec[j][i] = t;
		}
	}
}

void gda_float44_mul(float(*out)[4], float(*mat1)[4], float(*mat2)[4]){
	int i, j;
	float temp1[4][4], temp2[4][4];
	if (mat1 == out) { gda__memcpy_4byte(temp1, mat1, sizeof(temp1)); mat1 = temp1; }
	if (mat2 == out) { gda__memcpy_4byte(temp2, mat2, sizeof(temp2)); mat2 = temp2; }
	for (j = 0; j < 4; j++) {
		for (i = 0; i < 4; i++) {
			out[j][i] = mat1[0][i] * mat2[j][0]
				+ mat1[1][i] * mat2[j][1]
				+ mat1[2][i] * mat2[j][2]
				+ mat1[3][i] * mat2[j][3];
		}
	}
}

gdaVec4 gda_float44_mul_vec4(float m[4][4], gdaVec4 v1){
	gdaVec4 r;
	r.x = m[0][0] * v1.x + m[1][0] * v1.y + m[2][0] * v1.z + m[3][0] * v1.w;
	r.y = m[0][1] * v1.x + m[1][1] * v1.y + m[2][1] * v1.z + m[3][1] * v1.w;
	r.z = m[0][2] * v1.x + m[1][2] * v1.y + m[2][2] * v1.z + m[3][2] * v1.w;
	r.w = m[0][3] * v1.x + m[1][3] * v1.y + m[2][3] * v1.z + m[3][3] * v1.w;
	return(r);
}

/*Quaternions implementation*/
gdaQuat gda_quat(float x, float y, float z, float w){ gdaQuat q; q.x = x; q.y = y; q.z = z; q.w = w; return(q); }
gdaQuat gda_quat_from_array(float arr[4]){ gdaQuat q; q.x = arr[0]; q.y = arr[1]; q.z = arr[2]; q.w = arr[3]; return (q); }

gdaQuat gda_quat_from_angle(gdaVec3 axis, float angle_radians){
	gdaQuat q;
	q.xyz = gda_vec3_mul(gda_vec3_norm(axis), gda_sin(0.5f * angle_radians));
	q.w = gda_cos(0.5f * angle_radians);
	return q;
}
gdaQuat gda_quat_from_euler_angles(float pitch, float yaw, float roll){
	gdaQuat q, p, y, r;
	p = gda_quat_from_angle(gda_vec3(1, 0, 0), pitch);
	y = gda_quat_from_angle(gda_vec3(0, 1, 0), yaw);
	r = gda_quat_from_angle(gda_vec3(0, 0, 1), roll);
	q = gda_quat_mul(y, p);
	q = gda_quat_mul(q, r);
	return q;
}
gdaQuat gda_quat_identity(void){ gdaQuat q = { 0, 0, 0, 1 }; return q; }

gdaQuat gda_quat_add(gdaQuat q1, gdaQuat q2){gdaQuat q; q.xyzw = gda_vec4_add(q1.xyzw, q2.xyzw); return(q);}
gdaQuat gda_quat_sub(gdaQuat q1, gdaQuat q2){gdaQuat q; q.xyzw = gda_vec4_sub(q1.xyzw, q2.xyzw); return(q);}
gdaQuat gda_quat_mul(gdaQuat q1, gdaQuat q2){
	gdaQuat q;
	q.x = q1.w * q2.x + q1.x * q2.w + q1.y * q2.z - q1.z * q2.y;
	q.y = q1.w * q2.y - q1.x * q2.z + q1.y * q2.w + q1.z * q2.x;
	q.z = q1.w * q2.z + q1.x * q2.y - q1.y * q2.x + q1.z * q2.w;
	q.w = q1.w * q2.w - q1.x * q2.x - q1.y * q2.y - q1.z * q2.z;
	return(q);
}
gdaQuat gda_quat_div(gdaQuat q1, gdaQuat q2){ gdaQuat iq2 = gda_quat_inverse(q2); return(gda_quat_mul(q1, iq2)); }

gdaQuat gda_quat_mulf(gdaQuat q, float s){ gdaQuat r; r.xyzw = gda_vec4_mul(q.xyzw, s); return(r); }
gdaQuat gda_quat_divf(gdaQuat q, float s){ gdaQuat r; r.xyzw = gda_vec4_div(q.xyzw, s); return(r); }

float gda_quat_dot(gdaQuat q1, gdaQuat q2){ float r = gda_vec4_dot(q1.xyzw, q2.xyzw); return r; }
float gda_quat_mag(gdaQuat q){ return gda_sqrt(gda_quat_dot(q, q)); }

gdaQuat gda_quat_norm(gdaQuat q){ return(gda_quat_divf(q, gda_quat_mag(q))); }
gdaQuat gda_quat_conj(gdaQuat q){ gdaQuat r; r.xyz = gda_vec3(-q.x, -q.y, -q.z); r.w = q.w; return(r); }
gdaQuat gda_quat_inverse(gdaQuat q){ gdaQuat r = gda_quat_conj(q); r = gda_quat_divf(r, gda_quat_dot(q, q)); return(r); }

gdaVec3 gda_quat_axis(gdaQuat q){
	gdaQuat n = gda_quat_norm(q);
	gdaVec3 r = gda_vec3_div(n.xyz, gda_sin(gda_arccos(q.w)));
	return(r);
}
float gda_quat_angle(gdaQuat q){
	float mag = gda_quat_mag(q);
	float c = q.w * (1.0f / mag);
	float angle = 2.0f * gda_arccos(c);
	return(angle);
}

gdaVec3 gda_quat_rotate_vec3(gdaQuat q, gdaVec3 v){
	gdaVec3 r, t, p;
	t = gda_vec3_cross(q.xyz, v);
	t = gda_vec3_mul(t, 2.0f);
	p = gda_vec3_cross(q.xyz, t);
	return (gda_vec3_add(gda_vec3_add(gda_vec3_mul(t, q.w), v), p));
}

void gda_mat4_from_quat(gdaMat4* out, gdaQuat q){
	*(out->data + 1)  = 1.0 - 2 * q.y * q.y - 2 * q.z * q.z;
	*(out->data + 2)  = 2 * q.x * q.y + 2 * q.z * q.w;
	*(out->data + 3)  = 2 * q.x * q.z - 2 * q.y * q.w;
	*(out->data + 4)  = 0.0f;
					    
	*(out->data + 4)  = 2 * q.x * q.y - 2 * q.z * q.w;
	*(out->data + 5)  = 1.0 - 2 * q.x * q.x - 2 * q.z * q.z;
	*(out->data + 6)  = 2 * q.y * q.z + 2 * q.x * q.w;
	*(out->data + 7)  = 0.0f;
					    
	*(out->data + 8)  = 2 * q.x * q.z + 2 * q.y * q.w;
	*(out->data + 9)  = 2 * q.y * q.z - 2 * q.x * q.w;
	*(out->data + 10) = 1.0 - 2 * q.x * q.x - 2 * q.y * q.y;
	*(out->data + 11) = 0.0f;

	*(out->data + 12) = 0.0f;
	*(out->data + 13) = 0.0f;
	*(out->data + 14) = 0.0f;
	*(out->data + 15) = 1.0f;
}

gdaQuat gda_quat_from_mat4(gdaMat4* m){
	gdaQuat r;

	gdaFloat4 *fm;
	float four_x_squared_minus_1, four_y_squared_minus_1,
		four_z_squared_minus_1, four_w_squared_minus_1,
		four_biggest_squared_minus_1;

	int biggest_index = 0;
	float biggest_value, mult;

	fm = gda_float44_m(m);

	four_x_squared_minus_1 = fm[0][0] - fm[1][1] - fm[2][2];
	four_y_squared_minus_1 = fm[1][1] - fm[0][0] - fm[2][2];
	four_z_squared_minus_1 = fm[2][2] - fm[0][0] - fm[1][1];
	four_w_squared_minus_1 = fm[0][0] + fm[1][1] + fm[2][2];

	four_biggest_squared_minus_1 = four_w_squared_minus_1;
	if (four_x_squared_minus_1 > four_biggest_squared_minus_1){
		four_biggest_squared_minus_1 = four_x_squared_minus_1;
		biggest_index = 1;
	}
	if (four_y_squared_minus_1 > four_biggest_squared_minus_1){
		four_biggest_squared_minus_1 = four_y_squared_minus_1;
		biggest_index = 2;
	}
	if (four_z_squared_minus_1 > four_biggest_squared_minus_1){
		four_biggest_squared_minus_1 = four_z_squared_minus_1;
		biggest_index = 3;
	}

	biggest_value = gda_sqrt(four_biggest_squared_minus_1 + 1.0f) * 0.5f;
	mult = 0.25f / biggest_value;

	switch (biggest_index) {
		case 0:{
			r.w = biggest_value;
			r.x = (fm[1][2] - fm[2][1]) * mult;
			r.y = (fm[2][0] - fm[0][2]) * mult;
			r.z = (fm[0][1] - fm[1][0]) * mult;
		}break;
		case 1:{
			r.w = (fm[1][2] - fm[2][1]) * mult;
			r.x = biggest_value;
			r.y = (fm[0][1] + fm[1][0]) * mult;
			r.z = (fm[2][0] + fm[0][2]) * mult;
		}break;
		case 2:{
			r.w = (fm[2][0] - fm[0][2]) * mult;
			r.x = (fm[0][1] + fm[1][0]) * mult;
			r.y = biggest_value;
			r.z = (fm[1][2] + fm[2][1]) * mult;
		}break;
		case 3:{
			r.w = (fm[0][1] - fm[1][0]) * mult;
			r.x = (fm[2][0] + fm[0][2]) * mult;
			r.y = (fm[1][2] + fm[2][1]) * mult;
			r.z = biggest_value;
		}	break;
		default:{

		}break;
	}
	return(r);
}

/*
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

*/

#endif /*GDA_MATH_IMPLEMENTATION*/



