#ifndef GDA_MATH_H
#ifndef GDA_MATH_DEFENITIONS

#include <math.h>

#ifdef GDA_MATH_ALLOW_SIMD
#include <xmmintrin.h>
#endif

struct vector2{
	union{
		struct{
			float x, y;
		};
		float data[2];
	};
};

struct vector3{
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

struct vector4{
	union{
		struct{
			float x, y, z, w;
		};
		struct{
			vector3 xyz;
			float w;
		};
		struct{
			vector3 rgb;
			float a;
		};
		struct{
			vector2 xy;
			vector2 zw;
		};
		float data[4];
	};
};

struct quaternion{
	float x, y, z, w;
};

struct matrix4x4{
	union{
		float data[16];
		struct{
			float a1, a2, a3, a4,
					b1, b2, b3, b4,
					c1, c2, c3, c4,
					d1, d2, d3, d4;
		};
		struct {
			vector4 row1;
			vector4 row2;
			vector4 row3;
			vector4 row4;
		};
	};
};

#define GDA_MATH_DEFENITIONS
#endif

#define GDA_MATH_IMPLEMENTATION
#ifdef GDA_MATH_IMPLEMENTATION

inline vector2 Vector2(){
	vector2 res;
	res.x = 0.0f;
	res.y = 0.0f;
	return res;
}

inline vector2 Vector2(float x, float y){
	vector2 res;
	res.x = 0.0f;
	res.y = 0.0f;
	return res;
}

inline vector3 Vector3(){
	vector3 res;
	res.x = 0.0f;
	res.y = 0.0f;
	res.z = 0.0f;
	return res;
}

inline vector3 Vector3(float value){
	vector3 res;
	res.x = value;
	res.y = value;
	res.z = value;
	return res;
}

inline vector3 Vector3(float x, float y, float z){
	vector3 res;
	res.x = x;
	res.y = y;
	res.z = z;
	return res;
}

inline vector4 Vector4(){
	vector4 res;
	res.x = 0.0f;
	res.y = 0.0f;
	res.z = 0.0f;
	res.w = 0.0f;
	return res;
}

inline vector4 Vector4(float x, float y, float z, float w){
	vector4 res;
	res.x = x;
	res.y = y;
	res.z = z;
	res.w = w;
	return res;
}

inline vector4 Vector4(vector3 v, float w){
	vector4 res;
	res.x = v.x;
	res.y = v.y;
	res.z = v.z;
	res.w = w;
}

inline quaternion Quaternion(){
	quaternion Result;
	Result.x = 0.0f;
	Result.y = 0.0f;
	Result.z = 0.0f;
	Result.w = 1.0f;
	return(Result);
}

inline quaternion Quaternion(float x, float y, float z, float w){
	quaternion res;
	res.x = x;
	res.y = y;
	res.z = z;
	res.w = w;
	return res;
}

inline matrix4x4 Matrix4x4(){
	matrix4x4 Result = {};
	return(Result);
}

inline matrix4x4 Matrix4x4(float Value){
	matrix4x4 Result = {};
	*(Result.data + 0) = Value;
	*(Result.data + 5) = Value;
	*(Result.data + 10) = Value;
	*(Result.data + 15) = Value;
	return(Result);
}

//VECTOR2 OPERATIONS
inline float Length(vector2 vec){
	return sqrtf(vec.x * vec.x + vec.y * vec.y);
}

inline vector2 Normalize(vector2 vec){
	float len = Length(vec);
	vector2 res;
	res.x = vec.x / len;
	res.y = vec.y / len;
	return res;
}

inline float DotProduct(vector2 v1, vector2 v2){
	return (v1.x * v2.x + v1.y * v2.y);
}

//VECTOR3 OPERATIONS
inline float Length(vector3 vec){
	return sqrtf(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
}

inline vector3 Normalize(vector3 vec){
	float len = Length(vec);
	vector3 res;
	res.x = vec.x / len;
	res.y = vec.y / len;
	res.z = vec.z / len;
	return res;
}

inline float DotProduct(vector3 v1, vector3 v2){
	return (v1.x * v2.x + v1.y * v2.y + v1.z * v2.z);
}

inline vector3 CrossProduct(vector3 p1, vector3 p2){
	vector3 Result;

	Result.x = p1.y * p2.z - p2.y * p1.z;
	Result.y = p1.x * p2.z - p2.x * p1.z;
	Result.z = p1.x * p2.y - p2.x * p1.y;

	return(Result);
}



//VECTOR2 OPERATOR OVERLOADING
inline vector2 operator+(vector2 v1, vector2 v2){
	return Vector2(v1.x + v2.x, v1.y + v2.y);
}

inline vector2 operator-(vector2 v1, vector2 v2){
	return Vector2(v1.x - v2.x, v1.y - v2.y);
}

inline vector2 operator*(vector2 v, float f){
	return Vector2(v.x * f, v.y * f);
}

inline vector2 operator/(vector2 v, float f){
	return Vector2(v.x / f, v.y / f);
}

inline vector2& operator*=(vector2& v, float Value){
	v = v * Value;
	return(v);
}

inline vector2& operator/=(vector2& v, float Value){
	v = v / Value;
	return(v);
}

//VECTOR3 OPERATOR OVERLOADING
inline vector3 operator+(vector3 v1, vector3 v2){
	return Vector3(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z);
}

inline vector3 operator-(vector3 v1, vector3 v2){
	return Vector3(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z);
}

inline vector3 operator*(vector3 v, float f){
	return Vector3(v.x * f, v.y * f, v.z * f);
}

inline vector3 operator/(vector3 v, float f){
	return Vector3(v.x / f, v.y / f, v.z / f);
}

inline vector3& operator*=(vector3& v, float f){
	v = v * f;
	return(v);
}

inline vector3& operator/=(vector3& v, float f){
	v = v / f;
	return(v);
}

//VECTOR4 OPERATIONS
inline float DotProduct(vector4 v1, vector4 v2){

	float Result;

#ifndef GDA_MATH_ALLOW_SIMD
	Result = (v1.x * v2.x + v1.y * v2.y + v1.z * v2.z + v1.w * v2.w);
#else
	Result = (v1.x * v2.x + v1.y * v2.y + v1.z * v2.z + v1.w * v2.w);
#endif

	return(Result);
}


//QUATERNION OPERATIONS
inline float Length(quaternion q) {
	return sqrtf(q.x * q.x + q.y * q.y + q.z * q.z + q.w * q.w);
}

inline quaternion NormalizeQuaternion(quaternion quat){
	float length = Length(quat);

	float x = quat.x / length;
	float y = quat.x / length;
	float z = quat.x / length;
	float w = quat.x / length;
}

inline quaternion ConjugateQuaternion(quaternion quat){
	return Quaternion(-quat.x, -quat.y, -quat.z, quat.w);
}

inline quaternion MultiplyQuaternionByQuaternion(quaternion l, quaternion r){
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
	return Quaternion(x, y, z, w);
}

inline quaternion MultiplyQuaternionByVector(quaternion q, vector3 v){
	quaternion Result;
	Result.w = -q.x * v.x - q.y * v.y - q.z * v.z;
	Result.x = q.w * v.x + q.y * v.z - q.z * v.y;
	Result.y = q.w * v.y + q.z * v.x - q.x * v.z;
	Result.z = q.w * v.z + q.x * v.y - q.y * v.x;
	return(Result);
}

//MATRIX FUNCTIONS
inline matrix4x4 TransposeMatrix(matrix4x4 mat){
	matrix4x4 res;
	for (int i = 0; i < 4; i++){
		for (int j = 0; j < 4; j++){
			*(res.data + j * 4 + i) = *(mat.data + i * 4 + j);
		}
	}
}


//Maybe make SIMD version of those two
inline matrix4x4 MultiplyMatrices(matrix4x4 m1, matrix4x4 m2){
	matrix4x4 Result = {};
#ifndef GDA_MATH_ALLOW_SIMD
	for (int j = 0; j < 4; j++){
		for (int i = 0; i < 4; i++){

			vector4 SourceV = Vector4(
				*(m1.data + j * 4 + 0),
				*(m1.data + j * 4 + 1),
				*(m1.data + j * 4 + 2),
				*(m1.data + j * 4 + 3));
			vector4 DestV = Vector4(
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

inline vector4 MultiplyMatrixByVector4(matrix4x4 mat, vector4 vec){
	vector4 res;
	for (int j = 0; j < 4; j++){
		vector4 SourceV = Vector4(
			mat.data[j * 4 + 0],
			mat.data[j * 4 + 1],
			mat.data[j * 4 + 2],
			mat.data[j * 4 + 3]);

		res.data[j] = DotProduct(SourceV, vec);
	}
	return res;
}

inline matrix4x4 RotationMatrix(quaternion qu){
	matrix4x4 result;

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

inline matrix4x4 OrthograficProjectionMatrix(int Left, int Right, int Bottom, int Top){
	matrix4x4 Result = {};
	*(Result.data) = 2.0f / (Right - Left);
	*(Result.data + 3) = (Right + Left) / (Right - Left);

	*(Result.data + 5) = 2.0f / (Top - Bottom);
	*(Result.data + 7) = (Top + Bottom) / (Top - Bottom);

	*(Result.data + 10) = 1.0f;
	*(Result.data + 15) = 1.0f;
	return(Result);
}

inline matrix4x4 OrthograficProjectionMatrix(int Left, int Right, int Bottom, int Top, int Near, int Far){
	matrix4x4 Result = {};

	*(Result.data) = 2.0f / (Right - Left);
	*(Result.data + 3) = (Right + Left) / (Right - Left);

	*(Result.data + 5) = 2.0f / (Top - Bottom);
	*(Result.data + 7) = (Top + Bottom) / (Top - Bottom);

	*(Result.data + 10) = -2.0f / (Far - Near);
	*(Result.data + 11) = (Far + Near) / (Far - Near);

	*(Result.data + 15) = 1.0f;

	return(Result);
}

inline matrix4x4 PerspectiveProjectionMatrix(int Left, int Right, int Bottom, int Top, int Near, int Far){
	matrix4x4 Result;
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

inline matrix4x4 operator *(matrix4x4 m1, matrix4x4 m2){
	return MultiplyMatrices(m1, m2);
}

inline vector4 operator*(matrix4x4 mat, vector4 vec){
	return MultiplyMatrixByVector4(mat, vec);
}

#endif



#define GDA_MATH_H
#endif