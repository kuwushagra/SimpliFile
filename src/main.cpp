#include "../includes/simplifile/simplifile.h"
#include "../includes/simplifile/be_openfunctions.h"
#include "../includes/simplifile/ui_diskusageblock.h"
#include "../includes/simplifile/ui_styles.h"
#include "../includes/simplifile/ui_popuprename.h"
#include "../includes/simplifile/ui_popupabout.h"
#include "../includes/simplifile/ui_popupoperations.h"
#include "../includes/simplifile/ui_titlebar.h"
#include "../includes/simplifile/ui_sidepanel.h"
#include "../includes/simplifile/ui_mainpanel.h"

namespace fs = std::filesystem;

// Global Variable Declarations

ImFont* fontTitle = nullptr;
ImFont* fontVersion = nullptr;
ImFont* fontDef = nullptr;

GLFWwindow* window = nullptr;

ClipboardItem clipboard;

bool rename_active = false;
fs::path rename_target;
char rename_buffer[256];

fs::path current_dir = fs::current_path();

bool about_active = false;
bool customUIButtonsDisabled = true;
bool fullscreen = false;

///////////////////////////////

int main() {

    // GLFW Init
    setenv("GLFW_PLATFORM", "x11", 1);
    if (!glfwInit()) return 1;
    glfwWindowHint(GLFW_DECORATED, GLFW_TRUE);
    const char* glsl_version = "#version 130";
    window = glfwCreateWindow(800, 600, "SimpliFile - The Simple File Manager", NULL, NULL);
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

    fontTitle = io.Fonts->AddFontFromFileTTF(
        (base / "includes/fonts/Unbounded-SemiBold.ttf").string().c_str(), 22.0f);
    
    fontVersion = io.Fonts->AddFontFromFileTTF(
        (base / "includes/fonts/Unbounded-SemiBold.ttf").string().c_str(), 14.0f);

    fontDef = io.Fonts->AddFontFromFileTTF(
        (base / "includes/fonts/Onest-VariableFont_wght.ttf").string().c_str(), 18.0f);

    static const ImWchar icons_ranges[] = { 0xf000, 0xf8ff, 0 };
    ImFontConfig config; 
    config.MergeMode = true; 
    config.PixelSnapH = true;
    io.Fonts->AddFontFromFileTTF(
        (base / "includes/fonts/FontAwesome.ttf").string().c_str(),
        14.0f,
        &config,
        icons_ranges
    );
        
    io.FontDefault = fontDef;

    if (io.Fonts->Fonts.empty())
        io.FontDefault = io.Fonts->AddFontDefault();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    // Main app loop

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

        uiTitleBar(); 

        // Draw layout
        float totalWidth = ImGui::GetContentRegionAvail().x;
        float leftWidth = totalWidth * 0.20f;   

        popupAbout();
        uiSidePanel(leftWidth);
        ImGui::SameLine();
        uiMainPanel();
        popupRename();
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