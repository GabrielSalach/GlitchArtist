//
// Created by Gabriel Salach on 18/08/2025.
//

#include "Phaser.h"

#include <imgui.h>
#include <algorithm>
#ifdef _WIN32
#include <cmath>
#endif


namespace GlitchArtist {
Phaser::Phaser(const float smpl_rt) : sampleRate(smpl_rt) {
        // Initialiser les filtres
        for (int i = 0; i < NUM_STAGES; ++i) {
            filters[i].delay = 0.0f;
            filters[i].y1 = 0.0f;
        }
    }

    void Phaser::RenderUI() {
        if (ImGui::CollapsingHeader("Phaser")) {
            ImGui::Checkbox("Enabled", &isActive);
            ImGui::DragFloat("Phase", &lfoPhase, 0.01f, 0.0f, 1.0f, "%.2f");
            ImGui::DragFloat("Depth", &depth, 0.01f, 0.0f, 1.0f, "%.2f");
            ImGui::DragFloat("Feedback", &feedback, 0.01f, 0.0f, 0.9f, "%.2f");
            ImGui::DragFloat("WETNESS", &mixLevel, 0.01f, 0.0f, 1.0f, "%.2f");
        }
    }

    float Phaser::processAllPass(float input, AllPassFilter& filter, float delay) {
        // Filtre passe-tout du premier ordre
        // y[n] = -g*x[n] + x[n-1] + g*y[n-1]
        float output = -delay * input + filter.delay + delay * filter.y1;

        filter.delay = input;
        filter.y1 = output;

        return output;
    }

    void Phaser::ApplyEffect(std::vector<float>& samples) {
        if (!isActive) return;
        // Clamp les paramètres au cas où ImGui sortirait des bornes
        float clampedPhase = std::clamp(lfoPhase, 0.0f, 1.0f);
        float clampedDepth = std::clamp(depth, 0.0f, 1.0f);
        float clampedFeedback = std::clamp(feedback, 0.0f, 0.9f);
        float clampedMix = std::clamp(mixLevel, 0.0f, 1.0f);

        static float feedbackSample = 0.0f;

        // Convertir la phase (0-1) en radians
        float currentPhase = clampedPhase * 2.0f * M_PI;

        // Calculer la valeur du LFO (sinus)
        float lfoValue = std::sin(currentPhase);

        // Convertir le LFO en coefficient de délai pour les filtres passe-tout
        // Range typique: 0.2 à 0.9
        float delayCoeff = 0.2f + 0.7f * (lfoValue * 0.5f + 0.5f) * clampedDepth;

        for (float& sample : samples) {
            // Signal d'entrée avec feedback
            float inputWithFeedback = sample + feedbackSample * clampedFeedback;

            // Passer à travers les étages de filtres passe-tout
            float processed = inputWithFeedback;
            for (int i = 0; i < NUM_STAGES; ++i) {
                processed = processAllPass(processed, filters[i], delayCoeff);
            }

            // Stocker pour le feedback
            feedbackSample = processed;

            // Mélanger dry/wet
            sample = sample * (1.0f - clampedMix) + processed * clampedMix;
        }
    }

} // GlitchArtist