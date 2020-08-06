#include "ResourceManager.h"

ResourceManager& ResourceManager::instance()
{
    static ResourceManager resourceManager;
    return resourceManager;
}

// Загрузить меш
int ResourceManager::loadMesh(string filename)
{
    auto IT = meshes_id.find(filename);
    if (IT == meshes_id.end()) // Меш еще не был загружен
    {
        Mesh _mesh(filename);
        meshes.push_back(_mesh);
        meshes_id.insert(make_pair(filename, meshes.size() - 1));
    }
    return meshes_id[filename];
}

// Загрузить текстуру
int ResourceManager::loadTexture(string filename)
{
    auto IT = textures_id.find(filename);
    if (IT == textures_id.end()) // Текстура еще не была загружена
    {
        Texture texture;
        texture.load(filename);
        textures.push_back(texture);
        textures_id.insert(make_pair(filename, textures.size() - 1));
    }
    return textures_id[filename];
}

// Получить меш
Mesh* ResourceManager::getMesh(int index)
{
    if (index > -1 && index < meshes.size())
    {
        return &meshes[index];
    }
    return 0;
}

// Получить текстуру
Texture* ResourceManager::getTexture(int index)
{
    if (index > -1 && index < textures.size())
    {
        return &textures[index];
    }
    return 0;
}

int ResourceManager::createSharedMaterial(Material& _material)
{
    for (int i = 0; i < materials.size(); i++)
    {
        if (materials[i] == _material)
        {
            return i;
        }
    }
    materials.push_back(_material);
    return materials.size() - 1;
}

Material* ResourceManager::getMaterial(int _materialId)
{
    if (_materialId > -1 && _materialId < materials.size())
    {
        return &materials[_materialId];
    }
    return 0;
}

ResourceManager::ResourceManager()
{
}
