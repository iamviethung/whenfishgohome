#include "stdafx.h"
#include "WaterCaustic.h"
#include "ResourceManager.h"
#include "Processor.h"

WaterCaustic::WaterCaustic()
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
	deltaT = 0.01f;
}

WaterCaustic::WaterCaustic(const WaterCaustic &Obj)
{
	m_pos = Obj.m_pos;
	m_texBase = Obj.m_texBase;
	m_texDisp = Obj.m_texDisp;
	m_affCoe = Obj.m_affCoe;
	m_maModel = Matrix(Obj.m_maModel);
	m_shader = Obj.m_shader;
}

WaterCaustic::~WaterCaustic()
{
	m_shader = NULL;
}

void WaterCaustic::Init(GLuint modelPos, GLuint texBase, GLuint texDisp, GLuint shaderID)
{
	m_pos = ResourceManager::GetInstance()->m_objPos.at(modelPos);
	m_texBase = ResourceManager::GetInstance()->m_texID.at(texBase);
	m_texDisp = ResourceManager::GetInstance()->m_texID.at(texDisp);
	m_shader = ResourceManager::GetInstance()->m_shader.at(shaderID);

	glUseProgram(m_shader->getProgramId());
	glUniform1i(m_shader->getUniform("u_texBase"), 1);
	glUniform1i(m_shader->getUniform("u_texDisp"), 2);

	glUniform3f(m_shader->getUniform("u_camPos"), 1.0f, 1.0f, -1.0f);
}

void WaterCaustic::Draw()
{
	if (!this->m_drawable)
		return;

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glUseProgram(m_shader->getProgramId());

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, m_texBase);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, m_texDisp);

	glBindBuffer(GL_ARRAY_BUFFER, m_pos.vboID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_pos.iboID);

	glUniformMatrix4fv(m_shader->getUniform("u_WVP"), 1, GL_FALSE, m_maModel.m[0]);

	glUniform1f(m_shader->getUniform("u_time"), deltaT);
	if (deltaT < 1.0f)
		deltaT += 0.01f;
	else
		deltaT = 0.01f;

	//vertex pointer
	if (m_shader->getAttrib("a_posL") != -1)
	{
		glEnableVertexAttribArray(m_shader->getAttrib("a_posL"));
		glVertexAttribPointer(m_shader->getAttrib("a_posL"), 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	}

	//indices
	if (m_shader->getAttrib("a_uv") != -1)
	{
		glEnableVertexAttribArray(m_shader->getAttrib("a_uv"));
		glVertexAttribPointer(m_shader->getAttrib("a_uv"), 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (char*)0 + sizeof(Vector3));
	}

	glDrawElements(GL_TRIANGLES, m_pos.numIndices, GL_UNSIGNED_INT, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void WaterCaustic::Update()
{
	glUseProgram(m_shader->getProgramId());

	Matrix finalMatrix;
	m_maModel = finalMatrix.SetScale(m_affCoe.scaleX, m_affCoe.scaleY, m_affCoe.scaleZ);
	m_maModel = m_maModel * finalMatrix.SetRotationZ(DEG2RAD(m_affCoe.rotateZ));
	m_maModel = m_maModel * finalMatrix.SetRotationX(DEG2RAD(m_affCoe.rotateX));
	m_maModel = m_maModel * finalMatrix.SetRotationY(DEG2RAD(m_affCoe.rotateY));
	m_maModel = m_maModel * finalMatrix.SetTranslation(m_affCoe.translateX, m_affCoe.translateY, m_affCoe.translateZ);
}