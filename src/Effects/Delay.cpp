//
// Created by Gabriel Salach on 18/08/2025.
//

#include "Delay.h"
#include <imgui.h>

namespace GlitchArtist {
    Delay::Delay(const float smpl_rt) : sampleRate(smpl_rt) {
        // Buffer pour 3 secondes max
        maxDelayLength = static_cast<size_t>(3.0f * sampleRate);
        delayBuffer.resize(maxDelayLength, 0.0f);
        updateDelayLength();
    }

    void Delay::RenderUI() {

        if (ImGui::CollapsingHeader("Delay")) {
            ImGui::PushID(this);
            ImGui::Checkbox("Enabled", &isActive);
            ImGui::DragFloat("Time", &delayTime, 0.001, 0, 3, "%.3f s");
            ImGui::DragFloat("Decay", &decay, .01, 0, 0.9f, "%.2f");
            ImGui::DragFloat("WETNESS", &mixLevel, .01, 0, 1, "%.2f");
            ImGui::PopID();
        }
    }

    void Delay::updateDelayLength() {
        currentDelayLength = static_cast<size_t>(delayTime * sampleRate);
        currentDelayLength = std::min(currentDelayLength, maxDelayLength);
    }

    void Delay::ApplyEffect(std::vector<float>& samples) {
        if (!isActive) return;
        // Mettre à jour la longueur de délai si changée
        size_t newDelayLength = static_cast<size_t>(delayTime * sampleRate);
        newDelayLength = std::min(newDelayLength, maxDelayLength);
        if (newDelayLength != currentDelayLength) {
            currentDelayLength = newDelayLength;
        }

        // Clamp les paramètres au cas où ImGui sortirait des bornes
        float clampedDecay = std::clamp(decay, 0.0f, 0.9f);
        float clampedMix = std::clamp(mixLevel, 0.0f, 1.0f);

        for (float& sample : samples) {
            // Lire l'échantillon retardé
            size_t readIndex = (writeIndex - currentDelayLength + delayBuffer.size()) % delayBuffer.size();
            float delayed = delayBuffer[readIndex];

            // Écrire dans le buffer avec feedback
            delayBuffer[writeIndex] = sample + delayed * clampedDecay;

            // Mélanger dry/wet avec un seul paramètre
            sample = sample * (1.0f - clampedMix) + delayed * clampedMix;

            // Avancer l'index
            writeIndex = (writeIndex + 1) % delayBuffer.size();
        }
    }
} // GlitchArtist