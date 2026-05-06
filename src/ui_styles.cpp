#include "../includes/simplifile/ui_styles.h"

void applyCustomStyle() {
    ImGuiStyle& style = ImGui::GetStyle();

    style.WindowRounding = 10.0f;
    style.ChildRounding = 8.0f;
    style.FrameRounding = 6.0f;
    style.PopupRounding = 8.0f;

    style.WindowPadding = ImVec2(12, 12);
    style.FramePadding = ImVec2(10, 6);
    style.ItemSpacing = ImVec2(10, 8);

    ImVec4* colors = style.Colors;

    colors[ImGuiCol_WindowBg] = ImVec4(0, 0, 0, 1);
    colors[ImGuiCol_ChildBg]  = ImVec4(0, 0, 0, 1);

    colors[ImGuiCol_FrameBg]        = ImVec4(0.15f, 0.15f, 0.15f, 1);
    colors[ImGuiCol_FrameBgHovered] = ImVec4(0.25f, 0.25f, 0.25f, 1);

    colors[ImGuiCol_Button]         = ImVec4(0.2f, 0.2f, 0.2f, 1);
    colors[ImGuiCol_ButtonHovered]  = ImVec4(0.3f, 0.3f, 0.3f, 1);
    colors[ImGuiCol_ButtonActive]   = ImVec4(0.25f, 0.25f, 0.25f, 1);

    colors[ImGuiCol_Header]         = ImVec4(0.2f, 0.2f, 0.2f, 1);
    colors[ImGuiCol_HeaderHovered]  = ImVec4(0.3f, 0.3f, 0.3f, 1);
}
