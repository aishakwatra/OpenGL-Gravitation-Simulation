#include "DT285_Drawing.h"
#include <SDL.h>
#include <GL/glew.h>
#include <GL/gl.h>
#include <glm/glm.hpp>
#include <iostream>

#include <vector>
#include "Camera.h"
#include "Projection.h"

using namespace std;

vector<float> vertices;
vector<float> colors;
vector<int> indices;

std::vector<Point> temp_verts;


float Magnitude(Vector v) {

    return std::sqrt((v.x * v.x) + (v.y * v.y) + (v.z * v.z));

}


void DisplayEdges(Mesh& mesh, const Affine& obj2world, const Camera& cam, const Vector& clr) {

    Matrix viewMatrix = WorldToCamera(cam) * obj2world;
    Matrix projMatrix = CameraToNDC(cam) * viewMatrix;

    vertices.clear();
    indices.clear();
    colors.clear();

    for (int i = 0; i < mesh.VertexCount(); ++i) {

        Hcoords p = projMatrix * mesh.GetVertex(i);

        p.x = p.x / p.w;
        p.y = p.y / p.w;
        p.z = p.z / p.w;

        vertices.push_back(p.x);
        vertices.push_back(p.y);
        vertices.push_back(p.z);

        colors.push_back(clr.x);
        colors.push_back(clr.y);
        colors.push_back(clr.z);

    }

    for (int i = 0; i < mesh.EdgeCount(); ++i) {
        Mesh::Edge edge = mesh.GetEdge(i);

        Hcoords p1Cam = viewMatrix * mesh.GetVertex(edge.index1);
        Hcoords p2Cam = viewMatrix * mesh.GetVertex(edge.index2);

        if (p1Cam.z >= 0)
            continue;
        if (p2Cam.z >= 0)
            continue;

        indices.push_back(edge.index1);
        indices.push_back(edge.index2);

    }
    

    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, vertices.data());

    glEnableClientState(GL_COLOR_ARRAY);
    glColorPointer(3, GL_FLOAT, 0, colors.data());

    glDrawElements(GL_LINES, indices.size(), GL_UNSIGNED_INT, indices.data());

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);

}


void DisplayFaces(Mesh& mesh, const Affine& obj2world, const Camera& cam, const Vector& clr) {

    vertices.clear();
    indices.clear();
    colors.clear();

    temp_verts.clear();
    temp_verts.resize(mesh.VertexCount());

    Matrix camToNDC = CameraToNDC(cam);
    Matrix worldToCam = WorldToCamera(cam);

    Point cameraPosition = cam.Eye();
    Vector light = cam.Back();
    light.Normalize();

    for (int i = 0; i < mesh.VertexCount(); ++i) {
        temp_verts[i] = obj2world * mesh.GetVertex(i);
    }

    for (int i = 0; i < mesh.FaceCount(); ++i) {

        Mesh::Face face = mesh.GetFace(i);

        //Face Vertices
        Point p1 = temp_verts.at(face.index1);
        Point p2 = temp_verts.at(face.index2);
        Point p3 = temp_verts.at(face.index3);

        // Transform vertices to camera coordinates
        Point p1Camera = worldToCam * p1;
        Point p2Camera = worldToCam * p2;
        Point p3Camera = worldToCam * p3;

        if (p1Camera.z >= 0 || p2Camera.z >= 0 || p3Camera.z >= 0) {
            continue;
        }

        //Face Normal
        Vector normal = cross(p2 - p1, p3 - p1);
        normal.Normalize();

        Vector viewVector = cameraPosition - p1;
        viewVector.Normalize();

        if (dot(normal, viewVector) > 0) {

            float color_constant = ((dot(light, normal)) / (Magnitude(light) * Magnitude(normal)));
            Vector faceColor = color_constant * clr;

            Hcoords P = camToNDC * p1Camera;
            Hcoords Q = camToNDC * p2Camera;
            Hcoords R = camToNDC * p3Camera;

            P.x = P.x / P.w;
            P.y = P.y / P.w;
            P.z = P.z / P.w;

            Q.x = Q.x / Q.w;
            Q.y = Q.y / Q.w;
            Q.z = Q.z / Q.w;

            R.x = R.x / R.w;
            R.y = R.y / R.w;
            R.z = R.z / R.w;

            //Add indices

            int indice_index = (vertices.size() / 3);
            indices.push_back(indice_index + 0);
            indices.push_back(indice_index + 1);
            indices.push_back(indice_index + 2);

            //Add vertices

            vertices.push_back(P.x);
            vertices.push_back(P.y);
            vertices.push_back(P.z);

            vertices.push_back(Q.x);
            vertices.push_back(Q.y);
            vertices.push_back(Q.z);

            vertices.push_back(R.x);
            vertices.push_back(R.y);
            vertices.push_back(R.z);

            for (int i = 0; i < 3; ++i) {
                colors.push_back(faceColor.x);
                colors.push_back(faceColor.y);
                colors.push_back(faceColor.z);
            }
            
        }     

    }

    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, vertices.data());

    glEnableClientState(GL_COLOR_ARRAY);
    glColorPointer(3, GL_FLOAT, 0, colors.data());

    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, indices.data());

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);


}


