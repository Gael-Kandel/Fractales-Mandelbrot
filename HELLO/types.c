/* Travail réalisé sur un PC avec Windows 10 en utilisant WSL */


#include <stdio.h>

int main(void)
{
/*Déclaration des variables*/

    char c = 'a' ; /*Initialisé pour éviter une erreur de compilation*/
    short s;
    int i;
    long l;
    float f;
    double d;
    char str[10] = "warning: c"; 

/*Affichage de l'adresse, la valeur et la taille mémoire de l'objet */

    printf("le caractère stocké à l'adresse %p vaut '%c' et occupe %lu octet(s) en mémoire\n",
            (void *)&c,  c, sizeof(c)) ;

    printf("l'entier court stocké à l'adresse %p vaut %d  et occupe %lu octet(s) en mémoire\n",
            (void *)&s, s, sizeof(s)) ;

    printf("l'entier stocké à l'adresse %p vaut %d  et occupe %lu octet(s) en mémoire\n",
            (void *)&i, i, sizeof(i)) ;

    printf("l'entier long stocké à l'adresse %p vaut %ld  et occupe %lu octet(s) en mémoire\n",
            (void *)&l, l, sizeof(l)) ;

    printf("le nombre flottant simple précision stocké à l'adresse %p vaut %f  et occupe %lu octet(s) en mémoire\n",
            (void *)&f, f, sizeof(f)) ;

    printf("le nombre flottant double précision stocké à l'adresse %p vaut %f  et occupe %lu octet(s) en mémoire\n",
            (void *)&d, d, sizeof(d)) ;

    printf("la chaine de caractères stockée à l'adresse %p contient \"%s\"  et occupe %lu octet(s) en mémoire\n",
            (void *)str, str, sizeof(str)) ;


    return 0 ;
}


/*
Je n'ai pas les mêmes valeurs que mes camarades. Elles changent car on ne les a pas initialisées, elles prennent dès lors la valeur stockée précédement sur les bits occupés.
*/