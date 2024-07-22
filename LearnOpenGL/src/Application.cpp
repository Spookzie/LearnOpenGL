#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>


void main()
{
    GLFWwindow* window;

    //Initializing GLFW
    if (!glfwInit())
        std::cout << "ERROR::Application.cpp:: Failed to initialize glfw" << std::endl;


    //Creating Window
    window = glfwCreateWindow(640, 480, "Triangle", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        std::cout << "ERROR::Application.cpp:: Failed to create a window" << std::endl;
    }


    //Make the window's context current
    glfwMakeContextCurrent(window);


    //Initializing GLEW
    if (glewInit() != GLEW_OK)
        std::cout << "ERROR:: Failed to initialize GLEW" << std::endl;


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


    glfwTerminate();
}