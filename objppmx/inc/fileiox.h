#ifndef FILEIOX_H
#define FILEIOX_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include "dstructx.h"
#include "ptransformx.h"
#include "linedrawx.h"

#define LINE_READ_BUFFER_SIZE 100
#define DEBUG 0

/*
* @brief Structure to load configuration options from .objxppm file
*/
typedef struct {
    char source[LINE_READ_BUFFER_SIZE];
    char output[LINE_READ_BUFFER_SIZE];
    char animation[LINE_READ_BUFFER_SIZE];
    unsigned int width;
    unsigned int height;
    float scale;
    translate_t translate;
    projection2d_t center;
}config_h;

/*
* @brief Strcuture to handle 8 bits [0-255] RGB values
*/
typedef struct 
{
    unsigned char r;
    unsigned char g;
    unsigned char b;   
}pixrgb_t;

/*
* @brief Structure to handle the map and dimensions of the raster
*/
typedef struct
{
    unsigned int width;
    unsigned int height;
    unsigned char **map;
    pixrgb_t ** rgb_map;
}raster_t;

/*
* Tokenizes the extension of the file path given in _path and compares to extension
* @param _path String to validate it's extension
* @param extension Valid extension
*/
int validate_extension(char * _path, char * extension);

/*
*@brief Writes a raster map into a ppm file
*@param raster Raster strcuture containing the map to plot
*@param file_path Path and name of the output file 
*/
void write_ppm(raster_t * raster, char * file_path);

/*
* Sets the width and height of the raster, then allocates the necessary memory
*@param raster Raster pointer to raster structure to be allocated
*@param width Width of the raster
*@param height of the raster
*/
raster_t * init_raster(unsigned int width, unsigned int height);
/*
* Sets all the raster.map values to 0x00
*/
void clear_raster(raster_t * raster);

/*
*@brief Plots a line_t to a file
*@param line Structure with the line to plot
*@param raster 
*/
void plot_line(line_t * line, raster_t * raster, scale_t * scale, pixrgb_t color, char * file);

//void map_points(raster_t * raster, point2d_list * coords, pixrgb_t * color);
void plot_face(face_t * face, raster_t * raster, scale_t * scale, char * file, pixrgb_t color);


void plot_object(vfhandler_s * objxhandler, raster_t * raster, char * file, pixrgb_t * color);

/*
*@brief Parses and loads configuration values from a .obppmx file.
*@param file path to the configuration file to load
*/
config_h * parse_config(char * file);

/*
*@brief Loads the animation stack into memory using a animation_stack structure
*@param file_path Absolute or relative path to the .astck file containing the frame transformations
*/
animation_stack * load_animation_stack(char * file_path);
#endif