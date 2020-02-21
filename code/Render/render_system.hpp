#pragma once
#include "../common.hpp"
#include "shader.hpp"
#include "../Device/d3d.hpp"
#include "../Model/model.hpp"


namespace Render_System
{
	unordered_map<wstring, Shader *> shader_pool;
	unordered_map<wstring, Model *> model_pool;
	unordered_map<wstring, mat4x4 *> model_instance_pool;
	unordered_map<wstring, bool *> model_instance_index_list;

	class Renderer {
	public:
		void init(D3D* d3d);
		void render();
		void destroy();

	private:
		void model_render();

		D3D* d3d;
	};
}