#include <algorithm>
#include "Component.h"
#include "GameObject.h"
#include "Game.h"

Component::Component() :
	num_dependancies(0)
{


}

Component::Component(GameObject* game_object) :
	num_dependancies(0)
{ 
	containers.push_back(game_object);
}

Component::~Component() 
{

}

/**
 * Removes the gameObject from the Component's list of gameObjects that contain it
 * @return boolean true if the component is no longer used and should be deleted. The component cannot delete itself and needs your help.
 */
bool Component::removeContainer(GameObject* game_object) {
	containers.erase(std::remove(containers.begin(), containers.end(), game_object), containers.end());
	if (containers.size() <= 0) {
		return true;
	}
	return false;
}

void Component::update() {

}

void Component::draw() {

}