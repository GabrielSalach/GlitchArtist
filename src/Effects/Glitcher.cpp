//
// Created by Gabriel Salach on 21/08/2025.
//

#include "Glitcher.h"

#include <imgui.h>

namespace GlitchArtist {
    void Glitcher::RenderUI() {

        if (ImGui::CollapsingHeader("Distorsion")) {
            ImGui::PushID(this);
            ImGui::Checkbox("Enabled", &isActive);
            ImGui::PopID();
        }
    }
} // GlitchArtist
