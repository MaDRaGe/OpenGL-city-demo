#include "RenderManager.h"
#include <chrono>

void RenderManager::init()
{
    common.init(700, 700, false);
    multisample.init(700, 700, true);
    gaussVert.init(700, 700, false);
    gaussHoriz.init(700, 700, false);
    windowWidth = 700;
    windowHeight = 700;
    renderMode = 0;
    shaderQueue =
    {
        ShaderType::SIMPLE,
        ShaderType::GREY,
        ShaderType::SEPIA
    };
}

void RenderManager::start()
{
    graphicObjects.clear();
    type_shader[ShaderType::COMMON].activate();

    // Параметры тумана
    vec3 fogColor = vec3(0.5, 0.5, 1.0);
    type_shader[ShaderType::COMMON].setUniform("fogColor", fogColor);
    type_shader[ShaderType::COMMON].setUniform("fogNear", 100);
    type_shader[ShaderType::COMMON].setUniform("fogFar", 400);

    count = 0;
    lastMeshId = -1;
    lastMaterialId = -1;
    modelViewMatrixes.clear();
    graphicObjectsCount = 0;
}

void RenderManager::setCamera(Camera* _camera)
{
    camera = _camera;
}

void RenderManager::setLight(Light* _light)
{
    light = _light;
}

void RenderManager::addShaderProgram(ShaderType type, ShaderProgram shaderProgram)
{
    type_shader.insert(make_pair(type, shaderProgram));
}

void RenderManager::addToRenderQueue(GraphicObject* graphicObject)
{
    graphicObjects.push_back(graphicObject);
    graphicObjectsCount++;
}

void RenderManager::addToRenderQueue(vector<GraphicObject>& _graphicObjects)
{
    for (int i = 0; i < graphicObjects.size(); i++)
    {
        graphicObjects.push_back(&_graphicObjects[i]);
    }
}

void RenderManager::drawObjects()
{
    if (graphicObjects.size() == 0)
    {
        return;
    }
    drawCallCount = 0;
    materialChanges = 0;
    static bool init = true;

    sortGraphicObjects();

    // Установить матрицу проекции
    projectionMatrix = camera->getProjectionMatrix();
    type_shader[ShaderType::COMMON].setUniform("projectionMatrix", projectionMatrix);

    // Получить матрицу камеры
    viewMatrix = camera->getViewMatrix();

    vec3 cameraPos = camera->getPosition();
    type_shader[ShaderType::COMMON].setUniform("cameraPos", cameraPos);

    // Установить параметры света
    type_shader[ShaderType::COMMON].setUniform("lAmbient", light->getAmbient());
    type_shader[ShaderType::COMMON].setUniform("lDiffuse", light->getDiffuse());
    type_shader[ShaderType::COMMON].setUniform("lSpecular", light->getSpecular());
    type_shader[ShaderType::COMMON].setUniform("lPosition", viewMatrix * light->getDirection());

    ResourceManager& resourceManger = ResourceManager::instance();

    // Вывести объекты
    for (int i = 0; i < graphicObjectsCount; i++) // Все объекты
    {
        // Параметры объекта
        materialId = graphicObjects[i]->getMaterialId();
        meshId = graphicObjects[i]->getMeshId();
        modelViewMatrixesSize = modelViewMatrixes.size();

        // Вывести объекты из очереди
        if (   lastMaterialId != materialId      // Новый материал
            || ( lastMeshId != meshId         // Новый меш, старый материал
                 && lastMaterialId == materialId
               )
            || modelViewMatrixesSize == 5        // Очередь заполнена
            )
        {
            count++;
            type_shader[ShaderType::COMMON].setUniform("modelViewMatrix", modelViewMatrixes);

            mesh = resourceManger.getMesh(lastMeshId);
            if (mesh)
            {
                mesh->drawInst(modelViewMatrixes.size());

                // Был совершен еще один draw call
                drawCallCount++;
            }
            modelViewMatrixes.clear();
        }

        // Установить параметры нового материала
        if (lastMaterialId != materialId) // Новый материал
        {
            material = resourceManger.getMaterial(materialId);
            type_shader[ShaderType::COMMON].setUniform("mAmbient", material->getAmbient());
            type_shader[ShaderType::COMMON].setUniform("mDiffuse", material->getDiffuse());
            type_shader[ShaderType::COMMON].setUniform("mSpecular", material->getSpecular());
            type_shader[ShaderType::COMMON].setUniform("mShininess", material->getShininess());
            texture = resourceManger.getTexture(material->getTextureId());
            texture->apply();
            textureBlock = 0;
            int texUnit = 0;
            type_shader[ShaderType::COMMON].setUniform("texture_0", texUnit);

            // Материал был изменен
            materialChanges++;
        }

        // Добавить в очередь новый объект
        modelViewMatrix = viewMatrix * graphicObjects[i]->getModelMatrix();
        modelViewMatrixes.push_back(modelViewMatrix);

        lastMaterialId = materialId;
        lastMeshId = meshId;

    }
    type_shader[ShaderType::COMMON].setUniform("modelViewMatrix", modelViewMatrixes);

    mesh = resourceManger.getMesh(lastMeshId);
    if (mesh)
    {
        mesh->drawInst(modelViewMatrixes.size());

        // Был совершен еще один draw call
        drawCallCount++;
    }
    modelViewMatrixes.clear();
}

void RenderManager::finish()
{
    int texUnit0 = 0;
    int texUnit1 = 1;
    int texUnit2 = 2;
    GLenum texUnitEnum0 = GL_TEXTURE0;
    GLenum texUnitEnum1 = GL_TEXTURE1;
    GLenum texUnitEnum2 = GL_TEXTURE2;

    // Мультисэмпл
    multisample.bind();
    glClearColor(0.5, 0.5, 1.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    drawObjects();

    multisample.resolveToFbo(common);

    // Горизонтальный фильтр Гаусса
    gaussHoriz.bind();
    //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    type_shader[ShaderType::HORIZ_GAUSS].activate();
    common.bindColorTexture();
    type_shader[ShaderType::HORIZ_GAUSS].setUniform("texture_0", texUnit0);
    DrawObject();

    // Вертикальный фильтр гаусса
    gaussVert.bind();
    //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    type_shader[ShaderType::VERT_GAUSS].activate();
    gaussHoriz.bindColorTexture();
    type_shader[ShaderType::VERT_GAUSS].setUniform("texture_0", texUnit0);
    DrawObject();

    // Эффект DOF
    FBO::unbind();
    glViewport(0, 0, windowWidth, windowHeight);
    glDisable(GL_MULTISAMPLE);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    type_shader[ShaderType::DOF].activate();
    type_shader[ShaderType::DOF].setUniform("texture_0", texUnit0);
    type_shader[ShaderType::DOF].setUniform("texture_1", texUnit1);
    type_shader[ShaderType::DOF].setUniform("texture_2", texUnit2);
    type_shader[ShaderType::DOF].setUniform("zNear", 1);
    type_shader[ShaderType::DOF].setUniform("zFar", 800);
    common.bindColorTexture(texUnitEnum0);
    gaussVert.bindColorTexture(texUnitEnum1);
    common.bindDepthTexture(texUnitEnum2);
    DrawObject();

    /*FBO::unbind();
    glViewport(0, 0, windowWidth, windowHeight);
    glDisable(GL_MULTISAMPLE);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    type_shader[shaderQueue[renderMode]].activate();
    type_shader[shaderQueue[renderMode]].setUniform("texture_0", texUnit0);
    common.bindColorTexture();
    DrawObject();*/
}

float RenderManager::getFPS()
{
    calcFPS();
    return fps;
}

string& RenderManager::getRenderManagerDescription()
{
    description = " | objects draw = " + std::to_string(graphicObjects.size()) + " draw call count = " + std::to_string(drawCallCount) 
       + " Material changes = " + std::to_string(materialChanges);
    return description;
}


void RenderManager::keyboardFunc(unsigned char& key)
{
    if (key == '1')
    {
        renderMode++;
        if (renderMode == shaderQueue.size())
        {
            renderMode = 0;
        }
    }
}

void RenderManager::reshape(int& width, int& height)
{
    if (common.reshape(width, height))
    {
        common.init(width, height, false);
    }
    if (multisample.reshape(width, height))
    {
        multisample.init(width, height, true);
    }
    if (gaussVert.reshape(width, height))
    {
        gaussVert.init(width, height, false);
    }
    windowWidth = width;
    windowHeight = height;
}


// Сортировка графических объектов по идентификатору материала и меша
void RenderManager::sortGraphicObjects()
{
    sort(graphicObjects.begin(), graphicObjects.end(), [](GraphicObject* object1, GraphicObject* object2) { 
        if (object1->getMaterialId() < object2->getMaterialId())
        {
            return true;
        }
        else if (object1->getMaterialId() > object2->getMaterialId())
        {
            return false;
        }
        if (object1->getMeshId() < object2->getMeshId())
        {
            return true;
        }
        else
        {
            return false;
        }
        return false;
    });
}

void RenderManager::calcFPS()
{
    static bool init = true;
    static LARGE_INTEGER newValue;
    static LARGE_INTEGER oldValue;
    static LARGE_INTEGER counter;
    if (init)
    {
        QueryPerformanceCounter(&oldValue);
        QueryPerformanceFrequency(&counter);
        init = false;
    }
    else
    {
        QueryPerformanceCounter(&newValue);
        fps = 1 / ((double)(newValue.QuadPart - oldValue.QuadPart) / (double)counter.QuadPart);
        oldValue = newValue;
    }
}

void RenderManager::DrawObject() {
    // переменные для вывода объекта (прямоугольника из двух треугольников)
    static bool init = true;
    static GLuint VAO_Index = 0; // индекс VAO-буфера
    static GLuint VBO_Index = 0; // индекс VBO-буфера
    static int VertexCount = 0; // количество вершин
    // при первом вызове инициализируем VBO и VAO
    if (init) {
        init = false;
        // создание и заполнение VBO
        glGenBuffers(1, &VBO_Index);
        glBindBuffer(GL_ARRAY_BUFFER, VBO_Index);
        GLfloat Verteces[] = {
        -0.5, 0.5,
        -0.5, -0.5,
        0.5, 0.5,
        0.5, 0.5,
        -0.5, -0.5,
        0.5, -0.5
        };
        glBufferData(GL_ARRAY_BUFFER, sizeof(Verteces), Verteces, GL_STATIC_DRAW);
        // создание VAO
        glGenVertexArrays(1, &VAO_Index);
        glBindVertexArray(VAO_Index);
        // заполнение VAO
        glBindBuffer(GL_ARRAY_BUFFER, VBO_Index);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(0);
        // "отвязка" буфера VAO, чтоб случайно не испортить
        glBindVertexArray(0);
        // указание количество вершин
        VertexCount = 6;
    }
    // выводим прямоугольник
    glBindVertexArray(VAO_Index);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    return;
}