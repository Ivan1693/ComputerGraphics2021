#ifndef OBJPPMX_H
#define OBJPPMX_H

#include "dstructx.h"
#include "fileiox.h"
#include <unistd.h>

#define OBJFILE_LINE_SIZE 100
#define DEBUG 3

/*
* Loads the configuration file into memory using a config_h (Configuration handler) structure.
* @param config_file_path String containing the relative or absolute path to the .objxppm file
* @return Pointer to a configuration structure
*/
config_h * oxp_load_config(char * config_file_path);

/*
* Loads the vertexes and faces information into memory using the data
* structures defined in the dstructx.h header. The return value is a pointer
* to a handler which contains references to the vertexes and faces structures.
* @param file_path String containing the relative or absolute path to the .obj file
* @return Pointer to the vertexes/faces handler
*/
vfhandler_s * oxp_load_objx(char *file_path);

raster_t * oxp_init_raster(unsigned int width, unsigned int height);

int oxp_plot_object(vfhandler_s * objxhandler, raster_t * raster, char * file, pixrgb_t * color);
/*
* Deallocates the memory used to load the obj vertexes and faces
*/
void free_objx(vfhandler_s *vfhandler);

/*
*  Counts the number of vertexes and faces in the .obj file to allocate the necessary memory and
* then loads the vertexes and faces in it, taking the values from the strings readed in the file.
*/
int vfinit(vfhandler_s * vfhandler, char * source_file_path);

/*
* Creates the line strcutures for each face. It takes the integer value of the faces and gets the 
* corresponding vertex from the vertexes array. All the lines are stored as a vertex pair va,vb
* in a list that is part of the face structure.
*/
int lines_load(vfhandler_s * vfhandler);

#endif