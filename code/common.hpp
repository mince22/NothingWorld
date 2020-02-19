#pragma once

#define DEBUG

#include <stdint.h>
#include <string>
#include <array>
#include <vector>
#include <math.h>
#include <wrl.h>

typedef int8_t   s8;
typedef int16_t  s16;
typedef int32_t  s32;
typedef int64_t  s64;
typedef uint8_t  u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;
typedef float    f32;
typedef double   f64;

#define PI (3.14159265358979323846)
#define FORWARD { 0.0f, 0.0f, -1.0f }
#define RIGHT { -1.0f, 0.0f, 0.0f }
#define UP { 0.0f, 1.0f, 0.0f }

using namespace std;
using namespace Microsoft::WRL;

#define SAFE_DELETE(x)				{ if(x){ delete x; x = nullptr; } }
#define SAFE_DELETE_ARRAY(x)		{ if(x){ delete[] x; x = nullptr; } }
#define SAFE_RELEASE(x)				{ if(x){ x->Release(); x = nullptr; } }
#define SAFE_DESTROY_DELTE(x)		{ if(x){ x->Destroy(); delete x; x = nullptr; } }

union vec2 {
	struct { f32 x, y; };
	struct { f32 u, v; };

	vec2() {};
	vec2(f32 x, f32 y) : x(x), y(y) {};

	vec2 operator+(vec2 b) { vec2 result = { x + b.x, y + b.y }; return result; };
	vec2 operator-(vec2 b) { vec2 result = { x - b.x, y - b.y }; return result; };
	vec2 operator*(vec2 b) { vec2 result = { x * b.x, y * b.y }; return result; };
	vec2 operator/(vec2 b) { vec2 result = { x / b.x, y / b.y }; return result; };
};

union vec3 {
	struct { f32 x, y, z; };
	struct { f32 r, g, b; };

	vec3() : x(0.0f), y(0.0f), z(0.0f) {};
	vec3(f32 x, f32 y, f32 z) : x(x), y(y), z(z) {};

	vec3 operator+(vec3 b) { vec3 result = { x + b.x, y + b.y, z + b.z }; return result; };
	vec3 operator-(vec3 b) { vec3 result = { x - b.x, y - b.y, z - b.z }; return result; };
	
	vec3 operator*(vec3 b) { vec3 result = { x * b.x, y * b.y, z * b.z }; return result; };
	vec3 operator*(float b) { vec3 result = { x * b, y * b, z * b }; return result; };

	vec3 operator/(vec3 b) { vec3 result = { x / b.x, y / b.y, z / b.z }; return result; };
};

union vec4 {
	struct { f32 x, y, z, w; };
	struct { f32 r, g, b, a; };
	struct { vec3 xyz; };
	
	vec4() {};
	vec4(f32 x, f32 y, f32 z, f32 w) : x(x), y(y), z(z), w() {};

	float& operator[] (int index) {
		if (index < 0 || index > 3) return x;
		else if (index == 0) return x;
		else if (index == 1) return y;
		else if (index == 2) return z;
		else if (index == 3) return w;
	}
};

struct quat {
	f32 x; f32 y; f32 z; f32 w;

	quat() : x(0.0f), y(0.0f), z(0.0f), w(0.0f) {};
	quat(f32 x, f32 y, f32 z, f32 w) : x(x), y(y), z(z), w() {};
};

struct mat4x4 {
	f32 _11, _12, _13, _14, _21, _22, _23, _24, _31, _32, _33, _34, _41, _42, _43, _44;
	union
	{
		struct
		{
			float        _11, _12, _13, _14;
			float        _21, _22, _23, _24;
			float        _31, _32, _33, _34;
			float        _41, _42, _43, _44;
		};

		float _m[4][4] = { 0.0f, };
	};

	mat4x4() {};
	mat4x4(f32 _11, f32 _12, f32 _13, f32 _14
		, f32 _21, f32 _22, f32 _23, f32 _24
		, f32 _31, f32 _32, f32 _33, f32 _34
		, f32 _41, f32 _42, f32 _43, f32 _44)
		:_11(_11), _12(_12), _13(_13), _14(_14)
		, _21(_21), _22(_22), _23(_23), _24(_24)
		, _31(_31), _32(_32), _33(_33), _34(_34)
		, _41(_41), _42(_42), _43(_43), _44(_44) {};

	mat4x4 operator+(mat4x4 b) {
		mat4x4 result = {
			_11 + b._11, _12 + b._12, _13 + b._13, _14 + b._14
			, _21 + b._21, _22 + b._22, _23 + b._23, _24 + b._24
			, _31 + b._31, _32 + b._32, _33 + b._33, _34 + b._34
			, _41 + b._41, _42 + b._42, _43 + b._43, _44 + b._44
		};
		return result;
	}

	mat4x4 operator-(mat4x4 b) {
		mat4x4 result = {
			_11 - b._11, _12 - b._12, _13 - b._13, _14 - b._14
			, _21 - b._21, _22 - b._22, _23 - b._23, _24 - b._24
			, _31 - b._31, _32 - b._32, _33 - b._33, _34 - b._34
			, _41 - b._41, _42 - b._42, _43 - b._43, _44 - b._44
		};
		return result;
	}

	mat4x4 operator*(mat4x4 b) {
		mat4x4 result = {
			_11 * b._11 + _12 * b._21 + _13 * b._31 + _14 * b._41
			, _11 * b._12 + _12 * b._22 + _13 * b._32 + _14 * b._42
			, _11 * b._13 + _12 * b._23 + _13 * b._33 + _14 * b._43
			, _11 * b._14 + _12 * b._24 + _13 * b._34 + _14 * b._44

			, _21 * b._11 + _22 * b._21 + _23 * b._31 + _24 * b._41
			, _21 * b._12 + _22 * b._22 + _23 * b._32 + _24 * b._42
			, _21 * b._13 + _22 * b._23 + _23 * b._33 + _24 * b._43
			, _21 * b._14 + _22 * b._24 + _23 * b._34 + _24 * b._44

			, _31 * b._11 + _32 * b._21 + _33 * b._31 + _34 * b._41
			, _31 * b._12 + _32 * b._22 + _33 * b._32 + _34 * b._42
			, _31 * b._13 + _32 * b._23 + _33 * b._33 + _34 * b._43
			, _31 * b._14 + _32 * b._24 + _33 * b._34 + _34 * b._44

			, _41 * b._11 + _42 * b._21 + _43 * b._31 + _44 * b._41
			, _41 * b._12 + _42 * b._22 + _43 * b._32 + _44 * b._42
			, _41 * b._13 + _42 * b._23 + _43 * b._33 + _44 * b._43
			, _41 * b._14 + _42 * b._24 + _43 * b._34 + _44 * b._44
		};
		return result;
	}
};

f32 vec3_dot(const vec3& a, const vec3& b) {
	f32 result = a.x * b.x + a.y * b.y + a.z * b.z;

	return result;
}

vec3 vec3_cross(const vec3 a, const vec3 b) {
	vec3 result = { 
		a.y * b.z - a.z * b.y
		, a.z * b.x - a.x * b.z
		, a.x * b.y - a.y * b.x };

	return result;
}

f32 vec3_length(const vec3& a) {
	f32 length = sqrtf((a.x * a.x) + (a.y * a.y) + (a.z * a.z));

	return length;
}

vec3 vec3_get_normal(const vec3& a) {
	f32 length = vec3_length(a);
	vec3 normal = { a.x / length, a.y / length, a.z / length };

	return normal;
}

vec3 vec3_normalize(_Outptr_ vec3 *a) {
	f32 length = vec3_length(*a);
	a->x = a->x / length;
	a->y = a->y / length;
	a->z = a->z / length;

	return *a;
}

vec3 vec3_euler_normalize(const vec3& euler)
{
	int x = (int)(euler.x / 360.0f);
	int y = (int)(euler.y / 360.0f);
	int z = (int)(euler.z / 360.0f);

	return vec3(euler.x - x * 360.0f + (int)(euler.x < 0) * 360.0f,
				euler.y - y * 360.0f + (int)(euler.y < 0) * 360.0f,
				euler.z - z * 360.0f + (int)(euler.z < 0) * 360.0f);
}

vec3 vec3_euler_normalize(_Outptr_ vec3* euler)
{
	int x = (int)(euler->x / 360.0f);
	int y = (int)(euler->y / 360.0f);
	int z = (int)(euler->z / 360.0f);

	euler->x = euler->x - x * 360.0f + (int)(euler->x < 0) * 360.0f;
	euler->y = euler->y - y * 360.0f + (int)(euler->y < 0) * 360.0f;
	euler->z = euler->z - z * 360.0f + (int)(euler->z < 0) * 360.0f;

	return *euler;
}

vec3 vec3_transform_normal(const vec3 vec, const mat4x4 mat) {
	vec3 result = {
		mat._11 * vec.x + mat._12 * vec.y + mat._13 * vec.z
		, mat._21 * vec.x + mat._22 * vec.y + mat._23 * vec.z
		, mat._31 * vec.x + mat._32 * vec.y + mat._33 * vec.z};

	return result;
}

//http://www.gamedev.net/topic/643623-how-do-i-get-the-euler-angle-from-a-matrix/
vec3 vec3_rotation_from_matrix(const mat4x4& mat) {
	vec3 dotx = vec3(mat._11, mat._12, mat._13);
	vec3 doty = vec3(mat._21, mat._22, mat._23);
	vec3 dotz = vec3(mat._31, mat._32, mat._33);


	auto approximately = [](float a, float b) -> bool
	{
		return fabsf(a - b) < FLT_EPSILON;
	};

	float x = vec3_dot(dotx, dotx);
	if (approximately(x, 1.0f) == false)
	{
		float invx = 1.0f / sqrtf(x);
		dotx = dotx * invx;
	}

	float y = vec3_dot(doty, doty);
	if (approximately(y, 1.0f) == false)
	{
		float invy = 1.0f / sqrtf(y);
		doty = doty * invy;
	}

	float z = vec3_dot(dotz, dotz);
	if (approximately(z, 1.0f) == false)
	{
		float invz = 1.0f / sqrtf(z);
		dotz = dotz * invz;
	}

	float theta_x = 0;
	float theta_y = 0;
	float theta_z = 0;

	if (dotz.y < 1.0f)
	{
		if (dotz.y > -1.0f)
		{
			theta_x = asinf(-dotz.y);
			theta_y = atan2f(dotz.x, dotz.z);
			theta_z = atan2f(dotx.y, doty.y);
		}
		else
		{
			theta_x = PI * 0.5f;
			theta_y = -atan2f(-doty.x, dotx.x);
			theta_z = 0;
		}
	}
	else
	{
		theta_x = -PI * 0.5f;
		theta_y = atan2f(-doty.x, dotx.x);
		theta_z = 0;
	}

	return vec3_euler_normalize(vec3(theta_x, theta_y, theta_z));
}

///////////////////////////////////////////////////////////////////////////////////////////////
//Quaternion
///////////////////////////////////////////////////////////////////////////////////////////////
quat quat_rotation_from_matrix(const mat4x4& mat) {
	quat result;

	float s = 0.0f;

	float trace = mat._m[0][0] + mat._m[1][1] + mat._m[2][2] + 1.0f;
	if (trace > 1.0f)
	{
		s = 2.0f * sqrtf(trace);
		result.x = (mat._m[1][2] - mat._m[2][1]) / s;
		result.y = (mat._m[2][0] - mat._m[0][2]) / s;
		result.z = (mat._m[0][1] - mat._m[1][0]) / s;
		result.w = 0.25f * s;
	}
	else
	{
		int i, maxi = 0;

		for (i = 1; i < 3; i++)
		{
			if (mat._m[i][i] > mat._m[maxi][maxi])
				maxi = i;
		}

		switch (maxi)
		{
		case 0:
			s = 2.0f * sqrtf(1.0f + mat._m[0][0] - mat._m[1][1] - mat._m[2][2]);
			result.x = 0.25f * s;
			result.y = (mat._m[0][1] + mat._m[1][0]) / s;
			result.z = (mat._m[0][2] + mat._m[2][0]) / s;
			result.w = (mat._m[1][2] - mat._m[2][1]) / s;
			break;

		case 1:
			s = 2.0f * sqrtf(1.0f + mat._m[1][1] - mat._m[0][0] - mat._m[2][2]);
			result.x = (mat._m[0][1] + mat._m[1][0]) / s;
			result.y = 0.25f * s;
			result.z = (mat._m[1][2] + mat._m[2][1]) / s;
			result.w = (mat._m[2][0] - mat._m[0][2]) / s;
			break;

		case 2:
			s = 2.0f * sqrtf(1.0f + mat._m[2][2] - mat._m[0][0] - mat._m[1][1]);
			result.x = (mat._m[0][2] + mat._m[2][0]) / s;
			result.y = (mat._m[1][2] + mat._m[2][1]) / s;
			result.z = 0.25f * s;
			result.w = (mat._m[0][1] - mat._m[1][0]) / s;
			break;
		}
	}

	return result;
}

mat4x4 matrix_identity(_Outptr_ mat4x4 *mat) {
	memset(mat, 0, sizeof(f32) * 16);

	mat->_11 = mat->_22 = mat->_33 = mat->_44 = 1.0f;

	return *mat;
}

mat4x4* matrix_transpose(_Outptr_ mat4x4* result, mat4x4* source) {
	mat4x4 transpose = {
		source->_11, source->_21, source->_31, source->_41
		, source->_12, source->_22, source->_32, source->_42
		, source->_13, source->_23, source->_33, source->_43
		, source->_14, source->_24, source->_34, source->_44
	};
	*result = transpose;

	return result;
}

mat4x4 matrix_rotation_x(_Outptr_ mat4x4* mat, f32 radian) {
	matrix_identity(mat);

	mat->_22 = cosf(radian);
	mat->_23 = -sinf(radian);
	mat->_32 = sinf(radian);
	mat->_33 = cosf(radian);

	return *mat;
}

mat4x4 matrix_rotation_y(_Outptr_ mat4x4 *mat, f32 radian) {
	matrix_identity(mat);

	mat->_11 = cosf(radian);
	mat->_13 = sinf(radian);
	mat->_31 = -sinf(radian);
	mat->_33 = cosf(radian);

	return *mat;
}

mat4x4 matrix_rotation_z(_Outptr_ mat4x4 *mat, f32 radian) {
	matrix_identity(mat);

	mat->_11 = cosf(radian);
	mat->_12 = -sinf(radian);
	mat->_21 = sinf(radian);
	mat->_22 = cosf(radian);

	return *mat;
}


mat4x4 get_matrix_perspective_fov_LH(_Outptr_ mat4x4 *mat, f32 fovY, f32 aspect, f32 zn, f32 zf) {
	memset(mat, 0, sizeof(mat4x4));

	f32 height = 1.0f / tanf(fovY / 2.0f);
	f32 width = height / aspect;

	mat->_11 = width;
	mat->_22 = height;
	mat->_33 = zf / (zf - zn);
	mat->_34 = 1.0f;
	mat->_43 = -zn * zf / (zf - zn);

	return *mat;
}

mat4x4 get_matrix_perspective_fov_LH(_Outptr_ mat4x4 *mat, f32 width, f32 height, f32 fovY, f32 zn, f32 zf) {
	memset(mat, 0, sizeof(mat4x4));

	f32 aspect = height / width;

	mat->_11 = width;
	mat->_22 = height;
	mat->_33 = zf / (zf - zn);
	mat->_34 = 1.0f;
	mat->_43 = -zn * zf / (zf - zn);

	return *mat;
}

mat4x4 matrix_look_at_left_hand(const vec3 eye_position, const vec3 look_at, const vec3 up) {
	mat4x4 mat_eye;

	vec3 look = look_at;
	vec3 eye = eye_position;

	vec3 axis_z = vec3_get_normal(look - eye);

	vec3 axis_x = vec3_cross(up, axis_z);
	vec3_normalize(&axis_x);

	vec3 axis_y = vec3_cross(axis_z, axis_x);

	matrix_identity(&mat_eye);

	mat_eye._11 = axis_x.x;
	mat_eye._21 = axis_x.y;
	mat_eye._31 = axis_x.z;

	mat_eye._12 = axis_y.x;
	mat_eye._22 = axis_y.y;
	mat_eye._32 = axis_y.z;

	mat_eye._13 = axis_z.x;
	mat_eye._23 = axis_z.y;
	mat_eye._33 = axis_z.z;

	mat_eye._41 = -vec3_dot(axis_x, eye);
	mat_eye._42 = -vec3_dot(axis_y, eye);
	mat_eye._43 = -vec3_dot(axis_z, eye);

	return mat_eye;
}

mat4x4 matrix_make_rotation(const vec3& right, const vec3& up, const vec3& forward) {
	return mat4x4(	right.x,	right.y,	right.z,	0.0f
				,	up.x,		up.y,		up.z,		0.0f
				,	forward.x,	forward.y,	forward.z,	0.0f
				,	0.0f,		0.0f,		0.0f,		0.0f);
}

#define safe_release(com) com->Release(); com = nullptr;
#define safe_delete(ptr) delete ptr; ptr = nullptr;

#define zero_struct(instance) zero_size(sizeof(*instance), (instance))
#define zero_array(count, ptr) zero_size(count*sizeof((ptr)[0]), ptr)

void zero_size(size_t size, void *ptr) {
    auto byte = (u8*)ptr;
    while(size--) {
        *byte++ = 0;
    }
}
