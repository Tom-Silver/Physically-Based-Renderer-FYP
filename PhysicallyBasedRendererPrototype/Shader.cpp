#include "Shader.h"

// External includes
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

namespace TSFYP
{
	Shader::Shader(const std::string& name, const unsigned int programID, const std::string& vertexPath, const std::string& fragmentPath)
		: mName(name)
		, mID(programID)
		, mVertexPath(vertexPath)
		, mFragmentPath(fragmentPath)
	{}

	Shader::~Shader()
	{
		glDeleteProgram(mID);
	}

	void Shader::Use() const
	{
		glUseProgram(mID);
	}

	void Shader::Unuse() const
	{
		glUseProgram(0);
	}

	void Shader::SetUniform(const std::string& name, const bool value) const
	{
		GLint location = glGetUniformLocation(mID, name.c_str());
		if (location == -1) { return; }
		glUniform1i(location, value);
	}

	void Shader::SetUniform(const std::string& name, const int value) const
	{
		GLint location = glGetUniformLocation(mID, name.c_str());
		if (location == -1) { return; }
		glUniform1i(location, value);
	}

	void Shader::SetUniform(const std::string& name, const unsigned int value) const
	{
		GLint location = glGetUniformLocation(mID, name.c_str());
		if (location == -1) { return; }
		glUniform1ui(location, value);
	}

	void Shader::SetUniform(const std::string& name, const float value) const
	{
		GLint location = glGetUniformLocation(mID, name.c_str());
		if (location == -1) { return; }
		glUniform1f(location, value);
	}

	void Shader::SetUniform(const std::string& name, const glm::vec2& value) const
	{
		GLint location = glGetUniformLocation(mID, name.c_str());
		if (location == -1) { return; }
		glUniform2f(location, value.x, value.y);
	}

	void Shader::SetUniform(const std::string& name, const glm::vec3& value) const
	{
		GLint location = glGetUniformLocation(mID, name.c_str());
		if (location == -1) { return; }
		glUniform3f(location, value.x, value.y, value.z);
	}

	void Shader::SetUniform(const std::string& name, const glm::vec4& value) const
	{
		GLint location = glGetUniformLocation(mID, name.c_str());
		if (location == -1) { return; }
		glUniform4f(location, value.x, value.y, value.z, value.w);
	}

	void Shader::SetUniform(const std::string& name, const glm::mat4& value) const
	{
		GLint location = glGetUniformLocation(mID, name.c_str());
		if (location == -1) { return; }
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
	}
}