#pragma once
#include <filesystem>
#include <string>

using std::string;
namespace fs = std::filesystem;

class Shader
{
public:
    // Конструкторы
    Shader();
    Shader(string filepath);

    // Шейдер обновлен ?
    bool isUpdated();

    // Установить путь к шейдеру
    void setFilepath(string filepath);

    // Получить путь к шейдеру
    string getFilepath();

private:
    // Путь к шейдеру
    fs::path filePath;

    // Последнее время записи
    long long lastWriteTime = 0;
};