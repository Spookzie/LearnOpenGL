#include "TestClearColor.h"
#include "Renderer.h"
#include "imgui/imgui.h"


namespace test
{
	TestClearColor::TestClearColor()
		: clearColor {0.2f, 0.4f, 0.6f, 1.0f}
	{
	}

	TestClearColor::~TestClearColor()
	{
	}


	void TestClearColor::OnUpdate(float delta_time)
	{	
	}
	
	
	void TestClearColor::OnRender()
	{
		glErrorCall( glClearColor(clearColor[0], clearColor[1], clearColor[2], clearColor[3]) );
		glErrorCall( glClear(GL_COLOR_BUFFER_BIT) );
	}
	
	
	void TestClearColor::OnImGuiRender()
	{
		ImGui::ColorEdit4("Clear Color", clearColor);
	}
}