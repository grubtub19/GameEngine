#include "data.h"
RGBA::RGBA() : r(0), g(0), b(0), a(0) {}

RGBA::RGBA(float r, float g, float b, float a) : r(r), g(g), b(b), a(a) {}

RGBA::~RGBA() {}

float* RGBA::data() {
	return &r;
}