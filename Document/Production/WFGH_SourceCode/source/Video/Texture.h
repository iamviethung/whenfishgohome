#pragma once

//define use for cube texture
#define CUBE_TOTAL_SIDE 6

class Texture
{
protected:	
	char	m_sFilePath[BUFFER_SIZE];

	GLint	m_iTileMode;
	GLint	m_iMinFilter;
	GLint	m_iMagFilter;

	GLuint	m_glTexId;

	unsigned char*	m_pData;

	bool			m_isCubeTexture;
	unsigned char*	m_cubePixels[CUBE_TOTAL_SIDE];

	int		m_nWidth;
	int		m_nHeight;
	int		m_nBpp;

	bool	m_isLoaded;

private:	
	bool			load2dTexture();
	bool			loadCubeTexture();

	static unsigned char*	loadTextureData(const char* filePath, int& width, int& height, int&bpp);

public:
	Texture(const char* filePath, bool isCubeTexture = false);
	~Texture();

	virtual bool	loadTexture();
	virtual void	unloadTexture();

	int		getId();
	GLuint	getGlTexId();

	int		getWidth();
	int		getHeight();
	int		getBpp();

	bool	isCubeTexture();

	void	setTiledMode(GLint mode) { m_iTileMode = mode; }

	static bool CreateTexture2D(GLuint &texture, const char* filePath);
};