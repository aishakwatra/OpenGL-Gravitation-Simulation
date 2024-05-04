#include "DT285_Drawing.h"
#include <SDL.h>
#include <GL/glew.h>
#include <GL/gl.h>
#include <glm/glm.hpp>

#include <vector>

using namespace std;

vector<float> vertices;
vector<float> colors;
vector<int> indices;



Matrix PerspectiveProjection(float dist) {

	Matrix projectionMatrix;

	projectionMatrix[0] = Hcoords(1.0f, 0.0f, 0.0f, 0.0f);
	projectionMatrix[1] = Hcoords(0.0f, 1.0f, 0.0f, 0.0f);
	projectionMatrix[2] = Hcoords(0.0f, 0.0f, 0.0f, 0.0f);
	projectionMatrix[3] = Hcoords(0.0f, 0.0f, -1.0f/dist, 1.0f);

	return projectionMatrix;

}

//(mesh, cam to world, projection, color)
void DisplayEdges(Mesh& m, const Affine& A, const Matrix& Proj, const Vector& color) {


    vertices.clear();
    indices.clear();
    colors.clear();


    for (int i = 0; i < m.VertexCount(); ++i) {

        Hcoords p = Proj * A * m.GetVertex(i);

        p = (1.0f / p.w) * p;

        vertices.push_back(p.x);
        vertices.push_back(p.y);
        vertices.push_back(p.z);

        colors.push_back(color.x);
        colors.push_back(color.y);
        colors.push_back(color.z);

    }


    for (int i = 0; i < m.EdgeCount(); ++i) {
        Mesh::Edge edge = m.GetEdge(i);

        indices.push_back(edge.index1);
        indices.push_back(edge.index2);

    }

    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, vertices.data());

    glEnableClientState(GL_COLOR_ARRAY);
    glColorPointer(3, GL_FLOAT, 0, colors.data());

    glDrawElements(GL_LINES, m.EdgeCount() * 2, GL_UNSIGNED_INT, indices.data());

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);

}


float Magnitude(Vector v) {

    return std::sqrt((v.x * v.x) + (v.y * v.y) + (v.z * v.z));

}

Point TransformVertex(const Point& vertex, const Matrix& projectionMatrix) {

    // Transform the vertex using homogeneous coordinates
    Hcoords projVertex = projectionMatrix * vertex;

    float w = 1.0f / projVertex.w;

    Point transformedVertex = w * projVertex;

    return transformedVertex;

}



void DisplayFaces(Mesh& m, const Affine& A, const Matrix& Proj, const Vector& color) {


    vertices.clear();
    indices.clear();
    colors.clear();

    //vector for temp verts to calculate face normal and color
    std::vector<Point> temp_verts(m.VertexCount());


    Point cameraPosition(0, 0, 3);
    Vector light(0, 0, 1);
    light.Normalize();

    for (int i = 0; i < m.VertexCount(); ++i) {
        temp_verts[i] = A * m.GetVertex(i);
    }
 
    for (int i = 0; i < m.FaceCount(); ++i) {

        Mesh::Face face = m.GetFace(i);

        //Face Vertices
        Point p1 = temp_verts.at(face.index1);
        Point p2 = temp_verts.at(face.index2);
        Point p3 = temp_verts.at(face.index3);

        //Face Normal
        Vector V1 = (p2 - p1);
        Vector V2 = (p3 - p1);
        Vector normal = cross(V1, V2);
        normal.Normalize();

        Vector viewVector = cameraPosition - p1;
        viewVector.Normalize();

        if (dot(normal, viewVector) > 0) {

            float color_constant = ((dot(light, normal)) / (Magnitude(light) * Magnitude(normal)));

            Vector faceColor = color_constant * color;

            Point P = TransformVertex(temp_verts[face.index1], Proj);
            Point Q = TransformVertex(temp_verts[face.index2], Proj);
            Point R = TransformVertex(temp_verts[face.index3], Proj);

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

            //Add colors

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






