//
// Created by Gabriel Salach on 17/08/2025.
//

#ifndef GLITCHARTIST_EFFECTSTACK_H
#define GLITCHARTIST_EFFECTSTACK_H
#include <vector>

#include "IEffect.h"
#include "../AudioEncoding/IAudioCodec.h"
#include "../FileLoading/Image.h"

namespace GlitchArtist {

    class EffectStack {
    public:
        explicit EffectStack(Image* image, IAudioCodec* codec);

        ~EffectStack();

        void InitializeStack();
        void ApplyEffects() const;
        void RenderUI() const;
        std::vector<IEffect*> effects;
    private:
        std::vector<float>* input_samples = nullptr;
        std::vector<float>* output_samples = nullptr;
        Image* image = nullptr;
        IAudioCodec* codec = nullptr;
    };
} // GlitchArtist

#endif //GLITCHARTIST_EFFECTSTACK_H