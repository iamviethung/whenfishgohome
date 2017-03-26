#ifndef __MINIMAP_H__
#define __MINIMAP_H__

#include "Object.h"

class MiniMap
{
public:
	~MiniMap();
	static MiniMap* GetInstance();

public:
	GLuint fboID;
	GLuint colorTexID;
	GLuint depthTexID;
	Object* m_map;
	Object* m_border;

private:
	MiniMap();
	void Init();

private:
	static MiniMap* m_instance;
};

#endif