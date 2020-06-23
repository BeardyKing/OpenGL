#pragma once

#include "test.h"
#include <memory>
namespace test {

	class TestHelloTriangle : public Test 
	{
	public:
		TestHelloTriangle();
		~TestHelloTriangle();

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;
	private:
		std::unique_ptr<unsigned int> m_VAO;
		std::unique_ptr<unsigned int> m_VBO;
		std::unique_ptr<int> m_ShaderProgram;
	};
}
