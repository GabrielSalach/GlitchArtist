//
// Created by Gabriel Salach on 20/08/2025.
//

#ifndef GLITCHARTIST_CHORUS_H
#define GLITCHARTIST_CHORUS_H
#include <vector>

#include "IEffect.h"

namespace GlitchArtist {
    class Chorus : public IEffect {
    public:
        explicit Chorus(float sr = 44100.0f);

        void RenderUI() override;
        void ApplyEffect(std::vector<float> &samples) override;

    private:
        std::vector<float> delayBuffer;
        int bufferSize;
        int writeIndex;
        float sampleRate;
        float lfoPhase;

        // Paramètres du chorus
        float delayTime;      // Délai de base en millisecondes
        float modDepth;       // Profondeur de modulation en millisecondes
        float modRate;        // Vitesse de modulation en Hz
        float feedback;       // Feedback (0.0 à 0.95)
        float wetLevel;       // Mix dry/wet (0.0 = dry, 1.0 = wet)

        void initializeBuffer();
        void setPresetSubtle();
        void setPresetRich();
        void setPresetDeep();
        void resetToDefault();
    };
} // GlitchArtist

#endif //GLITCHARTIST_CHORUS_H