#pragma once

// External includes
#include <glm/glm.hpp>
#include <string>

namespace TSFYP
{
	struct Vector3;

	class Shader
	{
	public:
		Shader(const std::string& name, const unsigned int programID, const std::string& vertexPath, const std::string& fragmentPath);
		~Shader();

		void Use() const;
		void Unuse() const;

		void CreateGui();

		void SetUniform(const std::string& name, const bool value) const;
		void SetUniform(const std::string& name, const int value) const;
		void SetUniform(const std::string& name, const unsigned int value) const;
		void SetUniform(const std::string& name, const float value) const;
		void SetUniform(const std::string& name, const glm::vec2& value) const;
		void SetUniform(const std::string& name, const glm::vec3& value) const;
		void SetUniform(const std::string& name, const glm::vec4& value) const;
		void SetUniform(const std::string& name, const glm::mat4& value) const;

		const std::string mName;
		const std::string mVertexPath, mFragmentPath;

	private:
		// Not copyable
		Shader(const Shader&) = delete;
		Shader& operator=(Shader&) = delete;

		const unsigned int mID;
	};
}