#include "stdafx.h"
#include "Processor.h"
#include "SceneManager.h"

float DEG2RAD(float i)
{
	return i * PI / 180.0f;
}

float RAD2DEG(float i)
{
	return i * 180.0 / PI;
}

bool isCollide(Vector4 rec1, Vector4 rec2)

{
	if (rec1.x < rec2.y && rec1.y > rec2.x && rec1.z < rec2.w && rec1.w > rec2.z)
	{
		return true;
	}

	return false;
}

bool IsHitObject(Object *object, Object *&rObject) {
	Vector4 rec1(object->m_affCoe.translateX + object->m_pos.min.x, object->m_affCoe.translateX + object->m_pos.max.x,
		object->m_affCoe.translateZ + object->m_pos.min.z, object->m_affCoe.translateZ + object->m_pos.max.z);

	switch (object->GetType())
	{
	case PLAYER:
	{
		Object* gate = SceneManager::GetInstance()->m_gate;

		Vector4 rec2(gate->m_affCoe.translateX + gate->m_pos.min.x, gate->m_affCoe.translateX + gate->m_pos.max.x,
			gate->m_affCoe.translateZ + gate->m_pos.min.z, gate->m_affCoe.translateZ + gate->m_pos.max.z);

		if (gate->m_drawable && isCollide(rec1, rec2))
		{
			rObject = gate;
			return true;
		}

		for (vector<Object*>::iterator i = SceneManager::GetInstance()->m_listFish.begin(); i != SceneManager::GetInstance()->m_listFish.end(); ++i)
		{
			Vector4 rec2((*i)->m_affCoe.translateX + (*i)->m_pos.min.x, (*i)->m_affCoe.translateX + (*i)->m_pos.max.x,
				(*i)->m_affCoe.translateZ + (*i)->m_pos.min.z, (*i)->m_affCoe.translateZ + (*i)->m_pos.max.z);

			if ((*i)->m_drawable && isCollide(rec1, rec2))
			{
				rObject = *i;
				return true;
			}
		}
	}
	break;

	case FISH:
	{
		Object* o = SceneManager::GetInstance()->m_player;

		Vector4 rec2(o->m_affCoe.translateX + o->m_pos.min.x, o->m_affCoe.translateX + o->m_pos.max.x,
			o->m_affCoe.translateZ + o->m_pos.min.z, o->m_affCoe.translateZ + o->m_pos.max.z);

		if (isCollide(rec1, rec2))
		{
			rObject = o;
			o = NULL;
			return true;
		}
	}
	break;

	case SHARK:
	{
		// Collide into player
		Object* o = SceneManager::GetInstance()->m_player;

		Vector4 rec2(o->m_affCoe.translateX + o->m_pos.min.x, o->m_affCoe.translateX + o->m_pos.max.x,
			o->m_affCoe.translateZ + o->m_pos.min.z, o->m_affCoe.translateZ + o->m_pos.max.z);

		if (isCollide(rec1, rec2))
		{
			rObject = o;
			o = NULL;
			return true;
		}

		// Collide into fish
		for (vector<Object*>::iterator i = SceneManager::GetInstance()->m_listFish.begin(); i != SceneManager::GetInstance()->m_listFish.end(); ++i)
		{
			rec2 = Vector4((*i)->m_affCoe.translateX + (*i)->m_pos.min.x, (*i)->m_affCoe.translateX + (*i)->m_pos.max.x,
				(*i)->m_affCoe.translateZ + (*i)->m_pos.min.z, (*i)->m_affCoe.translateZ + (*i)->m_pos.max.z);

			if ((*i)->m_drawable && isCollide(rec1, rec2))
			{
				rObject = *i;
				return true;
			}
		}
	}
	break;
	}

	// Collide into Item
	for (vector<Object*>::iterator i = SceneManager::GetInstance()->m_listItem.begin(); i != SceneManager::GetInstance()->m_listItem.end(); ++i)
	{
		if ((*i)->m_drawable)
		{
			Vector4 rec2((*i)->m_affCoe.translateX + (*i)->m_pos.min.x, (*i)->m_affCoe.translateX + (*i)->m_pos.max.x,
				(*i)->m_affCoe.translateZ + (*i)->m_pos.min.z, (*i)->m_affCoe.translateZ + (*i)->m_pos.max.z);

			if ((*i)->m_drawable && isCollide(rec1, rec2))
			{
				rObject = *i;
				return true;
			}
		}
	}

	rObject = NULL;

	return false;
}

bool IsHitWall(Object *o)
{
	Vector2 far_left(o->m_affCoe.translateX + o->m_pos.min.x, o->m_affCoe.translateZ + o->m_pos.min.z);
	Vector2 near_left(o->m_affCoe.translateX + o->m_pos.min.x, o->m_affCoe.translateZ + o->m_pos.max.z);
	Vector2 far_right(o->m_affCoe.translateX + o->m_pos.max.x, o->m_affCoe.translateZ + o->m_pos.min.z);
	Vector2 near_right(o->m_affCoe.translateX + o->m_pos.max.x, o->m_affCoe.translateZ + o->m_pos.max.z);

	int p = (int)(near_left.y + SceneManager::GetInstance()->m_map->m_size / 2.0f);

	// tren map hang = z, cot = x
	if (SceneManager::GetInstance()->m_map->m_data[(int)(far_left.y + SceneManager::GetInstance()->m_map->m_size / 2.0f)][(int)(far_left.x + SceneManager::GetInstance()->m_map->m_size / 2.0f)] != '0') return true;
	if (SceneManager::GetInstance()->m_map->m_data[(int)(far_right.y + SceneManager::GetInstance()->m_map->m_size / 2.0f)][(int)(far_right.x + SceneManager::GetInstance()->m_map->m_size / 2.0f)] != '0') return true;
	if (SceneManager::GetInstance()->m_map->m_data[(int)(near_left.y + SceneManager::GetInstance()->m_map->m_size / 2.0f)][(int)(near_left.x + SceneManager::GetInstance()->m_map->m_size / 2.0f)] != '0') return true;
	if (SceneManager::GetInstance()->m_map->m_data[(int)(near_right.y + SceneManager::GetInstance()->m_map->m_size / 2.0f)][(int)(near_right.x + SceneManager::GetInstance()->m_map->m_size / 2.0f)] != '0') return true;

	return false;
}

void SetObstacleAt(Vector2 pos)
{
	SceneManager::GetInstance()->m_map->m_data[(GLint)pos.x][(GLint)pos.y] = '1';
}

void DestroyObstacleAt(Vector2 pos)
{
	SceneManager::GetInstance()->m_map->m_data[(GLint)pos.x][(GLint)pos.y] = '0';
}

bool CheckInBound(Vector3 point, Button* button)
{
	if (point.x <= (button->m_affCoe.translateX + button->m_pos.max.x) && point.x >= (button->m_affCoe.translateX - button->m_pos.max.x) &&
		(point.y <= (button->m_affCoe.translateY + button->m_pos.max.x) && point.y >= (button->m_affCoe.translateY - button->m_pos.max.x)))
	{
		return true;
	}

	return false;
}

Button* CheckCollision(Vector3 point, vector<Button*>* lstButtons)
{
	for (vector<Button*>::iterator i = lstButtons->begin(); i != lstButtons->end(); ++i)
	{
		if (point.x <= ((*i)->m_affCoe.translateX + (*i)->m_pos.max.x) && point.x >= ((*i)->m_affCoe.translateX - (*i)->m_pos.max.x) &&
			(point.y <= ((*i)->m_affCoe.translateY + (*i)->m_pos.max.y) && point.y >= ((*i)->m_affCoe.translateY - (*i)->m_pos.max.y)))
		{
			return *i;
		}
	}

	return NULL;
}

//------------------------------------------------------------------------------------
// \brief FindPathing using A-star - JPS algorithm
// \param xStart - x-coordinate for start position
// \param yStart - y-coordinate for start position
// \param xFinish - x-coordinate for end position
// \param yFinish - y-coordinate for end position
// \return List of Vector3 with each vector indicate position and direction
//------------------------------------------------------------------------------------
//unsigned step = 0, int skip = 0, // optional params
//size_t *stepsDone = NULL, size_t *nodesExpanded = NULL // for information
vector<Position*>* FindPath(int xStart, int yStart, int xFinish, int yFinish)
{
	Searcher search(*SceneManager::GetInstance()->m_map);
	search.setSkip(0);//(skip);

	vector<Position*>* path = new vector<Position*>();
	bool found = search.findPath(path, Pos(xStart, yStart), Pos(xFinish, yFinish), 1); //(1 = step)

	if (!found)
	{
		for (int i = 0; i < path->size(); i++)
		{
			if (path->at(i)) delete path->at(i);
			path->at(i) = NULL;
		}

		delete path;
		return new vector<Position*>();
	}

	return path;
}

Vector2 RotatePoint(Vector2 point, Vector2 center, float angle)
{
	angle = DEG2RAD(angle);
	return Vector2(cos(angle) * (point.x - center.x) - sin(angle) * (point.y - center.y) + center.x,
		sin(angle) * (point.x - center.x) + cos(angle) * (point.y - center.y) + center.y);
}

Vector2 MapToCoordinate(int x, int y)
{
	return Vector2(float(y - Map::m_size / 2), float(x - Map::m_size / 2));
}

Vector2 MapToCoordinate(Vector2 pos)
{
	return Vector2(float(pos.y - Map::m_size / 2), float(pos.x - Map::m_size / 2));
}

Vector2 MapToCoordinate(Position pos)
{
	return Vector2(float((int)pos.y - Map::m_size / 2), float((int)pos.x - Map::m_size / 2));
}

Vector2 CoordinateToMap(float x, float y)
{
	return Vector2(float(int(y + Map::m_size / 2.0f)), float(int(x + Map::m_size / 2.0f)));
}

Vector2 CoordinateToMap(Vector2 pos)
{
	return Vector2(float(int(pos.y + Map::m_size / 2.0f)), float(int(pos.x + Map::m_size / 2.0f)));
}

void UnProject(GLfloat winx, GLfloat winy, GLfloat winz, Matrix modelMatrix,
	Matrix projMatrix, Vector4 viewport, GLfloat *objx, GLfloat *objy, GLfloat *objz)
{
	Matrix finalMatrix;
	Vector4 in;
	Vector4 out;

	finalMatrix = (modelMatrix * projMatrix).Inverse();

	in.x = winx;
	in.y = winy;
	in.z = winz;
	in.w = 1.0;

	/* Map x and y from window coordinates */
	in.x = (in.x - viewport.x) / viewport.z;
	in.y = (in.y - viewport.y) / viewport.w;

	/* Map to range -1 to 1 */
	in.x = in.x * 2 - 1;
	in.y = in.y * 2 - 1;
	in.z = in.z * 2 - 1;

	out = finalMatrix * in;

	out.x /= out.w;
	out.y /= out.w;
	out.z /= out.w;

	*objx = out.x;
	*objy = out.y;
	*objz = out.z;
}

void Project(GLfloat objx, GLfloat objy, GLfloat objz, Matrix modelMatrix,
	Matrix projMatrix, Vector4 viewport, GLfloat *winx, GLfloat *winy, GLfloat *winz)
{
	Vector4 in;
	Vector4 out;

	in.x = objx;
	in.y = objy;
	in.z = objz;
	in.w = 1.0;

	out = modelMatrix * in;
	in = projMatrix * out;

	in.x /= in.w;
	in.y /= in.w;
	in.z /= in.w;
	/* Map x, y and z to range 0-1 */
	in.x = in.x * 0.5 + 0.5;
	in.y = in.y * 0.5 + 0.5;
	in.z = in.z * 0.5 + 0.5;

	/* Map x,y to viewport */
	in.x = in.x * viewport.z + viewport.x;
	in.y = in.y * viewport.w + viewport.y;

	*winx = in.x;
	*winy = in.y;
	*winz = in.z;
}

void CreateRay(int mouseX, int mouseY, Vector3 *mousePos)
{
	GLfloat x, y, z;
	int viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);

	// Mouse position from window coordinates to 3D scene position
	UnProject(mouseX, viewport[3] - mouseY, 0.0, Matrix(),
		Matrix(), Vector4(viewport[0], viewport[1], viewport[2], viewport[3]), &x, &y, &z);

	mousePos->x = x;
	mousePos->y = y;
	mousePos->z = z;
}

void CreateRay(int mouseX, int mouseY, Vector3 *mouseRay_start, Vector3 *mouseRay_end) {
	GLfloat x, y, z;
	int viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);

	// Mouse position from window coordinates to 3D scene position
	UnProject(mouseX, viewport[3] - mouseY, 0.0, Matrix(),
		Matrix(), Vector4(viewport[0], viewport[1], viewport[2], viewport[3]), &x, &y, &z);

	mouseRay_start->x = x;
	mouseRay_start->y = y;
	mouseRay_start->z = z;

	UnProject(mouseX, viewport[3] - mouseY, 1.0, Matrix(),
		Matrix(), Vector4(viewport[0], viewport[1], viewport[2], viewport[3]), &x, &y, &z);

	mouseRay_end->x = x;
	mouseRay_end->y = y;
	mouseRay_end->z = z;
}

float PointToLineDistance(Vector3 A, Vector3 B, Vector3 P) {
	Vector3 AB = B - A;
	Vector3 AP = P - A;

	double projection = AP.Dot(AB) / AB.Dot(AB);
	Vector3 nearPoint = A + AB * projection;

	return (nearPoint - P).Length();
}

bool CheckCollision(Vector3 point, Button* button)
{
	if (point.x <= (button->m_affCoe.translateX + button->m_pos.max.x) && point.x >= (button->m_affCoe.translateX - button->m_pos.max.x) &&
		(point.y <= (button->m_affCoe.translateY + button->m_pos.max.y) && point.y >= (button->m_affCoe.translateY - button->m_pos.max.y)))
	{
		return true;
	}

	return false;
}

