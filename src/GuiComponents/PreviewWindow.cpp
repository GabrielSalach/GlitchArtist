//
// Created by Gabriel Salach on 17/08/2025.
//

#include "PreviewWindow.h"

#include <imgui.h>
#include <GLFW/glfw3.h>

#include "../Helpers/ImGuiHelper.h"

namespace GlitchArtist {
    PreviewWindow::PreviewWindow(Image *image) {
        this->image = image;
    }

    void PreviewWindow::RenderWindow() {
        ImGui::Begin("Preview");
        if (!image->IsValid()) {
            CenterText("No preview image");
            ImGui::Text("No preview image");
        } else {
            CenterImage(ImVec2(500, 500));
            ImGui::Image(image->GetTexture(), ImVec2(500, 500));
        }
        ImGui::End();
    }
} // GlitchArtist