#pragma once
#include "glm.hpp" // vec4, mat4
#include "gtc/type_ptr.hpp"
#include "glew.h" // Включить перед GL\GL.h и freeglut.h
#include <GL\GL.h>
#include "freeglut.h" 
#include "Shader.h"

#include <iostream>
#include <fstream>
#include <map>
#include <vector>

#include <filesystem>

namespace fs = std::filesystem;
using glm::vec4;
using glm::vec3;
using glm::mat4;
using std::vector;
using std::make_pair;

class ShaderProgram
{
public:
    // Конструктор
    ShaderProgram();
    ShaderProgram(Shader vertexShader, Shader fragmentShader);

    // Скомпилировать
    bool load();

    // Активировать 
    void activate();

    // Деактивирвать
    static void deactivate();

    // Установить значение uniform-переменной
    void setUniform(string name, GLfloat& value);
    void setUniform(string name, GLfloat value);
    void setUniform(string name, vec4& value);
    void setUniform(string name, vec4&& value);
    void setUniform(string name, mat4& value);
    void setUniform(string name, GLuint& value);
    void setUniform(string name, vector<mat4>& modelViewMatrixes);
    void setUniform(string name, vec3& value);

    // Получение ID uniform-переменной
    GLuint getUniformID(string name);

    // Шейдерная программа обновленна ?
    bool isUpdated();

private:
    // Идентификатор шейдерной программы
    GLuint shaderProgram;

    // Имя uniform переменной и ее идентификатор
    std::map<string, GLuint> uniforms;

    // Шейдерная программа
    Shader vertexShader, fragmentShader;
private:
    // Создание шейдерного объекта и загрузка из указанного файла
    GLuint loadShaderFromFile(GLenum shaderType, string filepath);
};