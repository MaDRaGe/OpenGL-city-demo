#pragma once
#include <Windows.h>
#include <iostream>
#include "glm.hpp"
#include "gtc\matrix_transform.hpp"
#include "gtc\type_ptr.hpp"

using namespace glm;

class Camera{
public:
    // ������������
	Camera();

    /*
        ����� ������
    */
    void simulate(float time);

    // ���������� ������� ��������
    void setProjectionMatrix(float fovy, float aspect, float zNear, float zFar);

    // �������� ������� ��������
    mat4& getProjectionMatrix();

    // �������� ������� ����
    mat4& getViewMatrix();

    // �������� �������
    vec3& getPosition();

    // ����������� � �������������� ���������
    void move(float dx, float dz);

    // ��������� � �������������� � ������������ ����������
    void rotate(float horizAngle, float vertAngle);

    // ���������� ���������� �� ������
    void zoom(float dR);

private:
    // ��������� ���������
    void calcRelativePosition();
	void setPosition(vec3 _position);

    // ��������� ��������� ������
    void calcCenter();
    void setCenter(vec3 _position);
    vec3 getCenter();

    // ��������� ���� �� ���������
    void setVertAngle(float _degree);
    void calcVertAngle();
    float getVertAngle();
    void addVertAngle(float _degree);

    // ��������� ���� �� �����������
    void setHorizAngle(float _degree);
    void calcHorizAngle();
    float getHorizAngle();
    void addHorizAngle(float _degree);

    // ��������� ���������� �� ����� ����������
    void calcDistance();
    void setDistance(float _distance);
    float getDistance();
    void addDistance(float _deltaDistance);

private:
    // ������ �����
    vec3 up;

    // ��������� ������ � ���������� ������� ���������
	vec3 globalPosition;

    // ��������� ������ ������������ ����� ���������� (center)
    vec3 relativePosition;

    // ����� ���������� - ����������� ������� ������
    vec3 center;

    // ���� �������� �� �����������
	float vertAngle;

    // ���� �������� �� ���������
    float horizAngle;

    // ���������� �� ������ �����
	float distance;

    // ������� ��������
    mat4 projectionMatrix;

    // ������� ����
    mat4 viewMatrix;

    // ������� �������� �� �����������
    mat4 horizRotateMatrix;

    // ������� �������� �� ���������
    mat4 vertRotateMatrix;
};