//
// Created by Gabriel Salach on 21/08/2025.
//

#include "Glitcher.h"

#include <imgui.h>
#include <algorithm>
#include <random>

namespace GlitchArtist {
    void Glitcher::ApplyEffect(std::vector<float> &samples) {
        const float sample_gap_bounds = static_cast<float>(sample_size) * (randomness * 10 + 1);

        const int gap_modifier = (static_cast<int>(sample_gap_bounds) - sample_size)/2;
        const int gap_min = sample_size - gap_modifier;
        const int gap_max = sample_size + gap_modifier;

        int index_paste = 0;


        for (int i = 0; i < loop_amount; i++) {
            std::mt19937 rng(seed);

            std::uniform_int_distribution dist(gap_min,gap_max);
            const int index_start = (dist(rng) + index_paste) % (samples.size() - 1);
            std::uniform_int_distribution dist2(gap_min,gap_max);
            const int index_end = (dist2(rng) + index_start) % (samples.size() - 1);
            std::uniform_int_distribution dist3(gap_min,gap_max);
            index_paste = (dist3(rng) + index_end) % (samples.size() - 1);

            if (index_start >= 0 && index_paste < samples.size() && index_end > index_start) {
                std::copy(samples.begin() + index_start, samples.begin() + index_end, samples.begin() + index_paste);
            }
            index_paste += index_end - index_start;
        }
    }

    void Glitcher::RenderUI() {

        if (ImGui::CollapsingHeader("Glitcher")) {
            ImGui::PushID(this);
            ImGui::Checkbox("Enabled", &isActive);

            ImGui::DragInt("Seed", &seed, 1, 0,1000);
            ImGui::SliderInt("Sample Size", &sample_size, 1, 10000);
            ImGui::SliderFloat("Randomness", &randomness, 0, 1);
            ImGui::DragInt("Loop Amount", &loop_amount, 0.1, 1, 1000);
            ImGui::PopID();
        }
    }
} // GlitchArtist
