#include "Scene.h"
using std::cout;
using std::endl;

/*
    Конструктор
*/
Scene::Scene()
{
    isLoad = false;
    graphicObjectsCount = 0;
}

void Scene::init(string _filename)
{
    xml_parse_result result = modelsFile.load_file(_filename.c_str());
}

/*
    Загрузить из xml-файла

    @params:
        _filename - имя файла

    @return:
        true - сцена загружена успешно
        false - в противном случае
*/
bool Scene::loadFromXML(string _filename)
{
    models = modelsFile.child("Models");
    xml_document sceneFile;
    sceneFile.load_file(_filename.c_str());
    xml_node scene = sceneFile.child("scene");
    xml_node object = scene.first_child();
    while (object)
    {
        // Создать графический объект с указанным идентификатором модели
        modelId = object.child("Model").attribute("id").as_string();
        newGraphicObject = createGraphicObject(modelId);

        // Установить позицию созданного объекта
        stringstream positionStream;
        positionStream << object.child("Position").attribute("value").as_string();
        vec3 position;
        positionStream >> position.x;
        positionStream >> position.y;
        positionStream >> position.z;
        newGraphicObject.setPosition(position);

        // Установить угол поворота объекта
        newGraphicObject.setAngle(object.child("Rotation").attribute("value").as_float());

        quadtree.addGraphicObject(newGraphicObject);
        graphicObjectsCount++;
        object = object.next_sibling();
    }
    isLoad = true;
    return true;
}


/*
    Жизнь сцены
*/
void Scene::simulate(float sec)
{
    // Камера
    camera.simulate(sec);
    for (auto object : shared_data)
    {
        if (objectId_Description.find(object.first) == objectId_Description.end()) // Новый объект
        {
            objectId_Description.insert(object);
            GraphicObject newObject = createGraphicObject(object.second);
            quadtree.addGraphicObject(newObject);
        }
    }
}

/*
    Загрузить объекты в renderManager
*/
void Scene::draw()
{
    if (isLoad)
    {
        mat4 PV = camera.getProjectionMatrix() * camera.getViewMatrix();
        vec3 cameraPosition = camera.getPosition();
        quadtree.draw(PV, cameraPosition);
    }
}

/*
    Получить камеру

    @return:
        камера сцены
*/
Camera* Scene::getCamera()
{
    return &camera;
}

/*
    Получить описание сцены

    @return:
        строка с описанием сцены
*/
string& Scene::getSceneDescription()
{
    description = " | Amount of all objects = " + std::to_string(graphicObjectsCount);
    return description;
}

/*
    Создать графический объект по идентификатору модели

    @params:
        modelId - идентификатор модели

    @return:
        графический объект
*/
GraphicObject Scene::createGraphicObject(string modelId)
{
    xml_node model = models.find_child_by_attribute("id", modelId.c_str());
    GraphicObject newGraphicObject;

    // Тип
    newGraphicObject.setType(stringType[model.attribute("type").as_string()]);

    // Размер
    stringstream dimensionsStream; 
    dimensionsStream << model.child("dimensions").attribute("value").as_string();
    vec3 dimensions;
    dimensionsStream >> dimensions.x;
    dimensionsStream >> dimensions.y;
    dimensionsStream >> dimensions.z;
    newGraphicObject.setDimensions(dimensions);

    // Меш
    ResourceManager& resourceManager = ResourceManager::instance();
    int meshId = resourceManager.loadMesh(model.child("Mesh").attribute("path").as_string());
    newGraphicObject.setMeshId(meshId);

    // Текстура
    int textureId = resourceManager.loadTexture(model.child("Material").child("Texture").attribute("path").as_string());

    // Материал
    xml_node PhongParameters = model.child("Material").child("PhongParameters");
    
    // Дифузная составляющая 
    stringstream diffuseStream;
    diffuseStream << PhongParameters.attribute("diffuse").as_string();
    vec4 diffuse;
    diffuseStream >> diffuse.r;
    diffuseStream >> diffuse.g;
    diffuseStream >> diffuse.b;
    Material newMaterial;
    newMaterial.setDiffuse(diffuse);

    // Фоновая составляющая
    stringstream ambientStream;
    ambientStream << PhongParameters.attribute("ambient").as_string();
    vec4 ambient;
    ambientStream >> ambient.r;
    ambientStream >> ambient.g;
    ambientStream >> ambient.b;
    newMaterial.setAmbient(ambient);

    // Зеркальная составляющая
    stringstream specularStream;
    specularStream << PhongParameters.attribute("specular").as_string();
    vec4 specular;
    specularStream >> specular.r;
    specularStream >> specular.g;
    specularStream >> specular.b;
    newMaterial.setSpecular(specular);

    // Самосвечение
    newMaterial.setShininess(PhongParameters.attribute("shininess").as_float());

    // Установить идентификатор текстуры для созданного материала
    newMaterial.setTextureId(textureId);
    newGraphicObject.setMaterialId(resourceManager.createSharedMaterial(newMaterial));

    return newGraphicObject;
}

/*
    Создать графический объект по описанию

    @params:
        objectDescription - описание графического объекта

    @return:
        графический объект
*/
GraphicObject Scene::createGraphicObject(GraphicObjectDescription& objectDescription)
{

    // Тип
    xml_node model = models.find_child_by_attribute("id", objectDescription.model);
    GraphicObject newGraphicObject;
    newGraphicObject.setType(stringType[model.attribute("type").as_string()]);

    // Размер
    stringstream dimensionsStream;
    dimensionsStream << model.child("dimensions").attribute("value").as_string();
    vec3 dimensions;
    dimensionsStream >> dimensions.x;
    dimensionsStream >> dimensions.y;
    dimensionsStream >> dimensions.z;
    newGraphicObject.setDimensions(dimensions);

    // Меш
    ResourceManager& resourceManager = ResourceManager::instance();
    int meshId = resourceManager.loadMesh(model.child("Mesh").attribute("path").as_string());
    newGraphicObject.setMeshId(meshId);

    // Текстура
    int textureId = resourceManager.loadTexture(model.child("Material").child("Texture").attribute("path").as_string());

    // Материал
    xml_node PhongParameters = model.child("Material").child("PhongParameters");

    // Дифузная составляющая 
    stringstream diffuseStream;
    diffuseStream << PhongParameters.attribute("diffuse").as_string();
    vec4 diffuse;
    diffuseStream >> diffuse.r;
    diffuseStream >> diffuse.g;
    diffuseStream >> diffuse.b;
    Material newMaterial;
    newMaterial.setDiffuse(diffuse);

    // Фоновая составляющая
    stringstream ambientStream;
    ambientStream << PhongParameters.attribute("ambient").as_string();
    vec4 ambient;
    ambientStream >> ambient.r;
    ambientStream >> ambient.g;
    ambientStream >> ambient.b;
    newMaterial.setAmbient(ambient);

    // Зеркальная составляющая
    stringstream specularStream;
    specularStream << PhongParameters.attribute("specular").as_string();
    vec4 specular;
    specularStream >> specular.r;
    specularStream >> specular.g;
    specularStream >> specular.b;
    newMaterial.setSpecular(specular);

    // Самосвечение
    newMaterial.setShininess(PhongParameters.attribute("shininess").as_float());

    // Установить идентификатор текстуры для созданного материала
    newMaterial.setTextureId(textureId);
    newGraphicObject.setMaterialId(resourceManager.createSharedMaterial(newMaterial));
   
    // Позиция
    vec3 position = vec3(objectDescription.x, objectDescription.y, objectDescription.z);
    newGraphicObject.setPosition(position);

    // Угол поворота
    newGraphicObject.setAngle(objectDescription.yaw);
    return newGraphicObject;
}

/*
    Загрузить с сервера
*/
void Scene::loadFromServer()
{
    models = modelsFile.child("Models");
    NetProtocol& net = NetProtocol::instance();
    net.Send(2);
    cout << "send header" << endl;
    vec3 cameraPosition = camera.getPosition();
    net.SendPosition(cameraPosition.x, cameraPosition.y, cameraPosition.z);
    cout << "send position" << endl;
    net.SendRadius(500.0);
    cout << "send radius" << endl;
    NetworkHeader header;
    net.Recv(&header, sizeof(header));
    cout << "load header" << endl;
    uint32_t objectCount;
    net.Recv(&objectCount, sizeof(objectCount));
    cout << "load count" << (int)objectCount << endl;
    graphicObjectsCount = (int)objectCount;
    cout << "load objects" << endl;
    for (uint32_t i = 0; i < objectCount; i++) // Все объекты
    {
        GraphicObjectDescription object;
        net.Recv(&object, sizeof(object));
        if (shared_data.find((int)object.objectID) == shared_data.end()) // Объект еще не был создан
        {
            shared_data.insert(make_pair((int)object.objectID, object));
            /*GraphicObject newObject = createGraphicObject(object);
            quadtree.addGraphicObject(newObject);*/
        }
    }
    isLoad = true;
}
