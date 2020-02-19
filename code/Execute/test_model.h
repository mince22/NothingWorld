#pragma once
#include "../common.hpp"
#include "../Interface/i_execute.hpp"
#include "../Render/constant_buffer.hpp"

class Test_Model : public Execute::I_Execute {
public:
	//Test_Model() = default;
	Test_Model(D3D* d3d);

	// Execute��(��) ���� ��ӵ�
	virtual void init() override;
	virtual void update() override;
	virtual void render() override;
	virtual void destroy() override;

private:
	class Model* model;
	Model* paladin;

	World_Buffer* world_buffer;
	Perspective_Buffer* perspective_buffer;
	Eye_Buffer* eye_buffer;
};