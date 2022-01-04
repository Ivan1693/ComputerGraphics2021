#include "ptransformx.h"
extern unsigned int raster_center_x;
extern unsigned int raster_center_y;

void fswap(float * a, float * b){
    float swap = *b;
    *b=*a;
    *a=swap;
}

void iswap(int * a, int * b){
    int swap = *b;
    *b=*a;
    *a=swap;
}

int get_octant(int x, int y) {

    if (x > 0 && y > 0) {
        if (y < x)
            return 1;
        else
            return 2;
    } else if (x < 0 && y > 0) {
        if (abs(y) > abs(x))
            return 3;
        else
            return 4;
    } else if (x < 0 && y < 0) {
        if (abs(y) < abs(x))
            return 5;
        else
            return 6;
    } else if (x > 0 && y < 0) {
        if (abs(y) > abs(x)) {
            return 7;
        } else
            return 8;
    } else
        return -1;
}

int to_octant(int * xa, int * ya, int octant) {
    int x = * xa, y = * ya;

    if (octant <= 2) {
        /* Moves poit to first cuadrant */
        if (x < 0)
            x *= -1;
        if (y < 0)
            y *= -1;
        printf("  Movedpoint to 1st cuadrant > (%d, %d)\n", x, y);
        switch (octant) {
        case 1:
            if (x < y) {
                iswap( & x, & y);
                printf("  Movedpoint to 1st octant > (%d, %d) \n", x, y);
            }
            break;
        case 2:
            if (y < x) {
                iswap( & x, & y);
                printf("  Movedpoint to 2nd octant > (%d, %d) \n", x, y);
            }
            break;
        }
    } else if (octant <= 4) {
        /* Moves poit to 2nd cuadrant */
        if (x > 0)
            x *= -1;
        if (y < 0)
            y *= -1;
        printf("  Movedpoint to 2nd cuadrant > (%d, %d)\n", x, y);
        switch (octant) {
        case 3:
            if (abs(x) > abs(y)) {
                iswap( & x, & y);
                x *= -1;
                y *= -1;
                printf("  Movedpoint to 3rd octant > (%d, %d) \n", x, y);
            }
            break;
        case 4:
            if (abs(y) > abs(x)) {
                iswap( & x, & y);
                x *= -1;
                y *= -1;
                printf("  Movedpoint to 4th octant > (%d, %d) \n", x, y);
            }
            break;
        }
    } else if (octant <= 6) {
        /* Moves poit to 3rd cuadrant */
        if (x > 0)
            x *= -1;
        if (y > 0)
            y *= -1;
        printf("  Movedpoint to 3rd cuadrant > (%d, %d)\n", x, y);
        switch (octant) {
        case 5:
            if (abs(x) < abs(y)) {
                iswap( & x, & y);
                printf("  Movedpoint to 5th octant > (%d, %d) \n", x, y);
            }
            break;
        case 6:
            if (abs(y) < abs(x)) {
                iswap( & x, & y);
                printf("  Movedpoint to 6th octant > (%d, %d) \n", x, y);
            }
            break;
        }
    } else if (octant <= 8) {
        /* Moves poit to 4th cuadrant */
        if (x < 0)
            x *= -1;
        if (y > 0)
            y *= -1;
        printf("  Movedpoint to 4th cuadrant > (%d, %d)\n", x, y);
        switch (octant) {
        case 7:
            if (abs(x) > abs(y)) {
                iswap( & x, & y);
                x *= -1;
                y *= -1;
                printf("  Movedpoint to 7th octant > (%d, %d) \n", x, y);
            }
            break;
        case 8:
            if (abs(y) > abs(x)) {
                iswap( & x, & y);
                x *= -1;
                y *= -1;
                printf("  Movedpoint to 8th octant > (%d, %d) \n", x, y);
            }
            break;
        }
    } else
        return -1;

    * xa = x;
    * ya = y;
    return 0;
}

void center_line_vertexes(float * x1, float * y1, float * x2, float * y2, int width, int height){
    int Dx = (width/2)  - 1;
    int Dy = (height/2) - 1;

    *x1= Dx + *x1;
    *y1= Dy - *y1;

    *x2= Dx + *x2;
    *y2= Dy - *y2;
}

void center_point(unsigned int * x,unsigned int * y){

    //printf("c[%4d %4d]",*x,*y);

    *x= raster_center_x + *x;
    *y= raster_center_y - *y;

    //printf(" -> [%4d %4d]\n",*x,*y);


}

void scale_line(scale_t * scale, line_t * line){
    line->va.x*=scale->x;
    line->va.y*=scale->y;
    line->va.z*=scale->z;
    
    line->vb.x*=scale->x;
    line->vb.y*=scale->y;
    line->vb.z*=scale->z;
}

void scale_face(face_t * face, scale_t * scale){
    line_i line_iter;
    line_iter = face->lines->first;

    do{
        scale_line(scale, &line_iter->line);
        line_iter = line_iter->next;
    }while (line_iter!= line_iter->first);

}

void scale_object(vfhandler_s * objxhandler,scale_t * scale){
    register int i;
    face_t * face;

    for (i = 0; i < objxhandler->fcount; i++)
    {
        face = &objxhandler->faces[i];
        scale_face(face,scale);
    }
}

void transform_point(matrix4d_t m, float * x, float * y, float * z, float * w){
    float xr=*x,yr=*y,zr=*z,wr=*w;

    *x=m[0].x*(xr) + m[1].x*(yr) + m[2].x*(zr) + m[3].x*(wr);
    *y=m[0].y*(xr) + m[1].y*(yr) + m[2].y*(zr) + m[3].y*(wr);
    *z=m[0].z*(xr) + m[1].z*(yr) + m[2].z*(zr) + m[3].z*(wr);
    *w=m[0].w*(xr) + m[1].w*(yr) + m[2].w*(zr) + m[3].w*(wr);

}

void transform_line(matrix4d_t * matrix, line_t * line){
    float w=1;
    //printf("LinePointsIn:[%.2f %.2f %.2f ]\t[%.2f %.2f %.2f ]\n",line->va.x,line->va.y,line->va.z,line->vb.x,line->vb.y,line->vb.z);
    transform_point(*matrix, &line->va.x, &line->va.y, &line->va.z,&w);
    transform_point(*matrix, &line->vb.x, &line->vb.y, &line->vb.z,&w);
    //printf("LinePointsOut:[%.2f %.2f %.2f ]\t[%.2f %.2f %.2f ]\n",line->va.x,line->va.y,line->va.z,line->vb.x,line->vb.y,line->vb.z);    
}

void transform_face(matrix4d_t * matrix, face_t * face){
    line_i line_iter;
    line_iter = face->lines->first;

    do{
        transform_line(matrix, &line_iter->line);
        line_iter = line_iter->next;
    }while (line_iter!= line_iter->first);

}

void transform_object(vfhandler_s * objxhandler,matrix4d_t * matrix){
    register int i;
    face_t * face;

    for (i = 0; i < objxhandler->fcount; i++)
    {
        face = &objxhandler->faces[i];
        transform_face(matrix, face);
    }
}

matrix4d_t get_2dprojection_matrix(projection2d_t center_vector){

    matrix4d_t projection_matrix=(matrix4d_t)malloc(sizeof(point4d_t)*4);;
    memset(projection_matrix, 0x00,sizeof(point4d_t)*4);
/*
    c3  0   -c1 0
    0   c3  -c2 0
    0   0   0   0
    0   0   -1  c3
*/
    projection_matrix[0].x = center_vector.z;
    
    projection_matrix[1].y = center_vector.z;

    projection_matrix[2].x = -center_vector.x;
    projection_matrix[2].y = -center_vector.y;
    projection_matrix[2].w = -1;
    
    projection_matrix[3].w = -center_vector.z;

    return projection_matrix;
}

matrix4d_t init_object_scale(vfhandler_s * objxhandler,float value){
    matrix4d_t scale_matrix = (matrix4d_t)malloc(sizeof(point4d_t)*4);
    memset(scale_matrix, 0x00,sizeof(point4d_t)*4);

    scale_matrix[0].x = value;
    scale_matrix[1].y = value;
    scale_matrix[2].z = value;
    scale_matrix[3].w = 0;

    return scale_matrix;
}

matrix4d_t init_object_translate(translate_t translate_vector){
    matrix4d_t translate_matrix = (matrix4d_t)malloc(sizeof(point4d_t)*4);
    memset(translate_matrix, 0x00,sizeof(point4d_t)*4);

    translate_matrix[0].x = 1;
    translate_matrix[1].y = 1;
    translate_matrix[2].z = 1;
    translate_matrix[3].w = 1;

    translate_matrix[3].x = translate_vector.x;

    translate_matrix[3].y = translate_vector.y;

    translate_matrix[3].z = translate_vector.z;


    return translate_matrix;
}


matrix4d_t get_rotation_matrix(animation_frame * frame){
    matrix4d_t rotation_matrix=(matrix4d_t)malloc(sizeof(point4d_t)*4);
    point3d_t * cos = &frame->rotation.cos;
    point3d_t * sin = &frame->rotation.sin;
    point3d_t * rotcenter = &frame->rotation.center;
    double r[9];

    memset(rotation_matrix, 0x00,sizeof(point4d_t)*4);

    r[0] = cos->y*cos->z;                               //R1
    r[1] = (-cos->z*sin->x*sin->y) + (cos->x*sin->z);   //R2
    r[2] = (cos->x*cos->z*sin->y) + (sin->x*cos->z);    //R3
    r[3] = -cos->y*sin->z;                              //R4
    r[4] = (sin->x*sin->y*sin->z) + (cos->x*cos->z);    //R5
    r[5] = (-cos->x*sin->y*sin->z) + (cos->z*sin->x);   //R6
    r[6] = -sin->y;                                     //R7
    r[7] = -cos->y*sin->x;                              //R8
    r[8] = cos->x*cos->y;                               //R9

    rotation_matrix[0].x = r[0];
    rotation_matrix[0].y = r[1];
    rotation_matrix[0].z = r[2];
    rotation_matrix[0].w = 0;

    rotation_matrix[1].x = r[3];
    rotation_matrix[1].y = r[4];
    rotation_matrix[1].z = r[5];
    rotation_matrix[1].w = 0;

    rotation_matrix[2].x = r[6];
    rotation_matrix[2].y = r[7];
    rotation_matrix[2].z = r[8];
    rotation_matrix[2].w = 0;

    rotation_matrix[3].x = rotcenter->x;
    rotation_matrix[3].y = rotcenter->y;
    rotation_matrix[3].z = rotcenter->z;
    rotation_matrix[3].w = 1;

    return rotation_matrix;
}

int animation_stack_push(animation_stack ** top, animation_frame * frame){
    animation_stack * element = (animation_stack *)malloc(sizeof(animation_stack));

    if(element==NULL) return 1;

    element->frame = frame;

    if(*top==NULL)    {
        element->next=NULL;
        *top = element;
    }else{
        element->next = *top;
        *top = element;
    }

    element=NULL;

    return 0;
}

int animation_stack_pop(animation_stack ** top){
    animation_stack * element = *top;

    if(element->next!=NULL){
        *top = element->next;
        free(element);
        element=NULL;        
    }else{
        free(element);
        element=NULL;
        return 1;
    }

    return 0;
}