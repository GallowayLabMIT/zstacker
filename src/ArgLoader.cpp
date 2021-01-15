#include "ArgLoader.h"
/*
 class ArgLoader
 {
 public:
     ArgLoader(int argc, char* argv[]);
     
     std::filesystem::path inputFolder;
     std::filesystem::path outputFile;
     double threshold;
 };
*/
#include <sstream>
#include <iostream>

ArgLoader::ArgLoader(int argc, char* argv[])
{
    inputFolder = std::filesystem::path(argv[1]);
    outputFile = std::filesystem::path(argv[2]);
    std::istringstream thresholdParser(argv[3]);
    thresholdParser >> threshold;
    if (thresholdParser.fail())
    {
        std::cerr << "Threshold Parser has Failed";
    }
    else
    {
        std::cout << threshold << "\n";
    }
}
