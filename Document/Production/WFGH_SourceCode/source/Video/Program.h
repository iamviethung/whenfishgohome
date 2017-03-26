#ifndef _SHADER_H_
#define _SHADER_H_

class Program
{	
	char		m_sVertFilePath[BUFFER_SIZE];
	char		m_sFragFilePath[BUFFER_SIZE];

	GLuint		m_glProgram;
	GLuint		m_glVertexShader;
	GLuint		m_glFragmentShader;

	bool		m_isLoaded;

public:	
	Program(const char* vShaderPath, const char* fShaderPath);
	~Program();

	int		loadShaders();
	void	unloadShaders();

	int		getAttrib(const char* name);
	int		getUniform(const char* name);

	GLuint	getProgramId();

	inline bool	isLoad()	{ return m_isLoaded; }

	void	enableState(GLenum state);
	void	disableState(GLenum state);

private:
	static GLuint LoadShader ( GLenum type, char * filename );
	static GLuint LoadProgram ( GLuint vertexShader, GLuint fragmentShader );	
};

#endif