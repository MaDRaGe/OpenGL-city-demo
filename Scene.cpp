#include "Scene.h"
using std::cout;
using std::endl;

/*
    �����������
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
    ��������� �� xml-�����

    @params:
        _filename - ��� �����

    @return:
        true - ����� ��������� �������
        false - � ��������� ������
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
        // ������� ����������� ������ � ��������� ��������������� ������
        modelId = object.child("Model").attribute("id").as_string();
        newGraphicObject = createGraphicObject(modelId);

        // ���������� ������� ���������� �������
        stringstream positionStream;
        positionStream << object.child("Position").attribute("value").as_string();
        vec3 position;
        positionStream >> position.x;
        positionStream >> position.y;
        positionStream >> position.z;
        newGraphicObject.setPosition(position);

        // ���������� ���� �������� �������
        newGraphicObject.setAngle(object.child("Rotation").attribute("value").as_float());

        quadtree.addGraphicObject(newGraphicObject);
        graphicObjectsCount++;
        object = object.next_sibling();
    }
    isLoad = true;
    return true;
}


/*
    ����� �����
*/
void Scene::simulate(float sec)
{
    // ������
    camera.simulate(sec);
    for (auto object : shared_data)
    {
        if (objectId_Description.find(object.first) == objectId_Description.end()) // ����� ������
        {
            objectId_Description.insert(object);
            GraphicObject newObject = createGraphicObject(object.second);
            quadtree.addGraphicObject(newObject);
        }
    }
}

/*
    ��������� ������� � renderManager
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
    �������� ������

    @return:
        ������ �����
*/
Camera* Scene::getCamera()
{
    return &camera;
}

/*
    �������� �������� �����

    @return:
        ������ � ��������� �����
*/
string& Scene::getSceneDescription()
{
    description = " | Amount of all objects = " + std::to_string(graphicObjectsCount);
    return description;
}

/*
    ������� ����������� ������ �� �������������� ������

    @params:
        modelId - ������������� ������

    @return:
        ����������� ������
*/
GraphicObject Scene::createGraphicObject(string modelId)
{
    xml_node model = models.find_child_by_attribute("id", modelId.c_str());
    GraphicObject newGraphicObject;

    // ���
    newGraphicObject.setType(stringType[model.attribute("type").as_string()]);

    // ������
    stringstream dimensionsStream; 
    dimensionsStream << model.child("dimensions").attribute("value").as_string();
    vec3 dimensions;
    dimensionsStream >> dimensions.x;
    dimensionsStream >> dimensions.y;
    dimensionsStream >> dimensions.z;
    newGraphicObject.setDimensions(dimensions);

    // ���
    ResourceManager& resourceManager = ResourceManager::instance();
    int meshId = resourceManager.loadMesh(model.child("Mesh").attribute("path").as_string());
    newGraphicObject.setMeshId(meshId);

    // ��������
    int textureId = resourceManager.loadTexture(model.child("Material").child("Texture").attribute("path").as_string());

    // ��������
    xml_node PhongParameters = model.child("Material").child("PhongParameters");
    
    // �������� ������������ 
    stringstream diffuseStream;
    diffuseStream << PhongParameters.attribute("diffuse").as_string();
    vec4 diffuse;
    diffuseStream >> diffuse.r;
    diffuseStream >> diffuse.g;
    diffuseStream >> diffuse.b;
    Material newMaterial;
    newMaterial.setDiffuse(diffuse);

    // ������� ������������
    stringstream ambientStream;
    ambientStream << PhongParameters.attribute("ambient").as_string();
    vec4 ambient;
    ambientStream >> ambient.r;
    ambientStream >> ambient.g;
    ambientStream >> ambient.b;
    newMaterial.setAmbient(ambient);

    // ���������� ������������
    stringstream specularStream;
    specularStream << PhongParameters.attribute("specular").as_string();
    vec4 specular;
    specularStream >> specular.r;
    specularStream >> specular.g;
    specularStream >> specular.b;
    newMaterial.setSpecular(specular);

    // ������������
    newMaterial.setShininess(PhongParameters.attribute("shininess").as_float());

    // ���������� ������������� �������� ��� ���������� ���������
    newMaterial.setTextureId(textureId);
    newGraphicObject.setMaterialId(resourceManager.createSharedMaterial(newMaterial));

    return newGraphicObject;
}

/*
    ������� ����������� ������ �� ��������

    @params:
        objectDescription - �������� ������������ �������

    @return:
        ����������� ������
*/
GraphicObject Scene::createGraphicObject(GraphicObjectDescription& objectDescription)
{

    // ���
    xml_node model = models.find_child_by_attribute("id", objectDescription.model);
    GraphicObject newGraphicObject;
    newGraphicObject.setType(stringType[model.attribute("type").as_string()]);

    // ������
    stringstream dimensionsStream;
    dimensionsStream << model.child("dimensions").attribute("value").as_string();
    vec3 dimensions;
    dimensionsStream >> dimensions.x;
    dimensionsStream >> dimensions.y;
    dimensionsStream >> dimensions.z;
    newGraphicObject.setDimensions(dimensions);

    // ���
    ResourceManager& resourceManager = ResourceManager::instance();
    int meshId = resourceManager.loadMesh(model.child("Mesh").attribute("path").as_string());
    newGraphicObject.setMeshId(meshId);

    // ��������
    int textureId = resourceManager.loadTexture(model.child("Material").child("Texture").attribute("path").as_string());

    // ��������
    xml_node PhongParameters = model.child("Material").child("PhongParameters");

    // �������� ������������ 
    stringstream diffuseStream;
    diffuseStream << PhongParameters.attribute("diffuse").as_string();
    vec4 diffuse;
    diffuseStream >> diffuse.r;
    diffuseStream >> diffuse.g;
    diffuseStream >> diffuse.b;
    Material newMaterial;
    newMaterial.setDiffuse(diffuse);

    // ������� ������������
    stringstream ambientStream;
    ambientStream << PhongParameters.attribute("ambient").as_string();
    vec4 ambient;
    ambientStream >> ambient.r;
    ambientStream >> ambient.g;
    ambientStream >> ambient.b;
    newMaterial.setAmbient(ambient);

    // ���������� ������������
    stringstream specularStream;
    specularStream << PhongParameters.attribute("specular").as_string();
    vec4 specular;
    specularStream >> specular.r;
    specularStream >> specular.g;
    specularStream >> specular.b;
    newMaterial.setSpecular(specular);

    // ������������
    newMaterial.setShininess(PhongParameters.attribute("shininess").as_float());

    // ���������� ������������� �������� ��� ���������� ���������
    newMaterial.setTextureId(textureId);
    newGraphicObject.setMaterialId(resourceManager.createSharedMaterial(newMaterial));
   
    // �������
    vec3 position = vec3(objectDescription.x, objectDescription.y, objectDescription.z);
    newGraphicObject.setPosition(position);

    // ���� ��������
    newGraphicObject.setAngle(objectDescription.yaw);
    return newGraphicObject;
}

/*
    ��������� � �������
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
    for (uint32_t i = 0; i < objectCount; i++) // ��� �������
    {
        GraphicObjectDescription object;
        net.Recv(&object, sizeof(object));
        if (shared_data.find((int)object.objectID) == shared_data.end()) // ������ ��� �� ��� ������
        {
            shared_data.insert(make_pair((int)object.objectID, object));
            /*GraphicObject newObject = createGraphicObject(object);
            quadtree.addGraphicObject(newObject);*/
        }
    }
    isLoad = true;
}
