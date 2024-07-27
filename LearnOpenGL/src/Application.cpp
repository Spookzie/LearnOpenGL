#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <memory>

#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"


//Struct to handle source code for shaders
struct ShaderProgramSource
{
    std::string vertexSource;
    std::string fragmentSource;
};


//Dividing the source code in shaders so that they can easily be read separately
static ShaderProgramSource ParseShader(const std::string& file_path)
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


//Shader compilation 
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

        std::unique_ptr<char[]> message = std::make_unique<char[]>(len);
        //char* message = (char*)alloca(len * sizeof(char));        //Alternate line for message pointer (normal pointer instead of smart)

        glGetShaderInfoLog(id, len, &len, message.get());
        std::cout << "ERROR::Application.cpp::CompileShader():: Failed to compile "<< (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader" << std::endl;
        std::cout << message.get() << std::endl;
        glDeleteShader(id);
        return 0;
    }

    return id;
}


//Creating the shader in a program after compiling them
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

    //Setting OpenGL version & profile
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);  //Setting OpenGL version: 3._
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);  //Setting OpenGL version: 3.3
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  


    //Creating Window
    window = glfwCreateWindow(640, 480, "Triangle", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        std::cout << "ERROR::Application.cpp::Main():: Failed to create a window" << std::endl;
    }


    //Make the window's context current i.e. basically activates the window and makes sure all further changes are made on it
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);    //Enabling v-sync

    //Initializing GLEW
    if (glewInit() != GLEW_OK)
        std::cout << "ERROR::Application.cpp::Main():: Failed to initialize GLEW" << std::endl;

    std::cout << glGetString(GL_VERSION) << std::endl;

    {
        //Setting vertex positions for a square that is being made using 2 triangles- lower right & upper left
        float positions[] = {
            -0.5f, -0.5f,   //Lower left    : index-0
             0.5f, -0.5f,   //Lower right 
             0.5f,  0.5f,   //Upper right 
            -0.5f,  0.5f    //Upper left    : index-3
        };

        unsigned int indices[] = {
            0, 1, 2,    //Accessing vertices for lower right triangle
            2, 3, 0     //for upper left triangle
        };

        //Handling vertex array objects
        unsigned int vao;
        glErrorCall(glGenVertexArrays(1, &vao));
        glErrorCall(glBindVertexArray(vao));


        VertexBuffer vb(positions, 4 * 2 * sizeof(float));

        //Enabling & specifying vertex attributes
        glErrorCall(glEnableVertexAttribArray(0));
        glErrorCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0));

        IndexBuffer ib(indices, 6);

        ShaderProgramSource sourceShader = ParseShader("res/shaders/BaseShader.shader");
        unsigned int shader = CreateShader(sourceShader.vertexSource, sourceShader.fragmentSource);
        glErrorCall(glUseProgram(shader));

        //Setting up color vector uniform
        glErrorCall(int uColorLoc = glGetUniformLocation(shader, "u_Color"));
        ASSERT(uColorLoc != -1);    //uColorLoc will be -1 if above line isn't able to find it, so this is just a bit of error handling
        glErrorCall(glUniform4f(uColorLoc, 1.0f, 0.0f, 0.0f, 1.0f));

        //Unbinding all buffers
        glErrorCall(glBindVertexArray(0));
        glErrorCall(glUseProgram(0));
        glErrorCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
        glErrorCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));

        //Variables for color changing mechanism
        float g = 0.0f;
        float inc = 0.05f;


        //Game Loop 
        while (!glfwWindowShouldClose(window))
        {
            //  RENDER HERE  //
            glErrorCall(glClear(GL_COLOR_BUFFER_BIT));

            glErrorCall(glUseProgram(shader));
            glErrorCall(glUniform4f(uColorLoc, 0.4f, g, 0.8f, 1.0f));

            glErrorCall(glBindVertexArray(vao));
            ib.Bind();

            //Drawing triangle
            glErrorCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));

            //Color change loguc
            if (g > 1.0f)   inc = -0.05f;
            else if (g < 0.0f)   inc = 0.05f;
            g += inc;


            //Swaping front and back buffers
            glfwSwapBuffers(window);


            //Event polling
            glfwPollEvents();
        }

        glDeleteProgram(shader);
    }

    glfwTerminate();
    
    return 0;
}