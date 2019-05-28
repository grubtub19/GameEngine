#pragma once
#include <vector>

class GameObject;

class Component {
public:
	Component();
	Component(GameObject* game_object);
	virtual ~Component() = 0;

	bool removeContainer(GameObject* game_object);

	std::vector<GameObject*> containers;

	unsigned int num_dependancies;

	void update();
	void draw();
};