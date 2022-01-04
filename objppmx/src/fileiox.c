#include "fileiox.h"

int validate_extension(char * _path, char * extension){
    char * token,path[LINE_READ_BUFFER_SIZE];

    strcpy(path, _path);
    strtok(path, ".");
    token = strtok(NULL, "\n");

    if(strcmp(token,extension)==0){
        return 0;
    }else return 1;
}

void write_ppm(raster_t * raster, char * file_path){
    FILE * p_ppm = fopen(file_path,"w");
    register int x,y;
    if(p_ppm==NULL){
        perror("Cannot open file\n");
        exit(EXIT_FAILURE);
    }
    fprintf(p_ppm,"P3\t\n%d %d\n255\n",raster->width,raster->height);
    for(y=0;y<raster->height;y++){
        for(x=0;x<raster->width;x++){
            if(raster->map[y][x]==1){
                fprintf(p_ppm,"%d %d %d",raster->rgb_map[y][x].r,raster->rgb_map[y][x].g,raster->rgb_map[y][x].b);
            }else{
                fprintf(p_ppm,"0 0 0");
            }
            if(x<raster->width) fprintf(p_ppm,"\t");
        }
        fprintf(p_ppm,"\n");
    }

    fclose(p_ppm);    
}

raster_t * init_raster(unsigned int width, unsigned int height){
    raster_t * raster =(raster_t *)malloc(sizeof(raster_t));
    register int i;

    raster->width = width;
    raster->height = height;

    raster->map = (unsigned char **)calloc(raster->height, sizeof(unsigned char *));
    raster->rgb_map = (pixrgb_t **)calloc(raster->height, sizeof(pixrgb_t *));

    if(raster->map==NULL||raster->rgb_map==NULL){
        printf("Error allocating memory (row)");
        return NULL;
    }

    #if DEBUG > 2
        printf("Raster row memory allocating OK\n");
    #endif

    for (i = 0; i < raster->height; i++)
    {
        raster->map[i] = (unsigned char *)calloc(raster->width, sizeof(unsigned char));
        raster->rgb_map[i] = (pixrgb_t *)calloc(raster->width, sizeof(pixrgb_t));
        if(raster->map[i]==NULL||raster->rgb_map[i]==NULL){
            printf("Error allocating memory (column)");
            return NULL;
        }
    }

    #if DEBUG > 2
        printf("Raster column memory allocating OK\n");
    #endif

    return raster;
}

void clear_raster(raster_t * raster){
    register int k;

    for (k = 0; k < raster->height; k++)
    {
        memset(raster->map[k], 0, sizeof(unsigned char) * raster->width);
    }
}

void map_points(raster_t * raster, point2d_stack * coords, pixrgb_t * color){
    unsigned int x,y;
    do
    {

        y=(int)coords->point.y;
        x=(int)coords->point.x;
        //printf("Map: %d,%d\n",x,y);
        center_point(&x, &y);

        y = (y >= raster->height) ? raster->height - 1 : y;
        x = (x >= raster->width) ? raster->width - 1 : x;

        if (raster->map[y][x] == 0)
        {
            raster->map[y][x] = 1;
            raster->rgb_map[y][x].r = color->r;
            raster->rgb_map[y][x].g = color->g;
            raster->rgb_map[y][x].b = color->b;
        }
    }while(!point2d_stack_pop(&coords));
    //while (point_iter != coords->first);
}

void map_face(face_t * face, raster_t * raster, pixrgb_t * color){
    line_i line_iter;
    point2d_stack * coords=NULL;
    //point2d_list *coords=NULL;

    line_iter = face->lines->first;  

    do{
        /*printf("Calculating line: va[%4.2f %4.2f] vb[%4.2f %4.2f]\n", 
            line_iter->line.va.x,
            line_iter->line.va.y,
            line_iter->line.vb.x,
            line_iter->line.vb.y);*/
        bresenham(&line_iter->line,&coords);
        map_points(raster,coords,color);
        line_iter = line_iter->next;
        coords=NULL;

    }while (line_iter!= line_iter->first);
//    free(coords);
    coords=NULL;
    
}

void plot_line(line_t * line, raster_t * raster, scale_t * scale, pixrgb_t color, char * file){
    point2d_stack *coords=NULL;

    clear_raster(raster);
    scale_line(scale, line);
    bresenham(line, &coords);
    map_points(raster,coords,&color);
    write_ppm(raster, file);
}

void plot_face(face_t * face, raster_t * raster, scale_t * scale, char * file, pixrgb_t color){
    line_i line_iter;
    point2d_stack *coords=NULL;
    line_iter = face->lines->first;
    clear_raster(raster);

    do{
        scale_line(scale, &line_iter->line);
        bresenham(&line_iter->line, &coords);
        map_points(raster,coords,&color);
        line_iter = line_iter->next;

    }while (line_iter!= line_iter->first);
    write_ppm(raster, file);
}

void plot_object(vfhandler_s * objxhandler, raster_t * raster, char * file, pixrgb_t * color){
    register int i;
    face_t * face;
    //point2d_stack * coords=NULL;
    #if DEBUG > 2
        printf("Clearing raster...\n");
    #endif    
    clear_raster(raster);
    #if DEBUG > 2
     printf("Mapping faces...\n");
    #endif

    for (i = 0; i < objxhandler->fcount; i++)
    {
        face = &objxhandler->faces[i];
        map_face(face,raster,color);
    }
    write_ppm(raster, file);
}

config_h * parse_config(char * file){
    FILE * input_file;
    config_h * config;
    char buffer[LINE_READ_BUFFER_SIZE], * token;

    if(validate_extension(file,"oxp")){
        printf("Bad file extension - exit\n");
        return NULL;
    }

    input_file = fopen(file,"r");

    config = (config_h *)malloc(sizeof(config_h));

    while(fgets(buffer,LINE_READ_BUFFER_SIZE,input_file)!=NULL){
        token = strtok(buffer,":");

        if(strcmp(token,"file")==0){
            token = strtok(NULL,"\n");
            strcpy(config->source,token);
        }
        if(strcmp(token,"output")==0){
            token = strtok(NULL,"\n");
            strcpy(config->output,token);
        }
        if(strcmp(token,"width")==0){
            token = strtok(NULL,"\n");
            config->width = atoi(token);
        }
        if(strcmp(token,"height")==0){
            token = strtok(NULL,"\n");
            config->height = atoi(token);
        }
        if(strcmp(token,"scale")==0){
            token = strtok(NULL,"\n");
            config->scale = atof(token);
        }
        if(strcmp(token,"translate")==0){
            token = strtok(NULL," ");
            config->translate.x = atof(token);

            token = strtok(NULL," ");
            config->translate.y = atof(token);

            token = strtok(NULL," ");
            config->translate.z = atof(token);
        }
        
        if(strcmp(token,"center")==0){
            token = strtok(NULL," ");
            config->center.x = atof(token);

            token = strtok(NULL," ");
            config->center.y = atof(token);

            token = strtok(NULL," ");
            config->center.z = atof(token);
        }
        if(strcmp(token,"animation")==0){
            token = strtok(NULL,"\n");
            strcpy(config->animation,token);
        }        
    }

    fclose(input_file);

    if(config->source==NULL||config->width==0||config->height==0) return NULL;
   
    return config;

}

animation_stack * load_animation_stack(char * file_path){
    FILE * input_file;
    animation_stack * animation_top = NULL;
    animation_frame * frame;
    char buffer[LINE_READ_BUFFER_SIZE], * token;
    #if DEBUG > 2
        printf("[X] Animation stack loading\n      sx sy sz tx ty tz ax ay az cx cy cz\n");
    #endif


    if(validate_extension(file_path,"astck")){
        printf("Bad file extension - exit\n");
        return NULL;
    }

    input_file = fopen(file_path,"r");

    while(fgets(buffer,LINE_READ_BUFFER_SIZE,input_file)!=NULL){
        if(buffer[0]!='#'){
            frame = (animation_frame *)malloc(sizeof(animation_frame));

            token = strtok(buffer," ");
            frame->scale.x = atof(token);

            token = strtok(NULL," ");
            frame->scale.y = atof(token);

            token = strtok(NULL," ");
            frame->scale.z = atof(token);

            token = strtok(NULL," ");
            frame->translate.x = atof(token);

            token = strtok(NULL," ");
            frame->translate.y = atof(token);

            token = strtok(NULL," ");
            frame->translate.z = atof(token);

            token = strtok(NULL," ");
            frame->rotation.cos.x = cos(atof(token));
            frame->rotation.sin.x = sin(atof(token));

            token = strtok(NULL," ");
            frame->rotation.cos.y = cos(atof(token));
            frame->rotation.sin.y = sin(atof(token));

            token = strtok(NULL," ");
            frame->rotation.cos.z = cos(atof(token));
            frame->rotation.sin.z = sin(atof(token));

            token = strtok(NULL," ");
            frame->rotation.center.x = atof(token);

            token = strtok(NULL," ");
            frame->rotation.center.y = atof(token);

            token = strtok(NULL," ");
            frame->rotation.center.z = atof(token);

            if(animation_stack_push(&animation_top, frame)){
                printf("Cannnot create animation stack structure or push into it\n");
                return NULL;
            }
            #if DEBUG > 2
                printf("frame: %.2f %.2f %.2f %.2f %.2f %.2f %.2f %.2f %.2f %.2f %.2f %.2f %.2f %.2f %.2f\n",
                    frame->scale.x,
                    frame->scale.y,
                    frame->scale.z,
                    frame->translate.x,
                    frame->translate.y,
                    frame->translate.z,
                    frame->rotation.cos.x,
                    frame->rotation.cos.y,
                    frame->rotation.cos.z,
                    frame->rotation.sin.x,
                    frame->rotation.sin.y,
                    frame->rotation.sin.z,
                    frame->rotation.center.x,
                    frame->rotation.center.y,
                    frame->rotation.center.z
                );
            #endif
        }
    }
    #if DEBUG > 2
        printf("[X]OK\n");
    #endif
    return animation_top;
}