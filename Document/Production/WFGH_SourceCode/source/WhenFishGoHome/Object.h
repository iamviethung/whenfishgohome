#ifndef __OBJECT_H__
#define __OBJECT_H__

#include "Struct.h"

class Object
{
public:
	Object();
	Object(const Object &Obj);
	virtual ~Object();

	Object* Clone();
	void Init(GLuint modelPos, GLuint texID, GLuint shaderID);
	void Draw();
	void Update();
	bool IsEqual(Object *o);
	virtual Type GetType();

public:
	GLuint m_id; // Object id
	ObjectInfo m_pos; // Vertices and indices info
	GLuint m_texturePos; // Texture ID
	AffineCoefficient m_affCoe; // Affine
	Matrix m_maModel; // Model Matrix
	Program* m_shader; // Shader object
	bool m_drawable; // If true - draw
};

#endif