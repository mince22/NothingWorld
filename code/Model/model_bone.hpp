#pragma once

#include "../common.hpp"
#include "../assimp/assimp_type.hpp"

class Model_Bone {
public:
	Model_Bone(Assimp_Bone& bone);
	void destroy() {};

	mat4x4 get_transform() const { return transform; };

private:
	string name;
	int index;
	mat4x4 transform;
	
	Model_Bone* parent;
	int parent_index;
	
	vector<Model_Bone *> childs;
};