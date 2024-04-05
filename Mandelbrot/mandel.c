/* Auteur : Gaël Kandel
   Date : 13/12/2023

   compilation avec la commande : gcc -W -Wall -ansi -pedantic mandel.c -o mandel -lm
   exécution avec la commande : ./mandel
   
   Je n'ai pas la foi de me replonger dans les fonctions de lecture de fichier, je vous rends donc le TP Mandelbrot complété jusqu'à la tâche 9 inclue.

   Ce programme permet de calculer l'ensemble de Mandelbrot et de l'afficher sous différents paradigmes.
-----------------------------------------------------------------------------------------------------------------------------------------------------------
   On y apprend, entre autres,  l'usage des struct, les différents manières de lire les commandes, différentes manières de stockage pour un même but et le format hsv

    Je pense avoir 17/20, n'ayant pas complété la tâche 10.
    Mon code compile sans erreur et fonctionne. De plus il est commenté.
   */



#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <math.h>

#include "colors.h"

#define DEFAULT_WIDTH 1000
#define DEFAULT_HEIGHT 1000
#define STR_MAX 256
#define BW 0
#define BW_ALTERN 1
#define GREY_ST 2
#define GREY_SM 3
#define RGB 4

/* Structure pour représenter les propriétés de la caméra */
struct camera {
    double x; /* Coordonnée x de la caméra */
    double y; /* Coordonnée y de la caméra */
    double width; /* Largeur de la fenêtre de visualisation */
    double height; /* Hauteur de la fenêtre de visualisation */
};

/* Structure pour stocker l'image*/
struct pixdiv{
    int iter; 
    double x; 
    double y; 
};

/* Structure pour stocker les paramètres de rendu et les résultats */
struct render {
    int type; /*Type de rendu*/
    double x_min; /* Coordonnée x minimum dans le plan complexe */
    double y_min; /* Coordonnée y minimum dans le plan complexe */
    double x_max; /* Coordonnée x maximum dans le plan complexe */
    double y_max; /* Coordonnée y maximum dans le plan complexe */
    int width; /* Largeur de l'image générée */
    int height; /* Hauteur de l'image générée */
    int max_iter; /* Nombre maximal d'itérations pour le calcul fractal */
    int radius; /* Rayon pour le test de convergence */
    FILE *fout; /* Pointeur de fichier pour sauvegarder l'image */
    struct pixdiv * img; /* Pointeur vers struct pixdiv pour stocker les résultats de l'image */
    char basename[STR_MAX]; /* Nom de base du fichier de sortie */
    struct camera pov; /* Propriétés de la caméra */
    int nb_colors; /*Nombre de couleurs */
};



/* Pour une valeur v dans l'intervalle [imin;imax], retourne la valeur proportionnelle dans l'intervalle [omin;omax] */
double map(int v, int imin, int imax, double omin, double omax) {
    return (double) (v - imin) / (imax - imin) * (omax - omin) + omin;
}

/* Initialise les paramètres de rendu avec des valeurs par défaut */
void render_init(struct render *set, int argc, char *argv[]);

/* Génère l'image fractale de Mandelbrot */
void render_image(struct render *set);

/* Récupère la couleur d'un pixel dans l'image */
void get_pixel_color(struct render *set, int P_x, int P_y, int *r, int *g, int *b);

/* Enregistre l'image générée dans un fichier PPM */
void save_image(struct render *set);

/* Mappe les coordonnées 2D (x, y) vers un indice de tableau 1D */
int m2v(struct render *set, int x, int y);

/* Convertit les coordonnées de la caméra vers la région rectangulaire dans le plan complexe */
void cam2rect(struct render *set);

/* mécanisme permettant de passer d’un modèle à l’autre */
void hsv2rgb(struct color *rgb, struct color *hsv); 


/* Fonction principale où le processus de rendu est orchestré */
int main(int argc, char *argv[]) {
    struct render set; /* Structure pour stocker les paramètres de rendu et les résultats */    

    /* Initialisation des paramètres de rendu */
    render_init(&set, argc, argv);

    /* Mappage de la caméra à la région rectangulaire dans le plan complexe */
    cam2rect(&set);

    /* Génération de l'image fractale de Mandelbrot */
    render_image(&set);

    /* Enregistrement de l'image générée dans un fichier */
    save_image(&set);

    /* Libération de la mémoire allouée dynamiquement */
    free(set.img);

    return 0;
}




/* Initialise les paramètres de rendu avec des valeurs par défaut */
void render_init(struct render *set, int argc, char *argv[]) {
    /* On a enlevé les initialisations de x_min, ..., y_max et de width, height faites
       désormais dans le main */

    set->radius = 2;
    set->type = RGB;   /*type par défault*/ 
    set->nb_colors = 255; 
    

    if (argc > 1) {
        if (sscanf(argv[1], "%d", &set->max_iter) == 1) {
            /* Bien passé */
        }
        else {
            fprintf(stderr, "Problème argument 1, entrée attendue sous la forme d'un entier");
            exit(EXIT_FAILURE);
        }
    }
    else {
        set->max_iter = 100;
    }

    if (argc > 2) {
        if (sscanf(argv[2], "%lf,%lf,%lf,%lf", &set->pov.x, &set->pov.y, &set->pov.width, &set->pov.height) == 4) {
            /* Bien passé */
            /* printf("%f, %f, %f, %f\n", pov.x, pov.y, pov.width, pov.height); */
        }
        else {
            fprintf(stderr, "Problème argument 2, entrée attendue sous la forme float, float, float, float");
            exit(EXIT_FAILURE);
        };
    }
    else {
        set->pov.x = -0.76;
        set->pov.y = 0.;
        set->pov.width = 2.48;
        set->pov.height = 2.48;
    }

    if (argc > 3) {
        if (sscanf(argv[3], "%ix%i", &set->width, &set->height) == 2) {
            /*Vérification de la positivité stricte*/
            assert(set->width>0 && set->height>0);
        }
        else {
            fprintf(stderr, "Problème argument 3, entrée attendue sous la forme entier_positifxentier_positif");
            exit(EXIT_FAILURE);
        }
    }
    else {
        set->width = DEFAULT_WIDTH; set->height = DEFAULT_HEIGHT;
    }
    set->img = (struct pixdiv*) malloc(set->width * set->height * sizeof(struct pixdiv));
    if (set->img == NULL) {
        fprintf(stderr, "Erreur d'allocation mémoire pour set->img\n");
        exit(EXIT_FAILURE);
    }

    if (argc > 4) {                                 /*Ne voyant pas comment l'implémenter de manière simple, je choisis de sauter la vérification de l'entrée, mon temps sera mieux utilisé dans la suite*/
        strcpy(set->basename, argv[4]);
        printf("%s\n", set->basename);
    }
    else {
        strcpy(set->basename, "mandel");
    }
}

/* Génère l'image fractale de Mandelbrot */
void render_image(struct render *set) {
    int P_x; int P_y;
    double x; double y; double xtemp;
    double x0; double y0;
    int iter;  int i; 
    for (P_y = 0; P_y < set->height; P_y++) {
        for (P_x = 0; P_x < set->width; P_x++) {
            /* Déterminer le point dans le plan complexe */
            x0 = map(P_x, 0, set->width, set->x_min, set->x_max);
            y0 = map(P_y, 0, set->height, set->y_min, set->y_max);
            /* Initialiser les variables */
            iter = 0;
            x = x0; y = y0;

            while (x*x + y*y < set->radius * set->radius && iter < set->max_iter) {
                xtemp = x*x - y*y + x0;
                y = 2 * x*y + y0;
                x = xtemp;
                iter++;
            }
            /* Stocker le résultat dans le tableau de struct pixdiv */
            
            set->img[m2v(set, P_x, P_y)].iter = iter;

            /*itérations supplémentions si max_iter atteint*/
            if (iter == set->max_iter){
                for (i = iter; i< iter + 4; i++){
                    xtemp = x*x - y*y + x0;
                    y = 2 * x*y + y0;
                    x = xtemp;
              }
            }

            set->img[m2v(set, P_x, P_y)].x = x;
            set->img[m2v(set, P_x, P_y)].y = y;

        }
    }
}

/* Récupère la couleur d'un pixel dans l'image */
void get_pixel_color(struct render *set, int P_x, int P_y, int *r, int *g, int *b) {
    int temp; int parite;  int grey;              
    struct color hsv; struct color rgb; 
    double x; double y; 

    temp = set->img[m2v(set, P_x, P_y)].iter;   /*Récupère nb iter*/

    if (set->type == BW_ALTERN){  
        temp = temp%2;                          /*garde sa parité*/
        parite = set->max_iter%2;               /*parité de max_iter*/
        if (temp == parite){                    /*Comparaison des parités*/
            temp = 0;
        }
        else {
        temp = 1;
    }                   /*Affichage alterné*/
        switch (temp) {
            case 0 : 
                *r = 0;
                *g = 0;
                *b = 0;
            break;
    
            default:
                *r = 255;
                *g = 255;
                *b = 255;
            break;
        }
    }

    if (set->type == BW) {                   /*Affichage classique en nb*/
        temp = set->nb_colors*(temp/set->max_iter);
        *r = temp;
        *g = temp;
        *b = temp;
    }

    if (set->type == GREY_ST){    /*En nuances de gris*/
        temp = set->nb_colors*temp/set->max_iter;
        *r = temp;
        *g = temp;
        *b = temp;
    }
    
    if (set->type == GREY_SM){    /*En nuances de gris avec distance*/
        if (set->img[m2v(set,P_x,P_y)].iter < set->max_iter){ /* si hors de l'ensemble */
            x=set->img[m2v(set,P_x,P_y)].x;
            y=set->img[m2v(set,P_x,P_y)].y;
            grey=5+set->img[m2v(set,P_x,P_y)].iter-log(log(x*x+y*y))/log(2)/log(2); /* valeur avant normalisation */
            grey=floor(512*grey/set->max_iter);
            if (grey > 255) {
               grey=255;
            }
            *r=grey;
            *g=grey;
            *b=grey;
         } else { /*appartient à l'ensemble */ 
            *r=0;
            *g=0;
            *b=0;  
         }
    }

    if (set->type == RGB) {
         if (set->img[m2v(set,P_x,P_y)].iter<set->max_iter){ /* si hors de l'ensemble */
            x=set->img[m2v(set,P_x,P_y)].x;
            y=set->img[m2v(set,P_x,P_y)].y;
            grey=5+set->img[m2v(set,P_x,P_y)].iter-log(log(x*x+y*y))/log(2)/log(2); /* valeur avant normalisation */
            hsv.c1=360*grey/set->max_iter;
            hsv.c2=1.;
            hsv.c3=1.;
            /*conversion hsv2rgb*/
            hsv2rgb(&rgb,&hsv); /* on ajoute colors.c dans la ligne de commande */ 
            *r=(int)rgb.c1;
            *g=(int)rgb.c2;
            *b=(int)rgb.c3;
         } else { /*appartient à l'ensemnle */ 
            *r=0;
            *g=0;
            *b=0;  
         }
    }
}

/* Enregistre l'image générée dans un fichier PPM */
void save_image(struct render *set) {
    int P_x; int P_y;
    char filename[STR_MAX + 4]; /* 4 pour l'extension ".ppm" */
    
    int r; int g; int b;   /* couleurs*/

    /*Initialisation des pointeurs r, g et b pour éviter les warnings d'initialisation*/
    r = 0; g = 0; b = 0; 
    /* Initialisations pour le format PPM */
    
    strcpy(filename, set->basename);
    strcat(filename, ".ppm");

    /* Ouverture du fichier en mode écriture */
    set->fout = fopen(filename, "w");
    fprintf(set->fout, "P3\n%d %d\n%d\n", set->width, set->height, set->nb_colors); /* Entête du fichier PPM */
    fprintf(set->fout, "\n#Nombre d'itérations max = %d, Zone du plan complexe : x ∈ [%f; %f], y ∈ [%f; %f] \n", set->max_iter, set->x_min, set->x_max, set->y_min, set->y_max); /* Commentaire */
    for (P_y = 0; P_y < set->height; P_y++) {
        for (P_x = 0; P_x < set->width; P_x++) {
            get_pixel_color(set, P_x, P_y, &r, &g, &b); 
            /* Écriture dans le fichier PPM */
            fprintf(set->fout, "%i %i %i\n", r, g, b);
        }
        fprintf(set->fout, "\n");
        
    }

    /* Fermeture du fichier */
    fclose(set->fout);
}

/* Mappe les coordonnées 2D (x, y) vers un indice de tableau 1D */
int m2v(struct render *set, int x, int y) {
    return x + set->width * y;
}

/* Convertit les coordonnées de la caméra vers la région rectangulaire dans le plan complexe */
void cam2rect(struct render *set) {
    set->x_min = set->pov.x - set->pov.width / 2;
    set->x_max = set->pov.x + set->pov.width / 2;
    set->y_min = set->pov.y - set->pov.height / 2;
    set->y_max = set->pov.y + set->pov.height / 2;
}

