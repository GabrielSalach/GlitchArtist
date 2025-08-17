//
// Created by Gabriel Salach on 17/08/2025.
//

#include "EffectStackWindow.h"

#include <imgui.h>

namespace GlitchArtist {
    EffectStackWindow::EffectStackWindow(EffectStack* effect_stack) {
        this->effect_stack = effect_stack;
    }

    void EffectStackWindow::RenderWindow() {
        ImGui::Begin("Effect Stack");
        effect_stack->RenderUI();
        ImGui::End();
    }
} // GlitchArtist