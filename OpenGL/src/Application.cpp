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


int main(void){
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit()) {
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window){
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // set refresh rate to display refresh rate

    if (glewInit() != GLEW_OK) {
        return -1;
    }

    std::cout << glGetString(GL_VERSION) << std::endl;

    { // create scope
        //vertex positions
        float positions[] = {
            -0.8f, -0.45f, 0.0f, 0.0f,//0
             0.8f, -0.45f, 1.0f, 0.0f,//1
             0.8f,  0.45f, 1.0f, 1.0f,//2
            -0.8f,  0.45f, 0.0f, 1.0f//3
        };

        //index buffer of vertex positions
        unsigned int indices[] = {
            0,1,2,
            2,3,0
        };

        GLCALL(glEnable(GL_BLEND));
        GLCALL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

        VertexArray va;

        // Vertex Buffer // buffer of memory that is stored on the GPU
        VertexBuffer vb(positions, 4 * 4 * sizeof(float));

        VertexBufferLayout layout;
        layout.Push<float>(2);
        layout.Push<float>(2);
        va.AddBuffer(vb,layout);

        //index buffer object
        IndexBuffer ib(indices, 6);

        glm::mat4 proj = glm::ortho(-1.6f, 1.6f, -0.9f, 0.9f, -1.0f, 1.0f);

        Shader shader("res/shaders/Basic.shader");
        shader.Bind();
        shader.SetUniformsMat4f("u_MVP", proj);
        shader.SetUniforms4f("u_Color", 0.8f, 0.2f, 0.5f, 1.0f);

        Texture texture("res/textures/fg.png");
        texture.Bind();
        shader.SetUniforms1i("u_Texture", 0);
        
        
        shader.Unbind();
        vb.Unbind();
        ib.Unbind();

        Renderer renderer;
        
        float r = 0.0f;
        float increment = 0.05f;

        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window)){
            /* Render here */
            renderer.Clear();

            shader.Bind();
            shader.SetUniforms4f("u_Color", r, 0.3f, 0.8f, 1.0f);
            renderer.Draw(va, ib, shader);

            // change to bind shader -> bind vertex array -> index buffer -> issue draw call

            if (r > 1.0f) {
                increment = -0.05f;
            }
            else if (r < 0.0f) {
                increment = 0.05f;
            }
            r += increment;

            /* Swap front and back buffers */
            glfwSwapBuffers(window);
            /* Poll for and process events */
            glfwPollEvents();
        } 
    }
    glfwTerminate();
    return 0;
}