#pragma once

#include "glew.h"
#include <gl/GL.h>
#include <gl/GLU.h>
#include "freeglut.h"

class FBO
{
public:
    /*
        �����������
    */
    FBO();

    /*
        �������������

        @params:
            w - ������ ��������
            h - ������ ��������
            multiSamples - 
    */
    void init(int w, int h, bool multiSamples = false);

    /*
        ��������� ��������
    */
    void bind();

    /*
        �������� ��������
    */
    static void unbind();

    /*
        ����������� � ������ �����
    */
    void resolveToFbo(FBO& fbo);

    /*
        ��������� �������� �����
    */
    void bindColorTexture(GLenum texUnit = GL_TEXTURE0);

    /*
        ��������� �������� �������
    */
    void bindDepthTexture(GLenum texUnit = GL_TEXTURE1);

    bool reshape(int& _width, int& _height);

private:
    // ������ ������
    GLuint FBO_Index;

    // ������ �������� �����
    GLuint colorTex;

    // ������ �������� �������
    GLuint depthTex;
    
    // ������
    int width;

    // ������
    int height;

    // ���������� �������
    int samples;
};

