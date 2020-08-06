#pragma once
#include <string>
#include <sstream>
#include <vector>
#include <map>
#include "RenderManager.h"
#include "ResourceManager.h"
#include "GraphicObject.h"
#include "Camera.h"
#include "Light.h"
#include "Material.h"
#include "Quadtree.h"
#include "NetProtocol.h"
#include "pugixml.hpp"
#include "glm.hpp"
#include <omp.h>

using std::stringstream;
using std::string;
using std::vector;
using std::map;
using std::make_pair;
using glm::vec3;
using glm::vec4;
using namespace pugi;

// �����
class Scene
{
public:
    //�����������
    Scene();

    // �������������
    void init(string _filename);

    // ��������� �� xml-�����
    bool loadFromXML(string _filename);

    // ����� �����
    void simulate(float sec);

    // ������� �����
    void draw();

    // �������� ������
    Camera* getCamera();

    // �������� ���������� � �����
    string& getSceneDescription();

    // ��������� � �������
    void loadFromServer();

private:
    // ������� ������
    GraphicObject createGraphicObject(string modelId);
    GraphicObject createGraphicObject(GraphicObjectDescription& objectDescription);

private:
    bool isLoad;
    int countObjectsToRender;
    int graphicObjectsCount;
    string description;
    map<int, GraphicObjectDescription> objectId_Description;
    map<int, GraphicObjectDescription> shared_data;

    /*
        ��������������� ���������� ����������� � ������� finish
    */
    GraphicObjectType type;
    float distance;

    /*
        ��������������� ���������� ����������� � ������� finish
    */
    string modelId;
    GraphicObject newGraphicObject;

    // ������
    Camera camera;

    // ����
    Light light;

    // ����������� �������
    Quadtree quadtree;

    // �������� � ��������
    pugi::xml_document modelsFile;

    // ������
    xml_node models;

    // ��� ��������� � ������������ ���������
    string sceneFilename;
};

