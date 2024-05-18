#pragma once
#include <cmath>
#include <cassert>


struct Hcoords {
    float x, y, z, w;
    Hcoords(void);
    Hcoords(float X, float Y, float Z, float W);
    float& operator[](int i) { return *(&x + i); }
    float operator[](int i) const { return *(&x + i); }
    static bool Near(float x, float y) { return std::abs(x - y) < 1e-5f; }
};


struct Point : Hcoords {
    Point(void);
    Point(float X, float Y, float Z);
    Point(const Hcoords& v) : Hcoords(v) { assert(Near(w, 1)); }
};


struct Vector : Hcoords {
    Vector(void);
    Vector(float X, float Y, float Z);
    Vector(const Hcoords& v) : Hcoords(v) { assert(Near(w, 0)); }
    bool Normalize(void);
};


struct Matrix {
    Hcoords row[4];
    Matrix(void);
    Hcoords& operator[](int i) { return row[i]; }
    const Hcoords& operator[](int i) const { return row[i]; }
};


struct Affine : Matrix {
    Affine(void);
    Affine(const Vector& Lx, const Vector& Ly, const Vector& Lz, const Point& D);
    Affine(const Matrix& M) : Matrix(M)
    {
        assert(Hcoords::Near(M[3][0], 0) && Hcoords::Near(M[3][1], 0)
            && Hcoords::Near(M[3][2], 0) && Hcoords::Near(M[3][3], 1));
    }
};


Hcoords operator+(const Hcoords& u, const Hcoords& v);
Hcoords operator-(const Hcoords& u, const Hcoords& v);
Hcoords operator-(const Hcoords& v);
Hcoords operator*(float r, const Hcoords& v);
Hcoords operator*(const Matrix& A, const Hcoords& v);
Matrix operator*(const Matrix& A, const Matrix& B);

float dot(const Vector& u, const Vector& v);
float abs(const Vector& v);
Vector cross(const Vector& u, const Vector& v);

Affine Rot(float t, const Vector& v);
Affine Trans(const Vector& v);
Affine Scale(float r);
Affine Scale(float rx, float ry, float rz);
Affine Inverse(const Affine& A);







#include "Affine.h"



////  Hcoords
/////////////////////////////

Hcoords::Hcoords(void)
    : Hcoords(0, 0, 0, 0)
{}

Hcoords::Hcoords(float X, float Y, float Z, float W)
    : x(X), y(Y), z(Z), w(W)
{}
                Hcoords operator+(const Hcoords& u, const Hcoords& v)
                {
                    return Hcoords{ u.x + v.x, u.y + v.y, u.z + v.z, u.w + v.w };
                }

                Hcoords operator-(const Hcoords& u, const Hcoords& v)
                {
                    return Hcoords{ u.x - v.x, u.y - v.y, u.z - v.z, u.w - v.w };
                }

                Hcoords operator-(const Hcoords& v)
                {
                    return Hcoords{ -v.x, -v.y, -v.z, -v.w };
                }

                Hcoords operator*(float r, const Hcoords& v)
                {
                    return Hcoords{ r * v.x , r * v.y, r * v.z, r * v.w };
                }

                Hcoords operator*(const Matrix& A, const Hcoords& v)
                {
                    Hcoords result;

                    for (int i = 0; i < 4; i++)
                        result[i] = A[i].x * v.x + A[i].y * v.y + A[i].z * v.z + A[i].w * v.w;

                    return result;
                }



////  Points
/////////////////////////////

Point::Point(void)
    : Point(0, 0, 0)
{}

Point::Point(float X, float Y, float Z)
    : Hcoords(X, Y, Z, 1)
{}



////  Vector
/////////////////////////////

Vector::Vector(void)
    : Vector(0, 0, 0)
{}

Vector::Vector(float X, float Y, float Z)
    : Hcoords(X, Y, Z, 0)
{}

                bool Vector::Normalize(void)
                {
                    float length = abs(*this);

                    if (Near(length, 0))
                        return false;

                    x /= length;
                    y /= length;
                    z /= length;

                    return true;
                }
                float dot(const Vector& u, const Vector& v)
                {
                    return (u.x * v.x) + (u.y * v.y) + (u.z * v.z);
                }

                float abs(const Vector& v)
                {
                    return std::sqrt(dot(v, v));
                }

                Vector cross(const Vector& u, const Vector& v)
                {
                    return Vector(u.y * v.z - u.z * v.y, u.z * v.x - u.x * v.z, u.x * v.y - u.y * v.x);
                }



////  Matrix
/////////////////////////////

Matrix::Matrix(void): row{
        { 0, 0, 0, 0 },
        { 0, 0, 0, 0 },
        { 0, 0, 0, 0 },
        { 0, 0, 0, 0 }
    }
{}
                Matrix operator*(const Matrix& A, const Matrix& B)
                {
                    Matrix result;

                    for (int i = 0; i < 4; i++)
                    {
                        for (int j = 0; j < 4; j++)
                        {
                            result[i][j] = 0;

                            for (int k = 0; k < 4; k++)
                                result[i][j] += A[i][k] * B[k][j];
                        }
                    }

                    return result;
                }



////  Affine
/////////////////////////////

Affine::Affine(void)
{
    row[3][3] = 1;
}

Affine::Affine(const Vector& Lx, const Vector& Ly, const Vector& Lz, const Point& D)
{
    row[0] = { Lx.x, Ly.x, Lz.x, D.x };
    row[1] = { Lx.y, Ly.y, Lz.y, D.y };
    row[2] = { Lx.z, Ly.z, Lz.z, D.z };
    row[3][3] = 1;
}

                        Affine Rot(float t, const Vector& v)
                        {
                            float cosT = std::cos(t);

                            float length = abs(v);
                            float co1 = (1.0f - cosT) / (length * length);
                            float co2 = std::sin(t) / length;

                            Vector Lx = co1 * Vector{ v[0] * v[0], v[1] * v[0], v[2] * v[0] };
                            Vector Ly = co1 * Vector{ v[0] * v[1], v[1] * v[1], v[2] * v[1] };
                            Vector Lz = co1 * Vector{ v[0] * v[2], v[1] * v[2], v[2] * v[2] };

                            Vector Lx2 = co2 * Vector{ 0, v.z, -v.y };
                            Vector Ly2 = co2 * Vector{ -v.z, 0, v.x };
                            Vector Lz2 = co2 * Vector{ v.y, -v.x, 0 };

                            Affine R{ Lx + Lx2, Ly + Ly2, Lz + Lz2, {} };

                            R[0][0] += cosT;
                            R[1][1] += cosT;
                            R[2][2] += cosT;

                            return R;
                        }

                        Affine Trans(const Vector& v)
                        {
                            return Affine{ {1, 0, 0}, {0, 1, 0}, {0, 0, 1}, {v.x, v.y, v.z} };
                        }

                        Affine Scale(float r)
                        {
                            return Affine{ {r, 0, 0}, {0, r, 0}, {0, 0, r}, {} };
                        }

                        Affine Scale(float rx, float ry, float rz)
                        {
                            return Affine{ {rx, 0, 0}, {0, ry, 0}, {0, 0, rz}, {} };
                        }



                        Affine Inverse(const Affine& A)
                        {

                            float det = A[0][0] * A[1][1] * A[2][2]
                                + A[0][1] * A[1][2] * A[2][0]
                                + A[0][2] * A[1][0] * A[2][1]
                                - A[2][0] * A[1][1] * A[0][2]
                                - A[2][1] * A[1][2] * A[0][0]
                                - A[2][2] * A[1][0] * A[0][1];

                            Affine inverseTran{
                                {1, 0, 0},
                                {0, 1, 0},
                                {0, 0, 1},
                                {-A[0][3], -A[1][3], -A[2][3]}
                            };

                            Affine adj{
                                        (1 / det) * Vector {
                                            A[1][1] * A[2][2] - A[2][1] * A[1][2],
                                            -(A[1][0] * A[2][2] - A[2][0] * A[1][2]),
                                            A[1][0] * A[2][1] - A[2][0] * A[1][1]
                                        },
                                        (1 / det) * Vector {
                                            -(A[0][1] * A[2][2] - A[2][1] * A[0][2]),
                                            A[0][0] * A[2][2] - A[2][0] * A[0][2],
                                            -(A[0][0] * A[2][1] - A[2][0] * A[0][1])
                                        },
                                        (1 / det) * Vector {
                                            A[0][1] * A[1][2] - A[1][1] * A[0][2],
                                            -(A[0][0] * A[1][2] - A[1][0] * A[0][2]),
                                            A[0][0] * A[1][1] - A[1][0] * A[0][1]
                                        },
                                        {}
                            };

                            return adj * inverseTran;
                        }

