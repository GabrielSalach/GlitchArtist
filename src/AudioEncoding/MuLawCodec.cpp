//
// Created by Gabriel Salach on 17/08/2025.
//

#include "MuLawCodec.h"
#include <thread>
#ifdef _WIN32
#include <algorithm>
#endif

namespace GlitchArtist {
    void MuLawCodec::Decode(std::vector<float> &samples, std::vector<unsigned char> &raw_data_buffer) {
        samples.resize(raw_data_buffer.size());

        for (size_t i = 0; i < raw_data_buffer.size(); i++) {
            // Décodage μ-law vers PCM16 signé
            int16_t pcm16 = mulaw_decode_table[raw_data_buffer[i]];

            // Conversion PCM16 vers float (-1.0 à 1.0)
            samples[i] = static_cast<float>(pcm16) / 32768.0f;
        }
    }

    // Encodage float vers μ-law
    void MuLawCodec::Encode(std::vector<unsigned char> &raw_data, std::vector<float> &samples_buffer) {
        InitializeEncodeTable();

        const size_t size = samples_buffer.size();

        // Pré-allocation intelligente
        if (raw_data.capacity() < size) {
            raw_data.reserve(size * 1.2f);
        }
        raw_data.resize(size);

        const float* src = samples_buffer.data();
        unsigned char* dst = raw_data.data();

        // Seuil pour décider d'utiliser le threading
        const size_t threading_threshold = 1024 * 1024; // 1Mo

        if (size >= threading_threshold) {
            // Version threadée pour gros volumes
            EncodeOptimized_Threaded(raw_data, samples_buffer);
        } else {
            // Version optimisée single-thread pour petits volumes
            EncodeOptimized_Cache(raw_data, samples_buffer);
        }
    }

    void MuLawCodec::EncodeOptimized_Threaded(std::vector<unsigned char> &raw_data, const std::vector<float> &samples_buffer) {
        InitializeEncodeTable();

        const size_t size = samples_buffer.size();
        raw_data.resize(size);

        const float* src = samples_buffer.data();
        unsigned char* dst = raw_data.data();

        const size_t min_chunk_size = 64 * 1024; // 64KB minimum par thread
        const size_t num_threads = std::min(std::thread::hardware_concurrency(),
                                           static_cast<unsigned int>(size / min_chunk_size));

        if (num_threads > 1) {
            std::vector<std::thread> threads;
            const size_t chunk_size = size / num_threads;

            for (size_t t = 0; t < num_threads; t++) {
                size_t start = t * chunk_size;
                size_t end = (t == num_threads - 1) ? size : start + chunk_size;

                threads.emplace_back([&, start, end]() {
                    for (size_t i = start; i < end; i++) {
                        dst[i] = float_to_mulaw_table[FloatToTableIndex(src[i])];
                    }
                });
            }

            for (auto& thread : threads) {
                thread.join();
            }
        } else {
            EncodeOptimized_V1(raw_data, samples_buffer);
        }
    }

    void MuLawCodec::EncodeOptimized_V1(std::vector<unsigned char> &raw_data, const std::vector<float> &samples_buffer) {
        InitializeEncodeTable();

        const size_t size = samples_buffer.size();
        raw_data.resize(size);

        const float* src = samples_buffer.data();
        unsigned char* dst = raw_data.data();

        // Déroulement de boucle 4x
        size_t i = 0;
        const size_t unrolled_size = size & ~3;

        for (; i < unrolled_size; i += 4) {
            dst[i]     = float_to_mulaw_table[FloatToTableIndex(src[i])];
            dst[i + 1] = float_to_mulaw_table[FloatToTableIndex(src[i + 1])];
            dst[i + 2] = float_to_mulaw_table[FloatToTableIndex(src[i + 2])];
            dst[i + 3] = float_to_mulaw_table[FloatToTableIndex(src[i + 3])];
        }

        // Éléments restants
        for (; i < size; i++) {
            dst[i] = float_to_mulaw_table[FloatToTableIndex(src[i])];
        }
    }

    void MuLawCodec::EncodeOptimized_Cache(std::vector<unsigned char> &raw_data, const std::vector<float> &samples_buffer) {
        InitializeEncodeTable();

        const size_t size = samples_buffer.size();

        // Éviter réallocation
        if (raw_data.capacity() < size) {
            raw_data.reserve(size * 1.2f);
        }
        raw_data.resize(size);

        const float* src = samples_buffer.data();
        unsigned char* dst = raw_data.data();

        // Prefetch des données
        __builtin_prefetch(src, 0, 3);
        __builtin_prefetch(dst, 1, 3);

        // Traitement par blocs pour optimiser le cache
        const size_t block_size = 1024; // Taille de bloc optimale pour L1 cache

        for (size_t block_start = 0; block_start < size; block_start += block_size) {
            size_t block_end = std::min(block_start + block_size, size);

            // Prefetch du prochain bloc
            if (block_end < size) {
                __builtin_prefetch(&src[block_end], 0, 3);
                __builtin_prefetch(&dst[block_end], 1, 3);
            }

            // Traitement du bloc actuel
            for (size_t i = block_start; i < block_end; i++) {
                dst[i] = float_to_mulaw_table[FloatToTableIndex(src[i])];
            }
        }
    }


    unsigned char MuLawCodec::encodePcm16ToMuLaw(int16_t pcm16) {
        int16_t sign = (pcm16 >> 8) & 0x80;  // Bit de signe
        if (sign) {
            pcm16 = -pcm16;  // Valeur absolue
        }

        if (pcm16 > MU_LAW_CLIP) {
            pcm16 = MU_LAW_CLIP;
        }

        pcm16 += MU_LAW_BIAS;

        int16_t exponent = 7; int16_t mantissa;

        // Trouver l'exposant
        for (int16_t exp_lut = 0x4000; pcm16 < exp_lut && exponent > 0; exponent--, exp_lut >>= 1) {
            // Continue jusqu'à trouver le bon exposant
        }

        mantissa = (pcm16 >> (exponent + 3)) & 0x0F;

        unsigned char mulaw = ~(sign | (exponent << 4) | mantissa);

        return mulaw;
    }

    const int16_t MuLawCodec::mulaw_decode_table[256] = {
        -32124, -31100, -30076, -29052, -28028, -27004, -25980, -24956,
        -23932, -22908, -21884, -20860, -19836, -18812, -17788, -16764,
        -15996, -15484, -14972, -14460, -13948, -13436, -12924, -12412,
        -11900, -11388, -10876, -10364, -9852,  -9340,  -8828,  -8316,
        -7932,  -7676,  -7420,  -7164,  -6908,  -6652,  -6396,  -6140,
        -5884,  -5628,  -5372,  -5116,  -4860,  -4604,  -4348,  -4092,
        -3900,  -3772,  -3644,  -3516,  -3388,  -3260,  -3132,  -3004,
        -2876,  -2748,  -2620,  -2492,  -2364,  -2236,  -2108,  -1980,
        -1884,  -1820,  -1756,  -1692,  -1628,  -1564,  -1500,  -1436,
        -1372,  -1308,  -1244,  -1180,  -1116,  -1052,  -988,   -924,
        -876,   -844,   -812,   -780,   -748,   -716,   -684,   -652,
        -620,   -588,   -556,   -524,   -492,   -460,   -428,   -396,
        -372,   -356,   -340,   -324,   -308,   -292,   -276,   -260,
        -244,   -228,   -212,   -196,   -180,   -164,   -148,   -132,
        -120,   -112,   -104,   -96,    -88,    -80,    -72,    -64,
        -56,    -48,    -40,    -32,    -24,    -16,    -8,     0,
        32124,  31100,  30076,  29052,  28028,  27004,  25980,  24956,
        23932,  22908,  21884,  20860,  19836,  18812,  17788,  16764,
        15996,  15484,  14972,  14460,  13948,  13436,  12924,  12412,
        11900,  11388,  10876,  10364,  9852,   9340,   8828,   8316,
        7932,   7676,   7420,   7164,   6908,   6652,   6396,   6140,
        5884,   5628,   5372,   5116,   4860,   4604,   4348,   4092,
        3900,   3772,   3644,   3516,   3388,   3260,   3132,   3004,
        2876,   2748,   2620,   2492,   2364,   2236,   2108,   1980,
        1884,   1820,   1756,   1692,   1628,   1564,   1500,   1436,
        1372,   1308,   1244,   1180,   1116,   1052,   988,    924,
        876,    844,    812,    780,    748,    716,    684,    652,
        620,    588,    556,    524,    492,    460,    428,    396,
        372,    356,    340,    324,    308,    292,    276,    260,
        244,    228,    212,    196,    180,    164,    148,    132,
        120,    112,    104,    96,     88,     80,     72,     64,
        56,     48,     40,     32,     24,     16,     8,      0
    };

    void MuLawCodec::InitializeEncodeTable() {
        if (!encode_table_initialized) {
            for (int i = -32768; i <= 32767; i++) {
                int16_t pcm16 = static_cast<int16_t>(i);
                unsigned int index = static_cast<unsigned int>(i + 32768); // Décalage pour index positif
                float_to_mulaw_table[index] = encodePcm16ToMuLaw(pcm16);
            }
            encode_table_initialized = true;
        }
    }

    unsigned int MuLawCodec::FloatToTableIndex(float f) {
        f = std::clamp(f, -1.0f, 1.0f);
        int16_t pcm16 = static_cast<int16_t>(f * 32767.0f);
        return static_cast<unsigned int>(pcm16 + 32768);
    }

    unsigned char MuLawCodec::float_to_mulaw_table[65536]; // 16-bit range
    bool MuLawCodec::encode_table_initialized;
} // GlitchArtist