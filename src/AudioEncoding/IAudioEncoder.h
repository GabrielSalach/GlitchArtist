//
// Created by Gabriel Salach on 17/08/2025.
//

#ifndef GLITCHARTIST_IAUDIOENCODER_H
#define GLITCHARTIST_IAUDIOENCODER_H

namespace GlitchArtist {
    class IAudioEncoder {
    public:
        virtual ~IAudioEncoder() = default;
        virtual void Encode();
    };
} // GlitchArtist

#endif //GLITCHARTIST_IAUDIOENCODER_H