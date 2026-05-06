#pragma once 
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>
#include <filesystem>
#include <string>
#include <vector>
#include <iostream>

namespace fs = std::filesystem;

extern fs::path current_dir;

struct ClipboardItem {
    fs::path path;
    bool cut = false;
};
extern ClipboardItem clipboard;

extern fs::path rename_target;
extern char rename_buffer[256];
extern bool rename_active;

extern bool about_active;

extern bool customUIButtonsDisabled;
extern bool fullscreen;

extern ImFont* fontTitle;
extern ImFont* fontVersion;
extern ImFont* fontDef;

extern GLFWwindow* window;