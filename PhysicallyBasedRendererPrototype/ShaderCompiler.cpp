#include "ShaderCompiler.h"

// External includes
#include <fstream>
#include <glad/glad.h>
#include <iostream>
#include <sstream>

#define INVALID_SHADER_ID 0

namespace TSFYP
{
	void CheckShaderCompileErrors(const GLuint shaderID);
	void CheckShaderProgramLinkErrors(const GLuint programID);
	GLuint CompileShaderFromFile(const std::string& filePath, const GLint shaderType);

	Shader* CreateShader(const std::string& name, const std::string& vertexPath, const std::string& fragmentPath)
	{
		const GLuint vertexID = CompileShaderFromFile(vertexPath, GL_VERTEX_SHADER);
		const GLuint fragmentID = CompileShaderFromFile(fragmentPath, GL_FRAGMENT_SHADER);

		// Check if any shaders failed to compile
		if (vertexID == INVALID_SHADER_ID || fragmentID == INVALID_SHADER_ID)
		{
			return nullptr;
		}

		// Create shader program using compiled shaders
		const GLuint programID = glCreateProgram();
		glAttachShader(programID, vertexID);
		glAttachShader(programID, fragmentID);
		glLinkProgram(programID);
		CheckShaderProgramLinkErrors(programID);

		// Shaders are now linked into program and can be deleted
		glDetachShader(programID, vertexID);
		glDeleteShader(vertexID);
		glDetachShader(programID, fragmentID);
		glDeleteShader(fragmentID);

		return new Shader(name, programID, vertexPath, fragmentPath);
	}

	void CheckShaderCompileErrors(const GLuint shaderID)
	{
		GLint logLength;
		glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &logLength);

		if (logLength > 0)
		{
			GLchar* infoLog = new GLchar[logLength];
			glGetShaderInfoLog(shaderID, logLength, &logLength, infoLog);
			std::cerr << "Error: Shader compilation failed: " << infoLog << std::endl;
			delete[] infoLog;
		}
	}

	void CheckShaderProgramLinkErrors(const GLuint programID)
	{
		GLint logLength;
		glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &logLength);

		if (logLength > 0)
		{
			GLchar* infoLog = new GLchar[logLength];
			glGetProgramInfoLog(programID, logLength, &logLength, infoLog);
			std::cerr << "Error: Shader program linking failed: " << infoLog << std::endl;
			delete[] infoLog;
		}
	}

	GLuint CompileShaderFromFile(const std::string& filePath, const GLint shaderType)
	{
		if (shaderType != GL_VERTEX_SHADER && shaderType != GL_FRAGMENT_SHADER && shaderType != GL_GEOMETRY_SHADER && shaderType != GL_COMPUTE_SHADER)
		{
			std::cerr << "Invalid shader type requested in CompileShaderFromFile!" << std::endl;
			return INVALID_SHADER_ID;
		}

		std::ifstream inFile(filePath);
		if (!inFile.good())
		{
			std::cerr << "Failed to open shader file at: " << filePath << std::endl;
			return INVALID_SHADER_ID;
		}

		std::stringstream shaderCodeStream;
		shaderCodeStream << inFile.rdbuf();
		std::string shaderCodeString = shaderCodeStream.str();
		const GLchar* shaderCode = shaderCodeString.c_str();

		const GLuint id = glCreateShader(shaderType);
		glShaderSource(id, 1, &shaderCode, NULL);
		glCompileShader(id);
		CheckShaderCompileErrors(id);

		inFile.close();

		return id;
	}
}