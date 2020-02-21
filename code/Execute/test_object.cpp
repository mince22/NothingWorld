#pragma once
#include "test_object.hpp"
#include "../Model/model.hpp"
#include "../assimp/loader.hpp"
#include "../Component/model_component.hpp"

Test_Object::Test_Object(D3D* d3d)
	: Execute::I_Execute(d3d)
{
	renderer = new Render_System::Renderer();

	world_buffer = new World_Buffer(d3d->get_device(), d3d->get_context());
	eye_buffer = new Eye_Buffer(d3d->get_device(), d3d->get_context(), { 0.0f, 200.0f, -500.0f });
	perspective_buffer = new Perspective_Buffer(d3d->get_device(), 1280.0f, 720.0f);

	character[1] = new Object();
	character[0] = new Object();
	character[2] = new Object();
}


void Test_Object::init()
{
	renderer->init(this->d3d);

	/*character[0]->add_component<Model_Component>()->init_model(d3d, L"Kachujin");
	character[1]->add_component<Model_Component>()->init_model(d3d, L"Kachujin");
	character[2]->add_component<Model_Component>()->init_model(d3d, L"Kachujin");*/

	Model_Component* model;
	model = character[0]->add_component<Model_Component>();
	model->init_model(d3d, L"Kachujin");
	model = character[1]->add_component<Model_Component>();
	model->init_model(d3d, L"Kachujin");
	model = character[2]->add_component<Model_Component>();
	model->init_model(d3d, L"Kachujin");
}

void Test_Object::update()
{
}

void Test_Object::render()
{
	/*Shader* test_model_shader = Render_System::shader_pool.begin()->second;
	test_model_shader->render();*/
	d3d->get_context()->VSSetConstantBuffers(0, 1, &world_buffer->buffer);
	d3d->get_context()->VSSetConstantBuffers(1, 1, &eye_buffer->buffer);
	d3d->get_context()->VSSetConstantBuffers(2, 1, &perspective_buffer->buffer);
	renderer->render();
}

void Test_Object::destroy()
{
	renderer->destroy();
	SAFE_DELETE(renderer);

	for (auto obj : Execute::object_pool_index)
	{
		if (obj.second != nullptr)
			obj.second->destroy();
		SAFE_DELETE(obj.second);
	}

	cleanup_buffer(perspective_buffer->buffer);
	cleanup_buffer(eye_buffer->buffer);
	cleanup_buffer(world_buffer->buffer);

	SAFE_DELETE(perspective_buffer);
	SAFE_DELETE(eye_buffer);
	SAFE_DELETE(world_buffer);
}
