#include "Camera.h"

/*
    �����������
*/
Camera::Camera()
{ 
    globalPosition = vec3(0, 30, 50);
    center = vec3(0, 0, 0);
    relativePosition = globalPosition - center;
    calcDistance(); 
    calcVertAngle(); 
    calcHorizAngle(); 
}

void Camera::simulate(float time)
{
    // �����������
    float dx = 0;
    float dz = 0;
    if (GetAsyncKeyState('A'))
    {
        dx = -50 * time;
    }
    if (GetAsyncKeyState('D'))
    {
        dx = 50 * time;
    }
    if (GetAsyncKeyState('W'))
    {
        dz = 50 * time;
    }
    if (GetAsyncKeyState('S'))
    {
        dz = -50 * time;
    }
    this->move(dx, dz);

    // ���������
    static POINT point;
    static POINT new_point;
    static bool rbuttonDown = false;
    static bool firstMouseDown = true;
    static float rx = 0;
    static float ry = 0;
    if (GetAsyncKeyState(VK_RBUTTON))
    {
        if (firstMouseDown)
        {
            GetCursorPos(&point);
            firstMouseDown = false;
        }
        if (rbuttonDown)
        {
            GetCursorPos(&new_point);
            rx = new_point.x - point.x;
            ry = point.y - new_point.y;
            point = new_point;
        }
        else
        {
            GetCursorPos(&point);
            rbuttonDown = true;
        }
    }
    else
    {
        rbuttonDown = false;
        rx = 0;
        ry = 0;
    }
    this->rotate(rx, ry);
}

/*
    ���������� ������� ��������

    @params:
        fovy - 
        aspect -
        zNear - 
        zFar - 
*/
void Camera::setProjectionMatrix(float fovy, float aspect, float zNear, float zFar)
{
    projectionMatrix = perspective(radians(fovy), aspect, zNear, zFar);
}

/*
    �������� ������� ��������

    @return:
        ������� ��������
*/
mat4& Camera::getProjectionMatrix()
{
    return projectionMatrix;
}

/*
    �������� ������� ����

    @return:
       �������� ������� ����

    @requires:
        @field position - �������� ��������� ������
        @field center - �������� ��������� ������
*/
mat4& Camera::getViewMatrix()
{
    vec3 up = vec3(0, 1, 0);
    globalPosition = relativePosition + center;
    viewMatrix = lookAt(globalPosition, center, up);
    return viewMatrix;
}

/*
    ����������� ������ � �������������� ��������� �� ��������� ��������

    @params:
        dx - �������� �� ��� X
        dz - �������� �� ��� Z
*/
void Camera::move(float dx, float dz)
{
    float deltaX = (sin(radians(90 - horizAngle)) * dx - sin(radians(horizAngle)) * dz);
    float deltaZ = (-cos(radians(horizAngle)) * dz - cos(radians(90 - horizAngle)) * dx);
    relativePosition.x += deltaX;
    relativePosition.z += deltaZ;
    center.x += deltaX;
    center.z += deltaZ;
}

void Camera::rotate(float _deltaHorizAngle, float _deltaVertAngle)
{
    addHorizAngle(_deltaHorizAngle / 5);
    addVertAngle(_deltaVertAngle / 5);
}

/*
    �������� ��������� �� ������

    @params:
        dR - ������� ���������
*/
void Camera::zoom(float dR)
{
    addDistance(dR);
}

/*
    ��������� ��������� ������

    @requires:
        @field vertAngle - �������� ���� �������� �� ���������
        @field horizAngle - �������� ���� �������� �� �����������
        @field distance - �������� ���������� �� ������
*/
void Camera::calcRelativePosition()
{
    relativePosition.y = distance * sin(radians(vertAngle));
    relativePosition.x = distance * cos(radians(vertAngle)) * sin(radians(horizAngle));
    relativePosition.z = distance * cos(radians(vertAngle)) * cos(radians(horizAngle));
}

/*
    ���������� ��������� ������

    @params:
        _position - ����� ��������� ������
*/
void Camera::setPosition(vec3 _position)
{ 
    globalPosition = _position;
}

/*
    �������� ��������� ������

    @return:
        ��������� ������
*/
vec3& Camera::getPosition()
{ 
    globalPosition = relativePosition + center;
    return globalPosition;
}

void Camera::calcCenter()
{
}

void Camera::setCenter(vec3 _position)
{
    center = _position;
}

vec3 Camera::getCenter()
{
    return center;
}

/*
    ��������� ���� �������� �� ���������

    @requires:
        @field position - ��������� ������
        @field distance - ���������� �� ������
*/
void Camera::calcVertAngle()
{
    vertAngle = degrees(asin(relativePosition.y / distance));
}

/*
    ���������� ���� �������� �� ���������

    @params:
        _angle - ���� �������� �� ���������
*/
void Camera::setVertAngle(float _angle)
{
    if (_angle > 85) _angle = 85;
    if (_angle < 5) _angle = 5;
    vertAngle = _angle;
    calcRelativePosition();
}

/*
    �������� ���� �������� �� ���������

    @return:
        @field vertAngle - ���� �������� �� ���������
*/
float Camera::getVertAngle() 
{ 
    return vertAngle; 
}

void Camera::addVertAngle(float _degree)
{
    vertAngle += _degree;
    if (vertAngle > 85)
    {
        vertAngle = 85;
    }
    if (vertAngle < 5)
    {
        vertAngle = 5;
    }
    calcRelativePosition();
}

/*
    ��������� ���� �������� �� �����������

    @requires:
        �������� ��������� ������ (position)
*/
void Camera::calcHorizAngle()
{
    if (relativePosition.z == 0)
    {
        if (relativePosition.x > 0)
        {
            horizAngle = 90;
        }
        else if (relativePosition.x <= 0)
        {
            horizAngle = 270;
        }
    }
    else
    {
        horizAngle = degrees(atan((relativePosition.x) / (relativePosition.z)));
    }
}

/*
    ���������� ���� �������� �� �����������

    @params:
        _angle - ����� ����
*/
void Camera::setHorizAngle(float _angle)
{
    if (_angle > 360) _angle = -360;
    if (_angle < -360) _angle = 360;
    horizAngle = _angle;
    calcRelativePosition();
}

/*
    �������� ���� �������� �� �����������

    @return:
        ���� �������� �� �����������
*/
float Camera::getHorizAngle() 
{ 
    return horizAngle; 
}

/*
    ��������� �������������� ���� �� ��������� ����� ��������

    @params:
        _degree - ����� ��������
*/
void Camera::addHorizAngle(float _degree)
{
    horizAngle += _degree;
    if (horizAngle > 360) horizAngle = -360;
    if (horizAngle < -360) horizAngle = 360;
    calcRelativePosition();
}

/*
    ���������� ��������� �� ������
*/
void Camera::calcDistance()
{
    distance = glm::distance(globalPosition, center);
}

// ���������� ���������� �� ������
void Camera::setDistance(float _dist)
/*
    @params:
        _dist - ����� ����������
*/
{
    if (_dist < 1)
    {
        _dist = 1;
    }
    distance = _dist;
    calcRelativePosition();
}

float Camera::getDistance() 
/*
    @return:
        ���������� �� 
*/
{ 
    return distance; 
}

// ��������� ��������� �� ������ ������
void Camera::addDistance(float _deltaDistance)
/*
    @params:
        _deltaDistance - ��������, �� ������� ���� ���������
*/
{
    distance += _deltaDistance;
    if (distance < 1)
    {
        distance = 1;
    }
    calcRelativePosition();
}

