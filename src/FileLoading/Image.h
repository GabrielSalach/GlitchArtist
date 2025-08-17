//
// Created by Gabriel Salach on 17/08/2025.
//

#ifndef GLITCHARTIST_IMAGE_H
#define GLITCHARTIST_IMAGE_H
#include <vector>
#include <glad/glad.h>

#include "IFormatLoader.h"

namespace GlitchArtist {
    class Image {
    public:
        explicit Image(IFormatLoader* format_loader);
        void LoadImage(const std::string& filename) const;
        [[nodiscard]] GLuint GetTexture();
    private:
        std::vector<unsigned char>* data = nullptr;
        IFormatLoader* format_loader = nullptr;
    };
} // GlitchArtist

#endif //GLITCHARTIST_IMAGE_H