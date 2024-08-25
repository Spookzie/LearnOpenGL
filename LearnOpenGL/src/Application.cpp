#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw_gl3.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <memory>

#include "tests/TestClearColor.h"
#include "tests/TestTexture2D.h"


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
        glErrorCall( glEnable(GL_BLEND) );
        glErrorCall( glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA) );

        Renderer renderer;


        //  ImGui   //
        ImGui::CreateContext();
        ImGui_ImplGlfwGL3_Init(window, true);
        ImGui::StyleColorsDark();


        //  Testing //
        //Creating test menu
        test::Test* currentTest = nullptr;
        test::TestMenu* testMenu = new test::TestMenu(currentTest);
        currentTest = testMenu;

        //Creating new test in the menu
        testMenu->RegisterTest<test::TestClearColor>("Clear Color Test");
        testMenu->RegisterTest<test::TestTexture2D>("2D Texture Test");


        //  Game Loop   //
        while (!glfwWindowShouldClose(window))
        {
            glErrorCall( glClearColor(0.0f, 0.0f, 0.0f, 1.0f) );
            renderer.Clear();

            ImGui_ImplGlfwGL3_NewFrame();

            //Handling the deletion of test pointer
            if (currentTest)
            {
                currentTest->OnUpdate(0.0f);
                currentTest->OnRender();
                ImGui::Begin("Test");

                //Deleting the current test & setting it back to the main menu on press of back button
                if (currentTest != testMenu && ImGui::Button("<-"))
                {
                    delete currentTest;
                    currentTest = testMenu;
                }
                
                currentTest->OnImGuiRender();
                ImGui::End();
            }

            ImGui::Render();
            ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());


            //Swaping front and back buffers
            glfwSwapBuffers(window);

            //Event polling
            glfwPollEvents();
        }

        //Preventing memory leaks
        delete currentTest;
        if (currentTest != testMenu)
            delete testMenu;
    }

    ImGui_ImplGlfwGL3_Shutdown();
    ImGui::DestroyContext();

    glfwTerminate();
    
    return 0;
}