#include "grafo.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/*
@brief Inicializamos la red con una cantidad de routers y sin enlaces directos
*/

int inicializarRed(Red *red, int cantRouter){
    if(red == NULL || cantRouter < 0 || cantRouter > MAX_ROUTERS){
        return -1;
    }
    
    red->numRouters=cantRouter;

    for (int i = 0; i < cantRouter; i++){
        snprintf(red->nombres[i], MAX_NOMBRE, "R%d", i+1);

        for (int j = 0; j < MAX_ROUTERS; j++){
            if( i == j){
                red->matriz[i][j] = 0;
            }
            else{
                red->matriz[i][j] = INF;
            }
        }

    }
    return EXIT_SUCCESS;
}

int main (){
    Red red;
    
    printf ("Testing inicialización con 3 routers\n");

    if(inicializarRed(&red, 3) == EXIT_SUCCESS){
    
        printf(" -&gt; OK. Routers inicializados: %d\n", red.numRouters); 
    
        int filas = 3;
        int columnas = 3;
        for (int i = 0; i < filas; i++) {
            for (int j = 0; j < columnas; j++) {
                printf("%d ", red.matriz[i][j]); 
            }
            printf("\n");
        }
    }
    
    else { 
        printf(" -&gt; ERROR en inicializarRed\\n"); 
    }
    
    return EXIT_SUCCESS;
}