# zstacker
Zstacker is a utility function that converts a series of tiff images into a stacked volume object in OpenVDB file format that can be imported into 3D modeling software such as Blender. 

The resulting OpenVDB file has each of the 4 RGBA channels as a different 'grid' (identified as "channelR,""channelG,""channelB," and "channelA") which can then be mixed together with Blender's Volume Mix Shaders.  

![Example](/documentation_images/mix_shaders.png)

The resulting volume can be rendered into static images or video files.
![example_image](/documentation_images/example_render.png)

## Installation

If on Windows or MacOS (> 10.15), you should be able to use the prebuilt binaries. Download a zip file from the releases tab and unzip to the desired location.

If on another operating system or version, you can compile `zstacker` from the source code. See the below **Developer installation** section for details.

## Usage
`zstacker` has a command-line interface:

```
usage: zstacker [-h] [-t pixel_threshold] [-z zscale] input_paths output_vdb
```

### Required Arguments
`input_paths`: `zstacker` takes any number of input folders to combine into a single z-stacked volume. Folders passed to `zstacker` will be searched for `.tif` files; `.tif` files will be combined in alphabetical/lexographic order. Typically, this means that you should create a single folder for sequentially numbered images (e.g. `overlay_0001.tif`, `overlay_0002.tif`, and so on), then pass the path to that folder to `zstacker`.

`output_vdb`: The last argument passed should be the desired output volumetric data file and should have file extension `.vdb`. Any paths to folders passed before the last argume

### Optional Arguments

`-t threshold`: a number between 0 and 255 describing the pixel value cutoff. Values below this number are not rendered in the final object. For example, if the noise floor in your images has pixel value 25, setting a threshold of 30 will ensure that the resulting volume does not contain this noise.

`-z z_scale`: a positive number describing that changes the spacing of pixels in the z direction compared to the x and y directions. Default = 1, where z-pixel spacing is the same as x- and y- pixel spacing. Typical microscopes will have a smaller x-y spacing than the z-stack spacing, meaning that this value should typically be larger than 1.


## Example Use
```zstacker -t 30 -z .66 stack_folder output.vdb```

This causes `zstacker` to search for `.tif` files inside folder `stack_folder`, and writes a OpenVDB file written with name `output.vdb` with all pixel values below 30 not rendered into the volume and a z-pixel distance 2/3 that of the x and y distances. 

## Developer installation

You can build `zstacker` using `cmake`. 

If on Windows, you can use `vcpkg` to bootstrap most of the dependencies. Do the following in Powershell:
```
$ git clone --recursive https://github.com/GallowayLabMIT/zstacker.git
$ cd zstacker
$ .\bootstrap_windows.ps1   # install and bootstrap vcpkg
$ mkdir build     # create cmake build directory
$ cd build
$ cmake ..        # cmake configuration
$ cmake --build . # build zstacker!
```

If on Linux or MacOS, follow the instructions to first install the [OpenVDB dependencies](https://github.com/AcademySoftwareFoundation/openvdb#developer-quick-start). Importantly, you don't need to build OpenVDB on your own; this repository will do it for you. After installing the dependencies, run:
```
$ git clone --recursive https://github.com/GallowayLabMIT/zstacker.git
$ cd zstacker
$ mkdir build     # create cmake build directory
$ cd build
$ cmake ..        # cmake configuration
$ cmake --build . # build zstacker!
```

If you run into warnings about not being able to find `libtiff` or `openvdb` dependencies, you likely didn't initalize the git submodule properly (e.g. forgot to recursively git clone). Run the following to fix:
```
$ git submodule update --init --recursive
$ git submodule update --recursive
```

## License

This project is licensed under the MIT license. Use freely! `zstacker` is built using OpenVDB (licensed under the Mozilla Public License Version 2.0) and libtiff (licensed under an MIT-esque license).
