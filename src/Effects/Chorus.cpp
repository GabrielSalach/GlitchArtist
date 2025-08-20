//
// Created by Gabriel Salach on 20/08/2025.
//

#include "Chorus.h"
#include <imgui.h>

namespace GlitchArtist {
    Chorus::Chorus(float sr) :
        sampleRate(sr),
        writeIndex(0),
        lfoPhase(0.0f),
        delayTime(15.0f),     // 15ms de délai de base
        modDepth(5.0f),       // ±5ms de modulation
        modRate(0.5f),        // 0.5 Hz de modulation
        feedback(0.3f),       // 30% de feedback
        wetLevel(0.5f)        // 50% wet/dry mix
    {
        isActive = true;
        initializeBuffer();
    }

    void Chorus::ApplyEffect(std::vector<float>& samples) {
        if (!isActive || samples.empty()) return;

        for (size_t i = 0; i < samples.size(); ++i) {
            float input = samples[i];

            // Calcul du délai modulé par le LFO
            float lfoValue = std::sin(lfoPhase);
            float currentDelay = delayTime + (modDepth * lfoValue);
            float delaySamples = currentDelay * sampleRate / 1000.0f;

            // Position de lecture dans le buffer (avec interpolation)
            float readPos = writeIndex - delaySamples;
            if (readPos < 0) readPos += bufferSize;

            // Interpolation linéaire pour une lecture fractionnaire
            int readIndex1 = static_cast<int>(readPos) % bufferSize;
            int readIndex2 = (readIndex1 + 1) % bufferSize;
            float fraction = readPos - std::floor(readPos);

            float delayedSample = delayBuffer[readIndex1] * (1.0f - fraction) +
                                  delayBuffer[readIndex2] * fraction;

            // Application du feedback
            float feedbackSample = input + (delayedSample * feedback);

            // Écriture dans le buffer de délai
            delayBuffer[writeIndex] = feedbackSample;

            // Mélange dry/wet avec un seul paramètre
            float output = input * (1.0f - wetLevel) + delayedSample * wetLevel;

            // Limitation pour éviter le clipping
            output = std::max(-1.0f, std::min(1.0f, output));

            samples[i] = output;

            // Mise à jour de l'index d'écriture seulement
            writeIndex = (writeIndex + 1) % bufferSize;
        }
    }

    void Chorus::RenderUI() {
        if (ImGui::CollapsingHeader("Chorus")) {
            ImGui::PushID(this);

            ImGui::Checkbox("Enabled", &isActive);

            // Contrôle manuel de la phase LFO
            ImGui::DragFloat("LFO Phase", &lfoPhase, 0.001f, 0.0f, 2.0f * M_PI, "%.3f rad");

            // Paramètres de délai
            bool delayChanged = false;
            delayChanged |= ImGui::DragFloat("Delay Time", &delayTime, 0.1f, 1.0f, 50.0f, "%.1f ms");
            delayChanged |= ImGui::DragFloat("Mod. Depth", &modDepth, 0.01f, 0.0f, 20.0f, "%.2f ms");

            if (delayChanged) {
                initializeBuffer();
            }

            ImGui::DragFloat("Mod. Rate", &modRate, 0.001f, 0.1f, 10.0f, "%.3f Hz");
            ImGui::DragFloat("Feedback", &feedback, 0.001f, 0.0f, 0.95f, "%.3f");
            ImGui::DragFloat("Wet/Dry Mix", &wetLevel, 0.001f, 0.0f, 1.0f, "%.3f");

            // Visualisation du LFO
            float lfoVis = std::sin(lfoPhase);
            ImGui::ProgressBar((lfoVis + 1.0f) * 0.5f, ImVec2(0.0f, 0.0f), "LFO");

            // Boutons utilitaires
            if (ImGui::Button("Auto LFO")) {
                // Incrément automatique basé sur modRate
                lfoPhase += 2.0f * M_PI * modRate / sampleRate;
                if (lfoPhase >= 2.0f * M_PI) lfoPhase -= 2.0f * M_PI;
            }
            ImGui::SameLine();
            if (ImGui::Button("Reset Phase")) {
                lfoPhase = 0.0f;
            }

            // Presets
            if (ImGui::Button("Subtle")) {
                setPresetSubtle();
            }
            ImGui::SameLine();
            if (ImGui::Button("Rich")) {
                setPresetRich();
            }
            ImGui::SameLine();
            if (ImGui::Button("Deep")) {
                setPresetDeep();
            }

            if (ImGui::Button("Reset")) {
                resetToDefault();
            }

            // Informations
            ImGui::Text("Buffer: %d samples", bufferSize);
            ImGui::Text("Current Delay: %.1f ms", delayTime + (modDepth * std::sin(lfoPhase)));

            ImGui::PopID();
        }
    }

    void Chorus::initializeBuffer() {
        // Buffer pour délai maximum + marge de sécurité
        int newBufferSize = static_cast<int>((delayTime + modDepth * 2) * sampleRate / 1000.0f) + 128;
        if (newBufferSize != bufferSize) {
            bufferSize = newBufferSize;
            delayBuffer.clear();
            delayBuffer.resize(bufferSize, 0.0f);
            writeIndex = 0;
        }
    }

    void Chorus::setPresetSubtle() {
        delayTime = 10.0f;
        modDepth = 2.0f;
        modRate = 0.3f;
        feedback = 0.2f;
        wetLevel = 0.3f;
        lfoPhase = 0.0f;
        initializeBuffer();
    }

    void Chorus::setPresetRich() {
        delayTime = 15.0f;
        modDepth = 5.0f;
        modRate = 0.5f;
        feedback = 0.3f;
        wetLevel = 0.5f;
        lfoPhase = 0.0f;
        initializeBuffer();
    }

    void Chorus::setPresetDeep() {
        delayTime = 25.0f;
        modDepth = 8.0f;
        modRate = 0.8f;
        feedback = 0.4f;
        wetLevel = 0.6f;
        lfoPhase = 0.0f;
        initializeBuffer();
    }

    void Chorus::resetToDefault() {
        delayTime = 15.0f;
        modDepth = 5.0f;
        modRate = 0.5f;
        feedback = 0.3f;
        wetLevel = 0.5f;
        lfoPhase = 0.0f;
        isActive = true;
        initializeBuffer();
    }

} // GlitchArtist