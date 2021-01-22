#include "ArgLoader.h"
/*
 class ArgLoader
 {
 public:
     ArgLoader(int argc, char* argv[]);
     
     std::filesystem::path inputFolder;
     std::filesystem::path outputFile;
    double z_scale;
    double threshold;
 };
*/
#include <sstream>
#include <iostream>

ArgLoader::ArgLoader(int argc, char* argv[])
{
    inputFolder = std::filesystem::path(argv[1]);
    outputFile = std::filesystem::path(argv[2]);
    std::istringstream zscaleParser(argv[3]);
    zscaleParser >> z_scale;
    if (zscaleParser.fail())
    {
        std::cerr << "Z-Scale Parser has Failed";
    }
    else
    {
        std::cout << z_scale << "\n";
    }
    std::istringstream thresholdParser(argv[4]);
    thresholdParser >> threshold;
    if (thresholdParser.fail())
    {
        std::cerr << "Threshold Parser for has Failed";
    }
    else
    {
        std::cout << threshold << "\n";
    }
}
