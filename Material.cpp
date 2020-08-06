#include "Material.h"

// Конструкторы
Material::Material() 
{ 
    textureId = -1;
    diffuse = vec4(0.85, 0.85, 0.85, 1);
    ambient = vec4(0.35, 0.35, 0.35, 1);
    specular = vec4(0.25, 0.25, 0.25, 1);
    shininess = 64.0;
}

void Material::setAmbient(vec4& _ambient)
{
    ambient = _ambient;
}

void Material::setDiffuse(vec4& _diffuse)
{
    diffuse = _diffuse;
}

void Material::setSpecular(vec4& _specular)
{
    specular = _specular;
}

void Material::setShininess(float _shininess)
{
    shininess = _shininess;
}

vec4& Material::getAmbient()
{
    return ambient;
}

vec4& Material::getDiffuse()
{
    return diffuse;
}

vec4& Material::getSpecular()
{
    return specular;
}

float Material::getShininess()
{
    return shininess;
}

void Material::setTextureId(int _textureId)
{
    textureId = _textureId;
}

int Material::getTextureId()
{
    return textureId;
}

bool operator==(const Material& left, const Material& right)
{
    if (left.ambient.r == right.ambient.r
        && left.ambient.g == right.ambient.g
        && left.ambient.b == right.ambient.b
        && left.diffuse.r == right.diffuse.r
        && left.diffuse.g == right.diffuse.g
        && left.diffuse.b == right.diffuse.b
        && left.specular.r == right.specular.r
        && left.specular.g == right.specular.g
        && left.specular.b == right.specular.b
        && left.shininess == right.shininess
        && left.textureId == right.textureId
        )
    {
        return true;
    }
    return false;
}

bool operator<(const Material& left, const Material& right)
{
    if (left.ambient.r < right.ambient.r)
    {
        return true;
    }
    else if (left.ambient.r > right.ambient.r)
    {
        return false;
    }
    if (left.ambient.g < right.ambient.g)
    {
        return true;
    }
    else if (left.ambient.g > right.ambient.g)
    {
        return false;
    }
    if (left.ambient.b < right.ambient.b)
    {
        return true;
    }
    else if (left.ambient.b > right.ambient.b)
    {
        return false;
    }
    if (left.diffuse.r < right.diffuse.r)
    {
        return true;
    }
    else if (left.diffuse.r > right.diffuse.r)
    {
        return false;
    }
    if (left.diffuse.g < right.diffuse.g)
    {
        return true;
    }
    else if (left.diffuse.g > right.diffuse.g)
    {
        return false;
    }
    if (left.diffuse.b < right.diffuse.b)
    {
        return true;
    }
    else if (left.diffuse.b > right.diffuse.b)
    {
        return false;
    }
    if (left.specular.r < right.specular.r)
    {
        return true;
    }
    else if (left.specular.r > right.specular.r)
    {
        return false;
    }
    if (left.specular.g < right.specular.g)
    {
        return true;
    }
    else if (left.specular.g > right.specular.g)
    {
        return false;
    }
    if (left.specular.b < right.specular.b)
    {
        return true;
    }
    else if (left.specular.b > right.specular.b)
    {
        return false;
    }
    if (left.shininess < right.shininess)
    {
        return true;
    }
    else if (left.shininess > right.shininess)
    {
        return false;
    }
    if (left.textureId < right.textureId)
    {
        return true;
    }
    else if (left.textureId > right.textureId)
    {
        return false;
    }
    return false;

    /*
    else 
    {
        if (left.ambient.g < right.ambient.g)
        {
            return true;
        }
        else if (left.ambient.g > right.ambient.g)
        {
            return false;
        }
        else
        {
            if (left.ambient.b < right.ambient.b)
            {
                return true;
            }
            else if (left.ambient.b > right.ambient.b)
            {
                return false;
            }
            else
            {
                if (left.diffuse.r < right.diffuse.r)
                {
                    return true;
                }
                else if (left.diffuse.r > right.diffuse.r)
                {
                    return false;
                }
                else
                {
                    if (left.diffuse.g < right.diffuse.g)
                    {
                        return true;
                    }
                    else if (left.diffuse.g > right.diffuse.g)
                    {
                        return false;
                    }
                    else
                    {
                        if (left.diffuse.b < right.diffuse.b)
                        {
                            return true;
                        }
                        else if (left.diffuse.b > right.diffuse.b)
                        {
                            return false;
                        }
                        else
                        {
                            if (left.specular.r < right.specular.r)
                            {
                                return true;
                            }
                            else if (left.specular.r > right.specular.r)
                            {
                                return false;
                            }
                            else
                            {
                                if (left.specular.g < right.specular.g)
                                {
                                    return true;
                                }
                                else if (left.specular.g > right.specular.g)
                                {
                                    return false;
                                }
                                else
                                {
                                    if (left.specular.b < right.specular.b)
                                    {
                                        return true;
                                    }
                                    else if (left.specular.b > right.specular.b)
                                    {
                                        return false;
                                    }
                                    else
                                    {
                                        if (left.shininess < right.shininess)
                                        {
                                            return true;
                                        }
                                        else if (left.shininess > right.shininess)
                                        {
                                            return false;
                                        }
                                        else
                                        {
                                            if (left.textureId < right.textureId)
                                            {
                                                return true;
                                            }
                                            else if (left.textureId > right.textureId)
                                            {
                                                return false;
                                            }
                                            else
                                            {
                                                return false;
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }*/
}

