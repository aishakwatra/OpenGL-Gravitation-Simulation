// Projection.h
// -- world-to-camera, and camera-to-ndc transforms
// cs250 5/15

#ifndef DT285_PROJECTION_H
#define DT285_PROJECTION_H

#include "Camera.h"

Affine CameraToWorld(const Camera& cam)
{
	return { cam.Right(), cam.Up(), cam.Back(), cam.Eye() };
}

Affine WorldToCamera(const Camera& cam)
{
	return Inverse(CameraToWorld(cam));
}

Matrix CameraToNDC(const Camera& cam)
{
	Vector viewport = cam.ViewportGeometry();
	float W = viewport.x;
	float H = viewport.y;
	float D = viewport.z;
	float n = cam.NearDistance();
	float f = cam.FarDistance();

	Matrix m;
	m[0][0] = 2 * D / W;
	m[1][1] = 2 * D / H;
	m[2][2] = (n + f) / (n - f);
	m[2][3] = (2 * n * f) / (n - f);
	m[3][2] = -1;

	return m;
}


#endif

