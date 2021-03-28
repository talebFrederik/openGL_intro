#include "ShaderUtils.h"

#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <GL/glew.h>

GLuint loadShaders(const std::string vertexShaderPath, const std::string fragmentShaderPath) 
{

	std::string vertexShaderCode;
	std::string fragmentShaderCode;

	std::ifstream vertexShaderFile{ vertexShaderPath, std::ios_base::in };
	if (vertexShaderFile.is_open())
	{
		std::stringstream vertexShaderStream;
		vertexShaderStream << vertexShaderFile.rdbuf();
		vertexShaderCode = vertexShaderStream.str();
		vertexShaderFile.close();
	}

	std::ifstream fragmentShaderFile{ fragmentShaderPath, std::ios_base::in };
	if (fragmentShaderFile.is_open())
	{
		std::stringstream fragmentShaderStream;
		fragmentShaderStream << fragmentShaderFile.rdbuf();
		fragmentShaderCode = fragmentShaderStream.str();
		fragmentShaderFile.close();
	}

	GLuint vertexShaderId = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);

	char const* vertexShaderPtr = vertexShaderCode.c_str();
	glShaderSource(vertexShaderId, 1, &vertexShaderPtr, NULL);
	glCompileShader(vertexShaderId);

	// Check Vertex Shader
	GLint result = GL_FALSE;
	GLint infoLogLength;

	glGetShaderiv(vertexShaderId, GL_COMPILE_STATUS, &result);
	glGetShaderiv(vertexShaderId, GL_INFO_LOG_LENGTH, &infoLogLength);
	if (infoLogLength > 0) {
		std::vector<char> vertexShaderErrorMessage(infoLogLength + 1);
		glGetShaderInfoLog(vertexShaderId, infoLogLength, NULL, &vertexShaderErrorMessage[0]);
		printf("%s\n", &vertexShaderErrorMessage[0]);
	}

	char const* fragmentShaderPtr = fragmentShaderCode.c_str();
	glShaderSource(fragmentShaderId, 1, &fragmentShaderPtr, NULL);
	glCompileShader(fragmentShaderId);

	GLuint shaderProgramId = glCreateProgram();
	glAttachShader(shaderProgramId, vertexShaderId);
	glAttachShader(shaderProgramId, fragmentShaderId);
	glLinkProgram(shaderProgramId);

	glDetachShader(shaderProgramId, vertexShaderId);
	glDetachShader(shaderProgramId, fragmentShaderId);

	glDeleteShader(vertexShaderId);
	glDeleteShader(fragmentShaderId);

	return shaderProgramId;
}
