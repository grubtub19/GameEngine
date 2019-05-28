#include "GameObject.h"
#include "Transform.h"
#include "Mesh.h"
#include "Camera.h"
#include "Bones.h"
#include "Skeleton.h"
#include "AnimationController.h"
#include "MeshRenderer.h"
#include "SkinnedMeshRenderer.h"
#include "Game.h"

GameObject::GameObject() :
	parent(nullptr),
	transform(nullptr),
	mesh(nullptr),
	camera(nullptr),
	bones(nullptr),
	skeleton(nullptr),
	animation_controller(nullptr),
	mesh_renderer(nullptr),
	skinned_mesh_renderer(nullptr),
	destroy_on_load(true)
{

}

GameObject::GameObject(GameObject* parent) :
	parent(parent),
	transform(nullptr),
	mesh(nullptr),
	camera(nullptr),
	bones(nullptr),
	skeleton(nullptr),
	animation_controller(nullptr),
	mesh_renderer(nullptr),
	skinned_mesh_renderer(nullptr),
	destroy_on_load(true)
{

}

GameObject::~GameObject() {

}

/**
 * Prepares this game object to be deleted. Your parent inherits your kids. Components are removed.
 */
void GameObject::unload() {
	if (parent == nullptr) {	//if there is no parent

		for (GameObject* child : children) {
			child->parent = nullptr;	//remove the parent
			Game::get().scene_manager.root_game_objects.push_back(child);	//the child is now a root GameObject
		}
	} else {	//if there is a parent
		parent->removeChild(this);	// this object is no longer the parent's child

		for (GameObject* child : children) {
			child->parent = parent;		//this object's parent is assigned to this object's children
			parent->addChild(child);	//this object's children are added to the list of this object's parent's children
		}
	}
	
	removeAllComponents();
}

/**
 * Unlinks your components and deletes them if orphaned
 */
void GameObject::removeAllComponents()
{
	if (transform != nullptr) removeTransformComponent();
	if (mesh != nullptr) removeMeshComponent();
	if (camera != nullptr) removeCameraComponent();
	if (bones != nullptr) removeBonesComponent();
	if (skeleton != nullptr) removeSkeletonComponent();
	if (animation_controller != nullptr) removeAnimationControllerComponent();
	if (mesh_renderer != nullptr) removeMeshRendererComponent();
	if (skinned_mesh_renderer != nullptr) removeSkinnedMeshRendererComponent();
}

void GameObject::addChild(GameObject* child) {
	children.push_back(child);
}

void GameObject::removeChild(GameObject* child) {
	children.erase(std::remove(children.begin(), children.end(), child), children.end());
}

void GameObject::removeTransformComponent() {
	if (transform->removeContainer(this)) {	//remove this object from the Component's list of containers
		Game::get().data_manager.remove(transform);	//if the Component has no more containers delete it
	}
	transform = nullptr;	//remove from this object's map of Components
}
void GameObject::removeMeshComponent() {
	if (mesh->removeContainer(this)) {	//remove this object from the Component's list of containers
		Game::get().data_manager.remove(mesh);	//if the Component has no more containers delete it
	}
	mesh = nullptr;	//remove from this object's map of Components
}
void GameObject::removeCameraComponent() {
	if (camera->removeContainer(this)) {	//remove this object from the Component's list of containers
		Game::get().data_manager.remove(camera);	//if the Component has no more containers delete it
	}
	camera = nullptr;	//remove from this object's map of Components
}
void GameObject::removeBonesComponent() {
	if (bones->removeContainer(this)) {	//remove this object from the Component's list of containers
		Game::get().data_manager.remove(bones);	//if the Component has no more containers delete it
	}
	bones = nullptr;	//remove from this object's map of Components
}
void GameObject::removeSkeletonComponent() {
	if (skeleton->removeContainer(this)) {	//remove this object from the Component's list of containers
		Game::get().data_manager.remove(skeleton);	//if the Component has no more containers delete it
	}
	skeleton = nullptr;	//remove from this object's map of Components
}
void GameObject::removeAnimationControllerComponent() {
	if (animation_controller->removeContainer(this)) {	//remove this object from the Component's list of containers
		Game::get().data_manager.remove(animation_controller);	//if the Component has no more containers delete it
	}
	animation_controller = nullptr;	//remove from this object's map of Components
}
void GameObject::removeMeshRendererComponent() {
	if (mesh_renderer->removeContainer(this)) {	//remove this object from the Component's list of containers
		Game::get().data_manager.remove(mesh_renderer);	//if the Component has no more containers delete it
	}
	mesh_renderer = nullptr;	//remove from this object's map of Components
}
void GameObject::removeSkinnedMeshRendererComponent() {
	if (skinned_mesh_renderer->removeContainer(this)) {	//remove this object from the Component's list of containers
		Game::get().data_manager.remove(skinned_mesh_renderer);	//if the Component has no more containers delete it
	}
	skinned_mesh_renderer = nullptr;	//remove from this object's map of Components
}


Transform* GameObject::addTransformComponent() {
	transform = Game::get().data_manager.addData(Transform());	//Create the component in the DataManager
	transform->containers.push_back(this);	//add this object to the component's list of containers
	return transform;
}
Mesh* GameObject::addMeshComponent() {
	mesh = Game::get().data_manager.addData(Mesh());	//Create the component in the DataManager
	mesh->containers.push_back(this);	//add this object to the component's list of containers
	return mesh;
}
Camera* GameObject::addCameraComponent() {
	camera = Game::get().data_manager.addData(Camera());	//Create the component in the DataManager
	camera->containers.push_back(this);	//add this object to the component's list of containers
	return camera;
}
Bones* GameObject::addBonesComponent() {
	bones = Game::get().data_manager.addData(Bones());	//Create the component in the DataManager
	bones->containers.push_back(this);	//add this object to the component's list of containers
	return bones;
}
Skeleton* GameObject::addSkeletonComponent() {
	skeleton = Game::get().data_manager.addData(Skeleton());	//Create the component in the DataManager
	skeleton->containers.push_back(this);	//add this object to the component's list of containers
	return skeleton;
}
AnimationController* GameObject::addAnimationControllerComponent() {
	animation_controller = Game::get().data_manager.addData(AnimationController());	//Create the component in the DataManager
	animation_controller->containers.push_back(this);	//add this object to the component's list of containers
	return animation_controller;
}
MeshRenderer* GameObject::addMeshRendererComponent() {
	mesh_renderer = Game::get().data_manager.addData(MeshRenderer());	//Create the component in the DataManager
	mesh_renderer->containers.push_back(this);	//add this object to the component's list of containers
	return mesh_renderer;
}
SkinnedMeshRenderer* GameObject::addSkinnedMeshRendererComponent() {
	skinned_mesh_renderer = Game::get().data_manager.addData(SkinnedMeshRenderer());	//Create the component in the DataManager
	skinned_mesh_renderer->containers.push_back(this);	//add this object to the component's list of containers
	return skinned_mesh_renderer;
}


Transform* GameObject::addTransformComponent(const Transform& transform) {
	this->transform = Game::get().data_manager.addData(transform);	//Create the component in the DataManager
	this->transform->containers.push_back(this);	//add this object to the component's list of containers
	return this->transform;
}
Mesh* GameObject::addMeshComponent(const Mesh& mesh) {
	this->mesh = Game::get().data_manager.addData(mesh);	//Create the component in the DataManager
	this->mesh->containers.push_back(this);	//add this object to the component's list of containers
	return this->mesh;
}
Camera* GameObject::addCameraComponent(const Camera& camera) {
	this->camera = Game::get().data_manager.addData(camera);	//Create the component in the DataManager
	this->camera->containers.push_back(this);	//add this object to the component's list of containers
	return this->camera;
}
Bones* GameObject::addBonesComponent(const Bones& bones) {
	this->bones = Game::get().data_manager.addData(bones);	//Create the component in the DataManager
	this->bones->containers.push_back(this);	//add this object to the component's list of containers
	return this->bones;
}
Skeleton* GameObject::addSkeletonComponent(const Skeleton& skeleton) {
	this->skeleton = Game::get().data_manager.addData(skeleton);	//Create the component in the DataManager
	this->skeleton->containers.push_back(this);	//add this object to the component's list of containers
	return this->skeleton;
}
AnimationController* GameObject::addAnimationControllerComponent(const AnimationController& animation_controller) {
	this->animation_controller = Game::get().data_manager.addData(animation_controller);	//Create the component in the DataManager
	this->animation_controller->containers.push_back(this);	//add this object to the component's list of containers
	return this->animation_controller;
}
MeshRenderer* GameObject::addMeshRendererComponent(const MeshRenderer& mesh_renderer) {
	this->mesh_renderer = Game::get().data_manager.addData(mesh_renderer);	//Create the component in the DataManager
	this->mesh_renderer->containers.push_back(this);	//add this object to the component's list of containers
	return this->mesh_renderer;
}
SkinnedMeshRenderer* GameObject::addSkinnedMeshRendererComponent(const SkinnedMeshRenderer& skinned_mesh_renderer) {
	this->skinned_mesh_renderer = Game::get().data_manager.addData(skinned_mesh_renderer);	//Create the component in the DataManager
	this->skinned_mesh_renderer->containers.push_back(this);	//add this object to the component's list of containers
	return this->skinned_mesh_renderer;
}

Transform* GameObject::getTransformComponent() {
	return transform;
}

Mesh* GameObject::getMeshComponent() {
	return mesh;
}
Camera* GameObject::getCameraComponent() {
	return camera;
}
Bones* GameObject::getBonesComponent() {
	return bones;
}
Skeleton* GameObject::getSkeletonComponent() {
	return skeleton;
}
AnimationController* GameObject::getAnimationControllerComponent() {
	return animation_controller;
}
MeshRenderer* GameObject::getMeshRendererComponent() {
	return mesh_renderer;
}
SkinnedMeshRenderer* GameObject::getSkinnedMeshRendererComponent() {
	return skinned_mesh_renderer;
}

void GameObject::update() {
	//DEBUG START
	if(Game::get().is_debug_mode) Logger::log(directory_and_name + " updating");
	//DEBUG END
	if (animation_controller != nullptr) animation_controller->update();
	if (camera != nullptr) camera->update();
	if (transform != nullptr) transform->update();
	if (mesh != nullptr) mesh->update();
	if (bones != nullptr) bones->update();
	if (skeleton != nullptr) skeleton->update();
	if (mesh_renderer != nullptr) mesh_renderer->update();
	if (skinned_mesh_renderer != nullptr) skinned_mesh_renderer->update();
	//DEBUG START
	if (Game::get().is_debug_mode) Logger::log(directory_and_name + " finished updating");
	//DEBUG END
	for (GameObject* child : children) {
		child->update();
	}
	//DEBUG START
	if (Game::get().is_debug_mode) Logger::log(directory_and_name + " children finished updating");
	//DEBUG END
}