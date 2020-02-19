#pragma once
#include "../Component/Transform.hpp"
#include "../Device/d3d.hpp"
#include "../Object/object.hpp"

namespace Execute {

	Transform_Pool transform_pool;
	vector<Object *> object_pool;
	

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
