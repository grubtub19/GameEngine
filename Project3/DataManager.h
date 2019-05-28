#pragma once
#include <unordered_map>

#include "PoolAllocator.h"
#include "data.h"
/*
#include "GameObject.h"
#include "Transform.h"
#include "Mesh.h"
#include "Camera.h"
#include "Bones.h"
#include "Shader.h"
#include "MeshRenderer.h"
#include "SkinnedMeshRenderer.h"
#include "AnimationController.h"
*/
class GameObject;
class Transform;
class Mesh;
class Camera;
class Bones;
class Skeleton;
class Shader;
class MeshRenderer;
class SkinnedMeshRenderer;
class AnimationController;


class DataManager {
public:
	DataManager();
	~DataManager();

	std::tuple<
		PoolAllocator<GameObject>,
		PoolAllocator<Transform>,
		PoolAllocator<Mesh>,
		PoolAllocator<Camera>,
		PoolAllocator<Material>,
		PoolAllocator<Texture>,
		PoolAllocator<Bones>,
		PoolAllocator<Skeleton>,
		PoolAllocator<Shader>,
		PoolAllocator<MeshRenderer>,
		PoolAllocator<SkinnedMeshRenderer>,
		PoolAllocator<AnimationController>> pools;

	template <typename T>
	T* addData(const T& thing) {
		return std::get<PoolAllocator<T>>(pools).add(thing);
	}

	template <typename T>
	void remove(T* thing) {
		std::get<PoolAllocator<T>>(pools).free(thing);
	}

	template <typename T>
	std::vector<T*> getList() {
		return std::get<PoolAllocator<T>>(pools).used_list;
	}
};