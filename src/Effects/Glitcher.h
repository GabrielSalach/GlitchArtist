//
// Created by Gabriel Salach on 21/08/2025.
//

#ifndef GLITCHARTIST_GLITCHER_H
#define GLITCHARTIST_GLITCHER_H
#include <vector>

#include "IEffect.h"

namespace GlitchArtist {
    class Glitcher : public IEffect {
    public:
        ~Glitcher() override;

        void ApplyEffect(std::vector<float>& samples) override;
        void RenderUI() override;
    private:
        float seed;
        int sample_amount_min, sample_amount_max;
        float sample_amount_randomness;
        float spacing_min, spacing_max;
        float spacing_randomness;
    };
} // GlitchArtist

#endif //GLITCHARTIST_GLITCHER_H