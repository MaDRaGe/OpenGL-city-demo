#include "display.h"

void display()
{
    static bool init = true;
    
    RenderManager& renderManager = RenderManager::instance();
    renderManager.start();

    scene.draw();

    renderManager.setCamera(scene.getCamera());
    renderManager.setLight(&light);

    renderManager.finish();

    glutSwapBuffers();

    char windowTitle[512];
    sprintf_s(
        windowTitle, 
        sizeof(windowTitle), 
        "[%8.2f FPS] Shader managing | %s | %s", 
        renderManager.getFPS(), 
        scene.getSceneDescription().c_str(),
        renderManager.getRenderManagerDescription().c_str()
    );
    glutSetWindowTitle(windowTitle);
}