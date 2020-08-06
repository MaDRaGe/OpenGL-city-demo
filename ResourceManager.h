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

    // ��������� ��� �� �����
    int loadMesh(string filename);

    // ��������� ��������
    int loadTexture(string filename);

    // �������� ���
    Mesh* getMesh(int index);

    // �������� ��������
    Texture* getTexture(int index);

    // ������� ���������
    int createSharedMaterial(Material& _material);

    // �������� �������� �� ��������������
    Material* getMaterial(int _materialId);

private:
    // �����������
    ResourceManager();

    // ����������� �����������
    ResourceManager(const ResourceManager& v) = delete;
    
    // �������� ������������
    ResourceManager& operator=(const ResourceManager& v) = delete;

private:
    // ����
    vector<Mesh> meshes;

    // ��� - ������������� ����
    map<string, int> meshes_id;

    // ��������
    vector<Texture> textures;

    // �������� - ������������� ��������
    map<string, int> textures_id;

    // ���������
    vector<Material> materials;
};

