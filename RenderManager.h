#pragma once
#include "ShaderProgram.h"
#include "Light.h"
#include "Camera.h"
#include "ResourceManager.h"
#include "GraphicObject.h"
#include "Quadtree.h"
#include "FBO.h"
#include "freeglut.h"
#include "glm.hpp"
#include <vector>
#include <string>
#include <map>


using namespace glm;
using std::vector;
using std::string;
using std::map;
enum class ShaderType
{
    GREY,
    SEPIA,
    COMMON,
    GAUSS,
    SIMPLE,
    VERT_GAUSS,
    HORIZ_GAUSS,
    DOF
};
class RenderManager
{
public:
    static RenderManager& instance()
    {
        static RenderManager renderManager;
        return renderManager;
    }

    void init();
    void start();
    void setCamera(Camera* _camera);
    void setLight(Light* _light);
    void addShaderProgram(ShaderType type, ShaderProgram shaderProgram);
    void addToRenderQueue(GraphicObject* graphicObject);
    void addToRenderQueue(vector<GraphicObject>& graphicObjects);
    void drawObjects();
    void finish();
    float getFPS();
    string& getRenderManagerDescription();

    void keyboardFunc(unsigned char& key);
    void reshape(int& width, int& height);

private:
    RenderManager() {};
    RenderManager(const RenderManager& root) = delete;
    RenderManager& operator=(const RenderManager&) = delete;

    void sortGraphicObjects();

    void calcFPS();

    void DrawObject();

private:
    FBO common;
    FBO multisample;
    FBO gaussVert;
    FBO gaussHoriz;
    FBO dof;
    map<ShaderType, ShaderProgram> type_shader;
    vector<ShaderType> shaderQueue;
    
    Camera* camera;
    Light* light;
    vector<GraphicObject*> graphicObjects;
    float fps;
    int drawCallCount;
    int materialChanges;
    string description;

    int renderMode;
    int windowWidth;
    int windowHeight;

    /*
        Вспомогательные переменные исползуемые в функции finish
    */
    int graphicObjectsCount;
    int lastMaterialId;
    vector<mat4> modelViewMatrixes;
    int modelViewMatrixesSize;
    int lastMeshId;
    int count;
    mat4 projectionMatrix;
    mat4 viewMatrix;
    int materialId;
    int meshId;
    int textureBlock;
    mat4 modelViewMatrix;
    Mesh* mesh;
    Material* material;
    Texture* texture;

    /*
        Вспомогательные переменные исползуемые в функции sortGraphicObjects
    */
    int materialId1;
    int materialId2;
    int meshId1;
    int meshId2;

};

