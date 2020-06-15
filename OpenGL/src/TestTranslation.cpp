//#include <GL/glew.h>
//#include <GLFW/glfw3.h>
//
//#include <iostream>
//#include <fstream>
//#include <string>
//#include <sstream>
//
//#include "Renderer.h"
//
//#include "VertexBuffer.h"
//#include "VertexBufferLayout.h"
//#include "IndexBuffer.h"
//#include "Shader.h"
//#include "VertexArray.h"
//#include "Texture.h"
//
//#include "glm/glm.hpp"
//#include "glm/gtc/matrix_transform.hpp"
//
//#include "imgui/imgui.h"
//#include "imgui/imgui_impl_glfw.h"
//#include "imgui/imgui_impl_opengl3.h"
//
//
//int main(void) {
//    GLFWwindow* window;
//
//    /* Initialize the library */
//    if (!glfwInit()) {
//        return -1;
//    }
//
//    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
//    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
//    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
//
//    /* Create a windowed mode window and its OpenGL context */
//    window = glfwCreateWindow(960, 540, "Hello World", NULL, NULL);
//    if (!window) {
//        glfwTerminate();
//        return -1;
//    }
//
//    /* Make the window's context current */
//    glfwMakeContextCurrent(window);
//    glfwSwapInterval(1); // set refresh rate to display refresh rate
//
//    if (glewInit() != GLEW_OK) {
//        return -1;
//    }
//
//    std::cout << glGetString(GL_VERSION) << std::endl;
//
//#if __APPLE__
//    // GL 3.2 + GLSL 150
//    const char* glsl_version = "#version 150";
//    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
//    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
//    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
//    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // Required on Mac
//#else
//    // GL 3.0 + GLSL 130
//    const char* glsl_version = "#version 130";
//    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
//    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
//    //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
//    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only
//#endif
//
//    // Setup IMGUI
//    IMGUI_CHECKVERSION();
//    ImGui::CreateContext();
//    ImGuiIO& io = ImGui::GetIO(); (void)io;
//    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
//    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
//
//    // Setup Dear ImGui style
//    ImGui::StyleColorsDark();
//    //ImGui::StyleColorsClassic();
//
//    // Setup Platform/Renderer bindings
//    ImGui_ImplGlfw_InitForOpenGL(window, true);
//    ImGui_ImplOpenGL3_Init(glsl_version);
//
//
//    { // create scope
//        //vertex positions
//        float positions[] = {
//             -50.0f, -50.0f, 0.0f, 0.0f,//0
//              50.0f, -50.0f, 1.0f, 0.0f,//1
//              50.0f,  50.0f, 1.0f, 1.0f,//2
//             -50.0f,  50.0f, 0.0f, 1.0f//3
//        };
//
//        //index buffer of vertex positions
//        unsigned int indices[] = {
//            0,1,2,
//            2,3,0
//        };
//
//        GLCALL(glEnable(GL_BLEND));
//        GLCALL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
//
//        VertexArray va;
//
//        // Vertex Buffer // buffer of memory that is stored on the GPU
//        VertexBuffer vb(positions, 4 * 4 * sizeof(float));
//
//        VertexBufferLayout layout;
//        layout.Push<float>(2);
//        layout.Push<float>(2);
//        va.AddBuffer(vb, layout);
//
//        //index buffer object
//        IndexBuffer ib(indices, 6);
//
//        glm::mat4 proj = glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f);
//        glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
//        /* glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(200, 200, 0));
//
//         glm::mat4 mvp = proj * view * model;*/
//
//        Shader shader("res/shaders/Basic.shader");
//        shader.Bind();
//        //shader.SetUniformsMat4f("u_MVP", mvp);
//        shader.SetUniforms4f("u_Color", 0.8f, 0.2f, 0.5f, 1.0f);
//
//        Texture texture("res/textures/fg.png");
//        texture.Bind();
//        shader.SetUniforms1i("u_Texture", 0);
//
//
//        shader.Unbind();
//        vb.Unbind();
//        ib.Unbind();
//
//        Renderer renderer;
//
//        float r = 0.0f;
//        float increment = 0.05f;
//
//        glm::vec3 translationA(200, 200, 0);
//        glm::vec3 translationB(400, 200, 0);
//
//        /* Loop until the user closes the window */
//        while (!glfwWindowShouldClose(window)) {
//            glfwPollEvents();
//
//            ImGui_ImplOpenGL3_NewFrame();
//            ImGui_ImplGlfw_NewFrame();
//
//            ImGui::NewFrame();
//
//            {
//                static float f = 0.0f;
//                static int counter = 0;
//
//                ImGui::Begin("TRANSFROM");
//                ImGui::SliderFloat3("translationA", &translationA.x, 0.0f, 960.0f);
//                ImGui::SliderFloat3("translationB", &translationB.x, 0.0f, 960.0f);
//
//                ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
//                ImGui::End();
//            }
//
//
//
//            /* Render here */
//
//            renderer.Clear();
//            {
//                glm::mat4 model = glm::translate(glm::mat4(1.0f), translationA);
//                glm::mat4 mvp = proj * view * model;
//                shader.Bind();
//                shader.SetUniformsMat4f("u_MVP", mvp);
//
//                renderer.Draw(va, ib, shader);
//            }
//
//            {
//                glm::mat4 model = glm::translate(glm::mat4(1.0f), translationB);
//                glm::mat4 mvp = proj * view * model;
//                shader.Bind();
//                shader.SetUniformsMat4f("u_MVP", mvp);
//
//                renderer.Draw(va, ib, shader);
//            }
//
//
//            ImGui::Render();
//            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
//
//
//            // change to bind shader -> bind vertex array -> index buffer -> issue draw call
//
//            if (r > 1.0f) {
//                increment = -0.05f;
//            }
//            else if (r < 0.0f) {
//                increment = 0.05f;
//            }
//            r += increment;
//
//            /* Swap front and back buffers */
//            glfwSwapBuffers(window);
//            /* Poll for and process events */
//            glfwPollEvents();
//
//        }
//    }
//
//    // Shutdown
//    ImGui_ImplOpenGL3_Shutdown();
//    ImGui_ImplGlfw_Shutdown();
//
//    ImGui::DestroyContext();
//
//    glfwTerminate();
//    return 0;
//}