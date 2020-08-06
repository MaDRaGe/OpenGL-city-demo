#include "Data.h"

Shader vertexShader("data/SHADER/InstancedDiffuseTexture.vsh");
Shader fragmentShader("data/SHADER/InstancedDiffuseTexture.fsh");
ShaderProgram shaderProgram(vertexShader, fragmentShader);

Shader vertexShaderGrey("data/SHADER/GreyPostProcessing.vsh");
Shader fragmentShaderGrey("data/SHADER/GreyPostProcessing.fsh");
ShaderProgram shaderProgramGrey(vertexShaderGrey, fragmentShaderGrey);

Shader vertexShaderSepia("data/SHADER/SepiaPostProcessing.vsh");
Shader fragmentShaderSepia("data/SHADER/SepiaPostProcessing.fsh");
ShaderProgram shaderProgramSepia(vertexShaderSepia, fragmentShaderSepia);

Shader vertexShaderSimple("data/SHADER/SimplePostProcessing.vsh");
Shader fragmentShaderSimple("data/SHADER/SimplePostProcessing.fsh");
ShaderProgram shaderProgramSimple(vertexShaderSimple, fragmentShaderSimple);

// Шейдер - вертикальный эффект Гаусса
Shader vShaderVertGauss("data/SHADER/VertGauss.vsh");
Shader fShaderVertGauss("data/SHADER/VertGauss.fsh");
ShaderProgram shaderProgramVertGauss(vShaderVertGauss, fShaderVertGauss);

// Шейдер - горизонтальный эффект Гаусса
Shader vShaderHorizGauss("data/SHADER/HorizGauss.vsh");
Shader fShaderHorizGauss("data/SHADER/HorizGauss.fsh");
ShaderProgram shaderProgramHorizGauss(vShaderHorizGauss, fShaderHorizGauss);

// Шейдер - эффект DOF
Shader vShaderDOF("data/SHADER/DOF.vsh");
Shader fShaderDOF("data/SHADER/DOF.fsh");
ShaderProgram shaderProgramDOF(vShaderDOF, fShaderDOF);

Light light;
Scene scene;
int windowWidth; 
int windowHeight;
void dataInit()
{
    int windowWidth = 700;
    int windowHeight = 700;
    shaderProgram.load();
    shaderProgram.getUniformID("projectionMatrix");
    shaderProgram.getUniformID("lAmbient");
    shaderProgram.getUniformID("lDiffuse");
    shaderProgram.getUniformID("lSpecular");
    shaderProgram.getUniformID("lPosition");
    shaderProgram.getUniformID("modelViewMatrix");
    shaderProgram.getUniformID("mAmbient");
    shaderProgram.getUniformID("mDiffuse");
    shaderProgram.getUniformID("mSpecular");
    shaderProgram.getUniformID("mShininess");
    shaderProgram.getUniformID("texture_0");
    shaderProgram.getUniformID("cameraPos");
    shaderProgram.getUniformID("fogColor");
    shaderProgram.getUniformID("fogNear");
    shaderProgram.getUniformID("fogFar");

    shaderProgramGrey.load();
    shaderProgramGrey.getUniformID("texture_0");

    shaderProgramSepia.load();
    shaderProgramSepia.getUniformID("texture_0");

    shaderProgramSimple.load();
    shaderProgramSimple.getUniformID("texture_0");

    shaderProgramHorizGauss.load();
    shaderProgramHorizGauss.getUniformID("texture_0");

    shaderProgramVertGauss.load();
    shaderProgramVertGauss.getUniformID("texture_0");

    shaderProgramDOF.load();
    shaderProgramDOF.getUniformID("texture_0");
    shaderProgramDOF.getUniformID("texture_1");
    shaderProgramDOF.getUniformID("texture_2");
    shaderProgramDOF.getUniformID("zNear");
    shaderProgramDOF.getUniformID("zFar");

    RenderManager& renderManager = RenderManager::instance();
    renderManager.init();
    renderManager.addShaderProgram(ShaderType::COMMON, shaderProgram);
    renderManager.addShaderProgram(ShaderType::GREY, shaderProgramGrey);
    renderManager.addShaderProgram(ShaderType::SEPIA, shaderProgramSepia);
    renderManager.addShaderProgram(ShaderType::SIMPLE, shaderProgramSimple);
    renderManager.addShaderProgram(ShaderType::HORIZ_GAUSS, shaderProgramHorizGauss);
    renderManager.addShaderProgram(ShaderType::VERT_GAUSS, shaderProgramVertGauss);
    renderManager.addShaderProgram(ShaderType::DOF, shaderProgramDOF);
    scene.init("\DATA\\models.xml");
    scene.loadFromXML("\DATA\\demo_scene_2.xml");

    /*NetProtocol& net = NetProtocol::instance();
    net.Connect("127.0.0.1", 27000);
    net.Send(0);
    NetworkHeader header;
    net.Recv(&header, sizeof(header));

    char* str = new char[header.dataLen - 1];
    net.Recv(str, header.dataLen - 1);
    printf("%s\n", str);
    delete[] str;*/
}