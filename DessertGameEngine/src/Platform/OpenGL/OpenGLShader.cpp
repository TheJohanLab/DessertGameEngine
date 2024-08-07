#include "dgepch.h"
#include "OpenGLShader.h"

#include <fstream>

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

namespace Dessert {

	static GLenum ShaderTypeFromString(const std::string& type)
	{
		if (type == "vertex")
			return GL_VERTEX_SHADER;
		if (type == "fragment" || type == "pixel")
			return GL_FRAGMENT_SHADER;

		DGE_CORE_ASSERT(false, "Unknown shader type");
		return 0;
	}

	OpenGLShader::OpenGLShader(const std::string& filepath)
	{
		DGE_PROFILE_FUNCTION();

		std::string source = ReadFile(filepath);
		auto shaderSources = PreProcess(source);
		Compile(shaderSources);

		//assets/shaders/Texture.glsl
		auto lastSlash = filepath.find_last_of("/\\");
		lastSlash = lastSlash == std::string::npos ? 0 : lastSlash + 1;
		auto lastDot = filepath.rfind('.'); //Equivalent to find_last_of but find only this char instead of any of the char for find_last_of
		auto count = lastDot == std::string::npos ? filepath.size() - lastSlash : lastDot - lastSlash;
		m_Name = filepath.substr(lastSlash, count);
	

	}

	OpenGLShader::OpenGLShader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc)
		:m_Name(name)
	{
		DGE_PROFILE_FUNCTION();

		std::unordered_map<GLenum, std::string> sources;
		sources[GL_VERTEX_SHADER] = vertexSrc;
		sources[GL_FRAGMENT_SHADER] = fragmentSrc;
		Compile(sources);
	}

	OpenGLShader::~OpenGLShader()
	{
		DGE_PROFILE_FUNCTION();

		glDeleteProgram(m_RendererId);
	}


	std::string OpenGLShader::ReadFile(const std::string& filepath)
	{
		DGE_PROFILE_FUNCTION();

		std::string result;
		std::ifstream in(filepath, std::ios::in | std::ios::binary);
		if (in)
		{
			//0 = offset
			in.seekg(0, std::ios::end); // seek the end of the file
			result.resize(in.tellg()); // Resize the string to the size of the file (using the end of the file)
			in.seekg(0, std::ios::beg);
			in.read(&result[0], result.size());
			in.close();
		}
		else
		{
			DGE_CORE_ERROR("Could not open file '{0}'", filepath);
		}

		return result;
	}

	std::unordered_map<GLenum, std::string> OpenGLShader::PreProcess(const std::string& source)
	{
		DGE_PROFILE_FUNCTION();

		std::unordered_map<GLenum, std::string> shaderSources;

		const char* typeToken = "#type";
		size_t typeTokenLength = strlen(typeToken);
		size_t pos = source.find(typeToken, 0);
		while (pos != std::string::npos)
		{
			size_t eol = source.find_first_of("\r\n", pos);
			DGE_CORE_ASSERT(eol != std::string::npos, "Syntax error");

			size_t begin = pos + typeTokenLength + 1;
			std::string type = source.substr(begin, eol - begin);
			DGE_CORE_ASSERT(ShaderTypeFromString(type), "Invalid shader type specifier");

			size_t nextLinePos = source.find_first_not_of("\r\n", eol);
			pos = source.find(typeToken, nextLinePos);
			shaderSources[ShaderTypeFromString(type)] =
				source.substr(nextLinePos,
					pos - (nextLinePos == std::string::npos ? source.size() - 1 : nextLinePos));

		}

		return shaderSources;
	}

	void OpenGLShader::Compile(const std::unordered_map<GLenum, std::string>& shaderSources)
	{
		DGE_PROFILE_FUNCTION();

		// Vertex and fragment shaders are successfully compiled.
		// Now time to link them together into a program.
		// Get a program object.
		GLuint program = glCreateProgram();
		DGE_CORE_ASSERT(shaderSources.size() <= 2, "Only 2 shaders are supported");
		std::array<GLenum, 2> glShadersIDs;

		int glShaderIDIndex = 0;
		for (auto& keyValue : shaderSources)
		{
			GLenum shaderType = keyValue.first;
			const std::string& source = keyValue.second;

			// Create an empty vertex shader handle
			GLuint shader = glCreateShader(shaderType);

			// Send the vertex shader source code to GL
// Note that std::string's .c_str is NULL character terminated.
			const GLchar* sourceCStr = source.c_str();
			glShaderSource(shader, 1, &sourceCStr, 0);

			// Compile the vertex shader
			glCompileShader(shader);

			GLint isCompiled = 0;
			glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
			if (isCompiled == GL_FALSE)
			{
				GLint maxLength = 0;
				glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

				// The maxLength includes the NULL character
				std::vector<GLchar> infoLog(maxLength);
				glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);

				// We don't need the shader anymore.
				glDeleteShader(shader);

				DGE_CORE_ERROR("{0}", infoLog.data());
				DGE_CORE_ASSERT(false, "Shader compilation failure");

				break;
			}

			// Attach our shaders to our program
			glAttachShader(program, shader);
			glShadersIDs[glShaderIDIndex++] = shader;
		}

		
		// Link our program
		glLinkProgram(program);

		// Note the different functions here: glGetProgram* instead of glGetShader*.
		GLint isLinked = 0;
		glGetProgramiv(program, GL_LINK_STATUS, (int*)&isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

			// We don't need the program anymore.
			glDeleteProgram(program);
			// Don't leak shaders either.
			for (auto id : glShadersIDs)
				glDeleteShader(id);
			

			DGE_CORE_ERROR("{0}", infoLog.data());
			DGE_CORE_ASSERT(false, "Shader link failure");

			return;
		}

		// Always detach shaders after a successful link.
		for (auto id : glShadersIDs)
			glDetachShader(program, id);

		m_RendererId = program;
	}


	void OpenGLShader::Bind() const
	{
		DGE_PROFILE_FUNCTION();

		glUseProgram(m_RendererId);
	}

	void OpenGLShader::Unbind() const
	{
		DGE_PROFILE_FUNCTION();

		glUseProgram(0);
	}
	void OpenGLShader::SetMat4(const char* name, const glm::mat4& value)
	{
		DGE_PROFILE_FUNCTION();

		setUniformMat4(name, value);
	}

	void OpenGLShader::SetInt(const char* name, int value)
	{
		DGE_PROFILE_FUNCTION();

		setUniformInt(name, value);
	}

	void OpenGLShader::SetIntArray(const char* name, int* values, uint32_t count)
	{
		DGE_PROFILE_FUNCTION();

		setUniformIntArray(name, values, count);
	}

	void OpenGLShader::SetFloat(const char* name, float value)
	{
		DGE_PROFILE_FUNCTION();

		setUniformFloat(name, value);
	}

	void OpenGLShader::SetFloat2(const char* name, const glm::vec2& value)
	{
		DGE_PROFILE_FUNCTION();

		setUniformFloat2(name, value);
	}

	void OpenGLShader::SetFloat3(const char* name, const glm::vec3& value)
	{
		DGE_PROFILE_FUNCTION();

		setUniformFloat3(name, value);
	}

	void OpenGLShader::SetFloat4(const char* name, const glm::vec4& value)
	{
		DGE_PROFILE_FUNCTION();

		setUniformFloat4(name, value);
	}

	void OpenGLShader::setUniformMat4(const char* name, const glm::mat4& matrix) const
	{
		glUniformMatrix4fv(glGetUniformLocation(m_RendererId, name), 1, GL_FALSE, glm::value_ptr(matrix));
	}

	void OpenGLShader::setUniformInt(const char* name, int value) const
	{
		glUniform1i(glGetUniformLocation(m_RendererId, name), value);
	}

	void OpenGLShader::setUniformIntArray(const char* name, int* values, uint32_t count) const
	{
		glUniform1iv(glGetUniformLocation(m_RendererId, name), count, values);
	}

	void OpenGLShader::setUniformFloat(const char* name, float value) const
	{
		glUniform1f(glGetUniformLocation(m_RendererId, name), value);
	}

	void OpenGLShader::setUniformFloat2(const char* name, const glm::vec2& values) const
	{
		glUniform2f(glGetUniformLocation(m_RendererId, name), values.x, values.y);
	}

	void OpenGLShader::setUniformFloat3(const char* name, const glm::vec3& values) const
	{
		glUniform3f(glGetUniformLocation(m_RendererId, name), values.x, values.y, values.z);
	}

	void OpenGLShader::setUniformFloat4(const char* name, const glm::vec4& values) const
	{
		glUniform4f(glGetUniformLocation(m_RendererId, name), values.x, values.y, values.z, values.w);
	}


}