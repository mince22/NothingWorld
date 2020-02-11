#pragma once
#include "model_bone.hpp"

Model_Bone::Model_Bone(Assimp_Bone& bone)
{
	name = bone.name;
	index = bone.index;
	transform = bone.transform;

	parent_index = bone.parent_index;
}
