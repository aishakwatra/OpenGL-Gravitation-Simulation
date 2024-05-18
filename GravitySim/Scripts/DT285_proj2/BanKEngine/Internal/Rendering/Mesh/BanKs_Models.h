#pragma once

#include "Mesh.h"
#include <vector>


class MAKO_Mesh : public Mesh {
public:
    MAKO_Mesh() {
        ObjFileReader objReader("BanK_MAKO.obj");
        vertices = objReader.getVertices();
        faces = objReader.getFaces();
        edges = objReader.getEdges();
        dimensions = objReader.calculateDimensions();
        center = objReader.calculateCenter();

        for (const auto& face : objReader.getFaces()) {
            if (face.index1 >= vertices.size() ||  face.index2 >= vertices.size() || face.index3 >= vertices.size()) {
                throw runtime_error("Face index out of range.");
            }
        }

    };
    int VertexCount(void) override { return static_cast<int>(vertices.size()); }
    Point GetVertex(int i) override { return vertices[i]; }
    Vector Dimensions(void) override { return dimensions; }
    Point Center(void) override { return center; }
    int FaceCount(void) override { return static_cast<int>(faces.size()); }
    Face GetFace(int i) override { return faces[i]; }
    int EdgeCount(void) override { return static_cast<int>(edges.size()); }
    Edge GetEdge(int i) override { return edges[i]; }

private:
    vector<Point> vertices;
    vector
        <Face> faces;
    vector<Edge> edges;
    Vector dimensions;
    Point center;
};



class Avilo_Mesh : public Mesh {
public:
    Avilo_Mesh() {
        ObjFileReader objReader("BanK_Avilo.obj");
        vertices = objReader.getVertices();
        faces = objReader.getFaces();
        edges = objReader.getEdges();
        dimensions = objReader.calculateDimensions();
        center = objReader.calculateCenter();

        for (const auto& face : objReader.getFaces()) {
            if (face.index1 >= vertices.size() || face.index2 >= vertices.size() || face.index3 >= vertices.size()) {
                throw runtime_error("Face index out of range.");
            }
        }

    };
    int VertexCount(void) override { return static_cast<int>(vertices.size()); }
    Point GetVertex(int i) override { return vertices[i]; }
    Vector Dimensions(void) override { return dimensions; }
    Point Center(void) override { return center; }
    int FaceCount(void) override { return static_cast<int>(faces.size()); }
    Face GetFace(int i) override { return faces[i]; }
    int EdgeCount(void) override { return static_cast<int>(edges.size()); }
    Edge GetEdge(int i) override { return edges[i]; }

private:
    vector<Point> vertices;
    vector
        <Face> faces;
    vector<Edge> edges;
    Vector dimensions;
    Point center;
};
