#pragma once
#include <omp.h>
#include <WinSock2.h>
#include <Windows.h>
#include "Data.h"
#include "glew.h"
#include <gl/GL.h>
#include "freeglut.h"

#include "simulation.h"
#include "display.h"

#include "IL/il.h"
#include "IL/ilu.h"
#include "IL/ilut.h"


using std::cout;
using std::endl;

void OpenGLInit(int, char**);
void OpenGLInitWindow();
void reshape(int, int);
void mouseWheel(int, int, int, int);
void keyboardFunc(unsigned char, int, int);

int main(int argc, char **argv)
{
    // Сеть
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
    {
        printf("Failed. Error Code: %d\n", WSAGetLastError());
    }
    else
    {
        printf("OK\n");
    }

    // Подготовка OpenGL к работе
    OpenGLInit(argc, argv);
    OpenGLInitWindow();

    // Инициализация glew
    GLenum err = glewInit();
    if (GLEW_OK != err)
    {
        cout << glewGetErrorString(err) << endl;
        return -1;
    }
    else
    {
        cout << "GLEW library has been init | Version = " 
             << glGetString(GL_VERSION) 
             << endl;
    }  

    ilInit();
    iluInit();
    ilutInit();
    dataInit();
    simulationDataInit();
    glutSetWindow(1);
    glutReshapeFunc(reshape);
    glutDisplayFunc(display);
    glutMouseWheelFunc(mouseWheel);
    glutKeyboardFunc(keyboardFunc);
    glutIdleFunc(simulation);


    #pragma omp parallel num_threads(2)
    {
        if (omp_get_thread_num() == 0)
        {
            glutMainLoop();
        }
        else
        {
            while (true)
            {
                //scene.loadFromServer();
            }
        }
    }

    WSACleanup();
    return 0;
}

// Подготовка OpenGL к работе
void OpenGLInit(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH | GLUT_STENCIL | GLUT_MULTISAMPLE);
    glutInitContextVersion(3, 3);
    glutInitContextProfile(GLUT_CORE_PROFILE);
}

// Создание окна OpenGL
void OpenGLInitWindow()
{
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(700, 700);
    glutCreateWindow("Shader Handling");
}

void reshape(int width, int height)
{
    if (omp_get_thread_num() == 0)
    {
        glViewport(0, 0, width, height);
        scene.getCamera()->setProjectionMatrix(35.0f, (float)width / height, 1.0f, 800.0f);
        if (width != windowWidth || height != windowHeight)
        {
            RenderManager::instance().reshape(width, height);
            windowWidth = width;
            windowHeight = height;
        }
    }
}

// Кнопка нажата
void keyboardFunc(unsigned char key, int x, int y)
{
    if (omp_get_thread_num() == 0)
    {
        if (key == '1')
        {
            RenderManager::instance().keyboardFunc(key);
        }
    }
}

void mouseWheel(int wheel, int direction, int x, int y)
{
    if (omp_get_thread_num() == 0)
    {
        scene.getCamera()->zoom(direction);
    }
}