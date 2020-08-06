#pragma once

#include "glew.h"
#include <gl/GL.h>
#include <gl/GLU.h>
#include "freeglut.h"

class FBO
{
public:
    /*
        Конструткор
    */
    FBO();

    /*
        Инициализация

        @params:
            w - ширина текстуры
            h - высота текстуры
            multiSamples - 
    */
    void init(int w, int h, bool multiSamples = false);

    /*
        Привязать текстуру
    */
    void bind();

    /*
        Отвязать текстуру
    */
    static void unbind();

    /*
        Скопировать в другой буфер
    */
    void resolveToFbo(FBO& fbo);

    /*
        Привязать текстуру цвета
    */
    void bindColorTexture(GLenum texUnit = GL_TEXTURE0);

    /*
        Привязать текстуру глубины
    */
    void bindDepthTexture(GLenum texUnit = GL_TEXTURE1);

    bool reshape(int& _width, int& _height);

private:
    // Индекс буфера
    GLuint FBO_Index;

    // Индекс текстуры цвета
    GLuint colorTex;

    // Индекс текстуры глубины
    GLuint depthTex;
    
    // Ширина
    int width;

    // Высота
    int height;

    // Количество выборок
    int samples;
};

