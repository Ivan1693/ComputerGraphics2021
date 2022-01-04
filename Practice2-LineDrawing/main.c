#include <stdio.h>
#include <string.h>
#include "objppmx.h"

unsigned int raster_center_x;
unsigned int raster_center_y;

int main(int argc, char *argv[]){
    vfhandler_s *objxhandler;
    raster_t * raster;
    pixrgb_t color;      

    if((objxhandler = oxp_load_objx(argv[1]))==NULL){
        printf("Error at flow point B: Load .obj file into memory\n");
        exit(EXIT_FAILURE);
    }

    if((raster = oxp_init_raster(960, 540)) == NULL){
        printf("Error at flow point C: Initialize raster\n");
        exit(EXIT_FAILURE);        
    }

    color.r = 255;
    color.g = 255;
    color.b = 255;

    plot_object(objxhandler,raster,argv[2],&color);

    return 0;
}