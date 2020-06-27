#pragma once

#include "test.h"

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "Texture.h"
#include <memory>

namespace test {

	class TestTransformationPerspective : public Test
	{
	public:
		TestTransformationPerspective();
		~TestTransformationPerspective();

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
		glm::vec3 m_translationA;

		glm::vec3 m_ObjectTranslation, m_RotationAxis, m_Scale;
		
		float m_RotationAmount;
		float m_FOV;
		float m_SinTime;
	};
}