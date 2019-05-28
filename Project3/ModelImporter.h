#pragma once
#include <string>
#include <assimp/scene.h>
#include "data.h"
class Mesh;
class GameObject;

class ModelImporter {
public:
	GameObject* loadModel(const std::string& path);
	
private:
	static int MAX_BONES;
	std::string directory;
	std::vector<GameObject*> node_objects;	//this does not include Mesh GameObjects

	GameObject* processNode(GameObject* parent, const aiNode* node, const aiScene* scene);
	void processNodeMeshes(aiNode const* node, const aiScene* scene);
	GameObject* processMesh(GameObject* parent, const aiMesh* assimp_mesh, const aiScene* scene);
	void processVertices(Mesh* mesh, const aiMesh* assimp_mesh);
	void processIndices(Mesh* mesh, const aiMesh* assimp_mesh);
	Material* processMaterial(const aiMesh* assimp_mesh, const aiScene* scene);
	void processBones(Bones* bones, const aiMesh* assimp_mesh);
	void processAnimations(GameObject* root_node, const aiScene* scene);
	void loadAssimpMaterial(Material* material, aiMaterial *assimp_material);
	Texture* loadAssimpTextures(const aiMaterial* assimp_material, const aiTextureType& texture_type);
	Texture* loadTexture(const std::string& path);
	unsigned int TextureFromFile(const std::string& path);
	std::string filterPath(const aiString& path);

	void showNodeName(aiNode* node);
	Material* getMaterial(const std::string& material_path);
	Texture* getTexture(const std::string& texture_path);
	GameObject* getNodeObject(const std::string& game_object_directory_and_name);
};