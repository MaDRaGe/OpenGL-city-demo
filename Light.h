#pragma once

#include "glm.hpp"

using glm::vec4;

class Light
{
public:

    // ������������
    Light();

    // ��������� ����������
    void setDirection(vec4& _direction);
    void setAmbient(vec4& color);
    void setDiffuse(vec4& color);
    void setSpecular(vec4& color);

    vec4& getDirection();
    vec4& getAmbient();
    vec4& getDiffuse();
    vec4& getSpecular();
private:

    // ���������
    vec4 direction;
    vec4 diffuse;
    vec4 specular;
    vec4 ambient;
};