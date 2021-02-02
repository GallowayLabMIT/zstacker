#pragma once

#include <cstdint>
#include <vector>
#include <filesystem>

class ArgLoader
{
public:
    ArgLoader(int argc, char* argv[]);
    
    std::vector<std::filesystem::path> inputFolders;
    std::filesystem::path outputFile;
    double z_scale;
    double threshold;
    bool exitEarly;
};
