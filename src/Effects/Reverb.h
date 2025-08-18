//
// Created by Gabriel Salach on 17/08/2025.
//

#ifndef GLITCHARTIST_REVERB_H
#define GLITCHARTIST_REVERB_H
#include <vector>

#include "IEffect.h"

namespace GlitchArtist {
    class Reverb : public IEffect {
    public:
        explicit Reverb(float smpl_rt = 41000.0f);

        void RenderUI();
        void ApplyEffect(std::vector<float>& samples);

    private:
        // Paramètres audio
        float sampleRate;

        // Paramètres de la reverb
        float roomSize = 0.5f;      // Taille de la pièce (0-1)
        float damping = 0.5f;       // Amortissement des hautes fréquences (0-1)
        float width = 1.0f;         // Largeur stéréo (0-1)
        float mixLevel = 0.3f;      // Mix dry/wet (0-1)

        // Structure pour un délai avec filtre
        struct DelayLine {
            std::vector<float> buffer;
            size_t writeIndex = 0;
            size_t length;
            float feedback = 0.0f;
            float dampingState = 0.0f;  // État du filtre passe-bas

            DelayLine(size_t len) : length(len) {
                buffer.resize(len, 0.0f);
            }

            float process(float input, float feedbackAmount, float dampingAmount) {
                // Lire la sortie
                float output = buffer[writeIndex];

                // Filtre passe-bas simple pour le damping
                dampingState += (output - dampingState) * (1.0f - dampingAmount);

                // Écrire avec feedback filtré
                buffer[writeIndex] = input + dampingState * feedbackAmount;

                // Avancer l'index
                writeIndex = (writeIndex + 1) % length;

                return output;
            }
        };

        // Algorithme Freeverb simplifié
        static constexpr int NUM_COMBS = 8;
        static constexpr int NUM_ALLPASS = 4;

        std::array<DelayLine, NUM_COMBS> combDelays;
        std::array<DelayLine, NUM_ALLPASS> allpassDelays;

        // Longueurs des délais (en échantillons) - valeurs typiques Freeverb
        static constexpr std::array<int, NUM_COMBS> combLengths = {
            1116, 1188, 1277, 1356, 1422, 1491, 1557, 1617
        };

        static constexpr std::array<int, NUM_ALLPASS> allpassLengths = {
            556, 441, 341, 225
        };

        void updateParameters();
        float processAllpass(float input, DelayLine& delay);
    };

} // GlitchArtist

#endif //GLITCHARTIST_REVERB_H