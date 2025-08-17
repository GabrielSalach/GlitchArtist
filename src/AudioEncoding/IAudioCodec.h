//
// Created by Gabriel Salach on 17/08/2025.
//

#ifndef GLITCHARTIST_IAUDIOENCODER_H
#define GLITCHARTIST_IAUDIOENCODER_H
#include <vector>

namespace GlitchArtist {
    class IAudioCodec {
    public:
        virtual ~IAudioCodec() = default;
        virtual void Encode(std::vector<unsigned char>& raw_data, std::vector<float>& samples_buffer) = 0;
        virtual void Decode(std::vector<float>& samples, std::vector<unsigned char>& raw_data_buffer) = 0;
    };


} // GlitchArtist

#endif //GLITCHARTIST_IAUDIOENCODER_H