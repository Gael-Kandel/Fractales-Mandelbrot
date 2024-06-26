/* Auteur : Gaël Kandel
   Date : 30/11/2023

   compilation avec la commande : gcc -W -Wall -ansi -pedantic mandel.c -o mandel
   exécution avec la commande : ./mandel
   
   L'éxecution donne un segmentation fault, je joins ce fichier pour que vous 
   voyez où j'en suis.
   Pour le code jusqu'à la tâche 6 inclue se référer au code mandel_v5 fournit.
   */



#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define DEFAULT_WIDTH 1000
#define DEFAULT_HEIGHT 1000
#define STR_MAX 256

/* Structure pour représenter les propriétés de la caméra */
struct camera {
    double x; /* Coordonnée x de la caméra */
    double y; /* Coordonnée y de la caméra */
    double width; /* Largeur de la fenêtre de visualisation */
    double height; /* Hauteur de la fenêtre de visualisation */
};

/* Structure pour stocker les paramètres de rendu et les résultats */
struct render {
    float x_min; /* Coordonnée x minimum dans le plan complexe */
    float y_min; /* Coordonnée y minimum dans le plan complexe */
    float x_max; /* Coordonnée x maximum dans le plan complexe */
    float y_max; /* Coordonnée y maximum dans le plan complexe */
    int width; /* Largeur de l'image générée */
    int height; /* Hauteur de l'image générée */
    int max_iter; /* Nombre maximal d'itérations pour le calcul fractal */
    int radius; /* Rayon pour le test de convergence */
    FILE *fout; /* Pointeur de fichier pour sauvegarder l'image */
    int *img; /* Tableau pour stocker les résultats de l'image */
    char basename[STR_MAX]; /* Nom de base du fichier de sortie */
    struct camera pov; /* Propriétés de la caméra */
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
void cam2rect(struct render *set, struct camera *pov);

/* Fonction principale où le processus de rendu est orchestré */
int main(int argc, char *argv[]) {
    struct render set; /* Structure pour stocker les paramètres de rendu et les résultats */
    struct camera pov; /* Structure pour représenter les propriétés de la caméra */

    /* Mappage de la caméra à la région rectangulaire dans le plan complexe */
    cam2rect(&set, &pov);

    /* Initialisation des paramètres de rendu */
    render_init(&set, argc, argv);

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
    
    if (set->img == NULL) {
        fprintf(stderr, "Erreur d'allocation mémoire pour set->img\n");
        exit(EXIT_FAILURE);
    }

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
    set->img = (int*) malloc(set->width * set->height * sizeof(int));
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
    int iter;
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
            /* Stocker le résultat dans le tableau d'image */
            
            set->img[m2v(set, P_x, P_y)] = iter;
        }
        printf("Ligne en cours de calcul : %d\r", P_y);
    }
}

/* Récupère la couleur d'un pixel dans l'image */
void get_pixel_color(struct render *set, int P_x, int P_y, int *r, int *g, int *b) {
    int temp;
    temp = set->img[m2v(set, P_x, P_y)];

    /* La couleur du pixel est définie en nuances de gris pour une image en noir et blanc */
    *r = temp;
    *g = temp;
    *b = temp;
}

/* Enregistre l'image générée dans un fichier PPM */
void save_image(struct render *set) {
    int P_x; int P_y;
    char filename[STR_MAX + 4]; /* 4 pour l'extension ".ppm" */
    int nb_colors;
    int temp;

    /* Initialisations pour le format PPM */
    nb_colors = 255;
    strcpy(filename, set->basename);
    strcat(filename, ".ppm");

    /* Ouverture du fichier en mode écriture */
    set->fout = fopen(filename, "w");
    fprintf(set->fout, "P3\n%d %d\n%d\n", set->width, set->height, nb_colors); /* Entête du fichier PPM */
    fprintf(set->fout, "\n#Nombre d'itérations max = %d, Zone du plan complexe : x ∈ [%f; %f], y ∈ [%f; %f] \n", set->max_iter, set->x_min, set->x_max, set->y_min, set->y_max); /* Commentaire */
    for (P_y = 0; P_y < set->height; P_y++) {
        for (P_x = 0; P_x < set->width; P_x++) {
            /* Conversion du résultat en niveau de gris */
            temp = (set->img[m2v(set, P_x, P_y)] / set->max_iter) * nb_colors;

            /* Écriture dans le fichier PPM */
            fprintf(set->fout, "%i %i %i\n", temp, temp, temp);
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
void cam2rect(struct render *set, struct camera *pov) {
    set->x_min = pov->x - pov->width / 2;
    set->x_max = pov->x + pov->width / 2;
    set->y_min = pov->y - pov->height / 2;
    set->y_max = pov->y + pov->height / 2;
}
