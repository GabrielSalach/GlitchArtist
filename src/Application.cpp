//
// Created by Gabriel Salach on 17/08/2025.
//

#include "Application.h"

#include "GuiComponents/PreviewWindow.h"
#include "FileLoading/BMPLoader.h"

namespace GlitchArtist {
    void Application::Init(GLFWwindow* window, const char* glsl_version) {
        //Initialize ImGUI;
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO &io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init(glsl_version);
        ImGui::StyleColorsDark();

        LoadImage();
        // Initialize windows
        BindWindows();
    }


    void Application::LoadImage() {
        image = new Image(reinterpret_cast<IFormatLoader *>(new BMPLoader()));
        image->LoadImage(image_path);
    }

    void Application::BindWindows() {
        windows.push_back(std::make_unique<PreviewWindow>(image));
    }

    void Application::NewFrame() {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
    }

    void Application::Update() {
        ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport());
        for (auto& window : windows) {
            window->RenderWindow();
        }
    }


    void Application::Render() {
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }

    void Application::Shutdown() {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }
} // GlitchArtist