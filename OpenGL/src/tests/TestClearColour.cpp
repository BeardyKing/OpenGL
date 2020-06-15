#include "TestClearColour.h"
#include "Renderer.h"
#include "imgui/imgui.h"

namespace test {
	TestClearColour::TestClearColour()
		: m_ClearColour{0.2f,0.3f,0.8f,1.0f}
	{
	}

	TestClearColour::~TestClearColour() 
	{
	}

	void TestClearColour::OnUpdate(float deltaTime) 
	{
	}

	void TestClearColour::OnRender() {
		GLCALL(glClearColor(m_ClearColour[0], m_ClearColour[1], m_ClearColour[2], m_ClearColour[3]));
		GLCALL(glClear(GL_COLOR_BUFFER_BIT));
	}

	void TestClearColour::OnImGuiRender() 
	{
		ImGui::ColorEdit4("Clear Colour", m_ClearColour);
	}
}