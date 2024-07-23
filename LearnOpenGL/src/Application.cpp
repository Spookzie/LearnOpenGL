#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <memory>


struct ShaderProgramSource
{
    std::string vertexSource;
    std::string fragmentSource;
};


static ShaderProgramSource ParseShader(const std::string& file_path)
{
    std::ifstream stream(file_path);

    enum class ShaderType {
        NONE = -1, VERTEX = 0, FRAGMENT = 1
    };

    std::string line;
    std::stringstream ss[2];
    ShaderType type = ShaderType::NONE;

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


static unsigned int CompileShader(unsigned int type, const std::string& source)
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

        //char* message = (char*)alloca(len * sizeof(char));  // We did not use malloc becuase that requires manual deallocation, while alloca does it automatically
        std::unique_ptr<char[]> message = std::make_unique<char[]>(len);
        
        glGetShaderInfoLog(id, len, &len, message.get());
        std::cout << "ERROR::Application.cpp::CompileShader():: Failed to compile "<< (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader" << std::endl;
        std::cout << message.get() << std::endl;
        glDeleteShader(id);
        return 0;
    }

    return id;
}


static unsigned int CreateShader(const std::string& vertex_shader, const std::string& fragment_shader)
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


int main(void)
{
    GLFWwindow* window;

    //Initializing GLFW
    if (!glfwInit())
        std::cout << "ERROR::Application.cpp::Main():: Failed to initialize glfw" << std::endl;


    //Creating Window
    window = glfwCreateWindow(640, 480, "Triangle", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        std::cout << "ERROR::Application.cpp::Main():: Failed to create a window" << std::endl;
    }


    //Make the window's context current
    glfwMakeContextCurrent(window);


    //Initializing GLEW
    if (glewInit() != GLEW_OK)
        std::cout << "ERROR::Application.cpp::Main():: Failed to initialize GLEW" << std::endl;


    //Setting up a vertex buffer for triangle
    float positions[] = {
        -0.5f, -0.5f,
         0.5f, -0.5f,
         0.5f,  0.5f,

         0.5f,  0.5f,
        -0.5f,  0.5f,
        -0.5f, -0.5f
    };
    
    unsigned int buffer;
    glGenBuffers(1, &buffer);       //Generating a buffer
    glBindBuffer(GL_ARRAY_BUFFER, buffer);      //Binding a buffer with a buffer target i.e. what we want the buffer to be ig
    glBufferData(GL_ARRAY_BUFFER, 6 * 2 * sizeof(float), positions, GL_STATIC_DRAW);    //Giving buffer the data
    
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);

    ShaderProgramSource sourceShader = ParseShader("res/shaders/BaseShader.shader");
    unsigned int shader = CreateShader(sourceShader.vertexSource, sourceShader.fragmentSource);
    glUseProgram(shader);


    //Game Loop 
    while (!glfwWindowShouldClose(window))
    {
        //  RENDER HERE  //
        glClear(GL_COLOR_BUFFER_BIT);

        //Drawing Triangle
        glDrawArrays(GL_TRIANGLES, 0, 6);


        //Swaping front and back buffers
        glfwSwapBuffers(window);

        //Event polling
        glfwPollEvents();
    }

    glDeleteProgram(shader);

    glfwTerminate();
    return 0;
}