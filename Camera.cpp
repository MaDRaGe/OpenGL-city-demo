#include "Camera.h"

/*
     онструктор
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
    // ѕередвинуть
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

    // ѕовернуть
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
    ”становить матрицу проекции

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
    ѕолучить матрицу проекции

    @return:
        матрица проекции
*/
mat4& Camera::getProjectionMatrix()
{
    return projectionMatrix;
}

/*
    ѕолучить матрицу вида

    @return:
       получить матрицу вида

    @requires:
        @field position - известно положение камеры
        @field center - известно положение центра
*/
mat4& Camera::getViewMatrix()
{
    vec3 up = vec3(0, 1, 0);
    globalPosition = relativePosition + center;
    viewMatrix = lookAt(globalPosition, center, up);
    return viewMatrix;
}

/*
    ѕередвинуть камеру в горизонтальной плоскости на указанные значение

    @params:
        dx - значение по оси X
        dz - значение по оси Z
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
    »зменить дистанцию до центра

    @params:
        dR - разница дистанции
*/
void Camera::zoom(float dR)
{
    addDistance(dR);
}

/*
    ¬ычислить положение камеры

    @requires:
        @field vertAngle - известен угол поворота по вертикали
        @field horizAngle - известен угол поворота по горизонтали
        @field distance - известно рассто€ние до центра
*/
void Camera::calcRelativePosition()
{
    relativePosition.y = distance * sin(radians(vertAngle));
    relativePosition.x = distance * cos(radians(vertAngle)) * sin(radians(horizAngle));
    relativePosition.z = distance * cos(radians(vertAngle)) * cos(radians(horizAngle));
}

/*
    ”становить положение камеры

    @params:
        _position - новое положение камеры
*/
void Camera::setPosition(vec3 _position)
{ 
    globalPosition = _position;
}

/*
    ѕолучить положение камеры

    @return:
        положение камеры
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
    ¬ычислить угол поворота по вертикали

    @requires:
        @field position - положение камеры
        @field distance - рассто€ние до центра
*/
void Camera::calcVertAngle()
{
    vertAngle = degrees(asin(relativePosition.y / distance));
}

/*
    ”становить угол поворота по вертикали

    @params:
        _angle - угол поворота по вертикали
*/
void Camera::setVertAngle(float _angle)
{
    if (_angle > 85) _angle = 85;
    if (_angle < 5) _angle = 5;
    vertAngle = _angle;
    calcRelativePosition();
}

/*
    ѕолучить угол поворота по вертикали

    @return:
        @field vertAngle - угол поворота по вертикали
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
    ¬ычислить угол поворота по горизонтали

    @requires:
        известно положение камеры (position)
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
    ”становить угол поворота по горизонтали

    @params:
        _angle - новый угол
*/
void Camera::setHorizAngle(float _angle)
{
    if (_angle > 360) _angle = -360;
    if (_angle < -360) _angle = 360;
    horizAngle = _angle;
    calcRelativePosition();
}

/*
    ѕолучить угол поворота по горизонтали

    @return:
        угол поворота по горизонтали
*/
float Camera::getHorizAngle() 
{ 
    return horizAngle; 
}

/*
    ”величить горизонтальный угол на указанное число градусов

    @params:
        _degree - число градусов
*/
void Camera::addHorizAngle(float _degree)
{
    horizAngle += _degree;
    if (horizAngle > 360) horizAngle = -360;
    if (horizAngle < -360) horizAngle = 360;
    calcRelativePosition();
}

/*
    ¬ычисление дистанции до центра
*/
void Camera::calcDistance()
{
    distance = glm::distance(globalPosition, center);
}

// ”становить рассто€ние до центра
void Camera::setDistance(float _dist)
/*
    @params:
        _dist - новое рассто€ние
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
        рассто€ние до 
*/
{ 
    return distance; 
}

// ”величить дистанцию до центра экрана
void Camera::addDistance(float _deltaDistance)
/*
    @params:
        _deltaDistance - значение, на которое надо увеличить
*/
{
    distance += _deltaDistance;
    if (distance < 1)
    {
        distance = 1;
    }
    calcRelativePosition();
}

