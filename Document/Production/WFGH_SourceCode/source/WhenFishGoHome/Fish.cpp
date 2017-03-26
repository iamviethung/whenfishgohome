#include "stdafx.h"
#include "Fish.h"
#include "Globals.h"
#include "Processor.h"
#include "SceneManager.h"

Fish::Fish() : m_vLittleFishPath(NULL)
{}

Type Fish::GetType()
{
	return FISH;
}

//----------------------------------------------------------
/// \brief Clear fish path vector
//----------------------------------------------------------
void Fish::ClearFishPath()
{
	if (m_vLittleFishPath)
	{
		for (unsigned int i = 0; i < m_vLittleFishPath->size(); i++)
		{
			if (m_vLittleFishPath->at(i)) delete m_vLittleFishPath->at(i);
			m_vLittleFishPath->at(i) = NULL;
		}

		delete m_vLittleFishPath;
		m_vLittleFishPath = NULL;
	}
}

void Fish::SetSpeed(GLfloat speed)
{
	m_fFishSpeed = speed;
}

void Fish::Move(float playerPosX, float playerPosY)
{
	// Get fish and player position
	Vector2 fishPos = CoordinateToMap(m_affCoe.translateX, m_affCoe.translateZ);
	Vector2 playerPos = CoordinateToMap(playerPosX, playerPosY);

	// Check if fish has reach goal
	if (fishPos == Globals::goalPos)
	{
		SceneManager::GetInstance()->EndGame(true);
	}

	// Reset fish path list and make anew
	ClearFishPath();
	m_vLittleFishPath = FindPath((GLint)fishPos.x, (GLint)fishPos.y, (GLint)playerPos.x, (GLint)playerPos.y);

	if (m_vLittleFishPath->size() == 0)
		return;

	// Distance from fish to nearest position on path list
	Vector2 dest = MapToCoordinate(*m_vLittleFishPath->front());
	float x = dest.x - m_affCoe.translateX;
	float z = dest.y - m_affCoe.translateZ;

	float rotAngle = 90.0f - RAD2DEG(atan2(z, x));
	// Set fish velocity along xz-coordinate
	float dx = m_fFishSpeed * sin(DEG2RAD(rotAngle));
	float dz = m_fFishSpeed * cos(DEG2RAD(rotAngle));

	// Set fish's movement
	m_affCoe.rotateY = rotAngle;
	m_affCoe.translateX += dx;
	m_affCoe.translateZ += dz;

	// If collide wall or object
	Object* object;
	if (IsHitObject(this, object))
	{
		switch (object->GetType())
		{
		case PLAYER:
		{
			// Can't advance
			m_affCoe.translateX -= dx;
			m_affCoe.translateZ -= dz;
		}
		break;

		case SHARK:
		{
			// Lose
			SceneManager::GetInstance()->EndGame(false);
		}

		case ITEM:
		{
			/*if (((Item*)(object))->m_isBeingUsed)
			{
			switch (((Item*)(object))->GetItemType())
			{
			case CORAL:
			{
			// Can't advance
			fish->m_affCoe.translateX -= dx;
			fish->m_affCoe.translateZ -= dz;
			}
			break;

			default:
			break;
			}
			}*/
		}
		break;
		}
	}
}

Fish::~Fish()
{
	ClearFishPath();
}