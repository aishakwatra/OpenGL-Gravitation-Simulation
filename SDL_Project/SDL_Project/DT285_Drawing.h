// DT285_Drawing.h

#ifndef DT285_DRAWING_H
#define DT285_DRAWING_H

#include <cstdlib>
#include "Affine.h"
#include "Mesh.h"



void InitBuffer();

Matrix PerspectiveProjection(float dist);

void DisplayEdges(Mesh& m, const Affine& A, const Matrix& Proj, const Vector& color);

void DisplayFaces(Mesh& m, const Affine& A, const Matrix& Proj, const Vector& color);


#endif
