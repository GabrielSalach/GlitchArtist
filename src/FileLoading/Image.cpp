//
// Created by Gabriel Salach on 17/08/2025.
//

#include "Image.h"

namespace GlitchArtist {
    Image::Image(IFormatLoader *format_loader) {
        this->format_loader = format_loader;
    }

    void Image::LoadImage(const std::string &filename) const {
        format_loader->LoadImage(filename);
    }

    GLuint Image::GetTexture() {
        return format_loader->GetTexture();
    }
} // GlitchArtist