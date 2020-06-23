#include "TestBatchRendering.h"
#include "Renderer.h"
#include "imgui/imgui.h"

namespace test {
	TestBatchRendering::TestBatchRendering()
		: m_ClearColour{0.2f,0.3f,0.2f,1.0f}
	{
	}

	TestBatchRendering::~TestBatchRendering()
	{
	}

	void TestBatchRendering::OnUpdate(float deltaTime)
	{
	}

	void TestBatchRendering::OnRender() {

		GLCALL(glClearColor(m_ClearColour[0], m_ClearColour[1], m_ClearColour[2], m_ClearColour[3]));
		GLCALL(glClear(GL_COLOR_BUFFER_BIT));

		float vertices[] = {
			-0.5f, -0.5f, 0.0f,
			 0.5f, -0.5f, 0.0f,
			 0.5f, -0.5f, 0.0f,
			-0.5f,  0.5f, 0.0f
		};

		
	
	}

	void TestBatchRendering::OnImGuiRender()
	{
		ImGui::Begin("COLOUR");
		ImGui::ColorEdit4("Clear Colour", m_ClearColour);
		ImGui::End();
	}
}