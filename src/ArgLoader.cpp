#include "ArgLoader.h"
#include <sstream>
#include <iostream>
#include <stdexcept>

void print_usage()
{
    std::cerr << "usage: zstacker [-h] [-t pixel_threshold] [-z zscale] input_paths output_vdb\n"
        << "\n\tProcesses .tif files in 'input_paths', and outputs a combined OpenVDB volume"
        << "\n\tsuitable for rendering inside Blender and other 3D animation software packages."
        << "\n\n\tzstacker optionally allows specification of a pixel value cutoff, below which no"
        << "\n\tvolume is rendered and a zscale which modifies the z-pixel spacing."
        << "\n\npositional arguments:"
        << "\n\tinput_paths: any number of paths to folders. The folders are searched for .tif"
        << "\n\t\tfiles which are loaded in order into the zstack. If a folder is passed, .tif's are"
        << "\n\t\tloaded in alphabetic order."
        << "\n\toutput_vdb: a single path to the resulting .vdb that is created."
        << "\n\noptional arguments:"
        << "\n\t-h, --help: shows this help message and exits."
        << "\n\t-t, --threshold: sets a pixel threshold, from 0 (0%) to 255 (100%), below which pixels"
        << "\n\t\tdo not contribute to a voxel. Use to threshold away noise."
        << "\n\t-z, --zscale: Scales the spacing between z-stacked voxels by the given amount. If not given,"
        << "\n\t\tthis defaults to 1 (e.g. the x-y voxel distance is equal to the z-voxel distance).\n";
}

ArgLoader::ArgLoader(int argc, char* argv[])
{
    exitEarly = false;
    threshold = 0;
    z_scale = 1.0;
    for (int i = 1; i < argc - 1; ++i)
    {
        std::string arg(argv[i]);
        // Try to load flag arguments
        if (arg == "-h" || arg == "--help")
        {
            print_usage();
            exitEarly = true;
            return;
        } else if (arg == "-t" || arg == "--threshold") {
            if (i + 1 >= argc)
            {
                throw std::runtime_error("Must specify a threshold after the -t option");
            }

            // Try loading the threshold
            i += 1;
            std::istringstream thresholdParser(argv[i]);
            thresholdParser >> threshold;
            if (thresholdParser.fail() || threshold < 0 || threshold > 255)
            {
                throw std::runtime_error("Invalid threshold given: must be real number between 0 and 255");
            }
        }
        else if (arg == "-z" || arg == "--zscale") {
            if (i + 1 >= argc)
            {
                throw std::runtime_error("Must specify a z scale after the -z option");
            }

            // Try loading z scale
            i += 1;
            std::istringstream zscaleParser(argv[i]);
            zscaleParser >> z_scale;
            if (zscaleParser.fail() || z_scale <= 0.0)
            {
                throw std::runtime_error("Invalid z-scale given: must be positive real number");
            }
        } else {
            // We are in the final parsing section. Accumulate input folders.
            inputFolders.emplace_back(argv[i]);
        }

    }
    // We have one arg left: add it as outputFile
    outputFile = std::filesystem::path(argv[argc - 1]);
}
