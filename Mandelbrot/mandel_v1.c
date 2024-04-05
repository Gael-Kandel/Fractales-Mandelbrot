#include <stdio.h>

#define DEFAULT_WIDTH 1000
#define DEFAULT_HEIGHT 1000



/*pour une valeur v de l’intervalle [imin;imax], retourne la valeur proportionnelle dans l’intervalle  [omin;omax*/
double map(int v, int imin, int imax, double omin, double omax){
    return (double) (v-imin)/(imax-imin)*(omax-omin) + omin; 
}




int main(){    
    /*Déclarations*/
    int max_iter; int radius; 
    int P_x; int P_y; 
    double x; double y; double xtemp; 
    double x0; double y0; 
    int iter; 
    int nb_colors; 
    float x_min; float x_max; float y_min; float y_max;
    int img[DEFAULT_WIDTH][DEFAULT_HEIGHT];
    FILE* fout;

    /*Initialisations*/
    nb_colors = 255;
    max_iter = 100; radius = 2; 
    x_min = -2.; x_max = 0.48; y_min = -1.24; y_max = 1.24;
    

    for (P_y = 0; P_y < DEFAULT_HEIGHT; P_y++){
        for (P_x = 0; P_x < DEFAULT_WIDTH; P_x++)
        {
            /*Déterminer le point dans le plan complexe*/
            x0 = map(P_x, 0, DEFAULT_WIDTH, x_min, x_max); 
            y0 = map(P_y, 0, DEFAULT_HEIGHT, y_min, y_max); 
            /*Initialiser les variables*/
            iter = 0; 
            x = x0; y = y0; 

            while (x*x + y*y < radius*radius && iter < max_iter)
            {
                xtemp = x*x - y*y +x0; 
                y = 2*x*y + y0; 
                x = xtemp; 
                iter++;
            }
            img[P_x][P_y] = iter;
        }
    }
    /*En ppm*/
    fout = fopen("mandel.ppm", "w");
    fprintf(fout, "P3\n%d %d\n%d\n", DEFAULT_WIDTH, DEFAULT_HEIGHT, nb_colors); /*Entete*/
    fprintf(fout, "\n#Nombre d'itérations max = %d, Zone du plan complexe : x ∈ [%f; %f], y ∈ [%f; %f] \n", max_iter, x_min, x_max, y_min, y_max); /*Commentaire*/
    for (y = 0; y < DEFAULT_HEIGHT; y++){
        for (x = 0; x < DEFAULT_WIDTH; x++){
            fprintf(fout, "%d %d %d\n", nb_colors*(img[(int)x][(int)y]/max_iter), nb_colors*(img[(int)x][(int)y]/max_iter), nb_colors*(img[(int)x][(int)y]/max_iter)); /*noir ou blanc*/
        }
        fprintf(fout, "\n");
    }
    fclose(fout);


    return 0;
}