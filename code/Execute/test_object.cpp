#pragma once
#include "test_object.hpp"
#include "../Model/model.hpp"
#include "../assimp/loader.hpp"

Test_Object::Test_Object(D3D* d3d)
	: Execute::I_Execute(d3d)
{
	character[0] = new Object();
	character[1] = new Object();
}

void Test_Object::init()
{
	
}

void Test_Object::update()
{
}

void Test_Object::render()
{
	
}

void Test_Object::destroy()
{
}
