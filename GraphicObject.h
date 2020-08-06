#pragma once
#include <stdio.h>
#include <windows.h>
#include <iostream>
#include <map>

#include "Mesh.h"
#include "Material.h"

#include <gl\GL.h>
#include <gl\GLU.h>
#include "freeglut.h"

#include "glm.hpp"
#include "gtc\matrix_transform.hpp"
#include "gtc\type_ptr.hpp"

using namespace glm;

enum class GraphicObjectType
{
    road,
    building,
    vehicle,
    big_nature,
    small_nature,
    big_prop,
    medium_prop,
    small_prop
};

using std::map;
extern map<string, GraphicObjectType> stringType;

class GraphicObject
{
public:
    // ������������
    GraphicObject();
	GraphicObject(vec3, float, vec4);

    // ���������� / �������� ��� ������
    void setType(GraphicObjectType _type);
    GraphicObjectType getType();

    // ���������� / �������� ������ ������
    void setDimensions(vec3& _dimensions);
    vec3 getDimensions();

    // ���������� / �������� �������
	void setPosition(vec3& position);
    void setPosition(vec3&& position);
	vec3 getPosition();

    // ���������� / �������� ���� �������� �� �����������
	void setAngle(float grad);
	float getAngle();

    // ���������� / �������� ����
    void setColor(vec4& color);
    void setColor(vec4&& color);
    vec4 getColor();

    // ���������� / �������� ID ����
    void setMeshId(int meshId);
    int getMeshId();

    // ���������� / �������� ��������
    void setMaterialId(int _materialId);
    int getMaterialId();

    // �������� ������� ������
    mat4& getModelMatrix();

private:

    // ���
    GraphicObjectType type;

    // ������ ������
    vec3 dimensions;
    // ��������
    int materialId;

    // �������
	vec3 position;

    // ���� �������� �� �����������
	float angle;

    // ����
	vec4 color;

    // ������� ������
    mat4 modelMatrix;

    // ID ����
    int meshId;

    // ��������� ������� ������
	void recalculateModelMatrix();
};