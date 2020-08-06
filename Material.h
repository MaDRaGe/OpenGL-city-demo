#pragma once
#include "glm.hpp"

using glm::vec4;

class Material
{
public:
    // �����������
    Material();

    // ���������� �������� ���������
    void setAmbient(vec4& _ambient);
    void setDiffuse(vec4& _diffuse);
    void setSpecular(vec4& _specular);
    void setShininess(float _shininess);

    vec4& getAmbient();
    vec4& getDiffuse();
    vec4& getSpecular();
    float getShininess();

    void setTextureId(int _textureId);
    int getTextureId();

    friend bool operator==(const Material& left, const Material& right);
    friend bool operator<(const Material& left, const Material& right);
protected:

    // ���������
    float textureId;
    vec4 diffuse; 
    vec4 specular;
    vec4 ambient;
    float shininess;
};