#include "TestTransformationPerspective.h"

#include "Renderer.h"
#include "imgui/imgui.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include <GLFW/glfw3.h>


namespace test {
	TestTransformationPerspective::TestTransformationPerspective()
		:
		m_Proj(glm::perspective(glm::radians(70.0f), (float)960.0f / (float)540.0f, 0.1f, 300.0f)),
		m_View(glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 1.0f))),
		m_translationA(glm::vec3(0.5f, 0, -3)),
		m_RotationAxis(glm::vec3(0.5f, 1.0f, 0.5f)),
		m_ObjectTranslation(glm::vec3(0.0f, 0.0f, 0.0f)),
		m_Scale(glm::vec3(1.0f, 1.0f, 1.0f)),
		m_FOV(55),
		m_SinTime(0),
		m_RotationAmount(0)
	{
		float position[] = {
			-1.0f,-1.0f,0.0f,0.0f,
			 1.0f,-1.0f,1.0f,0.0f,
			 1.0f, 1.0f,1.0f,1.0f,
			-1.0f, 1.0f,0.0f,1.0f
		};

		unsigned int indices[] = {
			0,1,3,
			1,2,3
		};

		m_Shader = std::make_unique<Shader>("res/shaders/Basic_Pespective.shader");
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

	TestTransformationPerspective::~TestTransformationPerspective(){}

	void TestTransformationPerspective::OnUpdate(float deltaTime){}

	void TestTransformationPerspective::OnRender() {
		GLCALL(glClearColor(0.7f, 0.4f, 0.4f, 1.0f));
		GLCALL(glClear(GL_COLOR_BUFFER_BIT));
		Renderer renderer;

		m_Texture->Bind();
		{
			m_SinTime = glm::sin((float)glfwGetTime() / 2);

			glm::mat4 model = glm::mat4(1.0f);
			m_View = glm::mat4(1.0f);

			model = glm::rotate(model, glm::radians(m_RotationAmount), glm::vec3(m_RotationAxis.x, m_RotationAxis.y, m_RotationAxis.z));
			model += glm::scale(model, glm::vec3(m_Scale.x, m_Scale.y, m_Scale.z));
			model += glm::translate(model, glm::vec3(m_ObjectTranslation.x, m_ObjectTranslation.y, m_ObjectTranslation.z));

			m_View = glm::translate(m_View, glm::vec3(m_translationA.x, m_translationA.y, m_translationA.z));
			m_Proj = glm::perspective(glm::radians(m_FOV), (float)960.0f / (float)540.0f, 0.1f, 300.0f);

			glm::mat4 mvp = m_Proj * m_View * model;
			m_Shader->Bind();
			m_Shader->SetUniformsMat4f("u_MVP", mvp);

			renderer.Draw(*m_VAO, *m_IndexBuffer, *m_Shader);
		}
	}

	void TestTransformationPerspective::OnImGuiRender()
	{
		ImGui::Begin("Test Transform");
		ImGui::Text("View");
		ImGui::SliderFloat3("translationA", &m_translationA.x, -3, 3);
		ImGui::SliderFloat("FOV", &m_FOV, 45, 90);
		ImGui::Text("Object");
		ImGui::SliderFloat3("Translation", &m_ObjectTranslation.x, -10, 2);
		ImGui::SliderFloat3("Scale", &m_Scale.x, 0, 3);
		ImGui::SliderFloat3("Rotation Axis", &m_RotationAxis.x, 0, 1);
		ImGui::SliderFloat("Rotation Amount", &m_RotationAmount, 0, 180);
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::End();
	}
}