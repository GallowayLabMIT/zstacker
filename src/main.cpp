#include <cstdint>

#include <tiffio.h>
#include <openvdb/openvdb.h>

#define RESOLUTION 100

int main()
{
    TIFF* tif = TIFFOpen("test.tif", "w");
    TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, RESOLUTION);
    TIFFSetField(tif, TIFFTAG_IMAGELENGTH, RESOLUTION);
    TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, 32);
    TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, 1);
    TIFFSetField(tif, TIFFTAG_ROWSPERSTRIP, 1);
    TIFFSetField(tif, TIFFTAG_SAMPLEFORMAT, SAMPLEFORMAT_UINT);
    TIFFSetField(tif, TIFFTAG_COMPRESSION, COMPRESSION_NONE);

    uint32_t array[RESOLUTION];
    for (uint32_t i = 0; i < RESOLUTION; ++i)
    {
        // Update the array
        for (uint32_t j = 0; j < RESOLUTION; ++i)
        {
            array[j] = (i + j) % RESOLUTION;
        }
        TIFFWriteScanline(tif, array, i, 0);
    }
    
    TIFFClose(tif);

    openvdb::initialize();
    // Create a shared pointer to a newly-allocated grid of a built-in type:
    // in this case, a FloatGrid, which stores one single-precision floating point
    // value per voxel.  Other built-in grid types include BoolGrid, DoubleGrid,
    // Int32Grid and Vec3SGrid (see openvdb.h for the complete list).
    // The grid comprises a sparse tree representation of voxel data,
    // user-supplied metadata and a voxel space to world space transform,
    // which defaults to the identity transform.
    openvdb::FloatGrid::Ptr grid =
        openvdb::FloatGrid::create(/*background value=*/2.0);
    // Associate some metadata with the grid.
    grid->insertMeta("radius", openvdb::FloatMetadata(50.0));
    // Associate a scaling transform with the grid that sets the voxel size
    // to 0.5 units in world space.
    grid->setTransform(
        openvdb::math::Transform::createLinearTransform(/*voxel size=*/0.5));
    // Identify the grid as a level set.
    grid->setGridClass(openvdb::GRID_LEVEL_SET);
    // Name the grid "LevelSetSphere".
    grid->setName("LevelSetSphere");
    // Create a VDB file object.
    openvdb::io::File file("testgrid.vdb");
    // Add the grid pointer to a container.
    openvdb::GridPtrVec grids;
    grids.push_back(grid);
    // Write out the contents of the container.
    file.write(grids);
    file.close();
    return 0;
}
