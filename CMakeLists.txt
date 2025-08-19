cmake_minimum_required(VERSION 4.0)
project(GlitchArtist)

set(CMAKE_CXX_STANDARD 20)

add_executable(GlitchArtist main.cpp
        src/Application.cpp
        src/Application.h
        src/GuiComponents/PreviewWindow.cpp
        src/GuiComponents/PreviewWindow.h
        src/GuiComponents/IWindow.h
        src/FileLoading/BMPLoader.cpp
        src/FileLoading/BMPLoader.h
        src/FileLoading/IFormatLoader.h
        src/FileLoading/Image.cpp
        src/FileLoading/Image.h
        src/Effects/EffectStack.cpp
        src/Effects/EffectStack.h
        src/Effects/IEffect.h
        src/AudioEncoding/IAudioCodec.h
        src/AudioEncoding/MuLawCodec.cpp
        src/AudioEncoding/MuLawCodec.h
        src/GuiComponents/EffectStackWindow.cpp
        src/GuiComponents/EffectStackWindow.h
        src/Effects/Reverb.cpp
        src/Effects/Reverb.h
        src/Effects/Delay.cpp
        src/Effects/Delay.h
        src/Effects/Phaser.cpp
        src/Effects/Phaser.h
        src/GuiComponents/MainMenuBar.cpp
        src/GuiComponents/MainMenuBar.h
        src/Helpers/ImGuiHelper.h
        src/Effects/Distorsion.cpp
        src/Effects/Distorsion.h
        src/GuiComponents/AudioWindow.cpp
        src/GuiComponents/AudioWindow.h
)
find_package(glad CONFIG REQUIRED)
target_link_libraries(GlitchArtist PRIVATE glad::glad)
find_package(glfw3 CONFIG REQUIRED)
target_link_libraries(GlitchArtist PRIVATE glfw)
find_package(imgui CONFIG REQUIRED)
target_link_libraries(GlitchArtist PRIVATE imgui::imgui)
find_package(nfd CONFIG REQUIRED)
target_link_libraries(GlitchArtist PRIVATE nfd::nfd)
