#ifndef __PROCESSOR_H__
#define __PROCESSOR_H__

#include "Button.h"
#include "JumpPointSearch.h"

#include <vector>
using namespace std;

float DEG2RAD(float i);
float RAD2DEG(float i);
bool IsHitObject(Object *object, Object *&rObject);
bool IsHitWall(Object *o);
void SetObstacleAt(Vector2 pos);
void DestroyObstacleAt(Vector2 pos);
bool CheckInBound(Vector3 point, Button* button);
Button* CheckCollision(Vector3 point, vector<Button*>* lstButtons);
Vector2 RotatePoint(Vector2 point, Vector2 center, float angle);

// Map and coordinate converter
Vector2 MapToCoordinate(int x, int y);
Vector2 MapToCoordinate(Vector2 pos);
Vector2 MapToCoordinate(Position pos);
Vector2 CoordinateToMap(float x, float y);
Vector2 CoordinateToMap(Vector2 pos);

// Path finding using Jump Point Search
vector<Position*>* FindPath(int xStart, int yStart, int xFinish, int yFinish);

// Ray casting related function
void UnProject(GLfloat winx, GLfloat winy, GLfloat winz, Matrix modelMatrix,
	Matrix projMatrix, Vector4 viewport, GLfloat *objx, GLfloat *objy, GLfloat *objz);
void Project(GLfloat objx, GLfloat objy, GLfloat objz, Matrix modelMatrix,
	Matrix projMatrix, Vector4 viewport, GLfloat *winx, GLfloat *winy, GLfloat *winz);
void CreateRay(int mouseX, int mouseY, Vector3 *mousePos);
void CreateRay(int mouseX, int mouseY, Vector3 *mouseRay_start, Vector3 *mouseRay_end);
float PointToLineDistance(Vector3 A, Vector3 B, Vector3 P);

bool CheckCollision(Vector3 point, Button* button);

#endif