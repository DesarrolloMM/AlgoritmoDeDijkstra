#ifndef DIJKSTRA_H
#define DIJKSTRA_H

#include "grafo.h"

typedef struct {
    int distancia[MAX_ROUTERS];
    int predecesor[MAX_ROUTERS];
    int visitado[MAX_ROUTERS];
} ResultadoDijkstra;

int dijkstra(const Red *red, int origen, ResultadoDijkstra *res);

#endif /* DIJKSTRA_H */