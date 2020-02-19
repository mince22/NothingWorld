#pragma once

#include "../common.hpp"
#include "../Interface/i_component.hpp"
#include <unordered_map>

class Transform : public I_Component {
public:
	Transform(u32 object_id, Component_Type type);

public:
	const vec3 get_world_position() const;
	const vec3 get_world_scale() const;
	const vec3 fetch_world_euler() const;
	const quat fetch_world_quaternion() const;
	const mat4x4 get_world_matrix() const;

	const u32 get_owner_id() const;

	void set_transform_buffer_id(u32 id);

private:
	const vec3 get_world_forward(vec3& scale) const;
	const vec3 get_world_right(vec3& scale) const;
	const vec3 get_world_up(vec3& scale) const;

private:
	mat4x4 world_mat;
	mat4x4 local_mat;

	vec3 position;
	vec3 scale;
	quat quaternion;
	vec3 rotation;

	vector<u32> child_id;

	u32 transform_buffer_id;

	// I_Component을(를) 통해 상속됨
	virtual void init() override;
	virtual void update() override;
	virtual void render() override;
	virtual void destroy() override;
};

class Transform_Pool {
public:
	Transform_Pool() = default;
	
	//Disallow copy and assing.
	Transform_Pool(const Transform_Pool&) = delete;
	Transform_Pool& operator=(const Transform_Pool&) = delete;

	u32 add_transform(Transform* transform) {
		u32 owner_id = transform->get_owner_id();
		transform_pool.insert(unordered_map<u32, Transform *>::value_type(owner_id, transform));

		return 0;
	}

	u32 delete_transform(Transform* transform) {
		transform_pool.erase(transform->get_owner_id());
	}

	unordered_map<u32, Transform *> transform_pool;
};