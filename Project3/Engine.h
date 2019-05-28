#pragma once
#include <memory>
#include <array>

class Engine {
public:
	Engine();
	~Engine();

	void init();
	void start();
	void update();
	void draw();

	int width, height;

private:
	static bool instantiated;
};