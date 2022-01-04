#include "objppmx.h"
extern unsigned int raster_center_x;
extern unsigned int raster_center_y;

pthread_mutex_t vfthread_lock;

void * vfcount_elements(void * _thread_vfhandler){
    vfthread_h * thread_vfhandler = (vfthread_h *)_thread_vfhandler;
    vfhandler_s * vfhandler = thread_vfhandler->vfhandler;  
    char * thread_name = thread_vfhandler->name;
    char line[OBJFILE_LINE_SIZE];
    char line_id = strcmp(thread_name,"vcount_thread") == 0 ? 'v':'f';
    long int count=0;
    fpos_t offset;
    FILE * f_head = fopen(vfhandler->source_file_path,"r");

    if(f_head==NULL){
        printf("Error opening file %s",vfhandler->source_file_path);
        pthread_exit(NULL);
    }

    while(fgets(line,OBJFILE_LINE_SIZE,f_head)!=NULL&&count==0){
        if(line[0]==line_id){
            if(fgetpos(f_head, &offset)){
                printf("Error: getting offset at %s\n",thread_name);
                pthread_exit(NULL);
            }else{
                offset.__pos-=strlen(line);
                count++;           
            }
        }
    }

    while(fgets(line,OBJFILE_LINE_SIZE,f_head)!=NULL){
        if(line[0]==line_id) count++;
    }

    pthread_mutex_lock(&vfthread_lock);
        if(line_id=='v'){
            vfhandler->vcount = ++count;
            vfhandler->voffset = offset;
        }else{
            vfhandler->fcount = ++count;
            vfhandler->foffset = offset;
        }
    pthread_mutex_unlock(&vfthread_lock);

    fclose(f_head);
    pthread_exit((void *)_thread_vfhandler);
}

void vertexes_load(void * _vfhandler){
    vfhandler_s * vfhandler = _vfhandler;  
    FILE * f_head = fopen(vfhandler->source_file_path,"r");
    char buffer[OBJFILE_LINE_SIZE], * token;
    float x,y,z;
    register int vindex=0;

    #if DEBUG > 2
        printf(" vertexes load whence byte 0x%.4lX\n ", vfhandler->voffset.__pos);
    #endif

    fseek(f_head,vfhandler->voffset.__pos,SEEK_SET);

    for(vindex=0;vindex<vfhandler->vcount;vindex++){
        
        pthread_mutex_lock(&vfthread_lock);
        fgets(buffer,OBJFILE_LINE_SIZE,f_head);

        if(buffer!=NULL){
            token = strtok(buffer," ");

            token=strtok(NULL," ");
            x = (token!=NULL) ?  atof(token) : 0;

            token=strtok(NULL," ");
            y = (token!=NULL) ?  atof(token) : 0;

            token=strtok(NULL," ");
            z = (token!=NULL) ?  atof(token) : 0;

            pthread_mutex_unlock(&vfthread_lock);

            vfhandler->vertexes[vindex].x = x;
            vfhandler->vertexes[vindex].y = y;
            vfhandler->vertexes[vindex].z = z;       
        }        
    }
    fclose(f_head);

    #if DEBUG > 2
    printf(" vertexes loaded OK\n");
    #endif

    pthread_exit(_vfhandler);
}

void faces_load(void * _vfhandler){
    char buffer[OBJFILE_LINE_SIZE], * token;
    vfhandler_s * vfhandler = _vfhandler;  
    FILE * f_head = fopen(vfhandler->source_file_path,"r");
    uint_list * p_uinthead;
    register int findex=0;

    #if DEBUG > 2
        printf("faces load whence byte 0x%.4lX\n", vfhandler->foffset.__pos);
    #endif

    fseek(f_head,vfhandler->foffset.__pos,SEEK_SET);

    // Se debe manejar archivos diferentes v...f...

    for(findex=0;findex<vfhandler->fcount;findex++){
        pthread_mutex_lock(&vfthread_lock);
        fgets(buffer,OBJFILE_LINE_SIZE,f_head);

        if(buffer!=NULL){
            p_uinthead = NULL;
            token = strtok(buffer," ");

            while(token!=NULL){
                token = strtok(NULL," ");
                if(token!=NULL){
                    if(uint_list_insert(atoi(token),&p_uinthead)){
                        printf("Error: inserting face vertex into face lit\n");
                        pthread_exit(NULL);
                    }                
                }
            }
            pthread_mutex_unlock(&vfthread_lock);
            vfhandler->faces[findex].vertexes = p_uinthead;
        }
    }

    fclose(f_head);

    #if DEBUG > 2
        printf(" faces loaded OK\n");
    #endif

    pthread_exit(_vfhandler);
}

int vfinit(vfhandler_s * vfhandler, char * source_file_path){
    pthread_t thread_id[2];
    vfthread_h thread_vfhandler[2], * thread_return;
    register int i;

    if (pthread_mutex_init(&vfthread_lock, NULL) != 0) {
        perror("Mutex init has failed\n");
        return 1;
    }

    thread_vfhandler[0].vfhandler = vfhandler;
    thread_vfhandler[0].name = "vcount_thread";

    thread_vfhandler[1].vfhandler = vfhandler;
    thread_vfhandler[1].name = "fcount_thread";

    #if DEBUG > 2
    printf("Vertexes and faces count started > ");
    #endif

    for(i=0;i<2;i++) pthread_create(&thread_id[i], NULL,(void *)vfcount_elements, (void *)&thread_vfhandler[i]);
    for(i=0;i<2;i++){
        pthread_join(thread_id[i],(void *)&thread_return);
        if(thread_return==NULL){
            printf("Error: Cannot count/missing values at %s\n",(*thread_return).name);
            return 1;                
        }
    }

    #if DEBUG > 2
        printf("vertexes: %ld\tfaces:%ld\n",vfhandler->vcount, vfhandler->fcount);
    #endif

    vfhandler->vertexes = (vertex_t *)calloc(vfhandler->vcount,(sizeof(vertex_t)));
    vfhandler->faces = (face_t *)calloc(vfhandler->fcount,(sizeof(face_t)));

    if(vfhandler->vertexes==NULL||vfhandler->faces==NULL){
        printf("Error: Cannot allocate memory for file load\n");
        exit(EXIT_FAILURE);
        return 1;
    }

    #if DEBUG > 2
        printf("Memory allocated \n");
        printf("Vertex and faces load start >\n");
    #endif

    pthread_create(&thread_id[0], NULL,(void *)vertexes_load, (void *)vfhandler);
    pthread_create(&thread_id[1], NULL,(void *)faces_load, (void *)vfhandler);

    for(i=0;i<2;i++){
        pthread_join(thread_id[i],(void *)&thread_return);
        if(thread_return==NULL){
            printf("Error: Cannot load elements into memory\n");
            return 1;                
        }
    }

    pthread_mutex_destroy(&vfthread_lock);
    return 0;
}

parallel_h * init_parallel(int ncore, int core, long int length){
    parallel_h * phandler = (parallel_h *)malloc(sizeof(parallel_h));
    phandler->block_size = length/ncore;
    phandler->init_index = phandler->block_size*core;
    phandler->end_index = phandler->init_index + phandler->block_size;

    if(core==(ncore-1)) phandler->end_index+=length-phandler->end_index;

    #if DEBUG > 1
    printf("  core: %d\tblock_size: %ld\tinit_index:%ld\tend_index:%ld\n",
            core,phandler->block_size,phandler->init_index,phandler->end_index);
    #endif

    return phandler;
}

void face_line_load(void * arg){
    register int i, va_index,vb_index;
    vfthread_h * thread_vfhandler = (vfthread_h *)(arg);
    vfhandler_s * vfhandler = thread_vfhandler->vfhandler;
    parallel_h * parl = init_parallel(thread_vfhandler->ncore, atoi(thread_vfhandler->name), vfhandler->fcount);

    uint_list * p_vertex;
    line_list * face_lines;
    line_t * n_line = NULL;
    vertex_t * vertexes = vfhandler->vertexes;
    face_t * faces = vfhandler->faces;

    for(i=parl->init_index;i<parl->end_index;i++){
        face_lines = NULL;
        p_vertex = faces[i].vertexes->first;
        do{
            va_index = p_vertex->value-1;
            vb_index = p_vertex->next->value-1;

            n_line = new_line(&vertexes[va_index],&vertexes[vb_index]);
            
            line_list_insert(n_line,&face_lines);

            p_vertex=p_vertex->next;

        }while(p_vertex!=faces[i].vertexes->first);

        faces[i].lines = face_lines;
    }

    pthread_exit((void *)arg);
}

int lines_load(vfhandler_s * vfhandler){
    long ncore = sysconf(_SC_NPROCESSORS_ONLN);
    pthread_t thread_id[ncore];
    vfthread_h thread_vfhandler[ncore],* thread_return;
    int i;

    #if DEBUG > 2
    printf("Loading lines using %ld cores >\n",ncore);
    #endif

    for(i=0;i<ncore;i++){
        thread_vfhandler[i].name = (char *)malloc(2);
        sprintf(thread_vfhandler[i].name,"%d",i);
        thread_vfhandler[i].ncore = ncore;
        thread_vfhandler[i].vfhandler = vfhandler;
        
        pthread_create(&thread_id[i], NULL,(void *)face_line_load, (void *)&thread_vfhandler[i]);
    }
    
    for(i=0;i<ncore;i++){
        pthread_join(thread_id[i],(void *)&thread_return);
        if(thread_return==NULL){
            printf("Error: Cannot load lines\n");
            return 1;
        }
    }
    return 0;
}


void free_objx(vfhandler_s *vfhandler)
{
    free(vfhandler->vertexes);
    free(vfhandler->faces->vertexes);
    free(vfhandler->faces->lines);
    free(vfhandler->faces);
}

/// API

config_h * oxp_load_config(char * config_file_path){
    config_h * config;

    #if DEBUG > 1
        printf("[A] Configuration file loading\n");
    #endif
    if((config = parse_config(config_file_path))==NULL){
        return NULL;
    }

    #if DEBUG >= 2
    printf("OBJ source file: %s\nPPM output image dimensions: %dx%dpx\n",config->source, config->width,config->height);
    printf("Initials:\n  scale: %.2f\n  translation: [%.2f %.2f %.2f]\n  projection center: [%.2f %.2f %.2f]\n", config->scale,config->translate.x,config->translate.y,config->translate.z, config->center.x, config->center.y, config->center.z);
    printf("[A] Ok\n");
    #endif

    return config;
}


vfhandler_s * oxp_load_objx(char *file_path){
    vfhandler_s *vfhandler = (vfhandler_s *)malloc(sizeof(vfhandler_s));

    #if DEBUG > 1
        printf("[B] Load object into memory\n");
    #endif

    if (!validate_extension(file_path, "obj"))
    {
        vfhandler->source_file_path = file_path;
        #if DEBUG > 1
            printf("Source file extension OK\n");
        #endif

        if (vfinit(vfhandler, file_path))
        {
            printf("Cannot load vertexes and faces' list\n");
            return NULL;
        }

        #if DEBUG > 1
            printf("Vertexes and faces' list loaded into memory\n ");
        #endif

        if (lines_load(vfhandler))
        {
            printf("Cannot process lines - exit\n");
            return NULL;
        }

        #if DEBUG > 1
            printf("Lines loaded into memory\n");
        #endif        
    }
    else
    {
        printf("Bad file extension\n");
        return NULL;
    }

    free(vfhandler->vertexes);
    vfhandler->vertexes=NULL;
    free(vfhandler->faces->vertexes);
    vfhandler->faces->vertexes=NULL;    

    #if DEBUG > 1
        printf("[B] Ok\n");
    #endif

    return vfhandler;
}

raster_t * oxp_init_raster(unsigned int width, unsigned int height){
    raster_t * raster;
    #if DEBUG > 1
        printf("[C] Raster and RGBRaster allocation\n");
    #endif

    if((raster = init_raster(width, height))==NULL){
        return NULL;
    }

    raster_center_x = (raster->width / 2) - 1;
    raster_center_y = (raster->height / 2) - 1;
    
    #if DEBUG > 1
        printf("[C] Ok\n");
    #endif

    return raster;
}

int oxp_plot_object(vfhandler_s * objxhandler, raster_t * raster, char * file, pixrgb_t * color){
    int status;
    pid_t pid;

    #if DEBUG > 2
    printf("[oxp:plot_object] Writing frame: %s\n", file);
    #endif
    pid = fork();

    if(pid==-1){
        printf("Error creating forked process\n");
        return 1;
    }
    
    if(!pid){
        plot_object(objxhandler,raster,file,color);
        exit(0);
    }else{
        pid = wait(&status);
        if(status>>8==0){
            #if DEBUG > 2
                printf("[oxp:plot_object] Frame written: %s\n", file);
            #endif
            return 0;
        }else{
            #if DEBUG > 2
                printf("[oxp:plot_object] Error writing frame: %s\n", file);
            #endif
            return 1;
        }
    }
}