#include "stdafx.h"
#include "MiniMap.h"
#include "Globals.h"

MiniMap* MiniMap::m_instance = NULL;

MiniMap::MiniMap()
{
	Init();
	m_map = new Object();
	m_border = new Object();

	m_map->m_id = 0;
	m_map->m_texturePos = colorTexID;
	m_border->m_id = 0;
	m_border->m_texturePos = 0;
}

void MiniMap::Init()
{
	//gen fbo
	glGenFramebuffers(1, &fboID);
	glBindFramebuffer(GL_FRAMEBUFFER, fboID);

	//generate color texture
	glActiveTexture(GL_TEXTURE0);
	glGenTextures(1, &colorTexID);
	glBindTexture(GL_TEXTURE_2D, colorTexID);
	//initialize an empty texture with screen width & height
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, Globals::screenWidth, Globals::screenHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	//attach texture to GL_COLOR_ATTACHMENT0
	glBindFramebuffer(GL_FRAMEBUFFER, fboID);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorTexID, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	//bind system default texture
	glBindTexture(GL_TEXTURE_2D, 0);

	//generate depth texture – same as color texture
	glActiveTexture(GL_TEXTURE0 + 1);
	glGenTextures(1, &depthTexID);
	glBindTexture(GL_TEXTURE_2D, depthTexID);
	//initialize an empty texture with screen width & height
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, Globals::screenWidth, Globals::screenHeight, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_INT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	//attach texture to GL_DEPTH_ATTACHMENT
	glBindFramebuffer(GL_FRAMEBUFFER, fboID);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTexID, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	//bind system default texture
	glBindTexture(GL_TEXTURE_2D, 0);

	//return to default buffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

MiniMap* MiniMap::GetInstance()
{
	if (m_instance == NULL)
		m_instance = new MiniMap();

	return m_instance;
}

MiniMap::~MiniMap()
{
	if (m_map)
		delete m_map;
	m_map = NULL;

	if (m_border)
		delete m_border;
	m_border = NULL;
	m_instance = NULL;
}
