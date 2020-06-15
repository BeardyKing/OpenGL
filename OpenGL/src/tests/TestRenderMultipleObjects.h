#pragma once

#include "test.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace test {

	class TestRenderMultipleObjects : public Test
	{
	public:
		TestRenderMultipleObjects();
		~TestRenderMultipleObjects();

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;
	private:
		glm::vec3 m_translationA();
		glm::vec3 m_translationB();
	};
}
