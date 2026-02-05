//
// Created by Gabriel Salach on 17/08/2025.
//

#ifndef GLITCHARTIST_MULAWENCODER_H
#define GLITCHARTIST_MULAWENCODER_H
#include "IAudioCodec.h"
#ifdef _WIN32
#include <cstdint>
#endif

namespace GlitchArtist {
    class MuLawCodec : public IAudioCodec {
    public:
        void Encode(std::vector<unsigned char> &raw_data, std::vector<float> &samples_buffer) override;
        void Decode(std::vector<float> &samples, std::vector<unsigned char> &raw_data_buffer) override;
    private:
        static unsigned char encodePcm16ToMuLaw(int16_t pcm16);
        static void InitializeEncodeTable();
        static unsigned int FloatToTableIndex(float f);

        void EncodeOptimized_V1(std::vector<unsigned char> &raw_data, const std::vector<float> &samples_buffer);
        void EncodeOptimized_Threaded(std::vector<unsigned char> &raw_data, const std::vector<float> &samples_buffer);
        void EncodeOptimized_Cache(std::vector<unsigned char> &raw_data, const std::vector<float> &samples_buffer);

        // Table de décodage μ-law vers PCM16 signé
        static const int16_t mulaw_decode_table[256];

        // Table d'encodage PCM16 vers μ-law (indexée par PCM16 + 32768)
        static unsigned char float_to_mulaw_table[65536]; // 16-bit range
        static bool encode_table_initialized;

        // Constantes μ-law
        static constexpr int16_t MU_LAW_BIAS = 0x84;
        static constexpr int16_t MU_LAW_MAX = 0x1FFF;
        static constexpr int16_t MU_LAW_CLIP = 32635;
    };
} // GlitchArtist

#endif //GLITCHARTIST_MULAWENCODER_H