#include "stdafx.h"
#include "Globals.h"
#include "Camera.h"
#include "Processor.h"
#include "SceneManager.h"
#include <time.h>
#include <stdlib.h>


Player::Player() : m_fRotateAngle(0.0f), m_xVel(0.0f), m_zVel(0.0f), m_hostage(NULL), m_item(NULL), m_state(NORMAL), m_hasKey(false)
{}

void Player::ResetPlayer()
{
	ResetState();
	m_hostage = NULL;
	m_item = NULL;
	m_fRotateAngle = 0.0f;
	m_xVel = 0.0f;
	m_zVel = 0.0f;
	m_hasKey = false;
}

Type Player::GetType()
{
	return PLAYER;
}

Fish* Player::GetHostage()
{
	return m_hostage;
}

bool Player::HasHostage()
{
	if (m_hostage)
		return true;

	return false;
}

bool Player::InStateDuration()
{
	m_stateStartTime++;
	return m_stateStartTime < m_stateDuration;//(clock() - m_stateStartTime < m_stateDuration);
}

void Player::MoveCamera(GLfloat xCamVel, GLfloat zCamVel)
{
	Camera::getInstance()->m_vEye.x += xCamVel;
	Camera::getInstance()->m_vEye.z += zCamVel;
	Camera::getInstance()->m_vLook.x = m_affCoe.translateX;
	Camera::getInstance()->m_vLook.z = m_affCoe.translateZ;
	Camera::getInstance()->SetLookAt();
}

void Player::DropItem()
{
	if (m_hasKey)
	{
		Object* object;
		if (IsHitObject(this, object) && object->GetType() == OBJECT)
		{
			object->m_drawable = false;
			DestroyObstacleAt(CoordinateToMap(object->m_affCoe.translateX, object->m_affCoe.translateZ));
			object = NULL;
			m_key = NULL;
			m_hasKey = false;
			return;
		}
		object = NULL;
	}

	if (m_item == NULL)
		return;

	m_item->Use(m_affCoe.translateX, m_affCoe.translateZ);
	SceneManager::GetInstance()->m_lstButton.back()->m_texturePos = SceneManager::GetInstance()->m_lstButton.back()->m_UnClickedTexPos;
	m_item = NULL;
}

void Player::InitStateTime()
{
	m_stateStartTime = 0;
}

void Player::SpeedUp()
{
	m_state = BOOST;
	m_fPlayerSpeed *= 2;
	InitStateTime();
	m_stateDuration = 180;

	if (m_hostage)
		m_hostage->SetSpeed(m_fPlayerSpeed);
}

void Player::Poison()
{
	m_state = POISONED;
	InitStateTime();
	m_stateDuration = 180;
}

void Player::NormalizeSpeed()
{
	m_state = NORMAL;
	m_fPlayerSpeed /= 2;

	if (m_hostage)
		m_hostage->SetSpeed(m_fPlayerSpeed);
}

void Player::ResetState()
{
	switch (m_state)
	{
	case BOOST:
	{
		NormalizeSpeed();
	}
	break;

	case POISONED:
	{
		m_state = NORMAL;
	}
	break;
	}
}

void Player::Process()
{
	switch (m_state)
	{
	case BOOST:
	{
		if (!InStateDuration())
			NormalizeSpeed();

		Move();
	}
	break;

	case NORMAL:
	{
		Move();
	}
	break;

	case POISONED:
	{
		if (!InStateDuration())
			m_state = NORMAL;

		Move();
	}
	break;
	}
}

void Player::Move()
{
	Object* object;
	Vector2 pos;

	// Set player's movement
	m_affCoe.rotateY = m_fRotateAngle + ((m_state == POISONED) ? 180.0f : 0.0f);
	m_xVel = m_fPlayerSpeed * sin(DEG2RAD(m_affCoe.rotateY));
	m_zVel = m_fPlayerSpeed * cos(DEG2RAD(m_affCoe.rotateY));
	m_affCoe.translateX += m_xVel;
	m_affCoe.translateZ += m_zVel;

	float camX = m_xVel;
	float camZ = m_zVel;

	// If collide into objects
	if (IsHitObject(this, object))
	{
		switch (object->GetType())
		{
		case FISH:
		{
			if (m_hostage == NULL)
			{
				m_hostage = (Fish*)object;
				m_hostage->SetSpeed(m_fPlayerSpeed);
			}
		}
		break;

		case SHARK:
		{
			// End game
			SceneManager::GetInstance()->EndGame(false);
		}
		break;

		case ITEM:
		{
			switch (((Item*)object)->GetItemType())
			{
			case STARFISH: case POISON:
			{
				ResetState();
				((Item*)object)->DoEffect(this);
			}
			break;

			case WEED: case GREENTURTLE: case REDTURTLE:
			{
				if (!((Item*)object)->m_isBeingUsed)
				{
					if (m_item)
					{
						m_item->Drop();
					}

					m_item = (Item*)object;
					m_item->m_drawable = false;
					SceneManager::GetInstance()->m_lstButton.back()->m_texturePos = SceneManager::GetInstance()->m_listItemTexture.at(m_item->m_id);
				}
			}
			break;

			case KEY:
			{
				m_key = (Item*)object;
				m_key->m_drawable = false;
				m_hasKey = true;
			}
			break;

			}

		}
		break;

		default:
			break;
		}

		camX = m_xVel;
		camZ = m_zVel;
	}

	// If collide into wall
	if (IsHitWall(this))
	{
		// Move along walls
		m_affCoe.translateX -= m_xVel;

		if (IsHitWall(this))
		{
			m_affCoe.translateZ -= m_zVel;
			m_affCoe.translateX += m_xVel;

			if (IsHitWall(this))
			{
				m_affCoe.translateX -= m_xVel;
				camX = 0.0f;
				camZ = 0.0f;
			}
			else
			{
				camX = m_xVel;
				camZ = 0.0f;
			}
		}
		else
		{
			camX = 0.0f;
			camZ = m_zVel;
		}
	}

	MoveCamera(camX, camZ);
	object = NULL;
}

Player::~Player()
{}