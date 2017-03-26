#ifndef __SHARK_H__
#define __SHARK_H__

#include "Object.h"

#include <vector>
using namespace std;

class Shark : public Object
{
public:
	Shark();
	~Shark();
	void ResetShark();
	void Process();

	Type GetType();
	STATE GetState();
	vector<Position*>* GetSharkPath();

	// Item's effect
	void SlowDown();
	void Paralize(GLushort time);

public:
	GLfloat m_fSharkSpeed;

private:
	void ComputeSharkPath();
	void ClearSharkPath();
	void InitStateTime();
	void Move();
	void Pursue();
	void NormalizeSpeed();
	void ResetState();
	bool InStateDuration();
	bool DetectPrey();

private:
	bool m_hasPath;
	GLushort m_stateDuration;
	unsigned long m_stateStartTime;
	STATE m_state;
	vector<Position*>* m_vSharkPath;
	Object* m_pursuingTarget;
	GLint m_iDetectionSquare;
};

#endif