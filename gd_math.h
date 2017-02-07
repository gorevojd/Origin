/*
LICENSE
	This software is dual-licensed to the public domain and under the following
	license: you are granted a perpetual, irrevocable license to copy, modify,
	publish, and distribute this file as you see fit.

CREDITS
	Written by Gorevoy Dmitry
*/


#ifndef GD_MATH_H
#define GD_MATH_H

#ifdef GD_MATH_ALLOW_SIMD
#include <xmmintrin.h>
#endif

#ifdef IMPLEMENT_MATH_OURSELVES
#include <intrin.h>
#else
#include <math.h>
#endif

#ifndef GD_MATH_DEF
#ifdef GD_MATH_STATIC
#define GD_MATH_DEF static
#else
#define GD_MATH_DEF extern
#endif
#endif

#ifdef _MSC_VER
#pragma warning(push)
//#pragma warning(dasable:4201)
#endif

typedef union gdVec2{
    struct{
        float x, y;
    };
    float data[2];
}gdVec2;

typedef union gdVec3{
    struct{
        float x, y, z;
    };
    struct{
        float r, g, b;
    };
    gdVec2 xy;
    float data[3];
} gdVec3;

typedef union gdVec4{
    struct{ float x, y, z, w;};
    struct{ gdVec3 xyz; float w; };
    struct{
        union{
            gdVec3 rgb;
            struct{
                float r, g, b;
            };
        };
        float a; 
    };
    struct{ gdVec2 xy; gdVec2 zw; };
    float data[4];
} gdVec4;

typedef union gdMat2{
    struct{ gdVec2 x, y; };
    gdVec2 row[2];
    float data[4];
} gdMat2;

typedef union gdMat3{
    struct{ gdVec3 x, y, z; };
    gdVec3 row[3];
    float data[9];
} gdMat3;

typedef union gdMat4{
    struct{ gdVec4 x, y, z, w; };
    gdVec4 row[4];
    float data[16];
} gdMat4;

typedef union gdQuat{
    struct {
        float x, y, z, w;
    };
    gdVec3 xyz;
    gdVec4 xyzw;
} gdQuat;

typedef struct gdRect2{
    gdVec2 Pos;
    gdVec2 Dimension;
} gdRect2;

typedef struct gdRect3{
    gdVec3 Pos;
    gdVec3 Dimension;
} gdRect3;

typedef struct gdRect2i{
    int MinX;
    int MaxX;
    int MinY;
    int MaxY;
} gdRect2i;

typedef float gdFloat2[2];
typedef float gdFloat3[3];
typedef float gdFloat4[4];

typedef int gdm_bool32;

#ifdef _MSC_VER
#pragma warning(pop)
#endif

#ifndef GD_MATH_U8_MIN
#define GD_MATH_U8_MIN 0u;
#define GD_MATH_U8_MAX 0xFFu
#define GD_MATH_I8_MIN (-0x7f - 1)
#define GD_MATH_I8_MAX 0x7f

#define GD_MATH_U16_MIN 0u
#define GD_MATH_U16_MAX 0xffffu
#define GD_MATH_I16_MIN (-0x7fff - 1)
#define GD_MATH_I16_MAX 0x7fff

#define GD_MATH_U32_MIN 0u
#define GD_MATH_U32_MAX 0xffffffffu
#define GD_MATH_I32_MIN (-0x7fffffff - 1)
#define GD_MATH_I32_MAX 0x7fffffff

#define GD_MATH_U64_MIN 0ull
#define GD_MATH_U64_MAX 0xffffffffffffffffull
#define GD_MATH_I64_MIN (-0x7fffffffffffffffll - 1)
#define GD_MATH_I64_MAX 0x7fffffffffffffffll
#endif

#ifndef GD_MATH_CONSTANTS
#define GD_MATH_EPSILON 1.19209290e-7f
#define GD_MATH_ZERO 0.0f
#define GD_MATH_ONE 1.0f
#define GD_MATH_TWO_THIRDS 0.666666666666666666666666666666666666667f

#define GD_MATH_TAU 6.28318530717958647692528676655900576f
#define GD_MATH_PI 3.14159265358979323846264338327950288f
#define GD_MATH_ONE_OVER_TAU 0.636619772367581343075535053490057448f
#define GD_MATH_ONE_OVER_PI 0.318309886183790671537767526745028724f

#define GD_MATH_TAU_OVER_2 3.14159265358979323846264338327950288f
#define GD_MATH_TAU_OVER_4 1.570796326794896619231321691639751442f
#define GD_MATH_TAU_OVER_8 0.785398163397448309615660845819875721f

#define GD_MATH_E 2.71828182845904523536f
#define GD_MATH_SQRT_TWO 1.41421356237309504880168872420969808f
#define GD_MATH_SQRT_THREE 1.73205080756887729352744634150587236f
#define GD_MATH_SQRT_FIVE 2.23606797749978969640917366873127623f

#define GD_MATH_LOG_TWO 0.693147180559945309417232121458176568f
#define GD_MATH_LOG_TEN 2.30258509299404568401799145468436421f

#define GD_DEG_TO_RAD 0.0174533
#define GD_RAD_TO_DEG 57.2958

#define GD_MATH_CONSTANTS
#endif

#ifdef __cplusplus
extern "C" {
#endif

    /*Math functions definitions*/
    GD_MATH_DEF float gd_to_radians(float degrees);
    GD_MATH_DEF float gd_to_degrees(float radians);
    
    GD_MATH_DEF float gd_sqrt(float a);
    GD_MATH_DEF float gd_rsqrt(float a);

    GD_MATH_DEF float gd_sin(float a);
    GD_MATH_DEF float gd_cos(float a);
    GD_MATH_DEF float gd_tan(float a);
    GD_MATH_DEF float gd_arcsin(float a);
    GD_MATH_DEF float gd_arccos(float a);
    GD_MATH_DEF float gd_arctan(float a);
    GD_MATH_DEF float gd_arctan2(float y, float x);

    GD_MATH_DEF float gd_exp(float x);
    GD_MATH_DEF float gd_exp2(float x);
    GD_MATH_DEF float gd_log(float x);
    GD_MATH_DEF float gd_log2(float x);
    GD_MATH_DEF float gd_pow(float x, float y); /*x ^ y*/

    /*Vector definitions*/
    GD_MATH_DEF gdVec2 gd_vec2_zero(void);
    GD_MATH_DEF gdVec2 gd_vec2(float x, float y);
    GD_MATH_DEF gdVec2 gd_vec2_from_array(float arr[2]);

    GD_MATH_DEF gdVec3 gd_vec3_zero(void);
    GD_MATH_DEF gdVec3 gd_vec3(float x, float y, float z);
    GD_MATH_DEF gdVec3 gd_vec3_from_array(float arr[3]);
    GD_MATH_DEF gdVec3 gd_vec3_from_vec2(gdVec2 v, float z);

    GD_MATH_DEF gdVec4 gd_vec4_zero(void);
    GD_MATH_DEF gdVec4 gd_vec4(float x, float y, float z, float w);
    GD_MATH_DEF gdVec4 gd_vec4_from_array(float arr[4]);

    GD_MATH_DEF gdVec2 gd_vec2_add(gdVec2 v0, gdVec2 v1);
    GD_MATH_DEF gdVec2 gd_vec2_sub(gdVec2 v0, gdVec2 v1);
    GD_MATH_DEF gdVec2 gd_vec2_mul(gdVec2 v, float s);
    GD_MATH_DEF gdVec2 gd_vec2_div(gdVec2 v, float s);

    GD_MATH_DEF gdVec3 gd_vec3_add(gdVec3 v0, gdVec3 v1);
    GD_MATH_DEF gdVec3 gd_vec3_sub(gdVec3 v0, gdVec3 v1);
    GD_MATH_DEF gdVec3 gd_vec3_mul(gdVec3 v, float s);
    GD_MATH_DEF gdVec3 gd_vec3_div(gdVec3 v, float s);

    GD_MATH_DEF gdVec4 gd_vec4_add(gdVec4 v0, gdVec4 v1);
    GD_MATH_DEF gdVec4 gd_vec4_sub(gdVec4 v0, gdVec4 v1);
    GD_MATH_DEF gdVec4 gd_vec4_mul(gdVec4 v, float s);
    GD_MATH_DEF gdVec4 gd_vec4_div(gdVec4 v, float s);

    GD_MATH_DEF float gd_vec2_dot(gdVec2 v0, gdVec2 v1);
    GD_MATH_DEF float gd_vec3_dot(gdVec3 v0, gdVec3 v1);
    GD_MATH_DEF float gd_vec4_dot(gdVec4 v0, gdVec4 v1);

    GD_MATH_DEF float gd_vec2_cross(gdVec2 v0, gdVec2 v1);
    GD_MATH_DEF gdVec3 gd_vec3_cross(gdVec3 v0, gdVec3 v1);

	GD_MATH_DEF gdVec2 gd_vec2_hadamard(gdVec2 v0, gdVec2 v1);
	GD_MATH_DEF gdVec3 gd_vec3_hadamard(gdVec3 v0, gdVec3 v1);
	GD_MATH_DEF gdVec4 gd_vec4_hadamard(gdVec4 v0, gdVec4 v1);

    GD_MATH_DEF float gd_vec2_mag(gdVec2 v);
    GD_MATH_DEF float gd_vec3_mag(gdVec3 v);
    GD_MATH_DEF float gd_vec4_mag(gdVec4 v);

    GD_MATH_DEF float gd_vec2_squared_mag(gdVec2 v);
    GD_MATH_DEF float gd_vec3_squared_mag(gdVec3 v);
    GD_MATH_DEF float gd_vec4_squared_mag(gdVec4 v);

    GD_MATH_DEF gdVec2 gd_vec2_norm(gdVec2 v);
    GD_MATH_DEF gdVec3 gd_vec3_norm(gdVec3 v);
    GD_MATH_DEF gdVec4 gd_vec4_norm(gdVec4 v);

    GD_MATH_DEF gdVec2 gd_vec2_norm0(gdVec2 v);
    GD_MATH_DEF gdVec3 gd_vec3_norm0(gdVec3 v);
    GD_MATH_DEF gdVec4 gd_vec4_norm0(gdVec4 v);

    GD_MATH_DEF gdVec2 gd_vec2_reflect(gdVec2 i, gdVec2 n);
    GD_MATH_DEF gdVec3 gd_vec3_reflect(gdVec3 i, gdVec3 n);
    GD_MATH_DEF gdVec2 gd_vec2_refract(gdVec2 i, gdVec2 n, float theta);
    GD_MATH_DEF gdVec3 gd_vec3_refract(gdVec3 i, gdVec3 n, float theta);

    GD_MATH_DEF float gd_vec2_aspect_ratio(gdVec2 v);

    /*Matrix definitions*/
    GD_MATH_DEF void gd_mat2_identity(gdMat2* m);
    GD_MATH_DEF void gd_mat2_transpose(gdMat2* m);
    GD_MATH_DEF void gd_mat2_mul(gdMat2* out, gdMat2* m1, gdMat2* m2);
    GD_MATH_DEF gdVec2 gd_mat2_mul_vec2(gdMat2* m, gdVec2 v);
    GD_MATH_DEF void gd_mat2_inverse(gdMat2* out, gdMat2* m);
    GD_MATH_DEF float gd_mat2_determinant(gdMat2* m);

    GD_MATH_DEF void gd_mat3_identity(gdMat3* m);
    GD_MATH_DEF void gd_mat3_transpose(gdMat3* m);
    GD_MATH_DEF void gd_mat3_mul(gdMat3* out, gdMat3* m1, gdMat3* m2);
    GD_MATH_DEF gdVec3 gd_mat3_mul_vec3(gdMat3* m, gdVec3 v);
    GD_MATH_DEF void gd_mat3_inverse(gdMat3* out, gdMat3* m);
    GD_MATH_DEF float gd_mat3_determinant(gdMat3* m);

    GD_MATH_DEF void gd_mat4_identity(gdMat4* m);
    GD_MATH_DEF void gd_mat4_transpose(gdMat4* m);
    GD_MATH_DEF void gd_mat4_mul(gdMat4* out, gdMat4* m1, gdMat4* m2);
    GD_MATH_DEF gdVec4 gd_mat4_mul_vec4(gdMat4* m, gdVec4 v);
    GD_MATH_DEF void gd_mat4_inverse(gdMat4* out, gdMat4* in);

    GD_MATH_DEF void gd_mat4_translate(gdMat4* out, gdVec3 v);
    GD_MATH_DEF void gd_mat4_rotate(gdMat4* out, gdVec3 v, float angle_radians);
    GD_MATH_DEF void gd_mat4_scale(gdMat4* out, gdVec3 v);
    GD_MATH_DEF void gd_mat4_scalef(gdMat4* out, float s);
    GD_MATH_DEF void gd_mat4_ortho2d(gdMat4* out, float left, float right, float bottom, float top);
    GD_MATH_DEF void gd_mat4_ortho3d(gdMat4* out, float left, float right, float bottom, float top, float z_near, float z_far);
    GD_MATH_DEF void gd_mat4_perspective(gdMat4* out, float fovy, float aspect, float z_near, float z_far);
    GD_MATH_DEF void gd_mat4_infinite_perspective(gdMat4* out, float fovy, float aspect, float z_near);

    GD_MATH_DEF void gd_mat4_look_at(gdMat4* out, gdVec3 eye, gdVec3 centre, gdVec3 up);

    GD_MATH_DEF void gd_float22_identity(float m[2][2]);
    GD_MATH_DEF gdFloat2* gd_float22_m(gdMat2* m);
    GD_MATH_DEF gdFloat2* gd_float22_v(gdVec2 m[2]);
    GD_MATH_DEF gdFloat2* gd_float22_4(float m[4]);
    GD_MATH_DEF void gd_float22_transpose(float(*vec)[2]);
    GD_MATH_DEF void gd_float22_mul(float(*out)[2], float(*mat1)[2], float(*mat2)[2]);
    GD_MATH_DEF gdVec2 gd_float22_mul_vec2(float m[2][2], gdVec2 v1);

    GD_MATH_DEF void gd_float33_identity(float m[3][3]);
    GD_MATH_DEF gdFloat3* gd_float33_m(gdMat3* m);
    GD_MATH_DEF gdFloat3* gd_float33_v(gdVec3 m[3]);
    GD_MATH_DEF gdFloat3* gd_float33_9(float m[9]);
    GD_MATH_DEF void gd_float33_transpose(float(*vec)[3]);
    GD_MATH_DEF void gd_float33_mul(float(*out)[3], float(*mat1)[3], float(*mat2)[3]);
    GD_MATH_DEF gdVec3 gd_float33_mul_vec3(float m[3][3], gdVec3 v1);

    GD_MATH_DEF void gd_float44_identity(float m[4][4]);
    GD_MATH_DEF gdFloat4* gd_float44_m(gdMat4* m);
    GD_MATH_DEF gdFloat4* gd_float44_v(gdVec4 m[4]);
    GD_MATH_DEF gdFloat4* gd_float44_16(float m[16]);
    GD_MATH_DEF void gd_float44_transpose(float(*vec)[4]);
    GD_MATH_DEF void gd_float44_mul(float(*out)[4], float(*mat1)[4], float(*mat2)[4]);
    GD_MATH_DEF gdVec4 gd_float44_mul_vec4(float m[4][4], gdVec4 v1);

    /*Quaternions definitions*/
    GD_MATH_DEF gdQuat gd_quat(float x, float y, float z, float w);
    GD_MATH_DEF gdQuat gd_quat_from_array(float arr[4]);
    GD_MATH_DEF gdQuat gd_quat_from_angle(gdVec3 axis, float angle_radians);
    GD_MATH_DEF gdQuat gd_quat_from_euler_angles(float pitch, float yaw, float roll);
    GD_MATH_DEF gdQuat gd_quat_identity(void);

    GD_MATH_DEF gdQuat gd_quat_add(gdQuat q1, gdQuat q2);
    GD_MATH_DEF gdQuat gd_quat_sub(gdQuat q1, gdQuat q2);
    GD_MATH_DEF gdQuat gd_quat_mul(gdQuat q1, gdQuat q2);
    GD_MATH_DEF gdQuat gd_quat_div(gdQuat q1, gdQuat q2);

    GD_MATH_DEF gdQuat gd_quat_mulf(gdQuat q, float s);
    GD_MATH_DEF gdQuat gd_quat_divf(gdQuat q, float s);

    GD_MATH_DEF float gd_quat_dot(gdQuat q1, gdQuat q2);
    GD_MATH_DEF float gd_quat_mag(gdQuat q);

    GD_MATH_DEF gdQuat gd_quat_norm(gdQuat q);
    GD_MATH_DEF gdQuat gd_quat_conj(gdQuat q);
    GD_MATH_DEF gdQuat gd_quat_inverse(gdQuat q);

    GD_MATH_DEF gdVec3 gd_quat_axis(gdQuat q);
    GD_MATH_DEF float gd_quat_angle(gdQuat q);

    GD_MATH_DEF float gd_quat_roll(gdQuat q);
    GD_MATH_DEF float gd_quat_pitch(gdQuat q);
    GD_MATH_DEF float gd_quat_yaw(gdQuat q);

    GD_MATH_DEF gdVec3 gd_quat_rotate_vec3(gdQuat q, gdVec3 v);
    GD_MATH_DEF void gd_mat4_from_quat(gdMat4* out, gdQuat q);
    GD_MATH_DEF gdQuat gd_quat_from_mat4(gdMat4* m);

    /*Interpolations definitions*/
    GD_MATH_DEF float gd_lerp(float a, float b, float t);
    GD_MATH_DEF float gd_unlerp(float a, float b, float t);
    GD_MATH_DEF float gd_smooth_step(float a, float b, float t);
    GD_MATH_DEF float gd_smoother_step(float a, float b, float t);

    GD_MATH_DEF gdVec2 gd_vec2_lerp(gdVec2 a, gdVec2 b, float t);
    GD_MATH_DEF gdVec3 gd_vec3_lerp(gdVec3 a, gdVec3 b, float t);
    GD_MATH_DEF gdVec4 gd_vec4_lerp(gdVec4 a, gdVec4 b, float t);

    GD_MATH_DEF gdQuat gd_quat_lerp(gdQuat a, gdQuat b, float t);
    GD_MATH_DEF gdQuat gd_quat_nlerp(gdQuat a, gdQuat b, float t);
    GD_MATH_DEF gdQuat gd_quat_slerp(gdQuat a, gdQuat b, float t);
    GD_MATH_DEF gdQuat gd_quat_nquad(gdQuat p, gdQuat a, gdQuat b, gdQuat q, float t);
    GD_MATH_DEF gdQuat gd_quat_squad(gdQuat p, gdQuat a, gdQuat b, gdQuat q, float t);
    GD_MATH_DEF gdQuat gd_quat_slerp_approx(gdQuat a, gdQuat b, float t);
    GD_MATH_DEF gdQuat gd_quat_squad_approx(gdQuat p, gdQuat a, gdQuat b, gdQuat q, float t);

    /*Rectangle definitions*/
    GD_MATH_DEF gdRect2 gd_rect2(gdVec2 Pos, gdVec2 Dim);
    GD_MATH_DEF gdRect3 gd_rect3(gdVec3 Pos, gdVec3 Dim);
    GD_MATH_DEF gdRect2i gd_rect2_i(int MinX, int MinY, int MaxX, int MaxY);

    GD_MATH_DEF gdm_bool32 gd_rect2_contains(gdRect2 r, gdVec2 v);
    GD_MATH_DEF gdm_bool32 gd_rect2_has_intersection(gdRect2 r1, gdRect2 r2);
    GD_MATH_DEF gdRect2 gd_rect2_get_intersection(gdRect2 r1, gdRect2 r2);
    GD_MATH_DEF gdRect2i gd_rect2_i_get_intersection(gdRect2i r1, gdRect2i r2);
    GD_MATH_DEF gdRect2i gd_rect2_i_inverse_infinite();
#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
/*gdVec2 operator overloading*/
inline bool operator==(gdVec2 a, gdVec2 b) { return((a.x == b.x) && (a.y == b.y)); }
inline bool operator!=(gdVec2 a, gdVec2 b){ return((a.x != b.x) || (a.y != b.y)); }

inline gdVec2 operator+(gdVec2 a){ return(a); }
inline gdVec2 operator-(gdVec2 a){ gdVec2 r = { -a.x, -a.y }; return(r); }

inline gdVec2 operator+(gdVec2 a, gdVec2 b){ return gd_vec2_add(a, b); }
inline gdVec2 operator-(gdVec2 a, gdVec2 b){ return gd_vec2_sub(a, b); }

inline gdVec2 operator*(gdVec2 a, float s){ return gd_vec2_mul(a, s); }
inline gdVec2 operator*(float s, gdVec2 a){ return gd_vec2_mul(a, s); }
inline gdVec2 operator/(gdVec2 a, float s){ return gd_vec2_div(a, s); }

inline gdVec2 operator*(gdVec2 a, gdVec2 b){gdVec2 r = {a.x * b.x, a.y * b.y}; return(r);}
inline gdVec2 operator/(gdVec2 a, gdVec2 b){gdVec2 r = {a.x / b.x, a.y / b.y}; return(r);}

inline gdVec2 &operator+=(gdVec2& a, gdVec2 b){return(a = a + b);}
inline gdVec2 &operator-=(gdVec2& a, gdVec2 b){return(a = a - b);}
inline gdVec2 &operator*=(gdVec2& a, float s){return(a = a * s);}
inline gdVec2 &operator/=(gdVec2& a, float s){return(a = a / s);}

/*gdVec3 operator overloading*/
inline bool operator==(gdVec3 a, gdVec3 b){return((a.x == b.x) && (a.y == b.y) && (a.z == b.z));}
inline bool operator!=(gdVec3 a, gdVec3 b){return((a.x != b.x) || (a.y != b.y) || (a.z != b.z));}

inline gdVec3 operator+(gdVec3 a){return(a);}
inline gdVec3 operator-(gdVec3 a){gdVec3 r = {-a.x, -a.y, -a.z}; return(r);}

inline gdVec3 operator+(gdVec3 a, gdVec3 b){ return gd_vec3_add(a, b); }
inline gdVec3 operator-(gdVec3 a, gdVec3 b){ return gd_vec3_sub(a, b); }

inline gdVec3 operator*(gdVec3 a, float s){ return gd_vec3_mul(a, s); }
inline gdVec3 operator*(float s, gdVec3 a){ return gd_vec3_mul(a, s); }
inline gdVec3 operator/(gdVec3 a, float s){ return gd_vec3_div(a, s); }

inline gdVec3 operator*(gdVec3 a, gdVec3 b){gdVec3 r = {a.x * b.x, a.y * b.y, a.z * b.z}; return(r);}
inline gdVec3 operator/(gdVec3 a, gdVec3 b){gdVec3 r = {a.x / b.x, a.y / b.y, a.z / b.z}; return(r);}

inline gdVec3 &operator+=(gdVec3& a, gdVec3 b){return(a = a + b);}
inline gdVec3 &operator-=(gdVec3& a, gdVec3 b){return(a = a - b);}
inline gdVec3 &operator*=(gdVec3& a, float s){return(a = a * s);}
inline gdVec3 &operator/=(gdVec3& a, float s){return(a = a / s);}

/*gdVec4 operator overloading*/
inline bool operator==(gdVec4 a, gdVec4 b) { return((a.x == b.x) && (a.y == b.y) && (a.z == b.z) && (a.w == b.w)); }
inline bool operator!=(gdVec4 a, gdVec4 b){ return((a.x != b.x) || (a.y != b.y) || (a.z != b.z) || (a.w != b.w)); }

inline gdVec4 operator+(gdVec4 a){ return(a); }
inline gdVec4 operator-(gdVec4 a){ gdVec4 r = { -a.x, -a.y }; return(r); }

inline gdVec4 operator+(gdVec4 a, gdVec4 b){ return gd_vec4_add(a, b); }
inline gdVec4 operator-(gdVec4 a, gdVec4 b){ return gd_vec4_sub(a, b); }

inline gdVec4 operator*(gdVec4 a, float s){ return gd_vec4_mul(a, s); }
inline gdVec4 operator*(float s, gdVec4 a){ return gd_vec4_mul(a, s); }
inline gdVec4 operator/(gdVec4 a, float s){ return gd_vec4_div(a, s); }

inline gdVec4 operator*(gdVec4 a, gdVec4 b){gdVec4 r = {a.x * b.x, a.y * b.y, a.z * b.z, a.w * b.w}; return(r);}
inline gdVec4 operator/(gdVec4 a, gdVec4 b){gdVec4 r = {a.x / b.x, a.y / b.y, a.z / b.z, a.w / b.w}; return(r);}

inline gdVec4 &operator+=(gdVec4& a, gdVec4 b){return(a = a + b);}
inline gdVec4 &operator-=(gdVec4& a, gdVec4 b){return(a = a - b);}
inline gdVec4 &operator*=(gdVec4& a, float s){return(a = a * s);}
inline gdVec4 &operator/=(gdVec4& a, float s){return(a = a / s);}

/*gdQuat operator overloading*/
inline bool operator==(gdQuat a, gdQuat b){ return(a.xyzw == b.xyzw); };
inline bool operator!=(gdQuat a, gdQuat b){ return(!operator==(a, b)); }

inline gdQuat operator+(gdQuat q){ return(q); };
inline gdQuat operator-(gdQuat q){ return(gd_quat(-q.x, -q.y, -q.z, -q.w)); }

inline gdQuat operator+(gdQuat a, gdQuat b){ return(gd_quat_add(a, b)); }
inline gdQuat operator-(gdQuat a, gdQuat b){ return(gd_quat_sub(a, b)); }

inline gdQuat operator*(gdQuat a, gdQuat b){ return(gd_quat_mul(a, b)); }
inline gdQuat operator*(gdQuat q, float s){ return(gd_quat_mulf(q, s)); }
inline gdQuat operator*(float s, gdQuat q){ return(gd_quat_mulf(q, s)); }
inline gdQuat operator/(gdQuat q, float s){ return(gd_quat_divf(q, s)); }

inline gdQuat &operator+=(gdQuat& a, gdQuat b){ return(a = a + b); }
inline gdQuat &operator-=(gdQuat& a, gdQuat b){ return(a = a - b); }
inline gdQuat &operator*=(gdQuat& a, gdQuat b){ return(a = a * b); }
inline gdQuat &operator/=(gdQuat& a, gdQuat b){ return(a = gd_quat_div(a, b)); }

inline gdQuat &operator*=(gdQuat& q, float s){ return(q = q * s); }
inline gdQuat &operator/=(gdQuat& q, float s){ return(q = q / s); }

#endif

#endif /*GD_MATH_H*/

#if defined(GD_MATH_IMPLEMENTATION) && !defined(GD_MATH_IMPLEMENTATION_DONE)
#define GD_MATH_IMPLEMENTATION_DONE

static void gd__memcpy_4byte(void *dest, void const *src, size_t size){
    size_t i;
    unsigned int *d, *s;
    d = (unsigned int *)dest;
    s = (unsigned int *)src;
    for (int i = 0; i < size / 4; i++){
        *d++ = *s++;
    }
}

float gd_to_radians(float degrees){ return(degrees * GD_DEG_TO_RAD); }
float gd_to_degrees(float radians){ return(radians * GD_RAD_TO_DEG); }

#ifdef IMPLEMENT_MATH_OURSELVES
#ifdef _MSC_VER

float gd_sqrt(float a){ return(_mm_cvtss_f32(_mm_rsqrt_ss(_mm_set_ss(a)))); }
float gd_rsqrt(float a){ return(_mm_cvtss_f32(_mm_sqrt_ss(_mm_set_ss(a)))); }

float gd_sin(float a){
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
float gd_cos(float a){
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

float gd_tan(float a){
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

float gd_arcsin(float a){ return(gd_arctan2(a, gd_sqrt((1.0f + a) * (1.0f - a)))); }
float gd_arccos(float a){ return(gd_arctan2(gd_sqrt((1.0f + a) * (1.0f - a)), a)); }
float gd_arctan(float a){
    float u = a * a;
    float u2 = u * u;
    float u3 = u2 * u;
    float u4 = u3 * u;
    float f = 1.0f + 0.33288950512027f*u - 0.08467922817644f*u2 + 0.03252232640125f*u3 - 0.00749305860992f*u4;
    return a / f;
}
float gd_arctan2(float y, float x){
    if (gd_abs(x) > gd_abs(y)) {
        float a = gd_arctan(y / x);
        if (x > 0.0f)
            return a;
        else
            return y > 0.0f ? a+GD_MATH_TAU_OVER_2 : a-GD_MATH_TAU_OVER_2;
    } else {
        float a = gd_arctan(x/y);
        if (x > 0.0f)
            return y > 0.0f ? GD_MATH_TAU_OVER_4-a : -GD_MATH_TAU_OVER_4-a;
        else
            return y > 0.0f ? GD_MATH_TAU_OVER_4+a : -GD_MATH_TAU_OVER_4+a;
    }
}

float gd_exp(float x){
    union { float f; int i; } u, v;
    u.i = (int)(6051102 * x + 1056478197);
    v.i = (int)(1056478197 - 6051102 * x);
    return u.f / v.f;
}

float gd_log(float x){
    union { float f; int i; } u = { x };
    return (u.i - 1064866805) * 8.262958405176314e-8f;
}

float gd_pow(float x, float y){
    int flipped = 0, e;
    float f, r = 1.0f;
    if (y < 0) {
        flipped = 1;
        y = -y;
    }

    e = (int)y;
    f = gd_exp(y - e);

    while (e) {
        if (e & 1) r *= x;
        x *= x;
        e >>= 1;
    }

    r *= f;
    return flipped ? 1.0f / r : r;
}


#else

float gd_sqrt(float a){ return __builtin_sqrt(a); }
float gd_rsqrt(float a){ return 1.0f / __builtin_sqrt(a); }

float gd_sin(float radians){ return __builtin_sinf(radians); }
float gd_cos(float radians){ return __builtin_cosf(radians); }
float gd_tan(float radians){ return __builtin_tanf(radians); }
float gd_arcsin(float a){ return __builtin_asinf(a); }
float gd_arccos(float a){ return __builtin_acosf(a); }
float gd_arctan(float a){ return __builtin_atanf(a); }
float gd_arctan2(float y, float x){ return __builtin_atan2f(y, x); }

float gd_exp(float x){ return __builtin_expf(x);}
float gd_log(float x){ return __builtin_logf(x);}
float gd_pow(float x, float y){ return __builtin_powf(x, y); }

#endif
#else

float gd_sqrt(float a){ return sqrtf(a); }
float gd_rsqrt(float a){ return 1.0f / sqrtf(a); }

float gd_sin(float radians){return(sinf(radians));}
float gd_cos(float radians){return(cosf(radians));}
float gd_tan(float radians){return(tanf(radians));}
float gd_arcsin(float a){return(asinf(a));}
float gd_arccos(float a){return(acosf(a));}
float gd_arctan(float a){return(atanf(a));}
float gd_arctan2(float y, float x){ return(atan2f(y, x)); }

float gd_exp(float x){ return(expf(x)); }
float gd_log(float x){ return(logf(x)); }
float gd_pow(float x, float y){ return(powf(x, y)); }

#endif

//float gd_exp2(float x);
//float gd_log2(float x);

#define GD_VEC2_WTF1_OP(a, post) \
    gdVec2 Result;  \
    Result.x = (a).x post;  \
    Result.y = (a).y post;  \
    return(Result);

#define GD_VEC2_WTF2_OP(a, b, op) \
    gdVec2 Result; \
    Result.x = (a).x op (b.x); \
    Result.y = (a).y op (b.y); \
    return(Result);

#define GD_VEC3_WTF1_OP(a, post) \
    gdVec3 Result; \
    Result.x = (a).x post;  \
    Result.y = (a).y post;  \
    Result.z = (a).z post;  \
    return(Result);

#define GD_VEC3_WTF2_OP(a, b, op)   \
    gdVec3 Result;  \
    Result.x = (a).x op (b).x;  \
    Result.y = (a).y op (b).y;  \
    Result.z = (a).z op (b).z;  \
    return(Result);

#define GD_VEC4_WTF1_OP(a, post)    \
    gdVec4 Result;  \
    Result.x = (a).x post;  \
    Result.y = (a).y post;  \
    Result.z = (a).z post;  \
    Result.w = (a).w post;  \
    return(Result);

#define GD_VEC4_WTF2_OP(a, b, op)   \
    gdVec4 Result;  \
    Result.x = (a).x op (b).x;  \
    Result.y = (a).y op (b).y;  \
    Result.z = (a).z op (b).z;  \
    Result.w = (a).w op (b).w;  \
    return(Result);

gdVec2 gd_vec2_zero(void){ gdVec2 v = {}; return(v); }
gdVec2 gd_vec2(float x, float y){ gdVec2 v; v.x = x; v.y = y; return(v); }
gdVec2 gd_vec2_from_array(float arr[2]){ gdVec2 v; v.x = arr[0]; v.y = arr[1]; return(v); };

gdVec3 gd_vec3_zero(void){ gdVec3 v = {}; return(v); }
gdVec3 gd_vec3(float x, float y, float z){ gdVec3 v; v.x = x; v.y = y; v.z = z; return(v); }
gdVec3 gd_vec3_from_array(float arr[3]){ gdVec3 v; v.x = arr[0]; v.y = arr[1]; v.z = arr[2]; return(v); }
gdVec3 gd_vec3_from_vec2(gdVec2 v, float z){ gdVec3 r; r.x = v.x, r.y = v.y, r.z = z; return(r); }

gdVec4 gd_vec4_zero(void){ gdVec4 v = {}; return(v); }
gdVec4 gd_vec4(float x, float y, float z, float w){ gdVec4 v; v.x = x; v.y = y; v.z = z; v.w = w; return(v); }
gdVec4 gd_vec4_from_array(float arr[4]){ gdVec4 v; v.x = arr[0]; v.y = arr[1]; v.z = arr[2]; v.w = arr[3]; return(v); }

gdVec2 gd_vec2_add(gdVec2 v0, gdVec2 v1){GD_VEC2_WTF2_OP(v0, v1, +);}
gdVec2 gd_vec2_sub(gdVec2 v0, gdVec2 v1){GD_VEC2_WTF2_OP(v0, v1, -);}
gdVec2 gd_vec2_mul(gdVec2 v, float s){GD_VEC2_WTF1_OP(v, * s);}
gdVec2 gd_vec2_div(gdVec2 v, float s){GD_VEC2_WTF1_OP(v, / s);}

gdVec3 gd_vec3_add(gdVec3 v0, gdVec3 v1){GD_VEC3_WTF2_OP(v0, v1, +);}
gdVec3 gd_vec3_sub(gdVec3 v0, gdVec3 v1){GD_VEC3_WTF2_OP(v0, v1, -);}
gdVec3 gd_vec3_mul(gdVec3 v, float s){GD_VEC3_WTF1_OP(v, * s);}
gdVec3 gd_vec3_div(gdVec3 v, float s){GD_VEC3_WTF1_OP(v, / s);}

gdVec4 gd_vec4_add(gdVec4 v0, gdVec4 v1){GD_VEC4_WTF2_OP(v0, v1, +);}
gdVec4 gd_vec4_sub(gdVec4 v0, gdVec4 v1){GD_VEC4_WTF2_OP(v0, v1, -);}
gdVec4 gd_vec4_mul(gdVec4 v, float s){GD_VEC4_WTF1_OP(v, * s);}
gdVec4 gd_vec4_div(gdVec4 v, float s){GD_VEC4_WTF1_OP(v, / s);}

#undef GD_VEC2_WTF1_OP
#undef GD_VEC2_WTF2_OP
#undef GD_VEC3_WTF1_OP
#undef GD_VEC3_WTF2_OP
#undef GD_VEC4_WTF1_OP
#undef GD_VEC4_WTF2_OP

float gd_vec2_dot(gdVec2 v0, gdVec2 v1){ return v0.x * v1.x + v0.y * v1.y; }
float gd_vec3_dot(gdVec3 v0, gdVec3 v1){ return v0.x * v1.x + v0.y * v1.y + v0.z * v1.z; }
float gd_vec4_dot(gdVec4 v0, gdVec4 v1){ return v0.x * v1.x + v0.y * v1.y + v0.z * v1.z + v0.w * v1.w; }

float gd_vec2_cross(gdVec2 v0, gdVec2 v1){ return v0.x * v1.y - v1.x * v0.y; }
gdVec3 gd_vec3_cross(gdVec3 v0, gdVec3 v1){
    gdVec3 v;
    v.x = v0.y * v1.z - v1.y * v0.z;
    v.y = v0.z * v1.x - v1.z * v0.x;
    v.z = v0.x * v1.y - v1.x * v0.y;
    return(v);
}

gdVec2 gd_vec2_hadamard(gdVec2 v0, gdVec2 v1){ return (gd_vec2(v0.x * v1.x, v0.y * v1.y));}
gdVec3 gd_vec3_hadamard(gdVec3 v0, gdVec3 v1){ return (gd_vec3(v0.x * v1.x, v0.y * v1.y, v0.z * v1.z));}
gdVec4 gd_vec4_hadamard(gdVec4 v0, gdVec4 v1){ return (gd_vec4(v0.x * v1.x, v0.y * v1.y, v0.z * v1.z, v0.w * v1.w));}

float gd_vec2_mag(gdVec2 v){ return(gd_sqrt(gd_vec2_dot(v, v)));}
float gd_vec3_mag(gdVec3 v){ return(gd_sqrt(gd_vec3_dot(v, v)));}
float gd_vec4_mag(gdVec4 v){ return(gd_sqrt(gd_vec4_dot(v, v)));}

float gd_vec2_squared_mag(gdVec2 v){ return(gd_vec2_dot(v, v)); }
float gd_vec3_squared_mag(gdVec3 v){ return(gd_vec3_dot(v, v)); }
float gd_vec4_squared_mag(gdVec4 v){ return(gd_vec4_dot(v, v)); }

gdVec2 gd_vec2_norm(gdVec2 v){ return(gd_vec2_mul(v, gd_rsqrt(gd_vec2_dot(v, v)))); }
gdVec3 gd_vec3_norm(gdVec3 v){ return(gd_vec3_mul(v, gd_rsqrt(gd_vec3_dot(v, v)))); }
gdVec4 gd_vec4_norm(gdVec4 v){ return(gd_vec4_mul(v, gd_rsqrt(gd_vec4_dot(v, v)))); }

gdVec2 gd_vec2_norm0(gdVec2 v){ float sqmag = gd_vec2_dot(v, v); return((sqmag) == 0.0f ? gd_vec2_zero() : gd_vec2_mul(v, gd_rsqrt(sqmag))); }
gdVec3 gd_vec3_norm0(gdVec3 v){ float sqmag = gd_vec3_dot(v, v); return((sqmag) == 0.0f ? gd_vec3_zero() : gd_vec3_mul(v, gd_rsqrt(sqmag))); }
gdVec4 gd_vec4_norm0(gdVec4 v){ float sqmag = gd_vec4_dot(v, v); return((sqmag) == 0.0f ? gd_vec4_zero() : gd_vec4_mul(v, gd_rsqrt(sqmag))); }

gdVec2 gd_vec2_reflect(gdVec2 i, gdVec2 n){return(gd_vec2_sub(i, gd_vec2_mul(n, 2.0f * gd_vec2_dot(n, i))));}
gdVec3 gd_vec3_reflect(gdVec3 i, gdVec3 n){return(gd_vec3_sub(i, gd_vec3_mul(n, 2.0f * gd_vec3_dot(n, i))));}
gdVec2 gd_vec2_refract(gdVec2 i, gdVec2 n, float theta){
    gdVec2 r, a, b;
    float dv, k;
    dv = gd_vec2_dot(n, i);
    k = 1.0f - theta * theta * (1.0f - dv * dv);
    a = gd_vec2_mul(i, theta);
    b = gd_vec2_mul(n, theta * dv * gd_sqrt(k));
    r = gd_vec2_sub(a, b);
    r = gd_vec2_mul(r, (float)(k >= 0.0f));
    return(r);
}
gdVec3 gd_vec3_refract(gdVec3 i, gdVec3 n, float theta){
    gdVec3 r, a, b;
    float dv, k;
    dv = gd_vec3_dot(n, i);
    k = 1.0f - theta * theta * (1.0f - dv * dv);
    a = gd_vec3_mul(i, theta);
    b = gd_vec3_mul(n, theta * dv * gd_sqrt(k));
    r = gd_vec3_sub(a, b);
    r = gd_vec3_mul(r, (float)(k >= 0.0f));
    return(r);
}

float gd_vec2_aspect_ratio(gdVec2 v){ return ((v.y < 0.0001f) ? 0.0f : v.x / v.y); }

/*Matrices implementation*/
void gd_mat2_identity(gdMat2* m){ gd_float22_identity(gd_float22_m(m)); }
void gd_mat2_transpose(gdMat2* m){ gd_float22_transpose(gd_float22_m(m)); }
void gd_mat2_mul(gdMat2* out, gdMat2* m1, gdMat2* m2){ gd_float22_mul(gd_float22_m(out), gd_float22_m(m1), gd_float22_m(m2)); }
gdVec2 gd_mat2_mul_vec2(gdMat2* m, gdVec2 v){ return(gd_float22_mul_vec2(gd_float22_m(m), v));}

void gd_mat2_inverse(gdMat2* out, gdMat2* m){
    gdFloat2 *o = gd_float22_m(out);
    gdFloat2 *i = gd_float22_m(m);
    float ood = 1.0f / gd_mat2_determinant(m);

    o[0][0] = +i[1][1] * ood;
    o[0][1] = -i[0][1] * ood;
    o[1][0] = -i[1][0] * ood;
    o[1][1] = +i[0][0] * ood;
}

float gd_mat2_determinant(gdMat2* m){
    gdFloat2* e = gd_float22_m(m);
    return (e[0][0] * e[1][1] - e[1][0] * e[0][1]);
}

void gd_mat3_identity(gdMat3* m){ gd_float33_identity(gd_float33_m(m)); }
void gd_mat3_transpose(gdMat3* m){ gd_float33_transpose(gd_float33_m(m)); }
void gd_mat3_mul(gdMat3* out, gdMat3* m1, gdMat3* m2) { gd_float33_mul(gd_float33_m(out), gd_float33_m(m1), gd_float33_m(m2)); }
gdVec3 gd_mat3_mul_vec3(gdMat3* m, gdVec3 v){ return(gd_float33_mul_vec3(gd_float33_m(m), v)); }

void gd_mat3_inverse(gdMat3* out, gdMat3* m){
    gdFloat3 *o = gd_float33_m(out);
    gdFloat3 *i = gd_float33_m(m);

    float ood = 1.0f / gd_mat3_determinant(m);

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

float gd_mat3_determinant(gdMat3* m){
    gdFloat3 *e = gd_float33_m(m);
    return(+e[0][0] * (e[1][1] * e[2][2] - e[1][2] * e[2][1])
        - e[0][1] * (e[1][0] * e[2][2] - e[1][2] * e[2][0])
        + e[0][2] * (e[1][0] * e[2][1] - e[1][1] * e[2][0]));
}

void gd_mat4_identity(gdMat4* m){ gd_float44_identity(gd_float44_m(m)); }
void gd_mat4_transpose(gdMat4* m){ gd_float44_transpose(gd_float44_m(m)); }
void gd_mat4_mul(gdMat4* out, gdMat4* m1, gdMat4* m2){ gd_float44_mul(gd_float44_m(out), gd_float44_m(m1), gd_float44_m(m2)); }
gdVec4 gd_mat4_mul_vec4(gdMat4* m, gdVec4 v){ return(gd_float44_mul_vec4(gd_float44_m(m), v)); }

void gd_mat4_inverse(gdMat4* out, gdMat4* in){
    gdFloat4 *o = gd_float44_m(out);
    gdFloat4 *m = gd_float44_m(in);

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

void gd_mat4_translate(gdMat4* out, gdVec3 v){
    gd_mat4_identity(out);
    out->x.data[3] = v.x;
    out->y.data[3] = v.y;
    out->z.data[3] = v.y;
    out->data[15] = 1;
}

void gd_mat4_rotate(gdMat4* out, gdVec3 v, float angle_radians){
    float c, s;
    gdVec3 axis, t;
    gdFloat4* rot;

    c = gd_cos(angle_radians);
    s = gd_sin(angle_radians);

    axis = gd_vec3_norm(v);
    t = gd_vec3_mul(axis, 1.0f - c);

    gd_mat4_identity(out);
    rot = gd_float44_m(out);

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

void gd_mat4_scale(gdMat4* out, gdVec3 v){
    gd_mat4_identity(out);
    out->data[0] = v.x;
    out->data[5] = v.y;
    out->data[10] = v.z;
}

void gd_mat4_scalef(gdMat4* out, float s){
    gd_mat4_identity(out);
    out->data[0] = s;
    out->data[5] = s;
    out->data[10] = s;
}

void gd_mat4_ortho2d(gdMat4* out, float left, float right, float bottom, float top){
    gdFloat4* m;
    gd_mat4_identity(out);
    m = gd_float44_m(out);

    m[0][0] = 2.0f / (right - left);
    m[1][1] = 2.0f / (top - bottom);
    m[2][2] = -1.0f;
    m[3][0] = -(right + left) / (right - left);
    m[3][1] = -(top + bottom) / (top - bottom);
}

void gd_mat4_ortho3d(gdMat4* out, float left, float right, float bottom, float top, float z_near, float z_far){
    gdFloat4 *m;
    gd_mat4_identity(out);
    m = gd_float44_m(out);

    m[0][0] = +2.0f / (right - left);
    m[1][1] = +2.0f / (top - bottom);
    m[2][2] = -2.0f / (z_far - z_near);
    m[3][0] = -(right + left) / (right - left);
    m[3][1] = -(top + bottom) / (top - bottom);
    m[3][2] = -(z_far + z_near) / (z_far - z_near);
}

void gd_mat4_perspective(gdMat4* out, float fovy, float aspect, float z_near, float z_far){
    float tan_half_fovy = gd_tan(0.5f * fovy);
    gdMat4 zero_mat = { 0 };
    gdFloat4 *m = gd_float44_m(out);
    *out = zero_mat;

    m[0][0] = 1.0f / (aspect*tan_half_fovy);
    m[1][1] = 1.0f / (tan_half_fovy);
    m[2][2] = -(z_far + z_near) / (z_far - z_near);
    m[2][3] = -1.0f;
    m[3][2] = -2.0f*z_far*z_near / (z_far - z_near);
}

void gd_mat4_infinite_perspective(gdMat4* out, float fovy, float aspect, float z_near){
    float range = gd_tan(0.5f * fovy) * z_near;
    float left = -range * aspect;
    float right = range * aspect;
    float bottom = -range;
    float top = range;
    gdMat4 zero_mat = { 0 };
    gdFloat4 *m = gd_float44_m(out);
    *out = zero_mat;

    m[0][0] = (2.0f*z_near) / (right - left);
    m[1][1] = (2.0f*z_near) / (top - bottom);
    m[2][2] = -1.0f;
    m[2][3] = -1.0f;
    m[3][2] = -2.0f*z_near;
}

void gd_mat4_look_at(gdMat4* out, gdVec3 eye, gdVec3 centre, gdVec3 up){
    gdVec3 f, s, u;
    gdFloat4 *m;

    f = gd_vec3_sub(centre, eye);
    f = gd_vec3_norm(f);

    s = gd_vec3_cross(f, up);
    s = gd_vec3_norm(s);

    u = gd_vec3_cross(s, f);

    gd_mat4_identity(out);
    m = gd_float44_m(out);

    m[0][0] = +s.x;
    m[1][0] = +s.y;
    m[2][0] = +s.z;

    m[0][1] = +u.x;
    m[1][1] = +u.y;
    m[2][1] = +u.z;

    m[0][2] = -f.x;
    m[1][2] = -f.y;
    m[2][2] = -f.z;

    m[3][0] = gd_vec3_dot(s, eye);
    m[3][1] = gd_vec3_dot(u, eye);
    m[3][2] = gd_vec3_dot(f, eye);
}

void gd_float22_identity(float m[2][2]){
    m[0][0] = 1; m[0][1] = 0;
    m[1][0] = 0; m[1][1];
}
gdFloat2* gd_float22_m(gdMat2* m){ return ((gdFloat2*)m); }
gdFloat2* gd_float22_v(gdVec2 m[2]){ return ((gdFloat2*)m); }
gdFloat2* gd_float22_4(float m[4]){ return ((gdFloat2*)m); }
void gd_float22_transpose(float(*vec)[2]){
    int i, j;
    for (j = 0; j < 2; j++){
        for (i = j + 1; i < 2; i++){
            float t = vec[i][j];
            vec[i][j] = vec[j][i];
            vec[j][i] = t;
        }
    }
}
void gd_float22_mul(float(*out)[2], float(*mat1)[2], float(*mat2)[2]){
    int i, j;
    float temp1[2][2], temp2[2][2];
    if (mat1 == out){ gd__memcpy_4byte(temp1, mat1, sizeof(temp1)); mat1 = temp1; }
    if (mat2 == out){ gd__memcpy_4byte(temp2, mat2, sizeof(temp2)); mat2 = temp2; }
    for (j = 0; j < 2; j++){
        for (int i = 0; i < 2; i++){
            out[j][i] =
                mat1[0][i] * mat2[j][0] +
                mat1[1][i] * mat2[j][i];
        }
    }
}
gdVec2 gd_float22_mul_vec2(float m[2][2], gdVec2 v1){
    gdVec2 r;
    r.x = m[0][0] * v1.x + m[0][1] * v1.y;
    r.y = m[1][0] * v1.x + m[1][1] * v1.y;
    return(r);
}

void gd_float33_identity(float m[3][3]){
    m[0][0] = 1; m[0][1] = 0; m[0][2] = 0;
    m[1][0] = 0; m[1][1] = 1; m[1][2] = 0;
    m[2][0] = 0; m[2][1] = 0; m[2][2] = 1;
}
gdFloat3* gd_float33_m(gdMat3* m){ return((gdFloat3*)m); }
gdFloat3* gd_float33_v(gdVec3 m[3]){ return((gdFloat3*)m); }
gdFloat3* gd_float33_9(float m[9]){ return((gdFloat3*)m); }
void gd_float33_transpose(float(*vec)[3]){
    int i, j;
    for (j = 0; j < 3; j++) {
        for (i = j + 1; i < 3; i++) {
            float t = vec[i][j];
            vec[i][j] = vec[j][i];
            vec[j][i] = t;
        }
    }
}
void gd_float33_mul(float(*out)[3], float(*mat1)[3], float(*mat2)[3]){
    int i, j;
    float temp1[3][3], temp2[3][3];
    if (mat1 == out) { gd__memcpy_4byte(temp1, mat1, sizeof(temp1)); mat1 = temp1; }
    if (mat2 == out) { gd__memcpy_4byte(temp2, mat2, sizeof(temp2)); mat2 = temp2; }
    for (j = 0; j < 3; j++) {
        for (i = 0; i < 3; i++) {
            out[j][i] =
                mat1[0][i] * mat2[j][0] + 
                mat1[1][i] * mat2[j][1] +
                mat1[2][i] * mat2[j][2];
        }
    }
}
gdVec3 gd_float33_mul_vec3(float m[3][3], gdVec3 v1){
    gdVec3 r;
    r.x = m[0][0] * v1.x + m[0][1] * v1.y + m[0][2] * v1.z;
    r.y = m[1][0] * v1.x + m[1][1] * v1.y + m[1][2] * v1.z;
    r.z = m[2][0] * v1.x + m[2][1] * v1.y + m[2][2] * v1.z;
    return(r);
}

void gd_float44_identity(float m[4][4]){
    m[0][0] = 1; m[0][1] = 0; m[0][2] = 0; m[0][3] = 0;
    m[1][0] = 0; m[1][1] = 1; m[1][2] = 0; m[1][3] = 0;
    m[2][0] = 0; m[2][1] = 0; m[2][2] = 1; m[2][3] = 0;
    m[3][0] = 0; m[3][1] = 0; m[3][2] = 0; m[3][3] = 1;
}

gdFloat4* gd_float44_m(gdMat4* m){ return ((gdFloat4*)m); }
gdFloat4* gd_float44_v(gdVec4 m[4]){ return ((gdFloat4*)m); }
gdFloat4* gd_float44_16(float m[16]){ return ((gdFloat4*)m); }

void gd_float44_transpose(float(*vec)[4]){
    int i, j;
    for (j = 0; j < 4; j++) {
        for (i = j + 1; i < 4; i++) {
            float t = vec[i][j];
            vec[i][j] = vec[j][i];
            vec[j][i] = t;
        }
    }
}

void gd_float44_mul(float(*out)[4], float(*mat1)[4], float(*mat2)[4]){
    int i, j;
    float temp1[4][4], temp2[4][4];
    if (mat1 == out) { gd__memcpy_4byte(temp1, mat1, sizeof(temp1)); mat1 = temp1; }
    if (mat2 == out) { gd__memcpy_4byte(temp2, mat2, sizeof(temp2)); mat2 = temp2; }
    for (j = 0; j < 4; j++) {
        for (i = 0; i < 4; i++) {
            out[j][i] = mat1[0][i] * mat2[j][0]
                + mat1[1][i] * mat2[j][1]
                + mat1[2][i] * mat2[j][2]
                + mat1[3][i] * mat2[j][3];
        }
    }
}

gdVec4 gd_float44_mul_vec4(float m[4][4], gdVec4 v1){
    gdVec4 r;
    r.x = m[0][0] * v1.x + m[1][0] * v1.y + m[2][0] * v1.z + m[3][0] * v1.w;
    r.y = m[0][1] * v1.x + m[1][1] * v1.y + m[2][1] * v1.z + m[3][1] * v1.w;
    r.z = m[0][2] * v1.x + m[1][2] * v1.y + m[2][2] * v1.z + m[3][2] * v1.w;
    r.w = m[0][3] * v1.x + m[1][3] * v1.y + m[2][3] * v1.z + m[3][3] * v1.w;
    return(r);
}

/*Quaternions implementation*/
gdQuat gd_quat(float x, float y, float z, float w){ gdQuat q; q.x = x; q.y = y; q.z = z; q.w = w; return(q); }
gdQuat gd_quat_from_array(float arr[4]){ gdQuat q; q.x = arr[0]; q.y = arr[1]; q.z = arr[2]; q.w = arr[3]; return (q); }

gdQuat gd_quat_from_angle(gdVec3 axis, float angle_radians){
    gdQuat q;
    q.xyz = gd_vec3_mul(gd_vec3_norm(axis), gd_sin(0.5f * angle_radians));
    q.w = gd_cos(0.5f * angle_radians);
    return q;
}
gdQuat gd_quat_from_euler_angles(float pitch, float yaw, float roll){
    gdQuat q, p, y, r;
    p = gd_quat_from_angle(gd_vec3(1, 0, 0), pitch);
    y = gd_quat_from_angle(gd_vec3(0, 1, 0), yaw);
    r = gd_quat_from_angle(gd_vec3(0, 0, 1), roll);
    q = gd_quat_mul(y, p);
    q = gd_quat_mul(q, r);
    return q;
}
gdQuat gd_quat_identity(void){ gdQuat q = { 0, 0, 0, 1 }; return q; }

gdQuat gd_quat_add(gdQuat q1, gdQuat q2){gdQuat q; q.xyzw = gd_vec4_add(q1.xyzw, q2.xyzw); return(q);}
gdQuat gd_quat_sub(gdQuat q1, gdQuat q2){gdQuat q; q.xyzw = gd_vec4_sub(q1.xyzw, q2.xyzw); return(q);}
gdQuat gd_quat_mul(gdQuat q1, gdQuat q2){
    gdQuat q;
    q.x = q1.w * q2.x + q1.x * q2.w + q1.y * q2.z - q1.z * q2.y;
    q.y = q1.w * q2.y - q1.x * q2.z + q1.y * q2.w + q1.z * q2.x;
    q.z = q1.w * q2.z + q1.x * q2.y - q1.y * q2.x + q1.z * q2.w;
    q.w = q1.w * q2.w - q1.x * q2.x - q1.y * q2.y - q1.z * q2.z;
    return(q);
}
gdQuat gd_quat_div(gdQuat q1, gdQuat q2){ gdQuat iq2 = gd_quat_inverse(q2); return(gd_quat_mul(q1, iq2)); }

gdQuat gd_quat_mulf(gdQuat q, float s){ gdQuat r; r.xyzw = gd_vec4_mul(q.xyzw, s); return(r); }
gdQuat gd_quat_divf(gdQuat q, float s){ gdQuat r; r.xyzw = gd_vec4_div(q.xyzw, s); return(r); }

float gd_quat_dot(gdQuat q1, gdQuat q2){ float r = gd_vec4_dot(q1.xyzw, q2.xyzw); return r; }
float gd_quat_mag(gdQuat q){ return gd_sqrt(gd_quat_dot(q, q)); }

gdQuat gd_quat_norm(gdQuat q){ return(gd_quat_divf(q, gd_quat_mag(q))); }
gdQuat gd_quat_conj(gdQuat q){ gdQuat r; r.xyz = gd_vec3(-q.x, -q.y, -q.z); r.w = q.w; return(r); }
gdQuat gd_quat_inverse(gdQuat q){ gdQuat r = gd_quat_conj(q); r = gd_quat_divf(r, gd_quat_dot(q, q)); return(r); }

gdVec3 gd_quat_axis(gdQuat q){
    gdQuat n = gd_quat_norm(q);
    gdVec3 r = gd_vec3_div(n.xyz, gd_sin(gd_arccos(q.w)));
    return(r);
}
float gd_quat_angle(gdQuat q){
    float mag = gd_quat_mag(q);
    float c = q.w * (1.0f / mag);
    float angle = 2.0f * gd_arccos(c);
    return(angle);
}

gdVec3 gd_quat_rotate_vec3(gdQuat q, gdVec3 v){
    gdVec3 r, t, p;
    t = gd_vec3_mul(gd_vec3_cross(q.xyz, v), 2.0f);
    p = gd_vec3_cross(q.xyz, t);
    r = gd_vec3_add(gd_vec3_add(gd_vec3_mul(t, q.w), v), p);
    return (r);
}

float gd_quat_roll(gdQuat q){ return(gd_arctan2(2.0f*q.x*q.y + q.z*q.w, q.x*q.x + q.w*q.w - q.y*q.y - q.z*q.z)); }
float gd_quat_pitch(gdQuat q){ return(gd_arctan2(2.0f*q.y*q.z + q.w*q.x, q.w*q.w - q.x*q.x - q.y*q.y + q.z*q.z)); }
float gd_quat_yaw(gdQuat q){ return gd_arcsin(-2.0f*(q.x*q.z - q.w*q.y)); }

void gd_mat4_from_quat(gdMat4* out, gdQuat q){
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

gdQuat gd_quat_from_mat4(gdMat4* m){
    gdQuat r;

    gdFloat4 *fm;
    float four_x_squared_minus_1, four_y_squared_minus_1,
        four_z_squared_minus_1, four_w_squared_minus_1,
        four_biggest_squared_minus_1;

    int biggest_index = 0;
    float biggest_value, mult;

    fm = gd_float44_m(m);

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

    biggest_value = gd_sqrt(four_biggest_squared_minus_1 + 1.0f) * 0.5f;
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
        }   break;
        default:{

        }break;
    }
    return(r);
}

/*Interpolations implementation*/
float gd_lerp(float a, float b, float t){ return(a * (1.0f - t) + b * t); }
float gd_unlerp(float a, float b, float t){ return((t - a) / (b - a)); }
float gd_smooth_step(float a, float b, float t){ float x = (t - a) / (b - a); return(x * x * (3.0f - 2.0f * x)); }
float gd_smoother_step(float a, float b, float t){ float x = (t - a) / (b - a); return(x * x * x * (x * (6.0f * x - 15.0f) + 10.0f)); }

#define GD_VEC_LERPN(N, a, b, t) \
    gdVec##N r = gd_vec##N##_sub(b, a); \
    r = gd_vec##N##_mul(r, t); \
    r = gd_vec##N##_add(r, a); \
    return(r);
gdVec2 gd_vec2_lerp(gdVec2 a, gdVec2 b, float t){ GD_VEC_LERPN(2, a, b, t); }
gdVec3 gd_vec3_lerp(gdVec3 a, gdVec3 b, float t){ GD_VEC_LERPN(3, a, b, t); }
gdVec4 gd_vec4_lerp(gdVec4 a, gdVec4 b, float t){ GD_VEC_LERPN(4, a, b, t); }
#undef GD_VEC_LERPN

gdQuat gd_quat_lerp(gdQuat a, gdQuat b, float t){ gdQuat q; q.xyzw = gd_vec4_lerp(a.xyzw, b.xyzw, t); return(q); }
gdQuat gd_quat_nlerp(gdQuat a, gdQuat b, float t){ gdQuat q = gd_quat_norm(gd_quat_lerp(a, b, t)); return(q); }

gdQuat gd_quat_slerp(gdQuat a, gdQuat b, float t){
    gdQuat r;
    gdQuat x, y, z;
    float cos_theta, angle;
    float s1, s0, is;

    z = b;
    cos_theta = gd_quat_dot(a, b);

    if (cos_theta < 0.0f){
        z = gd_quat(-b.x, -b.y, -b.z, -b.w);
        cos_theta = -cos_theta;
    }

    if (cos_theta > 1.0f){
        r = gd_quat_lerp(a, b, t);
    }

    angle = gd_arccos(cos_theta);
    s1 = gd_sin(1.0f - t * angle);;
    s0 = gd_sin(t * angle);
    is = 1.0f / gd_sin(angle);
    x = gd_quat_mulf(z, s1);
    y = gd_quat_mulf(z, s0);
    r = gd_quat_mulf(gd_quat_add(x, y), is);
    return(r);
}

gdQuat gd_quat_slerp_approx(gdQuat a, gdQuat b, float t){
    float tp = t + (1.0f - gd_quat_dot(a, b)) / 3.0f * t * (-2.0f * t * t + 3.0f * t - 1.0f);
    return(gd_quat_nlerp(a, b, tp));
}

gdQuat gd_quat_nquad(gdQuat p, gdQuat a, gdQuat b, gdQuat q, float t){
    gdQuat x, y, r;
    x = gd_quat_nlerp(p, q, t);
    y = gd_quat_nlerp(a, b, t);
    r = gd_quat_nlerp(x, y, 2.0f * t * (1.0f - t));
    return(r);
}

gdQuat gd_quat_squad(gdQuat p, gdQuat a, gdQuat b, gdQuat q, float t){
    gdQuat x, y, r;
    x = gd_quat_slerp(p, q, t);
    y = gd_quat_slerp(a, b, t);
    r = gd_quat_slerp(x, y, 2.0f * t * (1.0f - t));
    return(r);
}

gdQuat gd_quat_squad_approx(gdQuat p, gdQuat a, gdQuat b, gdQuat q, float t){
    gdQuat x, y, r;
    x = gd_quat_slerp_approx(p, q, t);
    y = gd_quat_slerp_approx(a, b, t);
    r = gd_quat_slerp_approx(x, y, 2.0f * t * (1.0f - t));
    return(r);
}

/*Rectangles implementation*/
gdRect2 gd_rect2(gdVec2 Pos, gdVec2 Dim){
    gdRect2 Result;
    Result.Pos = Pos;
    Result.Dimension = Dim;
    return(Result);
}

gdRect3 gd_rect3(gdVec3 Pos, gdVec3 Dim){
    gdRect3 Result;
    Result.Pos = Pos;
    Result.Dimension = Dim;
    return(Result);
}

gdm_bool32 gd_rect2_contains(gdRect2 r, gdVec2 v){
    gdm_bool32 res = (v.x >= r.Pos.x) && (v.x <= r.Pos.x + r.Dimension.x) && (v.y >= r.Pos.y) && (v.y <= r.Pos.y + r.Dimension.y);
    return(res);
}

gdm_bool32 gd_rect2_has_intersection(gdRect2 r1, gdRect2 r2){
    gdm_bool32 Result;
    gdm_bool32 Cont1 = gd_rect2_contains(r2, r1.Pos);
    gdm_bool32 Cont2 = gd_rect2_contains(r2, r1.Pos + gd_vec2(r1.Dimension.x, 0.0f));
    gdm_bool32 Cont3 = gd_rect2_contains(r2, r1.Pos + gd_vec2(0.0f, r1.Dimension.y));
    gdm_bool32 Cont4 = gd_rect2_contains(r2, r1.Pos + r1.Dimension);
    Result = Cont1 || Cont2 || Cont3 || Cont4;
    return(Result);
}

gdRect2 gd_rect2_get_intersection(gdRect2 r1, gdRect2 r2){
    gdRect2 Result;

    float r1_max_x = r1.Pos.x + r1.Dimension.x;
    float r1_max_y = r1.Pos.y + r1.Dimension.y;
    float r2_max_x = r2.Pos.x + r2.Dimension.x;
    float r2_max_y = r2.Pos.y + r2.Dimension.y;

    Result.Pos.x = r1.Pos.x < r2.Pos.x ? r2.Pos.x : r1.Pos.x;
    Result.Pos.y = r1.Pos.y < r2.Pos.y ? r2.Pos.y : r1.Pos.y;

    Result.Dimension.x = r1_max_x > r2_max_x ? r2_max_x - Result.Pos.x : r1_max_x - Result.Pos.x;
    Result.Dimension.y = r1_max_y > r2_max_y ? r2_max_y - Result.Pos.y : r1_max_y - Result.Pos.y;

    return(Result);
}

gdRect2i gd_rect2_i(int MinX, int MinY, int MaxX, int MaxY){
    gdRect2i Result;
    Result.MinX = MinX;
    Result.MaxX = MaxX;
    Result.MinY = MinY;
    Result.MaxY = MaxY;
    return(Result);
}

gdRect2i gd_rect2_i_get_intersection(gdRect2i r1, gdRect2i r2){
    gdRect2i Result;
    Result.MinX = r1.MinX < r2.MinX ? r2.MinX : r1.MinX;
    Result.MaxX = r1.MaxX < r2.MaxX ? r2.MaxX : r1.MaxX;
    Result.MinY = r1.MinY > r2.MinY ? r2.MinY : r1.MinY;
    Result.MaxY = r1.MaxY > r2.MaxY ? r2.MaxY : r1.MaxY;
    return(Result);
}

gdRect2i gd_rect2_i_inverse_infinite(){
    gdRect2i Result;
    Result.MinX = GD_MATH_I32_MAX;
    Result.MaxX = GD_MATH_I32_MIN;
    Result.MinY = GD_MATH_I32_MAX;
    Result.MaxY = GD_MATH_I32_MIN;
    return(Result);
}

/*
//Maybe make SIMD version of those two
inline gdMat4 MultiplyMatrices(gdMat4 m1, gdMat4 m2){
    gdMat4 Result = {};
#ifndef GD_MATH_ALLOW_SIMD
    for (int j = 0; j < 4; j++){
        for (int i = 0; i < 4; i++){

            gdVec4 SourceV = gd_vec4(
                *(m1.data + j * 4 + 0),
                *(m1.data + j * 4 + 1),
                *(m1.data + j * 4 + 2),
                *(m1.data + j * 4 + 3));
            gdVec4 DestV = gd_vec4(
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

inline gdVec4 MultiplyMatrixBygdVec4(gdMat4 mat, gdVec4 vec){
    gdVec4 Result;
#ifndef GD_MATH_ALLOW_SIMD
    for (int j = 0; j < 4; j++){
        gdVec4 SourceV = gd_vec4(
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

#endif /*GD_MATH_IMPLEMENTATION*/



