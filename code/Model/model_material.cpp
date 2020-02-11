#pragma once
#include "model_material.hpp"
#include "../Utility/WICTextureLoader.h"
#include "../Utility/String.h"

Model_Material::Model_Material()
{
	int a = 10;
}

Model_Material::Model_Material(ID3D11Device1* device, Assimp_Matrial& material)
{
	name = material.name;

	DirectX::CreateWICTextureFromFile(device, String::ToWString(material.diffuse_map_file).c_str(), &diffuse_map, &diffuse_map_srv);
	diffuse_color = material.diffuse_color;

	DirectX::CreateWICTextureFromFile(device, String::ToWString(material.specular_map_file).c_str(), &specular_map, &specular_map_srv);
	specular_color = material.specular_color;
	specular_exp = material.specular_exp;

	DirectX::CreateWICTextureFromFile(device, String::ToWString(material.normal_map_file).c_str(), &normal_map, &normal_map_srv);
}

void Model_Material::destroy()
{
	diffuse_map_srv->Release();
	diffuse_map->Release();

	specular_map_srv->Release();
	specular_map->Release();

	normal_map_srv->Release();
	normal_map->Release();
}