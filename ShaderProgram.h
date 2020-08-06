#pragma once
#include "glm.hpp" // vec4, mat4
#include "gtc/type_ptr.hpp"
#include "glew.h" // �������� ����� GL\GL.h � freeglut.h
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
    // �����������
    ShaderProgram();
    ShaderProgram(Shader vertexShader, Shader fragmentShader);

    // ��������������
    bool load();

    // ������������ 
    void activate();

    // �������������
    static void deactivate();

    // ���������� �������� uniform-����������
    void setUniform(string name, GLfloat& value);
    void setUniform(string name, GLfloat value);
    void setUniform(string name, vec4& value);
    void setUniform(string name, vec4&& value);
    void setUniform(string name, mat4& value);
    void setUniform(string name, GLuint& value);
    void setUniform(string name, vector<mat4>& modelViewMatrixes);
    void setUniform(string name, vec3& value);

    // ��������� ID uniform-����������
    GLuint getUniformID(string name);

    // ��������� ��������� ���������� ?
    bool isUpdated();

private:
    // ������������� ��������� ���������
    GLuint shaderProgram;

    // ��� uniform ���������� � �� �������������
    std::map<string, GLuint> uniforms;

    // ��������� ���������
    Shader vertexShader, fragmentShader;
private:
    // �������� ���������� ������� � �������� �� ���������� �����
    GLuint loadShaderFromFile(GLenum shaderType, string filepath);
};