#pragma once

#include <cstdint>
#include <filesystem>

class ArgLoader
{
public:
    ArgLoader(int argc, char* argv[]);
    
    std::filesystem::path inputFolder;
    std::filesystem::path outputFile;
    double z_scale;
    double threshold;
};
