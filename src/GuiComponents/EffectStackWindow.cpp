//
// Created by Gabriel Salach on 17/08/2025.
//

#include "EffectStackWindow.h"

#include <imgui.h>

#include "../Effects/Delay.h"
#include "../Effects/Distorsion.h"
#include "../Effects/Phaser.h"
#include "../Effects/Reverb.h"

namespace GlitchArtist {
    EffectStackWindow::EffectStackWindow(EffectStack* effect_stack) {
        this->effect_stack = effect_stack;
    }

    void EffectStackWindow::RenderWindow() {
        ImGui::Begin("Effect Stack");

        if (ImGui::Button("Add")) {
            ImGui::OpenPopup("select_effect");
        }
        ImGui::SameLine();
        ImGui::Checkbox("Realtime", &effect_stack->real_time);
        if (effect_stack->real_time == false) {
            ImGui::SameLine();
            if (ImGui::Button("Render")) {
                effect_stack->ApplyEffects();
            }
        }

        if (ImGui::BeginPopup("select_effect")) {
            ImGui::SeparatorText("Effects");
            if (ImGui::Button("Reverb")) {effect_stack->PushEffect(new Reverb);}
            if (ImGui::Button("Delay")) {effect_stack->PushEffect(new Delay);}
            if (ImGui::Button("Phaser")) {effect_stack->PushEffect(new Phaser);}
            if (ImGui::Button("Distorsion")) {effect_stack->PushEffect(new Distorsion);}
            ImGui::EndPopup();
        }

        effect_stack->RenderUI();
        ImGui::End();
    }
} // GlitchArtist