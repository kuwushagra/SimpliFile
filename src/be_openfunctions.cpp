#include "../includes/simplifile/be_openfunctions.h"

namespace fs = std::filesystem;

void open_file(const fs::path& path) {
    #if defined(_WIN32) || defined(_WIN64)
        std::string cmd = "start \"\" \"" + path.string() + "\"";
    #elif defined(__APPLE__)
        std::string cmd = "open \"" + path.string() + "\"";
    #else 
        std::string cmd = "xdg-open \"" + path.string() + "\"";
    #endif
        system(cmd.c_str());
}

void open_web_link(const std::string& url) {
    #if defined(_WIN32) || defined(_WIN64)
        std::string cmd = "start \"\" \"" + url + "\"";
    #elif defined(__APPLE__)
        std::string cmd = "open \"" + url + "\"";
    #else 
        std::string cmd = "xdg-open \"" + url + "\"";
    #endif
        system(cmd.c_str());
}

fs::path getBasePath() {
    #ifdef _WIN32
        char buffer[MAX_PATH];
        GetModuleFileNameA(NULL, buffer, MAX_PATH);
        fs::path exe(buffer);
    #elif defined(__APPLE__)
        char buffer[PATH_MAX];
        uint32_t size = sizeof(buffer);
        if(_NSGetExecutablePath(buffer, &size) != 0) {
            throw std::runtime_error("Failed to get executable path ;(";
        }
        fs::path exe = fs::canonical(buffer);
    #else
        fs::path exe = std::filesystem::canonical("/proc/self/exe");
    #endif

    return exe.parent_path();
}
