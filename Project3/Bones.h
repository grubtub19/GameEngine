#pragma once
#include <vector>
#include "Component.h"
#include "data.h"
#include "Mtx.h"

class Mesh;

class Bones : public Component {
public:
	Bones();
	~Bones();

	Mesh* mesh;

	std::vector<Bone> bones;
	std::vector<VertexBoneData> vertex_data;

	std::vector<mtx::Matrix4>& getBoneMatrices();

	void addBone(const Bone& bone);

private:
	std::vector<mtx::Matrix4> bone_matrices;
public:
	void update();
};