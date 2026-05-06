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

struct ClipboardItem {
    fs::path path;
    bool cut = false;
};
ClipboardItem clipboard;

bool rename_active = false;
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

void ApplyCustomStyle() {
    ImGuiStyle& style = ImGui::GetStyle();

    style.WindowRounding = 10.0f;
    style.ChildRounding = 8.0f;
    style.FrameRounding = 6.0f;
    style.PopupRounding = 8.0f;

    style.WindowPadding = ImVec2(12, 12);
    style.FramePadding = ImVec2(10, 6);
    style.ItemSpacing = ImVec2(10, 8);

    ImVec4* colors = style.Colors;

    // PURE BLACK
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

    glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
    const char* glsl_version = "#version 130";

    GLFWwindow* window = glfwCreateWindow(800, 600, "SimpliFile - The Simple File Manager", NULL, NULL);
    if (!window) return 1;

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();

    ApplyCustomStyle();

    // Font
    std::filesystem::path exe = std::filesystem::canonical("/proc/self/exe");
    std::filesystem::path base = exe.parent_path();

    ImFont* fontDef = io.Fonts->AddFontFromFileTTF(
        (base / "includes/fonts/Onest-VariableFont_wght.ttf").string().c_str(), 18.0f);

    ImFont* fontTitle = io.Fonts->AddFontFromFileTTF(
        (base / "includes/fonts/Unbounded-SemiBold.ttf").string().c_str(), 22.0f);

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

        // ===== Custom Title Bar =====
        ImGui::BeginChild("TitleBar", ImVec2(0, 25), false);

        ImGui::PushFont(fontTitle);
        ImGui::Text(" SimpliFile");
        ImGui::PopFont();

        // Drag window
        if (ImGui::IsItemActive() && ImGui::IsMouseDragging(0)) {
            ImVec2 delta = ImGui::GetIO().MouseDelta;
            int x, y;
            glfwGetWindowPos(window, &x, &y);
            glfwSetWindowPos(window, x + (int)delta.x, y + (int)delta.y);
        }

        ImGui::SameLine(ImGui::GetWindowWidth() - 50);

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
        ImGui::EndChild();

        // ===== Layout =====
        float totalWidth = ImGui::GetContentRegionAvail().x;
        float leftWidth = totalWidth * 0.325f;

        // Sidebar
        ImGui::BeginChild("Sidebar", ImVec2(leftWidth, 0), true);

        ImGui::Text("Quick Access");
        ImGui::PushStyleColor(ImGuiCol_Separator, ImVec4(1.0f, 0.85f, 0.1f, 1.0f));
        ImGui::Separator();
        ImGui::PopStyleColor();
        ImGui::Spacing();
        
        if (ImGui::Button("Home")) current_dir = fs::path(getenv("HOME"));
        if (ImGui::Button("Desktop")) current_dir = fs::path(getenv("HOME")) / "Desktop";
        if (ImGui::Button("Documents")) current_dir = fs::path(getenv("HOME")) / "Documents";
        if (ImGui::Button("Downloads")) current_dir = fs::path(getenv("HOME")) / "Downloads";
        if (ImGui::Button("Pictures")) current_dir = fs::path(getenv("HOME")) / "Pictures";
        if (ImGui::Button("Videos")) current_dir = fs::path(getenv("HOME")) / "Videos";
        if (ImGui::Button("Music")) current_dir = fs::path(getenv("HOME")) / "Music";

        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.6f, 0.1f, 0.1f, 1));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.8f, 0.2f, 0.2f, 1));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.5f, 0.05f, 0.05f, 1));

        if (ImGui::Button("Root"))
            current_dir = fs::path("/");

        ImGui::PopStyleColor(3);

        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Spacing();

        if (ImGui::Button("Up")) {
            if (current_dir.has_parent_path()) {
            current_dir = current_dir.parent_path();
            }
        
        }

        ImGui::SameLine();

        if (ImGui::Button("New Folder")) {
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

            if (ImGui::Button("Paste")) {

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
                            fs::remove_all(entry.path()); //folder
                        } else {
                            fs::remove(entry.path()); //file
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

            if (ImGui::BeginPopupModal("Rename Item", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
                ImGui::InputText("New name", rename_buffer, IM_ARRAYSIZE(rename_buffer));

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
        }

        ImGui::End();

        // Render
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