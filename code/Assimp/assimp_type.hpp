#pragma once

#include "../common.hpp"
#include "../vertex_layout.hpp"

#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#pragma comment(lib, "assimp/assimp-vc141-mtd.lib")


struct Assimp_Matrial
{
	string name;

	string diffuse_map_file;
	vec4 diffuse_color;

	string specular_map_file;
	vec4 specular_color;
	float specular_exp;

	string normal_map_file;

	Assimp_Matrial() {};
};

struct Assimp_Bone
{
	int index;
	string name;

	int parent_index;

	mat4x4 transform;
};

struct Assimp_Mesh_Part
{
	string name;
	string material_name;

	u32 start_vertex;
	u32 vertex_count;

	u32 start_index;
	u32 index_count;
};

struct Assimp_Mesh
{
	string name;
	int parent_bone;

	vector<Vertex_Texture_Normal_Tangent_Blend> vertices;
	vector<u32> indices;

	vector<Assimp_Mesh_Part *> mesh_parts;
};

struct Assimp_Keyframe_Data
{
	float time;

	vec3 scale;
	quat rotation;
	vec3 translation;

	Assimp_Keyframe_Data()
		: scale(1, 1, 1), rotation(0, 0, 0, 0), translation(0, 0, 0)
	{}
};

struct Assimp_Keyframe
{
	string bone_name;
	vector<Assimp_Keyframe_Data> transforms;
};

struct Assimp_Clip
{
	string name;

	u32 frame_count;
	f32 frame_rate;
	f32 duration;

	vector<Assimp_Keyframe *> keyframes;
};