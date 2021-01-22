#include <cstdint>
#include <iostream>

#include <tiffio.h>
#include <openvdb/openvdb.h>

#include "ArgLoader.h"

#define RESOLUTION 100

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

    ArgLoader argLoader(argc, argv);
    if (argLoader.threshold < 0 || argLoader.threshold > 255)
    {
        std::cerr << "Invalid Threshold - Must be between 0 and 255\n";
        return 1;
    }
    
    openvdb::initialize();
    
    // Create a VDB file object.
    openvdb::io::File file(argLoader.outputFile.string());
    
    // creates the 4 color grids
    openvdb::FloatGrid::Ptr gridR =
        openvdb::FloatGrid::create(0);
    gridR->setTransform(
        openvdb::math::Transform::createLinearTransform(/*voxel size=*/1.0));
    gridR->setName("channelR");
    gridR->setGridClass(openvdb::GRID_FOG_VOLUME);
    
    openvdb::FloatGrid::Ptr gridG =
        openvdb::FloatGrid::create(0);
    gridG->setTransform(
        openvdb::math::Transform::createLinearTransform(/*voxel size=*/1.0));
    gridG->setName("channelG");
    gridG->setGridClass(openvdb::GRID_FOG_VOLUME);
    
    openvdb::FloatGrid::Ptr gridB =
        openvdb::FloatGrid::create(0);
    gridB->setTransform(
        openvdb::math::Transform::createLinearTransform(/*voxel size=*/1.0));
    gridB->setName("channelB");
    gridB->setGridClass(openvdb::GRID_FOG_VOLUME);
    
    openvdb::FloatGrid::Ptr gridA =
        openvdb::FloatGrid::create(0);
    gridA->setTransform(
        openvdb::math::Transform::createLinearTransform(/*voxel size=*/1.0));
    gridA->setName("channelA");
    gridA->setGridClass(openvdb::GRID_FOG_VOLUME);
    
    int i = 1;
    
    for (auto files : std::filesystem::directory_iterator(argLoader.inputFolder))
    {
        std::filesystem::path filepath = files.path();
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
