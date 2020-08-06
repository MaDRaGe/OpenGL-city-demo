#pragma once
#include <filesystem>
#include <string>

using std::string;
namespace fs = std::filesystem;

class Shader
{
public:
    // ������������
    Shader();
    Shader(string filepath);

    // ������ �������� ?
    bool isUpdated();

    // ���������� ���� � �������
    void setFilepath(string filepath);

    // �������� ���� � �������
    string getFilepath();

private:
    // ���� � �������
    fs::path filePath;

    // ��������� ����� ������
    long long lastWriteTime = 0;
};