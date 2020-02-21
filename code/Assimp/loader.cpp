#pragma once
#include "loader.hpp"
#include "../Utility/Path.h"
#include "../Utility/Xml.h"
#include "../Utility/BinaryFile.h"
#include "../Utility/String.h"

#include <assert.h>

Assimp_Loader::Assimp_Loader(wstring file_path, wstring save_folder, wstring save_name)
	:fbx_file(file_path), save_folder(save_folder), save_name(save_name)
{
	importer = new Assimp::Importer();

	scene = importer->ReadFile(
		String::ToString(file_path)
		, aiProcess_ConvertToLeftHanded | aiProcess_Triangulate
		//| aiProcess_OptimizeMeshes // 최적화옵션
		| aiProcess_GenNormals | aiProcess_CalcTangentSpace
		| aiProcess_LimitBoneWeights
		//| aiProcess_PreTransformVertices)
	);

	string err = importer->GetErrorString();
	assert(scene != nullptr);
}

void Assimp_Loader::destroy()
{
	SAFE_DELETE(importer);
}

void Assimp_Loader::export_mesh(wstring save_folder, wstring save_file_name)
{
	make_bone_list(scene->mRootNode, -1, -1);
	read_bone_data(scene->mRootNode, -1, -1);

	wstring temp_folder = save_folder.size() < 1 ? this->save_folder : save_folder;
	wstring temp_name = save_file_name.size() < 1 ? this->save_name : save_file_name;

	write_mesh_data(temp_folder, temp_name + L".mesh");
}

void Assimp_Loader::export_material(wstring save_folder, wstring save_file_name)
{
	read_material();

	wstring temp_folder = save_folder.size() < 1 ? this->save_folder : save_folder;
	wstring temp_name = save_file_name.size() < 1 ? this->save_name : save_file_name;

	write_material(temp_folder, temp_name + L".material");
}

void Assimp_Loader::read_material()
{
	for (u32 ii = 0; ii < scene->mNumMaterials; ii++)
	{
		aiMaterial* raw_material = scene->mMaterials[ii];

		Assimp_Matrial *material = new Assimp_Matrial();

		material->name = raw_material->GetName().C_Str();

		aiColor3D color;

		raw_material->Get(AI_MATKEY_COLOR_DIFFUSE, color);
		material->diffuse_color = vec4(color.r, color.g, color.b, 1.0f);

		raw_material->Get(AI_MATKEY_COLOR_SPECULAR, color);
		material->specular_color = vec4(color.r, color.g, color.b, 1.0f);

		float shininess;
		raw_material->Get(AI_MATKEY_SHININESS, shininess);
		material->specular_exp = shininess;

		float sp_factor;
		raw_material->Get(AI_MATKEY_SHININESS_STRENGTH, sp_factor);
		material->specular_color.a = sp_factor;

		aiString file_name;
		//If diffuse, specular, normal map count is higher than 1, this need to be correct.
		//Diffuse map
		{
			raw_material->GetTexture(aiTextureType_DIFFUSE, 0, &file_name);
			material->diffuse_map_file = file_name.C_Str();
		}

		//Specular map
		{
			raw_material->GetTexture(aiTextureType_SPECULAR, 0, &file_name);
			material->specular_map_file = file_name.C_Str();
		}

		//Normal map
		{
			raw_material->GetTexture(aiTextureType_NORMALS, 0, &file_name);
			material->normal_map_file = file_name.C_Str();
		}

		materials.push_back(material);
	}
}

void Assimp_Loader::write_material(wstring save_folder, wstring file_name)
{
	Path::CreateFolders(save_folder);

	Xml::XMLDocument* document = new Xml::XMLDocument();
	Xml::XMLDeclaration *decl = document->NewDeclaration();
	document->LinkEndChild(decl);

	Xml::XMLElement* root = document->NewElement("Materials");
	root->SetAttribute("xmlns:xsi", "http://www.w3.org/2001/XMLSchema-instance");
	root->SetAttribute("xmlns:xsd", "http://www.w3.org/2001/XMLSchema");
	document->LinkEndChild(root);

	for (Assimp_Matrial *material : materials)
	{
		Xml::XMLElement* node = document->NewElement("Material");
		root->LinkEndChild(node);

		Xml::XMLElement* element = NULL;

		element = document->NewElement("Name");
		element->SetText(material->name.c_str());
		node->LinkEndChild(element);



		element = document->NewElement("DiffuseFile");
		element->SetText(write_texture(material->diffuse_map_file).c_str());
		node->LinkEndChild(element);

		element = document->NewElement("SpecularFile");
		element->SetText(write_texture(material->specular_map_file).c_str());
		node->LinkEndChild(element);

		element = document->NewElement("NormalFile");
		element->SetText(write_texture(material->normal_map_file).c_str());
		node->LinkEndChild(element);


		element = document->NewElement("Diffuse");
		node->LinkEndChild(element);

		write_xml_color(document, element, material->diffuse_color);

		element = document->NewElement("Specular");
		node->LinkEndChild(element);

		write_xml_color(document, element, material->specular_color);

		element = document->NewElement("SpecularExp");
		element->SetText(material->specular_exp);
		node->LinkEndChild(element);

		SAFE_DELETE(material);
	}

	string file = String::ToString(save_folder + file_name);
	document->SaveFile(file.c_str());

	SAFE_DELETE(document);
}


void Assimp_Loader::read_bone_data(aiNode * node, int index, int parent)
{
	Assimp_Bone* bone = new Assimp_Bone();
	bone->index = index;
	bone->parent_index = parent;
	bone->name = node->mName.C_Str();

	mat4x4 transform;
	float* temp = node->mTransformation[0];
	float* _11 = &transform._11;
	for (int ii = 0; ii < 16; ii++)
		*_11++ = *temp++;

	matrix_transpose(&bone->transform, &transform);

	make_bone_relation(node, bone->transform);

	bones.push_back(bone);

	read_mesh_data(node, index);

	for (u32 ii = 0; ii < node->mNumChildren; ii++)
		read_bone_data(node->mChildren[ii], (s32)bones.size(), index);
}

void Assimp_Loader::make_bone_list(aiNode * node, int index, int parent)
{
	Assimp_Bone* bone = new Assimp_Bone();
	bone->index = index;
	bone->parent_index = parent;
	bone->name = node->mName.C_Str();

	bone_indices.push_back(bone);

	for (u32 ii = 0; ii < node->mNumChildren; ii++)
		make_bone_list(node->mChildren[ii], (u32)bone_indices.size(), index);
}

void Assimp_Loader::make_bone_relation(aiNode * node, mat4x4 & local_transform)
{
	if (node->mParent == nullptr)
		return;

	mat4x4 parent_transform;
	float* temp = node->mTransformation[0];
	float* _11 = &parent_transform._11;
	for (int ii = 0; ii < 16; ii++)
		*_11++ = *temp++;

	matrix_transpose(&parent_transform, &parent_transform);

	local_transform = local_transform * parent_transform;

	make_bone_relation(node->mParent, local_transform);

}

void Assimp_Loader::read_mesh_data(aiNode * node, int parent_bone)
{
	if (node->mNumMeshes < 1)
		return;

	Assimp_Mesh* mesh = new Assimp_Mesh();
	mesh->name = node->mName.C_Str();
	mesh->parent_bone = parent_bone;

	for (u32 ii = 0; ii < node->mNumMeshes; ii++)
	{
		u32 index = node->mMeshes[ii];
		aiMesh* mesh_data = scene->mMeshes[index];

		Assimp_Mesh_Part* mesh_part = new Assimp_Mesh_Part();
		mesh_part->name = mesh_data->mName.C_Str();
		mesh_part->material_name = scene->mMaterials[mesh_data->mMaterialIndex]->GetName().C_Str();

		mesh_part->start_vertex = (u32)mesh->vertices.size();
		mesh_part->vertex_count = mesh_data->mNumVertices;
		mesh_part->start_index = (u32)mesh->indices.size();
		mesh_part->index_count = mesh_data->mNumFaces * mesh_data->mFaces->mNumIndices;

		for (u32 ii = 0; ii < mesh_data->mNumVertices; ii++)
		{
			Vertex_Texture_Normal_Tangent_Blend vertex;
			memcpy(&vertex.position, &mesh_data->mVertices[ii], sizeof(vec3));

			if (mesh_data->HasTextureCoords(0))
				memcpy(&vertex.uv, &mesh_data->mTextureCoords[0][ii], sizeof(vec2));

			if (mesh_data->HasNormals())
				memcpy(&vertex.normal, &mesh_data->mNormals[ii], sizeof(vec3));

			if (mesh_data->HasTangentsAndBitangents())
				memcpy(&vertex.tangent, &mesh_data->mTangents[ii], sizeof(vec3));

			mesh->vertices.push_back(vertex);
		}//end mNumVertices

		//mNumFaces is the number of primitives.
		for (u32 ii = 0; ii < mesh_data->mNumFaces; ii++)
		{
			aiFace& face = mesh_data->mFaces[ii];

			for (u32 ii = 0; ii < face.mNumIndices; ii++)
			{
				mesh->indices.push_back(face.mIndices[ii]);
				//Each index should plus start vertex. 
				mesh->indices.back() += mesh_part->start_vertex;
			}
		}//end mNumFaces

		for (u32 ii = 0; ii < mesh_data->mNumBones; ii++)
		{
			const aiBone* bone_data = mesh_data->mBones[ii];
			for (u32 jj = 0; jj < bone_data->mNumWeights; jj++)
			{
				u32 vertex_id = bone_data->mWeights[jj].mVertexId;
				for (int weight_id = 0; weight_id < 4; weight_id++)
				{
					u32 bone_id;

					//If blend weight is exist already, jump next.
					if (mesh->vertices[vertex_id].blend_weights[weight_id] != 0)
						continue;

					else
					{
						string bone_name = bone_data->mName.C_Str();

						for (auto x : bone_indices)
						{
							if (bone_name == x->name)
							{
								bone_id = x->index;
								break;
							}
						}

						mesh->vertices[vertex_id].blend_weights[weight_id] = bone_data->mWeights[jj].mWeight;
						mesh->vertices[vertex_id].blend_indices[weight_id] = (float)bone_id;
						break;
					}
				}//end 4(wieght_id)
			}//end mNumWeight
		}//mNumBones

		mesh->mesh_parts.push_back(mesh_part);
	}//end mNumMeshes

	meshes.push_back(mesh);
}

void Assimp_Loader::write_mesh_data(wstring save_folder, wstring file_name)
{
	Path::CreateFolders(save_folder);

	BinaryWriter* writer = new BinaryWriter();
	writer->Open(save_folder + file_name);
	
	writer->UInt((u32)bones.size());
	for (auto bone : bones)
	{
		writer->Int(bone->index);
		writer->String(bone->name);
		writer->Int(bone->parent_index);
		writer->Matrix(bone->transform);
		
		SAFE_DELETE(bone);
	}

	writer->UInt((u32)meshes.size());
	for (auto mesh : meshes)
	{
		writer->String(mesh->name);
		writer->Int(mesh->parent_bone);
		
		writer->UInt((u32)mesh->vertices.size());
		writer->Byte(&mesh->vertices[0], (u32)sizeof(Vertex_Texture_Normal_Tangent_Blend) * (u32)mesh->vertices.size());

		writer->UInt((u32)mesh->indices.size());
		writer->Byte(&mesh->indices[0], (u32)sizeof(u32) * (u32)mesh->indices.size());

		/*writer->UInt(mesh->mesh_parts.size());
		for (auto part : mesh->mesh_parts)
		{
			writer->String(part->name);
			writer->String(part->material_name);

			writer->UInt(part->start_vertex);
			writer->UInt(part->vertex_count);

			writer->UInt(part->start_index);
			writer->UInt(part->index_count);

			SAFE_DELETE(part);
		}*/

		SAFE_DELETE(mesh);
	}

	writer->Close();
	SAFE_DELETE(writer);
}


void Assimp_Loader::write_xml_color(Xml::XMLDocument * document, Xml::XMLElement * element, vec4 & color)
{
	Xml::XMLElement* r = document->NewElement("R");
	r->SetText(color.r);
	element->LinkEndChild(r);

	Xml::XMLElement* g = document->NewElement("G");
	g->SetText(color.g);
	element->LinkEndChild(g);

	Xml::XMLElement* b = document->NewElement("B");
	b->SetText(color.b);
	element->LinkEndChild(b);

	Xml::XMLElement* a = document->NewElement("A");
	a->SetText(color.a);
	element->LinkEndChild(a);
}

string Assimp_Loader::write_texture(string file)
{
	if (file.length() < 1) return "";

	const aiTexture* texture = scene->GetEmbeddedTexture(file.c_str());
	
	//Case of Internal texture
	if (texture != nullptr)
	{
		//All Mixamo models use png texture file.
		string save_file = Path::GetFileNameWithoutExtension(file);
		save_file = String::ToString(save_folder) + save_file + ".png";

		//Heights some kind of texture's can be zero. ex) DXT_1, DXT_3, DXT_5.
		//In this case assume that image file includes header.
		if (texture->mHeight < 1)
		{
			FILE* fp;
			fopen_s(&fp, save_file.c_str(), "wb");
			fwrite(texture->pcData, texture->mWidth, 1, fp);
			fclose(fp);

			return save_file;
		}
		else
		{
			//// 색이 이상하게 나올꺼임.
			//ID3D11Texture2D* dest;
			//D3D11_TEXTURE2D_DESC destDesc;
			//ZeroMemory(&destDesc, sizeof(D3D11_TEXTURE2D_DESC));
			//destDesc.Width = texture->mWidth;
			//destDesc.Height = texture->mHeight;
			//destDesc.MipLevels = 1;
			//destDesc.ArraySize = 1;
			//destDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			//destDesc.SampleDesc.Count = 1;
			//destDesc.SampleDesc.Quality = 0;

			//D3D11_SUBRESOURCE_DATA subResource = { 0 };
			//subResource.pSysMem = texture->pcData;


			//HRESULT hr;
			//hr = D3D::GetDevice()->CreateTexture2D(&destDesc, &subResource, &dest);
			//assert(SUCCEEDED(hr));

			//D3DX11SaveTextureToFileA(D3D::GetDC(), dest, D3DX11_IFF_PNG, saveFile.c_str());

			//return saveFile;
		}
	}

	//Case of external texture
	else
	{
		wstring directory = Path::GetDirectoryName(fbx_file);
		string origin = String::ToString(directory) + file;
		bool bExist = Path::ExistFile(origin);
		if (bExist == false)
			return "";
		string fileName = String::ToString(save_folder) + Path::GetFileName(file);
		CopyFileA(origin.c_str(), fileName.c_str(), FALSE);

		return fileName;
	}

	return "";
}