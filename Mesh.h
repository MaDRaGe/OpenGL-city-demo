#pragma once
#include <stdio.h>
#include <windows.h>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <map>

#include "glew.h"

#include <gl/GL.h>
#include "freeglut.h"

#include "glm.hpp"
#include "gtc\matrix_transform.hpp"
#include "gtc\type_ptr.hpp"

using namespace std;
using namespace glm;

struct Vertex
{
    GLfloat coord[3];    // Текстурные координаты
    GLfloat normal[3];   // Векторы нормали
    GLfloat texCoord[2]; // Текстурные координаты нулевого текстурного блока
};

class Mesh
{
public:
    // Конструктор
    Mesh(string filename);

    // Загрузить из файла
    void load(string filename);

    // Вывести на экран
    void draw();

    // Вывести несколько
    void drawInst(int count);

private:
    // Добавить новую вершину
    void addNewVertex
        (
            string param, 
            vector<vec3>& v, 
            vector<vec3>& vn,
            vector<vec3>& vt
        );

    // Создать вершину
    Vertex createVertex
        (
            string param, 
            vector<vec3>& v, 
            vector<vec3>& vn, 
            vector<vec3>& vt
        );

private:
    vector<Vertex> vertices; // Вершины
    vector<GLuint> indices; // Индексы
    map<string,int> vertexToIndex;
	GLuint buffers[2]; // Буферы

    GLuint VAO;
};