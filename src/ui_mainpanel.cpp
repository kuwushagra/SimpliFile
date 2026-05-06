#include "../includes/simplifile/ui_popupabout.h"
#include "../includes/simplifile/be_openfunctions.h"
#include "../includes/simplifile/ui_popupoperations.h"

std::vector<fs::directory_entry> list_directory(const fs::path& dir) {
    std::vector<fs::directory_entry> entries;
    if (fs::exists(dir) && fs::is_directory(dir)) {
        for (auto& entry : fs::directory_iterator(dir))
            entries.push_back(entry);
    }
    return entries;
}

void uiMainPanel() {
    ImGui::BeginChild("MainPanel", ImVec2(0, 0), true);
    ImGui::Text("%s", current_dir.string().c_str());
    ImGui::PushStyleColor(ImGuiCol_Separator, ImVec4(1.0f, 0.85f, 0.1f, 1.0f));
    ImGui::Separator();
    ImGui::PopStyleColor();
    ImGui::Spacing();
    auto entries = list_directory(current_dir);

    for (auto& entry : entries) {
        std::string name = entry.path().filename().string();

        const char* icon = entry.is_directory() ? u8"\uf07b" : u8"\uf016";

        ImGui::PushID(name.c_str());

        std::string label = std::string(icon) + " " + name;

        if (ImGui::Selectable(label.c_str(), false)) {
            if (entry.is_directory()) {
                current_dir = entry.path();
            } else {
                open_file(entry.path());
            }
        }

        ImGui::PopID();
        
        popupOperations(name, entry);
        
    }
    ImGui::Spacing();
    ImGui::EndChild();
}