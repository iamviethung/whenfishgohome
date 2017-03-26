#ifndef __STRUCT_H__
#define __STRUCT_H__

#define PI 3.14159265f

enum Type { PLAYER, SHARK, FISH, ITEM, BUTTON, OBJECT };
enum ItemType { STARFISH, GREENTURTLE, REDTURTLE, WEED, POISON, KEY };
enum ButtonType { START, PAUSE, RESUME, TUTORIAL, OPTIONS, CREDITS, EXIT, QUIT, MENU, DROP_ITEM, REPLAY, LEVEL, RETURN };
enum LevelState { LOCKED, UNLOCKED, UNOPEN };
enum Result { NO_PATH, FOUND_PATH, NEED_MORE_STEPS };
enum STATE { NORMAL, PARALIZED, SLOW, PURSUING, BOOST, POISONED };
enum SCENE { MAINMENU, LEVEL_CHOOSING, CREDIT, TUTORIALS };

struct Color
{
	float R;
	float G;
	float B;
	float A;
};

struct Vertex
{
	Vector3 pos;
	Vector2 uv;
	Color color;
	Vector3 normal;
	Vector3 binormal;
	Vector3 tangent;
};

struct AffineCoefficient
{
	float translateX;
	float translateY;
	float translateZ;

	float rotateX;
	float rotateY;
	float rotateZ;

	float scaleX;
	float scaleY;
	float scaleZ;
};

struct ObjectInfo
{
	GLuint numIndices;
	GLuint vboID;
	GLuint iboID;
	Vector3 min;
	Vector3 max;
};

struct Position
{
	int x, y;

	inline bool operator==(const Position& p) const
	{
		return x == p.x && y == p.y;
	}
	inline bool operator!=(const Position& p) const
	{
		return x != p.x || y != p.y;
	}

	// for sorting
	inline bool operator<(const Position& p) const
	{
		return y < p.y || (y == p.y && x < p.x);
	}

	inline bool isValid() const { return x != unsigned(-1); }
};

#endif