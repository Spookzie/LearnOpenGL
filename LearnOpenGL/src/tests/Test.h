#pragma once


namespace test
{
	class Test
	{
	public:
		//Constructor & Destructor
		Test()	{}
		virtual ~Test()	{}

		virtual void OnUpdate(float delta_time)	{}
		virtual void OnRender()	{}
		virtual void OnImGuiRender()	{}
	};
}