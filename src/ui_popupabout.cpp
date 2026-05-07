#include "../includes/simplifile/ui_popupabout.h"

// About Popup
void popupAbout() {
    if (about_active) {
        ImGui::OpenPopup("About");

        ImGui::PushStyleColor(ImGuiCol_PopupBg, ImVec4(0, 0, 0, 1));
        ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0, 0, 0, 1));
        ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.15f, 0.15f, 0.15f, 1));
        ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, ImVec4(0.25f, 0.25f, 0.25f, 1));
        ImGui::PushStyleColor(ImGuiCol_FrameBgActive, ImVec4(0.30f, 0.30f, 0.30f, 1));

        ImGui::PushStyleColor(ImGuiCol_TitleBg, ImVec4(0.65f, 0.50f, 0.05f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ImVec4(0.75f, 0.58f, 0.08f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_TitleBgCollapsed, ImVec4(0.50f, 0.38f, 0.04f, 1.0f));

        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2f, 0.2f, 0.2f, 1));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.3f, 0.3f, 0.3f, 1));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.25f, 0.25f, 0.25f, 1));

        ImGui::PushFont(fontVersion);
        if (ImGui::BeginPopupModal("About", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
            ImGui::Text("SimpliFile - The Simple File Manager\n");
            ImGui::PopFont();
            ImGui::Text("This File Manager was made as a college project and can perform very basic operations such as:\n- Creation of New Folders\n- Deletion of Folders\n- Cut/Copy/Paste (on both files and folders)\n- Renaming of files and folders\n- Navigation of folders\n- Launching/opening files of various types by double click (opens in the default app set by your DE)\n- Show used space in volume");
            ImGui::Spacing();
            ImGui::PushFont(fontVersion);
            ImGui::Text("Version: v1.3.5");
            ImGui::PopFont();
            ImGui::Spacing();

            if (ImGui::Button("Ok")) {
                ImGui::CloseCurrentPopup();
                about_active = false;
            }
            
            ImGui::EndPopup();
        }
        
        ImGui::PopStyleColor(11);
    }
}
