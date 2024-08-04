#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <memory>

#include "Renderer.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Texture.h"


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
    window = glfwCreateWindow(1280, 720, "Loading Texture", NULL, NULL);
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
        /*
        Setting vertex positions & texture coordinates for texture.
        We make the rectangle of texture by joining 2 triangles- an upper left & a lower right.
        The first 2 floats of each vector define the position of each vertex to be drawn in window.
        The next 2 floats define the texture coordinates for their resp. vertex.
        */
        float positions[] = {
            200.0f, 200.0f, 0.0f, 0.0f, //Lower left    : index-0
            600.0f, 200.0f, 1.0f, 0.0f, //Lower right 
            600.0f, 600.0f, 1.0f, 1.0f, //Upper right 
            200.0f, 600.0f, 0.0f, 1.0f  //Upper left    : index-3
        };

        unsigned int indices[] = {
            0, 1, 2,    //Accessing vertices for lower right triangle
            2, 3, 0     //for upper left triangle
        };

        //Blending
        glErrorCall( glEnable(GL_BLEND) );
        glErrorCall( glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA) );

        //Handling vertex array & buffers
        VertexArray va;
        VertexBuffer vb(positions, 4 * 4 * sizeof(float));
        VertexBufferLayout layout;
        layout.Push<float>(2);
        layout.Push<float>(2);
        va.AddBuffer(vb, layout);

        IndexBuffer ib(indices, 6);

        //MVP (model view projection)
        glm::mat4 proj = glm::ortho(0.0f, 1280.0f, 0.0f, 720.0f /*-1.0f, 1.0f*/);   //Projection matrix (orthographic)
        glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(-100, 0, 0));    //View matrix
        glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(100, 100, 0));  //Model matrix
        glm::mat4 mvp = proj * view * model;

        //Setting up shader
        Shader shader("res/shaders/BaseShader.shader");
        shader.Bind();
        shader.SetUniform4f("u_Color", 1.0f, 0.0f, 0.0f, 1.0f);
        shader.SetUniformMat4f("u_MVP", mvp);

        //Setting up texture
        Texture texture("res/textures/Spookzie_Logo.png");
        texture.Bind();
        shader.SetUniform1i("u_Texture", 0);

        //Unbinding all buffers
        va.Unbind();
        shader.Unbind();
        vb.Unbind();
        ib.Unbind();

        Renderer renderer;

        //Variables for color changing mechanism
        float g = 0.0f;
        float inc = 0.05f;


        //Game Loop 
        while (!glfwWindowShouldClose(window))
        {
            //  RENDER HERE  //
            renderer.Clear();

            shader.Bind();
            shader.SetUniform4f("u_Color", 0.4f, g, 0.8f, 1.0f);

            renderer.Draw(va, ib, shader);

            //Color change loguc
            if (g > 1.0f)   inc = -0.05f;
            else if (g < 0.0f)   inc = 0.05f;
            g += inc;


            //Swaping front and back buffers
            glfwSwapBuffers(window);

            //Event polling
            glfwPollEvents();
        }
    }

    glfwTerminate();
    
    return 0;
}