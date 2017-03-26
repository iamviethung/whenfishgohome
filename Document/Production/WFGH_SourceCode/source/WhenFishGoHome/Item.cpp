#include "stdafx.h"
#include "Item.h"
#include "Map.h"
#include "Shark.h"
#include "Processor.h"
#include "SceneManager.h"
#include <time.h>
#include <stdlib.h>


Item::Item() : m_isBeingUsed(false)
{}

Item::~Item()
{}

Type Item::GetType()
{
	return ITEM;
}

ItemType Item::GetItemType()
{
	return m_iType;
}

void Item::SetItemType(ItemType type)
{
	m_iType = type;
}

void Item::Drop()
{
	int x, y;
	srand((GLuint)time(NULL));
	Map *map = SceneManager::GetInstance()->m_map;
	Vector2 pos = CoordinateToMap(this->m_affCoe.translateX, this->m_affCoe.translateZ);
	m_drawable = true;
	m_isBeingUsed = false;

	while (true)
	{
		x = -map->m_size + (rand() % (2 * map->m_size + 1)) + pos.x;
		y = -map->m_size + (rand() % (2 * map->m_size + 1)) + pos.y;

		if (x >= 0 && y >= 0 && x < map->m_size && y < map->m_size && map->m_data[x][y] == '0')
			break;
	}

	pos = MapToCoordinate(x, y);
	m_affCoe.translateX = pos.x;
	m_affCoe.translateZ = pos.y;
}

void Item::Use(GLfloat posX, GLfloat posZ)
{
	m_drawable = true;
	m_isBeingUsed = true;
	m_affCoe.translateX = posX;
	m_affCoe.translateZ = posZ;
}

void Item::DoEffect(Object* object)
{
	switch (m_iType)
	{
	case STARFISH:
	{
		((Player*)object)->SpeedUp();
		m_drawable = false;
	}
	break;

	case GREENTURTLE:
	{
		((Shark*)object)->Paralize(120);
		m_drawable = false;
	}
	break;

	case REDTURTLE:
	{
		((Shark*)object)->Paralize(240);
		m_drawable = false;
	}
	break;

	case WEED:
	{
		((Shark*)object)->SlowDown();
		m_drawable = false;
	}
	break;

	case POISON:
	{
		((Player*)object)->Poison();
		m_drawable = false;
	}
	break;

	case KEY:
	{

	}
	break;
	}
}