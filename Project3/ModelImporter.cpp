#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include <vector>
#include <iostream>
#include <assert.h>
#include "Game.h"
#include "GameObject.h"
#include "Transform.h"
#include "Mesh.h"
#include "Bones.h"
#include "Skeleton.h"
#include "ModelImporter.h"
#include "AnimationController.h"
#include "Logger.h"
#include "Mtx.h"
#include "MeshRenderer.h"
#include "SkinnedMeshRenderer.h"
//#include "data.h"
//#include "Shader.h"
//#include "Game.h"

#define AI_SCENE_FLAGS_IMCOMPLETE 0x1

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

int ModelImporter::MAX_BONES = 4;

/**
 * Loads the model from a specified path
 */
GameObject* ModelImporter::loadModel(std::string const &path) {
	Assimp::Importer import;

	//DEBUG START
	if (Game::get().is_debug_mode) Logger::log("\nLoading model: " + path);
	//DEBUG END

	const aiScene* scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_JoinIdenticalVertices | aiProcess_FlipUVs | aiProcess_GenNormals | aiProcess_LimitBoneWeights | aiProcess_Debone);	

	
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_IMCOMPLETE || !scene->mRootNode) {
		//DEBUG START
		if (Game::get().is_debug_mode) Logger::log("aiScene import failure: " + std::string(import.GetErrorString()));
		//DEBUG END
		exit(EXIT_FAILURE);
	}

	//DEBUG START
	if (Game::get().is_debug_mode) {
		Logger::log("    Num Meshes: " + std::to_string(scene->mNumMeshes));
		Logger::log("    Num Animations: " + std::to_string(scene->mNumAnimations));
		if (scene->HasAnimations()) {
			Logger::log("		mAnimations[0]->mNumChannels: " + std::to_string(scene->mAnimations[0]->mNumChannels));
			Logger::log("		mAnimations[0]->mDuration: " + std::to_string(scene->mAnimations[0]->mDuration));
			Logger::log("		mAnimations[0]->mTicksPerSecond: " + std::to_string(scene->mAnimations[0]->mTicksPerSecond));
		}
		Logger::log("    Node Names: ");
		showNodeName(scene->mRootNode);
	}
	//DEBUG END

	directory = path.substr(0, path.find_last_of('/') + 1);	// Ex: "Models/Cube/"

	//Recursively process all nodes starting at the root node
	GameObject* root_node = processNode(nullptr, scene->mRootNode, scene);
	//Process meshes only after the entire node graph is finished so the bones can make their connections properly
	processNodeMeshes(scene->mRootNode, scene);
	
	//Animations
	if (scene->HasAnimations()) {
		root_node->addAnimationControllerComponent();
		processAnimations(root_node, scene);
	}
	
	directory = "";
	node_objects.empty();

	return root_node;
}

/**
 * Processes all of a node's meshes then recursively calls any child nodes
 */
GameObject* ModelImporter::processNode(GameObject* parent, aiNode const *node, const aiScene *scene) {
	GameObject* object = Game::get().data_manager.addData(GameObject());	//create a new GameObject for this node
	node_objects.push_back(object);	//add this object to the list of this model's Node GameObjects

	//DEBUG START
	if (Game::get().is_debug_mode) Logger::log("Processing Node");
	//DEBUG END

	//scene graph
	object->parent = parent;	//make the parent this object's parent
	if (parent != nullptr) {
		parent->children.push_back(object);	//make this object a child of the parent
	}

	//name
	object->directory_and_name = directory + node->mName.C_Str();

	//transform
	Transform* transform = object->addTransformComponent();
	transform->setFromMatrix(node->mTransformation);

	//DEBUG START
	if (Game::get().is_debug_mode) {
	Logger::log("    Name: " + object->directory_and_name);
	Logger::log("	 NumMeshes: " + std::to_string(node->mNumMeshes));
	Logger::log("	 NumChildren: " + std::to_string(node->mNumChildren));
		Logger::log("    mTransformation: \n" + mtx::Matrix4(node->mTransformation).to_string(2));
	}
	//DEBUG END

	transform->update();

	//children
	for (unsigned int i = 0; i < node->mNumChildren; i++) {
		processNode(object, node->mChildren[i], scene);
	}
	return object;
}

void ModelImporter::processNodeMeshes(aiNode const* node, const aiScene* scene) {
	for (unsigned int i = 0; i < node->mNumChildren; i++) {
		processNodeMeshes(node->mChildren[i], scene);
	}
	//meshes
	for (unsigned int i = 0; i < node->mNumMeshes; i++) {
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];	//if the node has meshes, make those meshes children of this node
		processMesh(getNodeObject(directory + node->mName.C_Str()), mesh, scene);
	}
}

/**
 * Extracts Vertices and Indices and creates a new Mesh from them
 * Adds any new Material or Textures to their respective hashmaps
 * @return Mesh
 */
GameObject* ModelImporter::processMesh(GameObject* parent, const aiMesh* assimp_mesh, const aiScene* scene) {
	//DEBUG START
	if (Game::get().is_debug_mode) {
		Logger::log("Processing Mesh Object\n    Name: " + directory + assimp_mesh->mName.C_Str());
		Logger::log("	Parent: " + parent->directory_and_name);
	}
	//DEBUG END

	GameObject* object = Game::get().data_manager.addData(GameObject());	//create a new GameObject for this mesh
	object->directory_and_name = directory + assimp_mesh->mName.C_Str();
	assert(parent != nullptr);
	parent->children.push_back(object);	//make this object a child of the parent
	object->parent = parent;	//make the parent this object's parent

	Transform* transform = object->addTransformComponent();
	Mesh* mesh = object->addMeshComponent();	//add a Mesh Component to the GameObject
	
	//vertices
	processVertices(mesh, assimp_mesh);

	//DEBUG START
	if (Game::get().is_debug_mode) Logger::log("	Vertices length: " + mesh->vertices.size());
	//DEBUG END
	
	//indices
	processIndices(mesh, assimp_mesh);

	//DEBUG START
	if (Game::get().is_debug_mode) Logger::log("	Indices length: " + mesh->indices.size());
	//DEBUG END

	//material

	Material* material = processMaterial(assimp_mesh, scene);
	mesh->material = material;	//Assign the new Material as this Mesh's Material

	if (assimp_mesh->HasBones()) {
		//Add Bones component
		Bones* bones = object->addBonesComponent();
		bones->mesh = mesh;
		processBones(bones, assimp_mesh);
		//Add SkinnedMeshRenderer
		SkinnedMeshRenderer* skinned_mesh_renderer = object->addSkinnedMeshRendererComponent();
		//Link other Components
		skinned_mesh_renderer->transform = transform;
		skinned_mesh_renderer->bones = object->getBonesComponent();
		skinned_mesh_renderer->setMesh(mesh);
		
		//DEBUG START
		if (Game::get().is_debug_mode) Logger::log("		SkinnedMeshRenderer count: " + std::to_string(Game::get().data_manager.getList<SkinnedMeshRenderer>().size()));
		//DEBUG END
	} else {
		//Add MeshRenderer
		MeshRenderer* mesh_renderer = object->addMeshRendererComponent();
		//Link other Components
		mesh_renderer->transform = transform;
		mesh_renderer->setMesh(mesh);
		//DEBUG START
		if (Game::get().is_debug_mode) Logger::log("		MeshRenderer:");
		//DEBUG END
	}
	return object;
}

void ModelImporter::processVertices(Mesh* mesh, const aiMesh* assimp_mesh) {
	//resize the correct amount of vertices and indices
	mesh->vertices.resize(assimp_mesh->mNumVertices);
	for (unsigned int i = 0; i < assimp_mesh->mNumVertices; i++) {
		Vertex& vertex = mesh->vertices.at(i);

		//position
		vertex.Position.x = assimp_mesh->mVertices[i].x;
		vertex.Position.y = assimp_mesh->mVertices[i].y;
		vertex.Position.z = assimp_mesh->mVertices[i].z;

		//normal
		vertex.Normal.x = assimp_mesh->mNormals[i].x;
		vertex.Normal.y = assimp_mesh->mNormals[i].y;
		vertex.Normal.z = assimp_mesh->mNormals[i].z;

		//texture coordinates
		if (assimp_mesh->mTextureCoords[0]) {	//does the mesh have texture coordinates?
			vertex.TexCoord.x = assimp_mesh->mTextureCoords[0][i].x;
			vertex.TexCoord.y = assimp_mesh->mTextureCoords[0][i].y;	//assimp allows 8 texture coordinates, but we only care about the first one
		}
		else {
			vertex.TexCoord.x = 0.0f;	//if not give them default ones
			vertex.TexCoord.y = 0.0f;
		}
	}
}

void ModelImporter::processIndices(Mesh* mesh, const aiMesh* assimp_mesh) {
	mesh->indices.resize(assimp_mesh->mNumFaces * 3);	//resize 3 times the number of faces
	//process the indices
	for (unsigned int i = 0; i < assimp_mesh->mNumFaces; i++) {	//loop through faces
		aiFace face = assimp_mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++) {	//loop through the indices of each face
			mesh->indices.at(3 * i + j) = face.mIndices[j];
		}
	}
}

Material* ModelImporter::processMaterial(const aiMesh* assimp_mesh, const aiScene* scene) {
	aiMaterial* assimp_material = scene->mMaterials[assimp_mesh->mMaterialIndex];
	std::string material_name = std::string(assimp_material->GetName().C_Str());	//to avoid overlapping material names between models, we prepend the model's directory

	//DEBUG START
	if (Game::get().is_debug_mode) Logger::log("	Material: " + directory + material_name);
	//DEBUG END

	Material* repeat_material = getMaterial(directory + material_name);
	if (repeat_material != nullptr) {	//if the material is already loaded, we don't need to load another

		//DEBUG START
		if (Game::get().is_debug_mode) Logger::log(std::string("		Already loaded"));
		//DEBUG END

		return repeat_material;
	} else {

		//DEBUG START
		if (Game::get().is_debug_mode) Logger::log(std::string("		Loading new Material"));
		//DEBUG END

		Material* material = Game::get().data_manager.addData(Material());	//create a new material for the Mesh to point to
		loadAssimpMaterial(material, assimp_material);	//if the Material exists and is new, load it

		return material;
	}
}

void ModelImporter::processBones(Bones* bones, const aiMesh* assimp_mesh) {

	//each vertex will have VertexBoneData. By reserving it, we can access specific indices without going out of bounds
	bones->vertex_data.resize(assimp_mesh->mNumVertices);
	//DEBUG START
	if (Game::get().is_debug_mode) Logger::log("	Bones:");
	//DEBUG END

	//iterate over all the bones
	for (unsigned int bone_iterator = 0; bone_iterator < assimp_mesh->mNumBones; bone_iterator++) {
		aiBone* assimp_bone = assimp_mesh->mBones[bone_iterator];
		//Referenced game object
		std::string skeleton_name = assimp_bone->mName.C_Str();

		//START DEBUG
		if (Game::get().is_debug_mode) Logger::log("Bone's Name" + skeleton_name);

		GameObject* game_object = getNodeObject(directory + skeleton_name);

		//START DEBUG
		if (Game::get().is_debug_mode) Logger::log("Bone's GameObject" + game_object->directory_and_name);

		assert(game_object != nullptr);

		// Add bone to list of bones
		// Add referenced GameObject to the bone
		Bone bone;
		bone.referenced_game_object = game_object;
		// Add the inverse bind transform to the bone
		bone.inverse_bind_pose = (mtx::Matrix4) assimp_bone->mOffsetMatrix;

		//DEBUG START
		if (Game::get().is_debug_mode) Logger::log("Bone's Inverse Bind Pose:\n" + bone.inverse_bind_pose.to_string());
		//DEBUG END

		// Iterate over every vertex this bone influences
		for (unsigned int weight_iterator = 0; weight_iterator < assimp_bone->mNumWeights; weight_iterator++) {
			aiVertexWeight assimp_vertex_weight = assimp_bone->mWeights[weight_iterator];

			//select the VertexBoneData at the index of the influenced vertex
			VertexBoneData& bone_data = bones->vertex_data.at(assimp_vertex_weight.mVertexId);

			//iterate over the VertexBoneData's weight array to find an empty spot
			for (unsigned int i = 0; i < sizeof(bone_data.bone_weights); i++) {
				float weight = bone_data.bone_weights[i];

				//if we found an empty spot
				if (weight == 0.0f) {
					//add a bone influence for this vertex
					bone_data.bone_indices[i] = bone_iterator;
					//add the influence amount of the bone on this vertex
					bone_data.bone_weights[i] = assimp_vertex_weight.mWeight;
					break;
				}
			}
		}
		bones->addBone(bone);

		//DEBUG START
		//if (Game::get().is_debug_mode) {
		//	Logger::log("		Bone:");
		//	Logger::log("			Referenced GameObject: " + bone.referenced_game_object->directory_and_name);
		//	Logger::log("			Inverse Bind Pose: " + bone.inverse_bind_pose.to_string());
		//}
		//DEBUG END
	}
	//DEBUG START
	if (Game::get().is_debug_mode) {
		Logger::log("		VertexBoneData length: " + bones->vertex_data.size());
	}
	//DEBUG END

	//DEBUG START
	/*if (Game::get().is_debug_mode) {
		for (unsigned int i = 0; i < std::clamp((int)bones->vertex_data.size(), 0, Logger::bone_vertex_data_count); i++) {
			Logger::log("		VertexBoneData[" + std::to_string(i) + "]: ");
			for (unsigned int j = 0; j < MAX_BONES; j++) {
				Logger::log("			Bone index: [ " + std::to_string(bones->vertex_data.at(i).bone_indices[0]) + " " +
					std::to_string(bones->vertex_data.at(i).bone_indices[1]) + " " +
					std::to_string(bones->vertex_data.at(i).bone_indices[2]) + " " +
					std::to_string(bones->vertex_data.at(i).bone_indices[3]) + " ]");
				Logger::log("			Bone Weight: [ " + std::to_string(bones->vertex_data.at(i).bone_weights[0]) + " " +
					std::to_string(bones->vertex_data.at(i).bone_weights[1]) + " " +
					std::to_string(bones->vertex_data.at(i).bone_weights[2]) + " " +
					std::to_string(bones->vertex_data.at(i).bone_weights[3]) + " ]");
			}
		}
	}*/
	//DEBUG END
}

void ModelImporter::processAnimations(GameObject* root_node, const aiScene* scene) {
	// Get the Animation Controller for this model
	AnimationController* animation_controller = root_node->getAnimationControllerComponent();
	// Reserve an Animation for each
	animation_controller->animations.resize(scene->mNumAnimations);

	//DEBUG START
	if (Game::get().is_debug_mode) Logger::log("		AnimationController");
	//DEBUG END

	// Iterate over every every animation
	for (unsigned int animation_iterator = 0; animation_iterator < scene->mNumAnimations; animation_iterator++) {
		aiAnimation* assimp_animation = scene->mAnimations[animation_iterator];

		// Get the corresponding Animation from the AnimationController
		Animation& animation = animation_controller->animations.at(animation_iterator);

		animation.name = assimp_animation->mName.C_Str();

		//DEBUG START
		if (Game::get().is_debug_mode) Logger::log("			Animation: " + animation.name);
		//DEBUG END

		// Convert from ticks to seconds
		// mTicksPerSecond is 0 if not specified
		if (assimp_animation->mTicksPerSecond > 0) {
			animation.duration = (float) (assimp_animation->mDuration / assimp_animation->mTicksPerSecond);
		}
		else {
			animation.duration = (float) (assimp_animation->mDuration / AnimationController::default_ticks_per_second);
		}

		//DEBUG START
		if (Game::get().is_debug_mode) Logger::log("				Duration (seconds): " + std::to_string(animation.duration));
		//DEBUG END
		
		// Iterate over every skeleton used in this animation
		for (unsigned int channel_iterator = 0; channel_iterator < assimp_animation->mNumChannels; channel_iterator++) {
			aiNodeAnim* assimp_skeleton_anim = assimp_animation->mChannels[channel_iterator];

			// Get referenced Skeleton GameObject from the list of all node GameObjects in this model
			const std::string skeleton_name = assimp_skeleton_anim->mNodeName.C_Str();
			GameObject* node_object = getNodeObject(directory + skeleton_name);
			if (node_object == nullptr) {
				//DEBUG START
				if (Game::get().is_debug_mode) Logger::log("ERROR: Animation KeyFrames reference a non-existant skeleton");
				//DEBUG END
				break;
			}

			// Get the Skeleton Component or initialize it if it doesn't yet exist
			Skeleton* skeleton = node_object->getSkeletonComponent();
			if (skeleton == nullptr) {
				skeleton = node_object->addSkeletonComponent();
				skeleton->transform = node_object->getTransformComponent();
				skeleton->animation_controller = animation_controller;
				skeleton->animations.resize(scene->mNumAnimations);
			}

			//DEBUG START
			if (Game::get().is_debug_mode) Logger::log("				Skeleton Name: " + node_object->directory_and_name);
			//DEBUG END

			// Get the empty AnimationKeyFrames at the current animation_iterator from this Skeleton's animations
			AnimationKeyFrames& animation = skeleton->animations.at(animation_iterator);
			animation.is_used = true;

			//DEBUG START
			if (Game::get().is_debug_mode) Logger::log("				Num Position Keyframes: " + std::to_string(assimp_skeleton_anim->mNumPositionKeys));
			//DEBUG END

			// Iterate over all the position key frames
			for (unsigned int position_iterator = 0; position_iterator < assimp_skeleton_anim->mNumPositionKeys; position_iterator++) {
				aiVectorKey& assimp_position_key_frame = assimp_skeleton_anim->mPositionKeys[position_iterator];
				// create a PositionKeyFrame
				PositionKeyFrame position_key = PositionKeyFrame();
				// assign the values
				position_key.position = assimp_position_key_frame.mValue;
				position_key.time_stamp = (float) assimp_position_key_frame.mTime / assimp_animation->mTicksPerSecond;
				// push it into the AnimationKeyFrames's position_key_frames
				animation.position_key_frames.push_back(position_key);

				//DEBUG START
				//if (Game::get().is_debug_mode) {
				//	Logger::log("					Position: " + position_key.position.to_string());
				//	Logger::log("					Timestamp: " + std::to_string(position_key.time_stamp));
				//}
				//DEBUG END
			}

			//DEBUG START
			if (Game::get().is_debug_mode) Logger::log("				Num Scale Keyframes: " + assimp_skeleton_anim->mNumScalingKeys);
			//DEBUG END

			//iterate over all the scale key frames
			for (unsigned int scale_iterator = 0; scale_iterator < assimp_skeleton_anim->mNumScalingKeys; scale_iterator++) {
				aiVectorKey& assimp_scale_key_frame = assimp_skeleton_anim->mScalingKeys[scale_iterator];
				//create a ScaleKeyFrame
				ScaleKeyFrame scale_key = ScaleKeyFrame();
				//assign the values
				scale_key.scale = assimp_scale_key_frame.mValue;
				scale_key.time_stamp = (float) assimp_scale_key_frame.mTime / assimp_animation->mTicksPerSecond;
				//push it into the animation's scale_key_frames
				animation.scale_key_frames.push_back(scale_key);

				//DEBUG START
				//if (Game::get().is_debug_mode) {
				//	Logger::log("					Scale: " + scale_key.scale.to_string());
				//	Logger::log("					Timestamp: " + std::to_string(scale_key.time_stamp));
				//}
				//DEBUG END
			}

			//DEBUG START
			if (Game::get().is_debug_mode) Logger::log("				Num Rotation Keyframes: " + assimp_skeleton_anim->mNumRotationKeys);
			//DEBUG END

			//iterate over all the rotation key frames
			for (unsigned int rotation_iterator = 0; rotation_iterator < assimp_skeleton_anim->mNumRotationKeys; rotation_iterator++) {
				aiQuatKey assimp_rotation_key_frame = assimp_skeleton_anim->mRotationKeys[rotation_iterator];
				//create a RotationKeyFrame
				RotationKeyFrame rotation_key = RotationKeyFrame();
				//assign the values
				rotation_key.rotation = assimp_rotation_key_frame.mValue;
				rotation_key.time_stamp = (float) assimp_rotation_key_frame.mTime / assimp_animation->mTicksPerSecond;
				//push it into the animation's rotation_key_frames
				animation.rotation_key_frames.push_back(rotation_key);

				//DEBUG START
				//if (Game::get().is_debug_mode) {
				//	Logger::log("					Rotation: " + rotation_key.rotation.to_string());
				//	Logger::log("					Timestamp: " + std::to_string(rotation_key.time_stamp));
				//}
				//DEBUG END
			}
		}	
	}
}

/**
 * Loads a Material and any new Textures from the Assimp Material
 */
void ModelImporter::loadAssimpMaterial(Material* material, aiMaterial *assimp_material) {

	// Extract lighting properties
	assimp_material->Get(AI_MATKEY_COLOR_DIFFUSE, material->diffuse);
	assimp_material->Get(AI_MATKEY_COLOR_AMBIENT, material->ambient);
	assimp_material->Get(AI_MATKEY_COLOR_SPECULAR, material->specular);
	assimp_material->Get(AI_MATKEY_SHININESS_STRENGTH, material->shininess);
	
	material->diffuse[0] = 1.0f;
	material->diffuse[1] = 1.0f;
	material->diffuse[2] = 1.0f;
	material->ambient[0] = 1.0f;
	material->ambient[1] = 1.0f;
	material->ambient[2] = 1.0f;
	material->specular[0] = 1.0f;
	material->specular[1] = 1.0f;
	material->specular[2] = 1.0f;
	material->shininess = 10.0f;
	

	//DEBUG START
	if(Game::get().is_debug_mode) Logger::log("			Diffuse: [ " + std::to_string(material->diffuse[0]) + ", " + std::to_string(material->diffuse[1]) + ", " + std::to_string(material->diffuse[2]) + " ]");
	//DEBUG END

	// Load and assign Textures
	material->diffuse_map = loadAssimpTextures(assimp_material, aiTextureType_DIFFUSE);
	material->specular_map = loadAssimpTextures(assimp_material, aiTextureType_SPECULAR);
	material->normal_map = loadAssimpTextures(assimp_material, aiTextureType_NORMALS);
	material->height_map = loadAssimpTextures(assimp_material, aiTextureType_HEIGHT);

	std::string material_name = directory + std::string(assimp_material->GetName().C_Str());
}

/**
 * Loads the first texture of the specified type from the Assimp Material
 * If necessary, it will also add a new texture to the textures hashmap
 * The path variable will contain the path to the texture if a texture is found
 * @return bool whether or not the specified texture was found
 */
Texture* ModelImporter::loadAssimpTextures(const aiMaterial* assimp_material, const aiTextureType& texture_type) {

	//DEBUG START
	std::string type_string;
	if (Game::get().is_debug_mode) {
		switch (texture_type) {
		case aiTextureType_DIFFUSE: type_string = std::string("DIFFUSE"); break;
		case aiTextureType_SPECULAR: type_string = std::string("SPECULAR"); break;
		case aiTextureType_NORMALS: type_string = std::string("NORMAL"); break;
		case aiTextureType_HEIGHT: type_string = std::string("HEIGHT"); break;
		}
	}
	//DEBUG END

	aiString aiPath;
	// We only get the first of potentially many Textures of this TextureType
	assimp_material->GetTexture(texture_type, 0, &aiPath); 
	std::string texture_filename = filterPath(aiPath);
	

	// If the Material specifies this TextureType
	if (texture_filename.length() > 0) {	

		// Check if the Texture is already loaded
		Texture* texture = getTexture(directory + texture_filename);

		// If the Texture is already loaded
		if (texture != nullptr) {

			//DEBUG START
			if (Game::get().is_debug_mode)	Logger::log("        " + type_string + " already loaded at path  " + directory + texture_filename);
			//DEBUG END

			return texture;	// If the texture is already loaded, return a pointer to it
		} else {
			//DEBUG START
			if (Game::get().is_debug_mode) Logger::log("        " + type_string + " loading at path " + directory + texture_filename);
			//DEBUG END

			// If the Texture is new, load it
			return loadTexture(texture_filename);
		}
	} else {

		//DEBUG START
		if (Game::get().is_debug_mode)	Logger::log("        " + type_string + " not specified ");
		//DEBUG END

		// If this TextureType is not specified
		return nullptr;
	}
}

/**
 * Loads a new texture and adds it to the textures hashmap
 */
Texture* ModelImporter::loadTexture(std::string const &filename) {
	Texture* texture = Game::get().data_manager.addData(Texture());
	texture->path = directory + filename;
	texture->id = TextureFromFile(filename);
	return texture;
}

/**
 * Creates a new OpenGL texture object given the path/filename of the texture relative to the loaded model's file
 * @return GLuint ID of texture object
 */
unsigned int ModelImporter::TextureFromFile(std::string const &filename) {
	std::string relative_path = directory + filename;
	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;

	//DEBUG START
	if (Game::get().is_debug_mode) Logger::log("            Loading Image");
	//DEBUG END

	unsigned char* data = stbi_load(relative_path.c_str(), &width, &height, &nrComponents, 0);
	if (data) {
		GLenum format;
		if (nrComponents == 1) {
			format = GL_RED;
		}
		else if (nrComponents == 3) {
			format = GL_RGB;
		}
		else if (nrComponents == 4) {
			format = GL_RGBA;
		} 
		else {
			Logger::log("ERRER::SOIL2 improper number of color components when loading " + relative_path);
			std::cin.ignore();
			exit(EXIT_FAILURE);
		}

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
	else {
		Logger::log("Texture failed to load at path: " + relative_path + " : " + stbi_failure_reason());
		//std::cin.ignore();
		//exit(EXIT_FAILURE);
	}
	stbi_image_free(data);
	return textureID;
}

std::string ModelImporter::filterPath(const aiString& path) {
	std::string rtn = path.C_Str();
	if (rtn._Starts_with("C:")) {
		int c = rtn.rfind("\\") + 1;
		int a = rtn.rfind("/") + 1;
		int b = rtn.rfind("¥") + 1;
		rtn.erase(0, std::max(std::max(a,b),c));
	}
	return rtn;
}

void ModelImporter::showNodeName(aiNode* node) {
	Logger::log(node->mName.data);
	for (unsigned int i = 0; i < node->mNumChildren; i++) {
		showNodeName(node->mChildren[i]);
	}
}

/**
 * Finds the material in the DataManager
 * @return bool a pointer to the Material or nullptr if not found
 */
Material* ModelImporter::getMaterial(const std::string& material_path) {
	for (Material* material : Game::get().data_manager.getList<Material>()) {
		if (material->directory_and_name == material_path) {
			return material;
		}
	}
	return nullptr;
}

/**
 * Finds the texture in the DataManager
 * @return bool a pointer to the Texture or nullptr if not found
 */
Texture* ModelImporter::getTexture(const std::string& texture_path) {
	for (Texture* texture : Game::get().data_manager.getList<Texture>()) {
		if (texture->path == texture_path) {
			return texture;
		}
	}
	return nullptr;
}

/**
 * Find the node GameObject of this model
 */
GameObject* ModelImporter::getNodeObject(const std::string& game_object_directory_and_name) {
	for (GameObject* game_object : node_objects) {
		if (game_object->directory_and_name == game_object_directory_and_name) {
			return game_object;
		}
	}
	assert(false);
	return nullptr;
}