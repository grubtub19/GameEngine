#include "DataManager.h"
#include "Component.h"

DataManager::DataManager()  :
	pools(std::make_tuple(
		PoolAllocator<GameObject>(200),
		PoolAllocator<Transform>(200),
		PoolAllocator<Mesh>(200),
		PoolAllocator<Camera>(2),
		PoolAllocator<Material>(20),
		PoolAllocator<Texture>(20),
		PoolAllocator<Bones>(20),
		PoolAllocator<Skeleton>(40),
		PoolAllocator<Shader>(2),
		PoolAllocator<MeshRenderer>(20),
		PoolAllocator<SkinnedMeshRenderer>(20),
		PoolAllocator<AnimationController>(4)))
{
	Logger::log("DataManager Loaded");
}

DataManager::~DataManager() {
	Logger::log("DataManager Destruction");
}

