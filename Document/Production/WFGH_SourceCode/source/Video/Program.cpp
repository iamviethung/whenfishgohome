#include "stdafx.h"

#include "Program.h"


Program::Program(const char* vShaderPath, const char* fShaderPath) :
						m_isLoaded(false)
{	
	strcpy(m_sVertFilePath, vShaderPath);
	strcpy(m_sFragFilePath, fShaderPath);	
}

int Program::loadShaders()
{
	if (m_isLoaded)
		return 0;

	m_glVertexShader = LoadShader(GL_VERTEX_SHADER, m_sVertFilePath);

	if (m_glVertexShader == 0)
		return -1;

	m_glFragmentShader = LoadShader(GL_FRAGMENT_SHADER, m_sFragFilePath);

	if (m_glFragmentShader == 0)
		return -2;

	m_glProgram = LoadProgram(m_glVertexShader, m_glFragmentShader);

	m_isLoaded = true;

	LOGE("[LOADED] Vertex Shader: %s\n", m_sVertFilePath);
	LOGE("[LOADED] Fragment Shader: %s\n", m_sFragFilePath);

	return 0;
}

void Program::unloadShaders()
{
	//TODO: implement if need
	if (m_isLoaded)
	{
		glDeleteShader(m_glVertexShader);
		glDeleteShader(m_glFragmentShader);
		glDeleteProgram(m_glProgram);

		m_isLoaded = false;
	}
}

Program::~Program()
{
	if (m_isLoaded)
		unloadShaders();
}

GLint Program::getAttrib(const char* name)
{
	return glGetAttribLocation(m_glProgram, name);
}

GLint Program::getUniform(const char* name)
{
	return glGetUniformLocation(m_glProgram, name);
}

GLuint	Program::getProgramId()
{
	return m_glProgram;
}

/////
// private static func
/// \brief Load a shader, check for compile errors, print error messages to output log
/// \param type Type of shader (GL_VERTEX_SHADER or GL_FRAGMENT_SHADER)
/// \param Programrc Shader source string
/// \return A new shader object on success, 0 on failure
//
GLuint Program::LoadShader ( GLenum type, char * filename )
{
	GLuint shader;
	GLint compiled;

	// Create the shader object
	shader = glCreateShader ( type );

	if ( shader == 0 )
	return 0;

	// Load the shader source
	FILE * pf = fopen(filename, "rb");

	if (!pf)
		return NULL;
	fseek(pf, 0, SEEK_END);
	long size = ftell(pf);
	fseek(pf, 0, SEEK_SET);

	char * shader_src = new char[size + 1];
	fread(shader_src, sizeof(char), size, pf);
	shader_src[size] = 0;
	fclose(pf);
		
	glShaderSource(shader, 1, (const char **)&shader_src, NULL);
	delete[] shader_src;

	// Compile the shader
	glCompileShader ( shader );

	// Check the compile status
	glGetShaderiv ( shader, GL_COMPILE_STATUS, &compiled );

	if ( !compiled ) 
	{
		GLint infoLen = 0;

		glGetShaderiv ( shader, GL_INFO_LOG_LENGTH, &infoLen );

		if ( infoLen > 1 )
		{
			char* infoLog = new char  [infoLen];


			glGetShaderInfoLog ( shader, infoLen, NULL, infoLog );
			LOGI ( "Error compiling shader:\n%s\n", infoLog );            

			delete [] infoLog;
		}

		glDeleteShader ( shader );
		return 0;
	}

	return shader;
}

/// \brief Create a program object, link program.
//         Errors output to log.
/// \param vertProgramrc Vertex shader source code
/// \param fragProgramrc Fragment shader source code
/// \return A new program object linked with the vertex/fragment shader pair, 0 on failure

GLuint Program::LoadProgram ( GLuint vertexShader, GLuint fragmentShader )
{
	GLuint programObject;
	GLint linked;

	// Create the program object
	programObject = glCreateProgram ( );

	if ( programObject == 0 )
		return 0;

	glAttachShader ( programObject, vertexShader );
	glAttachShader ( programObject, fragmentShader );

	// Link the program
	glLinkProgram ( programObject );

	// Check the link status
	glGetProgramiv ( programObject, GL_LINK_STATUS, &linked );

	if ( !linked ) 
	{
		GLint infoLen = 0;

		glGetProgramiv ( programObject, GL_INFO_LOG_LENGTH, &infoLen );

		if ( infoLen > 1 )
		{
			char* infoLog = new char[sizeof(char) * infoLen];


			glGetProgramInfoLog ( programObject, infoLen, NULL, infoLog );
			LOGI ( "Error linking program:\n%s\n", infoLog );            

			delete infoLog;
		}

		glDeleteProgram ( programObject );
		return 0;
	}

	return programObject;
}

void Program::enableState(GLenum state)
{
	glEnable(state);
}

void Program::disableState(GLenum state)
{
	glDisable(state);
}