#pragma once

#include "../common.hpp"
#include "../Interface/i_component.hpp"
#include <unordered_map>

class Transform {
public:
	Transform();
	~Transform();

public:
	const vec3 get_world_position() const;
	const vec3 get_world_scale() const;
	const vec3 fetch_world_euler() const;
	const quat fetch_world_quaternion() const;
	const mat4x4 get_world_matrix() const;
	const u32 get_id() const;

	void set_id(u32 id);

	const mat4x4& compute_local_matrix();

	void world_mat_move_to_instance_pool(mat4x4* mat_in_instance_pool);

private:
	const vec3 get_world_forward(vec3& scale) const;
	const vec3 get_world_right(vec3& scale) const;
	const vec3 get_world_up(vec3& scale) const;

private:
	bool world_mat_in_instance;
	mat4x4* world_mat;
	mat4x4* parent_mat;
	mat4x4 local_mat;

	vec3 position = vec3(0.0f, 0.0f, 0.0f);
	vec3 scale = vec3(1.0f, 1.0f, 1.0f);
	quat quaternion = quat(0.0f, 0.0f, 0.0f, 0.0f);
	vec3 rotation = vec3(0.0f, 0.0f, 0.0f);

	vec3 forward = vec3(0.0f, 0.0f, 1.0f);
	vec3 up = vec3(0.0f, 1.0f, 0.0f);
	vec3 right = vec3(1.0f, 0.0f, 0.0f);

	vector<u32> child_id;
	u32 parent_id;

	u32 transform_id;

	bool dirty;
};

class Transform_Pool {
public:
	//Transform pool destructor only delete transform not include model instance pool; 
	~Transform_Pool() {
		for (auto tr : pool)
			SAFE_DELETE(tr.second);
	}

	Transform* get_transform(u32 id) {
		unordered_map<u32, Transform *>::iterator it;
		it = pool.find(id);
		assert(it != pool.end());	//Invalid ID

		return it->second;
	};

	void add_transform(Transform* tr) {
		assert(pool.find(current_index) == pool.end());	//Invalid current index

		pool.insert(make_pair(current_index, tr));
		tr->set_id(current_index);

		if (delete_index.size() > 0)
		{
			current_index = delete_index[delete_index.size() - 1];
			delete_index.pop_back();
		}
		else
			current_index++;
	};

	void remove_transform(Transform* tr) {
		u32 id = tr->get_id();

		unordered_map<u32, Transform *>::iterator it;
		it = pool.find(id);
		assert(it != pool.end());	//Invalid ID
		
		current_index = id;

		pool.erase(it);
	};

private:
	unordered_map<u32, Transform *> pool;
	u32 current_index = 0;
	vector<u32> delete_index;
};