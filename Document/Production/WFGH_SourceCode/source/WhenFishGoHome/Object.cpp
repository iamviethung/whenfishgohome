#include "stdafx.h"
#include "Object.h"
#include "Processor.h"
#include "ResourceManager.h"

Object::Object()
{
	m_affCoe.translateX = 0.0;
	m_affCoe.translateY = 0.0;
	m_affCoe.translateZ = 0.0;

	m_affCoe.rotateX = 0.0;
	m_affCoe.rotateY = 0.0;
	m_affCoe.rotateZ = 0.0;

	m_affCoe.scaleX = 1.0;
	m_affCoe.scaleY = 1.0;
	m_affCoe.scaleZ = 1.0;

	m_drawable = true;
	m_shader = NULL;
}

Object::Object(const Object &Obj)
{
	m_pos = Obj.m_pos;
	m_texturePos = Obj.m_texturePos;
	m_affCoe = Obj.m_affCoe;
	m_maModel = Matrix(Obj.m_maModel);
	m_shader = Obj.m_shader;
}

Object::~Object()
{
	m_shader = NULL;
}

Object* Object::Clone()
{
	Object* result = new Object(*this);
	return result;
}

void Object::Init(GLuint modelPos, GLuint texID, GLuint shaderID)
{
	m_pos = ResourceManager::GetInstance()->m_objPos.at(modelPos);
	m_texturePos = ResourceManager::GetInstance()->m_texID.at(texID);
	m_shader = ResourceManager::GetInstance()->m_shader.at(shaderID);
}

void Object::Draw()
{
	if (!this->m_drawable)
		return;

	glUseProgram(m_shader->getProgramId());

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, this->m_texturePos);
	
	glBindBuffer(GL_ARRAY_BUFFER, m_pos.vboID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_pos.iboID);

	glUniformMatrix4fv(m_shader->getUniform("u_mvpMatrix"), 1, GL_FALSE, m_maModel.m[0]);

	if(m_shader->getAttrib("a_posL") != -1)
	{
		glEnableVertexAttribArray(m_shader->getAttrib("a_posL"));
		glVertexAttribPointer(m_shader->getAttrib("a_posL"), 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	}

	if(m_shader->getAttrib("a_uv") != -1)
	{
		glEnableVertexAttribArray(m_shader->getAttrib("a_uv"));
		glVertexAttribPointer(m_shader->getAttrib("a_uv"), 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (char*)0 + sizeof(Vector3));
	}

	glDrawElements(GL_TRIANGLES, m_pos.numIndices, GL_UNSIGNED_INT, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Object::Update()
{
	glUseProgram(m_shader->getProgramId());

	Matrix finalMatrix;
	m_maModel = finalMatrix.SetScale(m_affCoe.scaleX, m_affCoe.scaleY, m_affCoe.scaleZ);
	m_maModel = m_maModel * finalMatrix.SetRotationZ(DEG2RAD(m_affCoe.rotateZ));
	m_maModel = m_maModel * finalMatrix.SetRotationX(DEG2RAD(m_affCoe.rotateX));
	m_maModel = m_maModel * finalMatrix.SetRotationY(DEG2RAD(m_affCoe.rotateY));
	m_maModel = m_maModel * finalMatrix.SetTranslation(m_affCoe.translateX, m_affCoe.translateY, m_affCoe.translateZ);
}

bool Object::IsEqual(Object *o) 
{
	return (this->m_texturePos == o->m_texturePos);
}

Type Object::GetType()
{
	return OBJECT;
}