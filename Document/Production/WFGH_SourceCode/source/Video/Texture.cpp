#include "stdafx.h"
#include "Texture.h"

#include "stb_image_aug.h"

Texture::Texture(const char* filePath, bool isCubeTexture) :
												m_glTexId(-1),
												m_pData(NULL),
												m_nWidth(-1),
												m_nHeight(-1),
												m_nBpp(-1),
												m_isCubeTexture(isCubeTexture),
												m_isLoaded(false),
												m_iMinFilter(GL_LINEAR),
												m_iMagFilter(GL_LINEAR),
												m_iTileMode(GL_CLAMP_TO_EDGE)
{	
	strcpy(m_sFilePath, filePath);	
	memset(m_cubePixels, 0, sizeof(char*)*CUBE_TOTAL_SIDE);
}

Texture::~Texture()
{
	unloadTexture();
}

bool Texture::loadTexture()
{
	if (m_isLoaded)
		return true;

	return m_isCubeTexture ? loadCubeTexture() : load2dTexture();
}

bool Texture::loadCubeTexture()
{
	char file_path[BUFFER_SIZE];

	//send data to gpu
	glGenTextures(1, &m_glTexId);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_glTexId);

	for (int i = 0; i < CUBE_TOTAL_SIDE; i++)
	{
		sprintf(file_path, m_sFilePath, i);

		m_cubePixels[i] = loadTextureData(file_path, m_nWidth, m_nHeight, m_nBpp);
		ASSERT_MSG(m_cubePixels[i] != NULL, file_path);

		if (m_nBpp == 24)
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB,
				m_nWidth, m_nHeight, 0, GL_RGB,
				GL_UNSIGNED_BYTE, m_cubePixels[i]);
		else if (m_nBpp == 32)
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA,
				m_nWidth, m_nHeight, 0, GL_RGBA,
				GL_UNSIGNED_BYTE, m_cubePixels[i]);

		// Delete after send to GPU
		SAFE_DEL_ARR(m_cubePixels[i]);
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, m_iMinFilter);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, m_iMagFilter);

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, m_iTileMode);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, m_iTileMode);

	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	m_isLoaded = true;

	LOGE("[LOADED] Cube Texture: %s\n", m_sFilePath);

	return true;
}

bool Texture::load2dTexture()
{	
	unsigned char* m_pData = loadTextureData(m_sFilePath, m_nWidth, m_nHeight, m_nBpp);
	ASSERT_MSG(m_pData != NULL, m_sFilePath);	

	glGenTextures(1, &m_glTexId);
	glBindTexture(GL_TEXTURE_2D, m_glTexId);

	if (m_nBpp == 24)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_nWidth, m_nHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, m_pData);
	}	
	else if (m_nBpp == 32)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_nWidth, m_nHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_pData);
	}	

	if (m_iMinFilter >= GL_NEAREST_MIPMAP_NEAREST && m_iMinFilter <= GL_LINEAR_MIPMAP_LINEAR)
		glGenerateMipmap(GL_TEXTURE_2D);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, m_iMinFilter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, m_iMagFilter);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, m_iTileMode);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, m_iTileMode);

	glBindTexture(GL_TEXTURE_2D, 0);

	m_isLoaded = true;
	SAFE_DEL_ARR(m_pData);

	LOGE("[LOADED] Texture: %s\n", m_sFilePath);

	return true;
}

void Texture::unloadTexture()
{
	SAFE_DEL_ARR(m_pData);		
		
	if (m_glTexId >= 0)
	{
		glDeleteTextures(1, &m_glTexId);
		m_glTexId = -1;
	}	

	if (m_isCubeTexture)
	{
		for (int i = 0; i < CUBE_TOTAL_SIDE; i++)
		{
			SAFE_DEL_ARR(m_cubePixels[i]);
		}
	}

	m_isLoaded = false;
}

GLuint Texture::getGlTexId()
{
	return m_glTexId;
}

int	Texture::getWidth()
{
	return m_nWidth;
}

int	Texture::getHeight()
{
	return m_nHeight;
}

int	Texture::getBpp()
{
	return m_nBpp;
}

bool Texture::isCubeTexture()
{
	return m_isCubeTexture;
}

unsigned char* Texture::loadTextureData(const char* filePath, int& width, int& height, int&bpp)
{
	FILE *f = fopen(filePath, "rb");
	if (!f)
		return false;

	fseek(f, 0, SEEK_END);
	int size = ftell(f);
	rewind(f);

	unsigned char* data_buffer = new unsigned char[size];
	fread(data_buffer, sizeof(unsigned char), size, f);\

	unsigned char* img_data;
	int img_w, img_h, img_bpp;

	if (stbi_tga_test_memory(data_buffer, size))
		img_data = stbi_tga_load_from_memory(data_buffer, size, &img_w, &img_h, &img_bpp, 0);
	else if (stbi_bmp_test_memory(data_buffer, size))
		img_data = stbi_bmp_load_from_memory(data_buffer, size, &img_w, &img_h, &img_bpp, 0);
	else if (stbi_jpeg_test_memory(data_buffer, size))
		img_data = stbi_jpeg_load_from_memory(data_buffer, size, &img_w, &img_h, &img_bpp, 0);
	else if (stbi_png_test_memory(data_buffer, size))
		img_data = stbi_png_load_from_memory(data_buffer, size, &img_w, &img_h, &img_bpp, 0);
	else
	{		
		return NULL;
	}

	width = img_w;
	height = img_h;
	bpp = img_bpp * 8;
	
	//clean up
	free(data_buffer);

	return img_data;
}

bool Texture::CreateTexture2D(GLuint &texture, const char* filePath)
{
	//Automatocally detects the format(from over 20 formats!)	
	unsigned char* img_data = NULL;
	int img_w, img_h, img_bpp;

	img_data = loadTextureData(filePath, img_w, img_h, img_bpp);

	if (img_data == NULL)
		return false;
	
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	
	int textureType = img_bpp == 24 ? GL_RGB : GL_RGBA;
	glTexImage2D(GL_TEXTURE_2D, 0, textureType, img_w, img_h, 0, textureType, GL_UNSIGNED_BYTE, img_data);

	// Enable mip-map by default
	glGenerateMipmap(GL_TEXTURE_2D);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glBindTexture(GL_TEXTURE_2D, 0);

	// Clean-up
	SAFE_DEL_ARR(img_data);

	LOGE("[LOADED] Texture: %s\n", filePath);

	return true;
}