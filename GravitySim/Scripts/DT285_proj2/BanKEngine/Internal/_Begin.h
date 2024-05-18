
#include <sstream>
#include <cstdlib>
#include <SDL.h>
#include <GL/glew.h>
#include <GL/gl.h>
#include <glm/glm.hpp>
#include <iostream>
#include <random>
using namespace std;


//Rendering
#include "Rendering/DT285_DrawingCam.h"

//Meshes
#include "Rendering/Mesh/SnubDodecMesh.h"
#include "Rendering/Mesh/CubeMesh.h"
#include "Rendering/Mesh/BanKs_Models.h"


struct Time {
	float Deltatime = 0;
	float TrueDeltatime = 0;
	float Fps = 0;
	float Scale = 1;

	struct Advanced {
		Uint32 lastFrameTime;
		Uint32 lastSecondTime = SDL_GetTicks();
		int framesInLastSecond = 0;
	}Advanced;
	void Calculate() {
		Uint32 currentFrameTime = SDL_GetTicks();
		TrueDeltatime = (currentFrameTime - Advanced.lastFrameTime) / 1000.0f; // Convert to seconds
		Deltatime = TrueDeltatime * Scale;
		Advanced.lastFrameTime = currentFrameTime;


		//int fps = Fps = 1 / Deltatime;
		//cout << endl << "FPS: " << fps;

	}
}Time;




float B_frand(float min, float max) {
	random_device rd;
	mt19937 gen(rd());
	uniform_real_distribution<float> dis(min, max);
	return dis(gen);
}
int B_irand(int min, int max) {
	random_device rd;
	mt19937 gen(rd());
	uniform_int_distribution<int> dis(min, max);
	return dis(gen);
}

float B_distance1D(float A, float B) {
	return abs(A - B);
}
float B_distance2D(glm::vec2& p1, const glm::vec2& p2) {
	float dx = p2.x - p1.x;
	float dy = p2.y - p1.y;
	return sqrt(dx * dx + dy * dy);
}
float B_distance2D(float x1, float y1, float x2, float y2) {
	float dx = x2 - x1;
	float dy = y2 - y1;
	return sqrt(dx * dx + dy * dy);
}
float B_distance3D(const Vector& point1, const Vector& point2) {
	float dx = point2.x - point1.x;
	float dy = point2.y - point1.y;
	float dz = point2.z - point1.z;
	return sqrt(dx * dx + dy * dy + dz * dz);
}

Vector hat(const Vector& v) {
	float length = sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
	if (length == 0) {
		throw invalid_argument("Cannot normalize a zero-length vector");
	}
	return Vector(v.x / length, v.y / length, v.z / length);
}

float B_normalize(float value, float min_val, float max_val) {
	return (value - min_val) / (max_val - min_val);
}
float B_normalize_reversed(float value, float min_val, float max_val) {
	return (value - max_val) / (min_val - max_val);
}
float B_clamp(float value, float minValue, float maxValue) {
	//max(A,B) returns A if A is the max value
	return max(minValue, min(value, maxValue));
}
float B_clampLoop(float value, float minValue, float maxValue) {
	if (value > maxValue) {
		return minValue;
	}
	else if (value < minValue) {
		return maxValue;
	}
	return value;
}
float B_clampSkin(float value, float minValue, float maxValue) {
	float middle = (minValue + maxValue) / 2.0f;
	if (value < middle) { return minValue; }
	return maxValue;
}

float B_lerp(float start, float end, float t) {
	//t = B_clamp(t, 0, 1);
	return start + t * (end - start);
}
float B_SnapToGrid(float& V, float gridSize) {
	return gridSize * roundf(V / gridSize);
}
