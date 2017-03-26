#ifndef __ITEM_H__
#define __ITEM_H__

#include "Struct.h"
#include "Object.h"

class Item : public Object
{
public:
	Item();
	~Item();
	Type GetType();
	ItemType GetItemType();
	void SetItemType(ItemType type);
	void Drop();
	void Use(GLfloat posX, GLfloat posZ);
	void DoEffect(Object* object);

public:
	bool m_isBeingUsed;

private:
	ItemType m_iType;
};

#endif