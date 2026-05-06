#include "../includes/simplifile/ui_titlebar.h"

// Title Bar
void uiTitleBar() {
    ImGui::BeginChild("TitleBar", ImVec2(0, 25), false);

    ImGui::PushFont(fontTitle);
    ImGui::Text(" SimpliFile");
    ImGui::PopFont();

    ImGui::SameLine(ImGui::GetWindowWidth() - 50);

    /* Custom title bar buttons will be disabled if running in wayland 
    since the behaviour is unpredictable. On other systems it will show */ 
    
    // if (glfwGetPlatform() != GLFW_PLATFORM_WAYLAND)

    /* Check removed since GitHub CD is not happy with it ;) */

    if (!customUIButtonsDisabled) {
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1.00f, 0.85f, 0.10f, 1));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(1.00f, 0.90f, 0.25f, 1));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.90f, 0.75f, 0.05f, 1));

        
        if (ImGui::Button("##-", ImVec2(10, 15)))
            glfwIconifyWindow(window);

        ImGui::SameLine();
        ImGui::PopStyleColor(3);
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.1f, 0.6f, 0.2f, 1));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.2f, 0.8f, 0.3f, 1));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.05f, 0.5f, 0.15f, 1));

        if (ImGui::Button("##o", ImVec2(10, 15))) {
            fullscreen = !fullscreen;

            if (fullscreen) {
                GLFWmonitor* monitor = glfwGetPrimaryMonitor();
                const GLFWvidmode* mode = glfwGetVideoMode(monitor);
                glfwSetWindowMonitor(window, monitor, 0, 0,
                    mode->width, mode->height, mode->refreshRate);
            } else {
                glfwSetWindowMonitor(window, NULL, 100, 100, 1000, 650, 0);
            }
        }
        ImGui::PopStyleColor(3);
        ImGui::SameLine();
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.6f, 0.1f, 0.1f, 1));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.8f, 0.2f, 0.2f, 1));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.5f, 0.05f, 0.05f, 1));
        if (ImGui::Button("##x", ImVec2(10, 15)))
            glfwSetWindowShouldClose(window, true);

        ImGui::PopStyleColor(3);
    }
    ImGui::EndChild();
}