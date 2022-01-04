#include "dstructx.h"
#include <stdio.h>
//Handle functions
int uint_list_insert(uint32_t _value,uint_list ** head){
    uint_list * node = (uint_list *)malloc(sizeof(uint_list));
    uint_list * p_head = *head;

    if(node==NULL) return 1;
   
    node->value = _value;

    if(p_head==NULL){
        node->first = node;
        node->next = node;
    }else{
        p_head->next = node;
        node->first = p_head->first;
        node->next = p_head->first;
    }
    *head=node;
    return 0;
}

void uint_list_print(char * format, uint_list * head){
    uint_list * p_head;
    if(head==NULL){
        printf("Error: no head provided\n");
        exit(EXIT_FAILURE);
    }
    p_head = head->first;
    do{
        printf(format, p_head->value);
        p_head=p_head->next;
    }while(p_head!=head->first);
}

int line_list_insert(line_t * _line, line_list ** head){
    line_list * node = (line_list *)malloc(sizeof(line_list));
    line_list * p_head = *head;

    if(node==NULL) return 1;
   
    node->line = *(_line);

    if(p_head==NULL){
        node->first = node;
        node->next = node;
    }else{
        p_head->next = node;
        node->first = p_head->first;
        node->next = p_head->first;
    }
    *head=node;
    return 0;
}

int line_stack_push(line_t * line, line_stack ** top){
    line_stack * element = (line_stack *)malloc(sizeof(line_stack));

    if(element==NULL) return 1;

    element->line = *line;

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

int line_stack_pop(line_stack ** top){
    line_stack * element = *top;

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

void line_list_print(char * format, line_list * head){
    line_list * p_head = head->first;
    line_t line;
    do{
        line = (p_head->line);
        printf(format,
            line.va.x,
            line.va.y,
            line.va.z,
            line.vb.x,
            line.vb.y,
            line.vb.z
        );
        p_head=p_head->next;
    }while(p_head!=head->first);
}

int point2d_list_insert(float x, float y, point2d_list ** head){
    point2d_list * node = (point2d_list *)malloc(sizeof(point2d_list));
    point2d_list * p_head = *head;

    if(node==NULL) return 1;
   
    node->point.x = x;
    node->point.y = y;

    if(p_head==NULL){
        node->first = node;
        node->next = node;
    }else{
        p_head->next = node;
        node->first = p_head->first;
        node->next = p_head->first;
    }
    *head=node;
    return 0;
}

void point2d_list_print(char * format, point2d_list * head){
    point2d_i point_iter = head->first;
    point2d_t point;
    do{
        point = point_iter->point;
        printf(format,
            point.x,
            point.y
        );
        point_iter=point_iter->next;
    }while(point_iter!=head->first);
}

int point2d_stack_push(float x, float y, point2d_stack ** top){
    point2d_stack * element = (point2d_stack *)malloc(sizeof(point2d_stack));

    if(element==NULL) return 1;

    element->point.x = x;
    element->point.y = y;

    if(*top==NULL)    {
        element->next=NULL;
        *top = element;
    }else{
        element->next = *top;
        *top = element;
    }

    //printf("Inserted: %.2f\t%.2f\n", element->point.x, element->point.y);

    element=NULL;

    return 0;
}

int point2d_stack_pop(point2d_stack ** top){
    point2d_stack * element = *top;
    if(element->next!=NULL){
        //printf("Pop: %.2f, %.2f\tNext: %.2f,%.2f\n", element->point.x, element->point.y, element->next->point.x,element->next->point.y);
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


//Get functions
point3d_t * new_point(float _x, float _y, float _z){
    point3d_t * point =(point3d_t *)malloc(sizeof(point3d_t));

    point->x = _x;
    point->y = _y;
    point->z = _z;

    return point;
}

line_t * new_line(vertex_t * _va, vertex_t * _vb){
    line_t * line = (line_t *)malloc(sizeof(line_t));

    line->va = *(_va);
    line->vb = *(_vb);

    return line;
}

//General functions
void free_vector(void * ptr_array[], int length){
    register int i=0;
    for(i=0;i<length;i++) free(ptr_array[i]);
}

void print_matrix4d(matrix4d_t matrix){
    FILE * f = fopen("rotationMatrix.txt","a");
    
    printf("%4.2f\t%4.2f\t%4.2f\t%4.2f\n", matrix[0].x, matrix[1].x, matrix[2].x, matrix[3].x);
    printf("%4.2f\t%4.2f\t%4.2f\t%4.2f\n", matrix[0].y, matrix[1].y, matrix[2].y, matrix[3].y);
    printf("%4.2f\t%4.2f\t%4.2f\t%4.2f\n", matrix[0].z, matrix[1].z, matrix[2].z, matrix[3].z);
    printf("%4.2f\t%4.2f\t%4.2f\t%4.2f\n", matrix[0].w, matrix[1].w, matrix[2].w, matrix[3].w);

    fclose(f);
}

void print_vector3d(point3d_t point){
    printf("[%4.2f  %4.2f  %4.2f]",point.x,point.y,point.z);
}
