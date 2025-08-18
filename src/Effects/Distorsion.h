//
// Created by Gabriel Salach on 18/08/2025.
//

#ifndef GLITCHARTIST_DISTORSION_H
#define GLITCHARTIST_DISTORSION_H
#include <vector>

#include "IEffect.h"

namespace GlitchArtist {
    class Distorsion : public IEffect{
    public:
        void ApplyEffect(std::vector<float>& samples) override;
        void RenderUI() override;
    private:
        bool isActive = false;
        float gain = 1.0f;        // Gain d'entrée (0.1 à 10.0)
        float threshold = 0.7f;   // Seuil de clipping (0.1 à 1.0)
        float mixLevel = 1.0f;    // Niveau de mix wet/dry (0.0 à 1.0)
    };
} // GlitchArtist

#endif //GLITCHARTIST_DISTORSION_H