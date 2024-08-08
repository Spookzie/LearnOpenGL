#pragma once

#include <vector>
#include <string>
#include <functional>
#include <iostream>


namespace test
{
	class Test
	{
	public:
		Test()	{}
		virtual ~Test()	{}

		virtual void OnUpdate(float delta_time)	{}
		virtual void OnRender()	{}
		virtual void OnImGuiRender()	{}
	};


	class TestMenu : public Test
	{
	private:
		Test*& currentTest;
		std::vector< std::pair<std::string, std::function<Test*()>> > tests;

	public:
		TestMenu(Test*& current_test_ptr);

		virtual void OnImGuiRender() override;

		template<typename T>
		void RegisterTest(const std::string& name)
		{
			std::cout << "Registering " << name << std::endl;
			tests.push_back( std::make_pair(name, []() { return new T(); }) );
		}
	};
}