#ifndef __SCENEMANAGER_H__
#define __SCENEMANAGER_H__

#include "State.h"
#include "Player.h"
#include "Button.h"
#include "Map.h"
#include "MiniMap.h"
#include "WaterCaustic.h"

#include <vector>
using namespace std;

class SceneManager : public State
{
public:
	static SceneManager* GetInstance();
	~SceneManager();
	void LMouseDown(int mouse_x, int mouse_y);
	void LMouseUp(int mouse_x, int mouse_y);
	void MouseMove(int mouse_x, int mouse_y);
	bool Init();
	void Update();
	void Draw();
	void Destroy();
	void ResetData();
	void ChangeStage();
	void EndGame(bool isWin);
	void SetStage(GLubyte stageID);

public:
	GLubyte m_stageID;
	Player* m_player;
	Object* m_menuScroll;
	Map* m_map;//list map
	MiniMap* m_miniMap; //mini map object
	GLuint m_currentMap; //pointer to current map
	Object* m_gate;
	vector<Object*> m_listShark;//list object shark
	vector<Object*> m_listFish;//list object little fish
	vector<Object*> m_listItem;//list item
	vector<Object*> m_listMapObj;
	vector<Object*> m_analogStick;
	vector<Object*> m_winLose;
	vector<Button*> m_lstButton;
	vector<Button*> m_lstSubMenuButton;
	vector<GLuint> m_listItemTexture;

	WaterCaustic m_waterCaustic;

private:
	SceneManager();

	// Init
	void InitGL();
	void InitGame();

	// Read
	void ReadObjectInfo(ifstream *file, Type type);
	void ReadMapInfo(ifstream *file);
	void ReadSkyDome(ifstream *file);
	void ReadMiniMap(ifstream *file);
	void ReadGameSpace(ifstream *file);
	void ReadSubMenu(ifstream *file);
	void ReadButtonsInfo(ifstream* file);
	void ReadControl(ifstream *file);
	void ReadGameConfig(ifstream *file);
	void ReadWinLoseScene(ifstream *file);
	void ReadGate(ifstream *file);
	void ReadWaterCaustic(ifstream *file);
	bool ReadScene();

	// Update
	void ProcessRayCast();
	void ProcessAI();
	void ProcessFish();
	void ProcessShark();
	void ProcessMenu();
	void ProcessController();
	void ProcessPlayer();
	void UpdateAllObjects();

	// Draw
	void ShowEndScene();
	void DrawStaticObjects();
	void DrawDynamicObjects();

	// Save
	void Save();

private:
	bool m_isMouseDown;
	bool m_isEvent;
	bool m_isEndSceneOn;
	bool m_isFBO;
	unsigned long m_startTime;
	GLfloat m_dMouseAngle;
	Button* m_chosenButton;
	static SceneManager* m_Instance;
	Vector2 m_mousePos;
};

#endif

