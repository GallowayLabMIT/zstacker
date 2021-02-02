#include <cstdint>
#include <iostream>
#include <stdexcept>
#include <filesystem>
#include <tiffio.h>
#include <openvdb/openvdb.h>

#include "ArgLoader.h"

#define RESOLUTION 100

namespace fs = std::filesystem;

void makeTiffFog(
    TIFF* tif,
    openvdb::FloatGrid::Ptr gridR,
    openvdb::FloatGrid::Ptr gridG,
    openvdb::FloatGrid::Ptr gridB,
    openvdb::FloatGrid::Ptr gridA,
    int k_val,
    double threshold_val)
{
//    gets TIFF dimensions/opens the raster for editing
    uint32_t width, height;
    TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &width);
    TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &height);
    uint32_t * raster = static_cast<uint32_t*>(_TIFFmalloc(width * height * sizeof(uint32_t)));
    TIFFReadRGBAImage(tif, width, height, raster, 0);
    openvdb::Coord ijk;
    int &i = ijk[0], &j = ijk[1], &k = ijk[2];
//    sets k value as the image number
    k = k_val;
//    for each coordinate, gets tiff pixel and sets fog depending on color
    openvdb::FloatGrid::Accessor accessorR= gridR->getAccessor();

    openvdb::FloatGrid::Accessor accessorG= gridG->getAccessor();

    openvdb::FloatGrid::Accessor accessorB= gridB->getAccessor();

    openvdb::FloatGrid::Accessor accessorA= gridA->getAccessor();

    for (i = 0; i < width; ++i){
        for (j = 0; j < height; ++j){
            int R= TIFFGetR(raster[j * width + i]);
            int G= TIFFGetG(raster[j * width + i]);
            int B= TIFFGetB(raster[j * width + i]);
            int A= TIFFGetA(raster[j * width + i]);
            
            if (R > threshold_val || G > threshold_val || B > threshold_val) {
                accessorR.setValue(ijk, R / 255.0);

                accessorG.setValue(ijk, G / 255.0);

                accessorB.setValue(ijk, B / 255.0);

                accessorA.setValue(ijk, A / 255.0);
            }
                
        }
    }
    _TIFFfree(raster);
}


int main(int argc, char *argv[])
{

    try
    {
        ArgLoader argLoader(argc, argv);

        if (argLoader.exitEarly)
        {
            return 0;
        }
        openvdb::initialize();

        // Create a VDB file object.
        openvdb::io::File file(argLoader.outputFile.string());
        openvdb::Vec3R scaler(1.0, 1.0, argLoader.z_scale);
        openvdb::Mat4R mat;
        mat.setToScale(scaler);

        // creates the 4 color grids
        openvdb::FloatGrid::Ptr gridR =
            openvdb::FloatGrid::create(0);
        gridR->setTransform(
            openvdb::math::Transform::createLinearTransform(mat));
        gridR->setName("channelR");
        gridR->setGridClass(openvdb::GRID_FOG_VOLUME);

        openvdb::FloatGrid::Ptr gridG =
            openvdb::FloatGrid::create(0);
        gridG->setTransform(
            openvdb::math::Transform::createLinearTransform(mat));
        gridG->setName("channelG");
        gridG->setGridClass(openvdb::GRID_FOG_VOLUME);

        openvdb::FloatGrid::Ptr gridB =
            openvdb::FloatGrid::create(0);
        gridB->setTransform(
            openvdb::math::Transform::createLinearTransform(mat));
        gridB->setName("channelB");
        gridB->setGridClass(openvdb::GRID_FOG_VOLUME);

        openvdb::FloatGrid::Ptr gridA =
            openvdb::FloatGrid::create(0);
        gridA->setTransform(
            openvdb::math::Transform::createLinearTransform(mat));
        gridA->setName("channelA");
        gridA->setGridClass(openvdb::GRID_FOG_VOLUME);

        int i = 1;
        std::set<fs::path> sorted_by_name;
        for (auto path : argLoader.inputFolders)
        {
            for (auto& entry : fs::directory_iterator(path))
            {
                sorted_by_name.insert(entry.path());
            }

            for (auto& filename : sorted_by_name)
            {
                std::filesystem::path filepath = filename;
                std::string fileExtension = filepath.extension().string();
                if (fileExtension == ".tif" || fileExtension == ".tiff")
                {
                    TIFF* tif = TIFFOpen(filepath.string().c_str(), "r");
                    if (tif == nullptr)
                    {
                        std::cerr << "Invalid TIF file\n";
                        return 1;
                    }
                    std::cout << "Opening TIFF\n";
                    makeTiffFog(tif, gridR, gridG, gridB, gridA, i, argLoader.threshold);
                    i += 1;
                    TIFFClose(tif);
                }
            }
        }

        openvdb::GridPtrVec grids;
        grids.push_back(gridR);
        grids.push_back(gridG);
        grids.push_back(gridB);
        grids.push_back(gridA);
        // Write out the contents of the container.
        file.write(grids);
        file.close();
        return 0;
    }
    catch (std::runtime_error & err)
    {
        std::cerr << "zstacker: " << err.what() << "\n\tRun 'zstacker -h' to see full help.\n";
        return 1;
    }
    catch (openvdb::Exception & err)
    {
        std::cerr << "zstacker: " << err.what() << "\n\tRun 'zstacker -h' to see full help.\n";
        return 1;
    }
    return 0;
}
