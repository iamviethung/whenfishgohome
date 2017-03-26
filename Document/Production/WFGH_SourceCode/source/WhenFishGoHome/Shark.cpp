#include "stdafx.h"
#include "Shark.h"
#include "Processor.h"
#include "SceneManager.h"

#include <time.h>
#include <stdlib.h>

Shark::Shark() : m_vSharkPath(new vector<Position*>()), m_iDetectionSquare(3), m_state(NORMAL), m_pursuingTarget(NULL)
{}

void Shark::ResetShark()
{
	ClearSharkPath();
	//m_vSharkPath = NULL;
	m_vSharkPath = new vector<Position*>();
	m_state = NORMAL;
	m_pursuingTarget = NULL;
	m_drawable = false;
}

void Shark::ComputeSharkPath()
{
	srand((GLuint)time(NULL));
	int x, y;
	Map *map = SceneManager::GetInstance()->m_map;
	Vector2 pos = CoordinateToMap(this->m_affCoe.translateX, this->m_affCoe.translateZ);

	while (true)
	{
		x = -m_iDetectionSquare + (rand() % (2 * m_iDetectionSquare + 1)) + (GLint)pos.x;
		y = -m_iDetectionSquare + (rand() % (2 * m_iDetectionSquare + 1)) + (GLint)pos.y;

		if (x >= 0 && y >= 0 && x < map->m_size && y < map->m_size && map->m_data[x][y] == '0')
			break;
	}

	ClearSharkPath();
	m_vSharkPath = FindPath((GLint)pos.x, (GLint)pos.y, x, y);
	map = NULL;
}

//----------------------------------------------------------
/// \brief Clear shark path vector
//----------------------------------------------------------
void Shark::ClearSharkPath()
{
	if (!m_vSharkPath->empty())
	{
		for (GLuint i = 0; i < m_vSharkPath->size(); i++)
		{
			if (m_vSharkPath->at(i)) delete m_vSharkPath->at(i);
			m_vSharkPath->at(i) = NULL;
		}

		m_vSharkPath->clear();
	}

	delete m_vSharkPath;
	m_vSharkPath = NULL;
}

void Shark::InitStateTime()
{
	m_stateStartTime = 0;
}

void Shark::SlowDown()
{
	m_state = SLOW;
	m_fSharkSpeed /= 4;
	InitStateTime();
	m_stateDuration = 300;
}

void Shark::NormalizeSpeed()
{
	m_state = NORMAL;
	m_fSharkSpeed *= 4;
}

void Shark::ResetState()
{
	switch (m_state)
	{
	case SLOW:
	{
		NormalizeSpeed();
	}
	break;

	case PARALIZED:
	{
		m_state = NORMAL;
	}
	break;
	}
}

void Shark::Paralize(GLushort time)
{
	m_state = PARALIZED;
	InitStateTime();
	m_stateDuration = time;
}

void Shark::Move()
{
	if (m_vSharkPath->empty())
		return;

	Vector2 dest = MapToCoordinate(*m_vSharkPath->front());

	if (CoordinateToMap(dest.x, dest.y) == CoordinateToMap(m_affCoe.translateX, m_affCoe.translateZ))
	{
		delete m_vSharkPath->at(0);
		m_vSharkPath->erase(m_vSharkPath->begin());
		dest = MapToCoordinate(*m_vSharkPath->at(0));
	}

	// Set shark velocity along xz-coordinate
	float rotAngle = 90.0f - RAD2DEG(atan2(dest.y - m_affCoe.translateZ, dest.x - m_affCoe.translateX));
	float dx = m_fSharkSpeed * sin(DEG2RAD(rotAngle));
	float dz = m_fSharkSpeed * cos(DEG2RAD(rotAngle));

	// Set shark's movement
	m_affCoe.rotateY = rotAngle;
	m_affCoe.translateX += dx;
	m_affCoe.translateZ += dz;

	Object* object;
	if (IsHitObject(this, object))
	{
		switch (object->GetType())
		{
		case PLAYER: case FISH:
		{
			SceneManager::GetInstance()->EndGame(false);
		}
		break;

		case ITEM:
		{
			if (((Item*)object)->m_isBeingUsed)
			{
				ResetState();
				((Item*)(object))->DoEffect(this);
			}
		}
		break;

		}
	}
}

void Shark::Process()
{
	switch (m_state)
	{
	case SLOW:
	{
		if (!InStateDuration())
		{
			NormalizeSpeed();
		}

		if (DetectPrey())
		{
			Pursue();
		}
		else
		{
			if (m_vSharkPath->size() < 2)
				ComputeSharkPath();
		}

		Move();
	}
	break;

	case NORMAL:
	{
		if (DetectPrey())
		{
			Pursue();
			m_state = PURSUING;
		}
		else
		{
			if (m_vSharkPath->size() < 2)
				ComputeSharkPath();
		}

		Move();
	}
	break;

	case PURSUING:
	{
		if (GetSharkPath()->size() > 2)
		{
			Move();
		}
		else
		{
			if (!DetectPrey())
				m_state = NORMAL;
			else
			{
				Pursue();
				Move();
			}
		}
	}
	break;

	case PARALIZED:
	{
		if (!InStateDuration())
			m_state = NORMAL;
	}
	break;
	}
}

void Shark::Pursue()
{
	Vector2 playerPos = CoordinateToMap(m_pursuingTarget->m_affCoe.translateX, m_pursuingTarget->m_affCoe.translateZ);
	Vector2 pos = CoordinateToMap(m_affCoe.translateX, m_affCoe.translateZ);
	ClearSharkPath();
	m_vSharkPath = FindPath((GLint)pos.x, (GLint)pos.y, (GLint)playerPos.x, (GLint)playerPos.y);
}

bool Shark::InStateDuration()
{
	m_stateStartTime++;
	return m_stateStartTime < m_stateDuration;
}

bool Shark::DetectPrey()
{
	// Detect player
	Object* player = SceneManager::GetInstance()->m_player;

	if (abs(player->m_affCoe.translateX - this->m_affCoe.translateX) <= m_iDetectionSquare &&
		abs(player->m_affCoe.translateZ - this->m_affCoe.translateZ) <= m_iDetectionSquare)
	{
		m_pursuingTarget = player;
		return true;
	}

	if (SceneManager::GetInstance()->m_gate->m_drawable)
	{
		m_pursuingTarget = NULL;
		return false;
	}

	// Detect fish
	for (vector<Object*>::iterator i = SceneManager::GetInstance()->m_listFish.begin(); i != SceneManager::GetInstance()->m_listFish.end(); ++i)
	{
		if (abs((*i)->m_affCoe.translateX - this->m_affCoe.translateX) <= m_iDetectionSquare &&
			abs((*i)->m_affCoe.translateZ - this->m_affCoe.translateZ) <= m_iDetectionSquare)
		{
			m_pursuingTarget = *i;
			return true;
		}
	}

	m_pursuingTarget = NULL;
	return false;
}

Type Shark::GetType()
{
	return SHARK;
}

STATE Shark::GetState()
{
	return m_state;
}

vector<Position*>* Shark::GetSharkPath()
{
	return m_vSharkPath;
}

Shark::~Shark()
{
	ClearSharkPath();
	delete m_vSharkPath;
}