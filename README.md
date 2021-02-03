# zstacker
Zstacker is a utility function that converts a series of tiff images into a stacked object in OpenVDB file format that can be imported into 3D modeling software such as Blender. 

The resulting OpenVDB file has each of the 4 RGBA channels as a different 'grid' (identified as "channelR,""channelG,""channelB," and "channelA") which can then be mixed together with Blender's Volume Mix Shaders.  

![Example](/documentation_images/mix_shaders.png)

## Installation

Download files and unzip into the desired location. 

## Required Arguments

'input_folder' - a path to any number of folders. The function searches for any tiff files and sorts them in alphabetical order. 

'output_vdb' - a path to the vdb that is to be created. 

## Optional Arguments

'-t threshold' - a number between 0 and 255 describing the pixel value cutoff. Values below this number are not rendered in the final object.

'-z z_scale' - a positive number describing that changes the spacing of pixels in the z direction compared to the x and y directions. Default = 1, where z-pixel spacing is the same as x- and y- pixel spacing. 


## Example Use
_zstacker -t 30 -z .66 input_folder output.vdb_
	This results in an OpenVDB file with all pixel values below 30 not rendered and a z-pixel distance 2/3 that of the x and y distances. 
