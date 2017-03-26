#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "Fish.h"
#include "Item.h"

class Player : public Object
{
public:
	Player();
	virtual ~Player();
	virtual Type GetType();
	Fish* GetHostage();
	void ResetPlayer();
	void DropItem();
	void Process();
	void SpeedUp();
	void Poison();
	bool HasHostage();

private:
	void MoveCamera(GLfloat xCamVel, GLfloat zCamVel);
	void Move();
	void InitStateTime();
	void ResetState();
	void NormalizeSpeed();
	bool InStateDuration();

public:
	bool m_hasKey;
	GLushort m_stateDuration;
	unsigned long m_stateStartTime;
	float m_xVel;
	float m_zVel;
	float m_fRotateAngle;
	Fish* m_hostage;
	Item* m_key;
	Item* m_item;
	STATE m_state;
	GLfloat m_fPlayerSpeed;
};

#endif