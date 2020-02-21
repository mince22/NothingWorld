#pragma once

#include "../common.hpp"
#include "../Device/d3d.hpp"
#include "../assimp/assimp_type.hpp"
#include "../Render/shader.hpp"

class Model {
public:
	Model();
	Model(D3D* d3d);
	Model(D3D* d3d, Shader* shader);

	void load_model(wstring model_name);

	void destroy();

	void render();

	Shader* get_shader() const { return shader; }

private:
	void read_mesh_file(wstring file);
	void read_material_file(wstring file);

	void create_vertex_buffer();
	void create_index_buffer();
	void create_instance_buffer(void* data);

	void create_bone_buffer();
private:
	wstring name;

	D3D* d3d;

	Shader* shader;

	string material_file_name;

	unordered_map<wstring, vector<class Model_Material *>> materials;
	vector<class Model_Bone *> bones;
	vector<Assimp_Mesh *> meshes;

	u32 buffer_count;
	vector<ID3D11Buffer *> vertex_buffers;
	vector<ID3D11Buffer *> index_buffers;

	ID3D11Buffer* instance_buffer = nullptr;
};