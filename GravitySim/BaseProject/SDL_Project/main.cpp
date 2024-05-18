#include <iostream>
#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <SDL.h>
#include <GL/glew.h>
#include <GL/gl.h>
#include <glm/glm.hpp>
using namespace std;


int draw_mode = 0;

float SUS = -0.5;

//Slower
void draw_basic() {

 // Order Matters
    //      GL_TRIANGLES //- Fastest
    glBegin(GL_POLYGON); //- Flexible but Slower

    glColor3f(1, 0, 0);
    glVertex2f(0,SUS);

    glColor3f(0, 1, 0);
    glVertex2f(1, -1);

    glColor3f(0, 0, 1);
    glVertex2f(0, 1);

    //Define morw vertices depending on GL_shapes
    glColor3f(1, 1, 1);
    glVertex2f(-1, -1);

    glEnd();

}

//Faster
void draw_array() {
    float vertices[] = {
        -1,-1,   
        1, -1,  
        0, 1,
    };
    float colors[] = {
        0.0f, 0.1f, 0.0f,
        0.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 0.0f,
    };

    glEnableClientState(GL_VERTEX_ARRAY);//Position (X,Y)
    glVertexPointer(2, GL_FLOAT, 0, vertices);//(Dimension,type,stride,inputPTR)

    glEnableClientState(GL_COLOR_ARRAY);//Color (R,G,B)
    glColorPointer(3, GL_FLOAT, 0, colors);//(Dimension,type,stride,inputPTR)

    //Draw
    glDrawArrays(GL_TRIANGLES, 0, 3);
}



//Decide Draw Mode
void draw() {
    if (draw_mode == 0) {
        draw_basic();
    }
    else {
        draw_array();
    }
}



void key_pressed(SDL_Keycode keycode) {
    if (keycode == SDLK_SPACE) {
        draw_mode = draw_mode == 1 ? 0 : 1;
    }
    else if (keycode == SDLK_1) 
    {
    }
    else if (keycode == SDLK_2)
    {
    }
    else if (keycode == SDLK_3)
    {
    }
}

int main(int argc, char* argv[]) {

    // SDL: initialize and create a window
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* window = SDL_CreateWindow("AMONGUS", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 500, 500, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
    SDL_GLContext context = SDL_GL_CreateContext(window);


    //// animation loop
    bool done = false;
    while (!done) {
        SDL_Event event;

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Set the clear color (black in this case)
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    done = true;
                    break;
                case SDL_KEYDOWN:
                    if (event.key.keysym.sym == SDLK_ESCAPE)
                        done = true;
                    else
                        key_pressed(event.key.keysym.sym);
                    break;
            }
        }
        draw();
        SUS += 0.0001;
        SDL_GL_SwapWindow(window);
    }



    // clean up
    SDL_GL_DeleteContext(context);
    SDL_Quit();


    //Quit
    return 0;
}

