#pragma once 
#include "simplifile.h"

namespace fs = std::filesystem;
void open_file(const fs::path& path);

void open_web_link(const std::string& url);