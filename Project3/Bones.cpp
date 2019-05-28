#include "Bones.h"
#include "GameObject.h"
#include "Transform.h"
#include "Game.h"

Bones::Bones() : mesh(nullptr)
{
}

Bones::~Bones()
{
}



std::vector<mtx::Matrix4>& Bones::getBoneMatrices() {
	for (unsigned int i = 0; i < bones.size(); i++) {
		Bone& bone = bones.at(i);

		Transform* bone_transform = bone.referenced_game_object->getTransformComponent();
		Transform* this_transform = containers.at(0)->getTransformComponent();

		//WARNING: I take the inverse of this gameobject's transform to patch a bug. This is not a good permanent solution!!!
		bone_matrices.at(i) = this_transform->global_transformation_matrix.inverse() * bone_transform->global_transformation_matrix * bone.inverse_bind_pose;
	}
	return bone_matrices;
}

void Bones::addBone(const Bone& bone) {
	bones.push_back(bone);
	bone_matrices.push_back(mtx::Matrix4(true));
}

void Bones::update() {

}