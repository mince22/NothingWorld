#pragma once
#include "render_system.hpp"

void Render_System::Renderer::init(D3D* d3d)
{
	this->d3d = d3d;

	for (u32 ii = 0; ii < u32(Shader_Type::Max); ii++)
	{
		Shader* shader = new Shader(d3d->get_device(), d3d->get_context());
		shader->create_shader(Shader_Type(ii));
		shader_pool.insert(unordered_map<wstring, Shader *>::value_type(shader_type_name[ii], shader));
	}
}

void Render_System::Renderer::render()
{
	model_render();
}

void Render_System::Renderer::destroy()
{
	unordered_map<wstring, Shader*>::iterator shader_it;
	for (shader_it = shader_pool.begin(); shader_it != shader_pool.end(); shader_it++)
	{
		shader_it->second->destroy();
		SAFE_DELETE(shader_it->second);
	}

	unordered_map<wstring, Model *>::iterator model_it;
	for (model_it = model_pool.begin(); model_it != model_pool.end(); model_it++)
	{
		model_it->second->destroy();
		SAFE_DELETE(model_it->second);
	}

	for (auto instance_pool : model_instance_pool)
		SAFE_DELETE_ARRAY(instance_pool.second);

	for (auto instance_index_list : model_instance_index_list)
		SAFE_DELETE_ARRAY(instance_index_list.second);
	
}

void Render_System::Renderer::model_render()
{
	Shader* model_shader = Render_System::shader_pool.find(L"model_instance")->second;
	model_shader->render();
	for (auto model : Render_System::model_pool)
	{
		model.second->render();
	}
}
