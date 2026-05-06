#include "../includes/simplifile/ui_popupoperations.h"

// Cut, Copy, Rename & Delete Popup
void popupOperations(const std::string& name, const fs::directory_entry& entry) {
    if (ImGui::BeginPopupContextItem(name.c_str())) {
        if (ImGui::MenuItem("Cut")) clipboard = { entry.path(), true };
        if (ImGui::MenuItem("Copy")) clipboard = { entry.path(), false };

        if (ImGui::MenuItem("Rename")) {
            rename_active = true;
            rename_target = entry.path();
            strncpy(rename_buffer, name.c_str(), sizeof(rename_buffer));
        }
        if (ImGui::MenuItem("Delete")) {
            try {
                if (fs::is_directory(entry.path())) {
                    fs::remove_all(entry.path()); // folder
                } else {
                    fs::remove(entry.path()); // file
                }
            } catch (const std::exception& e) {
                std::cerr << "Delete error: " << e.what() << std::endl;
            }
        }
        ImGui::EndPopup();
    }
}