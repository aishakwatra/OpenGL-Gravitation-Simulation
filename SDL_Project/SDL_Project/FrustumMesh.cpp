#include "FrustumMesh.h"

FrustumMesh::FrustumMesh(float fov, float a, float n, float f) {


	//calculate near plane vertices
	
	float nearWidth = 2 * n * tan(fov / 2);
	float nearHeight = nearWidth / a;

	vertices[0] = Point(nearWidth / 2, nearHeight / 2, -n); //top right
	vertices[1] = Point(nearWidth / 2, -nearHeight / 2, -n); // bottom right
	vertices[2] = Point(-nearWidth / 2, -nearHeight / 2, -n); // bottom left
	vertices[3] = Point(-nearWidth / 2, nearHeight / 2, -n); // top left


	//calculate far plane vertices

	float farWidth = (nearWidth / n) * f;
	float farHeight = farWidth / a;

	vertices[4] = Point(farWidth / 2, farHeight / 2, -f); //top right
	vertices[5] = Point(farWidth / 2, -farHeight / 2, -f); // bottom right
	vertices[6] = Point(-farWidth / 2, -farHeight / 2, -f); // bottom left
	vertices[7] = Point(-farWidth / 2, farHeight / 2, -f); // top left

	//center of projection at the origin
	vertices[8] = Point(0.0f, 0.0f, 0.0f);


}

int FrustumMesh::VertexCount(void) { return 9; }
Point FrustumMesh::GetVertex(int i) { return vertices[i]; }
Vector FrustumMesh::Dimensions(void) { return dimensions; }
Point FrustumMesh::Center(void) { return vertices[8] ; }
int FrustumMesh::FaceCount(void) { return 12; }
Mesh::Face FrustumMesh::GetFace(int i) { return faces[i]; }
int FrustumMesh::EdgeCount(void) { return 16; }
Mesh::Edge FrustumMesh::GetEdge(int i) { return edges[i]; }

const Mesh::Face FrustumMesh::faces[12] = {

	Face(0, 2, 1),
	Face(0, 3, 2),

	Face(6, 7, 4),
	Face(4, 5, 6),

	Face(0, 1, 5),
	Face(0, 5, 4),

	Face(6, 2, 3),
	Face(6, 3, 7),

	Face(5, 1, 2),
	Face(2, 6, 5),

	Face(4, 7, 3),
	Face(3, 0, 4)

};

const Mesh::Edge FrustumMesh::edges[16] = {

	Edge(0, 1),
	Edge(1, 2),
	Edge(2, 3),
	Edge(3, 0),
	Edge(4, 5),
	Edge(5, 6),
	Edge(6, 7),
	Edge(7, 4),

	Edge(0, 4),
	Edge(1, 5),
	Edge(2, 6),
	Edge(3, 7),

	Edge(0, 8),
	Edge(1, 8),
	Edge(2, 8),
	Edge(3, 8)

};





