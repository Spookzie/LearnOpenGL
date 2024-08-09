#pragma once

#include "Test.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "Texture.h"

#include <memory>


namespace test
{
	class TestTexture2D : public Test
	{
	private:
		glm::vec3 translationA, translationB;
		std::unique_ptr<VertexArray> va;
		std::unique_ptr<IndexBuffer> ib;
		std::unique_ptr<Shader> shader;
		std::unique_ptr<Texture> texture;

	public:
		TestTexture2D();
		~TestTexture2D();

		void OnUpdate(float delta_time) override;
		void OnRender() override;
		void OnImGuiRender() override;
	};
}