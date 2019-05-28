#include <assert.h>
#include "MeshRenderer.h"
#include "Game.h"
#include "GameObject.h"
#include "Transform.h"
#include "Camera.h"
#include "Mesh.h"

Shader* MeshRenderer::shader = nullptr;

MeshRenderer::MeshRenderer() :
	VAO(0), VBO(0), EBO(0),
	mesh(nullptr)
{
	if (shader == nullptr) {
		Shader new_shader = Shader(std::string("Shaders/mesh_shader.vert"), std::string("Shaders/mesh_shader.frag"));
		shader = Game::get().data_manager.addData(new_shader);
	}
}

MeshRenderer::~MeshRenderer() {
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
}

Mesh* MeshRenderer::getMesh() {
	//
	if (Game::get().is_debug_mode) {
		assert(mesh != nullptr);
	}
	//
	return mesh;
}

/**
 * adds a new Mesh and uploads it's data to the GPU. Will remove old data if necessary.
 */
void MeshRenderer::setMesh(Mesh* new_mesh) {
	assert(new_mesh != nullptr);
	//only unload buffers if previously loaded
	if (mesh != nullptr) {
		unloadBuffers();
	}
	mesh = new_mesh;
	setupBuffers();
}

void MeshRenderer::setupBuffers() {
	///////////////////
	if (Game::get().is_debug_mode) {
		/*Logger::log("Vertex.position" + vertices[0].Position.to_string());
		Logger::log("Vertex.normal" + vertices[0].Normal.to_string());
		Logger::log("Vertex.texcoord" + vertices[0].TexCoord.to_string());
		Logger::log("Vertex.tangent" + vertices[0].Tangent.to_string());
		Logger::log("Vertex.bitangent" + vertices[0].Bitangent.to_string());*/
	}
	///////////////////

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	//store vertices in VBO
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, mesh->vertices.size() * sizeof(Vertex), &mesh->vertices[0], GL_STATIC_DRAW);

	//store indices in EBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh->indices.size() * sizeof(unsigned int), &mesh->indices[0], GL_STATIC_DRAW);

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

	glBindVertexArray(0);
}

void MeshRenderer::unloadBuffers() {
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
}

void MeshRenderer::draw() {
	shader->use();
	shader->setUniform4fv("global_ambient", Game::get().light_manager.ambient_light.rgba.data());

	shader->setUniform3fv("material.diffuse", mesh->material->diffuse);
	shader->setUniform3fv("material.specular", mesh->material->specular);
	shader->setUniform3fv("material.ambient", mesh->material->ambient);
	shader->setUniform1f("material.shininess", mesh->material->shininess);

	LightValues& light_values = Game::get().scene_manager.light_values;

	shader->setUniform4fv("light.diffuse", light_values.diffuse);
	shader->setUniform4fv("light.specular", light_values.specular);
	shader->setUniform4fv("light.ambient", light_values.ambient);
	shader->setUniform3fv("light.position", Game::get().scene_manager.light->getTransformComponent()->position.data_ptr());

	if (mesh->material->diffuse_map != nullptr) {
		//Logger::log("Enabling Diffuse: GL_TEXTURE0: " + std::to_string(model.textures.at(material.diffuse_path).id));
		shader->setUniform1i("has_diffuse", 1);
		shader->setUniform1i("diffuse", 0);
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
	
	mtx::Matrix4 norm_matrix = transform->global_transformation_matrix * Game::get().scene_manager.main_camera->getCameraComponent()->getViewMatrix();
	norm_matrix.invert().transpose();
	shader->setUniformMatrix4fv("norm_matrix", norm_matrix, true);
	
	glBindVertexArray(VAO);

	glDrawElements(GL_TRIANGLES, (GLsizei) mesh->indices.size(), GL_UNSIGNED_INT, 0);
	
	//optional, but just in case for now
	glBindVertexArray(0);
	glActiveTexture(GL_TEXTURE0);
}
