#include "Camera.h"


Camera::Camera(int width, int height)
{
    Width = width;
    Height = height;

    Position = D3DXVECTOR3(0, 0, 0);
}


Camera::~Camera()
{

}

void Camera::SetPosition(float x, float y)
{
    SetPosition(D3DXVECTOR3(x, y, 0));
}

void Camera::SetPosition(D3DXVECTOR3 pos)
{
    Position = pos;
}

D3DXVECTOR3 Camera::GetPosition()
{
    return Position;
}

RECT Camera::GetBound()
{
    RECT bound;

    bound.left = Position.x - Width / 2;
    bound.right = bound.left + Width;;
    bound.top = Position.y - Height / 2;
    bound.bottom = bound.top + Height;

    return bound;
}

int Camera::GetWidth()
{
    return Width;
}

int Camera::GetHeight()
{
    return Height;
}