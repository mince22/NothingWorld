#pragma once
#include "transform.hpp"
#include "../Interface/i_execute.hpp"

Transform::Transform()
{
	world_mat = new mat4x4();
	*world_mat = compute_local_matrix();

	world_mat_in_instance = false;

	parent_id = -1;
	
	dirty = false;

	Execute::transform_pool.add_transform(this);
}

Transform::~Transform()
{
	if (world_mat_in_instance == false)
		SAFE_DELETE(world_mat);
}

const vec3 Transform::get_world_position() const
{
	return vec3(world_mat->_41, world_mat->_42, world_mat->_43);
}

const vec3 Transform::get_world_scale() const
{
	return vec3(vec3_length(vec3(world_mat->_11, world_mat->_11, world_mat->_11)),
				vec3_length(vec3(world_mat->_11, world_mat->_11, world_mat->_11)),
				vec3_length(vec3(world_mat->_11, world_mat->_11, world_mat->_11)) );
}

const vec3 Transform::fetch_world_euler() const
{
	vec3 scale = get_world_scale();
	mat4x4 rot_mat = matrix_make_rotation(get_world_right(scale), get_world_up(scale), get_world_forward(scale));

	return vec3_rotation_from_matrix(rot_mat);
}

const quat Transform::fetch_world_quaternion() const
{
	vec3 scale = get_world_scale();
	mat4x4 rot_mat = matrix_make_rotation(get_world_right(scale), get_world_up(scale), get_world_forward(scale));

	return quat_rotation_from_matrix(rot_mat);
}

const mat4x4 Transform::get_world_matrix() const
{
	return *world_mat;
}

const u32 Transform::get_id() const
{
	return transform_id;
}

void Transform::set_id(u32 id)
{
	this->transform_id = id;
}

const mat4x4 & Transform::compute_local_matrix()
{
	local_mat._11 = scale.x * right.x;
	local_mat._12 = scale.y * right.y;
	local_mat._13 = scale.z * right.z;
	local_mat._14 = 0.0f;

	local_mat._21 = scale.x * up.x;
	local_mat._22 = scale.y * up.y;
	local_mat._23 = scale.z * up.z;
	local_mat._24 = 0.0f;

	local_mat._31 = scale.x * forward.x;
	local_mat._32 = scale.y * forward.y;
	local_mat._33 = scale.z * forward.z;
	local_mat._34 = 0.0f;

	local_mat._41 = position.x;
	local_mat._42 =	position.y;
	local_mat._43 =	position.z;
	local_mat._44 =	1.0f;

	return local_mat;
}

void Transform::world_mat_move_to_instance_pool(mat4x4 * mat_in_instance_pool)
{
	*mat_in_instance_pool = *world_mat;
	SAFE_DELETE(world_mat);
	world_mat = mat_in_instance_pool;
	world_mat_in_instance = true;
}

const vec3 Transform::get_world_forward(vec3 & scale) const
{
	vec3 forward = vec3(world_mat->_31 / scale.x, world_mat->_32 / scale.y, world_mat->_33 / scale.z);

	return vec3_get_normal(forward);
}

const vec3 Transform::get_world_right(vec3 & scale) const
{
	vec3 right = vec3(world_mat->_11 / scale.x, world_mat->_12 / scale.y, world_mat->_13 / scale.z);

	return vec3_get_normal(right);
}

const vec3 Transform::get_world_up(vec3 & scale) const
{
	vec3 up = vec3(world_mat->_21 / scale.x, world_mat->_22 / scale.y, world_mat->_23 / scale.z);

	return vec3_get_normal(up);
}
