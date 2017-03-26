#ifndef __RESOURCEMANAGER_H__
#define __RESOURCEMANAGER_H__

#include "Struct.h"
#include "Map.h"

#include <vector>
using namespace std;

class ResourceManager
{
public:
	static ResourceManager* GetInstance();
	~ResourceManager();
	bool readSourceFile(string m_filePathPrefix);

public:
	vector<ObjectInfo> m_objPos;
	vector<GLuint> m_texID;
	vector<Program*> m_shader;
	vector<Map*> m_map;
	vector<string*> m_scenes;
	string* m_save;
	string* m_menu;

private:
	ResourceManager();
	void ReadMenu(ifstream* file, string m_filePathPrefix);
	void ReadScenes(ifstream* file, string m_filePathPrefix);
	bool ReadModels(ifstream* file, string m_filePathPrefix);
	bool ReadTextures(ifstream* file, string m_filePathPrefix);
	bool ReadShaders(ifstream* file, string m_filePathPrefix);
	bool ReadMap(ifstream* file, string m_filePathPrefix);
	void ReadSaveFile(ifstream* file, string m_filePathPrefix);
	int Init2DTexture(const char* image_path);
	void loadNFGModel(const char* fileName, Vertex* &gVerticesData, GLuint &numVertex, GLuint* &indices, GLuint &numIndice);
	void loadOBJModel(const char* fileName, Vertex* &gVerticesData, GLuint &numVertex, GLuint* &indices, GLuint &numIndice);

private:
	static ResourceManager *m_Intance;
};

#endif