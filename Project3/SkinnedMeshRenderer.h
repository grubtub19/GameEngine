#pragma once
#include "Component.h"
#include "Shader.h"

class Transform;
class Mesh;
class Bones;

class SkinnedMeshRenderer : public Component {
public:
	SkinnedMeshRenderer();
	~SkinnedMeshRenderer();

	static Shader* shader;
	Transform* transform;
	Bones* bones;

	Mesh* getMesh();
	void setMesh(Mesh* new_mesh);

	void draw();
private:
	Mesh* mesh;

	unsigned int VAO, VBO, BBO, EBO;

	void setupBuffers();
	void unloadBuffers();
};