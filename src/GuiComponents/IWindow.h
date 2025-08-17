//
// Created by Gabriel Salach on 17/08/2025.
//

#ifndef GLITCHARTIST_IWINDOW_H
#define GLITCHARTIST_IWINDOW_H

class IWindow {
public:
    virtual ~IWindow() {}
    virtual void RenderWindow() = 0;
};

#endif //GLITCHARTIST_IWINDOW_H