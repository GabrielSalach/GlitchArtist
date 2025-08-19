//
// Created by Gabriel Salach on 17/08/2025.
//

#include "EffectStack.h"

#include "Delay.h"
#include "Distorsion.h"
#include "Phaser.h"
#include "Reverb.h"

namespace GlitchArtist {
    EffectStack::EffectStack(IAudioCodec* codec) {
        this->codec = codec;
    }

    EffectStack::~EffectStack() {
        for (auto effect : effects) {
            delete effect;
        }
        delete this->input_samples;
        delete this->output_samples;
    }

    void EffectStack::InitializeStack() {
        input_samples = new std::vector<float>();
        output_samples = new std::vector<float>();
    }

    void EffectStack::LoadImage(Image *img) {
        if (!img->IsValid()) return;
        this->image = img;
        codec->Decode(*input_samples, *image->GetData());
        output_samples->clear();
        output_samples->resize(input_samples->size());
    }


    void EffectStack::ApplyEffects() const {
        if (image == nullptr) return;
        if (output_samples->size() < input_samples->size()) {
            printf("Error");
            return;
        }
        std::ranges::copy(*input_samples, output_samples->begin());

        for (auto effect : effects) {
            effect->ApplyEffect(*output_samples);
        }
        codec->Encode(*image->GetData(), *output_samples);
    }

    void EffectStack::RenderUI() const {
        ImGui::SeparatorText("Effects Chain");

        for (size_t i = 0; i < effects.size(); ++i) {
            ImGui::PushID(static_cast<int>(i));

            // Ligne de contrôle pour chaque effet
            ImGui::BeginGroup();

            // Flèches de déplacement
            if (ImGui::ArrowButton("up", ImGuiDir_Up) && i > 0) {
                const_cast<EffectStack*>(this)->MoveEffectUp(i);
            }
            ImGui::SameLine();
            if (ImGui::ArrowButton("down", ImGuiDir_Down) && i < effects.size() - 1) {
                const_cast<EffectStack*>(this)->MoveEffectDown(i);
            }
            ImGui::SameLine();

            // Bouton de suppression
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.8f, 0.2f, 0.2f, 1.0f));
            if (ImGui::Button("X")) {
                const_cast<EffectStack*>(this)->RemoveEffect(i);
                ImGui::PopStyleColor();
                ImGui::PopID();
                ImGui::EndGroup();
                break; // Sortir car le vector a changé
            }
            ImGui::PopStyleColor();

            ImGui::EndGroup();
            // Afficher l'UI de l'effet (avec sa checkbox Enabled intégrée)
            // ImGui::Indent();
            // ImGui::Unindent();
            ImGui::SameLine();
            effects[i]->RenderUI();

            ImGui::Separator();
            ImGui::PopID();
        }
    }


    void EffectStack::PushEffect(IEffect* effect) {
        effects.push_back(effect);
    }

    void EffectStack::RemoveEffect(size_t index) {
        if (index < effects.size()) {
            delete effects[index];
            effects.erase(effects.begin() + index);
        }
    }

    void EffectStack::MoveEffectUp(size_t index) {
        if (index > 0 && index < effects.size()) {
            std::swap(effects[index], effects[index - 1]);
        }
    }

    void EffectStack::MoveEffectDown(size_t index) {
        if (index < effects.size() - 1) {
            std::swap(effects[index], effects[index + 1]);
        }
    }

    std::vector<float> * EffectStack::GetSamples() const {
        return this->output_samples;
    }
} // GlitchArtist