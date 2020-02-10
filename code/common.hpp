#pragma once

#include <stdint.h>
#include <string>
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

	quat(f32 x, f32 y, f32 z, f32 w) : x(x), y(y), z(z), w() {};
};

struct mat4x4 {
	f32 _11, _12, _13, _14, _21, _22, _23, _24, _31, _32, _33, _34, _41, _42, _43, _44;

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

f32 vec3_dot(const vec3 a, const vec3 b) {
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

f32 vec3_length(const vec3 a) {
	f32 length = sqrtf((a.x * a.x) + (a.y * a.y) + (a.z * a.z));

	return length;
}

vec3 vec3_get_normal(const vec3 a) {
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

vec3 vec3_transform_normal(const vec3 vec, const mat4x4 mat) {
	vec3 result = {
		mat._11 * vec.x + mat._12 * vec.y + mat._13 * vec.z
		, mat._21 * vec.x + mat._22 * vec.y + mat._23 * vec.z
		, mat._31 * vec.x + mat._32 * vec.y + mat._33 * vec.z};

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
