#pragma once
#include "Transform.hpp"
#include "../Interface/i_execute.hpp"

Transform::Transform(u32 object_id, Component_Type type)
	: I_Component(object_id, type)
{
	Execute::transform_pool.add_transform(this);
}

const vec3 Transform::get_world_position() const
{
	return vec3(world_mat._41, world_mat._42, world_mat._43);
}

const vec3 Transform::get_world_scale() const
{
	return vec3(vec3_length(vec3(world_mat._11, world_mat._11, world_mat._11)),
				vec3_length(vec3(world_mat._11, world_mat._11, world_mat._11)),
				vec3_length(vec3(world_mat._11, world_mat._11, world_mat._11)) );
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
	return world_mat;
}

const u32 Transform::get_owner_id() const
{
	return owner_id;
}

void Transform::set_transform_buffer_id(u32 id)
{
	transform_buffer_id = id;
}

const vec3 Transform::get_world_forward(vec3 & scale) const
{
	vec3 forward = vec3(world_mat._31 / scale.x, world_mat._32 / scale.y, world_mat._33 / scale.z);

	return vec3_get_normal(forward);
}

const vec3 Transform::get_world_right(vec3 & scale) const
{
	vec3 right = vec3(world_mat._11 / scale.x, world_mat._12 / scale.y, world_mat._13 / scale.z);

	return vec3_get_normal(right);
}

const vec3 Transform::get_world_up(vec3 & scale) const
{
	vec3 up = vec3(world_mat._21 / scale.x, world_mat._22 / scale.y, world_mat._23 / scale.z);

	return vec3_get_normal(up);
}

void Transform::init()
{
}

void Transform::update()
{
}

void Transform::render()
{
}

void Transform::destroy()
{
}
