#pragma once

#include <iostream>
using namespace std;

#include <vector>

#include "../Affine/Affine.h"
#include "GL/glew.h"

#include <fstream>
#include <sstream>


float FOV = 1;
const Point OriginWLD(0, 0, 0);

Matrix Pers = [] {
    Matrix make;
    make.row[0] = Hcoords(1, 0, 0, 0);
    make.row[1] = Hcoords(0, 1, 0, 0);
    make.row[2] = Hcoords(0, 0, 1, 0);
    make.row[3] = Hcoords(0, 0, -1, 0);
    return make;
}();


#include"Shader/Vanilla.h"
                        namespace Rendering {

                                    namespace Shader {

                                                GLuint BanK_programID; GLuint VBO;
                                                GLuint vertexShader, fragmentShader;
                                                            string getProjectDirectory() {
                                                                string currentFile(__FILE__);

                                                                size_t found = currentFile.find_last_of("/\\");
                                                                if (found != string::npos) {
                                                                    return currentFile.substr(0, found);
                                                                }
                                                                return "";
                                                            }
                                                            string readShaderSource(const char* filePath) {
                                                                ifstream file(filePath);
                                                                if (!file.is_open()) {
                                                                    cerr << "Failed to open file: " << filePath << endl;
                                                                    return "";
                                                                }

                                                                stringstream buffer;
                                                                buffer << file.rdbuf();
                                                                return buffer.str();
                                                            }
                                                            GLuint createAndLinkShaderProgram(const char* vertexShaderSource, const char* fragmentShaderSource) {
                                                                vertexShader = glCreateShader(GL_VERTEX_SHADER);
                                                                glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
                                                                glCompileShader(vertexShader);

                                                                fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
                                                                glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
                                                                glCompileShader(fragmentShader);

                                                                GLuint shaderProgram = glCreateProgram();
                                                                glAttachShader(shaderProgram, vertexShader);
                                                                glAttachShader(shaderProgram, fragmentShader);
                                                                glLinkProgram(shaderProgram);

                                                                glDeleteShader(vertexShader);
                                                                glDeleteShader(fragmentShader);

                                                                ///// LINK AND USE GenVBO
                                                                glLinkProgram(BanK_programID);
                                                                glUseProgram(BanK_programID);
                                                                glGenBuffers(1, &VBO);// Generate buffer object
                                                                cout << "|created & Linked ShaderProgram|";

                                                                return shaderProgram;
                                                            }
                                                void Initiate() {
                                                    //Shader From Dir
                                                    BanK_programID = 
                                                        Shader::createAndLinkShaderProgram(
                                                            readShaderSource("C:/BanK/KMUTT/Y2T2/CG2 LAB/W7/Scripts/DT285_ProgrammingLab07/Rendering/Shader/Shader.vsh").c_str(), 
                                                            readShaderSource("C:/BanK/KMUTT/Y2T2/CG2 LAB/W7/Scripts/DT285_ProgrammingLab07/Rendering/Shader/Shader.fsh").c_str()
                                                        );
                                                    //Fail-Safe
                                                    BanK_programID = Shader::createAndLinkShaderProgram(vertexShaderSource, fragmentShaderSource);

                                                    glEnable(GL_DEPTH_TEST);
                                                    glDepthFunc(GL_LESS);
                                                }
                                                void Terminate() {
                                                    glDeleteProgram(BanK_programID);
                                                }
                                                                                    namespace ForDraw {
                                                                                        void SetRenderParameters(const Hcoords& color) {

                                                                                            int vertexColorLocation = glGetUniformLocation(BanK_programID, "c_pos");
                                                                                            glUniform4f(vertexColorLocation, color.x, color.y, color.z, 1.0f);
                                                                                        }
                                                                                                            
                                                                                                            int numVertices = 0; 
                                                                                                            void VBO_Load_Points(float pointsVector[],int size) {//overwrites the previous one
                                                                                                                
                                                                                                                glBindBuffer(GL_ARRAY_BUFFER, VBO);

                                                                                                                // Load data into buffer object
                                                                                                                numVertices = size;
                                                                                                                glBufferData(GL_ARRAY_BUFFER, size * sizeof(float), pointsVector, GL_STATIC_DRAW);

                                                                                                                // Unbind buffer object (optional, recommended)
                                                                                                                glBindBuffer(GL_ARRAY_BUFFER, 0);
                                                                                                            }
                                                                                                            void VBO_Draw(int GL_Shape) {
                                                                                                                // Bind the VBO
                                                                                                                glBindBuffer(GL_ARRAY_BUFFER, VBO);

                                                                                                                // Specify the vertex attributes ( 3 floats per vertex)
                                                                                                                glEnableVertexAttribArray(0); // Attribute index 0
                                                                                                                glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

                                                                                                                // Draw the geometry
                                                                                                                glDrawArrays(GL_Shape, 0, numVertices);

                                                                                                                // Clean up
                                                                                                                glDisableVertexAttribArray(0);
                                                                                                                glBindBuffer(GL_ARRAY_BUFFER, 0);
                                                                                                            }
                                                                                    }

                                    }



                                            const float PI = 4.0f * atan(1.0f);
                                            float lineVertices[6];
                                            void DrawLine(const Hcoords& start, const Hcoords& end, const Hcoords& color) {

 
                                                        lineVertices[0] = start.x;
                                                        lineVertices[1] = start.y;
                                                            lineVertices[2] = start.z;
                                                            lineVertices[3] = end.x;
                                                            lineVertices[4] = end.y;
                                                            lineVertices[5] = end.z;


                                                Shader::ForDraw::SetRenderParameters(color);
                                                Shader::ForDraw::VBO_Load_Points(lineVertices,6);
                                                Shader::ForDraw::VBO_Draw(GL_LINES);

                                            }

                                            float triangleVertices[9];
                                            void FillTriangle(const Hcoords& A, const Hcoords& B, const Hcoords& C, const Hcoords& color) {


                                                            triangleVertices[0] = A.x;
                                                            triangleVertices[1] = A.y;
                                                            triangleVertices[2] = A.z;
                                                                triangleVertices[3] = B.x;
                                                                triangleVertices[4] = B.y;
                                                                triangleVertices[5] = B.z;
                                                                    triangleVertices[6] = C.x;
                                                                    triangleVertices[7] = C.y;
                                                                    triangleVertices[8] = C.z;


                                                Shader::ForDraw::SetRenderParameters(color);
                                                Shader::ForDraw::VBO_Load_Points(triangleVertices,9);
                                                Shader::ForDraw::VBO_Draw(GL_TRIANGLES);

                                            }

                                            //homogeneous into Cartesian coordinates.
                                            void convertToScreenSpace(Hcoords& p) {
                                                p.x /= p.w;
                                                p.y /= p.w;
                                                p.z /= p.w;

                                                p.w = 1.0f;
                                            }

                        }
