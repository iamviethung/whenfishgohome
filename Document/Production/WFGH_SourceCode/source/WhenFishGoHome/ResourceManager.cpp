#include "stdafx.h"
#include "ResourceManager.h"
#include <fstream>
#include <stdlib.h>
#include "TGA.h"

ResourceManager* ResourceManager::m_Intance = NULL;

ResourceManager::ResourceManager()
{
}

void ResourceManager::ReadMenu(ifstream* file, string m_filePathPrefix)
{
	std::string str;
	char* menuFile = new char[256];

	std::getline(*file, str);
	std::getline(*file, str);
	sscanf(str.c_str(), "FILE %s", menuFile);

	m_menu = new string(m_filePathPrefix + menuFile);

	delete[] menuFile;
}

void ResourceManager::ReadScenes(ifstream* file, string m_filePathPrefix)
{
	std::string str;
	int sceneCount = 0;
	char* sceneFile = new char[256];

	std::getline(*file, str);
	sscanf(str.c_str(), "#Scenes: %d", &sceneCount);

	for (int i = 0; i < sceneCount; i++)
	{
		std::getline(*file, str);
		std::getline(*file, str);
		sscanf(str.c_str(), "FILE %s", sceneFile);

		m_scenes.push_back(new string(m_filePathPrefix + sceneFile));
	}

	delete[] sceneFile;
}

bool ResourceManager::ReadModels(ifstream *file, string m_filePathPrefix)
{
	std::string str;
	int posCount = 0;
	Vertex* vertex;
	GLuint numVertex;
	GLuint* indices;
	char* objFile = new char[256];

	std::getline(*file, str);
	sscanf(str.c_str(), "#Models: %d", &posCount);

	for (int i = 0; i < posCount; i++)
	{
		// TODO: READ ID
		std::getline(*file, str);
		// DO SOMETHING
		std::getline(*file, str);
		sscanf(str.c_str(), "FILE %s", objFile);

		ObjectInfo temp;

		if (str.find("obj") != -1)
			loadOBJModel((m_filePathPrefix + objFile).c_str(), vertex, numVertex, indices, temp.numIndices);
		else
			loadNFGModel((m_filePathPrefix + objFile).c_str(), vertex, numVertex, indices, temp.numIndices);

		if (vertex == NULL)
			return false;

		if (indices == NULL)
			return false;

		std::getline(*file, str);
		sscanf(str.c_str(), "MINCOLLIDER %f %f %f", &temp.min.x, &temp.min.y, &temp.min.z);
		std::getline(*file, str);
		sscanf(str.c_str(), "MAXCOLLIDER %f %f %f", &temp.max.x, &temp.max.y, &temp.max.z);

		glGenBuffers(1, &temp.vboID);
		glBindBuffer(GL_ARRAY_BUFFER, temp.vboID);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * numVertex, vertex, GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glGenBuffers(1, &temp.iboID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, temp.iboID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * temp.numIndices, indices, GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		m_objPos.push_back(temp);

		delete[] vertex;
		delete[] indices;
		vertex = NULL;
		indices = NULL;
	}

	delete[] objFile;

	return true;
}

bool ResourceManager::ReadTextures(ifstream *file, string m_filePathPrefix)
{
	std::string str;
	int textureCount = 0;
	char* texFile = new char[256];

	std::getline(*file, str);
	sscanf(str.c_str(), "#2D Textures: %d", &textureCount);

	for (int i = 0; i < textureCount; i++)
	{
		// TODO: READ ID
		std::getline(*file, str);
		// DO SOMETHING
		std::getline(*file, str);
		sscanf(str.c_str(), "FILE %s", texFile);
		int tex = Init2DTexture((m_filePathPrefix + texFile).c_str());
		if (tex == -1)
		{
			delete[] texFile;
			return false;
		}
		m_texID.push_back(tex);
	}

	delete[] texFile;
}

bool ResourceManager::ReadShaders(ifstream *file, string m_filePathPrefix)
{
	std::string str;
	int shaderCount = 0;
	char* fileVS = new char[256];
	char* fileFS = new char[256];

	std::getline(*file, str);
	sscanf(str.c_str(), "#Shader: %d", &shaderCount);

	for (int i = 0; i < shaderCount; i++)
	{
		// TODO: READ ID
		std::getline(*file, str);
		// DO SOMETHING
		std::getline(*file, str);
		sscanf(str.c_str(), "VS %s", fileVS);
		std::getline(*file, str);
		sscanf(str.c_str(), "FS %s", fileFS);

		m_shader.push_back(new Program((char*)(m_filePathPrefix + fileVS).c_str(), (char*)(m_filePathPrefix + fileFS).c_str()));
		if (m_shader.back()->loadShaders() != 0)
		{
			delete[] fileVS;
			delete[] fileFS;
			return false;
		}
	}

	delete[] fileVS;
	delete[] fileFS;
	return true;
}

bool ResourceManager::ReadMap(ifstream *file, string m_filePathPrefix)
{
	std::string str;
	int mapCount = 0;
	char* mapFile = new char[256];

	std::getline(*file, str);
	sscanf(str.c_str(), "#Map: %d", &mapCount);

	for (int i = 0; i < mapCount; i++)
	{
		// TODO: READ ID
		std::getline(*file, str);
		// DO SOMETHING
		std::getline(*file, str);
		sscanf(str.c_str(), "FILE %s", mapFile);

		m_map.push_back(new Map());
		if (!m_map.back()->ReadMap((m_filePathPrefix + mapFile).c_str()))
		{
			delete[] mapFile;
			return false;
		}
	}

	delete[] mapFile;
	return true;
}

void ResourceManager::ReadSaveFile(ifstream *file, string m_filePathPrefix)
{
	std::string str;
	char* saveFile = new char[256];

	std::getline(*file, str);
	std::getline(*file, str);
	sscanf(str.c_str(), "FILE %s", saveFile);

	m_save = new string(m_filePathPrefix + saveFile);

	delete[] saveFile;
}

//Read file Resourcemanager
bool ResourceManager::readSourceFile(string m_filePathPrefix)
{
	std::ifstream myFile((m_filePathPrefix + "/Resources/Manager/RM.txt").c_str());

	bool success = true;

	success &= myFile.good();
	ReadMenu(&myFile, m_filePathPrefix);
	ReadScenes(&myFile, m_filePathPrefix);
	success &= ReadModels(&myFile, m_filePathPrefix);
	success &= ReadTextures(&myFile, m_filePathPrefix);
	success &= ReadShaders(&myFile, m_filePathPrefix);
	success &= ReadMap(&myFile, m_filePathPrefix);
	ReadSaveFile(&myFile, m_filePathPrefix);

	myFile.close();
	return success;
}

ResourceManager* ResourceManager::GetInstance()
{
	if (m_Intance == NULL) {
		m_Intance = new ResourceManager();
	}

	return m_Intance;
}

ResourceManager::~ResourceManager()
{
	if (m_Intance != NULL)
	{
		for (int i = 0; i < Map::m_size * Map::m_size; i++)
		{
			if (Map::m_obstructors[i])
				delete Map::m_obstructors[i];
			Map::m_obstructors[i] = NULL;
		}
		delete[] Map::m_obstructors;
		Map::m_obstructors = NULL;

		for (vector<string*>::iterator i = m_scenes.begin(); i != m_scenes.end();)
		{
			string* temp = *i;
			i++;
			delete temp;
		}

		for (vector<Program*>::iterator i = m_shader.begin(); i != m_shader.end();)
		{
			Program* temp = *i;
			i++;
			delete temp;
		}

		for (vector<Map*>::iterator i = m_map.begin(); i != m_map.end();)
		{
			Map* temp = *i;
			i++;
			delete temp;
		}

		delete m_menu;
		m_menu = NULL;

		delete m_save;
		m_save = NULL;

		m_objPos.clear();
		m_texID.clear();
		m_shader.clear();
		m_map.clear();

		m_Intance = NULL;
	}
}

int ResourceManager::Init2DTexture(const char* image_path)
{
	glEnable(GL_TEXTURE_2D);
	int* img_w = new int();
	int* img_h = new int();
	int* img_bpp = new int();
	GLuint textureID;
	//load texture 0
	glActiveTexture(GL_TEXTURE0);
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	char *imageData = LoadTGA(image_path, img_w, img_h, img_bpp);
	if (*img_bpp == 32)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, *img_w, *img_h, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);
	}
	else
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, *img_w, *img_h, 0, GL_RGB, GL_UNSIGNED_BYTE, imageData);
	}

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	if (img_w != NULL) delete[]img_w;
	if (img_h != NULL) delete[]img_h;
	if (img_bpp != NULL) delete img_bpp;
	if (imageData != NULL) delete[]imageData;

	return textureID;
}

void ResourceManager::loadNFGModel(const char* fileName, Vertex* &gVerticesData, GLuint &numVertex, GLuint* &indices, GLuint &numIndice)
{
	fstream f;
	f.open(fileName, ios_base::in);

	if (!f.good())
	{
		return;
	}

	string line;
	getline(f, line);
	//lay so dinh
	sscanf(line.c_str(), "NrVertices: %d", &numVertex);
	gVerticesData = new Vertex[numVertex];

	int index = 0;
	for (unsigned int step = 0; step < numVertex; step++)
	{
		getline(f, line);

		index = atoi(line.substr(0, line.find(".")).c_str());
		line.erase(0, line.find(".") + 2);
		sscanf(line.c_str(), "pos:[%f, %f, %f]; norm:[%f, %f, %f]; binorm:[%f, %f, %f]; tgt:[%f, %f, %f]; uv:[%f, %f];",
			&gVerticesData[index].pos.x, &gVerticesData[index].pos.y, &gVerticesData[index].pos.z,
			&gVerticesData[index].normal.x, &gVerticesData[index].normal.y, &gVerticesData[index].normal.z,
			&gVerticesData[index].binormal.x, &gVerticesData[index].binormal.y, &gVerticesData[index].binormal.z,
			&gVerticesData[index].tangent.x, &gVerticesData[index].tangent.y, &gVerticesData[index].tangent.z,
			&gVerticesData[index].uv.x, &gVerticesData[index].uv.y);
	}

	//lay chi so indices
	getline(f, line);
	sscanf(line.c_str(), "NrIndices: %d", &numIndice);
	indices = new GLuint[numIndice];
	for (unsigned int step = 0; step < numIndice / 3; step++)
	{
		getline(f, line);
		sscanf(line.c_str(), "   %d.    %d,    %d,    %d", &index, &indices[step * 3], &indices[step * 3 + 1], &indices[step * 3 + 2]);
	}

	f.close();
}

void ResourceManager::loadOBJModel(const char* fileName, Vertex* &gVerticesData, GLuint &numVertex, GLuint* &indices, GLuint &numIndice)
{
	vector< GLuint > vertexIndices, textureIndices;
	vector< Vector3 > temp_vertices;
	vector< Vector2 > temp_uvs;
	GLuint a;
	ifstream myFile(fileName);
	if (!myFile.good()){
		printf("Impossible to open the file !\n");
		return;
	}
	string str;
	while (1){
		// doc 1 tu trong file
		myFile >> str;
		if (myFile.eof())
			break; // EOF = End Of File. Quit the loop.

		// else : parse lineHeader
		if (strcmp(str.c_str(), "v") == 0){
			Vector3 vertex;
			myFile >> vertex.x;
			myFile >> vertex.y;
			myFile >> vertex.z;
			temp_vertices.push_back(vertex);
		}
		else if (strcmp(str.c_str(), "vt") == 0){
			Vector2 uv;
			myFile >> uv.x;
			myFile >> uv.y;
			temp_uvs.push_back(uv);
		}
		else if (strcmp(str.c_str(), "f") == 0){
			GLuint vertexindice;
			GLuint textureindices;
			myFile >> str;
			sscanf(str.c_str(), "%d/%d", &vertexindice, &textureindices);
			vertexIndices.push_back(vertexindice);
			textureIndices.push_back(textureindices);
			myFile >> str;
			sscanf(str.c_str(), "%d/%d", &vertexindice, &textureindices);
			vertexIndices.push_back(vertexindice);
			textureIndices.push_back(textureindices);
			myFile >> str;
			sscanf(str.c_str(), "%d/%d", &vertexindice, &textureindices);
			vertexIndices.push_back(vertexindice);
			textureIndices.push_back(textureindices);
		}
	}
	myFile.close();
	numVertex = vertexIndices.size();
	gVerticesData = new Vertex[numVertex];
	GLuint vertexIndex;
	GLuint textureIndex;
	Vector3 vertices;
	Vector2 texture;
	for (int i = 0; i < numVertex; i++)
	{
		vertexIndex = vertexIndices[i];
		vertices = temp_vertices[vertexIndex - 1];
		gVerticesData[i].pos.x = vertices.x;
		gVerticesData[i].pos.y = vertices.y;
		gVerticesData[i].pos.z = vertices.z;
		textureIndex = textureIndices[i];
		texture = temp_uvs[textureIndex - 1];
		gVerticesData[i].uv.x = texture.x;
		gVerticesData[i].uv.y = texture.y;
	}
	numIndice = numVertex;
	indices = new GLuint[numIndice];
	for (int i = 0; i < numIndice; i++)
	{
		indices[i] = i;
	}

	//delete file;
}