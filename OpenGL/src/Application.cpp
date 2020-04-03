#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <fstream>
#include <string>
#include <sstream>

#include "Renderer.h"

#include "VertexBuffer.h"
#include "IndexBuffer.h"


struct ShaderProgramSource {
    std::string VertexSource;
    std::string FragmentSource;
};

static ShaderProgramSource ParseShader(const std::string& filepath) {
    std::ifstream stream(filepath);

    enum class ShaderType {
        NONE = -1, VERTEX = 0, FRAGMENT = 1
    };

    std::string line;
    std::stringstream ss[2];
    ShaderType type = ShaderType::NONE;

    while (getline(stream, line)) {
        if (line.find("#shader") != std::string::npos){
            if (line.find("vertex") != std::string::npos){
                type = ShaderType::VERTEX;
            }
            else if(line.find("fragment") != std::string::npos){
                type = ShaderType::FRAGMENT;
            }
        }
        else{
            ss[(int)type] << line << '\n';
        }
    }
    return { ss[0].str(), ss[1].str() };
}

static unsigned int CompileShader(unsigned int type, const std::string& source) {
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);

    if (result == GL_FALSE){
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*)malloc(length * sizeof(char)); // could be alloca
        glGetShaderInfoLog(id, length, &length, message);
        std::cout << "FAILED TO COMPILE " << (type == GL_VERTEX_SHADER ? "VERTEX" : "FRAGMENT") << std::endl;
        std::cout << message << std::endl;
        glDeleteShader(id);
        free(message);
        return 0;
    }
    return id;
}

static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader) {
    unsigned int program = glCreateProgram();
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}

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
    glfwSwapInterval(1); // set refreshrate to display refreshrate

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

        // vertext Attributes // layout of the buffer
        glEnableVertexAttribArray(0);// enable index 0
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);

        //index buffer object
        IndexBuffer ib(indices, 6);

        ShaderProgramSource source = ParseShader("res/shaders/Basic.shader");

        unsigned int shader = CreateShader(source.VertexSource, source.FragmentSource);
        glUseProgram(shader); // bound shader

        int location = glGetUniformLocation(shader, "u_Color"); //get id of the u_Color variable in the shader
        ASSERT(location != -1);
        GLCALL(glUniform4f(location, 0.8, 0.2, 0.5, 1.0f)); // set uniform to the shader
        GLCALL(glBindVertexArray(0));
        glUseProgram(0); // bound shader
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        float r = 0.0f;
        float increment = 0.05f;

        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window)){
            /* Render here */
            GLCALL(glClear(GL_COLOR_BUFFER_BIT));

            GLCALL(glUseProgram(shader)); // bind shader
            GLCALL(glUniform4f(location, r, 0.2, 0.5, 1.0f)); // set up uniforms

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
        glDeleteProgram(shader);
    }
    glfwTerminate();
    return 0;
}