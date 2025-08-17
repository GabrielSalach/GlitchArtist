//
// Created by Gabriel Salach on 17/08/2025.
//

#ifndef GLITCHARTIST_EFFECTSTACK_H
#define GLITCHARTIST_EFFECTSTACK_H
#include <vector>

#include "IEffect.h"
#include "../FileLoading/Image.h"

namespace GlitchArtist {

    class EffectStack {
    public:
        explicit EffectStack(Image* image);
        void ApplyEffects();
        std::vector<IEffect*> effects;
    private:

    };
} // GlitchArtist

#endif //GLITCHARTIST_EFFECTSTACK_H