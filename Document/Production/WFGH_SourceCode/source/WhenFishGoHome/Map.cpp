#include "stdafx.h"
#include "Globals.h"
#include "Map.h"
#include "Camera.h"
#include "SceneManager.h"
#include "Processor.h"

#include <fstream>

Object** Map::m_obstructors = NULL;
Object* Map::m_floor = NULL;
int Map::m_size = 0;

Map::Map()
{
}

bool Map::operator()(unsigned x, unsigned y) const
{
	if (x < m_size && y < m_size && x >= 0 && y >= 0)
	{
		if (m_data[x][y] == '0')
		{
			return true;
		}
	}
	return false;
}

bool Map::ReadMap(const char* file)
{
	std::ifstream myFile(file);

	if (!myFile.good())
		return false;

	std::string str;

	// Read Map
	// TODO: READ ID
	std::getline(myFile, str);
	// DO SOMETHING
	std::getline(myFile, str);
	sscanf(str.c_str(), "SIZE %d", &m_size);
	m_data = new char*[m_size];

	for (int i = 0; i < m_size; i++)
	{
		m_data[i] = new char[m_size + 1];
		m_data[i][m_size] = '\0';
		
		std::getline(myFile, str);
		for (int j = 0; j < m_size; j++)
		{
			m_data[i][j] = str.at(j * 2);
		}
	}

	// If first time call function
	if (m_obstructors == NULL)
		GenObjects();

	return true;
}

// Initualize all objects
void Map::GenObjects()
{
	m_floor = NULL;
	m_obstructors = new Object*[m_size * m_size];
	for (int i = 0; i < m_size * m_size; i++)
		m_obstructors[i] = NULL;
}

// Gen data vao object pointer
void Map::GenMap(Object* obstructor, Object* floor)
{
	m_floor = floor;

	for (int i = 0; i < m_size; i++)
	{
		for (int j = 0; j < m_size; j++)
		{
			if ((m_data[i][j] == '0') && (!(Vector2(i, j) == Globals::goalPos)))
			{
				if (m_obstructors[i*m_size + j])
					m_obstructors[i*m_size + j]->m_drawable = false;

				//m_obstructors[i*m_size + j] = NULL;
			}
			else
			{
				if (m_obstructors[i*m_size + j] == NULL)
					m_obstructors[i*m_size + j] = new Object();

				m_obstructors[i*m_size + j]->m_pos = obstructor->m_pos;
				m_obstructors[i*m_size + j]->m_texturePos = obstructor->m_texturePos;
				m_obstructors[i*m_size + j]->m_maModel = obstructor->m_maModel;
				m_obstructors[i*m_size + j]->m_shader = obstructor->m_shader;
				m_obstructors[i*m_size + j]->m_affCoe.translateX = -m_size / 2 + j;
				m_obstructors[i*m_size + j]->m_affCoe.translateY = obstructor->m_affCoe.translateY;
				m_obstructors[i*m_size + j]->m_affCoe.translateZ = -m_size / 2 + i;
				m_obstructors[i*m_size + j]->m_affCoe.scaleX = obstructor->m_affCoe.scaleX;
				m_obstructors[i*m_size + j]->m_affCoe.scaleY = obstructor->m_affCoe.scaleY;
				m_obstructors[i*m_size + j]->m_affCoe.scaleZ = obstructor->m_affCoe.scaleZ;
				m_obstructors[i*m_size + j]->m_drawable = true;
			}
		}
	}
}

void Map::TurnOnHighLight(vector<Vector3*>* route)
{
	int x;
	int y;

	for (vector<Vector3*>::iterator i = route->begin(); i != route->end(); ++i)
	{
		x = (int)((*i)->y + m_size / 2.0f);
		y = (int)((*i)->x + m_size / 2.0f);

		//m_data[x][y] = '1';
		m_obstructors[x*m_size + y]->m_texturePos = 4;
	}
}

void Map::TurnOffHightlightAt(float x, float y)
{
	int i = (int)(y + m_size / 2.0f);
	int j = (int)(x + m_size / 2.0f);

	m_obstructors[i*m_size + j]->m_texturePos = SceneManager::GetInstance()->m_listMapObj.at(SceneManager::GetInstance()->m_currentMap * 2)->m_texturePos;
}

void Map::TurnOffHightlight(vector<Vector3*>* route)
{
	int x;
	int y;

	for (vector<Vector3*>::iterator i = route->begin(); i != route->end(); ++i)
	{
		x = (int)((*i)->y + m_size / 2.0f);
		y = (int)((*i)->x + m_size / 2.0f);

		//m_data[x][y] = '1';
		m_obstructors[x*m_size + y]->m_texturePos = SceneManager::GetInstance()->m_listMapObj.at(SceneManager::GetInstance()->m_currentMap * 2)->m_texturePos;
	}
}

void Map::Draw()
{
	Vector2 playerPos = CoordinateToMap(SceneManager::GetInstance()->m_player->m_affCoe.translateX, SceneManager::GetInstance()->m_player->m_affCoe.translateZ);
	int left = playerPos.y - 10;
	int right = playerPos.y + 10;
	int up = playerPos.x - 10;
	int down = playerPos.x + 5;

	if (left < 0) left = 0;
	if (right > m_size - 1) right = m_size;
	if (up < 0) up = 0;
	if (down > m_size - 1) down = m_size;

	for (int i = up; i < down; i++)
	{
		for (int j = left; j < right; j++)
		{
			if (m_obstructors[i * m_size + j])
				m_obstructors[i * m_size + j]->Draw();
		}
	}

	m_floor->Draw();
}

void  Map::Update()
{
	Vector2 playerPos = CoordinateToMap(SceneManager::GetInstance()->m_player->m_affCoe.translateX, SceneManager::GetInstance()->m_player->m_affCoe.translateZ);
	int left = playerPos.y - 10;
	int right = playerPos.y + 10;
	int up = playerPos.x - 10;
	int down = playerPos.x + 5;

	if (left < 0) left = 0;
	if (right > m_size - 1) right = m_size;
	if (up < 0) up = 0;
	if (down > m_size - 1) down = m_size;

	for (int i = up; i < down; i++)
	{
		for (int j = left; j < right; j++)
		{
			if (m_obstructors[i * m_size + j])
			{
				m_obstructors[i * m_size + j]->Update();
				m_obstructors[i * m_size + j]->m_maModel = m_obstructors[i * m_size + j]->m_maModel * Camera::getInstance()->m_CameraView * Camera::getInstance()->m_Projection;
			}
		}
	}

	m_floor->Update();
	m_floor->m_maModel = m_floor->m_maModel * Camera::getInstance()->m_CameraView * Camera::getInstance()->m_Projection;

}

Map::~Map()
{
	for (int i = 0; i < m_size; i++) {
		if (m_data[i]) delete[] m_data[i];
		m_data[i] = NULL;
	}

	if (m_data) delete[] m_data;
	m_data = NULL;
}