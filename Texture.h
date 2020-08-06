#pragma once

#include <stdio.h>
#include <windows.h>
#include <iostream>
#include <string>

#include "glew.h"

#include <gl/GL.h>
#include <gl/GLU.h>
#include "freeglut.h"

#include "IL/il.h"
#include "IL/ilu.h"
#include "IL/ilut.h"

using std::string;

class Texture
{
public:
    Texture();
    void load(string filename);
    void apply(GLenum texUnit = GL_TEXTURE0);
    static void disableAll();


private:
    GLuint texIndex;
};
