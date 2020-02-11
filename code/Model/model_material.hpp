#pragma once

#include "../common.hpp"
#include "../assimp/assimp_type.hpp"

#include <d3d11_1.h>

class Model_Material {
public:
	Model_Material();
	Model_Material(ID3D11Device1* device, Assimp_Matrial& material);

	void destroy();

private:
	string name;

	ID3D11Resource* diffuse_map;
	ID3D11ShaderResourceView* diffuse_map_srv;
	vec4 diffuse_color;

	ID3D11Resource* specular_map;
	ID3D11ShaderResourceView* specular_map_srv;
	vec4 specular_color;
	float specular_exp;

	ID3D11Resource* normal_map;
	ID3D11ShaderResourceView* normal_map_srv;
};