#pragma once

#include "../common.hpp"
#include "../Device/d3d.hpp"
#include "../assimp/assimp_type.hpp"

#include <unordered_map>

class Model {
public:
	Model();
	Model(D3D* d3d);

	void destroy();


	void read_mesh_file(wstring file);
	void read_material_file(wstring file);

private:
	D3D* d3d;

	string material_file_name;

	unordered_map<string, class Model_Material *> materials;
	vector<class Model_Bone *> bones;
	vector<Assimp_Mesh *> meshes;
};