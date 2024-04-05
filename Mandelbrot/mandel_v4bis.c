#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define DEFAULT_WIDTH 1000
#define DEFAULT_HEIGHT 1000
#define STR_MAX 256

struct camera{
    double x; 
    double y; 
    double width; 
    double height; 
}; 

struct render{
    float x_min; float y_min; 
    float x_max; float y_max;
    int width;
    int height;
    int max_iter;
    int radius;
    FILE *fout;
    int *img; 
    char basename[STR_MAX]; 
    struct camera pov; 
}; 

/*pour une valeur v de l’intervalle [imin;imax], retourne la valeur proportionnelle dans l’intervalle  [omin;omax]*/
double map(int v, int imin, int imax, double omin, double omax){
    return (double) (v-imin)/(imax-imin)*(omax-omin) + omin; 
}

/* */
void render_init(struct render *set) ;

/* */
void render_image(struct render *set) ;

/* */
void get_pixel_color(struct render *set, int P_x, int P_y, int *r, int *g, int *b) ; 

void save_image(struct render *set) ;

int m2v(struct render *set, int x, int y) ;

void cam2rect(struct render *set, struct camera *pov); 

int main(){
    struct render set; 
    struct camera pov; 

    pov.x = -0.76; 
    pov.y = 0.; 
    pov.width = 2.48; 
    pov.height = 2.48; 
    
    cam2rect(&set, &pov); 
    render_init(&set);
    render_image(&set); 
    save_image(&set); 

    free(set.img); 
    return 0;
}


void render_init(struct render *set){
    set->width = DEFAULT_WIDTH; set->height = DEFAULT_HEIGHT; 

    set->img = (int*) malloc(set->width*set->height*sizeof(int)); 
    if (set->img == NULL) {
        fprintf(stderr, "Erreur d'allocation mémoire pour set->img\n");
        exit(EXIT_FAILURE);
    }

    set->max_iter = 100; 
    set->radius = 2; 
    strcpy(set->basename, "mandel"); 
}

void render_image(struct render *set){
    int P_x; int P_y; 
    double x; double y; double xtemp; 
    double x0; double y0; 
    int iter; 
    for (P_y = 0; P_y < set->height; P_y++){
        for (P_x = 0; P_x < set->width; P_x++){
            /*Déterminer le point dans le plan complexe*/
            x0 = map(P_x, 0, set->width, set->x_min, set->x_max); 
            y0 = map(P_y, 0, set->height, set->y_min, set->y_max); 
            /*Initialiser les variables*/
            iter = 0; 
            x = x0; y = y0; 

            while (x*x + y*y < set->radius*set->radius && iter < set->max_iter){
                xtemp = x*x - y*y +x0; 
                y = 2*x*y + y0; 
                x = xtemp; 
                iter++;
            }
            set->img[m2v(set, P_x, P_y)] = iter;
            
        }
    /*printf("Ligne en cours de calcul : %d\r", P_y);*/
    }
}


void get_pixel_color(struct render *set, int P_x, int P_y, int *r, int *g, int *b){
    int temp; 
    temp = set->img[m2v(set, P_x, P_y)]; 

    *r = temp; 
    *g = temp; 
    *b = temp; 
}

void save_image(struct render *set){
    int P_x; int P_y; 
    char filename[STR_MAX+4]; /*4 pour .ppm"*/
    int nb_colors; 
    int temp; 
 
    /*Initialisations*/
    nb_colors = 255;
    strcpy(filename, set->basename); 
    strcat(filename, ".ppm"); 
       
    /*Ecriture dans le fichier*/
    set->fout = fopen(filename, "w");
    fprintf(set->fout, "P3\n%d %d\n%d\n", set->width, set->height, nb_colors); /*Entete*/
    fprintf(set->fout, "\n#Nombre d'itérations max = %d, Zone du plan complexe : x ∈ [%f; %f], y ∈ [%f; %f] \n", set->max_iter, set->x_min, set->x_max, set->y_min, set->y_max); /*Commentaire*/
    for (P_y = 0; P_y < set->height; P_y++){
        for (P_x = 0; P_x < set->width; P_x++){
            temp = (set->img[m2v(set, P_x, P_y)]/set->max_iter)*nb_colors;  
            
            fprintf(set->fout, "%i %i %i\n", temp, temp, temp); /*nuances de gris*/
        }
        fprintf(set->fout, "\n");
    }
    fclose(set->fout);
}

int m2v(struct render *set, int x, int y) {
    return x+set->width*y; 
}


void cam2rect(struct render *set, struct camera *pov){
    set->x_min = pov->x - pov->width/2; 
    set->x_max = pov->x + pov->width/2; 
    set->y_min = pov->y - pov->height/2; 
    set->y_max = pov->y + pov->height/2; 
}