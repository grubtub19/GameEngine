#include <unordered_map>
#include "SceneManager.h"
#include "Scene.h"
#include "Game.h"
#include "MeshRenderer.h"
#include "SkinnedMeshRenderer.h"
#include "PoolAllocator.h"
#include "GameObject.h"
#define SENSITIVITY_MOVE 10.0f

SceneManager::SceneManager() :
	active_scene(nullptr),
	main_camera(nullptr),
	scenes(PoolAllocator<Scene>(2))
{
	Logger::log("SceneManager Loaded");
}

SceneManager::~SceneManager() {

}

void SceneManager::add(const GameObject& game_object) {
	GameObject* new_game_object = Game::get().data_manager.addData(game_object);
	root_game_objects.push_back(new_game_object);
}

Scene* SceneManager::getActiveScene() {
	return scenes_by_name.at(active_scene);
}

void SceneManager::addScene(const char* name, const Scene& scene) {
	scenes_by_name.insert(std::make_pair(name, scenes.add(scene)));
}

void SceneManager::loadScene(const char* scene_name) {
	unloadAll();
	scenes_by_name.at(scene_name)->load();
}

void SceneManager::updateLight() {
	light->update();
	std::map<std::string, KeyStatus> keys = Game::get().input_manager.keys;
	Transform* transform = light->getTransformComponent();
	float time_elapsed = Game::get().time_manager.time_elapsed;
	if (keys.at("light_forward").is_down) {
		transform->translate(0.0f, 0.0f, -SENSITIVITY_MOVE * time_elapsed);
	}
	if (keys.at("light_backward").is_down) {
		transform->translate(0.0f, 0.0f, SENSITIVITY_MOVE * time_elapsed);
	}
	if (keys.at("light_left").is_down) {
		transform->translate(-SENSITIVITY_MOVE * time_elapsed, 0.0f, 0.0f);
	}
	if (keys.at("light_right").is_down) {
		transform->translate(SENSITIVITY_MOVE * time_elapsed, 0.0f, 0.0f);
	}
	if (keys.at("light_up").is_down) {
		transform->translate(0.0f, -SENSITIVITY_MOVE * time_elapsed, 0.0f);
	}
	if (keys.at("light_down").is_down) {
		transform->translate(0.0f, SENSITIVITY_MOVE * time_elapsed, 0.0f);
	}
}

void SceneManager::init() {
	Scene level1 = Scene("nowuhu.json");
	addScene("Level1", level1);
	scenes_by_name.at("Level1")->load();
}

void SceneManager::update() {
	main_camera->update();
	updateLight();
	for (GameObject* root_object : root_game_objects) {
		root_object->update();
	}
	//DEBUG START
	if (Game::get().is_debug_mode) Logger::log("Add root game objects updated");
	//DEBUG END
}

void SceneManager::draw() {
	//DEBUG START
	if (Game::get().is_debug_mode) Logger::log("Drawing start\nDrawing mesh renderers");
	//DEBUG END
	for (MeshRenderer* mesh_renderer : Game::get().data_manager.getList<MeshRenderer>()) {
		mesh_renderer->draw();
	}
	//DEBUG START
	if (Game::get().is_debug_mode) Logger::log("Drawing skinned mesh renderers");
	//DEBUG END
	for (SkinnedMeshRenderer* skinned_mesh_renderer : Game::get().data_manager.getList<SkinnedMeshRenderer>()) {
		skinned_mesh_renderer->draw();
	}
	//DEBUG START
	if (Game::get().is_debug_mode) Logger::log("Drawing end");
	//DEBUG END
}

/**
 * Removes all GameObjects that should be deleted when a new scene is loaded
 */
void SceneManager::unloadAll() {
	std::vector<GameObject*> objects = Game::get().data_manager.getList<GameObject>();
	std::vector<GameObject*> objects_to_delete = std::vector<GameObject*>();

	for (GameObject* game_object : objects) {	//loop through used_list and populate a vector of GameObjects to delete
		if (game_object->destroy_on_load) {
			objects_to_delete.push_back(game_object);			//we don't want to free() the GameObjects here because it would break iteration
		}
	}
	for (GameObject* object_to_delete : objects_to_delete) {	//delete 
		object_to_delete->unload();
		Game::get().data_manager.remove(object_to_delete);
	}
}