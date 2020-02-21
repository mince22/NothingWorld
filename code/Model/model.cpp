#pragma once
#include "model.hpp"
#include "model_bone.hpp"
#include "model_material.hpp"
#include "../assimp/assimp_type.hpp"
#include "../Utility/BinaryFile.h"
#include "../Render/shader.hpp"


Model::Model()
{
}

Model::Model(D3D * d3d)
	: d3d(d3d)
{
	shader = Render_System::shader_pool.find(shader_type_name[u32(Shader_Type::Model_Instance)])->second;
}

Model::Model(D3D * d3d, Shader * shader)
	:d3d(d3d), shader(shader)
{

}

void Model::load_model(wstring model_name)
{
	name = model_name;

	wstring material_file = L"_model/" + model_name + L"/" + model_name + L".material";
	read_material_file(material_file);

	wstring mesh_file = L"_model/" + model_name + L"/" + model_name + L".mesh";
	read_mesh_file(mesh_file);

	buffer_count = (u32)meshes.size();

	create_vertex_buffer();
	create_index_buffer();

	if (shader == nullptr)
	{
		shader = new Shader(d3d->get_device(), d3d->get_context());
		shader->create_shader(Shader_Profile::VERTEX_SHADER, L"_asset/shader/model.hlsl", "vs_5_0");
		shader->create_shader(Shader_Profile::PIXEL_SHADER, L"_asset/shader/model.hlsl", "ps_5_0");
	}
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

	for (auto material_vector : materials)
	{
		for (auto material : material_vector.second)
			SAFE_DELETE(material);
	}

	if (shader != nullptr)
	{
		cleanup(shader);
		SAFE_DELETE(shader);
	}

	for (auto buffer : vertex_buffers)
		buffer->Release();

	for (auto buffer : index_buffers)
		buffer->Release();

	if (instance_buffer)
		instance_buffer->Release();
}

void Model::render()
{
	{
		mat4x4* instance_mat = Render_System::model_instance_pool.find(name)->second;
		instance_mat[0]._41 = -75.0f;
		instance_mat[0]._42 = 0.0f;
		instance_mat[0]._43 = 0.0f;
		instance_mat[1]._41 = 75.0f;
		instance_mat[2]._41 = 0.0f;

		if (!instance_buffer)
			create_instance_buffer(instance_mat);

		/*if (!instance_buffer)
			create_constant_buffer(d3d->get_device(), instance_mat, (u32)(sizeof(mat4x4) * MODEL_INSTANCE_MAX));*/
		//else
			//write_data_constant_buffer(d3d->get_context(), instance_buffer, instance_mat, (u32)(sizeof(mat4x4) * MODEL_INSTANCE_MAX));
	}
	

	ID3D11ShaderResourceView* diffuse_map_rtv = materials.find(this->name)->second[0]->get_diffuse_map_srv();
	d3d->get_context()->PSSetShaderResources(0, 1, &diffuse_map_rtv);

	for (int ii = 0; ii < index_buffers.size(); ii++)
	{
		u32 stride = sizeof(Vertex_Texture_Normal_Tangent_Blend);
		u32 offset = 0;
		d3d->get_context()->IASetVertexBuffers(0, 1, &vertex_buffers[ii], &stride, &offset);

		stride = sizeof(mat4x4);
		d3d->get_context()->IASetVertexBuffers(1, 1, &instance_buffer, &stride, &offset);
		d3d->get_context()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		d3d->get_context()->IASetIndexBuffer(index_buffers[ii], DXGI_FORMAT_R32_UINT, 0);
		//d3d->get_context()->DrawIndexed((u32)meshes[ii]->indices.size(), 0, 0);
		d3d->get_context()->DrawIndexedInstanced((u32)meshes[ii]->indices.size(), 3, 0, 0, 0);
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
	vector<Model_Material *> material_vector;
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

		//Model_Material* material = new Model_Material(d3d->get_device(), assimp_material);
		//material_vector.emplace_back(d3d->get_device(), assimp_material);
		material_vector.push_back(new Model_Material(d3d->get_device(), assimp_material));
	} while (matNode != NULL);

	materials.insert(unordered_map<wstring, vector<Model_Material *>>::value_type(this->name, material_vector));
	//document->
	SAFE_DELETE(document);
}

void Model::create_vertex_buffer()
{
	HRESULT hr;
	for (u32 ii = 0; ii < buffer_count; ii++)
	{
		// Fill in a buffer description.
		D3D11_BUFFER_DESC buffer_description;
		buffer_description.Usage = D3D11_USAGE_DEFAULT;
		buffer_description.ByteWidth = (u32)(sizeof(Vertex_Texture_Normal_Tangent_Blend) * meshes[ii]->vertices.size());
		buffer_description.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		buffer_description.CPUAccessFlags = 0;
		buffer_description.MiscFlags = 0;
		buffer_description.StructureByteStride = 0;

		// Fill in the subresource data.
		D3D11_SUBRESOURCE_DATA init_data;
		init_data.pSysMem = &meshes[ii]->vertices[0];
		init_data.SysMemPitch = 0;
		init_data.SysMemSlicePitch = 0;

		//Create buffer with the device
		ID3D11Buffer* buffer;
		hr = d3d->get_device()->CreateBuffer(&buffer_description, &init_data, &buffer);
		vertex_buffers.push_back(buffer);
		assert(SUCCEEDED(hr));
	}
}

void Model::create_index_buffer()
{
	HRESULT hr;

	for (u32 ii = 0; ii < buffer_count; ii++)
	{
		// Fill in a buffer description.
		D3D11_BUFFER_DESC buffer_description;
		buffer_description.Usage = D3D11_USAGE_DEFAULT;
		buffer_description.ByteWidth = (u32)(sizeof(u32) * meshes[ii]->indices.size());
		buffer_description.BindFlags = D3D11_BIND_INDEX_BUFFER;
		buffer_description.CPUAccessFlags = 0;
		buffer_description.MiscFlags = 0;
		buffer_description.StructureByteStride = 0;

		// Fill in the subresource data.
		D3D11_SUBRESOURCE_DATA init_data;
		init_data.pSysMem = &meshes[ii]->indices[0];
		init_data.SysMemPitch = 0;
		init_data.SysMemSlicePitch = 0;

		//Create buffer with the device
		ID3D11Buffer* buffer;
		hr = d3d->get_device()->CreateBuffer(&buffer_description, &init_data, &buffer);
		index_buffers.push_back(buffer);

		assert(SUCCEEDED(hr));
	}
}

void Model::create_instance_buffer(void* data)
{
	HRESULT hr;

	// Fill in a buffer description.
	D3D11_BUFFER_DESC buffer_description;
	buffer_description.Usage = D3D11_USAGE_DEFAULT;
	buffer_description.ByteWidth = (sizeof(mat4x4) * MODEL_INSTANCE_MAX);
	//buffer_description.ByteWidth = (sizeof(float) * 19 * 3);
	buffer_description.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	buffer_description.CPUAccessFlags = 0;
	buffer_description.MiscFlags = 0;
	buffer_description.StructureByteStride = 0;

	// Fill in the subresource data.
	D3D11_SUBRESOURCE_DATA init_data;
	init_data.pSysMem = data;
	init_data.SysMemPitch = 0;
	init_data.SysMemSlicePitch = 0;

	//Create buffer with the device
	ID3D11Buffer* buffer;
	hr = d3d->get_device()->CreateBuffer(&buffer_description, &init_data, &buffer);
	assert(SUCCEEDED(hr));

	instance_buffer = buffer;
}