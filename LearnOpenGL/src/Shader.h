#pragma once

#include <GL/glew.h>

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

	//Set uniforms
	void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);

private:
	ShaderProgramSource ParseShader(const std::string& file_path);
	unsigned int CompileShader(unsigned int type, const std::string& source);
	unsigned int CreateShader(const std::string& vertex_shader, const std::string& fragment_shader);
	
	unsigned int GetUniformLocation(const std::string& name);
};