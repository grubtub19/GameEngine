#include <assert.h>
#include "SkinnedMeshRenderer.h"
#include "Mesh.h"
#include "Game.h"
#include "Camera.h"
#include "Bones.h"
#include "Transform.h"

Shader* SkinnedMeshRenderer::shader = nullptr;

SkinnedMeshRenderer::SkinnedMeshRenderer() :
	VAO(0), BBO(0), VBO(0), EBO(0),
	transform(nullptr), mesh(nullptr), bones(nullptr)
{
	if (shader == nullptr) {
		Shader new_shader = Shader(std::string("Shaders/skinned_mesh_shader.vert"), std::string("Shaders/skinned_mesh_shader.frag"));
		shader = Game::get().data_manager.addData(new_shader);
	}
}

SkinnedMeshRenderer::~SkinnedMeshRenderer() {
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
}

Mesh* SkinnedMeshRenderer::getMesh() {
	assert(mesh != nullptr);
	return mesh;
}

/**
 * adds a new Mesh and uploads it's data to the GPU. Will remove old data if necessary.
 */
void SkinnedMeshRenderer::setMesh(Mesh * new_mesh) {
	assert(new_mesh != nullptr);
	//only unload buffers if previously loaded
	if (mesh != nullptr) {
		unloadBuffers();
	}
	mesh = new_mesh;
	setupBuffers();
}

void SkinnedMeshRenderer::setupBuffers() {
	//DEBUG START
	if (Game::get().is_debug_mode) {
		/*Logger::log("Vertex.position" + vertices[0].Position.to_string());
		Logger::log("Vertex.normal" + vertices[0].Normal.to_string());
		Logger::log("Vertex.texcoord" + vertices[0].TexCoord.to_string());
		Logger::log("Vertex.tangent" + vertices[0].Tangent.to_string());
		Logger::log("Vertex.bitangent" + vertices[0].Bitangent.to_string());*/
	}
	//DEBUG END

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &BBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	//DEBUG START
	if (Game::get().is_debug_mode) {
		Logger::log("mesh->vertices.size() " + std::to_string(mesh->vertices.size()));
		Logger::log("mesh->vertices.at(0).position: " + mesh->vertices.at(0).Position.to_string());
	}


	//store vertices in VBO
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, mesh->vertices.size() * sizeof(Vertex), mesh->vertices.data(), GL_STATIC_DRAW);

	//tell OpenGL how you want the Vertices interpreted

	//Position = layout -> 0
	glEnableVertexAttribArray(0);
	//this vertex attribute will take 3 floats starting from no offset for each Vertex
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

	//Normal = layout -> 1
	glEnableVertexAttribArray(1);
	//this vertex attribute will take 3 floats starting from the offset of the Normals for each Vertex
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));

	//TexCoords = layout -> 2
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoord));

	//store bone influence info in BBO
	glBindBuffer(GL_ARRAY_BUFFER, BBO);
	glBufferData(GL_ARRAY_BUFFER, bones->vertex_data.size() * sizeof(VertexBoneData), bones->vertex_data.data(), GL_STATIC_DRAW);

	//DEBUG START
	if (Game::get().is_debug_mode) {
		Logger::log("bones->vertex_data.size(): " + std::to_string(bones->vertex_data.size()));
		Logger::log("bones->vertex_data.at(0).bone_indices[0]: " + std::to_string(bones->vertex_data.at(0).bone_indices[0]));
		Logger::log("bones->vertex_data.at(0).bone_weights[0]: " + std::to_string(bones->vertex_data.at(0).bone_weights[0]));
	}
	//tell OpenGL how you want the VertexBoneData interpreted

	//BoneIndices = layout -> 3
	glEnableVertexAttribArray(3);
	glVertexAttribIPointer(3, 4, GL_INT, sizeof(VertexBoneData), (void*)0);

	//BoneWeights = layout -> 4
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(VertexBoneData), (void*)offsetof(VertexBoneData, bone_weights));

	//store indices in EBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh->indices.size() * sizeof(unsigned int), &mesh->indices[0], GL_STATIC_DRAW);

	glBindVertexArray(0);

	//DEBUG START
	if (Game::get().is_debug_mode) Logger::log(std::string("indices.size(): ") + std::to_string(mesh->indices.size()));
	//DEBUG END
}

void SkinnedMeshRenderer::unloadBuffers() {
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
}

void SkinnedMeshRenderer::draw() {
	shader->use();
	shader->setUniform4fv("global_ambient", Game::get().light_manager.ambient_light.rgba.data());

	shader->setUniform3fv("material.diffuse", mesh->material->diffuse);
	shader->setUniform3fv("material.specular", mesh->material->specular);
	shader->setUniform3fv("material.ambient", mesh->material->ambient);
	shader->setUniform1f("material.shininess", mesh->material->shininess);

	Light* light = Game::get().scene_manager.light->getLightComponent();

	shader->setUniform4fv("light.diffuse", light->diffuse);
	shader->setUniform4fv("light.specular", light->specular);
	shader->setUniform4fv("light.ambient", light->ambient);
	shader->setUniform3fv("light.position", Game::get().scene_manager.light->getTransformComponent()->position.data_ptr());
	shader->setUniform1f("light.constant_att", light->constant_att);
	shader->setUniform1f("light.linear_att", light->linear_att); 
	shader->setUniform1f("light.quad_att", light->quad_att);
	shader->setUniform1f("light.shininess", light->shininess);

	if (mesh->material->diffuse_map != nullptr) {
		//Logger::log("Enabling Diffuse: GL_TEXTURE0: " + std::to_string(model.textures.at(material.diffuse_path).id));
		shader->setUniform1i("has_diffuse", 1);
		shader->setUniform1i("diffuse", 0);	//synced up to GL_TEXTURE0
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, mesh->material->diffuse_map->id);
	}
	
	else {
		shader->setUniform1i("has_diffuse", 0);
	}
	/*
	if (material.specular_path.length() > 0) {
		Logger::log("Enabling Specular: GL_TEXTURE1" + std::to_string(GL_TEXTURE1));
		shader.setUniform1i("has_specular", 1);
		shader.setUniform1i("specular", 1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, model.textures.at(material.specular_path).id);
	}
	else {
		shader.setUniform1i("has_specular", 0);
	}

	if (material.normal_path.length() > 0) {
		Logger::log("Enabling Normal: GL_TEXTURE2" + std::to_string(GL_TEXTURE2));
		shader.setUniform1i("has_normal", 1);
		shader.setUniform1i("normal", 2);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, model.textures.at(material.normal_path).id);
	}
	else {
		shader.setUniform1i("has_normal", 0);
	}

	if (material.height_path.length() > 0) {
		Logger::log("Enabling Height: GL_TEXTURE3" + std::to_string(GL_TEXTURE3));
		shader.setUniform1i("has_height", 1);
		shader.setUniform1i("height", 3);
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, model.textures.at(material.height_path).id);
	}
	else {
		shader.setUniform1i("has_height", 0);
	}*/
	Camera* camera = Game::get().scene_manager.main_camera->getCameraComponent();
	shader->setUniformMatrix4fv("proj_matrix", camera->projection_matrix, true);
	shader->setUniformMatrix4fv("v_matrix", camera->getViewMatrix(), true);
	shader->setUniformMatrix4fv("m_matrix", transform->global_transformation_matrix, true);

	mtx::Matrix4 norm_matrix = camera->getViewMatrix() * transform->global_transformation_matrix;
	norm_matrix.invert().transpose();
	shader->setUniformMatrix4fv("norm_matrix", norm_matrix, true);

	std::vector<mtx::Matrix4>& bone_matrices = bones->getBoneMatrices();
	shader->setUniformMatrix4fv("bone_matrices", bone_matrices, true);

	glBindVertexArray(VAO);

	glDrawElements(GL_TRIANGLES, mesh->indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
	//optional, but just in case for now
	glBindVertexArray(0);
	glActiveTexture(GL_TEXTURE0);
}