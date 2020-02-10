#pragma once

#include "../common.hpp"
#include "assimp_type.hpp"

namespace Xml {
	class XMLDocument;
	class XMLElement;
}

class Assimp_Loader {
public:
	Assimp_Loader(wstring file_path, wstring save_folder, wstring save_name);
	void destroy();

	void export_mesh(wstring save_folder = L"", wstring save_file_name = L"");
	void export_material(wstring save_folder = L"", wstring save_file_name = L"");

private:
	void read_material();
	void write_material(wstring save_folder, wstring file_name);

	void read_bone_data(aiNode* node, int index, int parent);
	void make_bone_list(aiNode* node, int index, int parent);
	void make_bone_relation(aiNode* node, mat4x4& local_transform);

	void read_mesh_data(aiNode* node, int parent_bone);
	void write_mesh_data(wstring save_folder, wstring file_name);

	void write_xml_color(Xml::XMLDocument* document, Xml::XMLElement* element, vec4& color);
	string write_texture(string file);

private:
	Assimp::Importer *importer;
	const aiScene *scene;

	wstring fbx_file;
	wstring save_folder;
	wstring save_name;

	vector<Assimp_Matrial *> materials;
	vector<Assimp_Bone *> bones;
	vector<Assimp_Bone *> bone_indices;
	vector<Assimp_Mesh *> meshes;
};