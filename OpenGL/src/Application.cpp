#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "Renderer.h"

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "VertexArray.h"
#include "Texture.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "tests/ListOfTests.h"

int main(){

	#pragma region SETUP_GLFW

	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit()) { return -1; }

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(960, 540, "Hello World", NULL, NULL);

	if (!window) { glfwTerminate(); return -1; }

	/* Make the window's context current */
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1); // set refresh rate to display refresh rate

	if (glewInit() != GLEW_OK) { return -1; }

	#pragma endregion

    std::cout << glGetString(GL_VERSION) << std::endl;

	#pragma region GL_VERSIONCHECK

	#if __APPLE__

	// GL 3.2 + GLSL 150
	const char* glsl_version = "#version 150";
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // Required on Mac

	#else

	// GL 3.0 + GLSL 130
	const char* glsl_version = "#version 130";
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only

	#endif

	#pragma endregion

	#pragma region SETUP_IMGUI

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsClassic();

	// Setup Platform/Renderer bindings
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init(glsl_version);

	#pragma endregion

	Renderer renderer;

	GLCALL(glEnable(GL_BLEND));
	GLCALL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

    { 
		test::Test* currentTest = nullptr;
		test::TestMenu* testMenu = new test::TestMenu(currentTest);
		currentTest = testMenu;

		testMenu->RegisterTest<test::TestClearColour>			("Clear Colour");
		testMenu->RegisterTest<test::TestHelloTriangle>			("Hello Triangle - glDrawArrays");
		testMenu->RegisterTest<test::TestTexture2D>				("Texture Test");
		testMenu->RegisterTest<test::TestRenderMultipleObjects> ("Multiple Objects");
		testMenu->RegisterTest<test::TestTransformationOrtho>	("Transformation ortho");
		testMenu->RegisterTest<test::TestBatchRendering>		("Batch Rendering");


        while (!glfwWindowShouldClose(window)){
			GLCALL(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
			renderer.Clear();

			glfwPollEvents();
			{
				ImGui_ImplOpenGL3_NewFrame();
				ImGui_ImplGlfw_NewFrame();
				ImGui::NewFrame();

				if (currentTest){
					currentTest->OnUpdate(0.0f);
					currentTest->OnRender();
					ImGui::Begin("Test");
					if (currentTest != testMenu && ImGui::Button("<-"))
					{
						delete currentTest;
						currentTest = testMenu;
					}
					currentTest->OnImGuiRender();
					ImGui::End();
				}
				
				ImGui::Render();
				ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
			}
			      

            glfwSwapBuffers(window);
            glfwPollEvents();
        } 
		delete currentTest;
		if (currentTest != testMenu){
			delete testMenu;
		}
    }

	// Shutdown
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();

	ImGui::DestroyContext();

    glfwTerminate();
    return 0;
}