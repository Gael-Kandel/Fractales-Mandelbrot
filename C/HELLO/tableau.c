/* Travail réalisé sur un PC avec Windows 10 en utilisant WSL */

#include <stdio.h>

void tableau_affiche(int T[], int sz); 
void tableau_init(int T[], int sz, int t0) ;

int main(){
    int entiers[42];
    /* Ancien code
    
    int i = 1; 
    entiers[0] = 0; 
    
    while (i<42)
    {
        entiers[i] = 2*entiers[i-1] + 2; 
        i++; 
    }
    
    for (int i = 0; i < 42; i++)
    {
        printf("élément à l'indice %i (%p) : %i\n", i, (void*) &i, entiers[i]);
    }*/

    tableau_init(entiers, 42, 0);
    tableau_affiche(entiers, 42);
    

    /*Les derniers éléments du tableaux auront des valeurs aberrantes, puisque les valeurs 
    sont trop grandes pour êtres stockées sous ce format */
    return 0;
}

int i; 

void tableau_affiche(int T[], int sz){
    for (i = 0; i < sz; i++)
    {
        printf("élément à l'indice %i (%p) : %i\n", i, (void*) &T[i], T[i]);
    }
}

void tableau_init(int T[], int sz, int t0){
    T[0] = t0;
    for (i = 1; i < sz; i++)
    {
        T[i] = 2*T[i-1] + 2; 
    }
}

/*5. Les adresses mémoire des éléments du tableau sont contigues (ici par blocks de 4 bits)*/

/*Fonctions : 
2. La compilation affiche des erreurs (implicit déclaration).

*/