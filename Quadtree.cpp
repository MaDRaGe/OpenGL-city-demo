#include "Quadtree.h"

void Quadtree::startDraw()
{
}

void Quadtree::finishDraw()
{
}

/*
    Конструктор

    @params:
        _xMin - минимальная граница по оси X
        _xMax - максимальная граница по оси X
        _zMin - минимальная граница по оси Z
        _zMax - максимальная граница по оси Z

    @features:
        Если узел не достаточно маленький,
        то находятся дочерние узлы меньшего размера
*/
Quadtree::Quadtree(
    float _xMin, 
    float _xMax, 
    float _zMin, 
    float _zMax
)
{
    xMin = _xMin;
    xMax = _xMax;
    xHalf = (int)((_xMax - _xMin) / 2 + _xMin);
    zMin = _zMin;
    zMax = _zMax;
    zHalf = (int)((_zMax - _zMin) / 2 + _zMin);
    width = _xMax - _xMin;
    height = _zMax - _zMin;
    
    if (    width > 16  // Узел большой
         && height > 16
       )
    {
        // Найти дочерние узлы меньшего размера
        childrenQuadtrees[0] = make_shared<Quadtree>(_xMin, xHalf, _zMin, zHalf);
        childrenQuadtrees[1] = make_shared<Quadtree>(xHalf, _xMax, _zMin, zHalf);
        childrenQuadtrees[2] = make_shared<Quadtree>(_xMin, xHalf, zHalf, _zMax);
        childrenQuadtrees[3] = make_shared<Quadtree>(xHalf, _xMax, zHalf, _zMax);
    }
}

Quadtree::Quadtree()
    : Quadtree(-1000, 1000, -1000, 1000)
{
}

/*
    Вывести информацию об узле на консоль

    @requers:
        width - ширина узла
        height - высота узла
*/
void Quadtree::out() const
{
    printf_s("Width = %f Height = %f\n", width, height);
    for (int i = 0; i < 4; i++)
    {
        if (childrenQuadtrees[i])
        {
            childrenQuadtrees[i]->out();
        }
    }
}

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
void Quadtree::addGraphicObject(GraphicObject graphicObject)
{
    if (isLeaf()) 
    {
        graphicObjects.push_back(graphicObject);
    }
    else
    {
        vec3 graphicObjectDimensions = graphicObject.getDimensions();
        mat4 modelMatrix = graphicObject.getModelMatrix();
        vector<vec4> hitbox = {
            modelMatrix * vec4(+graphicObjectDimensions.x / 2, +graphicObjectDimensions.y / 2, +graphicObjectDimensions.z / 2, 1.0),
            modelMatrix * vec4(+graphicObjectDimensions.x / 2, +graphicObjectDimensions.y / 2, -graphicObjectDimensions.z / 2, 1.0),
            modelMatrix * vec4(+graphicObjectDimensions.x / 2, -graphicObjectDimensions.y / 2, +graphicObjectDimensions.z / 2, 1.0),
            modelMatrix * vec4(+graphicObjectDimensions.x / 2, -graphicObjectDimensions.y / 2, -graphicObjectDimensions.z / 2, 1.0),
            modelMatrix * vec4(-graphicObjectDimensions.x / 2, +graphicObjectDimensions.y / 2, +graphicObjectDimensions.z / 2, 1.0),
            modelMatrix * vec4(-graphicObjectDimensions.x / 2, +graphicObjectDimensions.y / 2, -graphicObjectDimensions.z / 2, 1.0),
            modelMatrix * vec4(-graphicObjectDimensions.x / 2, -graphicObjectDimensions.y / 2, +graphicObjectDimensions.z / 2, 1.0),
            modelMatrix * vec4(-graphicObjectDimensions.x / 2, -graphicObjectDimensions.y / 2, -graphicObjectDimensions.z / 2, 1.0)
        };

        // Будем знать, когда один из дочерних узлов содержит объект
        bool isChildrenConsistHitbox = false;

        GraphicObjectConsists consist;
        for (int i = 0; i < 4; i++) // Все дочерние узлы
        {
            consist = childrenQuadtrees[i]->getGraphicObjectConsists(hitbox);
            if (consist == GraphicObjectConsists::FULL) // Объект полность входит в дочерний узел
            {
                childrenQuadtrees[i]->addGraphicObject(graphicObject);
                return;
            }
            else if (consist == GraphicObjectConsists::PART) // Объект частично входит в дочерний узел
            {
                if (isChildrenConsistHitbox) // Объект уже попадал в какой-то дочерний узел
                {
                    graphicObjects.push_back(graphicObject);
                    return;
                }
                else
                {
                    // Графический объект частично попадает в один из дочерних улов
                    isChildrenConsistHitbox = true;
                }
            }
        }
    }
}

/*
    Узел лист
    
    @return:
        true - узел является листом
        false - в противном случае
*/
bool Quadtree::isLeaf()
{
    if (!childrenQuadtrees[0])
    {
        return true;
    }
    return false;
}

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
bool Quadtree::isVisible(mat4& PV)
{
    vector<vec4> hitbox = // Углы узла
    {
        PV * vec4(xMin, 1, zMin, 1.0),
        PV * vec4(xMax, 1, zMin, 1.0),
        PV * vec4(xMin, 1, zMax, 1.0),
        PV * vec4(xMax, 1, zMax, 1.0)
    };

    // Обработка углов узла
    int countVertexOutLeft = 0;  // Количество углов лежащих за левым краем
    int countVertexOutRight = 0; // Количество углов лежащих за правым краем
    int countVertexOutFar = 0;   // Количество углов лежащих за дальним краем
    int countVertexOutNear = 0;  // Количество углов лежащих за ближнем
    for (int i = 0; i < 4; i++) // Все углы
    {
        if (hitbox[i].x < -hitbox[i].w) // Угол за левым краем 
        {
            countVertexOutLeft++;
        }
        if (hitbox[i].x > hitbox[i].w) // Угол за правым краем
        {
            countVertexOutRight++;
        }
        if (hitbox[i].z < -hitbox[i].w) // Угол за дальним краем
        {
            countVertexOutFar++;
        }
        if (hitbox[i].z > hitbox[i].w) // Угол за ближним краем
        {
            countVertexOutNear++;
        }
    }
    return (    countVertexOutNear < 4
             && countVertexOutFar < 4
             && countVertexOutLeft < 4
             && countVertexOutRight < 4
           );
}

/*
    Вывести объекты
    
    @params:
        PV - матрица проекции вида
        cameraPosition - положение камеры
    
    @features:
        выводит объекты текущего узла и объекты дочерних узлов (если они есть)
*/
void Quadtree::draw(
    mat4& PV, 
    vec3& cameraPosition
)
{
    if (isVisible(PV)) // Узел виден
    {
        // Вывести объекты
        drawObjects(PV, cameraPosition);
    
        // Вывести объекты дочерних узлов
        if (!isLeaf()) // Узел не лист
        {
            for (int i = 0; i < 4; i++)
            {
                childrenQuadtrees[i]->draw(PV, cameraPosition);
            }
        }
    }
}

/*
    Вывести графичекие объекты
    
    @params:
        PV - матрица проекции-вида
        cameraPosition - положение камеры

    @features:
        - выводит графические объекты предварительно проведя тесты уровней детализации и frustum culling 
*/
void Quadtree::drawObjects(
    mat4& PV, 
    vec3& cameraPosition
)
{
    RenderManager& renderManager = RenderManager::instance();
    for (int i = 0; i < graphicObjects.size(); i++) // Все объекты
    {
        if (   leveOfDetailTest(graphicObjects[i], cameraPosition) // Объект прошел тесты
             & frustumCullingTest(graphicObjects[i], PV)
           ) 
        {
            renderManager.addToRenderQueue(&graphicObjects[i]);
        }
    }
}

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
bool Quadtree::leveOfDetailTest(
    GraphicObject& graphicObject,
    vec3& cameraPosition
)
{
    GraphicObjectType type = graphicObject.getType();
    float distance = glm::distance(graphicObject.getPosition(), cameraPosition);
    return (    type == GraphicObjectType::vehicle && distance < 500       // Уровни идеализации
             || type == GraphicObjectType::big_nature && distance < 350
             || type == GraphicObjectType::small_nature && distance < 200
             || type == GraphicObjectType::big_prop && distance < 400
             || type == GraphicObjectType::medium_prop && distance < 300
             || type == GraphicObjectType::small_prop && distance < 200
             || type == GraphicObjectType::road
             || type == GraphicObjectType::building
           );
}

/*
    Тест frustum culling для графического объекта
    
    @params:
        graphicObject - графический объект
        PV - матриц проекции вида
    
    @return:
        true - в поле зрение камеры
        false - в противном случае
*/
bool Quadtree::frustumCullingTest(
    GraphicObject& graphicObject, 
    mat4& PV
)
{
    mat4 PVM = PV * graphicObject.getModelMatrix();
    vec3 dimensions = graphicObject.getDimensions();
    vector<vec4> hitBox = // хитбокс объекта
    {
        PVM * vec4(+dimensions.x / 2, +dimensions.y / 2, +dimensions.z / 2, 1.0),
        PVM * vec4(+dimensions.x / 2, +dimensions.y / 2, -dimensions.z / 2, 1.0),
        PVM * vec4(+dimensions.x / 2, -dimensions.y / 2, +dimensions.z / 2, 1.0),
        PVM * vec4(+dimensions.x / 2, -dimensions.y / 2, -dimensions.z / 2, 1.0),
        PVM * vec4(-dimensions.x / 2, +dimensions.y / 2, +dimensions.z / 2, 1.0),
        PVM * vec4(-dimensions.x / 2, +dimensions.y / 2, -dimensions.z / 2, 1.0),
        PVM * vec4(-dimensions.x / 2, -dimensions.y / 2, +dimensions.z / 2, 1.0),
        PVM * vec4(-dimensions.x / 2, -dimensions.y / 2, -dimensions.z / 2, 1.0)
    };

    int vertexOutLeftCount = 0;
    int vertexOutRightCount = 0;
    int vertexOutFrontCount = 0;
    int vertexOutBackCount = 0;
    int vertexOutTopCount = 0;
    int vertexOutBottomCount = 0;

    for (int j = 0; j < 8; j++) // Все углы хитбокса
    {
        if (hitBox[j].x < -hitBox[j].w)
        {
            vertexOutLeftCount++;
        }
        if (hitBox[j].x > hitBox[j].w)
        {
            vertexOutRightCount++;
        }
        if (hitBox[j].z < -hitBox[j].w)
        {
            vertexOutFrontCount++;
        }
        if (hitBox[j].z > hitBox[j].w)
        {
            vertexOutBackCount++;
        }
        if (hitBox[j].y < -hitBox[j].w)
        {
            vertexOutBottomCount++;
        }
        if (hitBox[j].y > hitBox[j].w)
        {
            vertexOutTopCount++;
        }
    }

    return 
        (    vertexOutLeftCount < 8
          && vertexOutRightCount < 8
          && vertexOutTopCount < 8
          && vertexOutBottomCount < 8
          && vertexOutFrontCount < 8
          && vertexOutBackCount < 8
        );
}

/*
    Получить информацию о попадании графического объекта в узел

    @params:
        hitbox - хитбокс объекта

    @return:
        GraphicObjectConsists::FULL - объект полностью попадает в узел
        GraphicObjectConsists::PART - объект частично попадает в узел
        GraphicObjectConsists::NONE - объект полностью не попадает в узел
*/
GraphicObjectConsists Quadtree::getGraphicObjectConsists(const vector<vec4>& hitbox)
{
    int vertexConsistsCount = 0; // Количество углов хитбокса попадающие в узел
    for (int i = 0; i < 8; i++) // Все углы хитбокса
    {
        if (    hitbox[i].x > xMin  // Угол хитбокса попадает в узел
             && hitbox[i].x <= xMax
             && hitbox[i].z > zMin
             && hitbox[i].z <= zMax
           )
        {
            vertexConsistsCount++;
        }
    }
    if (vertexConsistsCount == 8)
    {
        return GraphicObjectConsists::FULL;
    }
    else if (vertexConsistsCount == 0)
    {
        return GraphicObjectConsists::NONE;
    }
    else
    {
        return GraphicObjectConsists::PART;
    }
}
