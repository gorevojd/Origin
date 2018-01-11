#ifndef ORIGIN_DIMA
#define ORIGIN_DIMA

#include <stdio.h>
#include <stdint.h>
#include <math.h>

#ifndef INTERNAL_FUNCTION
#define INTERNAL_FUNCTION static
#endif

#ifndef GLOBAL_VARIABLE
#define GLOBAL_VARIABLE static
#endif

#ifndef ABS
#define ABS(Value) ((Value) < 0 ? -(Value) : (Value))
#endif

#ifndef DEG_TO_RAD
#define DEG_TO_RAD 0.0174532925f
#endif

#ifndef RAD_TO_DEG
#define RAD_TO_DEG 57.2958f
#endif

#ifdef PRETTY_TYPES_DEFINED
#undef DO_NOT_DEFINE_PRETTY_TYPES
#endif

#if !defined(DO_NOT_DEFINE_PRETTY_TYPES) && !defined(PRETTY_TYPES_DEFINED)
#define PRETTY_TYPES_DEFINED

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef int8_t s8;
typedef int16_t s16;
typedef int32_t s32;
typedef int64_t s64;

typedef float f32;
typedef float r32;

typedef double f64;
typedef double r64;

typedef size_t memory_index;
typedef uintptr_t umm;
typedef intptr_t smm;

typedef union v2{
	struct {
		float x;
		float y;
	};

	float E[2];
} v2;

typedef union v3{
	struct {
		float x;
		float y;
		float z;
	};

	struct {
		float r;
		float g;
		float b;
	};

	float E[3];
} v3;

typedef union v4{
	struct {
		union{
			struct {
				float x;
				float y;
				float z;
			};

			v3 xyz;
		};

		float w;
	};

	struct {
		union{
			struct{
				float r;
				float g;
				float b;
			};

			v3 rgb;
		};

		float a;
	};

	struct {
		union{
			struct {
				float A;
				float B;
				float C;
			};
			v3 ABC;
			v3 N;
		};

		float D;
	};

	float E[4];
} v4;

typedef union mat4{
	float E[16];
	v4 Rows[4];
} mat4;

typedef union quat{
	struct {
		float x, y, z, w;
	};
	v3 xyz;
	v4 xyzw;
} quat;

#endif /*Pretty types definitions*/

inline float Sqrt(float Value) {
	float Result;
	Result = sqrtf(Value);
	return(Result);
}

inline float RSqrt(float Value) {
	float Result;
	Result = 1.0f / sqrtf(Value);
	return(Result);
}

inline float Sin(float Rad) {
	float Result = sinf(Rad);
	return(Result);
}

inline float Cos(float Rad) {
	float Result = cosf(Rad);
	return(Result);
}

inline float Tan(float Rad) {
	float Result = tanf(Rad);
	return(Result);
}

inline float ASin(float Value) {
	float Result = asinf(Value);
	return(Result);
}

inline float ACos(float Value) {
	float Result = acosf(Value);
	return(Result);
}

inline float ATan(float Value) {
	float Result = atan(Value);
	return(Result);
}

inline float ATan2(float Y, float X) {
	float Result = atan2f(Y, X);
	return(Result);
}

inline float Exp(float Value) {
	float Result = expf(Value);
	return(Result);
}

inline float Log(float Value) {
	float Result = logf(Value);
	return(Result);
}

inline float Pow(float a, float b) {
	float Result = powf(a, b);
	return(Result);
}

#if !defined(DO_NOT_IMPLEMENT_VECTORS) && !defined(VECTORS_IMPLEMENTED)
#define VECTORS_IMPLEMENTED

inline v2 V2(float x, float y) {
	v2 Result;

	Result.x = x;
	Result.y = y;

	return(Result);
}

inline v3 V3(float x, float y, float z) {
	v3 Result;

	Result.x = x;
	Result.y = y;
	Result.z = z;

	return(Result);
}

inline v4 V4(float Value) {
	v4 Result;
	Result.x = Value;
	Result.y = Value;
	Result.z = Value;
	Result.w = Value;
	return(Result);
}

inline v4 V4(float x, float y, float z, float w) {
	v4 Result;

	Result.x = x;
	Result.y = y;
	Result.z = z;
	Result.w = w;

	return(Result);
}

inline v4 V4(v3 InitVector, float w) {
	v4 Result;
	Result.x = InitVector.x;
	Result.y = InitVector.y;
	Result.z = InitVector.z;
	Result.w = w;
	return(Result);
}

/*Dot operations*/
inline float Dot(v2 A, v2 B) { 
	return A.x * B.x + A.y * B.y; 
}

inline float Dot(v3 A, v3 B) { 
	return A.x * B.x + A.y * B.y + A.z * B.z; 
}

inline float Dot(v4 A, v4 B) { 
	return A.x * B.x + A.y * B.y + A.z * B.z + A.w * B.w; 
}

/*Cross product*/
inline float Cross(v2 A, v2 B){ return A.x * B.y - B.x * A.y; }
inline v3 Cross(v3 A, v3 B){
    v3 R;
    R.x = A.y * B.z - B.y * A.z;
    R.y = A.z * B.x - B.z * A.x;
    R.z = A.x * B.y - B.x * A.y;
    return(A);
}

/*Add operation*/
inline v2 Add(v2 a, v2 b){
	a.x += b.x;
	a.y += b.y;
	return(a);
}

inline v3 Add(v3 a, v3 b){
	a.x += b.x;
	a.y += b.y;
	a.z += b.z;
	return(a);
}

inline v4 Add(v4 a, v4 b){
	a.x += b.x;
	a.y += b.y;
	a.z += b.z;
	a.w += b.w;
	return(a);
}

/*Subtract operation*/
inline v2 Sub(v2 a, v2 b){
	a.x -= b.x;
	a.y -= b.y;
	return(a);
}

inline v3 Sub(v3 a, v3 b){
	a.x -= b.x;
	a.y -= b.y;
	a.z -= b.z;
	return(a);
}

inline v4 Sub(v4 a, v4 b){
	a.x -= b.x;
	a.y -= b.y;
	a.z -= b.z;
	a.w -= b.w;
	return(a);
}

/*Multiply operation*/
inline v2 Mul(v2 a, float s){
	a.x *= s;
	a.y *= s;
	return(a);
}

inline v3 Mul(v3 a, float s){
	a.x *= s;
	a.y *= s;
	a.z *= s;
	return(a);
}

inline v4 Mul(v4 a, float s){
	a.x *= s;
	a.y *= s;
	a.z *= s;
	a.w *= s;
	return(a);
}

/*Divide operation*/
inline v2 Div(v2 a, float s){
	float OneOverS = 1.0f / s;
	a.x *= OneOverS;
	a.y *= OneOverS;
	return(a);
}

inline v3 Div(v3 a, float s){
	float OneOverS = 1.0f / s;
	a.x *= OneOverS;
	a.y *= OneOverS;
	a.z *= OneOverS;
	return(a);
}

inline v4 Div(v4 a, float s){

	float OneOverS = 1.0f / s;
	a.x *= OneOverS;
	a.y *= OneOverS;
	a.z *= OneOverS;
	a.w *= OneOverS;
	return(a);
}

/*Hadamard product*/
inline v2 Hadamard(v2 A, v2 B){ return (V2(A.x * B.x, A.y * B.y));}
inline v3 Hadamard(v3 A, v3 B){ return (V3(A.x * B.x, A.y * B.y, A.z * B.z));}
inline v4 Hadamard(v4 A, v4 B){ return (V4(A.x * B.x, A.y * B.y, A.z * B.z, A.w * B.w));}

/*Magnitude of the vector*/
inline float Magnitude(v2 A){ return(Sqrt(Dot(A, A)));}
inline float Magnitude(v3 A){ return(Sqrt(Dot(A, A)));}
inline float Magnitude(v4 A){ return(Sqrt(Dot(A, A)));}

/*Squared magnitude*/
inline float SqMagnitude(v2 A){ return(Dot(A, A)); }
inline float SqMagnitude(v3 A){ return(Dot(A, A)); }
inline float SqMagnitude(v4 A){ return(Dot(A, A)); }

/*v2 operator overloading*/
inline bool operator==(v2 a, v2 b) { return((a.x == b.x) && (a.y == b.y)); }
inline bool operator!=(v2 a, v2 b){ return((a.x != b.x) || (a.y != b.y)); }

inline v2 operator+(v2 a){ return(a); }
inline v2 operator-(v2 a){ v2 r = { -a.x, -a.y }; return(r); }

inline v2 operator+(v2 a, v2 b){ return Add(a, b); }
inline v2 operator-(v2 a, v2 b){ return Sub(a, b); }

inline v2 operator*(v2 a, float s){ return Mul(a, s); }
inline v2 operator*(float s, v2 a){ return Mul(a, s); }
inline v2 operator/(v2 a, float s){ return Div(a, s); }

inline v2 operator*(v2 a, v2 b){v2 r = {a.x * b.x, a.y * b.y}; return(r);}
inline v2 operator/(v2 a, v2 b){v2 r = {a.x / b.x, a.y / b.y}; return(r);}

inline v2 &operator+=(v2& a, v2 b){return(a = a + b);}
inline v2 &operator-=(v2& a, v2 b){return(a = a - b);}
inline v2 &operator*=(v2& a, float s){return(a = a * s);}
inline v2 &operator/=(v2& a, float s){return(a = a / s);}

/*v3 operator overloading*/
inline bool operator==(v3 a, v3 b){return((a.x == b.x) && (a.y == b.y) && (a.z == b.z));}
inline bool operator!=(v3 a, v3 b){return((a.x != b.x) || (a.y != b.y) || (a.z != b.z));}

inline v3 operator+(v3 a){return(a);}
inline v3 operator-(v3 a){v3 r = {-a.x, -a.y, -a.z}; return(r);}

inline v3 operator+(v3 a, v3 b){ return Add(a, b); }
inline v3 operator-(v3 a, v3 b){ return Sub(a, b); }

inline v3 operator*(v3 a, float s){ return Mul(a, s); }
inline v3 operator*(float s, v3 a){ return Mul(a, s); }
inline v3 operator/(v3 a, float s){ return Div(a, s); }

inline v3 operator*(v3 a, v3 b){v3 r = {a.x * b.x, a.y * b.y, a.z * b.z}; return(r);}
inline v3 operator/(v3 a, v3 b){v3 r = {a.x / b.x, a.y / b.y, a.z / b.z}; return(r);}

inline v3 &operator+=(v3& a, v3 b){return(a = a + b);}
inline v3 &operator-=(v3& a, v3 b){return(a = a - b);}
inline v3 &operator*=(v3& a, float s){return(a = a * s);}
inline v3 &operator/=(v3& a, float s){return(a = a / s);}

/*v4 operator overloading*/
inline bool operator==(v4 a, v4 b) { return((a.x == b.x) && (a.y == b.y) && (a.z == b.z) && (a.w == b.w)); }
inline bool operator!=(v4 a, v4 b){ return((a.x != b.x) || (a.y != b.y) || (a.z != b.z) || (a.w != b.w)); }

inline v4 operator+(v4 a){ return(a); }
inline v4 operator-(v4 a){ v4 r = { -a.x, -a.y }; return(r); }

inline v4 operator+(v4 a, v4 b){ return Add(a, b); }
inline v4 operator-(v4 a, v4 b){ return Sub(a, b); }

inline v4 operator*(v4 a, float s){ return Mul(a, s); }
inline v4 operator*(float s, v4 a){ return Mul(a, s); }
inline v4 operator/(v4 a, float s){ return Div(a, s); }

inline v4 operator*(v4 a, v4 b){v4 r = {a.x * b.x, a.y * b.y, a.z * b.z, a.w * b.w}; return(r);}
inline v4 operator/(v4 a, v4 b){v4 r = {a.x / b.x, a.y / b.y, a.z / b.z, a.w / b.w}; return(r);}

inline v4 &operator+=(v4& a, v4 b){return(a = a + b);}
inline v4 &operator-=(v4& a, v4 b){return(a = a - b);}
inline v4 &operator*=(v4& a, float s){return(a = a * s);}
inline v4 &operator/=(v4& a, float s){return(a = a / s);}

/*Normalization of the vector*/
inline v2 Normalize(v2 v){ return(Mul(v, RSqrt(Dot(v, v)))); }
inline v3 Normalize(v3 v){ return(Mul(v, RSqrt(Dot(v, v)))); }
inline v4 Normalize(v4 v){ return(Mul(v, RSqrt(Dot(v, v)))); }

inline v2 NOZ(v2 v){ float sqmag = Dot(v, v); return((sqmag) == 0.0f ? V2(0.0f, 0.0f) : v * RSqrt(sqmag)); }
inline v3 NOZ(v3 v){ float sqmag = Dot(v, v); return((sqmag) == 0.0f ? V3(0.0f, 0.0f, 0.0f) : v * RSqrt(sqmag)); }
inline v4 NOZ(v4 v){ float sqmag = Dot(v, v); return((sqmag) == 0.0f ? V4(0.0f, 0.0f, 0.0f, 0.0f) : v * RSqrt(sqmag)); }

/*Vector LERPS*/
#if 0
inline v2 Lerp(v2 A, v2 B, float t){ return(A + (B - A) * t);}
inline v3 Lerp(v3 A, v3 B, float t){ return(A + (B - A) * t);}
inline v4 Lerp(v4 A, v4 B, float t){ return(A + (B - A) * t);}
#else
inline v2 Lerp(v2 A, v2 B, float t){ return((1.0f - t) * A + B * t); }
inline v3 Lerp(v3 A, v3 B, float t){ return((1.0f - t) * A + B * t); }
inline v4 Lerp(v4 A, v4 B, float t){ return((1.0f - t) * A + B * t); }
#endif

#endif /*vector implementation*/


#if !defined(DO_NOT_IMPLEMENT_MATRICES) && !defined(MATRICES_IMPLEMENTED)

/*Matrix 4x4 functions and operators*/
inline mat4 Multiply(mat4 M1, mat4 M2){
	mat4 Result = {};

	Result.E[0] = M1.E[0] * M2.E[0] + M1.E[1] * M2.E[4] + M1.E[2] * M2.E[8] + M1.E[3] * M2.E[12];
	Result.E[1] = M1.E[0] * M2.E[1] + M1.E[1] * M2.E[5] + M1.E[2] * M2.E[9] + M1.E[3] * M2.E[13];
	Result.E[2] = M1.E[0] * M2.E[2] + M1.E[1] * M2.E[6] + M1.E[2] * M2.E[10] + M1.E[3] * M2.E[14];
	Result.E[3] = M1.E[0] * M2.E[3] + M1.E[1] * M2.E[7] + M1.E[2] * M2.E[11] + M1.E[3] * M2.E[15];

	Result.E[4] = M1.E[4] * M2.E[0] + M1.E[5] * M2.E[4] + M1.E[6] * M2.E[8] + M1.E[7] * M2.E[12];
	Result.E[5] = M1.E[4] * M2.E[1] + M1.E[5] * M2.E[5] + M1.E[6] * M2.E[9] + M1.E[7] * M2.E[13];
	Result.E[6] = M1.E[4] * M2.E[2] + M1.E[5] * M2.E[6] + M1.E[6] * M2.E[10] + M1.E[7] * M2.E[14];
	Result.E[7] = M1.E[4] * M2.E[3] + M1.E[5] * M2.E[7] + M1.E[6] * M2.E[11] + M1.E[7] * M2.E[15];

	Result.E[8] = M1.E[8] * M2.E[0] + M1.E[9] * M2.E[4] + M1.E[10] * M2.E[8] + M1.E[11] * M2.E[12];
	Result.E[9] = M1.E[8] * M2.E[1] + M1.E[9] * M2.E[5] + M1.E[10] * M2.E[9] + M1.E[11] * M2.E[13];
	Result.E[10] = M1.E[8] * M2.E[2] + M1.E[9] * M2.E[6] + M1.E[10] * M2.E[10] + M1.E[11] * M2.E[14];
	Result.E[11] = M1.E[8] * M2.E[3] + M1.E[9] * M2.E[7] + M1.E[10] * M2.E[11] + M1.E[11] * M2.E[15];

	Result.E[12] = M1.E[12] * M2.E[0] + M1.E[13] * M2.E[4] + M1.E[14] * M2.E[8] + M1.E[15] * M2.E[12];
	Result.E[13] = M1.E[12] * M2.E[1] + M1.E[13] * M2.E[5] + M1.E[14] * M2.E[9] + M1.E[15] * M2.E[13];
	Result.E[14] = M1.E[12] * M2.E[2] + M1.E[13] * M2.E[6] + M1.E[14] * M2.E[10] + M1.E[15] * M2.E[14];
	Result.E[15] = M1.E[12] * M2.E[3] + M1.E[13] * M2.E[7] + M1.E[14] * M2.E[11] + M1.E[15] * M2.E[15];

	return(Result);
}

inline v4 Multiply(mat4 M, v4 V){
	v4 Result;

	Result.E[0] = V.E[0] * M.E[0] + V.E[0] * M.E[1] + V.E[0] * M.E[2] + V.E[0] * M.E[3];
	Result.E[1] = V.E[1] * M.E[4] + V.E[1] * M.E[5] + V.E[1] * M.E[6] + V.E[1] * M.E[7];
	Result.E[2] = V.E[2] * M.E[8] + V.E[2] * M.E[9] + V.E[2] * M.E[10] + V.E[2] * M.E[11];
	Result.E[3] = V.E[3] * M.E[12] + V.E[3] * M.E[13] + V.E[3] * M.E[14] + V.E[3] * M.E[15];

	return(Result);
}

inline mat4 Identity(){
	mat4 Result;

	Result.Rows[0] = {1.0f, 0.0f, 0.0f, 0.0f};
	Result.Rows[1] = {0.0f, 1.0f, 0.0f, 0.0f};
	Result.Rows[2] = {0.0f, 0.0f, 1.0f, 0.0f};
	Result.Rows[3] = {0.0f, 0.0f, 0.0f, 1.0f};

	return(Result);
}

inline mat4 Transpose(mat4 M){
	mat4 Result;

	for(int RowIndex = 0; RowIndex < 4; RowIndex++){
		for(int ColumtIndex = 0; ColumtIndex < 4; ColumtIndex++){
			Result.E[ColumtIndex * 4 + RowIndex] = M.E[RowIndex * 4 + ColumtIndex];
		}
	}

	return(Result);
}

inline mat4 TranslationMatrix(v3 Translation){
	mat4 Result = Identity();

	Result.E[3] = Translation.x;
	Result.E[7] = Translation.y;
	Result.E[11] = Translation.z;

	return(Result);
}

inline mat4 RotationMatrix(v3 R, float Angle){
	mat4 Result;

	float CosT = Cos(Angle);
	float SinT = Sin(Angle);
	float InvCosT = 1.0f - CosT;

	float RxRyInvCos = R.x * R.y * InvCosT;
	float RxRzInvCos = R.x * R.z * InvCosT;
	float RyRzInvCos = R.y * R.z * InvCosT;

	Result.E[0] = CosT + R.x * R.x * InvCosT;
	Result.E[1] = RxRyInvCos - R.z * SinT;
	Result.E[2] = RxRzInvCos + R.y * SinT;
	Result.E[3] = 0;

	Result.E[4] = RxRyInvCos + R.z * SinT;
	Result.E[5] = CosT * R.y * R.y * InvCosT;
	Result.E[6] = RyRzInvCos - R.x * SinT;
	Result.E[7] = 0.0f;

	Result.E[8] = RxRzInvCos - R.y * SinT;
	Result.E[9] = RyRzInvCos + R.x * SinT;
	Result.E[10] = CosT + R.z * R.z * InvCosT;
	Result.E[11] = 0.0f;

	Result.E[12] = 0.0f;
	Result.E[13] = 0.0f;
	Result.E[14] = 0.0f;
	Result.E[15] = 1.0f;

	return(Result);
}

inline mat4 ScalingMatrix(v3 Scale){
	mat4 Result = Identity();
	Result.E[0] = Scale.x;
	Result.E[5] = Scale.y;
	Result.E[10] = Scale.z;

	return(Result);
}

inline mat4 Translate(mat4 M, v3 P){
	mat4 Result = M;

	Result.E[3] += P.x;
	Result.E[7] += P.y;
	Result.E[11] += P.z;

	return(Result);
}

inline mat4 operator*(mat4 M1, mat4 M2){
	return(Multiply(M1, M2));
}

inline v4 operator*(mat4 M1, v4 V){
	return(Multiply(M1, V));
}

inline mat4 LookAt(v3 Pos, v3 TargetPos, v3 WorldUp){
	mat4 Result;

	v3 Fwd = TargetPos - Pos;
	Fwd = NOZ(Fwd);

	v3 Left = Normalize(Cross(WorldUp, Fwd));
	v3 Up = Normalize(Cross(Fwd, Left));

	v3 Eye = Pos;

	Result.E[0] = Left.x;
	Result.E[1] = Left.y;
	Result.E[2] = Left.z;
	Result.E[3] = -Dot(Left, Eye);

	Result.E[4] = Up.x;
	Result.E[5] = Up.y;
	Result.E[6] = Up.z;
	Result.E[7] = -Dot(Up, Eye);

	Result.E[8] = -Fwd.x;
	Result.E[9] = -Fwd.y;
	Result.E[10] = -Fwd.z;
	Result.E[11] = Dot(Fwd, Eye);

	Result.E[12] = 0.0f;
	Result.E[13] = 0.0f;
	Result.E[14] = 0.0f;
	Result.E[15] = 1.0f;

	return(Result);
}

inline mat4 PerspectiveProjection(u32 Width, u32 Height, float FOV, float Far, float Near)
{
	mat4 Result = {};

	float AspectRatio = (float)Width / (float)Height;

	float S = 1.0f / (Tan(FOV * 0.5f * DEG_TO_RAD));
	float A = S / AspectRatio;
	float B = S;
	float OneOverFarMinusNear = 1.0f / (Far - Near);
	Result.E[0] = A;
	Result.E[5] = B;
	Result.E[10] = -(Far + Near) * OneOverFarMinusNear;
	Result.E[11] = -(2.0f * Far * Near) * OneOverFarMinusNear;
	Result.E[14] = -1.0f;

	return(Result);
}

inline mat4 OrthographicProjection(
	u32 Right, u32 Left,
	u32 Top, u32 Bottom,
	float Far, float Near)
{
	mat4 Result = {};

	float OneOverRmL = 1.0f / ((float)Right - (float)Left);
	float OneOverTmB = 1.0f / ((float)Top - (float)Bottom);
	float OneOverFmN = 1.0f / (Far - Near);

	Result.E[0] = 2.0f * OneOverRmL;
	Result.E[3] = - (float)(Right + Left) * OneOverRmL;
	Result.E[5] = 2.0f * OneOverTmB;
	Result.E[7] = - (float)(Top + Bottom) * OneOverTmB;
	Result.E[10] = -2.0f * OneOverFmN;
	Result.E[11] = -(Far + Near) * OneOverFmN;
	Result.E[15] = 1.0f;

	return(Result);
}

inline mat4 OrthographicProjection(
	u32 Width, u32 Height,
	float Far, float Near)
{
	mat4 Result = {};

	float OneOverFmN = 1.0f / (Far - Near);
	Result.E[0] = 2.0f / (float)Width;
	Result.E[3] = -1.0f;
	Result.E[5] = 2.0f / (float)Height;
	Result.E[7] = -1.0f;
	Result.E[10] = -2.0f * OneOverFmN;
	Result.E[11] = -(Far + Near) * OneOverFmN;
	Result.E[15] = 1.0f;

	return(Result);
}

inline mat4 OrthographicProjection(u32 Width, u32 Height){
	mat4 Result = {};

	Result.E[0] = 2.0f / (float)Width;
	Result.E[3] = -1.0f;
	Result.E[5] = 2.0f / (float)Height;
	Result.E[7] = -1.0f;
	Result.E[10] = 1.0f;
	Result.E[15] = 1.0f;

	return(Result);
}
#endif /*matrices implementation*/

#if !defined(DO_NOT_IMPLEMENT_QUATERNIONS) && !defined(QUATERNIONS_IMPLEMENTED)

/*Quaternion constructors and operations*/

inline quat Quat(v3 axis, float theta){
	quat Result;

	float HalfTheta = theta * 0.5f;
	float SinScalar = Sin(HalfTheta);
	float OneOverAxisLen = 1.0f / Sqrt((axis.x * axis.x + axis.y * axis.y + axis.z * axis.z));
	Result.x = OneOverAxisLen * axis.x * SinScalar;
	Result.y = OneOverAxisLen * axis.y * SinScalar;
	Result.z = OneOverAxisLen * axis.z * SinScalar;
	Result.w = Cos(HalfTheta);

	return(Result);
}

inline quat Quat(float x, float y, float z, float theta){
	quat Result;

	float HalfTheta = theta * 0.5f;
	float SinScalar = Sin(HalfTheta);
	float OneOverAxisLen = 1.0f / Sqrt((x * x + y * y + z * z));
	Result.x = x * OneOverAxisLen * SinScalar;
	Result.y = y * OneOverAxisLen * SinScalar;
	Result.z = z * OneOverAxisLen * SinScalar;
	Result.w = Cos(HalfTheta);

	return(Result);
}

inline quat Mul(quat q1, quat q2){
    quat q;
    q.x = q1.w * q2.x + q1.x * q2.w + q1.y * q2.z - q1.z * q2.y;
    q.y = q1.w * q2.y - q1.x * q2.z + q1.y * q2.w + q1.z * q2.x;
    q.z = q1.w * q2.z + q1.x * q2.y - q1.y * q2.x + q1.z * q2.w;
    q.w = q1.w * q2.w - q1.x * q2.x - q1.y * q2.y - q1.z * q2.z;
    return(q);
}

inline quat Mul(quat Q, float s){
	quat Result;

	Result.x = Q.x * s;
	Result.y = Q.y * s;
	Result.z = Q.z * s;
	Result.w = Q.w * s;

	return(Result);
}

inline quat Quat(float Yaw, float Pitch, float Roll){
	quat q1 = Quat(1.0f, 0.0f, 0.0f, Pitch);
	quat q2 = Quat(0.0f, 1.0f, 0.0f, Yaw);
	quat q3 = Quat(0.0f, 0.0f, 1.0f, Roll);

	quat Result = Mul(q2, q1);
	Result = Mul(Result, q3);
	return(Result);
}

inline quat QuatIdentity(){
	quat Result;

	Result.x = 0.0f;
	Result.y = 0.0f;
	Result.z = 0.0f;
	Result.w = 1.0f;

	return(Result);
}

inline quat Conjugate(quat q){
	quat Result;

	Result.x = -q.x;
	Result.y = -q.y;
	Result.z = -q.z;
	Result.w = q.w;

	return(Result);
}

inline float Dot(quat q1, quat q2){
	float Result = q1.x * q2.x + q1.y * q2.y + q1.z * q2.z + q1.w * q2.w;

	return(Result);
}

inline quat Add(quat A, quat B){
	quat Result;

	Result.x = A.x + B.x;
	Result.y = A.y + B.y;
	Result.z = A.z + B.z;
	Result.w = A.w + B.w;

	return(Result);
}

inline quat Sub(quat A, quat B){
	quat Result;

	Result.x = A.x - B.x;
	Result.y = A.y - B.y;
	Result.z = A.z - B.z;
	Result.w = A.w - B.w;

	return(Result);
}

inline quat Div(quat q, float s){
	quat Result;
	float OneOverS = 1.0f / s;
	Result.x = q.x * OneOverS;
	Result.y = q.y * OneOverS;
	Result.z = q.z * OneOverS;
	Result.w = q.w * OneOverS;

	return(Result);
}

inline float Length(quat q){
	float Result = Sqrt(Dot(q, q));
	return(Result);
}

inline quat Normalize(quat q){
	quat Result;

	float Len = Length(q);
	float OneOverLen = 1.0f / Len;

	Result.w = q.w * OneOverLen;
	Result.x = q.x * OneOverLen;
	Result.y = q.y * OneOverLen;
	Result.z = q.z * OneOverLen;

	return(Result);
}

inline quat Inverse(quat q){
	quat Result;

	Result = Conjugate(q);
	Result = Div(Result, Dot(q, q));

	return(Result);
}

inline mat4 RotationMatrix(quat Q){
	
	mat4 Result;

	float xy = Q.x * Q.y;
	float xz = Q.x * Q.z;
	float xw = Q.x * Q.w;
	float yz = Q.y * Q.z;
	float yw = Q.y * Q.w;
	float zw = Q.z * Q.w;
	float xSquared = Q.x * Q.x;
	float ySquared = Q.y * Q.y;
	float zSquared = Q.z * Q.z;

	Result.E[0] = 1.0f - 2.0f * (ySquared * zSquared);
	Result.E[1] = 2.0f * (xy - zw);
	Result.E[2] = 2.0f * (xz + yw);
	Result.E[3] = 0.0f;

	Result.E[4] = 2.0f * (xy + zw);
	Result.E[5] = 1.0f - 2.0f * (xSquared + zSquared);
	Result.E[6] = 2.0f * (yz - xw);
	Result.E[7] = 0.0f;

	Result.E[8] = 2.0f * (xz - yw);
	Result.E[9] = 2.0f * (yz + xw);
	Result.E[10] = 1.0f - 2.0f * (xSquared + ySquared);
	Result.E[11] = 0.0f;

	Result.E[8] = 0.0f;
	Result.E[8] = 0.0f;
	Result.E[8] = 0.0f;
	Result.E[8] = 1.0f;

	return(Result);
}

inline quat Slerp(quat A, quat B, float Delta){
	quat Result;

	float dot = Dot(A, B);
	float InvDelta = 1.0f - Delta;

	if(dot < 0.0f){
		B = Quat(-B.x, -B.y -B.z, -B.w);
		dot = -dot;
	}

	float k0, k1;
	if(ABS(dot) > 0.9999f){
		k0 = 1.0f - Delta;
		k1 = Delta;
	}
	else{
		float SinTheta = Sqrt(1.0f - dot * dot);

		float Theta = ATan2(SinTheta, dot);
		float OneOverSinTheta = 1.0f / SinTheta;

		k0 = Sin((1 - Delta) * Theta) * OneOverSinTheta;
		k1 = Sin(Delta * Theta) * OneOverSinTheta;
	}

	Result.x = A.x * k0 + B.x * k1;
	Result.y = A.y * k0 + B.y * k1;
	Result.z = A.z * k0 + B.z * k1;
	Result.w = A.w * k0 + B.w * k1;

	return(Result);
}
#endif /*Quaternions implementation*/

#endif /*ORIGIN_DIMA*/