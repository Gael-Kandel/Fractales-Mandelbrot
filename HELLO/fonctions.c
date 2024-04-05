/* Travail réalisé sur un PC avec Windows 10 en utilisant WSL */

#include <stdio.h>

void passe_valeur(int a){
    printf("L'adresse mémoire de ce paramètre est %p, ce paramètre vaut %d\n", (void*)&a, a) ;
    a += 5; 
}

int retourne_valeur(int a) {
    printf("L'adresse mémoire de ce paramètre est %p, ce paramètre vaut %d\n", (void*)&a, a) ;
    a *= 7;
    return a; 
}

void passe_reference(int * a){
    printf("L'adresse mémoire de ce paramètre est %p, ce paramètre vaut %d\n", (void*)a, *a) ;
    *a += 11;
}

int main(){
    int j = 1 ;
    printf("\n\nLa variable stockée à l'adresse %p contient la valeur %d\n", (void*)&j, j) ;
    printf("\n") ;

    printf("Appel à passe_valeur() ...\n") ;
    passe_valeur(j) ;
    printf("nouvelle valeur de j : %d\n", j) ;
    printf("\n") ;

    printf("Appel à retourne_valeur() ...\n") ;
    j = retourne_valeur(j) ;
    printf("nouvelle valeur de j : %d\n", j) ;
    printf("\n") ;

    printf("Appel à passe_reference() ...\n") ;
    passe_reference(&j) ;
    printf("nouvelle valeur de j : %d\n", j) ;
    printf("\n") ;
    printf("On remarque une propriétés sur la portée des variables, \nle passage par référence permet de modifier la valeur de l'objet globalement contrairement au passage par valeurs\n");
    return 0;
}
/*5. 
On remarque que pour changer la valeur d'un paramètre global via une fonction, il y a deux possibilités: 
- que la fonction renvoie la nouvelle valeur, on peut alors l'affecter au paramètre 
- changer directement la valeur stockée à l'adresse de l'ancienne valeur

6. Un tableau est de type int*, on travaille donc sur les pointeurs (deuxième possibilité de la question 5)
*/