#pragma once
#include <glad/glad.h>
#include <assimp/scene.h>
#include <string>
#include <vector>
#include "Mtx.h"

class GameObject;

struct RGBA {
	RGBA();
	RGBA(float r, float g, float b, float a);
	~RGBA();
	float* data();
	float r = 0;
	float g = 0;
	float b = 0;
	float a = 0;
};

struct Vertex {
	mtx::Vec3 Position = mtx::Vec3(0.0f, 0.0f, 0.0f);
	mtx::Vec3 Normal = mtx::Vec3(0.0f, 0.0f, 0.0f);
	mtx::Vec2 TexCoord = mtx::Vec2(0.0f, 0.0f);
};

enum class textureType {
	DIFFUSE = 0x1,
	SPECULAR = 0x2,
	NORMAL = 0x3,
	HEIGHT = 0x4
};

struct Texture {
	std::string path;	// directory + filename
	unsigned int id;
};

struct Material {
	std::string directory_and_name; // directory + name

	float ambient[3] = { 0.1f, 0.1f, 0.1f };
	float diffuse[3] = { 1.0f, 1.0f, 1.0f };
	float specular[3] = { 1.0f, 1.0f, 1.0f };
	float shininess = 1;

	Texture* diffuse_map = nullptr;
	Texture* specular_map = nullptr;
	Texture* normal_map = nullptr;
	Texture* height_map = nullptr;
};

struct LightValues {
	float ambient[4] = { 0.2f, 0.1f, 0.1f, 1.0f };
	float specular[4] = { 1.0f, 1.0f, 1.0f, 0.2f };
	float diffuse[4] = { 1.0f, 1.0f, 1.0f, 0.2f };
	float shininess = 1.0f;
};

struct Bone {
	GameObject* referenced_game_object;
	mtx::Matrix4 inverse_bind_pose;
};

struct VertexBoneData {
	int bone_indices[4];	//indices into the Bone Component's bones vector
	float bone_weights[4] = { 0.0, 0.0, 0.0, 0.0 };
};

struct PositionKeyFrame {
	mtx::Vec3 position;
	float time_stamp;
};

struct ScaleKeyFrame {
	mtx::Vec3 scale;
	float time_stamp;
};

struct RotationKeyFrame {
	mtx::Quaternion rotation;
	float time_stamp;
};

struct AnimationKeyFrames {
	bool is_used = false;
	std::vector<PositionKeyFrame> position_key_frames;
	std::vector<ScaleKeyFrame> scale_key_frames;
	std::vector<RotationKeyFrame> rotation_key_frames;
};

struct Animation {
	std::string name;
	float duration;	// In seconds
};

