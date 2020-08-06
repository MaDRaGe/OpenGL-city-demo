#pragma once
#include <Windows.h>
#include <iostream>
#include "glm.hpp"
#include "gtc\matrix_transform.hpp"
#include "gtc\type_ptr.hpp"

using namespace glm;

class Camera{
public:
    // Конструкторы
	Camera();

    /*
        Жизнь камеры
    */
    void simulate(float time);

    // Установить матрицу проекции
    void setProjectionMatrix(float fovy, float aspect, float zNear, float zFar);

    // Получить матрицу проекции
    mat4& getProjectionMatrix();

    // Получить матрицу вида
    mat4& getViewMatrix();

    // Получить позицию
    vec3& getPosition();

    // Передвинуть в горизонтальной плоскости
    void move(float dx, float dz);

    // Повернуть в горизонтальной и вертикальной плоскостях
    void rotate(float horizAngle, float vertAngle);

    // Измененить расстояние до центра
    void zoom(float dR);

private:
    // Обработка положения
    void calcRelativePosition();
	void setPosition(vec3 _position);

    // Обработка положения центра
    void calcCenter();
    void setCenter(vec3 _position);
    vec3 getCenter();

    // Обработка угла по вертикали
    void setVertAngle(float _degree);
    void calcVertAngle();
    float getVertAngle();
    void addVertAngle(float _degree);

    // Обработка угла по горизонтали
    void setHorizAngle(float _degree);
    void calcHorizAngle();
    float getHorizAngle();
    void addHorizAngle(float _degree);

    // Обработка расстояния до точки наблюдения
    void calcDistance();
    void setDistance(float _distance);
    float getDistance();
    void addDistance(float _deltaDistance);

private:
    // Вектор вверх
    vec3 up;

    // Положение камеры в глобальной системе координат
	vec3 globalPosition;

    // Положение камеры относительно точки наблюдения (center)
    vec3 relativePosition;

    // Точка наблюдения - направление взгляда камеры
    vec3 center;

    // Угол поворота по горизонтали
	float vertAngle;

    // Угол поворота по вертикали
    float horizAngle;

    // Расстояние до центра сцены
	float distance;

    // Матрица проекции
    mat4 projectionMatrix;

    // Матрица вида
    mat4 viewMatrix;

    // Матрица поворота по горизонтали
    mat4 horizRotateMatrix;

    // Матрица поворота по вертикали
    mat4 vertRotateMatrix;
};