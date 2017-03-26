#ifndef __WATERCAUSTIC_H__
#define __WATERCAUSTIC_H__

#include "Struct.h"

class WaterCaustic
{
public:
	WaterCaustic();
	WaterCaustic(const WaterCaustic &wc);
	~WaterCaustic();

	void Init(GLuint modelPos, GLuint texBase, GLuint texDisp, GLuint shaderID);
	void Draw();
	void Update();

public:
	bool m_drawable; // If true - draw
	GLuint m_texBase;
	GLuint m_texDisp;
	ObjectInfo m_pos; // Vertices and indices info
	AffineCoefficient m_affCoe; // Affine
	Matrix m_maModel; // Model Matrix
	Program* m_shader; // Shader object

private:
	float deltaT;
};

#endif