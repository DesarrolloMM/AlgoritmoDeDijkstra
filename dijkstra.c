#include "dijkstra.h"
#include<stdio.h>
#include<stdlib.h>

/*
@brief Ejecuta el algoritmo
*/
int dijkstra(const Red *red, int origen, ResultadoDijkstra *res){
    
    int i,j,vecino;

    if(red == NULL || res == NULL || origen < 0 ||origen >= red->numRouters){
        return -1;
    }

    for (i = 0; i < red->numRouters; i++)
    {
        res->distancia[i] = INF;
        res->predecesor[i] = -1;
        res->visitado[i]= 0;
    }
    res->distancia[origen] = 0;

    for (i = 0; i < red->numRouters; i++)
    {
        int distanciaMinima = INF;
        int indiceNodo = -1;

        for (j = 0; j < red->numRouters; j++)
        {
            if(res->visitado[j] == 0 && res->distancia[j] < distanciaMinima){
                distanciaMinima = res->distancia[j];
                indiceNodo = j;
            }

        }
        
        if(indiceNodo == -1 || distanciaMinima == INF){
            break;
        }

        res->visitado[indiceNodo] = 1;
        
        for (vecino = 0; vecino < red->numRouters; vecino++)
        {
            if(res->visitado[vecino] == 0 && red->matriz[indiceNodo][vecino] != INF){
                {
                    if(res->distancia[indiceNodo] != INF){
                        int nuevaDistancia = res->distancia[indiceNodo] + red->matriz[indiceNodo][vecino];

                        if(nuevaDistancia < res->distancia[vecino]){
                            res->distancia[vecino] = nuevaDistancia;
                            res->predecesor[vecino] = indiceNodo;
                        }
                    }
                }
            }
        
        }
        
    }

    return 0;
}