#include "stdafx.h"
#include "SceneManager.h"
#include "Globals.h"
#include "ResourceManager.h"
#include "Camera.h"
#include "Shark.h"
#include "Fish.h"
#include "Processor.h"
#include <fstream>
#include <time.h>

SceneManager* SceneManager::m_Instance = NULL;

//------------------------------------------------------------------
/// PUBLIC METHODS
//------------------------------------------------------------------
SceneManager::SceneManager() :
m_stageID(0),
m_currentMap(0),
m_player(NULL),
m_isFBO(true),
m_isMouseDown(false),
m_chosenButton(NULL),
m_menuScroll(NULL),
m_isEndSceneOn(false),
m_gate(NULL)
{
	//ResourceManager::GetInstance()->readSourceFile();
	//Init();
}

bool SceneManager::Init()
{
	bool success = ReadScene();
	InitGL();
	InitGame();
	return success;
}

void SceneManager::LMouseDown(int mouse_x, int mouse_y)
{
	m_player->m_xVel = 0.0f;
	m_player->m_zVel = 0.0f;
	m_dMouseAngle = 0.0f;
	m_mousePos.x = mouse_x;
	m_mousePos.y = mouse_y;
	m_isMouseDown = true;
}

void SceneManager::LMouseUp(int mouse_x, int mouse_y)
{
	if (m_chosenButton && m_chosenButton->m_isHovered)
	{
		m_chosenButton->DoClick();
	}

	if (m_isMouseDown && Globals::isEndGame)
	{
		m_isEndSceneOn = false;
	}

	m_isMouseDown = false;
}

void SceneManager::MouseMove(int mouse_x, int mouse_y)
{
	if (m_isMouseDown && m_chosenButton == NULL)
	{
		m_dMouseAngle = 90.0f - RAD2DEG(atan2((mouse_y - m_mousePos.y), (mouse_x - m_mousePos.x)));
		m_player->m_fRotateAngle = m_dMouseAngle;
		m_player->m_xVel = m_player->m_fPlayerSpeed * sin(DEG2RAD(m_dMouseAngle));
		m_player->m_zVel = m_player->m_fPlayerSpeed * cos(DEG2RAD(m_dMouseAngle));
	}
	else
	{
		m_mousePos.x = mouse_x;
		m_mousePos.y = mouse_y;
	}
}

void SceneManager::SetStage(GLubyte stageID)
{
	m_stageID = stageID;
}

void SceneManager::EndGame(bool isWin)
{
	Globals::isEndGame = true;
	Globals::isWinGame = isWin;
	m_isFBO = false;
	m_analogStick.front()->m_drawable = false;
	m_analogStick.back()->m_drawable = false;

	for (vector<Button*>::iterator i = m_lstButton.begin(); i != m_lstButton.end(); ++i)
	{
		(*i)->m_drawable = false;
	}

	m_isMouseDown = false;
	m_isEndSceneOn = true;
}

void  SceneManager::Update()
{
	ProcessRayCast();

	if (m_isActive && !Globals::isEndGame)
	{
		ProcessController();
		ProcessAI();
		ProcessPlayer();
	}
	else if (!m_isActive)
	{
		ProcessMenu();
	}
	else if (m_isEndSceneOn)
	{
		ShowEndScene();
	}
	else
	{
		if (Globals::isWinGame)
			ChangeStage();
		else
			Pause();
	}

	UpdateAllObjects();
}

void SceneManager::Draw()
{
	if (m_isFBO)
	{
		//Draw FBO
		glBindFramebuffer(GL_FRAMEBUFFER, MiniMap::GetInstance()->fboID);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		Camera::getInstance()->SetFBOLookAt();
		UpdateAllObjects();
		DrawStaticObjects();
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		//Draw Scene
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		Camera::getInstance()->SetLookAt();
		UpdateAllObjects();
		MiniMap::GetInstance()->m_map->Draw();
		MiniMap::GetInstance()->m_border->Draw();
		DrawStaticObjects();
		DrawDynamicObjects();
	}
	else
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		DrawStaticObjects();
		DrawDynamicObjects();
	}
}

SceneManager* SceneManager::GetInstance()
{
	if (m_Instance == NULL)
	{
		m_Instance = new SceneManager();
	}

	return m_Instance;
}

void SceneManager::ResetData()
{
	Globals::isEndGame = false;
	Globals::isWinGame = false;
	m_isFBO = true;

	if (m_Instance != NULL)
	{
		if (m_player)
		{
			m_player->ResetPlayer();
		}

		m_isMouseDown = false;
		m_chosenButton = NULL;

		m_waterCaustic.m_drawable = false;

		if (m_gate)
			DestroyObstacleAt(CoordinateToMap(m_gate->m_affCoe.translateX, m_gate->m_affCoe.translateZ));

		for (vector<Object*>::iterator i = m_listShark.begin(); i != m_listShark.end(); ++i)
		{
			((Shark*)(*i))->ResetShark();
		}

		for (vector<Object*>::iterator i = m_listFish.begin(); i != m_listFish.end(); ++i)
		{
			((Fish*)(*i))->ClearFishPath();
			(*i)->m_drawable = false;
		}

		for (vector<Object*>::iterator i = m_listItem.begin(); i != m_listItem.end(); ++i)
		{
			((Item*)(*i))->m_isBeingUsed = false;
			(*i)->m_drawable = false;
		}

		for (vector<Object*>::iterator i = m_listMapObj.begin(); i != m_listMapObj.end(); ++i)
		{
			(*i)->m_drawable = false;
		}

		for (vector<Object*>::iterator i = m_winLose.begin(); i != m_winLose.end(); ++i)
		{
			(*i)->m_drawable = false;
		}

		for (vector<Button*>::iterator i = m_lstButton.begin(); i != m_lstButton.end(); ++i)
		{
			(*i)->m_drawable = false;
		}

		if (m_menuScroll)
			m_menuScroll->m_affCoe.translateY += 1.8;

		for (vector<Button*>::iterator i = m_lstSubMenuButton.begin(); i != m_lstSubMenuButton.end(); ++i)
		{
			(*i)->m_affCoe.translateY += 1.8;
		}

		for (vector<Object*>::iterator i = m_winLose.begin(); i != m_winLose.end(); ++i)
		{
			(*i)->m_affCoe.translateY = 1.5f;
		}
	}
}

SceneManager::~SceneManager()
{
	if (m_Instance != NULL)
	{
		if (m_player) delete m_player;
		m_player = NULL;

		if (m_menuScroll) delete m_menuScroll;
		m_menuScroll = NULL;

		if (m_gate) delete m_gate;
		m_gate = NULL;

		//if (m_waterCaustic) delete m_waterCaustic;
		//m_waterCaustic = NULL;

		for (vector<Object*>::iterator i = m_listShark.begin(); i != m_listShark.end();)
		{
			Object* temp = *i;
			i++;
			delete temp;
		}

		for (vector<Object*>::iterator i = m_listFish.begin(); i != m_listFish.end();)
		{
			Object* temp = *i;
			i++;
			delete temp;
		}

		for (vector<Object*>::iterator i = m_listItem.begin(); i != m_listItem.end();)
		{
			Object* temp = *i;
			i++;
			delete temp;
		}

		for (vector<Object*>::iterator i = m_listMapObj.begin(); i != m_listMapObj.end();)
		{
			Object* temp = *i;
			i++;
			delete temp;
		}

		for (vector<Button*>::iterator i = m_lstButton.begin(); i != m_lstButton.end();)
		{
			Object* temp = *i;
			i++;
			delete temp;
		}

		for (vector<Button*>::iterator i = m_lstSubMenuButton.begin(); i != m_lstSubMenuButton.end();)
		{
			Object* temp = *i;
			i++;
			delete temp;
		}

		for (vector<Object*>::iterator i = m_analogStick.begin(); i != m_analogStick.end();)
		{
			Object* temp = *i;
			i++;
			delete temp;
		}

		for (vector<Object*>::iterator i = m_winLose.begin(); i != m_winLose.end();)
		{
			Object* temp = *i;
			i++;
			delete temp;
		}

		m_map = NULL;

		m_listShark.clear();
		m_listFish.clear();
		m_listItem.clear();
		m_listMapObj.clear();
		m_lstButton.clear();

		//delete ResourceManager::GetInstance();
		delete Camera::getInstance();
		delete MiniMap::GetInstance();
		m_Instance = NULL;
	}
}

//------------------------------------------------------------------
/// PRIVATE METHODS
//------------------------------------------------------------------

//-Init Game
void SceneManager::InitGL()
{
	//xoa bo dem mau va do sau
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClearDepthf(1.0f);

	// Enable depth buffer
	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_BACK);
}

void SceneManager::InitGame()
{
	m_isEvent = 0;

	// Setup camera
	Camera::getInstance()->SetLookAt();
	Camera::getInstance()->m_vEye.x += m_player->m_affCoe.translateX;
	Camera::getInstance()->m_vEye.z += m_player->m_affCoe.translateZ;

	// Setup Gate
	SetObstacleAt(CoordinateToMap(m_gate->m_affCoe.translateX, m_gate->m_affCoe.translateZ));

	// Randomize item position
	for (vector<Object*>::iterator i = m_listItem.begin(); i != m_listItem.end(); ++i)
	{
		((Item*)(*i))->Drop();
	}

	m_startTime = clock();
}

//-Read File Scene Manager
void SceneManager::ReadObjectInfo(ifstream *file, Type type)
{
	char itemType[20];
	string str;

	GLuint objID;
	GLuint texIndex;
	GLuint modelIndex;
	GLuint shaderIndex;
	GLuint cubeIndex;
	GLuint modelCount;

	vector<Object*>* listObj;

	std::getline(*file, str);

	switch (type)
	{
	case PLAYER:
		sscanf(str.c_str(), "#PLAYER: %d", &modelCount);
		break;
	case SHARK:
		sscanf(str.c_str(), "#SHARK: %d", &modelCount);
		break;
	case FISH:
		sscanf(str.c_str(), "#FISH: %d", &modelCount);
		break;
	case ITEM:
		sscanf(str.c_str(), "#ITEM: %d", &modelCount);
		break;
	}

	for (unsigned int i = 0; i < modelCount; i++)
	{
		getline(*file, str);
		sscanf(str.c_str(), "ID %d", &objID);

		if (type == ITEM)
		{
			getline(*file, str);
			sscanf(str.c_str(), "TYPE %s", itemType);
		}

		getline(*file, str);
		sscanf(str.c_str(), "MODEL %d", &modelIndex);
		getline(*file, str);
		sscanf(str.c_str(), "TEXTURE %d", &texIndex);
		getline(*file, str);
		sscanf(str.c_str(), "CUBETEXTURES %d", &cubeIndex);
		getline(*file, str);
		sscanf(str.c_str(), "SHADER %d", &shaderIndex);

		switch (type)
		{
		case PLAYER:
			if (m_player == NULL)
				m_player = new Player();
			m_player->m_id = objID;
			m_player->Init(modelIndex, texIndex, shaderIndex);
			break;
		case SHARK:
			if (m_listShark.size() < modelCount)
				m_listShark.push_back(new Shark());
			m_listShark.at(i)->m_id = objID;
			m_listShark.at(i)->Init(modelIndex, texIndex, shaderIndex);
			listObj = &m_listShark;
			break;
		case FISH:
			if (m_listFish.size() < modelCount)
				m_listFish.push_back(new Fish());
			m_listFish.at(i)->m_id = objID;
			m_listFish.at(i)->Init(modelIndex, texIndex, shaderIndex);
			listObj = &m_listFish;
			break;
		case ITEM:
			if (m_listItem.size() < modelCount)
				m_listItem.push_back(new Item());

			if (strcmp(itemType, "WEED") == 0)
				((Item*)m_listItem.at(i))->SetItemType(WEED);
			else if (strcmp(itemType, "GREENTURTLE") == 0)
				((Item*)m_listItem.at(i))->SetItemType(GREENTURTLE);
			else if (strcmp(itemType, "REDTURTLE") == 0)
				((Item*)m_listItem.at(i))->SetItemType(REDTURTLE);
			else if (strcmp(itemType, "STARFISH") == 0)
				((Item*)m_listItem.at(i))->SetItemType(STARFISH);
			else if (strcmp(itemType, "POISON") == 0)
				((Item*)m_listItem.at(i))->SetItemType(POISON);
			else
				((Item*)m_listItem.at(i))->SetItemType(KEY);

			m_listItem.at(i)->m_id = objID;
			m_listItem.at(i)->Init(modelIndex, texIndex, shaderIndex);
			listObj = &m_listItem;
			break;
		}

		// Read Model's Matrix
		if (type == PLAYER)
		{
			getline(*file, str);
			sscanf(str.c_str(), "POSITION %f, %f, %f", &m_player->m_affCoe.translateX, &m_player->m_affCoe.translateY, &m_player->m_affCoe.translateZ);
			getline(*file, str);
			sscanf(str.c_str(), "ROTATION %f, %f, %f", &m_player->m_affCoe.rotateX, &m_player->m_affCoe.rotateY, &m_player->m_affCoe.rotateZ);
			getline(*file, str);
			sscanf(str.c_str(), "SCALE %f, %f, %f", &m_player->m_affCoe.scaleX, &m_player->m_affCoe.scaleY, &m_player->m_affCoe.scaleZ);
		}
		else
		{
			getline(*file, str);
			sscanf(str.c_str(), "POSITION %f, %f, %f", &listObj->at(i)->m_affCoe.translateX, &listObj->at(i)->m_affCoe.translateY, &listObj->at(i)->m_affCoe.translateZ);
			getline(*file, str);
			sscanf(str.c_str(), "ROTATION %f, %f, %f", &listObj->at(i)->m_affCoe.rotateX, &listObj->at(i)->m_affCoe.rotateY, &listObj->at(i)->m_affCoe.rotateZ);
			getline(*file, str);
			sscanf(str.c_str(), "SCALE %f, %f, %f", &listObj->at(i)->m_affCoe.scaleX, &listObj->at(i)->m_affCoe.scaleY, &listObj->at(i)->m_affCoe.scaleZ);
			listObj->at(i)->m_drawable = true;
		}
	}

	listObj = NULL;
}

void SceneManager::ReadMapInfo(ifstream *file)
{
	string str;
	GLuint texPos;
	GLuint texIndex;
	GLuint modelIndex;
	GLuint shaderIndex;
	GLuint mapCount;
	GLuint mapPos;

	std::getline(*file, str);
	std::getline(*file, str);
	sscanf(str.c_str(), "ID %d", &texPos);
	std::getline(*file, str);
	sscanf(str.c_str(), "MAP %d", &mapPos);

	m_map = ResourceManager::GetInstance()->m_map.at(mapPos);

	// Read Obstacle object
	if (m_listMapObj.size() == 0)
		m_listMapObj.push_back(new Object());

	std::getline(*file, str);
	std::getline(*file, str);
	sscanf(str.c_str(), "MODEL %d", &modelIndex);
	std::getline(*file, str);
	sscanf(str.c_str(), "TEXTURE %d", &texIndex);
	std::getline(*file, str);
	sscanf(str.c_str(), "SHADER %d", &shaderIndex);
	std::getline(*file, str);
	sscanf(str.c_str(), "POSITION %f, %f, %f", &m_listMapObj.front()->m_affCoe.translateX, &m_listMapObj.front()->m_affCoe.translateY, &m_listMapObj.front()->m_affCoe.translateZ);
	std::getline(*file, str);
	sscanf(str.c_str(), "ROTATION %f, %f, %f", &m_listMapObj.front()->m_affCoe.rotateX, &m_listMapObj.front()->m_affCoe.rotateY, &m_listMapObj.front()->m_affCoe.rotateZ);
	std::getline(*file, str);
	sscanf(str.c_str(), "SCALE %f, %f, %f", &m_listMapObj.front()->m_affCoe.scaleX, &m_listMapObj.front()->m_affCoe.scaleY, &m_listMapObj.front()->m_affCoe.scaleZ);

	m_listMapObj.front()->Init(modelIndex, texIndex, shaderIndex);
	m_listMapObj.front()->m_drawable = true;

	// Read floor object
	if (m_listMapObj.size() == 1)
		m_listMapObj.push_back(new Object());

	std::getline(*file, str);
	std::getline(*file, str);
	sscanf(str.c_str(), "MODEL %d", &modelIndex);
	std::getline(*file, str);
	sscanf(str.c_str(), "TEXTURE %d", &texIndex);
	std::getline(*file, str);
	sscanf(str.c_str(), "SHADER %d", &shaderIndex);
	std::getline(*file, str);
	sscanf(str.c_str(), "POSITION %f, %f, %f", &m_listMapObj.at(1)->m_affCoe.translateX, &m_listMapObj.at(1)->m_affCoe.translateY, &m_listMapObj.at(1)->m_affCoe.translateZ);
	std::getline(*file, str);
	sscanf(str.c_str(), "ROTATION %f, %f, %f", &m_listMapObj.at(1)->m_affCoe.rotateX, &m_listMapObj.at(1)->m_affCoe.rotateY, &m_listMapObj.at(1)->m_affCoe.rotateZ);
	std::getline(*file, str);
	sscanf(str.c_str(), "SCALE %f, %f, %f", &m_listMapObj.at(1)->m_affCoe.scaleX, &m_listMapObj.at(1)->m_affCoe.scaleY, &m_listMapObj.at(1)->m_affCoe.scaleZ);

	m_listMapObj.at(1)->Init(modelIndex, texIndex, shaderIndex);
	m_listMapObj.at(1)->m_drawable = true;
}

void SceneManager::ReadMiniMap(ifstream *file)
{
	string str;
	GLuint modelIndex;
	GLuint shaderIndex;
	GLuint texIndex;

	std::getline(*file, str);
	std::getline(*file, str);
	std::getline(*file, str);
	sscanf(str.c_str(), "MODEL %d", &modelIndex);
	std::getline(*file, str);
	sscanf(str.c_str(), "SHADER %d", &shaderIndex);
	std::getline(*file, str);
	sscanf(str.c_str(), "POSITION %f, %f, %f", &MiniMap::GetInstance()->m_map->m_affCoe.translateX, &MiniMap::GetInstance()->m_map->m_affCoe.translateY, &MiniMap::GetInstance()->m_map->m_affCoe.translateZ);
	std::getline(*file, str);
	sscanf(str.c_str(), "ROTATION %f, %f, %f", &MiniMap::GetInstance()->m_map->m_affCoe.rotateX, &MiniMap::GetInstance()->m_map->m_affCoe.rotateY, &MiniMap::GetInstance()->m_map->m_affCoe.rotateZ);
	std::getline(*file, str);
	sscanf(str.c_str(), "SCALE %f, %f, %f", &MiniMap::GetInstance()->m_map->m_affCoe.scaleX, &MiniMap::GetInstance()->m_map->m_affCoe.scaleY, &MiniMap::GetInstance()->m_map->m_affCoe.scaleZ);

	// Read FBO View Matrix
	std::getline(*file, str);
	sscanf(str.c_str(), "EYE %f %f %f", &Camera::getInstance()->m_fboEye.x, &Camera::getInstance()->m_fboEye.y, &Camera::getInstance()->m_fboEye.z);

	MiniMap::GetInstance()->m_map->m_pos = ResourceManager::GetInstance()->m_objPos.at(modelIndex);
	MiniMap::GetInstance()->m_map->m_shader = ResourceManager::GetInstance()->m_shader.at(shaderIndex);

	std::getline(*file, str);
	std::getline(*file, str);
	sscanf(str.c_str(), "MODEL %d", &modelIndex);
	std::getline(*file, str);
	sscanf(str.c_str(), "TEXTURE %d", &texIndex);
	std::getline(*file, str);
	sscanf(str.c_str(), "SHADER %d", &shaderIndex);
	std::getline(*file, str);
	sscanf(str.c_str(), "POSITION %f, %f, %f", &MiniMap::GetInstance()->m_border->m_affCoe.translateX, &MiniMap::GetInstance()->m_border->m_affCoe.translateY, &MiniMap::GetInstance()->m_border->m_affCoe.translateZ);
	std::getline(*file, str);
	sscanf(str.c_str(), "ROTATION %f, %f, %f", &MiniMap::GetInstance()->m_border->m_affCoe.rotateX, &MiniMap::GetInstance()->m_border->m_affCoe.rotateY, &MiniMap::GetInstance()->m_border->m_affCoe.rotateZ);
	std::getline(*file, str);
	sscanf(str.c_str(), "SCALE %f, %f, %f", &MiniMap::GetInstance()->m_border->m_affCoe.scaleX, &MiniMap::GetInstance()->m_border->m_affCoe.scaleY, &MiniMap::GetInstance()->m_border->m_affCoe.scaleZ);

	MiniMap::GetInstance()->m_border->m_pos = ResourceManager::GetInstance()->m_objPos.at(modelIndex);
	MiniMap::GetInstance()->m_border->m_texturePos = ResourceManager::GetInstance()->m_texID.at(texIndex);
	MiniMap::GetInstance()->m_border->m_shader = ResourceManager::GetInstance()->m_shader.at(shaderIndex);
}

void SceneManager::ReadSkyDome(ifstream *file)
{
	string str;
	GLuint texPos;
	GLuint texIndex;
	GLuint modelIndex;
	GLuint shaderIndex;
	GLuint mapCount;
	GLuint mapPos;

	std::getline(*file, str);

	std::getline(*file, str);
	sscanf(str.c_str(), "ID %d", &texPos);

	if (m_listMapObj.size() == 2)
		m_listMapObj.push_back(new Object());

	std::getline(*file, str);
	sscanf(str.c_str(), "MODEL %d", &modelIndex);
	std::getline(*file, str);
	sscanf(str.c_str(), "TEXTURE %d", &texIndex);
	std::getline(*file, str);
	sscanf(str.c_str(), "SHADER %d", &shaderIndex);
	std::getline(*file, str);
	sscanf(str.c_str(), "POSITION %f, %f, %f", &m_listMapObj.back()->m_affCoe.translateX, &m_listMapObj.back()->m_affCoe.translateY, &m_listMapObj.back()->m_affCoe.translateZ);
	std::getline(*file, str);
	sscanf(str.c_str(), "ROTATION %f, %f, %f", &m_listMapObj.back()->m_affCoe.rotateX, &m_listMapObj.back()->m_affCoe.rotateY, &m_listMapObj.back()->m_affCoe.rotateZ);
	std::getline(*file, str);
	sscanf(str.c_str(), "SCALE %f, %f, %f", &m_listMapObj.back()->m_affCoe.scaleX, &m_listMapObj.back()->m_affCoe.scaleY, &m_listMapObj.back()->m_affCoe.scaleZ);

	m_listMapObj.back()->Init(modelIndex, texIndex, shaderIndex);
	m_listMapObj.back()->m_drawable = true;
}

void SceneManager::ReadGameSpace(ifstream *file)
{
	std::string str;
	float f_fov;
	float f_near;
	float f_far;

	// Read Perspective Matrix
	std::getline(*file, str);
	std::getline(*file, str);
	sscanf(str.c_str(), "NEAR %f", &f_near);
	std::getline(*file, str);
	sscanf(str.c_str(), "FAR %f", &f_far);
	std::getline(*file, str);
	sscanf(str.c_str(), "FOV %f", &f_fov);
	Camera::getInstance()->m_Projection.SetPerspective(DEG2RAD(f_fov), (float)Globals::screenWidth / (float)Globals::screenHeight, f_near, f_far);

	// Read View Matrix
	std::getline(*file, str);
	sscanf(str.c_str(), "EYE %f %f %f", &Camera::getInstance()->m_vEye.x, &Camera::getInstance()->m_vEye.y, &Camera::getInstance()->m_vEye.z);
	std::getline(*file, str);
	sscanf(str.c_str(), "CENTER %f %f %f", &Camera::getInstance()->m_vLook.x, &Camera::getInstance()->m_vLook.y, &Camera::getInstance()->m_vLook.z);
	std::getline(*file, str);
	sscanf(str.c_str(), "UP %f %f %f", &Camera::getInstance()->m_vUp.x, &Camera::getInstance()->m_vUp.y, &Camera::getInstance()->m_vUp.z);

}

void SceneManager::ReadButtonsInfo(ifstream* file)
{
	char buttonType[20];
	GLuint buttonCount;
	GLuint objID;
	GLuint unClickedTexIndex;
	GLuint clickedTexIndex;
	GLuint modelIndex;
	GLuint shaderIndex;

	std::string str;

	getline(*file, str);
	sscanf(str.c_str(), "#BUTTON: %d", &buttonCount);

	// Pause button
	getline(*file, str);
	sscanf(str.c_str(), "ID %d", &objID);
	getline(*file, str);
	sscanf(str.c_str(), "TYPE %s", buttonType);
	getline(*file, str);
	sscanf(str.c_str(), "MODEL %d", &modelIndex);
	getline(*file, str);
	sscanf(str.c_str(), "TEXTURE %d %d", &unClickedTexIndex, &clickedTexIndex);
	getline(*file, str);
	sscanf(str.c_str(), "SHADER %d", &shaderIndex);

	if (m_lstButton.size() < buttonCount)
		m_lstButton.push_back(new Button());
	m_lstButton.back()->m_id = objID;
	m_lstButton.back()->Init(modelIndex, unClickedTexIndex, shaderIndex);
	m_lstButton.back()->m_UnClickedTexPos = ResourceManager::GetInstance()->m_texID.at(unClickedTexIndex);
	m_lstButton.back()->m_clickedTexPos = ResourceManager::GetInstance()->m_texID.at(clickedTexIndex);
	m_lstButton.back()->m_drawable = true;

	m_lstButton.back()->SetButtonType(PAUSE);

	getline(*file, str);
	sscanf(str.c_str(), "POSITION %f, %f, %f", &m_lstButton.back()->m_affCoe.translateX, &m_lstButton.back()->m_affCoe.translateY, &m_lstButton.back()->m_affCoe.translateZ);
	getline(*file, str);
	sscanf(str.c_str(), "ROTATION %f, %f, %f", &m_lstButton.back()->m_affCoe.rotateX, &m_lstButton.back()->m_affCoe.rotateY, &m_lstButton.back()->m_affCoe.rotateZ);
	getline(*file, str);
	sscanf(str.c_str(), "SCALE %f, %f, %f", &m_lstButton.back()->m_affCoe.scaleX, &m_lstButton.back()->m_affCoe.scaleY, &m_lstButton.back()->m_affCoe.scaleZ);

	// Drop item button
	int a, b, c;
	getline(*file, str);
	sscanf(str.c_str(), "ID %d", &objID);
	getline(*file, str);
	sscanf(str.c_str(), "TYPE %s", buttonType);
	getline(*file, str);
	sscanf(str.c_str(), "MODEL %d", &modelIndex);
	getline(*file, str);
	sscanf(str.c_str(), "TEXTURE %d %d %d %d %d", &unClickedTexIndex, &clickedTexIndex, &a, &b, &c);
	getline(*file, str);
	sscanf(str.c_str(), "SHADER %d", &shaderIndex);

	if (m_lstButton.size() < buttonCount)
		m_lstButton.push_back(new Button());
	m_lstButton.back()->m_id = objID;
	m_lstButton.back()->Init(modelIndex, unClickedTexIndex, shaderIndex);
	m_lstButton.back()->m_UnClickedTexPos = ResourceManager::GetInstance()->m_texID.at(unClickedTexIndex);
	m_lstButton.back()->m_clickedTexPos = ResourceManager::GetInstance()->m_texID.at(clickedTexIndex);
	m_listItemTexture.push_back(ResourceManager::GetInstance()->m_texID.at(a));
	m_listItemTexture.push_back(ResourceManager::GetInstance()->m_texID.at(b));
	m_listItemTexture.push_back(ResourceManager::GetInstance()->m_texID.at(c));
	m_lstButton.back()->m_drawable = true;

	m_lstButton.back()->SetButtonType(DROP_ITEM);

	getline(*file, str);
	sscanf(str.c_str(), "POSITION %f, %f, %f", &m_lstButton.back()->m_affCoe.translateX, &m_lstButton.back()->m_affCoe.translateY, &m_lstButton.back()->m_affCoe.translateZ);
	getline(*file, str);
	sscanf(str.c_str(), "ROTATION %f, %f, %f", &m_lstButton.back()->m_affCoe.rotateX, &m_lstButton.back()->m_affCoe.rotateY, &m_lstButton.back()->m_affCoe.rotateZ);
	getline(*file, str);
	sscanf(str.c_str(), "SCALE %f, %f, %f", &m_lstButton.back()->m_affCoe.scaleX, &m_lstButton.back()->m_affCoe.scaleY, &m_lstButton.back()->m_affCoe.scaleZ);
}

void SceneManager::ReadSubMenu(ifstream *file)
{
	std::string str;
	char buttonType[20];
	GLuint buttonCount;
	GLuint objID;
	GLuint unClickedTexIndex;
	GLuint clickedTexIndex;
	GLuint texIndex;
	GLuint modelIndex;
	GLuint shaderIndex;

	// Read scroll
	getline(*file, str);
	getline(*file, str);
	getline(*file, str);
	sscanf(str.c_str(), "ID %d", &objID);
	getline(*file, str);
	sscanf(str.c_str(), "MODEL %d", &modelIndex);
	getline(*file, str);
	sscanf(str.c_str(), "TEXTURE %d", &texIndex);
	getline(*file, str);
	sscanf(str.c_str(), "SHADER %d", &shaderIndex);

	if (m_menuScroll == NULL)
		m_menuScroll = new Object();
	m_menuScroll->m_id = objID;
	m_menuScroll->Init(modelIndex, texIndex, shaderIndex);
	m_menuScroll->m_drawable = true;

	getline(*file, str);
	sscanf(str.c_str(), "POSITION %f, %f, %f", &m_menuScroll->m_affCoe.translateX, &m_menuScroll->m_affCoe.translateY, &m_menuScroll->m_affCoe.translateZ);
	getline(*file, str);
	sscanf(str.c_str(), "ROTATION %f, %f, %f", &m_menuScroll->m_affCoe.rotateX, &m_menuScroll->m_affCoe.rotateY, &m_menuScroll->m_affCoe.rotateZ);
	getline(*file, str);
	sscanf(str.c_str(), "SCALE %f, %f, %f", &m_menuScroll->m_affCoe.scaleX, &m_menuScroll->m_affCoe.scaleY, &m_menuScroll->m_affCoe.scaleZ);

	// Read Submenu buttons
	getline(*file, str);
	sscanf(str.c_str(), "#BUTTON: %d", &buttonCount);


	for (unsigned int i = 0; i < buttonCount; i++)
	{
		getline(*file, str);
		sscanf(str.c_str(), "ID %d", &objID);
		getline(*file, str);
		sscanf(str.c_str(), "TYPE %s", buttonType);
		getline(*file, str);
		sscanf(str.c_str(), "MODEL %d", &modelIndex);
		getline(*file, str);
		sscanf(str.c_str(), "TEXTURE %d %d", &unClickedTexIndex, &clickedTexIndex);
		getline(*file, str);
		sscanf(str.c_str(), "SHADER %d", &shaderIndex);

		if (m_lstSubMenuButton.size() < buttonCount)
			m_lstSubMenuButton.push_back(new Button());
		m_lstSubMenuButton.at(i)->m_id = objID;
		m_lstSubMenuButton.at(i)->Init(modelIndex, unClickedTexIndex, shaderIndex);
		m_lstSubMenuButton.at(i)->m_UnClickedTexPos = ResourceManager::GetInstance()->m_texID.at(unClickedTexIndex);
		m_lstSubMenuButton.at(i)->m_clickedTexPos = ResourceManager::GetInstance()->m_texID.at(clickedTexIndex);
		m_lstSubMenuButton.at(i)->m_drawable = true;

		if (strcmp(buttonType, "RESUME") == 0)
			m_lstSubMenuButton.at(i)->SetButtonType(RESUME);
		else if (strcmp(buttonType, "QUIT") == 0)
			m_lstSubMenuButton.at(i)->SetButtonType(QUIT);
		else
			m_lstSubMenuButton.at(i)->SetButtonType(REPLAY);

		getline(*file, str);
		sscanf(str.c_str(), "POSITION %f, %f, %f", &m_lstSubMenuButton.at(i)->m_affCoe.translateX, &m_lstSubMenuButton.at(i)->m_affCoe.translateY, &m_lstSubMenuButton.at(i)->m_affCoe.translateZ);
		getline(*file, str);
		sscanf(str.c_str(), "ROTATION %f, %f, %f", &m_lstSubMenuButton.at(i)->m_affCoe.rotateX, &m_lstSubMenuButton.at(i)->m_affCoe.rotateY, &m_lstSubMenuButton.at(i)->m_affCoe.rotateZ);
		getline(*file, str);
		sscanf(str.c_str(), "SCALE %f, %f, %f", &m_lstSubMenuButton.at(i)->m_affCoe.scaleX, &m_lstSubMenuButton.at(i)->m_affCoe.scaleY, &m_lstSubMenuButton.at(i)->m_affCoe.scaleZ);
	}
}

void SceneManager::ReadControl(ifstream *file)
{
	std::string str;
	GLuint count;
	GLuint objID;
	GLuint texIndex;
	GLuint modelIndex;
	GLuint shaderIndex;

	getline(*file, str);
	sscanf(str.c_str(), "#CONTROL: %d", &count);

	for (unsigned int i = 0; i < count; i++)
	{
		getline(*file, str);
		sscanf(str.c_str(), "ID %d", &objID);
		getline(*file, str);
		sscanf(str.c_str(), "MODEL %d", &modelIndex);
		getline(*file, str);
		sscanf(str.c_str(), "TEXTURE %d", &texIndex);
		getline(*file, str);
		sscanf(str.c_str(), "SHADER %d", &shaderIndex);

		if (m_analogStick.size() <  count)
			m_analogStick.push_back(new Object());
		m_analogStick.at(i)->m_id = objID;
		m_analogStick.at(i)->Init(modelIndex, texIndex, shaderIndex);
		m_analogStick.at(i)->m_drawable = false;

		getline(*file, str);
		sscanf(str.c_str(), "POSITION %f, %f, %f", &m_analogStick.at(i)->m_affCoe.translateX, &m_analogStick.at(i)->m_affCoe.translateY, &m_analogStick.at(i)->m_affCoe.translateZ);
		getline(*file, str);
		sscanf(str.c_str(), "ROTATION %f, %f, %f", &m_analogStick.at(i)->m_affCoe.rotateX, &m_analogStick.at(i)->m_affCoe.rotateY, &m_analogStick.at(i)->m_affCoe.rotateZ);
		getline(*file, str);
		sscanf(str.c_str(), "SCALE %f, %f, %f", &m_analogStick.at(i)->m_affCoe.scaleX, &m_analogStick.at(i)->m_affCoe.scaleY, &m_analogStick.at(i)->m_affCoe.scaleZ);
	}
}

void SceneManager::ReadGameConfig(ifstream *file)
{
	std::string str;

	std::getline(*file, str);
	std::getline(*file, str);
	//sscanf(str.c_str(), "GOAL %f, %f", &m_vGoalPos.x, &m_vGoalPos.y);
	sscanf(str.c_str(), "GOAL %f, %f", &Globals::goalPos.x, &Globals::goalPos.y);
	std::getline(*file, str);
	sscanf(str.c_str(), "PLAYERSPEED %f", &m_player->m_fPlayerSpeed);

	std::getline(*file, str);
	for (vector<Object*>::iterator i = m_listFish.begin(); i != m_listFish.end(); ++i)
	{
		sscanf(str.c_str(), "FISHSPEED %f", &((Fish*)(*i))->m_fFishSpeed);
	}

	std::getline(*file, str);
	for (vector<Object*>::iterator i = m_listShark.begin(); i != m_listShark.end(); ++i)
	{
		sscanf(str.c_str(), "SHARKSPEED %f", &((Shark*)(*i))->m_fSharkSpeed);
	}
}

void SceneManager::ReadWinLoseScene(ifstream *file)
{
	string str;
	GLuint objID;
	GLuint texIndex;
	GLuint modelIndex;
	GLuint shaderIndex;

	getline(*file, str);
	for (unsigned int i = 0; i < 2; i++)
	{
		getline(*file, str);
		sscanf(str.c_str(), "ID %d", &objID);
		getline(*file, str);
		sscanf(str.c_str(), "MODEL %d", &modelIndex);
		getline(*file, str);
		sscanf(str.c_str(), "TEXTURE %d", &texIndex);
		getline(*file, str);
		sscanf(str.c_str(), "SHADER %d", &shaderIndex);

		if (m_winLose.size() < 2)
			m_winLose.push_back(new Object());
		m_winLose.at(i)->m_id = objID;
		m_winLose.at(i)->Init(modelIndex, texIndex, shaderIndex);
		m_winLose.at(i)->m_drawable = false;

		getline(*file, str);
		sscanf(str.c_str(), "POSITION %f, %f, %f", &m_winLose.at(i)->m_affCoe.translateX, &m_winLose.at(i)->m_affCoe.translateY, &m_winLose.at(i)->m_affCoe.translateZ);
		getline(*file, str);
		sscanf(str.c_str(), "ROTATION %f, %f, %f", &m_winLose.at(i)->m_affCoe.rotateX, &m_winLose.at(i)->m_affCoe.rotateY, &m_winLose.at(i)->m_affCoe.rotateZ);
		getline(*file, str);
		sscanf(str.c_str(), "SCALE %f, %f, %f", &m_winLose.at(i)->m_affCoe.scaleX, &m_winLose.at(i)->m_affCoe.scaleY, &m_winLose.at(i)->m_affCoe.scaleZ);
	}
}

void SceneManager::ReadGate(ifstream *file)
{
	string str;
	GLuint objID;
	GLuint texIndex;
	GLuint modelIndex;
	GLuint shaderIndex;

	getline(*file, str);
	getline(*file, str);
	sscanf(str.c_str(), "ID %d", &objID);
	getline(*file, str);
	sscanf(str.c_str(), "MODEL %d", &modelIndex);
	getline(*file, str);
	sscanf(str.c_str(), "TEXTURE %d", &texIndex);
	getline(*file, str);
	sscanf(str.c_str(), "SHADER %d", &shaderIndex);

	if (m_gate == NULL)
		m_gate = new Object();
	m_gate->m_id = objID;
	m_gate->Init(modelIndex, texIndex, shaderIndex);
	m_gate->m_drawable = true;

	getline(*file, str);
	sscanf(str.c_str(), "POSITION %f, %f, %f", &m_gate->m_affCoe.translateX, &m_gate->m_affCoe.translateY, &m_gate->m_affCoe.translateZ);
	getline(*file, str);
	sscanf(str.c_str(), "ROTATION %f, %f, %f", &m_gate->m_affCoe.rotateX, &m_gate->m_affCoe.rotateY, &m_gate->m_affCoe.rotateZ);
	getline(*file, str);
	sscanf(str.c_str(), "SCALE %f, %f, %f", &m_gate->m_affCoe.scaleX, &m_gate->m_affCoe.scaleY, &m_gate->m_affCoe.scaleZ);
}

void SceneManager::ReadWaterCaustic(ifstream *file)
{
	GLuint texBase;
	GLuint texDisp;
	GLuint modelIndex;
	GLuint shaderIndex;

	std::string str;

	getline(*file, str);
	getline(*file, str);
	getline(*file, str);
	sscanf(str.c_str(), "MODEL %d", &modelIndex);
	getline(*file, str);
	sscanf(str.c_str(), "TEXTURE %d %d", &texBase, &texDisp);
	getline(*file, str);
	sscanf(str.c_str(), "SHADER %d", &shaderIndex);

	//if (m_waterCaustic == NULL)
		//m_waterCaustic = new WaterCaustic();

	m_waterCaustic.Init(modelIndex, texBase, texDisp, shaderIndex);
	m_waterCaustic.m_drawable = true;

	getline(*file, str);
	sscanf(str.c_str(), "POSITION %f, %f, %f", &m_waterCaustic.m_affCoe.translateX, &m_waterCaustic.m_affCoe.translateY, &m_waterCaustic.m_affCoe.translateZ);
	getline(*file, str);
	sscanf(str.c_str(), "ROTATION %f, %f, %f", &m_waterCaustic.m_affCoe.rotateX, &m_waterCaustic.m_affCoe.rotateY, &m_waterCaustic.m_affCoe.rotateZ);
	getline(*file, str);
	sscanf(str.c_str(), "SCALE %f, %f, %f", &m_waterCaustic.m_affCoe.scaleX, &m_waterCaustic.m_affCoe.scaleY, &m_waterCaustic.m_affCoe.scaleZ);
}

bool SceneManager::ReadScene()
{
	std::ifstream myFile(ResourceManager::GetInstance()->m_scenes.at(m_stageID)->c_str());

	if (!myFile.good())
		return false;

	// Read Objects
	ReadObjectInfo(&myFile, PLAYER);
	ReadObjectInfo(&myFile, FISH);
	ReadObjectInfo(&myFile, SHARK);
	ReadObjectInfo(&myFile, ITEM);

	// Read Game Space (Camera + perspective)
	ReadGameSpace(&myFile);

	// Read Map
	ReadMapInfo(&myFile);

	// Read MiniMap
	ReadMiniMap(&myFile);
	MiniMap::GetInstance()->m_map->Update();//temp
	MiniMap::GetInstance()->m_border->Update();

	// Read Skydome
	ReadSkyDome(&myFile);
	m_listMapObj.at(1)->m_drawable = false;
	m_listMapObj.back()->m_drawable = true; // Temporary not draw

	// Read Buttons
	ReadButtonsInfo(&myFile);

	// Read Submenu
	ReadSubMenu(&myFile);

	// Analog Stick
	ReadControl(&myFile);

	// Read Goal position
	ReadGameConfig(&myFile);

	// Read Win lose
	ReadWinLoseScene(&myFile);

	// Read Lock and Key
	ReadGate(&myFile);

	//read water caustic
	ReadWaterCaustic(&myFile);

	// GenMap
	m_map->GenMap(m_listMapObj.at(m_currentMap * 2), m_listMapObj.at(m_currentMap * 2 + 1));
	m_map->m_obstructors[(int)(Globals::goalPos.x * m_map->m_size + Globals::goalPos.y)]->m_pos = ResourceManager::GetInstance()->m_objPos.at(5);
	m_map->m_obstructors[(int)(Globals::goalPos.x * m_map->m_size + Globals::goalPos.y)]->m_texturePos = ResourceManager::GetInstance()->m_texID.at(49);
	m_map->m_obstructors[(int)(Globals::goalPos.x * m_map->m_size + Globals::goalPos.y)]->m_drawable = true;

	myFile.close();

	return true;
}

// Draw Object
void SceneManager::ShowEndScene()
{
	if (Globals::isWinGame)
	{
		m_winLose.front()->m_drawable = true;

		if (m_winLose.front()->m_affCoe.translateY > 0.0f)
		{
			m_winLose.front()->m_affCoe.translateY -= 0.1;
		}
	}
	else
	{
		m_winLose.back()->m_drawable = true;

		if (m_winLose.back()->m_affCoe.translateY > 0.0f)
		{
			m_winLose.back()->m_affCoe.translateY -= 0.1;
		}
	}
}

void SceneManager::DrawStaticObjects()
{
	m_player->Draw();

	for (unsigned int i = 0; i < m_listShark.size(); i++)
	{
		m_listShark.at(i)->Draw();
	}
	for (unsigned int i = 0; i < m_listFish.size(); i++)
	{
		m_listFish.at(i)->Draw();
	}
	for (unsigned int i = 0; i < m_listItem.size(); i++)
	{
		m_listItem.at(i)->Draw();
	}

	m_gate->Draw();
	m_map->Draw();
	m_listMapObj.back()->Draw(); // Skydome

	m_waterCaustic.Draw();
}

void SceneManager::DrawDynamicObjects()
{
	for (unsigned int i = 0; i < m_lstButton.size(); i++)
	{
		m_lstButton.at(i)->Draw();
	}

	if (!m_isActive)
	{
		for (unsigned int i = 0; i < m_lstSubMenuButton.size(); i++)
		{
			m_lstSubMenuButton.at(i)->Draw();
		}

		m_menuScroll->Draw();
	}

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	for (unsigned int i = 0; i < m_analogStick.size(); i++)
	{
		m_analogStick.at(i)->Draw();
	}

	for (unsigned int i = 0; i < m_winLose.size(); i++)
	{
		m_winLose.at(i)->Draw();
	}

	glDisable(GL_BLEND);
}

// Update
void SceneManager::ChangeStage()
{
	m_stageID++;
	ResetData();
	Init();
	Resume();
	Save();
}

void SceneManager::ProcessRayCast()
{
	if (m_isMouseDown)
	{
		Vector3 ray_start;
		CreateRay(m_mousePos.x, m_mousePos.y, &ray_start);

		if (m_chosenButton == NULL)
		{
			if (m_isActive && !Globals::isEndGame)
				m_chosenButton = CheckCollision(ray_start, &m_lstButton);
			else
				m_chosenButton = CheckCollision(ray_start, &m_lstSubMenuButton);

			if (m_chosenButton)
			{
				if (m_chosenButton->GetButtonType() == DROP_ITEM && m_player->m_item);
				else
					m_chosenButton->m_texturePos = m_chosenButton->m_clickedTexPos;
			}
		}
		else
		{
			if (!CheckInBound(ray_start, m_chosenButton))
			{
				if (m_chosenButton->GetButtonType() == DROP_ITEM && m_player->m_item);
				else
					m_chosenButton->m_texturePos = m_chosenButton->m_UnClickedTexPos;
				m_chosenButton->m_isHovered = false;
			}
			else
			{
				if (m_chosenButton->GetButtonType() == DROP_ITEM && m_player->m_item);
				else
					m_chosenButton->m_texturePos = m_chosenButton->m_clickedTexPos;
				m_chosenButton->m_isHovered = true;
			}
		}
	}
	else
	{
		if (m_chosenButton)
		{
			if (m_chosenButton->GetButtonType() == DROP_ITEM && m_player->m_item);
			else
				m_chosenButton->m_texturePos = m_chosenButton->m_UnClickedTexPos;
		}
		m_chosenButton = NULL;
	}
}

void SceneManager::ProcessMenu()
{
	if (m_menuScroll->m_affCoe.translateY - 0.3 > -0.25)
	{
		m_menuScroll->m_affCoe.translateY -= 0.08;

		for (vector<Button*>::iterator i = m_lstSubMenuButton.begin(); i != m_lstSubMenuButton.end(); ++i)
			(*i)->m_affCoe.translateY -= 0.08;
	}
}

void SceneManager::ProcessController()
{
	if (m_chosenButton)
		return;

	Vector3 mousePos;
	CreateRay(m_mousePos.x, m_mousePos.y, &mousePos);
	if (m_isMouseDown)
	{
		if (m_analogStick.at(0)->m_drawable == false)
		{
			// Main circle
			m_analogStick.at(0)->m_affCoe.translateX = mousePos.x;
			m_analogStick.at(0)->m_affCoe.translateY = mousePos.y;
			m_analogStick.at(0)->m_drawable = true;

			// Direction circle
			m_analogStick.at(1)->m_affCoe.translateX = mousePos.x;
			m_analogStick.at(1)->m_affCoe.translateY = mousePos.y;
			m_analogStick.at(1)->m_drawable = true;
		}
		else if (m_dMouseAngle)
		{
			// Direction circle
			m_analogStick.at(1)->m_affCoe.translateX = mousePos.x + 0.115 * sin(DEG2RAD(180.0 - m_dMouseAngle));
			m_analogStick.at(1)->m_affCoe.translateY = mousePos.y + 0.16 * cos(DEG2RAD(180.0 - m_dMouseAngle));
		}
	}
	else
	{
		m_analogStick.at(0)->m_drawable = false;
		m_analogStick.at(1)->m_drawable = false;
	}
}

void SceneManager::ProcessAI()
{
	if (clock() - m_startTime >= 10)
	{
		ProcessFish();
		ProcessShark();

		m_startTime = clock();
	}
}

void SceneManager::ProcessFish()
{
	if (m_player->HasHostage())
		m_player->GetHostage()->Move(m_player->m_affCoe.translateX, m_player->m_affCoe.translateZ);
}

void SceneManager::ProcessShark()
{
	for (vector<Object*>::iterator i = SceneManager::GetInstance()->m_listShark.begin(); i != SceneManager::GetInstance()->m_listShark.end(); ++i)
	{
		((Shark*)(*i))->Process();
	}
}

void SceneManager::ProcessPlayer()
{
	// If player move
	if (m_isEvent == 0 || m_isMouseDown)
	{
		if (m_chosenButton == NULL)
			m_player->Process();
		m_isEvent++;
	}
}

void SceneManager::UpdateAllObjects()
{
	m_player->Update();
	m_player->m_maModel = m_player->m_maModel * Camera::getInstance()->m_CameraView * Camera::getInstance()->m_Projection;

	for (unsigned int i = 0; i < m_listShark.size(); i++)
	{
		m_listShark.at(i)->Update();
		m_listShark.at(i)->m_maModel = m_listShark.at(i)->m_maModel * Camera::getInstance()->m_CameraView * Camera::getInstance()->m_Projection;
	}

	for (unsigned int i = 0; i < m_listFish.size(); i++)
	{
		m_listFish.at(i)->Update();
		m_listFish.at(i)->m_maModel = m_listFish.at(i)->m_maModel * Camera::getInstance()->m_CameraView * Camera::getInstance()->m_Projection;
	}

	for (unsigned int i = 0; i < m_listItem.size(); i++)
	{
		m_listItem.at(i)->Update();
		m_listItem.at(i)->m_maModel = m_listItem.at(i)->m_maModel * Camera::getInstance()->m_CameraView * Camera::getInstance()->m_Projection;
	}

	for (unsigned int i = 0; i < m_lstButton.size(); i++)
	{
		m_lstButton.at(i)->Update();
	}

	for (unsigned int i = 0; i < m_winLose.size(); i++)
	{
		m_winLose.at(i)->Update();
	}

	m_gate->Update();
	m_gate->m_maModel = m_gate->m_maModel * Camera::getInstance()->m_CameraView * Camera::getInstance()->m_Projection;

	m_map->Update();

	if (!m_isActive)
	{
		for (unsigned int i = 0; i < m_lstSubMenuButton.size(); i++)
		{
			m_lstSubMenuButton.at(i)->Update();
		}

		m_menuScroll->Update();
	}

	if (m_isMouseDown && m_chosenButton == NULL)
	{
		for (unsigned int i = 0; i < m_analogStick.size(); i++)
		{
			m_analogStick.at(i)->Update();
		}
	}

	// Skydome
	m_listMapObj.back()->Update();
	m_listMapObj.back()->m_maModel = m_listMapObj.back()->m_maModel * Camera::getInstance()->m_CameraView * Camera::getInstance()->m_Projection;

	m_waterCaustic.Update();
}

// Save
void SceneManager::Save()
{
	string line;
	GLuint stageCount;
	ifstream myFile(ResourceManager::GetInstance()->m_save->c_str());

	getline(myFile, line);
	sscanf(line.c_str(), "%d", &stageCount);

	if (stageCount <= (int)(m_stageID))
	{
		ofstream myFile(ResourceManager::GetInstance()->m_save->c_str());
		myFile.clear();
		myFile << (int)(m_stageID + 1);
	}

	myFile.close();
}