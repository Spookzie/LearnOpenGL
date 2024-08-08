#include "Test.h"
#include "imgui/imgui.h"


namespace test
{
	test::TestMenu::TestMenu(Test*& current_test_ptr)
		:currentTest(current_test_ptr)
	{
	}
	

	void test::TestMenu::OnImGuiRender()
	{
		for (auto& test : tests)
		{
			if (ImGui::Button(test.first.c_str()))
				currentTest = test.second();
		}
	}
}