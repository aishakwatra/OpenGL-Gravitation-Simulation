// cs250_h4.h
// -- display edges/faces of a mesh, second version
// cs250 1/15

#ifndef DT285_DRAWING_CAM_H
#define DT285_DRAWING_CAM_H


#include "Camera.h"
#include "Mesh/Mesh.h"

#include "BanK_Rendering.h"
#include "Projection.h"



Matrix modelView;
Matrix modelViewProj;
void DisplayEdges(Mesh& mesh, const Affine& obj2world, const Camera& cam, const Vector& clr)
{
    modelView = WorldToCamera(cam) * obj2world;
    modelViewProj = CameraToNDC(cam) * modelView;

    for (int i = 0; i < mesh.EdgeCount(); ++i)
    {
        Mesh::Edge edge = mesh.GetEdge(i);

        Hcoords Cullp1 = modelView * mesh.GetVertex(edge.index1);
        if (Cullp1.z >= 0) { continue; }
        Hcoords Cullp2 = modelView * mesh.GetVertex(edge.index2);
        if (Cullp2.z >= 0) { continue; }

        Hcoords p1 = modelViewProj * mesh.GetVertex(edge.index1); Rendering::convertToScreenSpace(p1);
        Hcoords p2 = modelViewProj * mesh.GetVertex(edge.index2); Rendering::convertToScreenSpace(p2);

        Rendering::DrawLine(p1, p2, clr);
    }
}


float Strength_D = 0.8;
float Strength_A = 1.32 - Strength_D;
Vector LightSource(-1.5f, 1, 0);

Matrix P_W2C;
Matrix P_C2N;
Matrix P_W2N;

void DisplayFaces(Mesh& m, const Affine& obj2world, const Camera& cam, const Vector& color) {

     P_W2C = WorldToCamera(cam);
     P_C2N = CameraToNDC(cam);
     P_W2N = P_C2N * P_W2C;

            for (int n = 0; n < m.FaceCount(); ++n) {

                Mesh::Face face = m.GetFace(n);
                Hcoords P = obj2world * m.GetVertex(face.index1);
                Hcoords Q = obj2world * m.GetVertex(face.index2);
                Hcoords R = obj2world * m.GetVertex(face.index3);

                //Cam Behind Check
                Hcoords viewP = P_W2C * P;
                if (viewP.z >= 0) { continue; }
                Hcoords viewQ = P_W2C * Q;
                if (viewQ.z >= 0) { continue; }
                Hcoords viewR = P_W2C * R;
                if (viewR.z >= 0) { continue; }



                Vector V1 = (Q - P); Vector V2 = (R - P);
                Vector Normalc = cross(V1, V2);
                Point Viewer(cam.Eye());

                        // Culling
                        if (dot(Normalc, Viewer - P) > 0) {

                            // Normal For Light
                            Vector Normalb = cross(Q - P, R - P);
                            float LightValue = dot(LightSource, Normalb) / (abs(LightSource) * abs(Normalb));

                            // Apply Perspective Projection
                            Hcoords P_proj = P_W2N * P;                            Rendering::convertToScreenSpace(P_proj);
                            Hcoords Q_proj = P_W2N * Q;                            Rendering::convertToScreenSpace(Q_proj);
                            Hcoords R_proj = P_W2N * R;                            Rendering::convertToScreenSpace(R_proj);

                            Rendering::FillTriangle(P_proj, Q_proj, R_proj, Strength_D * LightValue * color + Strength_A * color);
                        }
            }
}








#endif