#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>
#include <filesystem>
#include <string>
#include <vector>
#include <iostream>

namespace fs = std::filesystem;

void open_file(const fs::path& path) {
    std::string cmd = "xdg-open \"" + path.string() + "\"";
    system(cmd.c_str());
}

void open_web_link(const std::string& url) {
    std::string cmd = "xdg-open \"" + url + "\"";  // For Linux systems
    system(cmd.c_str());
}

struct ClipboardItem {
    fs::path path;
    bool cut = false;
};
ClipboardItem clipboard;

bool rename_active = false;
bool about_active = false;
fs::path rename_target;
char rename_buffer[256];

std::vector<fs::directory_entry> list_directory(const fs::path& dir) {
    std::vector<fs::directory_entry> entries;
    if (fs::exists(dir) && fs::is_directory(dir)) {
        for (auto& entry : fs::directory_iterator(dir))
            entries.push_back(entry);
    }
    return entries;
}

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

int main() {
    setenv("GLFW_PLATFORM", "x11", 1);

    if (!glfwInit()) return 1;

    if (glfwGetPlatform() != GLFW_PLATFORM_WAYLAND) glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
    else glfwWindowHint(GLFW_DECORATED, GLFW_TRUE);

    const char* glsl_version = "#version 130";

    GLFWwindow* window = glfwCreateWindow(800, 600, "SimpliFile - The Simple File Manager", NULL, NULL);
    if (!window) return 1;

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();

    applyCustomStyle();

    // Load Font
    std::filesystem::path exe = std::filesystem::canonical("/proc/self/exe");
    std::filesystem::path base = exe.parent_path();

    ImFont* fontTitle = io.Fonts->AddFontFromFileTTF(
        (base / "includes/fonts/Unbounded-SemiBold.ttf").string().c_str(), 22.0f);
    
    ImFont* fontVersion = io.Fonts->AddFontFromFileTTF(
        (base / "includes/fonts/Unbounded-SemiBold.ttf").string().c_str(), 14.0f);

    ImFont* fontDef = io.Fonts->AddFontFromFileTTF(
        (base / "includes/fonts/Onest-VariableFont_wght.ttf").string().c_str(), 18.0f);

    static const ImWchar icons_ranges[] = { 0xf000, 0xf8ff, 0 };

    ImFontConfig config;
    config.MergeMode = true;
    config.PixelSnapH = true;
    io.Fonts->AddFontFromFileTTF("includes/fonts/FontAwesome.ttf", 14.0f, &config, icons_ranges);
        
    io.FontDefault = fontDef;

    if (io.Fonts->Fonts.empty())
        io.FontDefault = io.Fonts->AddFontDefault();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    fs::path current_dir = fs::current_path();

    bool fullscreen = false;

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::SetNextWindowPos(ImVec2(0, 0));
        ImGui::SetNextWindowSize(io.DisplaySize);

        ImGui::Begin("Main", nullptr,
            ImGuiWindowFlags_NoResize |
            ImGuiWindowFlags_NoCollapse |
            ImGuiWindowFlags_NoMove |
            ImGuiWindowFlags_NoTitleBar);

        // Title Bar
        ImGui::BeginChild("TitleBar", ImVec2(0, 25), false);

        ImGui::PushFont(fontTitle);
        ImGui::Text(" SimpliFile");
        ImGui::PopFont();

        ImGui::SameLine(ImGui::GetWindowWidth() - 50);

    /* Custom title bar buttons will be disabled if running in wayland 
    since the behaviour is unpredictable. On other systems it will show */  

        if (glfwGetPlatform() != GLFW_PLATFORM_WAYLAND) {
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

        // ====================== Application Split Layout =============================
        float totalWidth = ImGui::GetContentRegionAvail().x;
        float leftWidth = totalWidth * 0.19f;

        // Side panel
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

        if (ImGui::Button("\uf07b (!) Root"))
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

        if (ImGui::Button("\uf07b")) {
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
        ImGui::SetCursorPosY(ImGui::GetWindowHeight() - ImGui::GetTextLineHeightWithSpacing() - 17);
        //ImGui::SetCursorPosX(ImGui::GetWindowWidth() - ImGui::CalcTextSize("uuf0+v1.0.1").x - 17);
        if (ImGui::Button("\uf05a About ")) about_active = true;
        ImGui::SameLine();
        if (ImGui::Button("\uf09b")) open_web_link("https://www.github.com/kuwushagra/SimpliFile");
        

         // About Popup
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
                ImGui::Text("This File Manager was made as a college project and can perform very basic operations such as:\n- Creation of New Folders\n- Deletion of Folders\n- Cut/Copy/Paste (on both files and folders)\n- Renaming of files and folders\n- Navigation of folders\n- Launching/opening files of various types by double click (opens in the default app set by your DE)");
                ImGui::PushFont(fontVersion);
                ImGui::Text("Version: v1.1.0");
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

        ImGui::EndChild();

        ImGui::SameLine();

        // Main panel
        ImGui::BeginChild("MainPanel", ImVec2(0, 0), true);

        ImGui::Text("%s", current_dir.string().c_str());
        ImGui::PushStyleColor(ImGuiCol_Separator, ImVec4(1.0f, 0.85f, 0.1f, 1.0f));
        ImGui::Separator();
        ImGui::PopStyleColor();
        ImGui::Spacing();
        auto entries = list_directory(current_dir);

        for (auto& entry : entries) {
            std::string name = entry.path().filename().string();

            ImGui::Selectable(name.c_str());

            if (ImGui::IsItemHovered() &&
                ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left)) {

                if (entry.is_directory()) {
                    current_dir = entry.path();
                } else {
                    open_file(entry.path());
                }

            }
            
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
        ImGui::Spacing();
        
        ImGui::EndChild();
        

        // Rename popup
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

        ImGui::End();

        // Render window
        ImGui::Render();
        int w, h;
        glfwGetFramebufferSize(window, &w, &h);

        glViewport(0, 0, w, h);
        glClearColor(0, 0, 0, 1); 
        glClear(GL_COLOR_BUFFER_BIT);

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(window);
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}