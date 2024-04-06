#include <stdio.h>
#include <time.h>
#include <stdlib.h>


int i; 
float tab[100]; 


float alea(float a, float b){
    int r = rand();
    return a + (b-a)/RAND_MAX*r;
}



void affiche_tableau(float T[], int n){
    for (i=0; i < n; i++)
    {
        if (i % 5 == 0)
        {
            printf("\n");
        }
        printf("%5.2f ", T[i]);
    }
    printf("\n");
}


int main(){
    srand(time(NULL));

    
    for (i =0; i<100; i++){
        tab[i] = alea(-1, 1);
    }

    affiche_tableau(tab, 100); 
    return 0;   
}