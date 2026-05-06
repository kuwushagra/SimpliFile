#include "../includes/simplifile/ui_popuprename.h"

// Rename popup 
void popupRename() {
    if (rename_active) {
        ImGui::OpenPopup("Rename Item");

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
        if (ImGui::BeginPopupModal("Rename Item", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
            ImGui::PopFont();
            ImGui::Text(" Enter the new name: ");
            ImGui::InputText("##New name", rename_buffer, IM_ARRAYSIZE(rename_buffer));
            ImGui::Spacing();

            if (ImGui::Button("OK")) {
                fs::path new_path = rename_target.parent_path() / rename_buffer;
                try { fs::rename(rename_target, new_path); }
                catch (...) {}
                rename_active = false;
                ImGui::CloseCurrentPopup();
            }

            ImGui::SameLine();

            if (ImGui::Button("Cancel")) {
                rename_active = false;
                ImGui::CloseCurrentPopup();
            }

            ImGui::EndPopup();
        }
        
        ImGui::PopStyleColor(11);
    }
}