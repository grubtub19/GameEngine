#pragma once
#include "PoolAllocator.h"
class GameObject;
class Scene;

class SceneManager {
public:
	SceneManager();
	~SceneManager();

	std::vector<GameObject*> root_game_objects;
	GameObject* main_camera;
	GameObject* light;

private:
	std::unordered_map<const char*, Scene*> scenes_by_name;
	PoolAllocator<Scene> scenes;
	const char* active_scene;

public:
	void add(const GameObject& game_object);

	Scene* getActiveScene();
	void addScene(const char* name, const Scene& scene);
	void loadScene(const char* scene_name);
	void updateLight();

private:
	void unloadAll();

public:
	void init();
	void update();
	void draw();
};