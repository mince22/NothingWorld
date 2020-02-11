#pragma once
#include "model.hpp"
#include "model_bone.hpp"
#include "model_material.hpp"
#include "../assimp/assimp_type.hpp"
#include "../Utility/BinaryFile.h"

Model::Model()
{
}

Model::Model(D3D * d3d)
	: d3d(d3d)
{
}

void Model::destroy()
{
	for (auto bone : bones)
	{
		bone->destroy();
		SAFE_DELETE(bone);
	}

	for (auto mesh : meshes)
		SAFE_DELETE(mesh);

	unordered_map<string, Model_Material*>::iterator iter;
	for (iter = materials.begin(); iter != materials.end(); iter++)
	{
		iter->second->destroy();
		SAFE_DELETE(iter->second);
	}

}

void Model::read_mesh_file(wstring file)
{
	BinaryReader* reader = new BinaryReader();
	reader->Open(file);

	u32 bone_count = reader->UInt();
	for (u32 ii = 0; ii < bone_count; ii++)
	{
		Assimp_Bone assimp_bone;
		assimp_bone.index = reader->Int();
		assimp_bone.name = reader->String();
		assimp_bone.parent_index = reader->Int();
		assimp_bone.transform = reader->Matrix();

		Model_Bone* bone = new Model_Bone(assimp_bone);
		bones.push_back(bone);
	}

	u32 mesh_count = reader->UInt();
	for (u32 ii = 0; ii < mesh_count; ii++)
	{
		Assimp_Mesh* assimp_mesh = new Assimp_Mesh();
		assimp_mesh->name = reader->String();
		assimp_mesh->parent_bone = reader->Int();

		u32 vertices_size = reader->UInt();
		assimp_mesh->vertices.assign(vertices_size, Vertex_Texture_Normal_Tangent_Blend());
		void* ptr = (void*)&(assimp_mesh->vertices[0]);
		reader->Byte(&ptr, sizeof(Vertex_Texture_Normal_Tangent_Blend) * vertices_size);

		u32 indices_size = reader->UInt();
		assimp_mesh->indices.assign(indices_size, 0);
		ptr = (void*)&(assimp_mesh->indices[0]);
		reader->Byte(&ptr, sizeof(u32) * indices_size);

		meshes.push_back(assimp_mesh);
	}
}

void Model::read_material_file(wstring file)
{
	Xml::XMLDocument* document = new Xml::XMLDocument();

	string tempFile = String::ToString(file);
	Xml::XMLError error = document->LoadFile(tempFile.c_str());
	assert(error == Xml::XML_SUCCESS);

	Xml::XMLElement* root = document->FirstChildElement();
	Xml::XMLElement* matNode = root->FirstChildElement();

	do
	{
		Xml::XMLElement* node = NULL;

		Assimp_Matrial assimp_material;

		node = matNode->FirstChildElement();
		assimp_material.name = node->GetText();

		string directory = Path::GetDirectoryName(tempFile);

		node = node->NextSiblingElement();
		string diffuseTexture = node->GetText();
		diffuseTexture = Path::GetFileName(diffuseTexture);
		if (diffuseTexture.length() > 0)
			assimp_material.diffuse_map_file = (directory + diffuseTexture);

		node = node->NextSiblingElement();
		string specularTexture = node->GetText();
		specularTexture = Path::GetFileName(specularTexture);
		if (specularTexture.length() > 0)
			assimp_material.specular_map_file = (directory + specularTexture);

		node = node->NextSiblingElement();
		string normalTexture = node->GetText();
		normalTexture = Path::GetFileName(normalTexture);
		if (normalTexture.length() > 0)
			assimp_material.normal_map_file = (directory + normalTexture);


		vec4 x_color;
		Xml::XMLElement* color;

		//DiffuseColor
		node = node->NextSiblingElement();
		color = node->FirstChildElement();
		x_color.r = color->FloatText();

		color = color->NextSiblingElement();
		x_color.g = color->FloatText();

		color = color->NextSiblingElement();
		x_color.b = color->FloatText();

		color = color->NextSiblingElement();
		x_color.a = color->FloatText();
		assimp_material.diffuse_color = x_color;


		//SpecularColor
		node = node->NextSiblingElement();
		color = node->FirstChildElement();
		x_color.r = color->FloatText();

		color = color->NextSiblingElement();
		x_color.g = color->FloatText();

		color = color->NextSiblingElement();
		x_color.b = color->FloatText();

		color = color->NextSiblingElement();
		x_color.a = color->FloatText();
		assimp_material.specular_color = x_color;


		node = node->NextSiblingElement();
		assimp_material.specular_exp = (node->FloatText());


		matNode = matNode->NextSiblingElement();

		Model_Material* material = new Model_Material(d3d->get_device(), assimp_material);
		materials.insert(unordered_map<string, Model_Material *>::value_type(assimp_material.name, material));
	} while (matNode != NULL);
}
