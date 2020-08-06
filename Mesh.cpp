#include "Mesh.h"

// Конструктор
Mesh::Mesh(string filename)
{
    load(filename);
}

// Вывести на экран
void Mesh::draw()
{
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void Mesh::drawInst(int count)
{
    glBindVertexArray(VAO);
    glDrawElementsInstanced(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0, count);
    glBindVertexArray(0);
}

// Загрузить из файла
void Mesh::load(string filename)
/*
    Параметры:
        filename - путь к файлу
*/
{
    ifstream file(filename.c_str());
    string param, tmp;
    vector<vec3> v;  // Геометрические координаты
    vector<vec3> vn; // Координаты нормалей
    vector<vec3> vt; // Текстурные коорднаты
    if (file.is_open()) // Файл открыт
    {
		// Загрузить параметры вершины меша
        while (!file.eof()) // До конца файла
        {
            // Геометрические координаты
			file >> param;
            if (param == "v")
            {
                vec3 newV;
                file >> newV.x;
                file >> newV.y;
                file >> newV.z;
                v.push_back(newV);
            }

            // Координаты нормалей
            if (param == "vn")
            {
                vec3 newV;
                file >> newV.x;
                file >> newV.y;
                file >> newV.z;
                vn.push_back(newV);
            }

            // Текстурные координаты
            if (param == "vt")
            {
                vec3 newV;
                file >> newV.x;
                file >> newV.y;
                //file >> newV.z;
                vt.push_back(newV);
            }

            // Индексы параметров трех вершин
            if (param == "f")
            {
                file >> param;
                addNewVertex(param, v, vn, vt);
                file >> param;
                addNewVertex(param, v, vn, vt);
                file >> param;
                addNewVertex(param, v, vn, vt);
            }
            getline(file, tmp);
        }
    }
    file.close();

    // Создать VAO-буфер
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

	// Создать буферы
	glGenBuffers(2, buffers);
	glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), &vertices[0].coord, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, coord));

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, normal));

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, texCoord));

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * indices.size(), &indices[0], GL_STATIC_DRAW);
    glBindVertexArray(0);
}

// Добавить индекс вершины
void Mesh::addNewVertex
    (
        string param, 
        vector<vec3>& v, 
        vector<vec3>& vn, 
        vector<vec3>& vt
    )
{
    auto it = vertexToIndex.find(param);
    if (it != vertexToIndex.end()) // Вершина уже встречалась
    {
        indices.push_back(vertexToIndex[param]);
    }
    else
    {
        vertices.push_back(createVertex(param, v, vn, vt));
        vertexToIndex.insert(make_pair(param, vertexToIndex.size()));
        indices.push_back(vertexToIndex.size() - 1);
    }
}

// Создать вершину
Vertex Mesh::createVertex
    (
        string param, 
        vector<vec3>& v, 
        vector<vec3>& vn, 
        vector<vec3>& vt
    )
{
    string index;    // Индекс в строковом формате
    int f;           // Индекс
    int i = 0;       // Позиция курсора в строке
    Vertex newVertex;// Новая вершина
    param.push_back('/');

    // Геометрические координаты вершины
    index = "";
    while (param[i] != '/')
    {
        index += param[i];
        i++;
    }
    i++;
    f = stoi(index);
    newVertex.coord[0] = v[f - 1].x;
    newVertex.coord[1] = v[f - 1].y;
    newVertex.coord[2] = v[f - 1].z;

    // Текстурные координаты
    index = "";
    while (param[i] != '/')
    {
        index += param[i];
        i++;
    }
    i++;
    f = stoi(index);
    newVertex.texCoord[0] = vt[f - 1].x;
    newVertex.texCoord[1] = vt[f - 1].y;
    //newVertex.texCoord[2] = vt[f - 1].z;

    // Координаты нормалей
    index = "";
    while (param[i] != '/')
    {
        index += param[i];
        i++;
    }
    f = stoi(index);
    newVertex.normal[0] = vn[f - 1].x;
    newVertex.normal[1] = vn[f - 1].y;
    newVertex.normal[2] = vn[f - 1].z;
    return newVertex;
}