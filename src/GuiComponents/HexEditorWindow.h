//
// Created by Gabriel Salach on 19/08/2025.
//

#ifndef GLITCHARTIST_HEXEDITORWINDOW_H
#define GLITCHARTIST_HEXEDITORWINDOW_H
#include <imgui.h>

#include "IWindow.h"

namespace GlitchArtist {
    class Image;

    class HexEditorWindow : public IWindow {
    public:
        explicit HexEditorWindow(Image* image);
        void RenderWindow() override;
    private:
        Image* image;
    };

} // GlitchArtist

#endif //GLITCHARTIST_HEXEDITORWINDOW_H