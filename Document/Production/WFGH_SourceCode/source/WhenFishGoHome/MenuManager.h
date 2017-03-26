#ifndef __MENUMANAGER_H__
#define __MENUMANAGER_H__

#include "State.h"
#include "Button.h"
#include "WaterCaustic.h"

#include <vector>
using namespace std;

class MenuManager : public State
{
public:
	static MenuManager* GetInstance();
	~MenuManager();
	void Update();
	void Draw();
	void ResetData();

	void LMouseDown(int mouse_x, int mouse_y);
	void LMouseUp(int mouse_x, int mouse_y);
	void MouseMove(int mouse_x, int mouse_y);

	void DrawMainMenu();
	void DrawLevelMenu();
	void ShowCredit();
	void ShowTutorial();

public:
	Object* m_background;
	Object* m_levelBackground;
	Object* m_credit;
	Object* m_tutorial;
	Button* m_return;
	vector<Button*> m_lstButton;
	vector<Button*> m_lstLevelButton;

	WaterCaustic m_waterCaustic;

private:
	MenuManager();
	bool Init();
	bool ReadMenu();
	bool ReadLevelMenu();
	void ReadBackgroundInfo(ifstream *file, Object*& background);
	void ReadButtonsInfo(ifstream *file);
	void ReadLevelButtonInfo(ifstream *file);
	void ReadWaterCaustic(ifstream* file);
	void ReadReturnButton(ifstream *file);
	void ReadCredit(ifstream *file);
	void ReadTutorial(ifstream *file);
	void ReadSaveFile();
	void ProcessRayCast();
	void UpdateBackGround();
	void UpdateAllButtons();

private:
	bool m_isMouseDown;
	bool m_isDrawMainMenu;
	SCENE m_displayScene;
	int m_mouseX;
	int m_mouseY;
	Button* m_chosenButton;
	static MenuManager* m_instance;
};


#endif