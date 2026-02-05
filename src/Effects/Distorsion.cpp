//
// Created by Gabriel Salach on 18/08/2025.
//

#include "Distorsion.h"
#include <imgui.h>
#ifdef _WIN32
#include <algorithm>
#endif

namespace GlitchArtist {
    void Distorsion::ApplyEffect(std::vector<float>& samples) {
        if (!isActive) return;

        for (float& sample : samples) {
            // Sauvegarde du signal original pour le mix
            float originalSample = sample;

            // Application du gain
            sample *= gain;

            // Distorsion par clipping asymétrique
            if (sample > threshold) {
                sample = threshold + (sample - threshold) / (1.0f + ((sample - threshold) / (1.0f - threshold)) * 2.0f);
            } else if (sample < -threshold) {
                sample = -threshold + (sample + threshold) / (1.0f + ((-sample - threshold) / (1.0f - threshold)) * 2.0f);
            }

            // Mix entre signal original et signal distordu
            sample = originalSample * (1.0f - mixLevel) + sample * mixLevel;

            // Limitation pour éviter le clipping numérique
            sample = std::clamp(sample, -1.0f, 1.0f);
        }
    }

    void Distorsion::RenderUI() {
        if (ImGui::CollapsingHeader("Distorsion")) {
            ImGui::PushID(this);
            ImGui::Checkbox("Enabled", &isActive);
            ImGui::DragFloat("Gain", &gain, 0.1f, 0.1f, 10.0f, "%.1f");
            ImGui::DragFloat("Threshold", &threshold, 0.01f, 0.1f, 1.0f, "%.2f");
            ImGui::DragFloat("Mix", &mixLevel, 0.01f, 0.0f, 1.0f, "%.2f");
            ImGui::PopID();
        }
    }
} // GlitchArtist