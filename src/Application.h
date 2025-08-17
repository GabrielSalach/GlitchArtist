//
// Created by Gabriel Salach on 17/08/2025.
//

#ifndef GLITCHARTIST_APPLICATION_H
#define GLITCHARTIST_APPLICATION_H

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <vector>

#include "Effects/EffectStack.h"
#include "GuiComponents/IWindow.h"
#include "FileLoading/Image.h"

namespace GlitchArtist {
    class Application {
    public:
        void Init(GLFWwindow* window, const char* glsl_version);
        void NewFrame();
        void Update();
        void Render();
        void Shutdown();
    private:
        void BindWindows();
        void LoadImage();
        void LoadEffectStack();

        std::vector<std::unique_ptr<IWindow>> windows;
        // const std::string image_path = "/Users/gabrielsalach/Desktop/test.bmp";
        const std::string image_path = "/Users/gabrielsalach/Desktop/test3.bmp";
        Image* image = nullptr;
        EffectStack* effect_stack = nullptr;
    };
} // GlitchArtist

#endif //GLITCHARTIST_APPLICATION_H