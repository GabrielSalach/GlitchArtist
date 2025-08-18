//
// Created by Gabriel Salach on 18/08/2025.
//

#ifndef GLITCHARTIST_IMGUIHELPER_H
#define GLITCHARTIST_IMGUIHELPER_H
#include <imgui.h>

static void CenterText(const char *text) {
    ImVec2 const text_size = ImGui::CalcTextSize(text);
    ImVec2 const window_size = ImGui::GetContentRegionAvail();
    ImVec2 const cursor_pos = ImGui::GetCursorPos();

    float const offset_x = (window_size.x - text_size.x) * 0.5f;
    float const offset_y = (window_size.y - text_size.y) * 0.5f;

    ImGui::SetCursorPos(ImVec2(cursor_pos.x + offset_x, cursor_pos.y + offset_y));
}

static void CenterImage(const ImVec2 image_size) {
    ImVec2 const window_size = ImGui::GetContentRegionAvail();
    ImVec2 const cursor_pos = ImGui::GetCursorPos();

    // Calcul du décalage
    float const offset_x = (window_size.x - image_size.x) * 0.5f;
    float const offset_y = (window_size.y - image_size.y) * 0.5f;

    // Applique le décalage
    ImGui::SetCursorPos(ImVec2(cursor_pos.x + offset_x, cursor_pos.y + offset_y));
}

#endif //GLITCHARTIST_IMGUIHELPER_H