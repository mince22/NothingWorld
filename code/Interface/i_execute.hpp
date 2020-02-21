#pragma once
#include "../Component/Transform.hpp"
#include "../Device/d3d.hpp"
#include "../Object/object.hpp"
#include "../Object/object_id.hpp"

namespace Execute {
	unordered_map<wstring, Object *> object_pool_name;
	unordered_map<u32, Object *> object_pool_index;
	Transform_Pool transform_pool;
	

	void add_object_pool(Object* object) {
		Object_ID id = object->get_id();
		object_pool_index.insert(make_pair(id.get_id(), object));
		object_pool_name.insert(make_pair(id.get_name(), object));
	}


	class I_Execute
	{
	public:
		I_Execute(class D3D* d3d) : d3d(d3d) {	};

		virtual void init() = 0;
		virtual void update() = 0;
		virtual void render() = 0;
		virtual void destroy() = 0;

	protected:
		D3D* d3d;

		ID3D11Buffer* world_view_projection[3];
	};
}
