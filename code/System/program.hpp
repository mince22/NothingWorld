#pragma once

#include "../common.hpp"

class Program
{
public:
	Program(D3D* d3d);

	void init();
	void update();
	void render();
	void destroy();

private:
	vector<Execute::I_Execute *> executes;
	class D3D* d3d;

	class Test_Model* test_model;
	class Test_Object* test_object;
};