#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "Renderer.h"

#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Shader.h"


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
            -0.5f, -0.5f, //0
             0.5f, -0.5f, //1
             0.5f,  0.5f, //2
            -0.5f,  0.5f, //3
        };

        //index buffer of vertex positions
        unsigned int indices[] = {
            0,1,2,
            2,3,0
        };

        unsigned int vao;

        GLCALL(glGenVertexArrays(1, &vao));
        GLCALL(glBindVertexArray(vao));

        // Vertex Buffer // buffer of memory that is stored on the GPU
        VertexBuffer vb(positions, 4 * 2 * sizeof(float));

        // vertex Attributes // layout of the buffer
        glEnableVertexAttribArray(0);// enable index 0
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);

        //index buffer object
        IndexBuffer ib(indices, 6);

        Shader shader("res/shaders/Basic.shader");
        shader.Bind();
        shader.SetUniforms4f("u_Color", 0.8f, 0.2f, 0.5f, 1.0f);
        
        
        shader.Unbind();
        vb.Unbind();
        ib.Unbind();
        
        float r = 0.0f;
        float increment = 0.05f;

        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window)){
            /* Render here */
            GLCALL(glClear(GL_COLOR_BUFFER_BIT));
            
            shader.Bind();
            shader.SetUniforms4f("u_Color", r, 0.2f, 0.5f, 1.0f);

            GLCALL(glBindVertexArray(vao));
            ib.Bind(); //bind index buffer

            // Draw amount of indices
            GLCALL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));

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