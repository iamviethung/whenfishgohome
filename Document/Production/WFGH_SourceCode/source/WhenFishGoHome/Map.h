#ifndef __MAP_H__
#define __MAP_H__

#include "Object.h"
#include <vector>
using namespace std;

class Map
{
public:
	Map();
	~Map();

	void GenMap(Object* obstructors, Object* floor);
	void Draw();
	void Update();
	bool ReadMap(const char* file);
	void TurnOnHighLight(vector<Vector3*>* route);
	void TurnOffHightlight(vector<Vector3*>* route);
	void TurnOffHightlightAt(float x, float y);
	bool operator()(unsigned x, unsigned y) const;

public:
	char** m_data;
	static int m_size;
	static Object** m_obstructors;
	static Object* m_floor;

private:
	void GenObjects();
};


#endif