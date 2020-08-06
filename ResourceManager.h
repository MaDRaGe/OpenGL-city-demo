#pragma once
#include <memory>
#include <string>
#include <vector>
#include <map>
#include "Texture.h"
#include "Mesh.h"
#include "Material.h"

using std::vector;
using std::string;
using std::map;
class ResourceManager
{
public:
    static ResourceManager& instance();

    // Загрузить меш из файла
    int loadMesh(string filename);

    // Загрузить текстуру
    int loadTexture(string filename);

    // Получить меш
    Mesh* getMesh(int index);

    // Получить текстуру
    Texture* getTexture(int index);

    // Создать материала
    int createSharedMaterial(Material& _material);

    // Получить материал по идентификатору
    Material* getMaterial(int _materialId);

private:
    // Конструктор
    ResourceManager();

    // Конструктор копирования
    ResourceManager(const ResourceManager& v) = delete;
    
    // Оператор присваивания
    ResourceManager& operator=(const ResourceManager& v) = delete;

private:
    // Меши
    vector<Mesh> meshes;

    // Меш - идентификатор меша
    map<string, int> meshes_id;

    // Текстуры
    vector<Texture> textures;

    // Текстура - идентификатор текстуры
    map<string, int> textures_id;

    // Материалы
    vector<Material> materials;
};

