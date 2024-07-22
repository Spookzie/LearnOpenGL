#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>


static unsigned int CompileShader(unsigned int type, const std::string& source)
{
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();       // Convert the source string to a C-style string, can also use ' &source[0] '
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    //Error Handling
    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE)
    {
        int len;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &len);
        char* message = (char*)alloca(len * sizeof(char));  // We did not use malloc becuase that requires manual deallocation, while alloca does it automatically
        glGetShaderInfoLog(id, len, &len, message);
        std::cout << "ERROR::Application.cpp::CompileShader():: Failed to compile"<< (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << "shader" << std::endl;
        std::cout << message << std::endl;
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


void main()
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
    float positions[6] = {
        -0.5f, -0.5f,
         0.0f , 0.5f,
         0.5f, -0.5f 
    };
    
    unsigned int buffer;
    glGenBuffers(1, &buffer);       //Generating a buffer
    glBindBuffer(GL_ARRAY_BUFFER, buffer);      //Binding a buffer with a buffer target i.e. what we want the buffer to be ig
    glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), positions, GL_STATIC_DRAW);    //Giving buffer the data
    
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);

    //Source code for the vertex shader
    std::string vertexShader =
        "#version 330 core\n"
        ""
        "layout(location = 0) in vec4 position;\n"
        ""
        "void main()\n"
        "{\n"
        "   gl_Position = position;\n"
        "}\n";
    
    //Source code for the fragment shader
    std::string fragmentShader =
        "#version 330 core\n"
        ""
        "layout(location = 0) out vec4 color;\n"
        ""
        "void main()\n"
        "{\n"
        "   color = vec4(1.0, 0.0, 0.0, 1.0);\n"
        "}\n";

    unsigned int shader = CreateShader(vertexShader, fragmentShader);
    glUseProgram(shader);


    //Game Loop 
    while (!glfwWindowShouldClose(window))
    {
        //  RENDER HERE  //
        glClear(GL_COLOR_BUFFER_BIT);

        //Drawing Triangle
        glDrawArrays(GL_TRIANGLES, 0, 3);


        //Swaping front and back buffers
        glfwSwapBuffers(window);

        //Event polling
        glfwPollEvents();
    }

    glDeleteProgram(shader);

    glfwTerminate();
}