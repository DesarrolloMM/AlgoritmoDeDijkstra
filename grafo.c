#include "grafo.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/*
@brief Inicializa la red con una cantidad N de routers y sin enlaces.
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
    return 0;
}

/*
@brief Asigna un nombnre legible a un router
*/

int setNombreRouter (Red *red, int idx, const char *nombre){
    if(red == NULL || nombre == NULL || idx < 0 || idx >= red->numRouters){
        return -1;
    }
    else{
        strncpy(red->nombres[idx], nombre, MAX_NOMBRE -1);
    }
    return 0;
}

int main (){
    Red red;
    int cantRouters = 3;
    int filas = cantRouters;
    int columnas = cantRouters;
    
    printf ("Testing inicialización con 3 routers\n");

    if(inicializarRed(&red, cantRouters) == 0){
    
        printf(" -&gt; OK. Routers inicializados: %d\n", red.numRouters); 

       

        for (int i = 0; i < filas; i++) {
            for (int j = 0; j < columnas; j++) {
                printf("%d ", red.matriz[i][j]); 
            }
            printf("\n");
        }

            for (int i = 0; i < filas; i++) {
                for (int j = 0; j < columnas; j++) {
                    printf("%c", red.nombres[i][j]); 
                }
            printf("\n");
        }    
    }
    
    else { 
        printf(" -&gt; ERROR en inicializarRed\n"); 
    }
    

    printf("\n[Test 2] Asignando nombres personalizados...\n");
     setNombreRouter(&red, 0, "Router_A");
     setNombreRouter(&red, 1, "Router_B");
     setNombreRouter(&red, 2, "Router_C");

      for (int i = 0; i < 3; i++) {
        printf("%s", red.nombres[i]);
        printf("\n");
      }
    return EXIT_SUCCESS;
}