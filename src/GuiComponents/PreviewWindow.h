//
// Created by Gabriel Salach on 17/08/2025.
//

#ifndef GLITCHARTIST_PREVIEWWINDOW_H
#define GLITCHARTIST_PREVIEWWINDOW_H
#include "IWindow.h"
#include "../FileLoading/Image.h"

namespace GlitchArtist {
    class PreviewWindow : public IWindow {
    public:
        explicit PreviewWindow(Image* image);
        void RenderWindow() override;
    private:
        Image* image;
    };
} // GlitchArtist

#endif //GLITCHARTIST_PREVIEWWINDOW_H