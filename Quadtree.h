#pragma once
#include "GraphicObject.h"
#include "RenderManager.h"
#include "glm.hpp"
#include <vector>

using std::vector;
using std::shared_ptr;
using std::make_shared;
using glm::vec3;
using glm::vec4;
using glm::mat4;

// Узел содержит графический объект
enum class GraphicObjectConsists
{
    FULL, // Полностью
    PART, // Частично
    NONE  // Не содержит
};

/*
    Квадратчное дерево
*/
class Quadtree
{
public:
    void startDraw();
    void finishDraw();

    /*
        Конструкторы
    */
    Quadtree(float _xMin, float _xMax, float _zMin, float _zMax);
    Quadtree();

    /*  
        Вывести информацию об узле на консоль
    */
    void out() const;

    /*
        Добавить графический объект

        @params:
            graphicObject - графический объект

        @features:
            - добавляет графический объект в текущий узел, если узел является листом
            или если объект лежит сразу в нескольких дочерних узлах
            - если объект полностью лежит в одном из дочерних узлов, то добавляет
            объект в соответствующий дочерний узел
    */
    void addGraphicObject(GraphicObject graphicObject);

    /*
        Узел лист

        @return:
            true - узел является листом
            false - в противном случае
    */
    bool isLeaf();

    /*
        Узел виден

        @params:
            PV - матрица проекции-вида

        @return:
            true - узел полностью или частично
            false - узел полностью не виден

        @features:
            узел виден, если нет края за которым лежат все углы
    */
    bool isVisible(mat4& PV);

    /*
        Вывести узел

        @params:
            PV - матрица проекции вида
            cameraPosition - положение камеры

        @features:
            выводит объекты текущего узла и объекты дочерних узлов (если они есть)
    */
    void draw(mat4& PV, vec3& cameraPosition);

    /*
        Вывести графичекие объекты

        @params:
            PV - матрица проекции-вида
            cameraPosition - положение камеры
    */
    void drawObjects(mat4& PV, vec3& cameraPosition);

private:
    /*
        Объект попадает в узел

        @params:
            border - хитбокс объекта
            
        @return:
            GraphicObjectConsists::FULL - полностью
            GraphicObjectConsists::PART - частично
            GraphicObjectConsists::NONE - не попадает
            
    */
    GraphicObjectConsists getGraphicObjectConsists(const vector<vec4>& border);

    /*
        Тест графического объекта на уровень детализации

        @params:
            graphicObject - графический объект
            cameraPosition - положение камеры

        @return:
            true - объект прошел тест
            false - в противном случае

        @features:
            - В зависимости от типа объекта и максимальной видимости объектов данного типа,
            определить виден объект или нет

    */
    bool leveOfDetailTest(
        GraphicObject& graphicObject,
        vec3& cameraPosition
    );

    /*
        Тест frustum culling для графического объекта

        @params:
            graphicObject - графический объект
            PV - матриц проекции вида

        @return:
            true - в поле зрение камеры
            false - в противном случае
    */
    bool frustumCullingTest(
        GraphicObject& graphicObject,
        mat4& PV
    );
private:
    // Дочерние деревья
    shared_ptr<Quadtree> childrenQuadtrees[4];

    // Графические объекты лежащие в области узла
    vector<GraphicObject> graphicObjects;

    // Левая граница
    float xMin;

    // Правая граница
    float xMax;

    // Верхняя граница
    float zMin;

    // Нижняя граница
    float zMax;

    // Середина между zMin и zMax
    float zHalf;

    // Середина между xMin и zMax
    float xHalf;

    // Ширина
    float width;

    // Высота 
    float height;
};

