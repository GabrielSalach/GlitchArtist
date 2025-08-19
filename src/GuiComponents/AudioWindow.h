//
// Created by Gabriel Salach on 19/08/2025.
//

#ifndef GLITCHARTIST_AUDIOWINDOW_H
#define GLITCHARTIST_AUDIOWINDOW_H
#include "IWindow.h"
#include "../Effects/EffectStack.h"

namespace GlitchArtist {
    class AudioWindow : public IWindow {
    public:
        explicit AudioWindow(EffectStack* stack);
        void RenderWindow() override;
    private:
        void RenderWaveform() const;

        EffectStack* effect_stack;
        std::vector<float>* audio_samples;
    };
} // GlitchArtist

#endif //GLITCHARTIST_AUDIOWINDOW_H