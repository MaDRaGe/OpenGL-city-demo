#include "simulation.h"
#include <chrono>
#include <future>

double simulationTime;
LARGE_INTEGER oldValue, newValue, counter;

using namespace std;

// ����������� ������ ��� ���������
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
    
    // ������������ ����
    glutPostRedisplay();
}

// ����� ������ ������ �����
double getSimulationTime()
/*
    return:
        ����� ������ ����� ���������
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

// ���������� ��������� ����������
void shaderProgramManage()
{
    if (shaderProgram.isUpdated()) // ��������� ��������� ���������
    {
        shaderProgram.load();
    }
}
