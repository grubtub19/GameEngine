#pragma once
#include <rapidjson/document.h>

class Scene {
public:
	Scene();
	Scene(const char* file_name);
	~Scene();

	std::string file_name;

	void load() const;
	void parseGameObjects(const rapidjson::Document& document) const;
	void parseCamera(const rapidjson::Document& document) const;
	void parseLight(const rapidjson::Document& document) const;
};