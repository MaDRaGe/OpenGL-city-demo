#include "Shader.h"

// ������������
Shader::Shader(string filepath)
{
    filePath = fs::current_path() / filepath;
    lastWriteTime = fs::last_write_time(filePath).time_since_epoch().count();
}
Shader::Shader()
{
    lastWriteTime = 0;
}

// ���������� ���� � �������
void Shader::setFilepath(string filepath)
/*
    @param filepath - ���� � �������
*/
{
    filePath = fs::current_path() / filepath;
}

// �������� ���� � �������
string Shader::getFilepath()
{
    return filePath.string();
}

// ������ �������� ?
bool Shader::isUpdated()
{
    long long newLastWriteTime = fs::last_write_time(filePath).time_since_epoch().count();
    if ( newLastWriteTime != this->lastWriteTime) // ���� ������� ��� �����������
    {
        this->lastWriteTime = newLastWriteTime;
        return true;
    }
    else
    {
        return false;
    }
}