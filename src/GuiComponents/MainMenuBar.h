//
// Created by Gabriel Salach on 18/08/2025.
//

#ifndef GLITCHARTIST_MAINMENUBAR_H
#define GLITCHARTIST_MAINMENUBAR_H
#include "IWindow.h"

namespace GlitchArtist {
    class Application;
    class MainMenuBar : public IWindow {
    public:
        explicit MainMenuBar(Application* app);
        void RenderWindow() override;
    private:
        Application* application;
    };
} // GlitchArtist

#endif //GLITCHARTIST_MAINMENUBAR_H