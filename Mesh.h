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
    GLfloat coord[3];    // ���������� ����������
    GLfloat normal[3];   // ������� �������
    GLfloat texCoord[2]; // ���������� ���������� �������� ����������� �����
};

class Mesh
{
public:
    // �����������
    Mesh(string filename);

    // ��������� �� �����
    void load(string filename);

    // ������� �� �����
    void draw();

    // ������� ���������
    void drawInst(int count);

private:
    // �������� ����� �������
    void addNewVertex
        (
            string param, 
            vector<vec3>& v, 
            vector<vec3>& vn,
            vector<vec3>& vt
        );

    // ������� �������
    Vertex createVertex
        (
            string param, 
            vector<vec3>& v, 
            vector<vec3>& vn, 
            vector<vec3>& vt
        );

private:
    vector<Vertex> vertices; // �������
    vector<GLuint> indices; // �������
    map<string,int> vertexToIndex;
	GLuint buffers[2]; // ������

    GLuint VAO;
};