#pragma once

#include "Test.h"


namespace test
{
	class TestClearColor : public Test
	{
	private:
		float clearColor[4];

	public:
		TestClearColor();
		~TestClearColor();

		void OnUpdate(float delta_time) override;
		void OnRender() override;
		void OnImGuiRender() override;
	};
}