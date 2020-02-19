#pragma once
#include "test_model.h"
#include "../Model/model.hpp"
#include "../assimp/loader.hpp"
#include "../Device/d3d.hpp"

Test_Model::Test_Model(D3D* d3d)
	: Execute::I_Execute(d3d)
{
	Assimp_Loader* model_loader = new Assimp_Loader(L"_asset/fbx_model/Paladin/Paladin.fbx", L"_model/Paladin/", L"Paladin");
	model_loader->export_material();
	model_loader->export_mesh();
	model_loader->destroy();
	SAFE_DELETE(model_loader);
}

void Test_Model::init()
{
	model = new Model(d3d);
	model->load_model(L"Kachujin");

	paladin = new Model(d3d, model->get_shader());
	paladin->load_model(L"Paladin");

	world_buffer = new World_Buffer(d3d->get_device(), d3d->get_context());
	perspective_buffer = new Perspective_Buffer(d3d->get_device(), 1280, 720);
	eye_buffer = new Eye_Buffer(d3d->get_device(), d3d->get_context(), { 0.0f, 200.0f, -500.0f });

	world_view_projection[0] = world_buffer->buffer;
	world_view_projection[1] = eye_buffer->buffer;
	world_view_projection[2] = perspective_buffer->buffer;
	d3d->get_context()->VSSetConstantBuffers(0, 3, world_view_projection);
}

void Test_Model::update()
{
}

void Test_Model::render()
{
	world_buffer->set_position(vec3(0,0,0));
	world_view_projection[0] = world_buffer->buffer;
	d3d->get_context()->VSSetConstantBuffers(0, 3, world_view_projection);
	model->render();

	world_buffer->set_position(vec3(50, 50, 50));
	world_view_projection[0] = world_buffer->buffer;
	d3d->get_context()->VSSetConstantBuffers(0, 3, world_view_projection);
	paladin->render();
}

void Test_Model::destroy()
{
	paladin->destroy();
	model->destroy();

	for (int ii = 0; ii < 3; ii++)
		cleanup_buffer(world_view_projection[ii]);
}
