#include "GraphicObject.h"
using namespace std;

map<string, GraphicObjectType> stringType
{
    {"road", GraphicObjectType::road },
    {"building", GraphicObjectType::building },
    {"vehicle", GraphicObjectType::vehicle },
    {"big nature", GraphicObjectType::big_nature },
    {"small nature", GraphicObjectType::small_nature },
    {"big prop", GraphicObjectType::big_prop },
    {"medium prop", GraphicObjectType::medium_prop },
    {"small prop", GraphicObjectType::small_prop }
};

// ������������
GraphicObject::GraphicObject()
{
    position = vec3(0, 0, 0);
    angle = 0;
    color = vec4(0, 0, 0, 0);
    meshId = -1;
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            modelMatrix[i][j] = 0;
        }
    }
    modelMatrix[0][0] = 1;
    modelMatrix[1][1] = 1;
    modelMatrix[2][2] = 1;
    modelMatrix[3][3] = 1;
}
GraphicObject::GraphicObject(
    vec3 _position, 
    float _angle,
    vec4 _color
)
/*
    ���������: 
        _position - ������� �������
        _angle - ���� �������� �� �����������
        _color - ����
*/
{
	position = _position;
    angle = _angle;
    color = _color;
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            modelMatrix[i][j] = 0;
        }
    }
    modelMatrix[0][0] = 1;
    modelMatrix[1][1] = 1;
    modelMatrix[2][2] = 1;
    modelMatrix[3][3] = 1;
    recalculateModelMatrix();
}

void GraphicObject::setType(GraphicObjectType _type)
{
    type = _type;
}

GraphicObjectType GraphicObject::getType()
{
    return type;
}

void GraphicObject::setDimensions(vec3& _dimensions)
{
    dimensions = _dimensions;
}

vec3 GraphicObject::getDimensions()
{
    return dimensions;
}

// ���������� �������
void GraphicObject::setPosition(vec3& _position)
/*
    ���������:
        _position - �������
*/
{
	position = _position;
	recalculateModelMatrix();
}

void GraphicObject::setPosition(vec3&& _position)
/*
    ���������:
        _position - �������
*/
{
    position = _position;
    recalculateModelMatrix();
}

vec3 GraphicObject::getPosition()
{
	return position;
}

// ���������� ���� ��������
void GraphicObject::setAngle(float _angle)
/*
    ���������:
        _angle - ���� ��������
*/
{
	angle = _angle;
	recalculateModelMatrix();
}

// �������� ���� ��������
float GraphicObject::getAngle()
{
	return angle;
}

// ���������� ����
void GraphicObject::setColor(vec4& _color)
/*
    ���������:
        _color - ����
*/
{
    color = _color;
}

void GraphicObject::setColor(vec4&& _color)
/*
    ���������:
        _color - ����
*/
{
    color = _color;
}

// �������� ����
vec4 GraphicObject::getColor()
{
    return color;
}

// ���������� ID ����
void GraphicObject::setMeshId(int _meshId)
/*
    ���������:
        _meshId - ID ���� � ResourceManager
*/
{
    meshId = _meshId;
}

int GraphicObject::getMeshId()
{
    return meshId;
}

void GraphicObject::setMaterialId(int _materialId)
{
    materialId = _materialId;
}

int GraphicObject::getMaterialId()
{
    return materialId;
}

mat4& GraphicObject::getModelMatrix()
{
    return modelMatrix;
}

// ������ ������� ������
void GraphicObject::recalculateModelMatrix()
{
    modelMatrix[0][0] = cos(angle * 3.14 / 180);
    modelMatrix[0][2] = -sin(angle * 3.14 / 180);

    modelMatrix[2][0] = sin(angle * 3.14 / 180);
    modelMatrix[2][2] = cos(angle * 3.14 / 180);

    modelMatrix[3][0] = position.x;
    modelMatrix[3][1] = position.y;
    modelMatrix[3][2] = position.z;
}
