#include "enrutamiento.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


/*
@brief Busca el siguiente salto
*/

static int obtenerSiguienteSalto(const ResultadoDijkstra *res, int origen, int destino){

    int nodoActual = destino;

    if( res == NULL || origen == destino ||res->distancia[destino] == INF || res->predecesor[destino] == -1){
        return -1;
    }

    while (res->predecesor[nodoActual] != origen)
    {
        nodoActual = res->predecesor[nodoActual];

    }
    
    return nodoActual;
}

/*
@brief Genera la tabla con next hop por destino
*/
int  construirTablaRutas(const Red *red, int origen, const ResultadoDijkstra *res, EntradaRuta *tabla){

    if(red == NULL || res == NULL || tabla == NULL || origen < 0 || origen >= red->numRouters){
        return -1;
    }

    for (int i = 0; i < red->numRouters; i++)
    {
        tabla[i].costo = res->distancia[i];
        tabla[i].destino = i;
        tabla[i].siguienteSalto = obtenerSiguienteSalto(res, origen, i);
    }

    return 0;
}


/*
@brief Imprime la tabla formateada
*/
void mostrarTablaRutas(const Red *red, const EntradaRuta *tabla){
    
    if(red == NULL || tabla == NULL){
        return;
    }

    printf("Tabla de enrutamiento\n");
    printf ("%-12s %-10s %-15s\n", "Destino", "Costo", "Siguiente Salto");
    
    for (int i = 0; i < red->numRouters ; i++)
    {
        if(tabla[i].costo == INF){
            printf("%-12s %-10s %-15s\n", red->nombres[i], "INF", "Sin ruta");
        }
        else if(tabla[i].costo == 0){
            printf("%-12s %-10d %-15s\n" ,red->nombres[i], 0, "Local");
        }
        else{
            int siguiente = tabla[i].siguienteSalto;
            const char *nombreSiguiente = (siguiente >= 0 && siguiente < red->numRouters) ? red->nombres[siguiente] : "Desconocido";
            printf("%-12s %-10d %-15s\n", red->nombres[i], tabla[i].costo, nombreSiguiente);
        }
    }
    printf("\n");
}

/*
@brief Muestra la ruta completa de un paquete
*/
int  simularEnvioPaquete(const Red *red, int origen, int destino, const ResultadoDijkstra *res){
    if(red == NULL || res == NULL || destino < 0 || destino >= red->numRouters || origen < 0 || origen >= red->numRouters){
        return -1;
    }
    
    if(res->distancia[destino] == INF){
        printf("No existe ruta desde %s hacia %s.\n", red->nombres[origen], red->nombres[destino]);
        return -1;
    }

    int camino[MAX_ROUTERS];
    int pasos = 0;
    int actual = destino;

    camino[pasos++] = actual;
    while (actual != origen){
        actual = res->predecesor[actual];
        
        if(actual == -1){
            printf("Error: Ruta incompleta\n");
            return -1;
        }
        camino[pasos++] = actual;
    }

    printf("Simulación de paquete: Ruta de %s a %s | Costo total: %d\n", red->nombres[origen], red->nombres[destino], res->distancia[destino]);

    for (int i = pasos -1 ; i >= 0; i--)
    {
        int idxRouter = camino[i];
        if(i == 0){
            printf("%s", red->nombres[idxRouter]);
        }
        else{
            printf("%s ->", red->nombres[idxRouter]);
        }
    } 
    printf("\n");
    return 0;
}