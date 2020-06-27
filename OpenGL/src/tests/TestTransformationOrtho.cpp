#include "TestTransformationOrtho.h"

#include "Renderer.h"
#include "imgui/imgui.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include <GLFW/glfw3.h>


namespace test {
	TestTransformationOrtho::TestTransformationOrtho()
		:
		m_Proj(glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -200.0f, 200.0f)),
		//m_Proj(glm::perspective(glm::radians(0.0f), (float)960 / (float)540, -1.0f, 1.0f)),
		m_View(glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0))),
		m_translationA(glm::vec3(300, 300, 0)),
		m_RotationAxis(glm::vec3(0.0f, 1.0f, 0.5f)),
		m_RotationSpeedDelta(65),
		m_ScaleTimer(0)
	{
		float position[] = {
			-50.0f,-50.0f,0.0f,0.0f,
			 50.0f,-50.0f,1.0f,0.0f,
			 50.0f, 50.0f,1.0f,1.0f,
			-50.0f, 50.0f,0.0f,1.0f
		};

		unsigned int indices[] = {
			0,1,3,
			1,2,3
		};

		m_Shader = std::make_unique<Shader>("res/shaders/Basic.shader");
		m_VAO = std::make_unique <VertexArray>();

		GLCALL(glEnable(GL_BLEND));
		GLCALL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

		m_VertexBuffer = std::make_unique<VertexBuffer>(position, 4 * 4 * sizeof(float));
		VertexBufferLayout layout;
		layout.Push<float>(2);
		layout.Push<float>(2);

		m_VAO->AddBuffer(*m_VertexBuffer, layout);
		m_IndexBuffer = std::make_unique<IndexBuffer>(indices, 6);

		m_Shader->Bind();

		m_Texture = std::make_unique<Texture>("res/textures/fg.png");
		m_Shader->SetUniforms1i("u_Texture", 0);

	}

	TestTransformationOrtho::~TestTransformationOrtho()
	{
	}

	void TestTransformationOrtho::OnUpdate(float deltaTime)
	{
	}

	void TestTransformationOrtho::OnRender() {
		GLCALL(glClearColor(0.7f, 0.4f, 0.4f, 1.0f));
		GLCALL(glClear(GL_COLOR_BUFFER_BIT));
		Renderer renderer;

		m_Texture->Bind();
		{
			m_ScaleTimer = glm::sin((float)glfwGetTime() / 2);

			glm::mat4 model = glm::translate(glm::mat4(1), m_translationA);
			model = glm::rotate(model, (float)glfwGetTime() * m_RotationSpeedDelta, m_RotationAxis);
			model = glm::scale(model, glm::vec3(1, 1, 1) * m_ScaleTimer + 1.5f);

			glm::mat4 mvp = m_Proj * m_View* model;
			m_Shader->Bind();
			m_Shader->SetUniformsMat4f("u_MVP", mvp);

			renderer.Draw(*m_VAO, *m_IndexBuffer, *m_Shader);
		}
	}

	void TestTransformationOrtho::OnImGuiRender()
	{
		ImGui::Begin("TRANSFROM");
		ImGui::SliderFloat3("translationA", &m_translationA.x, 0.0f, 960.0f);
		ImGui::SliderFloat3("Rotation Axis", &m_RotationAxis.x, 0.0f, 1.0f);
		ImGui::SliderFloat("Rotation Speed", &m_RotationSpeedDelta, -1, 1);
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::End();
	}
}