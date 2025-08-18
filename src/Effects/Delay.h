//
// Created by Gabriel Salach on 18/08/2025.
//

#ifndef GLITCHARTIST_DELAY_H
#define GLITCHARTIST_DELAY_H

#include <vector>

#include "IEffect.h"

namespace GlitchArtist {
    class Delay: public IEffect {

    public:

        explicit Delay(float smpl_rt = 44100.0f);
        void ApplyEffect(std::vector<float>& samples) override;
        void RenderUI() override;

    private:
        std::vector<float> delayBuffer;
        size_t writeIndex = 0;
        size_t maxDelayLength;
        float sampleRate;

        float delayTime = 0.5f;      // Temps en secondes (0.1 - 3.0)
        float decay = 0.3f;           // Decay/feedback (0.0 - 0.9)
        float mixLevel = 0.3f;          // Niveau reverb (0.0 - 1.0)

        size_t currentDelayLength = 0;

        void updateDelayLength();
    };
} // GlitchArtist

#endif //GLITCHARTIST_DELAY_H