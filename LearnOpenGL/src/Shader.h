#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

#include <string>
#include <unordered_map>


//Struct to handle source code for shaders
struct ShaderProgramSource
{
	std::string vertexSource;
	std::string fragmentSource;
};


class Shader
{
private:
	std::string filepath;
	unsigned int rendererID;
	
	//Caching for uniforms
	std::unordered_map<std::string, int> uLocationCache;

public:
	//Constructor & Destructor
	Shader(const std::string& file_path);
	~Shader();

	void Bind() const;
	void Unbind() const;

	//Set uniforms' value(s)
	void SetUniform1i(const std::string& name, int value);
	void SetUniformMat4f(const std::string& name, const glm::mat4 matrix);

private:
	ShaderProgramSource ParseShader(const std::string& file_path);
	unsigned int CompileShader(unsigned int type, const std::string& source);
	unsigned int CreateShader(const std::string& vertex_shader, const std::string& fragment_shader);
	
	int GetUniformLocation(const std::string& name);
};