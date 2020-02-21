#pragma once

#include "../common.hpp"

#include <d3d11_1.h>


ID3D11Buffer* create_constant_buffer(ID3D11Device1 *device, void *data, u32 size) {
	//Fill in a description for constant buffer.
	D3D11_BUFFER_DESC desc;
	desc.ByteWidth = size;
	desc.Usage = D3D11_USAGE_DYNAMIC;
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	desc.MiscFlags = 0;
	desc.StructureByteStride = 0;

	//Fill in a data of subresource.
	D3D11_SUBRESOURCE_DATA init_data;
	init_data.pSysMem = data;
	init_data.SysMemPitch = 0;
	init_data.SysMemSlicePitch = 0;

	ID3D11Buffer* constant_buffer;

	HRESULT hr = device->CreateBuffer(&desc, &init_data, &constant_buffer);

	if (FAILED(hr)) {
		//printf("Creating constant buffer failed.");
	}

	return constant_buffer;
}

void write_data_constant_buffer(ID3D11DeviceContext1 *context, ID3D11Buffer *buffer, void *data, u32 size) {
	D3D11_MAPPED_SUBRESOURCE subResource;

	HRESULT hr = context->Map
	(
		buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &subResource
	);

	memcpy(subResource.pData, data, size);

	context->Unmap(buffer, 0);
}

struct World_Buffer {
	ID3D11Device1 *device;
	ID3D11DeviceContext1 *context;

	mat4x4 mat;
	ID3D11Buffer *buffer;

	World_Buffer(ID3D11Device1 *device, ID3D11DeviceContext1 *context)
	: device(device), context(context) {
		matrix_identity(&mat);

		mat4x4 transpose_mat;
		matrix_transpose(&transpose_mat, &mat);

		buffer = create_constant_buffer(device, &transpose_mat, sizeof(mat4x4));
	}

	void set_position(vec3 pos) {
		mat._41 = pos.x;
		mat._42 = pos.y;
		mat._43 = pos.z;

		mat4x4 transpose_mat;
		matrix_transpose(&transpose_mat, &mat);
		buffer->Release();
		buffer = create_constant_buffer(device, &transpose_mat, sizeof(mat4x4));
	}

	void cleanup() {
		if (buffer) {
			buffer->Release();
			buffer = nullptr;
		}
	}
};

struct Perspective_Buffer {
	ID3D11Device1 *device;
	ID3D11DeviceContext1 *context;

	ID3D11Buffer *buffer;

	mat4x4 mat;

	f32 width;
	f32 height;
	f32 fov;
	f32 zn;
	f32 zf;

	Perspective_Buffer(ID3D11Device1 *device, f32 width, f32 height, f32 fov = (f32)PI * 0.25f, f32 zn = 0.1f, f32 zf = 1000.0f)
		:device(device), width(width), height(height), fov(fov), zn(zn), zf(zf) {
		
		//get_matrix_perspective_fov_LH(&mat, width, height, fov, zn, zf);
		get_matrix_perspective_fov_LH(&mat, fov, width / height, zn, zf);

		mat4x4 transpose_mat;
		matrix_transpose(&transpose_mat, &mat);

		buffer = create_constant_buffer(device, &transpose_mat, sizeof(mat4x4));
	}

	void set_perspective(f32 width, f32 height, f32 fov = (f32)PI * 0.25f, f32 zn = 0.1f, f32 zf = 1000.0f) {
		this->width = width;
		this->height = height;
		this->fov = fov;
		this->zn = zn;
		this->zf = zf;

		get_matrix_perspective_fov_LH(&mat, width, height, fov, zn, zf);

		if (buffer)
			buffer->Release();

		buffer = create_constant_buffer(device, &mat, sizeof(mat4x4));
	}

	void cleanup() {
		if (buffer) {
			buffer->Release();
			buffer = nullptr;
		}
	}
};

struct Eye_Buffer {
	ID3D11Device1 *device;
	ID3D11DeviceContext1 *context;

	ID3D11Buffer *buffer;

	vec3 position;
	vec2 rotation;

	vec3 forward, right, up;

	mat4x4 mat_eye;

	Eye_Buffer(ID3D11Device1 *device, ID3D11DeviceContext1 *context, vec3 pos = { 0.0f, 0.0f, 0.0f }, vec2 rot = { 0.0f, 0.0f })
		: device(device), forward({ 0.0f, 0.0f, 1.0f }), right({ 1.0f, 0.0f, 0.0f }), up({ 0.0f, 1.0f, 0.0f })
		, position(pos), rotation(rot), context(context) {
		
		matrix_identity(&mat_eye);

		caculate_eye_mat();

		mat4x4 transpose_mat;
		matrix_transpose(&transpose_mat, &mat_eye);

		buffer = create_constant_buffer(device, &transpose_mat, sizeof(mat4x4));
	}

	void caculate_eye_mat(vec3 eye_position, vec3 look_at, vec3 up) {
		mat_eye = matrix_look_at_left_hand(eye_position, look_at, up);
	}

	void caculate_eye_mat() {
		mat_eye = matrix_look_at_left_hand(position, position + forward, up);
	}

	void move_forward(float scale) {
		position = position + (forward * scale);
	}

	void move_right(float scale) {
		position = position + (right * scale);
	}

	void move_to_position(vec3 position) {
		this->position = position;
	}

	void change_rotation(f32 left_right_radian, f32 up_down_radian) {
		mat4x4 rot_x, rot_y, rot_total;
		matrix_identity(&rot_total);

		rotation.x = left_right_radian;
		rotation.y = up_down_radian;

		matrix_rotation_x(&rot_x, rotation.x);
		matrix_rotation_y(&rot_y, rotation.y);

		rot_total = rot_x * rot_y;

		forward = vec3_transform_normal(FORWARD, rot_total);
		right = vec3_transform_normal(RIGHT, rot_total);
		up = vec3_transform_normal(UP, rot_total);
	}

	//If there is difference between current pos, rot from previous data, update eye matrix and buffer.
	void update_matrix() {
		static vec3 prev_position = position;
		static vec2 prev_rotation = rotation;

		if (prev_position.x != position.x || prev_position.y != position.y || prev_position.z != position.z) {
			caculate_eye_mat();

			mat4x4 transpose_mat;
			matrix_transpose(&transpose_mat, &mat_eye);

			write_data_constant_buffer(context, buffer, &transpose_mat, (u32)sizeof(mat4x4));
		}

		if (prev_rotation.x != rotation.x || prev_rotation.y != rotation.y) {
			caculate_eye_mat();
			
			mat4x4 transpose_mat;
			matrix_transpose(&transpose_mat, &mat_eye);

			write_data_constant_buffer(context, buffer, &transpose_mat, (u32)sizeof(mat4x4));
		}
	}
};

void cleanup_buffer(ID3D11Buffer *buffer) {
	if (buffer)
		buffer->Release();
}