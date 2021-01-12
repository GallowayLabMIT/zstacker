#include <cstdint>
#include <iostream>

#include <tiffio.h>
#include <openvdb/openvdb.h>

#define RESOLUTION 100

void makeTiffFog(TIFF* tif,openvdb::FloatGrid::Ptr grid, int k_val)
{
    // creates the 4 color grids
    openvdb::FloatGrid::Ptr gridR =
        openvdb::FloatGrid::create(0);
    gridR->setTransform(
        openvdb::math::Transform::createLinearTransform(/*voxel size=*/1.0));
    
    openvdb::FloatGrid::Ptr gridG =
        openvdb::FloatGrid::create(0);
    gridG->setTransform(
        openvdb::math::Transform::createLinearTransform(/*voxel size=*/1.0));
    
    openvdb::FloatGrid::Ptr gridB =
        openvdb::FloatGrid::create(0);
    gridB->setTransform(
        openvdb::math::Transform::createLinearTransform(/*voxel size=*/1.0));
    
    openvdb::FloatGrid::Ptr gridA =
        openvdb::FloatGrid::create(0);
    gridA->setTransform(
        openvdb::math::Transform::createLinearTransform(/*voxel size=*/1.0));
    
    
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
    for (i = 0; i < width; ++i){
        for (j = 0; j < height; ++j){
            int R= TIFFGetR(raster[i*j]);
            gridR->setName("channelR");
            openvdb::FloatGrid::Accessor accessorR= gridR->getAccessor();
            accessorR.setValue(ijk, R);
            
            int G= TIFFGetG(raster[i*j]);
            gridG->setName("channelG");
            openvdb::FloatGrid::Accessor accessorG= gridG->getAccessor();
            accessorG.setValue(ijk, G);
            
            int B= TIFFGetB(raster[i*j]);
            gridB->setName("channelB");
            openvdb::FloatGrid::Accessor accessorB= gridB->getAccessor();
            accessorB.setValue(ijk, B);
            
            int A= TIFFGetA(raster[i*j]);
            gridA->setName("channelA");
            openvdb::FloatGrid::Accessor accessorA= gridA->getAccessor();
            accessorA.setValue(ijk, A);
        }
    }
    
    // Create a VDB file object.
    openvdb::io::File file("tiffgrid.vdb");
    // Add the grid pointer to a container.
    openvdb::GridPtrVec grids;
    grids.push_back(gridR);
    grids.push_back(gridG);
    grids.push_back(gridB);
    grids.push_back(gridA);
    // Write out the contents of the container.
    file.write(grids);
    file.close();
    _TIFFfree(raster);
}


int main(int argc, char *argv[])
{

    openvdb::initialize();
    
    openvdb::FloatGrid::Ptr grids =
        openvdb::FloatGrid::create(0);
    grids->setTransform(
        openvdb::math::Transform::createLinearTransform(/*voxel size=*/1.0));
    grids->setGridClass(openvdb::GRID_FOG_VOLUME);
    // Name the grid "LevelSetSphere".
    
    // argument handling
    for (int i = 1; i < argc; ++i)
    {
        TIFF* tif = TIFFOpen(argv[i], "r");
        if (tif == nullptr)
        {
            std::cerr << "Invalid TIF file\n";
            return 1;
        }
        
    std::cout << "Opening TIFF\n";
    makeTiffFog(tif, grids, i);
    TIFFClose(tif);
    }
    
    return 0;
}
