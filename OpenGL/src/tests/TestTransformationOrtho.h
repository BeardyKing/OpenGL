#pragma once

#include "test.h"

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "Texture.h"
#include <memory>

namespace test {

	class TestTransformationOrtho : public Test
	{
	public:
		TestTransformationOrtho();
		~TestTransformationOrtho();

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;
	private:
		std::unique_ptr <VertexArray> m_VAO;
		std::unique_ptr <IndexBuffer> m_IndexBuffer;
		std::unique_ptr <Shader>	  m_Shader;
		std::unique_ptr <Texture>	  m_Texture;
		std::unique_ptr <VertexBuffer>m_VertexBuffer;

		glm::mat4 m_Proj,m_View;

		glm::vec3 m_translationA, m_RotationAxis;

		float m_RotationSpeedDelta;
		float m_ScaleTimer;
		
	};
}