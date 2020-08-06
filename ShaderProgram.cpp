#include "ShaderProgram.h"

ShaderProgram::ShaderProgram()
{
}

// �����������
ShaderProgram::ShaderProgram(
    Shader _vertexShader, 
    Shader _fragmentShader
)
/*
    @param _vertexShader - ��������� ������
    @param _fragmentShader - ����������� ������
*/
{
    vertexShader = _vertexShader;
    fragmentShader = _fragmentShader;
}

// �������������� ��������� ���������
bool ShaderProgram::load()
/*
    Return:
        true - ��������� ��������� ������� ���������������
        false - � ��������� ������
*/
{
    uniforms.clear();
    if (shaderProgram)
    {
        glDeleteProgram(shaderProgram);
        shaderProgram = 0;
    }
    if (!shaderProgram)
    {
        shaderProgram = glCreateProgram();
    }
    glAttachShader(
        shaderProgram, 
        loadShaderFromFile(GL_VERTEX_SHADER, vertexShader.getFilepath())
    );
    this->vertexShader.setFilepath(vertexShader.getFilepath());

    glAttachShader(
        shaderProgram, 
        loadShaderFromFile(GL_FRAGMENT_SHADER, fragmentShader.getFilepath())
    );
    this->fragmentShader.setFilepath(fragmentShader.getFilepath());

    glLinkProgram(shaderProgram);
    GLint status;
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &status);
    if (!status) // Program link went wrong
    {
        return false;
    }
    return true;
}

// ������������
void ShaderProgram::activate()
{
    glUseProgram(shaderProgram);
}

// ��������������
void ShaderProgram::deactivate()
{
    glUseProgram(0);
}

// ���������� �������� uniform-���������� (GLfloat&)
void ShaderProgram::setUniform(
    string name, 
    GLfloat& value
)
/*
    @param name - �������� ����������
    @parma value - ��������
*/
{
    glUniform1f(uniforms[name], value);
}

// ���������� �������� uniform-���������� (GLfloat)
void ShaderProgram::setUniform(
    string name,
    GLfloat value
)
/*
    @param name - ��������
    @parma value - ��������
*/
{
    glUniform1f(uniforms[name], value);
}

// ���������� �������� uniform-���������� (vec4&)
void ShaderProgram::setUniform(
    string name, 
    vec4& value
)
/*
    @param name - ��������
    @param value - ��������
*/
{
    float valueFloat[] = { value.x, value.y, value.z, value.a };
    glUniform4fv(uniforms[name], 1, valueFloat);
}

// ���������� �������� uniform-���������� (vec4)
void ShaderProgram::setUniform(
    string name,
    vec4&& value
)
/*
    @param name - ��������
    @param value - ��������
*/
{
    float valueFloat[] = { value.x, value.y, value.z, value.a };
    glUniform4fv(uniforms[name], 1, valueFloat);
}

// ���������� �������� uniform-���������� (mat4&)
void ShaderProgram::setUniform(
    string name, 
    mat4& value
)
/*
    @param name - ��������
    @param value - ��������
*/
{
    glUniformMatrix4fv(uniforms[name], 1, GL_FALSE, glm::value_ptr(value));
}

void ShaderProgram::setUniform(string name, GLuint& value)
{
    glUniform1i(uniforms[name], value);
}

void ShaderProgram::setUniform(string name, vector<glm::mat4>& modelViewMatrixes)
{
    const GLfloat* matrices[5];
    for (int i = 0; i < modelViewMatrixes.size(); i++)
    {
        matrices[i] = glm::value_ptr(modelViewMatrixes[i]);
    }
    glUniformMatrix4fv(uniforms[name], modelViewMatrixes.size(), GL_FALSE, *matrices);
}

void ShaderProgram::setUniform(string name, vec3& value)
{
    float valueFloat[] = { value.x, value.y, value.z };
    glUniform3fv(uniforms[name], 1, valueFloat);
}

// ��������� ��������� ���� ����������
bool ShaderProgram::isUpdated()
{
    return (vertexShader.isUpdated()
        || fragmentShader.isUpdated());
}

// ��������� ������ �� �����
GLuint ShaderProgram::loadShaderFromFile(
    GLenum shaderType, 
    string filepath
)
/*
    @param shaderType - ��� ������� (��������� ��� �����������)
    @param filepath - ���� � �������

    Return:
        ID ���������� �������
*/
{
    // ��������� ������ �� �����
    std::fstream file(filepath);
    if (!file.is_open())
    {
        return GLuint();
    }
    char* shaderCode = new char[10000];
    file.get(shaderCode, 10000, 0);

    // ������� ��������� ������ � ����������� ��������
    GLuint shaderID = glCreateShader(shaderType);
    GLchar* const shaderTmp = { shaderCode };
    glShaderSource(shaderID, 1, &shaderTmp, NULL);
    glCompileShader(shaderID);
    GLchar shaderInfoLog[100];
    shaderInfoLog[0] = '0';
    glGetShaderInfoLog(shaderID, 100, 0, shaderInfoLog);
    if (shaderInfoLog[0] != '0')
    {
        std::cout << shaderInfoLog << std::endl;
    }
    delete[] shaderCode;
    return shaderID;
}

// �������� ID uniform-����������
GLuint ShaderProgram::getUniformID(string name)
/*
    @param name - �������� ����������

    Return:
        ID uniform-����������
*/
{
    if (uniforms.find(name) == uniforms.end()) // ����� uniform-����������
    {
        uniforms.insert(make_pair(name, glGetUniformLocation(shaderProgram, name.c_str())));
    }
    return uniforms[name];
}