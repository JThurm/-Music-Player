#include <iostream>

#include <GLFW/glfw3.h>

#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"


int main()
{
    // --------------------------------------------------------
    // Initialize GLFW
    // --------------------------------------------------------

    if (!glfwInit())
    {
        std::cerr << "Failed to initialize GLFW." << std::endl;
        return 1;
    }


    // --------------------------------------------------------
    // Configure OpenGL
    // --------------------------------------------------------

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


    // --------------------------------------------------------
    // Create Window
    // --------------------------------------------------------

    GLFWwindow* window = glfwCreateWindow(
        1280,
        720,
        "Pulse",
        nullptr,
        nullptr
    );

    if (!window)
    {
        std::cerr << "Failed to create GLFW window." << std::endl;

        glfwTerminate();

        return 1;
    }


    glfwMakeContextCurrent(window);

    glfwSwapInterval(1);


    // --------------------------------------------------------
    // Initialize Dear ImGui
    // --------------------------------------------------------

    IMGUI_CHECKVERSION();

    ImGui::CreateContext();

    ImGuiIO& io = ImGui::GetIO();

    (void)io;


    // --------------------------------------------------------
    // Initialize ImGui Backends
    // --------------------------------------------------------

    ImGui_ImplGlfw_InitForOpenGL(
        window,
        true
    );

    ImGui_ImplOpenGL3_Init(
        "#version 330"
    );


    // --------------------------------------------------------
    // Main Loop
    // --------------------------------------------------------

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();


        // Start ImGui frame

        ImGui_ImplOpenGL3_NewFrame();

        ImGui_ImplGlfw_NewFrame();

        ImGui::NewFrame();


        // ----------------------------------------------------
        // Temporary UI
        // ----------------------------------------------------

        ImGui::Begin("Pulse");

        ImGui::Text(
            "Welcome to the Pulse Music Player!"
        );

        ImGui::Separator();

        if (ImGui::Button("Click Me"))
        {
            std::cout << "Button clicked!" << std::endl;
        }

        ImGui::End();

        // hello


        // ----------------------------------------------------
        // Render
        // ----------------------------------------------------

        ImGui::Render();

        int displayWidth;
        int displayHeight;

        glfwGetFramebufferSize(
            window,
            &displayWidth,
            &displayHeight
        );

        glViewport(
            0,
            0,
            displayWidth,
            displayHeight
        );

        glClearColor(
            0.08f,
            0.08f,
            0.10f,
            1.0f
        );

        glClear(
            GL_COLOR_BUFFER_BIT
        );

        ImGui_ImplOpenGL3_RenderDrawData(
            ImGui::GetDrawData()
        );

        glfwSwapBuffers(window);
    }


    // --------------------------------------------------------
    // Cleanup
    // --------------------------------------------------------

    ImGui_ImplOpenGL3_Shutdown();

    ImGui_ImplGlfw_Shutdown();

    ImGui::DestroyContext();


    glfwDestroyWindow(window);

    glfwTerminate();


    return 0;
}