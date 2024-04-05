#include <stdio.h>
#include <string.h>

#define DEFAULT_WIDTH 1000
#define DEFAULT_HEIGHT 1000
#define STR_MAX 256

struct render{
    float x_min; float y_min; 
    float x_max; float y_max;
    int width;
    int height;
    int max_iter;
    int radius;
    FILE *fout;
    int img[DEFAULT_WIDTH][DEFAULT_HEIGHT]; 
    char basename[STR_MAX]; 
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
void get_pixel_color(struct render *set, int Px, int Py, int *r, int *g, int *b) ; 

void save_image(struct render *set) ;

int main(){
    struct render set; 

    render_init(&set);
    render_image(&set); 
    save_image(&set); 

    return 0;
}


void render_init(struct render *set){
    set->x_min = -2.; set->x_max = 0.48; 
    set->y_min = -1.24; set->y_max = 1.24;
    set->width = DEFAULT_WIDTH; set->height = DEFAULT_HEIGHT; 
    set->max_iter = 100; set->radius = 2; 
    strcpy(set->basename, "mandel"); 
}

void render_image(struct render *set){
    int P_x; int P_y; 
    double x; double y; double xtemp; 
    double x0; double y0; 
    int iter; 
    for (P_y = 0; P_y < DEFAULT_HEIGHT; P_y++){
        for (P_x = 0; P_x < DEFAULT_WIDTH; P_x++){
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
            set->img[P_x][P_y] = iter;
            
        }
    printf("Ligne en cours de calcul : %d\r", P_y);
    }
}


void get_pixel_color(struct render *set, int Px, int Py, int *r, int *g, int *b){
    int temp; 
    temp = set->img[Px][Py]; 

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
    fprintf(set->fout, "P3\n%d %d\n%d\n", DEFAULT_WIDTH, DEFAULT_HEIGHT, nb_colors); /*Entete*/
    fprintf(set->fout, "\n#Nombre d'itérations max = %d, Zone du plan complexe : x ∈ [%f; %f], y ∈ [%f; %f] \n", set->max_iter, set->x_min, set->x_max, set->y_min, set->y_max); /*Commentaire*/
    for (P_y = 0; P_y < DEFAULT_HEIGHT; P_y++){
        for (P_x = 0; P_x < DEFAULT_WIDTH; P_x++){
            temp = nb_colors*(set->img[P_x][P_y]/set->max_iter);  /*noir ou blanc*/
            
            fprintf(set->fout, "%i %i %i\n", temp, temp, temp); 
        }
        fprintf(set->fout, "\n");
    }
    fclose(set->fout);
}

