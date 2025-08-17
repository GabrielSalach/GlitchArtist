//
// Created by Gabriel Salach on 17/08/2025.
//

#include "PreviewWindow.h"

#include <imgui.h>
#include <GLFW/glfw3.h>

namespace GlitchArtist {
    PreviewWindow::PreviewWindow(Image *image) {
        this->image = image;
    }

    void PreviewWindow::RenderWindow() {
        ImGui::Begin("Preview");
        GLuint textureID = image->GetTexture();
        //ImGui::Text(std::to_string(textureID).c_str());
        ImGui::Image(textureID, ImVec2(500, 500));
        ImGui::End();
    }
} // GlitchArtist