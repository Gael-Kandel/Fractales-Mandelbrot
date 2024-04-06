/* Travail réalisé sur un PC avec Windows 10 en utilisant WSL */

/*Estimation de la note + justification*/

#include <stdio.h>
#include <stdlib.h> 
#include <time.h>   


/*Ajout des constantes ALEA_MAX et T_MAX*/
int ALEA_MAX = 80; int T_MAX = 1000;
/*Déclaration de min et max, ainsi que définition de hmax*/
int min; int max; int hmax = 20; 


/*Fonction aléatoire renvoyant un flottant dans [a; b[*/
float alea(float a, float b){
    int r = rand();
    return a + (b-a)/RAND_MAX*r;
}



int main(){

    /*Initialisation du tableau histo (registre des valeurs obtenues)
     et des constantes i (compteur de boucles) et v (stocker temporairement une valeur aléatoire)*/

    /*On remplit 80 manuellement pour éviter un VLA*/
    int histo[80];
    int i; 
    int v; 

    int j; /* second compteur de boucle */

    srand(time(NULL));

    for (i = 0; i<ALEA_MAX; i++){
        histo[i] = 0;
    }

    /*Génération de l'histogramme*/
    for (i = 0; i<T_MAX; i++){
        v = (int) alea(0, ALEA_MAX);
        histo[v] += 1;
    }

    /*On cherche min et max*/
    min = histo[0]; 
    max = histo[0];
    for (i = 1; i<ALEA_MAX; i++){
        if (histo[i]<min){
            min = histo[i]; 
        }
        else if (histo[i]>max){
            max = histo[i]; 
        }
    }

    /*Affichage de l'histogramme (c'est dur :)*/

    for (j = max; j>-1; j--){
        printf("\n"); 
        for (i = 0; i<ALEA_MAX; i++){
            if (histo[i] - j < 0){
                printf(" "); 
            }
            else {
                printf("#"); 
            }
        }
    }
    printf("\n min = %i, max = %i \n", min, max); 
        
        
        /*↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓*/
    
        
    /*->*/    /*utilise hmax batard*/  /*<-*/



    
    return 0; 
    
}