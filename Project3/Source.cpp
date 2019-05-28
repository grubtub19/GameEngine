#include "Game.h"
int main(void)
{
	Game::get().start();
}
/*
void project1() {
	Logger::log("\n------------------------------------------------------------");
	Logger::log("Startup of Components");
	Logger::log("------------------------------------------------------------\n");
	Game::get();
	Logger::log("");
	Logger::log("------------------------------------------------------------");
	Logger::log("Allocate a large number of vertices. Rotate, translate and scale them");
	Logger::log("------------------------------------------------------------\n");
	mtx::Quaternion q;
	q.make_rotation(mtx::Vec3(1.0f, 1.0f, 0.0f), mtx::to_radians(60));
	Logger::log("60 degree rotation around Vec3(1,1,0) is Quaternion:");
	Logger::log(q.to_string());
	mtx::Matrix4 rotation;
	rotation.make_rotation(q);
	Logger::log("Rotation Matrix of Quaternion:");
	Logger::log(rotation.to_string());
	mtx::Matrix4 translation;
	translation.make_translation(mtx::Vec3(20.0f, 30.0f, 40.0f));
	Logger::log("Translation Matrix of Vec3(20,30,40):");
	Logger::log(translation.to_string());
	mtx::Matrix4 scale;
	scale.make_scale(2.0f, 3.0f, 4.0f);
	Logger::log("Scale Matrix of Vec3(2,3,4):");
	Logger::log(scale.to_string());

	Logger::log("Allocating 50000 Vec3's");
	PoolAllocator<mtx::Vec3> vec3_pool = PoolAllocator<mtx::Vec3>(50000);
	mtx::Vec3* vec3_array[100000];
	for (int i = 0; i < 100000; i++) {
		vec3_array[i] = new (vec3_pool.get_memory()) mtx::Vec3(1.0f, 2.0f, 3.0f);
	}
	Logger::log("");

	for (int i = 0; i < 100000; i += 10000) {
		Logger::log("rotation * translation * scale * vec3_array[" + std::to_string(i) + "]:");
		Logger::log((rotation * translation * scale * (*vec3_array[i])).to_string());
	}

	Logger::log("------------------------------------------------------------");
	Logger::log("Rotate vectors with a set of quaternions then use the inverse to rotate back");
	Logger::log("------------------------------------------------------------\n");

	Logger::log("Quaternion q1:");
	mtx::Quaternion q1 = mtx::Quaternion();
	q1.make_rotation(mtx::Vec3(0, 1, 0), mtx::to_radians(160));
	Logger::log(q1.to_string());

	Logger::log("Quaternion q2:");
	mtx::Quaternion q2 = mtx::Quaternion();
	q2.make_rotation(mtx::Vec3(0, 0, 1), mtx::to_radians(40));
	Logger::log(q2.to_string());

	mtx::Vec3 v = mtx::Vec3(1, 1, 1);
	Logger::log("Vec3 v:");
	Logger::log(v.to_string());

	v.rotate(q1);
	Logger::log("v.rotate(q1):");
	Logger::log(v.to_string());

	v.rotate(q2);
	Logger::log("v.rotate(q2):");
	Logger::log(v.to_string());

	v.rotate(q2.invert());
	Logger::log("v.rotate(q2.inverse()):");
	Logger::log(v.to_string());

	v.rotate(q1.invert());
	Logger::log("v.rotate(q1.inverse()):");
	Logger::log(v.to_string());

	Logger::log("------------------------------------------------------------");
	Logger::log("Shutdown of Components");
	Logger::log("------------------------------------------------------------\n");

	std::cin.ignore();
}
*/