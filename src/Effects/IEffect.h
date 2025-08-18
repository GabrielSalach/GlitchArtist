//
// Created by Gabriel Salach on 17/08/2025.
//

#ifndef GLITCHARTIST_IEFFECT_H
#define GLITCHARTIST_IEFFECT_H

namespace GlitchArtist {
    class IEffect {
    public:
        virtual ~IEffect() = default;
        virtual void ApplyEffect(std::vector<float>& samples) = 0;
        virtual void RenderUI() = 0;
    protected:
        bool isActive = false;
    };
}

#endif //GLITCHARTIST_IEFFECT_H