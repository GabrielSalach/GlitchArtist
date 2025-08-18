//
// Created by Gabriel Salach on 18/08/2025.
//

#ifndef GLITCHARTIST_PHASER_H
#define GLITCHARTIST_PHASER_H
#include <vector>

#include "IEffect.h"

namespace GlitchArtist {
    class Phaser : public IEffect {
    public:
        explicit Phaser(float smpl_rt = 44100.0f);

        void RenderUI();
        void ApplyEffect(std::vector<float>& samples);

    private:
        // Paramètres audio
        float sampleRate;

        // Paramètres du phaser
        float lfoPhase = 0.0f;      // Phase du LFO (0-1)
        float depth = 0.7f;         // Profondeur de modulation (0-1)
        float feedback = 0.3f;      // Feedback (0-0.9)
        float mixLevel = 0.5f;      // Mix dry/wet (0-1)

        // Filtres passe-tout (4 étages)
        static constexpr int NUM_STAGES = 4;
        struct AllPassFilter {
            float delay = 0.0f;
            float y1 = 0.0f;
        };
        AllPassFilter filters[NUM_STAGES];

        // Méthodes privées
        float processAllPass(float input, AllPassFilter& filter, float delay);
    };
} // GlitchArtist

#endif //GLITCHARTIST_PHASER_H
