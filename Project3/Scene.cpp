#include <rapidjson/document.h>
#include <rapidjson/error/en.h>
#include <sstream>
#include <fstream>
#include <assert.h>
#include "Scene.h"
#include "Game.h"
#include "ModelImporter.h"
#include "GameObject.h"
#include "Camera.h"
#include "Transform.h"

Scene::Scene()
{

}

Scene::Scene(const char* file_path) :
	file_name(file_path)
{

}

Scene::~Scene() {

}

void Scene::load() const {
	std::ostringstream sstream;
	std::ifstream fs(("Scenes/" + file_name).c_str());
	sstream << fs.rdbuf();
	const std::string str(sstream.str());
	const char* ptr = str.c_str();

	rapidjson::Document document;
	document.Parse<rapidjson::kParseCommentsFlag>(ptr);
	fprintf(stderr, "\nError(offset %u): %s\n",
		(unsigned)document.GetErrorOffset(),
		rapidjson::GetParseError_En(document.GetParseError()));
	assert(document.IsObject());
	if (document.HasMember("gameObjects")) {
		parseGameObjects(document);
	}
	
	parseCamera(document);
	parseLight(document);
}

void Scene::parseGameObjects(const rapidjson::Document& document) const {
	const rapidjson::Value& game_objects = document["gameObjects"];
	assert(game_objects.IsArray());
	ModelImporter importer = ModelImporter();
	for (auto& v : game_objects.GetArray()) {
		assert(v.IsObject());
		assert(v.HasMember("model"));
		assert(v["model"].IsString());
		GameObject* game_object = importer.loadModel(v["model"].GetString());
		Game::get().scene_manager.root_game_objects.push_back(game_object);
		Transform* transform = game_object->getTransformComponent();
		if (v.HasMember("translate")) {
			assert(v["translate"].IsArray());
			assert(v["translate"].Size() == 3);
			assert(v["translate"][0].IsFloat());
			transform->position.x += v["translate"][0].GetFloat();
			assert(v["translate"][1].IsFloat());
			transform->position.y += v["translate"][1].GetFloat();
			assert(v["translate"][2].IsFloat());
			transform->position.z += v["translate"][2].GetFloat();
		}
		if (v.HasMember("rotate")) {
			const rapidjson::Value& rotate = v["rotate"];
			assert(rotate.IsObject());
			assert(rotate.HasMember("axis"));
			assert(rotate["axis"].IsArray());
			assert(rotate["axis"].Size() == 3);
			mtx::Vec3 axis;
			assert(rotate["axis"][0].IsFloat());
			axis.x = rotate["axis"][0].GetFloat();
			assert(rotate["axis"][1].IsFloat());
			axis.y = rotate["axis"][1].GetFloat();
			assert(rotate["axis"][2].IsFloat());
			axis.z = rotate["axis"][2].GetFloat();
			assert(rotate.HasMember("angle"));
			assert(rotate["angle"].IsFloat());
			float angle = rotate["angle"].GetFloat();
			transform->rotation.make_rotation(axis, angle);
		}
		if (v.HasMember("scale")) {
			const rapidjson::Value& rotate = v["scale"];
			assert(rotate.IsFloat());
			float scale_value = rotate.GetFloat();
			transform->scaler = mtx::Vec3(scale_value, scale_value, scale_value);
		}
		assert(v.HasMember("destroy"));
		assert(v["destroy"].IsBool());
		game_object->destroy_on_load = v["destroy"].GetBool();
	}
}

void Scene::parseCamera(const rapidjson::Document& document) const {
	assert(document.HasMember("camera"));
	const rapidjson::Value& camera = document["camera"];
	GameObject* object = Game::get().data_manager.addData(GameObject());
	Transform* transform = object->addTransformComponent();
	Game::get().scene_manager.main_camera = object;
	Camera* camera_object = object->addCameraComponent();
	camera_object->transform = transform;
	assert(camera.IsObject());
	assert(camera.HasMember("fov_y"));
	assert(camera["fov_y"].IsFloat());
	float fov_y = camera["fov_y"].GetFloat();
	assert(camera.HasMember("aspect"));
	assert(camera["aspect"].IsFloat());
	float aspect = camera["aspect"].GetFloat();
	assert(camera.HasMember("near_plane"));
	assert(camera["near_plane"].IsFloat());
	float near_plane = camera["near_plane"].GetFloat();
	assert(camera.HasMember("far_plane"));
	assert(camera["far_plane"].IsFloat());
	float far_plane = camera["far_plane"].GetFloat();
	camera_object->initProjectionMatrix(fov_y, aspect, near_plane, far_plane);
	assert(camera.HasMember("translate"));
	assert(camera["translate"].IsArray());
	assert(camera["translate"].Size() == 3);
	assert(camera["translate"][0].IsFloat());
	transform->position.x = camera["translate"][0].GetFloat();
	assert(camera["translate"][1].IsFloat());
	transform->position.y = camera["translate"][1].GetFloat();
	assert(camera["translate"][2].IsFloat());
	transform->position.z = camera["translate"][2].GetFloat();
}

void Scene::parseLight(const rapidjson::Document& document) const {
	assert(document.HasMember("light"));
	const rapidjson::Value& light = document["light"];
	GameObject* object;
	Transform* transform;
	if (light.HasMember("model")) {
		ModelImporter importer = ModelImporter();
		object = importer.loadModel(light["model"].GetString());
		transform = object->getTransformComponent();
	}
	else {
		object = Game::get().data_manager.addData(GameObject());
		transform = object->addTransformComponent();
	}	
	
	Game::get().scene_manager.light = object;
	assert(light.IsObject());
	if (light.HasMember("translate")) {
		assert(light["translate"].IsArray());
		assert(light["translate"].Size() == 3);
		assert(light["translate"][0].IsFloat());
		transform->position.x = light["translate"][0].GetFloat();
		assert(light["translate"][1].IsFloat());
		transform->position.y = light["translate"][1].GetFloat();
		assert(light["translate"][2].IsFloat());
		transform->position.z = light["translate"][2].GetFloat();
	}
	if (light.HasMember("scale")) {
		assert(light["scale"].IsFloat());
		const rapidjson::Value& scale = light["scale"];
		float scale_value = scale.GetFloat();
		transform->scaler = mtx::Vec3(scale_value, scale_value, scale_value);
	}
}