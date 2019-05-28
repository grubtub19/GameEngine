#pragma once
#include <glad/glad.h>
#include <string>
#include <map>
#include <iostream>
#include <vector>
#include <assert.h>
#include "Game.h"
#include "Logger.h"
#include "data.h"
#include "Shader.h"
#include "Mesh.h"

#define glCheckError() Logger::glCheckError_(__FILE__, __LINE__) 

Mesh::Mesh() :
	material(nullptr)
{

}

Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, Material* material) :
	vertices(vertices),
	indices(indices),
	material(material)
{
	
}

Mesh::~Mesh() {
	
}


