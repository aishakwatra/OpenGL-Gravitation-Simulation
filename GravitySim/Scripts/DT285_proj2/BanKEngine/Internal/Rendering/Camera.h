// -- 3D camera class
// cs250 1/15

#ifndef CS250_CAMERA_H
#define CS250_CAMERA_H

#include "../Affine/Affine.h"

class Camera {
public:
    Camera(void)
        : eye(Point(0, 0, 0))
        , right(Vector(1, 0, 0)), up(Vector(0, 1, 0)), back(Vector(0, 0, 1))
        , width(1), height(1)
        , distance(0.1f), near(0.1f), far(10.0f)
    {}

    Camera(const Point& E, const Vector& look, const Vector& up, float fov, float aspect, float near, float far)
        : eye(E)
        , distance(near), near(near), far(far)
    {
        Vector n = -look;
        n.Normalize();

        Vector u = cross(up, n);
        u.Normalize();

        Vector v = cross(n, u);
        right = u;
        this->up = v;
        back = n;

        width = 2 * distance * std::tan(fov / 2.0f);
        height = width / aspect;
    }

    Point Eye() const
    {
        return eye;
    }

    Vector Right() const
    {
        return right;
    }

    Vector Up() const
    {
        return up;
    }

    Vector Back() const
    {
        return back;
    }

    Vector ViewportGeometry() const
    {
        return Vector(width, height, distance);
    }

    float NearDistance() const
    {
        return near;
    }

    float FarDistance() const
    {
        return far;
    }

    Camera& Zoom(float factor)
    {
        width *= factor;
        height *= factor;
        return *this;
    }

    Camera& Forward(float distance)
    {
        eye = eye + distance * -back;
        return *this;
    }

    Camera& Yaw(float angle)
    {
        Affine rotation = Rot(angle, up);
        right = rotation * right;
        back = rotation * back;
        return *this;
    }

    Camera& Pitch(float angle)
    {
        Affine rotation = Rot(angle, right);
        up = rotation * up;
        back = rotation * back;
        return *this;
    }

    Camera& Roll(float angle)
    {
        Affine rotation = Rot(angle, back);
        right = rotation * right;
        up = rotation * up;
        return *this;
    }

    void LookAt(Point target)
    {
        Vector look = target - eye;
        look.Normalize();

        back = -look;
        right = cross(up, back);
        right.Normalize();
        up = cross(back, right);
    }

    void LookAt(const Affine& target) {
        Point position(target[0].w, target[1].w, target[2].w);
        LookAt(position);
    }

    void EyeMoveTo(Point X) {
        eye = X;
    }

    void EyeMoveTo(Affine& target) {
        Point position(target[0].w, target[1].w, target[2].w);
        eye = position;
    }

    void LookInDirection(Vector direction)
    {
        direction.w = 0;
        direction.Normalize();

        back = -direction; 
        right = cross(up, back);
        right.Normalize();
        //up = cross(back, right);
    }




private:
    Point eye;
    Vector right, up, back;
    float width, height, distance,
        near, far;
};

#endif // CS250_CAMERA_H
