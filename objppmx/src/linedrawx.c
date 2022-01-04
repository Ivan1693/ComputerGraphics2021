#include "linedrawx.h"
#include <stdio.h>
#include "dstructx.h"
#include "ptransformx.h"

void special_case(line_t * line, point2d_stack ** coords){
    int x1,x2,y1,y2,x,y;
    int p0,pf,p,dx,dy,length;

    x1 =(int)line->va.x;
    y1 =(int)line->va.y;

    x2 =(int)line->vb.x;
    y2 =(int)line->vb.y;


    dx = x2-x1;
    dy = y2-y1;


    if(dx==0){
        p0 = y1;
        pf = y2;
        if(p0>pf) iswap(&p0,&pf);

        length = (pf-p0)+1;

        for(p=0;p<=length;p++){
            //point2d_list_insert(x1,p0+p,coords);
            point2d_stack_push(x1,p0+p,coords);
        }
        return;
    }

    if(dy==0){
        p0 = x1;
        pf = x2;
        if(p0>pf) iswap(&p0,&pf);

        length = (pf-p0)+1;
        
        for(p=0;p<length;p++){
            //point2d_list_insert(p0+p,y1,coords);
            point2d_stack_push(p0+p,y1,coords);
        }
        return;
    }

    if((dy/dx)==1){
        //printf("> Positive slope\n");
        if(x1<x2){
            //printf("> Forward direction\n");
            for(x=x1, y=y1;x<=x2;x++,y++){
                //point2d_list_insert(x,y,coords);
                point2d_stack_push(x,y,coords);                
            }
        }else{
            //printf("> Back direction x: %d - %d ; y: %d - %d\n",x1,x2,y1,y2);
            for(x=x1, y=y1;x>=x2;x--,y--){
//                point2d_list_insert(x,y,coords);
                  point2d_stack_push(x,y,coords);
            }            
        }
    }else{
        //printf("> Negative slope\n");
        if(x1<x2){
            //printf("> Forward direction\n");
            for(x=x1, y=y1;x<=x2;x++, y--){
                //point2d_list_insert(x,y,coords);
                point2d_stack_push(x,y,coords);                
            }
        }else{
            //printf("> Back direction\n");
            for(x=x1, y=y1;x>=x2;x--, y++){
                //point2d_list_insert(x,y,coords);
                point2d_stack_push(x,y,coords);
            }
        }
    }
    //point2d_list_print("[%4.0f %4.0f]\n",*coords);
}

void bresenham(line_t * line, point2d_stack ** coords){
    int x1,x2,y1,y2;
    int dx,dy,p,neg,pos;
    int x,y,endpoint,inc;
    float slope;

    x1 =(int)line->va.x;
    y1 =(int)line->va.y;

    x2 =(int)line->vb.x;
    y2 =(int)line->vb.y;


    dx = abs(x1-x2);
    dy = abs(y1-y2);

    if(dx==0){
        special_case(line,coords);
        return;
    }else {
        if(dy==0||(dy/dx)==1){
            special_case(line,coords);
            return;
        }
    }  

    slope=(float)(y2-y1)/(x2-x1);

    if(abs(slope)<1){
        p = 2*dy-dx;
        neg = 2*dy;
        pos = 2*(dy-dx);
        inc =(float)(y2-y1)/(x2-x1) > 0 ? 1 : -1;

        //printf("m=%s %4.2f\n",(float)(y2-y1)/(x2-x1) > 0 ? "positive" : "negative", slope );

        if(x1>x2){
            x = x2;
            y = y2;
            endpoint = x1;
        }else{
            x=x1;
            y=y1;
            endpoint = x2;
        }

        //point2d_list_insert(x,y,coords);
        point2d_stack_push(x,y,coords);

        while(x<endpoint){
            x++;
            if(p<0){
                p+=neg;
            }else{
                y+=inc;
                p+=pos;
            }
            //point2d_list_insert(x,y,coords);
            point2d_stack_push(x,y,coords);
        }
    }else{
        p=2*dx-dy;
        neg=2*dx;
        pos=2*(dx-dy);
        inc =(float)(x2-x1)/(y2-y1) > 0 ? 1 : -1;
        //printf("m=%s %4.2f\n",(float)(x2-x1)/(y2-y1) > 0 ? "positive" : "negative", slope );    

        if(y1>y2){
            x = x2;
            y = y2;
            endpoint = y1;
        }else{
            x=x1;
            y=y1;
            endpoint = y2;
        }

        //point2d_list_insert(x,y,coords);
        point2d_stack_push(x,y,coords);

        while(y<endpoint){
            y++;
            if(p<0){
                p+=neg;
            }else{
                x+=inc;
                p+=pos;
            }
            //point2d_list_insert(x,y,coords);
            point2d_stack_push(x,y,coords);
        }
    }

    //point2d_list_print("[%4.0f %4.0f]\n",*coords);
}
/*
void dda(line_t * line, point2d_list ** coords){
    int x1,y1,x2,y2;
    int dx, dy, x_inc, inc, x, y;
    int p,steps;

    x1 =(int)line->va.x;
    y1 =(int)line->va.y;

    x2 =(int)line->vb.x;
    y2 =(int)line->vb.y;


    x=x1;
    y=y1;

    dx=x2-x1;
    dy=y2-y1;

    if(dx==0){
        special_case(line,coords);
        return;
    }else {
        if(dy==0||(dy/dx)==1){
            special_case(line,coords);
            return;
        }
    }

    if(abs(dx)>abs(dy)) steps = abs(dx);
    else steps = abs(dy);

    x_inc = dx / (float) steps;
    inc = dy / (float) steps;
    
    point2d_list_insert(x,y,coords);

    for(p=1;p<steps;p++){
        x+=x_inc;
        y+=inc;
        point2d_list_insert(x,y,coords);
    }

}

void slope_intercept(line_t * line, point2d_list ** coords){
    float dx,dy,m,b;
    float x1,y1,x2,y2;
    int p0,pf,p;

    x1 = line->va.x;
    y1 = line->va.y;

    x2 = line->vb.x;
    y2 = line->vb.y;


    dx=x2-x1;
    dy=y2-y1;


    if(dx==0){
        special_case(line,coords);
        return;
    }else {
        if(dy==0||(dy/dx)==1){
            special_case(line,coords);
            return;
        }
    }

    p0 = x1;
    pf = x2;
    if(p0>pf) iswap(&p0,&pf);

    
    m = dy/dx;
    b = y1 - m*x1;
    for(p=p0;p<=pf;p++){
        point2d_list_insert(p,m*p + b,coords);
    }
}
*/