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

// Сцена
class Scene
{
public:
    //Конструктор
    Scene();

    // Инициализация
    void init(string _filename);

    // Загрузить из xml-файла
    bool loadFromXML(string _filename);

    // Жизнь сцены
    void simulate(float sec);

    // Вывести сцену
    void draw();

    // Получить камеру
    Camera* getCamera();

    // Получить информацию о сцене
    string& getSceneDescription();

    // Загрузить с сервера
    void loadFromServer();

private:
    // Создать объект
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
        Вспомогательные переменные исползуемые в функции finish
    */
    GraphicObjectType type;
    float distance;

    /*
        Вспомогательные переменные исползуемые в функции finish
    */
    string modelId;
    GraphicObject newGraphicObject;

    // Камера
    Camera camera;

    // Свет
    Light light;

    // Графические объекты
    Quadtree quadtree;

    // Документ с моделями
    pugi::xml_document modelsFile;

    // Модели
    xml_node models;

    // Имя документа с графическими объектами
    string sceneFilename;
};

