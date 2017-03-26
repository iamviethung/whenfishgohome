#include "stdafx.h"
#include "MenuManager.h"
#include "ResourceManager.h"
#include "Processor.h"
#include <fstream>

MenuManager* MenuManager::m_instance = NULL;

MenuManager::MenuManager() : m_isMouseDown(false),
m_isDrawMainMenu(true),
m_chosenButton(NULL),
m_background(NULL),
m_levelBackground(NULL),
m_credit(NULL),
m_return(NULL),
m_tutorial(NULL),
m_displayScene(MAINMENU)
{
}

MenuManager* MenuManager::GetInstance()
{
	if (m_instance == NULL)
		m_instance = new MenuManager();

	return m_instance;
}

void MenuManager::ReadBackgroundInfo(ifstream* file, Object*& background)
{
	GLuint objID;
	GLuint texIndex;
	GLuint modelIndex;
	GLuint shaderIndex;

	std::string str;

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

	if (background == NULL)
		background = new Object();
	background->m_id = objID;
	background->Init(modelIndex, texIndex, shaderIndex);
	background->m_drawable = true;

	getline(*file, str);
	sscanf(str.c_str(), "POSITION %f, %f, %f", &background->m_affCoe.translateX, &background->m_affCoe.translateY, &background->m_affCoe.translateZ);
	getline(*file, str);
	sscanf(str.c_str(), "ROTATION %f, %f, %f", &background->m_affCoe.rotateX, &background->m_affCoe.rotateY, &background->m_affCoe.rotateZ);
	getline(*file, str);
	sscanf(str.c_str(), "SCALE %f, %f, %f", &background->m_affCoe.scaleX, &background->m_affCoe.scaleY, &background->m_affCoe.scaleZ);
}

void MenuManager::ReadButtonsInfo(ifstream* file)
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

	for (unsigned int i = 0; i < buttonCount; i++)
	{
		getline(*file, str);
		sscanf(str.c_str(), "ID %d", &objID);
		getline(*file, str);
		sscanf(str.c_str(), "MODEL %d", &modelIndex);
		getline(*file, str);
		sscanf(str.c_str(), "TEXTURE %d %d", &unClickedTexIndex, &clickedTexIndex);
		getline(*file, str);
		sscanf(str.c_str(), "SHADER %d", &shaderIndex);
		getline(*file, str);
		sscanf(str.c_str(), "TYPE %s", buttonType);

		if (m_lstButton.size() < buttonCount)
			m_lstButton.push_back(new Button());
		m_lstButton.at(i)->m_id = objID;
		m_lstButton.at(i)->Init(modelIndex, unClickedTexIndex, shaderIndex);
		m_lstButton.at(i)->m_UnClickedTexPos = ResourceManager::GetInstance()->m_texID.at(unClickedTexIndex);
		m_lstButton.at(i)->m_clickedTexPos = ResourceManager::GetInstance()->m_texID.at(clickedTexIndex);
		m_lstButton.at(i)->m_drawable = true;

		if (strcmp(buttonType, "START") == 0)
			m_lstButton.at(i)->SetButtonType(START);
		else if (strcmp(buttonType, "RESUME") == 0)
			m_lstButton.at(i)->SetButtonType(RESUME);
		else if (strcmp(buttonType, "TUTORIAL") == 0)
			m_lstButton.at(i)->SetButtonType(TUTORIAL);
		else if (strcmp(buttonType, "OPTIONS") == 0)
			m_lstButton.at(i)->SetButtonType(OPTIONS);
		else if (strcmp(buttonType, "CREDITS") == 0)
			m_lstButton.at(i)->SetButtonType(CREDITS);
		else if (strcmp(buttonType, "EXIT") == 0)
			m_lstButton.at(i)->SetButtonType(EXIT);
		else
			m_lstButton.at(i)->SetButtonType(LEVEL);

		getline(*file, str);
		sscanf(str.c_str(), "POSITION %f, %f, %f", &m_lstButton.at(i)->m_affCoe.translateX, &m_lstButton.at(i)->m_affCoe.translateY, &m_lstButton.at(i)->m_affCoe.translateZ);
		getline(*file, str);
		sscanf(str.c_str(), "ROTATION %f, %f, %f", &m_lstButton.at(i)->m_affCoe.rotateX, &m_lstButton.at(i)->m_affCoe.rotateY, &m_lstButton.at(i)->m_affCoe.rotateZ);
		getline(*file, str);
		sscanf(str.c_str(), "SCALE %f, %f, %f", &m_lstButton.at(i)->m_affCoe.scaleX, &m_lstButton.at(i)->m_affCoe.scaleY, &m_lstButton.at(i)->m_affCoe.scaleZ);
	}
}

void MenuManager::ReadLevelButtonInfo(ifstream *file)
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

	for (unsigned int i = 0; i < buttonCount; i++)
	{
		getline(*file, str);
		sscanf(str.c_str(), "ID %d", &objID);
		getline(*file, str);
		sscanf(str.c_str(), "MODEL %d", &modelIndex);
		getline(*file, str);
		sscanf(str.c_str(), "TEXTURE %d %d", &unClickedTexIndex, &clickedTexIndex);
		getline(*file, str);
		sscanf(str.c_str(), "SHADER %d", &shaderIndex);
		getline(*file, str);
		sscanf(str.c_str(), "TYPE %s", buttonType);

		if (m_lstLevelButton.size() < buttonCount)
			m_lstLevelButton.push_back(new Button());

		m_lstLevelButton.at(i)->m_id = objID;
		m_lstLevelButton.at(i)->Init(modelIndex, unClickedTexIndex, shaderIndex);
		m_lstLevelButton.at(i)->m_UnClickedTexPos = ResourceManager::GetInstance()->m_texID.at(unClickedTexIndex);
		m_lstLevelButton.at(i)->m_clickedTexPos = ResourceManager::GetInstance()->m_texID.at(clickedTexIndex);
		m_lstLevelButton.at(i)->m_drawable = true;
		m_lstLevelButton.at(i)->SetButtonType(LEVEL);

		getline(*file, str);
		sscanf(str.c_str(), "STATE %s", buttonType);

		if (strcmp(buttonType, "LOCKED") == 0)
			m_lstLevelButton.at(i)->m_levelState = LOCKED;
		else if (strcmp(buttonType, "UNLOCKED") == 0)
			m_lstLevelButton.at(i)->m_levelState = UNLOCKED;
		else
			m_lstLevelButton.at(i)->m_levelState = UNOPEN;

		getline(*file, str);
		sscanf(str.c_str(), "POSITION %f, %f, %f", &m_lstLevelButton.at(i)->m_affCoe.translateX, &m_lstLevelButton.at(i)->m_affCoe.translateY, &m_lstLevelButton.at(i)->m_affCoe.translateZ);
		getline(*file, str);
		sscanf(str.c_str(), "ROTATION %f, %f, %f", &m_lstLevelButton.at(i)->m_affCoe.rotateX, &m_lstLevelButton.at(i)->m_affCoe.rotateY, &m_lstLevelButton.at(i)->m_affCoe.rotateZ);
		getline(*file, str);
		sscanf(str.c_str(), "SCALE %f, %f, %f", &m_lstLevelButton.at(i)->m_affCoe.scaleX, &m_lstLevelButton.at(i)->m_affCoe.scaleY, &m_lstLevelButton.at(i)->m_affCoe.scaleZ);
	}
}

void MenuManager::ReadReturnButton(ifstream *file)
{
	char buttonType[20];
	GLuint objID;
	GLuint unClickedTexIndex;
	GLuint clickedTexIndex;
	GLuint modelIndex;
	GLuint shaderIndex;
	std::string str;

	getline(*file, str);
	sscanf(str.c_str(), "ID %d", &objID);
	getline(*file, str);
	sscanf(str.c_str(), "MODEL %d", &modelIndex);
	getline(*file, str);
	sscanf(str.c_str(), "TEXTURE %d %d", &unClickedTexIndex, &clickedTexIndex);
	getline(*file, str);
	sscanf(str.c_str(), "SHADER %d", &shaderIndex);
	getline(*file, str);
	sscanf(str.c_str(), "TYPE %s", buttonType);

	if (m_return == NULL)
		m_return = new Button();
	m_return->m_id = objID;
	m_return->Init(modelIndex, unClickedTexIndex, shaderIndex);
	m_return->m_UnClickedTexPos = ResourceManager::GetInstance()->m_texID.at(unClickedTexIndex);
	m_return->m_clickedTexPos = ResourceManager::GetInstance()->m_texID.at(clickedTexIndex);
	m_return->m_drawable = true;
	m_return->SetButtonType(RETURN);

	getline(*file, str);
	sscanf(str.c_str(), "POSITION %f, %f, %f", &m_return->m_affCoe.translateX, &m_return->m_affCoe.translateY, &m_return->m_affCoe.translateZ);
	getline(*file, str);
	sscanf(str.c_str(), "ROTATION %f, %f, %f", &m_return->m_affCoe.rotateX, &m_return->m_affCoe.rotateY, &m_return->m_affCoe.rotateZ);
	getline(*file, str);
	sscanf(str.c_str(), "SCALE %f, %f, %f", &m_return->m_affCoe.scaleX, &m_return->m_affCoe.scaleY, &m_return->m_affCoe.scaleZ);
}

void MenuManager::ReadWaterCaustic(ifstream* file)
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

void MenuManager::ReadCredit(ifstream *file)
{
	char buttonType[20];
	GLuint objID;
	GLuint texIndex;
	GLuint clickedTexIndex;
	GLuint modelIndex;
	GLuint shaderIndex;
	std::string str;

	getline(*file, str);
	getline(*file, str);
	sscanf(str.c_str(), "ID %d", &objID);
	getline(*file, str);
	sscanf(str.c_str(), "MODEL %d", &modelIndex);
	getline(*file, str);
	sscanf(str.c_str(), "TEXTURE %d", &texIndex);
	getline(*file, str);
	sscanf(str.c_str(), "SHADER %d", &shaderIndex);

	if (m_credit == NULL)
		m_credit = new Object();
	m_credit->m_id = objID;
	m_credit->Init(modelIndex, texIndex, shaderIndex);
	m_credit->m_drawable = true;

	getline(*file, str);
	sscanf(str.c_str(), "POSITION %f, %f, %f", &m_credit->m_affCoe.translateX, &m_credit->m_affCoe.translateY, &m_credit->m_affCoe.translateZ);
	getline(*file, str);
	sscanf(str.c_str(), "ROTATION %f, %f, %f", &m_credit->m_affCoe.rotateX, &m_credit->m_affCoe.rotateY, &m_credit->m_affCoe.rotateZ);
	getline(*file, str);
	sscanf(str.c_str(), "SCALE %f, %f, %f", &m_credit->m_affCoe.scaleX, &m_credit->m_affCoe.scaleY, &m_credit->m_affCoe.scaleZ);
}

void MenuManager::ReadTutorial(ifstream *file)
{
	char buttonType[20];
	GLuint objID;
	GLuint texIndex;
	GLuint clickedTexIndex;
	GLuint modelIndex;
	GLuint shaderIndex;
	std::string str;

	getline(*file, str);
	getline(*file, str);
	sscanf(str.c_str(), "ID %d", &objID);
	getline(*file, str);
	sscanf(str.c_str(), "MODEL %d", &modelIndex);
	getline(*file, str);
	sscanf(str.c_str(), "TEXTURE %d", &texIndex);
	getline(*file, str);
	sscanf(str.c_str(), "SHADER %d", &shaderIndex);

	if (m_tutorial == NULL)
		m_tutorial = new Object();
	m_tutorial->m_id = objID;
	m_tutorial->Init(modelIndex, texIndex, shaderIndex);
	m_tutorial->m_drawable = true;

	getline(*file, str);
	sscanf(str.c_str(), "POSITION %f, %f, %f", &m_tutorial->m_affCoe.translateX, &m_tutorial->m_affCoe.translateY, &m_tutorial->m_affCoe.translateZ);
	getline(*file, str);
	sscanf(str.c_str(), "ROTATION %f, %f, %f", &m_tutorial->m_affCoe.rotateX, &m_tutorial->m_affCoe.rotateY, &m_tutorial->m_affCoe.rotateZ);
	getline(*file, str);
	sscanf(str.c_str(), "SCALE %f, %f, %f", &m_tutorial->m_affCoe.scaleX, &m_tutorial->m_affCoe.scaleY, &m_tutorial->m_affCoe.scaleZ);
}

void MenuManager::ReadSaveFile()
{
	ifstream myFile(ResourceManager::GetInstance()->m_save->c_str());

	if (!myFile.good())
	{
		ofstream file(ResourceManager::GetInstance()->m_save->c_str());
		file << 1;
		file.close();
	}
	else
	{
		string line;
		int stageCount;

		getline(myFile, line);
		sscanf(line.c_str(), "%d", &stageCount);

		for (int i = 1; i < stageCount; i++)
		{
			m_lstLevelButton.at(i)->m_texturePos = m_lstLevelButton.at(i)->m_clickedTexPos;
			m_lstLevelButton.at(i)->m_levelState = UNLOCKED;
		}
	}

	myFile.close();
}

bool MenuManager::ReadMenu()
{
	std::ifstream myFile(ResourceManager::GetInstance()->m_menu->c_str());

	if (!myFile.good())
		return false;

	// Read Main Menu
	ReadBackgroundInfo(&myFile, m_background);
	ReadButtonsInfo(&myFile);

	//Read Level Menu
	ReadBackgroundInfo(&myFile, m_levelBackground);
	ReadLevelButtonInfo(&myFile);
	ReadReturnButton(&myFile);
	ReadCredit(&myFile);
	ReadTutorial(&myFile);
	ReadSaveFile();

	ReadWaterCaustic(&myFile);

	myFile.close();
	return true;
}

bool MenuManager::Init()
{
	//xoa bo dem mau va do sau
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClearDepthf(1.0f);

	// Enable depth buffer
	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_BACK);

	bool success = ReadMenu();

	return success;
}

void MenuManager::DrawMainMenu()
{
	m_displayScene = MAINMENU;
}

void MenuManager::DrawLevelMenu()
{
	m_displayScene = LEVEL_CHOOSING;
}

void MenuManager::ShowCredit()
{
	m_displayScene = CREDIT;
}

void MenuManager::ShowTutorial()
{
	m_displayScene = TUTORIALS;
}

void MenuManager::Draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	switch (m_displayScene)
	{
	case MAINMENU:
	{
		m_background->Draw();

		for (vector<Button*>::iterator i = m_lstButton.begin(); i != m_lstButton.end(); ++i)
		{
			(*i)->Draw();
		}
	}
	break;

	case LEVEL_CHOOSING:
	{
		m_levelBackground->Draw();

		for (vector<Button*>::iterator i = m_lstLevelButton.begin(); i != m_lstLevelButton.end(); ++i)
		{
			(*i)->Draw();
		}

		m_return->Draw();
	}
	break;

	case CREDIT:
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		m_levelBackground->Draw();
		m_credit->Draw();
		m_return->Draw();
		glDisable(GL_BLEND);
	}
	break;

	case TUTORIAL:
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		m_levelBackground->Draw();
		m_tutorial->Draw();
		m_return->Draw();
		glDisable(GL_BLEND);
	}
	break;
	}

	m_waterCaustic.Draw();
}

void MenuManager::Update()
{
	//if (!m_isActive)
	//return;

	ProcessRayCast();
	UpdateBackGround();
	UpdateAllButtons();

	m_waterCaustic.Update();
}

void MenuManager::ProcessRayCast()
{
	if (m_isMouseDown)
	{
		Vector3 ray_start, ray_end;
		CreateRay(m_mouseX, m_mouseY, &ray_start, &ray_end);

		if (m_chosenButton == NULL)
		{
			switch (m_displayScene)
			{
			case MAINMENU:
			{
				m_chosenButton = CheckCollision(ray_start, &m_lstButton);
			}
			break;

			case LEVEL_CHOOSING:
			{
				m_chosenButton = CheckCollision(ray_start, &m_lstLevelButton);

				if (m_chosenButton == NULL)
					if (CheckCollision(ray_start, m_return))
						m_chosenButton = m_return;
			}
			break;

			case CREDIT: case TUTORIAL:
			{
				if (CheckCollision(ray_start, m_return))
					m_chosenButton = m_return;
			}
			break;
			}

			if (m_chosenButton && m_chosenButton->GetButtonType() != LEVEL)
				m_chosenButton->m_texturePos = m_chosenButton->m_clickedTexPos;
		}
		else
		{
			if (!CheckInBound(ray_start, m_chosenButton))
			{
				m_chosenButton->m_texturePos = m_chosenButton->m_UnClickedTexPos;
				m_chosenButton->m_isHovered = false;
			}
			else
			{
				if (m_chosenButton && m_chosenButton->GetButtonType() != LEVEL)
					m_chosenButton->m_texturePos = m_chosenButton->m_clickedTexPos;
				m_chosenButton->m_isHovered = true;
			}
		}
	}
	else
	{
		if (m_chosenButton)
			m_chosenButton->m_texturePos = m_chosenButton->m_UnClickedTexPos;
		m_chosenButton = NULL;
	}
}

void MenuManager::LMouseDown(int mouse_x, int mouse_y)
{
	m_isMouseDown = true;
	m_mouseX = mouse_x;
	m_mouseY = mouse_y;
}

void MenuManager::LMouseUp(int mouse_x, int mouse_y)
{
	if (m_chosenButton && m_chosenButton->m_isHovered)
	{
		m_chosenButton->DoClick();
	}
	m_isMouseDown = false;
}

void MenuManager::MouseMove(int mouse_x, int mouse_y)
{
	if (m_isMouseDown)
	{
		m_mouseX = mouse_x;
		m_mouseY = mouse_y;
	}
}

void MenuManager::UpdateBackGround()
{
	switch (m_displayScene)
	{
	case MAINMENU:
	{
		m_background->Update();
	}
	break;

	case LEVEL_CHOOSING:
	{
		m_levelBackground->Update();
	}
	break;

	case CREDIT:
	{
		m_levelBackground->Update();
		m_credit->Update();
	}
	break;

	case TUTORIAL:
	{
		m_levelBackground->Update();
		m_tutorial->Update();
	}
	break;
	}
}

void MenuManager::UpdateAllButtons()
{
	switch (m_displayScene)
	{
	case MAINMENU:
	{
		for (vector<Button*>::iterator i = m_lstButton.begin(); i != m_lstButton.end(); ++i)
		{
			(*i)->Update();
		}
	}
	break;

	case LEVEL_CHOOSING:
	{
		for (vector<Button*>::iterator i = m_lstLevelButton.begin(); i != m_lstLevelButton.end(); ++i)
		{
			(*i)->Update();
		}

		m_return->Update();
	}
	break;

	case CREDIT: case TUTORIAL:
	{
		m_return->Update();
	}
	break;
	}
}

void MenuManager::ResetData()
{
	if (m_instance != NULL)
	{
		m_isMouseDown = false;
		m_chosenButton = NULL;

		if (m_background)
			m_background->m_drawable = false;

		if (m_levelBackground)
			m_levelBackground->m_drawable = false;

		for (vector<Button*>::iterator i = m_lstButton.begin(); i != m_lstButton.end(); ++i)
		{
			(*i)->m_drawable = false;
		}

		for (vector<Button*>::iterator i = m_lstLevelButton.begin(); i != m_lstLevelButton.end(); ++i)
		{
			(*i)->m_drawable = false;
		}
	}
}

MenuManager::~MenuManager()
{
	if (m_instance)
	{
		if (m_background)
			delete m_background;
		m_background = NULL;

		if (m_levelBackground)
			delete m_levelBackground;
		m_levelBackground = NULL;

		if (m_return)
			delete m_return;
		m_return = NULL;

		if (m_credit)
			delete m_credit;
		m_credit = NULL;

		if (m_tutorial)
			delete m_tutorial;
		m_tutorial = NULL;

		for (vector<Button*>::iterator i = m_lstButton.begin(); i != m_lstButton.end();)
		{
			Button* temp = *i;
			i++;
			delete temp;
		}

		for (vector<Button*>::iterator i = m_lstLevelButton.begin(); i != m_lstLevelButton.end();)
		{
			Button* temp = *i;
			i++;
			delete temp;
		}

		m_lstButton.clear();
		m_lstLevelButton.clear();
		m_instance = NULL;
	}
}