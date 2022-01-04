/*
* @brief Data structures to handle obj->ppm convertion
* Each .obj file has defined in it a set of vertexes and a set of faces. A vertex
* has three values corresponding to its x,y,z coordinate. A face has a set of 
* integer numbers, which indicates the set of vertexes that represent the face.
* 
* Needed strcutures: 
*        Vertices    3 coordenadas v(float x,float y,float z)
*        Lineas      2 vertices l(struct v1,struct v2)
*        Caras       3 o mas lineas f(line_list lines, int_list faces)
*/
#ifndef DSTRUCTX_H
#define DSTRUCTX_H

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>


/*
* @brief Unsigned integers list used to hold the faces' vertexes index
*/
typedef struct uint_list
{
    unsigned int value;
    struct uint_list * next;
    struct uint_list * first;
}uint_list;

typedef struct point4d_t{
    float x;
    float y;
    float z;
    float w;
}point4d_t;

typedef struct point3d_t{
    float x;
    float y;
    float z;
}point3d_t;

typedef struct {
    float x;
    float y;
}point2d_t;

typedef struct point2d_list_t{
    point2d_t point;
    struct point2d_list_t * first;
    struct point2d_list_t * next;    
}point2d_list;

typedef struct point2d_stack
{
    point2d_t point;
    struct point2d_stack * next;

}point2d_stack;



typedef point2d_list * point2d_i;


/*
* @brief Vertex structure
*/
typedef point3d_t vertex_t;
typedef point3d_t * matrix3d_t;
typedef point4d_t * matrix4d_t;
/*
* @brief Line structure
*/
typedef struct
{
    vertex_t va;
    vertex_t vb;
}line_t;

/*
* @brief Line list
*/
typedef struct line_list
{
    line_t line;
    struct line_list * next;
    struct line_list * first;  
}line_list;

typedef line_list * line_i;

typedef struct line_stack
{
    line_t line;
    struct line_stack * next;
}line_stack;

/*
* @brief Face structure
*/
typedef struct
{
    line_list * lines;
    uint_list * vertexes;
}face_t;

/*
* @brief Vertex/face handler.
*/
typedef struct
{
    char * source_file_path;    /*<! Path to source file */
    fpos_t voffset;             /*<! Offset within source file whence vertexes definition begin*/
    fpos_t foffset;             /*<! Offset within source file whence faces definition begin*/
    long unsigned int vcount;   /*<! Vertexes count */
    long unsigned int fcount;   /*<! Faces count */
    vertex_t * vertexes;        /*<! Vertexes array */
    face_t * faces;             /*<! Faces array */

}vfhandler_s;

typedef struct {
    char * name;                /*<! Name of the handler to identify within a thread*/
    vfhandler_s * vfhandler;    /*<! Vertexes/Faces handler */
    int ncore;                  /*<! Number of CPU's avaliable */
}vfthread_h;

typedef struct {
    long int block_size;
    long int init_index;
    long int end_index;
}parallel_h;








int uint_list_insert(uint32_t _value,uint_list ** head);
void uint_list_print(char * format, uint_list * head);

int line_list_insert(line_t * _line,line_list ** head);
void line_list_print(char * format, line_list * head);


int point2d_list_insert(float x, float y, point2d_list ** head);
void point2d_list_print(char * format, point2d_list * head);
void point2d_list_free(point2d_list * head);

point3d_t * new_point(float x, float y, float z);
line_t * new_line(vertex_t * _va, vertex_t * _vb);

void free_vector(void * ptr_array[], int length);

void print_matrix4d(matrix4d_t matrix);
void print_vector3d(point3d_t point);


int point2d_stack_push(float x, float y, point2d_stack ** top);
int point2d_stack_pop(point2d_stack ** top);

#endif
