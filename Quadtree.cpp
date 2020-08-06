#include "Quadtree.h"

void Quadtree::startDraw()
{
}

void Quadtree::finishDraw()
{
}

/*
    �����������

    @params:
        _xMin - ����������� ������� �� ��� X
        _xMax - ������������ ������� �� ��� X
        _zMin - ����������� ������� �� ��� Z
        _zMax - ������������ ������� �� ��� Z

    @features:
        ���� ���� �� ���������� ���������,
        �� ��������� �������� ���� �������� �������
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
    
    if (    width > 16  // ���� �������
         && height > 16
       )
    {
        // ����� �������� ���� �������� �������
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
    ������� ���������� �� ���� �� �������

    @requers:
        width - ������ ����
        height - ������ ����
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
    �������� ����������� ������
    
    @params:
        graphicObject - ����������� ������
    
    @features:
        - ��������� ����������� ������ � ������� ����, ���� ���� �������� ������
        ��� ���� ������ ����� ����� � ���������� �������� �����
        - ���� ������ ��������� ����� � ����� �� �������� �����, �� ���������
        ������ � ��������������� �������� ����
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

        // ����� �����, ����� ���� �� �������� ����� �������� ������
        bool isChildrenConsistHitbox = false;

        GraphicObjectConsists consist;
        for (int i = 0; i < 4; i++) // ��� �������� ����
        {
            consist = childrenQuadtrees[i]->getGraphicObjectConsists(hitbox);
            if (consist == GraphicObjectConsists::FULL) // ������ �������� ������ � �������� ����
            {
                childrenQuadtrees[i]->addGraphicObject(graphicObject);
                return;
            }
            else if (consist == GraphicObjectConsists::PART) // ������ �������� ������ � �������� ����
            {
                if (isChildrenConsistHitbox) // ������ ��� ������� � �����-�� �������� ����
                {
                    graphicObjects.push_back(graphicObject);
                    return;
                }
                else
                {
                    // ����������� ������ �������� �������� � ���� �� �������� ����
                    isChildrenConsistHitbox = true;
                }
            }
        }
    }
}

/*
    ���� ����
    
    @return:
        true - ���� �������� ������
        false - � ��������� ������
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
    ���� �����
    
    @params:
        PV - ������� ��������-����
    
    @return:
        true - ���� ��������� ��� ��������
        false - ���� ��������� �� �����
    
    @features:
        ���� �����, ���� ��� ���� �� ������� ����� ��� ����
*/
bool Quadtree::isVisible(mat4& PV)
{
    vector<vec4> hitbox = // ���� ����
    {
        PV * vec4(xMin, 1, zMin, 1.0),
        PV * vec4(xMax, 1, zMin, 1.0),
        PV * vec4(xMin, 1, zMax, 1.0),
        PV * vec4(xMax, 1, zMax, 1.0)
    };

    // ��������� ����� ����
    int countVertexOutLeft = 0;  // ���������� ����� ������� �� ����� �����
    int countVertexOutRight = 0; // ���������� ����� ������� �� ������ �����
    int countVertexOutFar = 0;   // ���������� ����� ������� �� ������� �����
    int countVertexOutNear = 0;  // ���������� ����� ������� �� �������
    for (int i = 0; i < 4; i++) // ��� ����
    {
        if (hitbox[i].x < -hitbox[i].w) // ���� �� ����� ����� 
        {
            countVertexOutLeft++;
        }
        if (hitbox[i].x > hitbox[i].w) // ���� �� ������ �����
        {
            countVertexOutRight++;
        }
        if (hitbox[i].z < -hitbox[i].w) // ���� �� ������� �����
        {
            countVertexOutFar++;
        }
        if (hitbox[i].z > hitbox[i].w) // ���� �� ������� �����
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
    ������� �������
    
    @params:
        PV - ������� �������� ����
        cameraPosition - ��������� ������
    
    @features:
        ������� ������� �������� ���� � ������� �������� ����� (���� ��� ����)
*/
void Quadtree::draw(
    mat4& PV, 
    vec3& cameraPosition
)
{
    if (isVisible(PV)) // ���� �����
    {
        // ������� �������
        drawObjects(PV, cameraPosition);
    
        // ������� ������� �������� �����
        if (!isLeaf()) // ���� �� ����
        {
            for (int i = 0; i < 4; i++)
            {
                childrenQuadtrees[i]->draw(PV, cameraPosition);
            }
        }
    }
}

/*
    ������� ���������� �������
    
    @params:
        PV - ������� ��������-����
        cameraPosition - ��������� ������

    @features:
        - ������� ����������� ������� �������������� ������� ����� ������� ����������� � frustum culling 
*/
void Quadtree::drawObjects(
    mat4& PV, 
    vec3& cameraPosition
)
{
    RenderManager& renderManager = RenderManager::instance();
    for (int i = 0; i < graphicObjects.size(); i++) // ��� �������
    {
        if (   leveOfDetailTest(graphicObjects[i], cameraPosition) // ������ ������ �����
             & frustumCullingTest(graphicObjects[i], PV)
           ) 
        {
            renderManager.addToRenderQueue(&graphicObjects[i]);
        }
    }
}

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
bool Quadtree::leveOfDetailTest(
    GraphicObject& graphicObject,
    vec3& cameraPosition
)
{
    GraphicObjectType type = graphicObject.getType();
    float distance = glm::distance(graphicObject.getPosition(), cameraPosition);
    return (    type == GraphicObjectType::vehicle && distance < 500       // ������ �����������
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
    ���� frustum culling ��� ������������ �������
    
    @params:
        graphicObject - ����������� ������
        PV - ������ �������� ����
    
    @return:
        true - � ���� ������ ������
        false - � ��������� ������
*/
bool Quadtree::frustumCullingTest(
    GraphicObject& graphicObject, 
    mat4& PV
)
{
    mat4 PVM = PV * graphicObject.getModelMatrix();
    vec3 dimensions = graphicObject.getDimensions();
    vector<vec4> hitBox = // ������� �������
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

    for (int j = 0; j < 8; j++) // ��� ���� ��������
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
    �������� ���������� � ��������� ������������ ������� � ����

    @params:
        hitbox - ������� �������

    @return:
        GraphicObjectConsists::FULL - ������ ��������� �������� � ����
        GraphicObjectConsists::PART - ������ �������� �������� � ����
        GraphicObjectConsists::NONE - ������ ��������� �� �������� � ����
*/
GraphicObjectConsists Quadtree::getGraphicObjectConsists(const vector<vec4>& hitbox)
{
    int vertexConsistsCount = 0; // ���������� ����� �������� ���������� � ����
    for (int i = 0; i < 8; i++) // ��� ���� ��������
    {
        if (    hitbox[i].x > xMin  // ���� �������� �������� � ����
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
