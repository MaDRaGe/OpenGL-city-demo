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

// ���� �������� ����������� ������
enum class GraphicObjectConsists
{
    FULL, // ���������
    PART, // ��������
    NONE  // �� ��������
};

/*
    ����������� ������
*/
class Quadtree
{
public:
    void startDraw();
    void finishDraw();

    /*
        ������������
    */
    Quadtree(float _xMin, float _xMax, float _zMin, float _zMax);
    Quadtree();

    /*  
        ������� ���������� �� ���� �� �������
    */
    void out() const;

    /*
        �������� ����������� ������

        @params:
            graphicObject - ����������� ������

        @features:
            - ��������� ����������� ������ � ������� ����, ���� ���� �������� ������
            ��� ���� ������ ����� ����� � ���������� �������� �����
            - ���� ������ ��������� ����� � ����� �� �������� �����, �� ���������
            ������ � ��������������� �������� ����
    */
    void addGraphicObject(GraphicObject graphicObject);

    /*
        ���� ����

        @return:
            true - ���� �������� ������
            false - � ��������� ������
    */
    bool isLeaf();

    /*
        ���� �����

        @params:
            PV - ������� ��������-����

        @return:
            true - ���� ��������� ��� ��������
            false - ���� ��������� �� �����

        @features:
            ���� �����, ���� ��� ���� �� ������� ����� ��� ����
    */
    bool isVisible(mat4& PV);

    /*
        ������� ����

        @params:
            PV - ������� �������� ����
            cameraPosition - ��������� ������

        @features:
            ������� ������� �������� ���� � ������� �������� ����� (���� ��� ����)
    */
    void draw(mat4& PV, vec3& cameraPosition);

    /*
        ������� ���������� �������

        @params:
            PV - ������� ��������-����
            cameraPosition - ��������� ������
    */
    void drawObjects(mat4& PV, vec3& cameraPosition);

private:
    /*
        ������ �������� � ����

        @params:
            border - ������� �������
            
        @return:
            GraphicObjectConsists::FULL - ���������
            GraphicObjectConsists::PART - ��������
            GraphicObjectConsists::NONE - �� ��������
            
    */
    GraphicObjectConsists getGraphicObjectConsists(const vector<vec4>& border);

    /*
        ���� ������������ ������� �� ������� �����������

        @params:
            graphicObject - ����������� ������
            cameraPosition - ��������� ������

        @return:
            true - ������ ������ ����
            false - � ��������� ������

        @features:
            - � ����������� �� ���� ������� � ������������ ��������� �������� ������� ����,
            ���������� ����� ������ ��� ���

    */
    bool leveOfDetailTest(
        GraphicObject& graphicObject,
        vec3& cameraPosition
    );

    /*
        ���� frustum culling ��� ������������ �������

        @params:
            graphicObject - ����������� ������
            PV - ������ �������� ����

        @return:
            true - � ���� ������ ������
            false - � ��������� ������
    */
    bool frustumCullingTest(
        GraphicObject& graphicObject,
        mat4& PV
    );
private:
    // �������� �������
    shared_ptr<Quadtree> childrenQuadtrees[4];

    // ����������� ������� ������� � ������� ����
    vector<GraphicObject> graphicObjects;

    // ����� �������
    float xMin;

    // ������ �������
    float xMax;

    // ������� �������
    float zMin;

    // ������ �������
    float zMax;

    // �������� ����� zMin � zMax
    float zHalf;

    // �������� ����� xMin � zMax
    float xHalf;

    // ������
    float width;

    // ������ 
    float height;
};

