#include "Shader.h"

// Конструкторы
Shader::Shader(string filepath)
{
    filePath = fs::current_path() / filepath;
    lastWriteTime = fs::last_write_time(filePath).time_since_epoch().count();
}
Shader::Shader()
{
    lastWriteTime = 0;
}

// Установить путь к шейдеру
void Shader::setFilepath(string filepath)
/*
    @param filepath - путь к шейдеру
*/
{
    filePath = fs::current_path() / filepath;
}

// Получить путь к шейдеру
string Shader::getFilepath()
{
    return filePath.string();
}

// Шейдер обновлен ?
bool Shader::isUpdated()
{
    long long newLastWriteTime = fs::last_write_time(filePath).time_since_epoch().count();
    if ( newLastWriteTime != this->lastWriteTime) // Файл шейдера был перезаписан
    {
        this->lastWriteTime = newLastWriteTime;
        return true;
    }
    else
    {
        return false;
    }
}