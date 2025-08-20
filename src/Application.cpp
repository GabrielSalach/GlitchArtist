//
// Created by Gabriel Salach on 17/08/2025.
//

#include "Application.h"

#include "AudioEncoding/MuLawCodec.h"
#include "GuiComponents/PreviewWindow.h"
#include "FileLoading/BMPLoader.h"
#include "GuiComponents/AudioWindow.h"
#include "GuiComponents/EffectStackWindow.h"
#include "GuiComponents/HexEditorWindow.h"

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



        LoadEffectStack();
        LoadImage(image_path);
        // Initialize windows
        BindWindows();
    }

    void Application::LoadImage(const std::string &path) {
        if (image == nullptr)
            image = new Image(new BMPLoader());
        if (path.empty()) {
            return;
        }
        image->LoadImage(path);
        effect_stack->LoadImage(image);
    }

    void Application::LoadEffectStack() {
        effect_stack = new EffectStack(new MuLawCodec());
        effect_stack->InitializeStack();
    }

    void Application::BindWindows() {
        main_menu = new MainMenuBar(this);
        windows.push_back(std::make_unique<EffectStackWindow>(effect_stack));
        windows.push_back(std::make_unique<PreviewWindow>(image));
        windows.push_back(std::make_unique<AudioWindow>(effect_stack));
        windows.push_back(std::make_unique<HexEditorWindow>(image));
    }

    void Application::NewFrame() {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
    }

    void Application::Update() {
        main_menu->RenderWindow();
        ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport());
        if (effect_stack->real_time)
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