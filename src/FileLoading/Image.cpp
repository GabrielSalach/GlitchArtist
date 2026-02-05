//
// Created by Gabriel Salach on 17/08/2025.
//

#include "Image.h"
#ifdef _WIN32
#include <string>
#endif

namespace GlitchArtist {
    Image::Image(IFormatLoader *format_loader) {
        this->format_loader = format_loader;
    }

    void Image::LoadImage(const std::string &filename) const {
        format_loader->LoadImage(filename);
    }

    ImVec2 Image::GetSize() const {
    }

    GLuint Image::GetTexture() {
        return format_loader->GetTexture();
    }

    std::vector<unsigned char> * Image::GetData() {
        return format_loader->GetData();
    }

    bool Image::IsValid() const {
        return format_loader->GetData() != nullptr;
    }
} // GlitchArtist