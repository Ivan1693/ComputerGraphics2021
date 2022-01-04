#ifndef PTRANSFORMX_H
#define PTRANSFORMX_H

#include "dstructx.h"

typedef point3d_t scale_t;
typedef point3d_t translate_t;
typedef point3d_t projection2d_t;

typedef struct rotation{
    point3d_t cos;
    point3d_t sin;
    point3d_t center;
}rotation_t;

typedef struct frame
{
    scale_t scale;
    translate_t translate;
    rotation_t rotation;
}animation_frame;


typedef struct animation_stack
{
    animation_frame * frame;
    struct animation_stack * next;

}animation_stack;


void iswap(int * a, int * b);
int get_octant(int x, int y);
int to_octant(int * xa, int * ya, int octant);
void center_point(unsigned int * x,unsigned int * y);
/*
* @brief Centers (in a width*height raster/image) a pair of points/vertexes.
*/
void center_line_vertexes(float * x1, float * y1, float * x2, float * y2, int width, int height);

/*
* @brief Scales a line's pair of vertexes va and vb using a scale vector
*/
void scale_line(scale_t * scale, line_t * line);
void scale_object(vfhandler_s * objxhandler,scale_t * scale);
void transform_point(matrix4d_t m, float * x, float * y, float * z, float * w);
void transform_line(matrix4d_t * matrix, line_t * line);

/*
* Applies a matriz transformation into an object.
* @param objxhandler Handler of an object loaded into memory
* @param matrix Matrix to perform a transformation 
*/
void transform_object(vfhandler_s * objxhandler,matrix4d_t * matrix);


/*
* Calculates the projection matrix given a center vector
* @param center_vector a vector defining the center of projection for the object
* @return A matrix in homogeneous space to apply to the object
*/
matrix4d_t get_2dprojection_matrix(projection2d_t center_vector);
matrix4d_t init_object_scale(vfhandler_s * objxhandler,float value);
matrix4d_t init_object_translate(translate_t translate_vector);

matrix4d_t get_rotation_matrix(animation_frame * frame);
int animation_stack_push(animation_stack ** stack, animation_frame * frame);
int animation_stack_pop(animation_stack ** stack);

#endif