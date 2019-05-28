#pragma once
#include <unordered_map>
#include <vector>
#include <typeindex>
#include <type_traits>
#include "Component.h"
#include "Transform.h"
#include "Mesh.h"
#include "Camera.h"
#include "Bones.h"
#include "Skeleton.h"
#include "AnimationController.h"
#include "MeshRenderer.h"
#include "SkinnedMeshRenderer.h"

class GameObject {
public:
	GameObject();
	GameObject(GameObject* parent);
	~GameObject();

	std::string directory_and_name;
	GameObject* parent;
	std::vector<GameObject*> children;
	bool destroy_on_load;

private:
	Transform* transform;
	Mesh* mesh;
	Camera* camera;
	Bones* bones;
	Skeleton* skeleton;
	AnimationController* animation_controller;
	MeshRenderer* mesh_renderer;
	SkinnedMeshRenderer* skinned_mesh_renderer;


public:
	void unload();
	void removeAllComponents();
	void addChild(GameObject* child);
	void removeChild(GameObject* child);

	void removeTransformComponent();
	void removeMeshComponent();
	void removeCameraComponent();
	void removeBonesComponent();
	void removeSkeletonComponent();
	void removeAnimationControllerComponent();
	void removeMeshRendererComponent();
	void removeSkinnedMeshRendererComponent();
	
	Transform* addTransformComponent();
	Mesh* addMeshComponent();
	Camera* addCameraComponent();
	Bones* addBonesComponent();
	Skeleton* addSkeletonComponent();
	AnimationController* addAnimationControllerComponent();
	MeshRenderer* addMeshRendererComponent();
	SkinnedMeshRenderer* addSkinnedMeshRendererComponent();

	Transform* addTransformComponent(const Transform& transform);
	Mesh* addMeshComponent(const Mesh& mesh);
	Camera* addCameraComponent(const Camera& camera);
	Bones* addBonesComponent(const Bones& bones);
	Skeleton* addSkeletonComponent(const Skeleton& skeleton);
	AnimationController* addAnimationControllerComponent(const AnimationController& animation_controller);
	MeshRenderer* addMeshRendererComponent(const MeshRenderer& mesh_renderer);
	SkinnedMeshRenderer* addSkinnedMeshRendererComponent(const SkinnedMeshRenderer& skinned_mesh_renderer);

	Transform* getTransformComponent();
	Mesh* getMeshComponent();
	Camera* getCameraComponent();
	Bones* getBonesComponent();
	Skeleton* getSkeletonComponent();
	AnimationController* getAnimationControllerComponent();
	MeshRenderer* getMeshRendererComponent();
	SkinnedMeshRenderer* getSkinnedMeshRendererComponent();

	void update();
};