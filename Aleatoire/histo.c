/* Travail réalisé sur un PC avec Windows 10 en utilisant WSL */

/* Auteur : Gaël Kandel
   Date : 25/10/2023

   compilation avec la commande : gcc -W -Wall -ansi -pedantic histo.c -o histo
   exécution avec la commande : ./histo

    Ce programme permet de tester si la fonction rand génère des nombres aléatoires uniformément distribués.
    ---------------------------------------------------------------------------
    On apprend que le générateur de nombres aléatoires  génère des nombres aléatoires uniformément distribués de manière convenable mais pas parfaite.
    En effet, il passe le test du khi2 environ 1 fois sur 2.
    De plus, on constate que la moyenne converge vers l'espérance pour un grand nombre de tirages, conformément à la loi des grands nombres.

    IMPORTANT :

    Réponses aux questions de l'énoncé :

    Etape 2 : 

    Q6 : On remarque une forte modification de l'histogramme à chaque exécution du programme. Cela est du au faible nombre de tirages.

    Q7 : Plus T_MAX est grand, moins l'histogramme (mis à l'échelle) change entre chaque éxécution : la distribution des valeurs aléatoires est uniforme en proportion.
         Cette uniformité semble conforme à ce qu'on doit attendre d'un générateur de nombres aléatoires.
*/

/*
    Estimation de ma note : 19 /20
 */

/*
    Je pense avoir implémenté correctement le programme, en respectant les consignes de l'énoncé, et de manière claire.
    Les nombreux commentaires contribuent également à cette clarté.
    Mon programme compile sans erreur ni warning, et son éxecution est conforme à ce qui est attendu.

*/

#include <stdio.h>
#include <stdlib.h> 
#include <time.h>   
#include <math.h>

/*Ajout des constantes ALEA_MAX et T_MAX*/
#define ALEA_MAX 80
#define T_MAX 1000000


/*Fonction aléatoire renvoyant un flottant dans [a; b[*/
float alea(float a, float b){
    int r = rand();
    return a + (b-a)/(RAND_MAX+1.0)*r;   /*division non entière*/
}

double khi2(int T[], int n, int v); 
double variance(int T[], int sz); 


int main(){

    /*Initialisation du tableau histo (registre des valeurs obtenues)
     et des constantes i (compteur de boucles) et v (stocker temporairement une valeur aléatoire)*/

    int histo[ALEA_MAX];
    int i; 
    int v; 
    int histo_echelle[ALEA_MAX];
    int j; /* second compteur de boucle */
    double moyenne; 

    /*Déclaration de min et max, ainsi que définition de hmax*/
    int min; int max; int hmax; 

    hmax = 20; 
    /*Initialisation du générateur aleatoire*/
    srand(time(NULL));

    /*Initialisation de l'histogramme*/
    for (i = 0; i<ALEA_MAX; i++){
        histo[i] = 0;
    }

    /*Génération de l'histogramme*/
    for (i = 0; i<T_MAX; i++){
        v = (int) alea(0, ALEA_MAX);
        histo[v] += 1;
    }

    /*On cherche min et max, ainsi que la moyenne*/
    min = histo[0]; 
    max = histo[0];
    moyenne = 0; 
    for (i = 0; i<ALEA_MAX; i++){
        if (histo[i]<min){
            min = histo[i]; 
        }
        else if (histo[i]>max){
            max = histo[i]; 
        }
        moyenne += (double) histo[i]; 
    }

    


   /*Affichage de l'histogramme sans mise à l'échelle

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
    printf("\nmin = %i, max = %i \n", min, max); 
        
    */ 

    /*Affichage de l'histogramme avec mise à l'échelle*/

    for (j = hmax; j>-1; j--){
        printf("\n"); 
        for (i = 0; i<ALEA_MAX  ; i++){
            histo_echelle[i] = (int) ((float)  histo[i]/max*hmax  ); 
            if (histo_echelle[i] - j < 0){
                printf(" "); 
            }
            else {
                printf("#"); 
            }
        }  
    }

    /*Affichage du min et du max*/
    printf("\nmin = %i, max = %i \n", min, max); 


    /*Affichage de la moyenne, de la variance, de l'écart-type et de khi2*/
    printf("La moyenne est de %.2f, la variance de %.2f, l'écart-type de %.2f et khi de %.2f \n", moyenne/ALEA_MAX, variance(histo, ALEA_MAX), sqrt(variance(histo, ALEA_MAX)),khi2(histo, T_MAX, ALEA_MAX));
    
    /*Affichage du résultat du test du khi2*/
    if (T_MAX > 10*ALEA_MAX) {
        if (khi2(histo, T_MAX, ALEA_MAX) - v < 2*sqrt(v) && khi2(histo, T_MAX, ALEA_MAX) - v > - 2*sqrt(v)){
            printf("Le générateur passe le test du khi2\n"); 
        }
        else {
            printf("Le générateur ne passe pas le test du khi2\n");
        }
    }
    else {
        printf("T_MAX n'est pas supérieur à 10 * ALEA_Max, les hypothèses du test du khi2 ne sont pas vérifiées\n");
    }

    return 0; 
    
}


/*Variance de la population d’entiers contenus dans T et de taille sz*/
double khi2(int T[], int n, int v){
    long int temp; int i;    /*usage de long int pour eviter un dépassement quand T_MAX = 1 000 000*/
    temp = 0; 
    for (i = 0; i<v; i++){
        temp += T[i]*T[i];  
    }
    return (double) v/n*temp - n ; 

}

double variance(int T[], int sz){
    double m; double s; int j; int i; double m_prev; 
    m = 0; s = 0; j = 0; 

    for (i = 0; i<sz; i++){
        j ++;
        m_prev = m; 
        m += (double) (T[i]-m)/(j);; 
        s += (double) (T[i]-m)*(T[i]-m_prev); 
    }

    return s/(j-1); 
}