#include "simulation.h"
#include <chrono>
#include <future>

double simulationTime;
LARGE_INTEGER oldValue, newValue, counter;

using namespace std;

// ќпределение данных дл€ симул€ции
void simulationDataInit()
{
    QueryPerformanceCounter(&oldValue);
    QueryPerformanceFrequency(&counter);
}

void simulation()
{
    simulationTime = getSimulationTime();
    scene.simulate(simulationTime);
    shaderProgramManage();
    
    // ѕерерисовать окно
    glutPostRedisplay();
}

// ¬рем€ вывода одного кадра
double getSimulationTime()
/*
    return:
        врем€ одного цикла программы
*/
{
    QueryPerformanceCounter(&newValue);
    double time = (double)(newValue.QuadPart - oldValue.QuadPart) / (double)counter.QuadPart;
    while (time < (1 / FPS))
    {
        QueryPerformanceCounter(&newValue);
        time = (double)(newValue.QuadPart - oldValue.QuadPart) / (double)counter.QuadPart;
    }
    oldValue = newValue;
    return time;
}

// ”правление шейдерной программой
void shaderProgramManage()
{
    if (shaderProgram.isUpdated()) // Ўейдерна€ программа обновлена
    {
        shaderProgram.load();
    }
}
