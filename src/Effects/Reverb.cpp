//
// Created by Gabriel Salach on 17/08/2025.
//

#include "Reverb.h"

#include <imgui.h>
#ifdef _WIN32
#include <algorithm>
#endif

namespace GlitchArtist {
    Reverb::Reverb(const float smpl_rt) : sampleRate(smpl_rt),
        // Initialiser les délais comb avec les longueurs ajustées au sample rate
        combDelays{{
            DelayLine(static_cast<size_t>(combLengths[0] * sampleRate / 44100.0f)),
            DelayLine(static_cast<size_t>(combLengths[1] * sampleRate / 44100.0f)),
            DelayLine(static_cast<size_t>(combLengths[2] * sampleRate / 44100.0f)),
            DelayLine(static_cast<size_t>(combLengths[3] * sampleRate / 44100.0f)),
            DelayLine(static_cast<size_t>(combLengths[4] * sampleRate / 44100.0f)),
            DelayLine(static_cast<size_t>(combLengths[5] * sampleRate / 44100.0f)),
            DelayLine(static_cast<size_t>(combLengths[6] * sampleRate / 44100.0f)),
            DelayLine(static_cast<size_t>(combLengths[7] * sampleRate / 44100.0f))
        }},
        // Initialiser les délais allpass
        allpassDelays{{
            DelayLine(static_cast<size_t>(allpassLengths[0] * sampleRate / 44100.0f)),
            DelayLine(static_cast<size_t>(allpassLengths[1] * sampleRate / 44100.0f)),
            DelayLine(static_cast<size_t>(allpassLengths[2] * sampleRate / 44100.0f)),
            DelayLine(static_cast<size_t>(allpassLengths[3] * sampleRate / 44100.0f))
        }} {
        updateParameters();
    }

    void Reverb::RenderUI() {
        if (ImGui::CollapsingHeader("Reverb")) {
            ImGui::PushID(this);
            ImGui::Checkbox("Enabled", &isActive);
            ImGui::DragFloat("Room Size", &roomSize, 0.01f, 0.0f, 1.0f, "%.2f");
            ImGui::DragFloat("Damping", &damping, 0.01f, 0.0f, 1.0f, "%.2f");
            ImGui::DragFloat("Width", &width, 0.01f, 0.0f, 1.0f, "%.2f");
            ImGui::DragFloat("WETNESS", &mixLevel, 0.01f, 0.0f, 1.0f, "%.2f");
            ImGui::PopID();
        }
    }

    void Reverb::updateParameters() {
        // Mettre à jour les paramètres de feedback des combs basés sur roomSize
        float feedback = 0.28f + (roomSize * 0.7f);

        for (auto& comb : combDelays) {
            comb.feedback = feedback;
        }
    }

    float Reverb::processAllpass(float input, DelayLine& delay) {
        float delayed = delay.buffer[delay.writeIndex];
        float output = -input + delayed;

        delay.buffer[delay.writeIndex] = input + delayed * 0.5f;
        delay.writeIndex = (delay.writeIndex + 1) % delay.length;

        return output;
    }

    void Reverb::ApplyEffect(std::vector<float>& samples) {
        if (!isActive) return;

        // Clamp les paramètres
        float clampedRoomSize = std::clamp(roomSize, 0.0f, 1.0f);
        float clampedDamping = std::clamp(damping, 0.0f, 1.0f);
        float clampedWidth = std::clamp(width, 0.0f, 1.0f);
        float clampedMix = std::clamp(mixLevel, 0.0f, 1.0f);

        // Mettre à jour les paramètres si roomSize a changé
        static float lastRoomSize = -1.0f;
        if (std::abs(clampedRoomSize - lastRoomSize) > 0.001f) {
            roomSize = clampedRoomSize;
            updateParameters();
            lastRoomSize = clampedRoomSize;
        }

        for (float& sample : samples) {
            float input = sample;
            float output = 0.0f;

            // Étape 1: Traitement par les filtres comb (en parallèle)
            for (auto& comb : combDelays) {
                output += comb.process(input, comb.feedback, clampedDamping);
            }

            // Moyenner la sortie des combs
            output *= 0.125f; // 1/8

            // Étape 2: Traitement par les filtres allpass (en série)
            for (auto& allpass : allpassDelays) {
                output = processAllpass(output, allpass);
            }

            // Contrôle de largeur stéréo (simplifié pour mono)
            output *= clampedWidth;

            // Mix final dry/wet
            sample = input * (1.0f - clampedMix) + output * clampedMix;
        }
    }

} // GlitchArtist