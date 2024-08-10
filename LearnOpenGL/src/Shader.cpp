#include "Shader.h"
#include "Renderer.h"

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>


//Constructor
Shader::Shader(const std::string& file_path)
	: filepath(file_path), rendererID(0)
{
    ShaderProgramSource sourceShader = ParseShader(file_path);
    rendererID = CreateShader(sourceShader.vertexSource, sourceShader.fragmentSource);
}

//Destructor
Shader::~Shader()
{
    glErrorCall( glDeleteProgram(rendererID) );
}


void Shader::Bind() const
{
    glErrorCall( glUseProgram(rendererID) );
}


void Shader::Unbind() const
{
    glErrorCall( glUseProgram(0) );
}


//Setting the uniform's value (in 1 int) in shader source code
void Shader::SetUniform1i(const std::string& name, int value)
{
    glErrorCall( glUniform1i(GetUniformLocation(name), value) );
}

//Setting the uniform's value (in 1 matrix) in shader source code
void Shader::SetUniformMat4f(const std::string& name, const glm::mat4 matrix)
{
    glErrorCall( glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &matrix[0][0]) );

}


//Dividing the source code in shaders so that they can easily be read separately
ShaderProgramSource Shader::ParseShader(const std::string& file_path)
{
    std::ifstream stream(file_path);

    enum class ShaderType {
        NONE = -1, VERTEX = 0, FRAGMENT = 1
    };

    std::string line;
    std::stringstream ss[2];
    ShaderType type = ShaderType::NONE;

    //Reading the shader source file and using keywords to split it
    while (getline(stream, line))
    {
        if (line.find("#shader") != std::string::npos)
        {
            if (line.find("vertex") != std::string::npos)
                type = ShaderType::VERTEX;
            else if (line.find("fragment") != std::string::npos)
                type = ShaderType::FRAGMENT;
        }
        else
            ss[(int)type] << line << '\n';
    }

    return { ss[0].str(), ss[1].str() };
}


//Compiling shader source code
unsigned int Shader::CompileShader(unsigned int type, const std::string& source)
{
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();       // Convert the source string to a C-style string
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    //Error Handling
    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE)
    {
        int len;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &len);

        std::unique_ptr<char[]> message = std::make_unique<char[]>(len);
        //char* message = (char*)alloca(len * sizeof(char));        //Alternate line for message pointer (normal pointer instead of smart)

        glGetShaderInfoLog(id, len, &len, message.get());
        std::cout << "ERROR::Application.cpp::CompileShader():: Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader" << std::endl;
        std::cout << message.get() << std::endl;
        glDeleteShader(id);
        return 0;
    }

    return id;
}


//Creating the shader in a program after compiling the source code
unsigned int Shader::CreateShader(const std::string& vertex_shader, const std::string& fragment_shader)
{
    unsigned int program = glCreateProgram();
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertex_shader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragment_shader);

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}


//Getting the uniform's location from shader source code
int Shader::GetUniformLocation(const std::string& name)
{
    if (uLocationCache.find(name) != uLocationCache.end())
        return uLocationCache[name];

    glErrorCall(int location = glGetUniformLocation(rendererID, name.c_str()));
    if (location == -1)
        std::cout << "WARNGING::Shader.cpp: Unfirom '" << name << "' doesn't exist" << std::endl;
    
    uLocationCache[name] = location;
    return location;
}