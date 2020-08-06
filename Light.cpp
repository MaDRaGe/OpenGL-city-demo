#include "Light.h"

// Конструкторы
Light::Light()
{
    direction = vec4(0.3, 1.0, 0.5, 0);
    ambient = vec4(1.0, 1.0, 1.0, 1);
    diffuse = vec4(1, 1, 1, 1);
    specular = vec4(0.4, 0.4, 0.4, 1);
}

// Установка параметров освещения
void Light::setDirection(vec4& _direction)
{
    direction = _direction;
}

void Light::setAmbient(vec4& color)
{ 
    ambient = color; 
}

void Light::setDiffuse(vec4& color) 
{ 
    diffuse = color; 
}

void Light::setSpecular(vec4& color) 
{ 
    specular = color; 
}

vec4& Light::getDirection()
{
    return direction;
}

vec4& Light::getAmbient()
{
    return ambient;
}

vec4& Light::getDiffuse()
{
    return diffuse;
}

vec4& Light::getSpecular()
{
    return specular;
}
