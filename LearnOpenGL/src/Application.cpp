#include <GL/glew.h>
#include "vendors/imGui/imgui.h"
#include "vendors/imGui/imgui_impl_glfw.h"
#include "vendors/imGui/imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "Renderer.h"
#include "VertexBufferLayout.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Textures.h"

#include "vendors/glm/glm.hpp"
#include "vendors/glm/gtc/matrix_transform.hpp"


#include <stdio.h>

float screenWidth;
float screenHeight;

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    const char* glsl_version = "#version 330";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    screenWidth = 1280.0f;
    screenHeight = 720.0f;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(screenWidth, screenHeight, "Output Window", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    glfwSwapInterval(1);

    if (glewInit() != GLEW_OK)
    {
        std::cout << "GLEW Init Error!" << std::endl;
    }

    std::cout << glGetString(GL_VERSION) << std::endl;

    {
        //Set Indices and vertex positions
        float positions[] = {
             0.0f,   0.0f, 0.0f, 0.0f,
             486.0f, 0.0f, 1.0f, 0.0f,
             486.0f, 260.0f, 1.0f, 1.0f,
             0.0f,   260.0f, 0.0f, 1.0f
        };

        unsigned int indices[] =
        {
            0,1,2,
            2,3,0
        };

        //Setup blending method
        GLCall(glEnable(GL_BLEND));
        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

        Shader shader("res/shaders/Basic.shader");
        shader.Bind();
        shader.SetUniform4f("u_Color", 0.2f, 0.2f, 1.0f, 1.0f);

        //Generate vertex buffer and vertex array
        VertexArray va;
        VertexBuffer vb(positions, 4 * 4 * sizeof(float));
        VertexBufferLayout layout;
        layout.Push<float>(2);
        layout.Push<float>(2);
        va.AddBuffer(vb, layout);

        //Generate index buffer
        IndexBuffer ib(indices, 6);

        //Generate model view projection matrix (orthographic
        glm::mat4 proj = glm::ortho(0.0f, screenWidth, 0.0f, screenHeight, -1.0f, 1.0f); //projection matrix
        glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0)); //view matrix (camera

        // Generate Texture and bind to shader
        Texture texture("res/textures/Texture_meme.png");
        texture.Bind();
        shader.SetUniform1i("u_Texture", 0);

        // Unbind Everything
        va.UnBind();
        vb.Unbind();
        ib.Unbind();
        shader.UnBind();

        Renderer renderer;
        
        // Setup ImGui context
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGui::StyleColorsDark();

        glm::vec3 translation(200, 200, 0);

        // Setup Renderer backends
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init(glsl_version);

        //ImGui test window states
        bool show_demo_window = true;
        bool show_another_window = false;
        ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

        float r = 0.0f;
        float increment = 0.05f;

        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {
            /* Render here */
            renderer.Clear();

            //ImGui new frame
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            glm::mat4 model = glm::translate(glm::mat4(1.0f), translation); //model matrix
            glm::mat4 mvp = proj * view * model;

            shader.Bind();
            renderer.Draw(va, ib, shader);
            shader.SetUniformMat4f("u_ModelViewProjectionMatrix", mvp);


            {
                static float f = 0.0f;

                ImGui::SliderFloat3("Translation", &translation.x, 0.0f, 960.0f);
                ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            }


            //ImGui rendering
            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

            /* Swap front and back buffers */
            GLCall(glfwSwapBuffers(window));

            /* Poll for and process events */
            GLCall(glfwPollEvents());
        }
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}