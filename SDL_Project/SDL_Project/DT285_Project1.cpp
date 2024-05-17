// cs250project1.empty.cpp
// -- template for first draft of the final project
// cs250 5/15

#include <sstream>
#include <cstdlib>
#include <SDL.h>
#include <GL/glew.h>
#include <GL/gl.h>
#include <glm/glm.hpp>
#include <iostream>
#include <vector>

#include "SnubDodecMesh.h"
#include "CubeMesh.h"
#include "Projection.h"
#include "DT285_DrawingCam.h"

using namespace std;


int width = 700, height = 700;
const char* name = "Final Project First Draft";
double time_last;
double current_time;
int frame_count;
double frame_time;
bool should_close = false;
SDL_Window* window;

const Point O(0, 0, 0);
const Vector ex(1, 0, 0),
ey(0, 1, 0),
ez(0, 0, 1);
const float PI = 4.0f * atan(1.0f);

std::vector<Camera> cameras;

Camera cam2_0, cam3_0;
int currentCameraIndex = 0;

Camera CAM;

Point cam1_Distance(0, 0, 4.0f);

SnubDodecMesh snub; //earth and moon mesh
Affine snub2worldEarth, snub2worldMoon;

float earth_rot_rate = 2 * PI / 15.0f;
Vector earth_rot_axis(0, 1, 0);
Point earth_center(0, 0, -4);

float moon_rot_rate = 2 * PI / 5.0f;
Vector moon_rot_axis(0, 1, 0);
Point moon_center(0, 0, -6);

bool draw_solid = true;

float moon_scale;
float earth_scale;

float aspect = float(width) / float(height);

//OBJECT COLORS

Vector earthColor = Vector(0.4f, 0.8f, 0.4f);
Vector moonColor = Vector(0.5f, 0.6f, 0.7f);


void Resized(int W, int H) {
	width = W;
	height = H;
	glViewport(0, 0, W, H);
}


void Init(void) {
	glEnable(GL_DEPTH_TEST);
	time_last = float(SDL_GetTicks() / 1000.0f);
	frame_count = 0;
	frame_time = 0;
	current_time = 0;
	Resized(width, height);

	//Full view camera - Cam 1

	Vector pairVector = moon_center - earth_center;
	float pairDistance = abs(pairVector);

	float camDistance = pairDistance * 2.8f;

	Vector cameraPositionVector = camDistance * ez;
	Point cameraPosition = earth_center + cameraPositionVector; 

	Vector lookVector = -ez;

	CAM = Camera(cameraPosition, lookVector, Vector(0, 1, 0), 0.5f * PI, aspect, 0.01f, 20.0f);

	//EARTH & MOON TRANSFORMATIONS

	earth_scale = max(snub.Dimensions().x, max(snub.Dimensions().y, snub.Dimensions().z));

	snub2worldEarth = Trans(earth_center - O) 
					* Scale(2.0f / earth_scale) 
					* Trans(O - snub.Center());

	moon_scale = earth_scale * 2.5f;

	snub2worldMoon = Trans(moon_center - O)
		* Scale(2.0f / moon_scale)
		* Trans(O - snub.Center());

}


void Draw(void) {
	float t = float(SDL_GetTicks() / 1000.0f);
	float dt = t - time_last;
	time_last = t;

	// frame rate
	++frame_count;
	frame_time += dt;
	if (frame_time >= 0.5) {
		double fps = frame_count / frame_time;
		frame_count = 0;
		frame_time = 0;
		stringstream ss;
		ss << name << " [fps=" << int(fps) << "]";
		SDL_SetWindowTitle(window, ss.str().c_str());
	}

	// clear the screen
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	// clear the z-buffer
	glClearDepth(1);
	glClear(GL_DEPTH_BUFFER_BIT);

	// EARTH TRANSFORMATIONS
	snub2worldEarth = Trans(earth_center - O) 
					* Rot(earth_rot_rate * dt, earth_rot_axis) 
					* Trans(O - earth_center) 
					* snub2worldEarth;


	Affine translateToOrigin = Trans(O - earth_center);  //translate to origin (earth center)
	Affine rotateAroundEarth = Rot(moon_rot_rate * dt, Vector(0, 1, 0));  //rotate
	Affine translateBack = Trans(earth_center - O);  // translate back

	Affine moonTransformation = translateBack * rotateAroundEarth * translateToOrigin;

	snub2worldMoon = moonTransformation * snub2worldMoon;

	DisplayFaces(snub, snub2worldEarth, CAM, earthColor);
	DisplayFaces(snub, snub2worldMoon, CAM, moonColor);
	

}

void key_pressed(SDL_Keycode keycode) {
	if (keycode == SDLK_ESCAPE) {
		should_close = true;
	}
	switch (keycode) {
	case '\x1b':
		exit(0);
		break;
	}

}


int main(int argc, char* argv[]) {
	SDL_Init(SDL_INIT_VIDEO);
	window = SDL_CreateWindow(name, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
	SDL_GLContext context = SDL_GL_CreateContext(window);
	Resized(width, height);
	// GLEW
	glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	if (err != GLEW_OK) {
		printf("ERROR: %s\n", glewGetErrorString(err));
	}

	Init();

	while (!should_close) {
		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
			case SDL_QUIT:
				should_close = true;
				break;
			case SDL_KEYDOWN:
				key_pressed(event.key.keysym.sym);
				break;
			}
		}
		Draw();
		SDL_GL_SwapWindow(window);
	}

	return 0;
}




