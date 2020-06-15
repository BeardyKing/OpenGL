#pragma once
#include <iostream>

#include "test.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "VertexBufferLayout.h"
#include "Texture.h"
#include "GLFW/glfw3.h"

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
		Renderer *m_renderer;
	public:
		VertexBufferLayout	*m_layout;
		VertexBuffer		*m_vb;
		VertexArray			m_va;
		IndexBuffer			*m_ib;
	private:
		Shader m_shader;
		Texture m_texture;
	private:
		float m_positions		[4 * 4];
		unsigned int m_indices	[3 * 3];
	public:
		glm::vec3 m_translationA;
		glm::vec3 m_translationB;
	private:
		glm::mat4 m_proj;
		glm::mat4 m_view;
	public:
	};
}
