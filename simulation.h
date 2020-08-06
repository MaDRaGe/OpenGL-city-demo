#pragma once
#include "Data.h"
#include "RenderManager.h"
#include "display.h"
#include <omp.h>

#define FPS 120
#define SPEED 5

extern int fps;

extern double simulationTime;
extern LARGE_INTEGER oldValue, newValue, counter;

extern void shaderProgramManage();
extern void simulationDataInit();
extern void simulation();
extern double getSimulationTime();