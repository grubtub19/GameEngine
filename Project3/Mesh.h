#pragma once
#include <glad/glad.h>
#include <map>
#include <string>
#include <vector>
#include "Component.h"
#include "data.h"

class Mesh: public Component {
public:
	Mesh();
	Mesh(std::vector<Vertex> const &vertices, std::vector<unsigned int> const &indices, Material* material);

	~Mesh();

	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	Material* material;
};