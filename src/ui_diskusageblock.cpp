#include "../includes/simplifile/ui_diskusageblock.h"

void drawDiskUsageBar(const std::filesystem::path& path) {
    static std::filesystem::space_info last_space;
    static std::filesystem::path last_path;
    static float timer = 0.0f;

    timer += ImGui::GetIO().DeltaTime;

    if (timer > 1.0f || path != last_path) {
        timer = 0.0f;
        last_path = path;
        last_space = std::filesystem::space(path);
    }

    uintmax_t total = last_space.capacity;
    uintmax_t free  = last_space.free;
    uintmax_t used  = total - free;

    float used_ratio = (total > 0) ? (float)used / (float)total : 0.0f;

    ImGui::Text("Storage used:");

    ImGui::PushStyleColor(ImGuiCol_PlotHistogram,
        used_ratio > 0.9f
            ? IM_COL32(220, 60, 60, 255)
            : IM_COL32(60, 200, 120, 255));

    ImGui::ProgressBar(used_ratio, ImVec2(-1, 8), "");

    ImGui::PopStyleColor();

    ImGui::Text("%.2fGB / %.2fGB", (total / (1024.0 * 1024.0 * 1024.0))- (free / (1024.0 * 1024.0 * 1024.0)), total / (1024.0 * 1024.0 * 1024.0));

}