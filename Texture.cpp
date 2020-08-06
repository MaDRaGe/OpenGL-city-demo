#include "Texture.h"

// �����������
Texture::Texture()
{
}

// �������� �������� �� �����
void Texture::load(string filename)
/*
    ���������:
        filename - ���� � �����
*/
{
    // ������� ��������
    glActiveTexture(GL_TEXTURE0);
    glGenTextures(1, &texIndex);
    glBindTexture(GL_TEXTURE_2D, texIndex);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    // ��������� ����������� �� �����
    ILuint imageId = ilGenImage();
    ilBindImage(imageId);
    wchar_t Unicode_string[256];
    wsprintf(Unicode_string, L"%S", filename.c_str());
    bool result = ilLoadImage(Unicode_string);
    int width = ilGetInteger(IL_IMAGE_WIDTH);
    int height = ilGetInteger(IL_IMAGE_HEIGHT);
    int format = ilGetInteger(IL_IMAGE_FORMAT);
    int type = ilGetInteger(IL_IMAGE_TYPE);

    // ��������� ����������� � ��������� ��������
    GLubyte* _texture = new GLubyte[width * height * 4];
    ilCopyPixels(0, 0, 0, width, height, 1, IL_RGBA, GL_UNSIGNED_BYTE, _texture);
    ilDeleteImage(imageId);
    ilBindImage(0);   
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, _texture);
    glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
    glGenerateMipmap(GL_TEXTURE_2D);
    delete[] _texture;
    glBindTexture(GL_TEXTURE_2D, 0);
}

// ��������� ���������������
void Texture::apply(GLenum texUnit)
/*
    ���������:
        texUnit - ������������� ��������
*/
{
    glActiveTexture(texUnit);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texIndex);
}

// ���������� ���������������
void Texture::disableAll()
{
    glDisable(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_TEXTURE_2D);
}


