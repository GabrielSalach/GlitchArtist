//
// Created by Gabriel Salach on 17/08/2025.
//

#include "Application.h"

#include "AudioEncoding/MuLawCodec.h"
#include "GuiComponents/PreviewWindow.h"
#include "FileLoading/BMPLoader.h"
#include "GuiComponents/EffectStackWindow.h"

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
        LoadEffectStack();
        // Initialize windows
        BindWindows();
    }


    void Application::LoadImage() {
        image = new Image(reinterpret_cast<IFormatLoader *>(new BMPLoader()));
        image->LoadImage(image_path);
    }

    void Application::LoadEffectStack() {
        effect_stack = new EffectStack(image, new MuLawCodec());
        effect_stack->InitializeStack();
    }


    void Application::BindWindows() {
        windows.push_back(std::make_unique<EffectStackWindow>(effect_stack));
        windows.push_back(std::make_unique<PreviewWindow>(image));
    }

    void Application::NewFrame() {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
    }

    void Application::Update() {
        ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport());
        effect_stack->ApplyEffects();

        ImGui::Begin("Debug");
        ImGui::Text("%.f fps", 1.0f / ImGui::GetIO().DeltaTime);
        ImGui::End();

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

        delete effect_stack;
        delete image;
        windows.clear();

    }
} // GlitchArtist