#include <stdio.h>


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
    int W; int H; 
    int iter; 
    float x_min; float x_max; float y_min; float y_max;


    /*Initialisations*/
    max_iter = 100; radius = 2; 
    W = 160; H = 50; 
    x_min = -2.; x_max = 0.48; y_min = -1.24; y_max = 1.24;


    for (P_y = 0; P_y < H; P_y++){
        for (P_x = 0; P_x < W; P_x++)
        {
            /*Déterminer le point dans le plan complexe*/
            x0 = map(P_x, 0, W, x_min, x_max); 
            y0 = map(P_y, 0, H, y_min, y_max); 
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

            if (x*x + y*y < radius*radius){
                printf("*");
                
            }
            else{
                printf(" ");

            }

        }
        printf("\n");
    }



    return 0;
}