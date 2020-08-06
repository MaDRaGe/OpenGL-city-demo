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

// Конструкторы
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
    Параметры: 
        _position - позиция объекта
        _angle - угол поворота по горизонтали
        _color - цвет
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

// Установить позицию
void GraphicObject::setPosition(vec3& _position)
/*
    Параметры:
        _position - позиция
*/
{
	position = _position;
	recalculateModelMatrix();
}

void GraphicObject::setPosition(vec3&& _position)
/*
    Параметры:
        _position - позиция
*/
{
    position = _position;
    recalculateModelMatrix();
}

vec3 GraphicObject::getPosition()
{
	return position;
}

// Установить угол поворота
void GraphicObject::setAngle(float _angle)
/*
    Параметры:
        _angle - угол поворота
*/
{
	angle = _angle;
	recalculateModelMatrix();
}

// Получить угол поворота
float GraphicObject::getAngle()
{
	return angle;
}

// Установить цвет
void GraphicObject::setColor(vec4& _color)
/*
    Параметры:
        _color - цвет
*/
{
    color = _color;
}

void GraphicObject::setColor(vec4&& _color)
/*
    Параметры:
        _color - цвет
*/
{
    color = _color;
}

// Получить цвет
vec4 GraphicObject::getColor()
{
    return color;
}

// Установить ID меша
void GraphicObject::setMeshId(int _meshId)
/*
    Параметры:
        _meshId - ID меша в ResourceManager
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

// Расчет матрицы модели
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
