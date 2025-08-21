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
        void ApplyEffect(std::vector<float>& samples) override;
        void RenderUI() override;
    private:
        int seed = 0;
        int sample_size = 0;
        float randomness = 0;
        int loop_amount = 0;
    };
} // GlitchArtist

#endif //GLITCHARTIST_GLITCHER_H