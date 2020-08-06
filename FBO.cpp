#include "FBO.h"

FBO::FBO()
{
}

void FBO::init(int w, int h, bool multiSamples)
{
    glGenFramebuffers(1, &FBO_Index);
    glBindFramebuffer(GL_FRAMEBUFFER, FBO_Index);
    width = w;
    height = h;
    if (multiSamples) // Текстуры с мультисэмплом
    {
        // Текстура цвета
        glGenTextures(1, &colorTex);
        glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, colorTex);
        glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 8, GL_RGBA8, w, h, false);
        glFramebufferTexture2D(
            GL_FRAMEBUFFER,
            GL_COLOR_ATTACHMENT0,
            GL_TEXTURE_2D_MULTISAMPLE,
            colorTex,
            0
        );

        // Текстура глубины
        glGenTextures(1, &depthTex);
        glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, depthTex);
        glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 8, GL_DEPTH_COMPONENT24, w, h, false);
        glFramebufferTexture2D(
            GL_FRAMEBUFFER,
            GL_DEPTH_ATTACHMENT,
            GL_TEXTURE_2D_MULTISAMPLE,
            depthTex,
            0
        );
        samples = 8;
    }
    else // Текстуры без мультисэмпла
    {
        // Текстура цвета
        glGenTextures(1, &colorTex);
        glBindTexture(GL_TEXTURE_2D, colorTex);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
        glFramebufferTexture2D(
            GL_FRAMEBUFFER,
            GL_COLOR_ATTACHMENT0,
            GL_TEXTURE_2D,
            colorTex,
            0
        );
    
        // Текстура глубины
        glGenTextures(1, &depthTex);
        glBindTexture(GL_TEXTURE_2D, depthTex);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, w, h, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
        glFramebufferTexture2D(
            GL_FRAMEBUFFER,
            GL_DEPTH_ATTACHMENT,
            GL_TEXTURE_2D,
            depthTex,
            0
        );
        samples = 0;
    }
}

void FBO::bind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, FBO_Index);
}

void FBO::unbind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FBO::resolveToFbo(FBO& fbo)
{
    glBindFramebuffer(GL_READ_FRAMEBUFFER, FBO_Index);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo.FBO_Index);
    glBlitFramebuffer(
        0, 0, width, height,
        0, 0, fbo.width, fbo.height,
        GL_COLOR_BUFFER_BIT, GL_NEAREST
    );
}

void FBO::bindColorTexture(GLenum texUnit)
{
    glActiveTexture(texUnit);
    glBindTexture(GL_TEXTURE_2D, colorTex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}

void FBO::bindDepthTexture(GLenum texUnit)
{
    glActiveTexture(texUnit);
    glBindTexture(GL_TEXTURE_2D, depthTex);
}

bool FBO::reshape(int& _width, int& _height)
{
    if (width != _width || height != _height)
    {
        glDeleteTextures(1, &colorTex);
        glDeleteTextures(1, &depthTex);
        return true;
    }
    return false;
}
