//
// Created by Gabriel Salach on 17/08/2025.
//

#ifndef GLITCHARTIST_EFFECTSTACKWINDOW_H
#define GLITCHARTIST_EFFECTSTACKWINDOW_H
#include "IWindow.h"
#include "../Effects/EffectStack.h"
#include "../FileLoading/Image.h"

namespace GlitchArtist {

    class EffectStackWindow : public IWindow{
    public:
        explicit EffectStackWindow(EffectStack* effect_stack);
        void RenderWindow() override;
    private:
        EffectStack* effect_stack;
    };
} // GlitchArtist

#endif //GLITCHARTIST_EFFECTSTACKWINDOW_H