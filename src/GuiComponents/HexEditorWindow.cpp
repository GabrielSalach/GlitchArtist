//
// Created by Gabriel Salach on 19/08/2025.
//

#include "HexEditorWindow.h"

#include <iomanip>
#include <sstream>

#include "../FileLoading/Image.h"

namespace GlitchArtist {
    HexEditorWindow::HexEditorWindow(Image *image) {
        this->image = image;
    }

    const char* toHex(const unsigned char* data, size_t len) {
        // chaque byte = 2 caractères + '\0'
        char* out = (char*)malloc(len * 2 + 1);
        if (!out) return nullptr;

        for (size_t i = 0; i < len; i++) {
            std::sprintf(out + i * 2, "%02X caca", data[i]);
        }

        out[len * 2] = '\0';
        return out; // ⚠️ penser à free() après utilisation
    }

    void HexEditorWindow::RenderWindow() {
        ImGui::Begin("Hex Editor");
        if (image != nullptr && image->IsValid()) {
            const char* out = toHex(image->GetData()->data(), image->GetData()->size());
            ImGui::TextWrapped(out);
            delete out;
        }
        ImGui::End();
    }
} // GlitchArtist
