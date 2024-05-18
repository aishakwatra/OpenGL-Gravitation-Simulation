// CubeMesh.h
// -- axis-aligned cube
// cs250 12/14

#include "Mesh.h"

class CubeMesh : public Mesh {
public:
    int VertexCount(void) { return 8; }
    Point GetVertex(int i) { return vertices[i]; }
    Vector Dimensions(void) { return Vector(2, 2, 2); }
    Point Center(void) { return Point(0, 0, 0); }
    int FaceCount(void) { return 12; }
    Face GetFace(int i) { return faces[i]; }
    int EdgeCount(void) { return 12; }
    Edge GetEdge(int i) { return edges[i]; }
private:
    static Point vertices[8];
    static Face faces[12];
    static Edge edges[12];
};

Mesh::Face CubeMesh::faces[12] = {
    Face(0, 1, 2), Face(3, 0, 2),
    Face(6,7,4), Face(4, 5, 6),
    Face(0,1,5), Face(0, 5, 4),
    Face(6,2,3), Face(6, 3, 7),
    Face(5,1,2), Face(2, 6, 5),
    Face(4,7,3), Face(3, 0, 4)
};

Mesh::Edge CubeMesh::edges[12] = {
    Edge(0, 1), Edge(1, 2), Edge(2, 3), Edge(3, 0),
    Edge(4, 5), Edge(5, 6), Edge(6, 7), Edge(7, 4),
    Edge(0, 4), Edge(1, 5), Edge(6, 2), Edge(3, 7)
};

Point CubeMesh::vertices[8] = {
    Point(-1, -1, -1), Point(1, -1, -1),
    Point(1, 1, -1), Point(-1, 1, -1),
    Point(-1, -1, 1), Point(1, -1, 1),
    Point(1, 1, 1), Point(-1, 1, 1)
};
