//
// Created by Gabriel Salach on 18/08/2025.
//

#include "MainMenuBar.h"
#include <imgui.h>
#include <nfd.h>
#include <string>
#include "../Application.h"

namespace GlitchArtist {
    MainMenuBar::MainMenuBar(Application* app) {
        application = app;
    }

    void MainMenuBar::RenderWindow() {
        ImGui::BeginMainMenuBar();
        if (ImGui::BeginMenu("File")) {
            if (ImGui::MenuItem("Load File")) {
                nfdchar_t *outPath = NULL;
                if (NFD_OpenDialog(&outPath, nullptr, 0, nullptr) == NFD_OKAY) {
                    // Utilise ton path ici
                    const std::string filePath(outPath);
                    application->LoadImage(filePath);
                    free(outPath);
                }
            }
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }
} // GlitchArtist