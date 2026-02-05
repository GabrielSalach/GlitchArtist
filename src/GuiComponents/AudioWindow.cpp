//
// Created by Gabriel Salach on 19/08/2025.
//

#include "AudioWindow.h"

#include <imgui.h>
#ifdef _WIN32
#include <limits>
#endif

namespace GlitchArtist {
    AudioWindow::AudioWindow(EffectStack *stack) : audio_samples(nullptr) {
        effect_stack = stack;
    }

    void AudioWindow::RenderWindow() {
        ImGui::Begin("Audio");
        RenderWaveform();
        ImGui::End();
    }

    void AudioWindow::RenderWaveform() const {
        if (effect_stack == nullptr
            || effect_stack->GetSamples() == nullptr
            || effect_stack->GetSamples()->empty()) return;

        const int sample_count = static_cast<int>(effect_stack->GetSamples()->size());
        const ImVec2 canvas_size = ImGui::GetContentRegionAvail();

        // Créer un canvas invisible qui prend exactement l'espace disponible
        ImGui::InvisibleButton("waveform_canvas", canvas_size);

        // Nombre d'échantillons par pixel
        const int samples_per_pixel = std::max(1, sample_count / static_cast<int>(canvas_size.x));

        ImDrawList* draw_list = ImGui::GetWindowDrawList();
        ImVec2 canvas_pos = ImGui::GetItemRectMin(); // Position du canvas

        // Fond rouge
        draw_list->AddRectFilled(canvas_pos,
                               ImVec2(canvas_pos.x + canvas_size.x, canvas_pos.y + canvas_size.y),
                               IM_COL32(30,30,30,255));

        // Dessiner la waveform
        for (int x = 0; x < static_cast<int>(canvas_size.x); x++) {
            float min_val = std::numeric_limits<float>::max();
            float max_val = -std::numeric_limits<float>::max();

            // Calculer l'index de début pour ce pixel
            int start_sample = x * samples_per_pixel;
            int end_sample = std::min(start_sample + samples_per_pixel, sample_count);

            // Trouver min/max pour ce pixel
            for (int sample_idx = start_sample; sample_idx < end_sample; sample_idx++) {
                float value = effect_stack->GetSamples()->at(sample_idx);
                min_val = std::min(min_val, value);
                max_val = std::max(max_val, value);
            }

            // Convertir les valeurs audio (-1 à 1) en coordonnées d'écran
            float center_y = canvas_pos.y + canvas_size.y * 0.5f;
            float y_min = center_y - (max_val * canvas_size.y * 0.5f);
            float y_max = center_y - (min_val * canvas_size.y * 0.5f);

            float x_pos = canvas_pos.x + x;
            // Dessiner la ligne verticale représentant l'amplitude à cette position
            draw_list->AddLine(ImVec2(x_pos, y_min), ImVec2(x_pos, y_max), IM_COL32(41,74,122,255), 1.0f);
        }
    }
} // GlitchArtist