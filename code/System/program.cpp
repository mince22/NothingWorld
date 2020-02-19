#pragma once

#include "program.hpp"
#include "../Interface/i_execute.hpp"

#include "../Execute/test_model.h"
#include "../Execute/test_object.hpp"

Program::Program(D3D* d3d)
	:d3d(d3d)
{
	//test_model = new Test_Model(d3d);
	test_object = new Test_Object(d3d);
	
	executes.emplace_back(test_object);
}

void Program::init()
{
	for (auto exe : executes)
		exe->init();
}

void Program::update()
{
	for (auto exe : executes)
		exe->update();
}

void Program::render()
{
	for (auto exe : executes)
		exe->render();
}

void Program::destroy()
{
	for (auto exe : executes)
		exe->destroy();
}
