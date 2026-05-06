#include "../includes/simplifile/be_openfunctions.h"

namespace fs = std::filesystem;

void open_file(const fs::path& path) {
    std::string cmd = "xdg-open \"" + path.string() + "\"";
    system(cmd.c_str());
}

void open_web_link(const std::string& url) {
    std::string cmd = "xdg-open \"" + url + "\"";  // For Linux systems
    system(cmd.c_str());
}