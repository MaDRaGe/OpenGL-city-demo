#pragma once
#include <WinSock2.h>
#include "NetProtocol.h"
#include "Scene.h"
#include "ShaderProgram.h"
#include "Shader.h"
#include "RenderManager.h"
#include "Light.h"

// �������
extern ShaderProgram shaderProgram;
extern Shader vertexShader;
extern Shader fragmentShader;

extern ShaderProgram shaderProgramGrey;
extern Shader vertexShaderGrey;
extern Shader fragmentShaderGrey;

extern ShaderProgram shaderProgramSepia;
extern Shader vertexShaderSepia;
extern Shader fragmentShaderSepia;

extern ShaderProgram shaderProgramSimple;
extern Shader vertexShaderSimple;
extern Shader fragmentShaderSimple;

// ������ - ������������ ������ ������
extern Shader vShaderVertGauss;
extern Shader fShaderVertGauss;
extern ShaderProgram shaderProgramVertGauss;

// ������ - �������������� ������ ������
extern Shader vShaderHorizGauss;
extern Shader fShaderHorizGauss;
extern ShaderProgram shaderProgramHorizGauss;

// ������ - ������ DOF
extern Shader vShaderDOF;
extern Shader fShaderDOF;
extern ShaderProgram shaderProgramDOF;

extern Scene scene;
extern Light light;
extern int windowWidth;
extern int windowHeight;

extern void dataInit();