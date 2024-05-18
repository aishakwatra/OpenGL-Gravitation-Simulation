

#include "BanKEngine/BanKEngine.h"







SnubDodecMesh snub;
GameObj* EarthObj;
GameObj* MoonObj;



float visual_scale = 10;

float earth_radius = 0.5 ;
float moon_radius = 0.3f;
float earth_mass = 200/1000.0f;
float moon_mass = 3/1000.0f;
float G = 6.67430e-11f;

Vector earth_velocity;
Vector moon_velocity;


void Resized(int W, int H) {
	width = W;
	height = H;
	glViewport(0, 0, W, H);
}


void Init(void) {
	glEnable(GL_DEPTH_TEST);
	Resized(width, height);

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	camA.Zoom(2);

	EarthObj = GameObj::Create();
	Edit_Renderer = EarthObj->AddComponent(new Renderer);
	Edit_Renderer->SetMesh(snub);
	Edit_Renderer->Color = Vector(1, 0.6, 0.3);
	EarthObj->Transform.PositionWLD.x = 0;
	EarthObj->Transform.PositionWLD.y = 1;
	EarthObj->Transform.Scale = earth_radius;

	MoonObj = GameObj::Create();
	Edit_Renderer = MoonObj->AddComponent(new Renderer);
	Edit_Renderer->SetMesh(snub);
	Edit_Renderer->Color = Vector(1, 0.8, 0.4);
	MoonObj->Transform.PositionWLD.x = 2;
	MoonObj->Transform.PositionWLD.y = 1;
	MoonObj->Transform.Scale = moon_radius;


	 earth_velocity = Vector(0, -sqrt(G * moon_mass / abs(MoonObj->Transform.PositionWLD - EarthObj->Transform.PositionWLD)), 0);
	 moon_velocity = Vector(0, sqrt(G * earth_mass / abs(MoonObj->Transform.PositionWLD - EarthObj->Transform.PositionWLD)), 0);

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	Rendering::Shader::Initiate();

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
	glClearColor(0.04, 0.02, 0.06, 0);
	glClear(GL_COLOR_BUFFER_BIT);
	// clear the z-buffer
	glClearDepth(1);
	glClear(GL_DEPTH_BUFFER_BIT);

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	camA.LookAt(EarthObj->Transform.PositionWLD);					

	if (Input::Key::W.Pressed) { Time.Scale += Time.TrueDeltatime*10; }
	if (Input::Key::S.Pressed) { Time.Scale -= Time.TrueDeltatime*10; }
	float MultDelta = Time.Deltatime * 200000;

	Vector dVec = MoonObj->Transform.PositionWLD-EarthObj->Transform.PositionWLD; // Calculate Vector From Earth to Moon
	Vector Fg = -(G * earth_mass * moon_mass / abs(dVec)) * (hat(dVec)); //Use Formula
																																												//earth_velocity = earth_velocity + (1 / earth_mass * MultDelta) * Fg ;
																																												//EarthObj->Transform.PositionWLD = EarthObj->Transform.PositionWLD + (MultDelta * earth_velocity);
	moon_velocity = moon_velocity + (1 / moon_mass * MultDelta)* Fg; // Apply Force to Velocity of Moon
	MoonObj->Transform.PositionWLD = MoonObj->Transform.PositionWLD+(MultDelta * moon_velocity);// Apply Velocity of Moon to Update Moon Position

	cout << endl; cout << "Moon " << moon_velocity.x << " " << moon_velocity.y;//Out put Position



		for (Renderer* pInst : sRenderers) {	//Render 
			pInst->Draw(*ViewPort.ActiveCam);
		} 
		ViewPort.ViewPortUpdate();
		Input::Update();
		Time.Calculate();

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
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
			}
		}
		Draw();
		SDL_GL_SwapWindow(window);
	}

	return 0;
}




