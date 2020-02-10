#pragma once

#include "../common.hpp"

class BinaryWriter
{
public:
	BinaryWriter();
	~BinaryWriter();

	void Open(wstring filePath, UINT openOption = CREATE_ALWAYS);
	void Close();

	void Bool(bool data);
	void Word(WORD data);
	void Int(int data);
	void UInt(UINT data);
	void Float(float data);
	void Double(double data);

	void Vector2(const vec2& data);
	void Vector3(const vec3& data);
	void Vector4(const vec4& data);
	//void Color3f(const D3DXCOLOR& data);
	//void Color4f(const D3DXCOLOR& data);
	void Matrix(const mat4x4& data);

	void String(const string& data);
	void Byte(void* data, UINT dataSize);

protected:
	HANDLE fileHandle;
	DWORD size;
};

//////////////////////////////////////////////////////////////////////////

class BinaryReader
{
public:
	BinaryReader();
	~BinaryReader();

	void Open(wstring filePath);
	void Close();

	bool Bool();
	WORD Word();
	int Int();
	UINT UInt();
	float Float();
	double Double();

	vec2 Vector2();
	vec3 Vector3();
	vec4 Vector4();
	//D3DXCOLOR Color3f();
	//D3DXCOLOR Color4f();
	mat4x4 Matrix();

	string String();
	void Byte(void** data, UINT dataSize);

protected:
	HANDLE fileHandle;
	DWORD size;
};