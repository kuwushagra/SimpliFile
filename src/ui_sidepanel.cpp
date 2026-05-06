#include "../includes/simplifile/ui_sidepanel.h"
#include "../includes/simplifile/ui_diskusageblock.h"
#include "../includes/simplifile/be_openfunctions.h"

// Side Panel
void uiSidePanel(float leftWidth) {
    ImGui::BeginChild("Sidebar", ImVec2(leftWidth, 0), true);

    ImGui::Text("Quick Access");
    ImGui::PushStyleColor(ImGuiCol_Separator, ImVec4(1.0f, 0.85f, 0.1f, 1.0f));
    ImGui::Separator();
    ImGui::PopStyleColor();
    ImGui::Spacing();
    
    if (ImGui::Button("\uf015 Home")) current_dir = fs::path(getenv("HOME"));
    if (ImGui::Button("\uf03e Desktop")) current_dir = fs::path(getenv("HOME")) / "Desktop";
    if (ImGui::Button("\uf02d Documents")) current_dir = fs::path(getenv("HOME")) / "Documents";
    if (ImGui::Button("\uf019 Downloads")) current_dir = fs::path(getenv("HOME")) / "Downloads";
    if (ImGui::Button("\uf030 Pictures")) current_dir = fs::path(getenv("HOME")) / "Pictures";
    if (ImGui::Button("\uf03d Videos")) current_dir = fs::path(getenv("HOME")) / "Videos";
    if (ImGui::Button("\uf001 Music")) current_dir = fs::path(getenv("HOME")) / "Music";

    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.6f, 0.1f, 0.1f, 1));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.8f, 0.2f, 0.2f, 1));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.5f, 0.05f, 0.05f, 1));

    if (ImGui::Button("\uf071 [!] Root"))
        current_dir = fs::path("/");

    ImGui::PopStyleColor(3);

    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();

    if (ImGui::Button("\uf062")) {
        if (current_dir.has_parent_path()) {
        current_dir = current_dir.parent_path();
        }
    
    }

    ImGui::SameLine();

    if (ImGui::Button("\uf07c")) {
        try {
            fs::path new_dir = current_dir / "New Folder";

            int i = 1;
            while (fs::exists(new_dir)) {
                new_dir = current_dir / ("New Folder (" + std::to_string(i++) + ")");
            }

            fs::create_directory(new_dir);
        }
        catch (const std::exception& e) {
            std::cerr << "Create folder error: " << e.what() << std::endl;
        }
    }

    ImGui::SameLine();

    if (!clipboard.path.empty()) {
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.1f, 0.6f, 0.2f, 1));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.2f, 0.8f, 0.3f, 1));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.05f, 0.5f, 0.15f, 1));

        if (ImGui::Button("\uf0ea")) {

            fs::path dest = current_dir / clipboard.path.filename();

            try {
                if (clipboard.cut) {
                    fs::rename(clipboard.path, dest);
                    clipboard = {};
                } 
                else {
                    if (fs::is_directory(clipboard.path)) {
                        fs::copy(
                            clipboard.path,
                            dest,
                            fs::copy_options::recursive | fs::copy_options::overwrite_existing
                        );
                    } 
                    else {
                        fs::copy_file(
                            clipboard.path,
                            dest,
                            fs::copy_options::overwrite_existing
                        );
                    }
                }
            } 
            catch (const std::exception& e) {
                std::cerr << "Paste error: " << e.what() << std::endl;
            }
        }
        ImGui::PopStyleColor(3);
    }

    ImGui::NewLine();

    ImGui::SetCursorPosY(ImGui::GetWindowHeight() - ImGui::GetTextLineHeightWithSpacing() - 95);
    //ImGui::SetCursorPosX(ImGui::GetWindowWidth() - ImGui::CalcTextSize("uuf0+v1.0.1").x - 17);
    drawDiskUsageBar(current_dir);
    ImGui::Spacing();

    if (ImGui::Button("\uf05a About ")) about_active = true;
    ImGui::SameLine();
    if (ImGui::Button("\uf09b")) open_web_link("https://www.github.com/kuwushagra/SimpliFile");
    


    ImGui::EndChild();
}