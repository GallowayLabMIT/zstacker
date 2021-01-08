#include <cstdint>
#include <iostream>

#include <tiffio.h>
#include <openvdb/openvdb.h>

#define RESOLUTION 100

void makeTiffFog(TIFF* tif)
{
    openvdb::FloatGrid::Ptr grid =
        openvdb::FloatGrid::create(0);
    grid->setTransform(
        openvdb::math::Transform::createLinearTransform(/*voxel size=*/1.0));
    grid->setGridClass(openvdb::GRID_FOG_VOLUME);
    // Name the grid "LevelSetSphere".
    TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, RESOLUTION);
    TIFFSetField(tif, TIFFTAG_IMAGELENGTH, RESOLUTION);
    TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, 8);
    TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, 1);
    TIFFSetField(tif, TIFFTAG_ROWSPERSTRIP, 1);
    TIFFSetField(tif, TIFFTAG_ORIENTATION, ORIENTATION_TOPLEFT);
    TIFFSetField(tif, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);
    TIFFSetField(tif, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_MINISBLACK);
    TIFFSetField(tif, TIFFTAG_SAMPLEFORMAT, SAMPLEFORMAT_UINT);
    TIFFSetField(tif, TIFFTAG_COMPRESSION, COMPRESSION_NONE);
    
    uint32_t width, height;
    TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &width);
    TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &height);
    uint32_t * raster = static_cast<uint32_t*>(_TIFFmalloc(width * height * sizeof(uint32_t)));
    TIFFReadRGBAImage(tif, width, height, raster, 0);
    openvdb::FloatGrid::Accessor accessor= grid->getAccessor();
    openvdb::Coord ijk;
    int &i = ijk[0], &j = ijk[1], &k = ijk[2];
    k = 1;
//    for each coordinate, gets tiff pixel and sets fog depending on color
    for (i = 0 - width; i < 0 + width; ++i){
        for (j = 0 - height; j < 0 + height; ++j){
            int R= TIFFGetR(raster[i*j]);
            grid->setName("channelR");
            if (R > 122.5){
                accessor.setValue(ijk, 1);
            }
            else{
                accessor.setValueOff(ijk, 0);
            }
            int G= TIFFGetG(raster[i*j]);
            grid->setName("channelG");
            if (G > 122.5){
                accessor.setValue(ijk, 1);
            }
            else{
                accessor.setValueOff(ijk, 0);
            }
            int B= TIFFGetB(raster[i*j]);
            grid->setName("channelB");
            if (B > 122.5){
                accessor.setValue(ijk, 1);
            }
            else{
                accessor.setValueOff(ijk, 0);
            }
            int A= TIFFGetA(raster[i*j]);
            grid->setName("channelA");
            if (A > 122.5){
                accessor.setValue(ijk, 1);
            }
            else{
                accessor.setValueOff(ijk, 0);
            }
        }
    }
    
    // Create a VDB file object.
    openvdb::io::File file("tiffgrid.vdb");
    // Add the grid pointer to a container.
    openvdb::GridPtrVec grids;
    grids.push_back(grid);
    // Write out the contents of the container.
    file.write(grids);
    file.close();
    _TIFFfree(raster);
}


int main()
{
    openvdb::initialize();
    
    TIFF* tif = TIFFOpen("test.tif", "w");
    makeTiffFog(tif);
    
    TIFFClose(tif);
    return 0;
}
