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

            img_size.x += io.MouseWheel * zoom_sensitivity;
            img_size.y += io.MouseWheel * zoom_sensitivity;

            CenterImage(img_size);
            ImGui::Image(image->GetTexture(), img_size);
        }
        ImGui::End();
    }
} // GlitchArtist