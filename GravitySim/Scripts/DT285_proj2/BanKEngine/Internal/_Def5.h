#include "_Def4.h"



int width = 600, height = 600;
const char* name = "Final Project First Draft";
double time_last;
double current_time;
int frame_count;
double frame_time;
bool should_close = false;
SDL_Window* window;


//Cam
float aspect = float(width) / float(height);
const Vector ex(1, 0, 0), ey(0, 1, 0), ez(0, 0, 1);
Camera camA(OriginWLD + ez, -ez, ey, 0.5f * Rendering::PI, aspect, 0.01f, 1000);


class ViewPort : public BanKBehavior {
public:
	Camera* ActiveCam;
	vector <Camera*> AllCam;
	int CamIndex = 0;

	ViewPort() {
		AllCam.push_back(&camA);
		ActiveCam = AllCam[0];
	}

	void ViewPortUpdate() {
		//if (Input::Key::Space.OnPressed) {
		//	CamIndex = B_clampLoop(CamIndex+1, 0, AllCam.size()-1);
		//	ActiveCam = AllCam[CamIndex]; 
		//}
	}
}ViewPort;


