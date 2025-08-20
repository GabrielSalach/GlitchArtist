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
        img_size = ImVec2(500, 500);
    }

    void PreviewWindow::RenderWindow() {
        ImGuiIO& io = ImGui::GetIO();

        ImGui::Begin("Preview");
        if (!image->IsValid()) {
            CenterText("No preview image");
            ImGui::Text("No preview image");
        } else {
            ImGui::SliderFloat("Zoom Level", &zoom_level, 0.3f, 2.0f);
            ImVec2 real_size = img_size;
            real_size.x *= zoom_level;
            real_size.y *= zoom_level;

            CenterImage(real_size);
            ImGui::Image(image->GetTexture(), real_size);
        }
        ImGui::End();
    }
} // GlitchArtist