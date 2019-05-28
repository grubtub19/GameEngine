#pragma once
#include "Component.h"
#include "Shader.h"
class Transform;
class Mesh;

class MeshRenderer : public Component {
public:
	MeshRenderer();
	~MeshRenderer();

	static Shader* shader;
	Transform* transform;

	Mesh* getMesh();
	void setMesh(Mesh* new_mesh);

	void draw();
private:
	Mesh* mesh;

	unsigned int VAO, VBO, EBO;

	void setupBuffers();
	void unloadBuffers();
};