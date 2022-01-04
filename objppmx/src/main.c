#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include "dstructx.h"
#include "fileiox.h"
#include "linedrawx.h"
#include "ptransformx.h"
#include "objppmx.h"

unsigned int raster_center_x;
unsigned int raster_center_y;

int main(int argc, char *argv[]){
    config_h * config;
    vfhandler_s *objxhandler;
    raster_t * raster;
    pixrgb_t color;      
    matrix4d_t tmatrix,scale,translate,rotmatrix;
    animation_stack * astack;
    char file_name[25];
    int frame_index=0;

    if((config = oxp_load_config(argv[1])) == NULL){
        printf("Error at flow point A: Load configuration file\n");
        exit(EXIT_FAILURE);
    }

    if((objxhandler = oxp_load_objx(config->source))==NULL){
        printf("Error at flow point B: Load .obj file into memory\n");
        exit(EXIT_FAILURE);
    }

    if((raster = oxp_init_raster(config->width, config->height)) == NULL){
        printf("Error at flow point C: Initialize raster\n");
        exit(EXIT_FAILURE);        
    }

    if((astack = load_animation_stack(config->animation)) == NULL ){
        printf("Error at flow point X: Load animation stack");
        exit(EXIT_FAILURE);
    }

    color.r = 255;
    color.g = 255;
    color.b = 255;

    printf("Init Translate...\n");
    translate = init_object_translate(config->translate);
    transform_object(objxhandler,&translate);

    printf("Init scale...\n");
    scale=init_object_scale(objxhandler,config->scale);
    transform_object(objxhandler,&scale);

    printf("Projection...\n");
    tmatrix=get_2dprojection_matrix(config->center);
    transform_object(objxhandler,&tmatrix);
  
    plot_object(objxhandler,raster,"ppm/frame0.ppm",&color);

   do{
        frame_index++;
        sprintf(file_name,"ppm/frame%d.ppm",frame_index);

        rotmatrix = get_rotation_matrix(astack->frame);
        transform_object(objxhandler,&rotmatrix);

        tmatrix=get_2dprojection_matrix(config->center);
        transform_object(objxhandler,&tmatrix);

        if(frame_index==1){
            print_matrix4d(rotmatrix);
        }

        if(oxp_plot_object(objxhandler, raster,file_name,&color)){
            printf("Error at flow point Y: Plotting Frame");
            exit(EXIT_FAILURE);            
        }
    }while(!animation_stack_pop(&astack));

    free_objx(objxhandler);

    return 0;
}