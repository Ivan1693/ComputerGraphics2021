# Computer Graphics 2021 - OBJPPMx
> Brandon Iván Hernández Reséndiz - 2018630101 - 3CV14

Project developed for the ESCOM's Computer Graphics course imparted by Professor Rafael Norman. It contains a set modules to write C programs to load basic .obj files and render to .ppm files.

## Brief description

The modules that make up this project, contain data structures and functions to load a model/object's vertexes and faces information from a .obj file into memory, in order to compute the lines and points that the vertexes and faces describe, transform them and finally plot them to a .ppm file.

### Modules

#### `objppmx`

API to call the main functions used to load, transform and plot the objects.

#### `dstructx`

Defines the data structures and its functions to handle de vertexes, faces, lines and points information. The structure defined as `vf_handler` (wich comes from **v**ertexes and **f**aces handler) is the first structure to be used, in order to load the .obj information into memory.

#### `fileiox`

Defines the data structures and its functions to open and write files. It also parses the configuration file with extension `.oxp`, defined and proposed, to set the source files, output files and initial setting of the image to plot (scale, translation and center of projection). The animation stack is also defined in a proposed file type with extension `.astck` (from **a**nimation **st**a**ck**).

#### `linedrawx`

Contains the algorithms to write lines, using the naive approach, the DDA algorithm and the Bresenham algorithm.

#### `ptansformx`

Defines a set of functions to transform the lines, and points of a model. Transformations as scaling, translating, 3D to 2D projections, among others can be made using the functions implemented in this module.

## OXP and ASTCK files
This file extension  is a text plain file used to configure the input and the output of the program. It uses a set of defined key-value pairs to set the input and output file, the dimmensions, initial scale, translation and center of projection, and finally if defined, the animation stack to make an AVI video file. This animation stack defines on each line of a plain text file the transformation for each frame given in terms of scale, translation, angle of traslation in the x,y,z axis and the center of this traslation.

### Example of a .objxppm file
```
file:test/model.obj
width:1920
height:1080
output:ppm/model.ppm
scale:20000
translate:1.7 -18.5 0 0
center:0.01 0.01 0.02
transformation:test/model.astck
```
### Example of a .astck file
```
1.5 1.5 1.5 0 0 0 0.01 0.01 0.01 0 0 0
1.4 1.4 1.4 0 0 0 0.01 0.01 0.01 0 0 0
1.3 1.3 1.3 0 0 0 0.01 0.01 0.01 0 0 0
1.2 1.2 1.2 0 0 0 0.01 0.01 0.01 0 0 0
1.1 1.1 1.1 0 0 0 0.01 0.01 0.01 0 0 0
```

## Basic flow of a program

1. [A] Load the configuration file into memory.
2. [B] Load the .obj file into memory
3. [C] Initiliazes raster
4. [D] Initiliazes object scale, translation and projection
5. [E] Plot the object

