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
    // Конструкторы
    GraphicObject();
	GraphicObject(vec3, float, vec4);

    // Установить / Получить тип модели
    void setType(GraphicObjectType _type);
    GraphicObjectType getType();

    // Установить / Получить размер модели
    void setDimensions(vec3& _dimensions);
    vec3 getDimensions();

    // Установить / Получить позицию
	void setPosition(vec3& position);
    void setPosition(vec3&& position);
	vec3 getPosition();

    // Установить / Получить угол поворота по горизонтали
	void setAngle(float grad);
	float getAngle();

    // Установить / Получить цвет
    void setColor(vec4& color);
    void setColor(vec4&& color);
    vec4 getColor();

    // Установить / Получить ID меша
    void setMeshId(int meshId);
    int getMeshId();

    // Установить / Получить материал
    void setMaterialId(int _materialId);
    int getMaterialId();

    // Получить матрицу модели
    mat4& getModelMatrix();

private:

    // Тип
    GraphicObjectType type;

    // Размер модели
    vec3 dimensions;
    // Материал
    int materialId;

    // Позиция
	vec3 position;

    // Угол поворота по горизонтали
	float angle;

    // Цвет
	vec4 color;

    // Матрица модели
    mat4 modelMatrix;

    // ID меша
    int meshId;

    // Расчитать матрицу модели
	void recalculateModelMatrix();
};