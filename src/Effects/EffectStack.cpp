//
// Created by Gabriel Salach on 17/08/2025.
//

#include "EffectStack.h"

#include "Delay.h"
#include "Phaser.h"
#include "Reverb.h"

namespace GlitchArtist {
    EffectStack::EffectStack(Image *image, IAudioCodec* codec) {
        this->image = image;
        this->codec = codec;
    }

    EffectStack::~EffectStack() {
        delete this->input_samples;
        delete this->output_samples;
    }

    void EffectStack::InitializeStack() {
        input_samples = new std::vector<float>();
        codec->Decode(*input_samples, *image->GetData());
        output_samples = new std::vector<float>(input_samples->size());

        // effects.push_back(new Reverb());
        //effects.push_back(new Delay());
        effects.push_back(new Phaser());
    }

    void EffectStack::ApplyEffects() const {
        if (output_samples->size() < input_samples->size()) {
            printf("Error");
            return;
        }
        std::ranges::copy(*input_samples, output_samples->begin());

        for (size_t i = 0; i < effects.size(); i++) {
            effects[i]->ApplyEffect(*output_samples);
        }
        codec->Encode(*image->GetData(), *output_samples);
    }

    void EffectStack::RenderUI() const {
        for (const auto & effect : effects) {
            effect->RenderUI();
        }
    }
} // GlitchArtist