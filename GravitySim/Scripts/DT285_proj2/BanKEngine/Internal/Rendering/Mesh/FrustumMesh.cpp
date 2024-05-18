
#include "FrustumMesh.h"

float FrusSize=2*2.0f;
FrustumMesh::FrustumMesh(float fov, float aspectRatio, float nearPlane, float farPlane) {

    // Calculate half Sizes
    float halfHeightNear = tan(fov / FrusSize) * nearPlane;
    float halfWidthNear = halfHeightNear * aspectRatio;
    float halfHeightFar = tan(fov / FrusSize) * farPlane;
    float halfWidthFar = halfHeightFar * aspectRatio;

    // Define vertices
    ///////////////////////
    vertices[0] = Point(-halfWidthNear, -halfHeightNear, -nearPlane);  // Near bottom left
    vertices[1] = Point(halfWidthNear, -halfHeightNear, -nearPlane);   // Near bottom right
    vertices[2] = Point(halfWidthNear, halfHeightNear, -nearPlane);    // Near top right
    vertices[3] = Point(-halfWidthNear, halfHeightNear, -nearPlane);   // Near top left

    vertices[4] = Point(-halfWidthFar, -halfHeightFar, -farPlane);     // Far bottom left
    vertices[5] = Point(halfWidthFar, -halfHeightFar, -farPlane);      // Far bottom right
    vertices[6] = Point(halfWidthFar, halfHeightFar, -farPlane);       // Far top right
    vertices[7] = Point(-halfWidthFar, halfHeightFar, -farPlane);      // Far top left

    vertices[8] = Point(0.0f, 0.0f,0);                                 // frustum Tip


    //center and dimensions
    center = Point(0.0f, 0.0f, (-nearPlane - farPlane) / FrusSize);
    dimensions = Vector(fabs(halfWidthFar - halfWidthNear),
        fabs(halfHeightFar - halfHeightNear),
        farPlane - nearPlane);
}

int FrustumMesh::VertexCount(void) { return 9; }
Point FrustumMesh::GetVertex(int i) { return vertices[i]; }
Vector FrustumMesh::Dimensions(void) { return dimensions; }
Point FrustumMesh::Center(void) { return center; }
int FrustumMesh::FaceCount(void) { return 12; }
Mesh::Face FrustumMesh::GetFace(int i) { return faces[i]; }
int FrustumMesh::EdgeCount(void) { return 16; }
Mesh::Edge FrustumMesh::GetEdge(int i) { return edges[i]; }


const Mesh::Edge FrustumMesh::edges[16] = {
    Edge(0, 1), Edge(1, 2), Edge(2, 3), Edge(3, 0), // Near plane edges
    Edge(4, 5), Edge(5, 6), Edge(6, 7), Edge(7, 4), // Far plane edges
    Edge(0, 4), Edge(1, 5), Edge(2, 6), Edge(3, 7), // Connecting edges
    Edge(0, 8), Edge(1, 8), Edge(2, 8), Edge(3, 8)  // Edges from vertices to tip
};

const Mesh::Face FrustumMesh::faces[12] = {
    Mesh::Face(0, 1, 2), Mesh::Face(0, 2, 3),   // Near plane faces
    Mesh::Face(6, 5, 4), Mesh::Face(7, 6, 4),   // Far plane faces
    Mesh::Face(5, 1, 0), Mesh::Face(4, 5, 0),   // Side faces
    Mesh::Face(6, 2, 1), Mesh::Face(5, 6, 1),   // Side faces
    Mesh::Face(7, 3, 2), Mesh::Face(6, 7, 2),   // Side faces
    Mesh::Face(4, 0, 3), Mesh::Face(7, 4, 3)    // Side faces
};

