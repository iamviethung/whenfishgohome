#ifndef __FISH_H__
#define __FISH_H__

#include "Struct.h"
#include "Object.h"

#include <vector>
using namespace std;

class Fish : public Object
{
public:
	Fish();
	~Fish();
	Type GetType();
	void ClearFishPath();
	void SetSpeed(GLfloat speed);
	void Move(float playerPosX, float playerPosY);

public:
	GLfloat m_fFishSpeed;
	vector<Position*>* m_vLittleFishPath;
};

#endif