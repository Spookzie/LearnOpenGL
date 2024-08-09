#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "imgui/imgui.h"

#include "TestTexture2D.h"
#include "Renderer.h"


namespace test
{
    TestTexture2D::TestTexture2D()
        : proj(glm::ortho(0.0f, 1280.0f, 0.0f, 720.0f /*-1.0f, 1.0f*/)), 
        view(glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0))),
        translationA(400, 400, 0), translationB(900, 400, 0)
	{
        /*
        Setting vertex positions & texture coordinates for texture.
        We make the rectangle of texture by joining 2 triangles- an upper left & a lower right.
        The first 2 floats of each vector define the position of each vertex to be drawn in window.
        The next 2 floats define the texture coordinates for their resp. vertex.
        */
        float positions[] = {
            -50.0f, -50.0f, 0.0f, 0.0f, //Lower left    : index-0
             50.0f, -50.0f, 1.0f, 0.0f, //Lower right 
             50.0f,  50.0f, 1.0f, 1.0f, //Upper right 
            -50.0f,  50.0f, 0.0f, 1.0f  //Upper left    : index-3
        };

        unsigned int indices[] = {
            0, 1, 2,    //Accessing vertices for lower right triangle
            2, 3, 0     //for upper left triangle
        };

        //Blending
        glErrorCall(glEnable(GL_BLEND));
        glErrorCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

        //Setting up vertex array & buffer
        va = std::make_unique<VertexArray>();
        vb = std::make_unique<VertexBuffer>(positions, 4 * 4 * sizeof(float));
        VertexBufferLayout layout;
        layout.Push<float>(2);
        layout.Push<float>(2);
        va->AddBuffer(*vb, layout);

        ib = std::make_unique<IndexBuffer>(indices, 6);

        //Setting up shader & texture
        shader = std::make_unique<Shader>("res/shaders/BaseShader.shader");
        shader->Bind();
        texture = std::make_unique<Texture>("res/textures/Spookzie_Logo.png");
        shader->SetUniform1i("u_Texture", 0);
	}

	TestTexture2D::~TestTexture2D()
	{
	}


	void TestTexture2D::OnUpdate(float delta_time)
	{	
	}
	
	
	void TestTexture2D::OnRender()
	{
		glErrorCall( glClearColor(0.0f, 0.0f, 0.0f, 1.0f) );
		glErrorCall( glClear(GL_COLOR_BUFFER_BIT) );

        //  Drawing Textures    //
        Renderer renderer;
        texture->Bind();

        //First
        {
            glm::mat4 model = glm::translate(glm::mat4(1.0f), translationA);
            glm::mat4 mvp = proj * view * model;
            shader->Bind();
            shader->SetUniformMat4f("u_MVP", mvp);

            renderer.Draw(*va, *ib, *shader);
        }

        //Second
        {
            glm::mat4 model = glm::translate(glm::mat4(1.0f), translationB);
            glm::mat4 mvp = proj * view * model;
            shader->Bind();
            shader->SetUniformMat4f("u_MVP", mvp);

            renderer.Draw(*va, *ib, *shader);
        }
	}
	
	
	void TestTexture2D::OnImGuiRender()
	{
        ImGui::SliderFloat2("Translation A", &translationA.x, 0.0f, 1280.0f);
        ImGui::SliderFloat2("Translation B", &translationB.x, 0.0f, 1280.0f);
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	}
}