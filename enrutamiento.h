#ifndef ENRUTAMIENTO_H
#define ENRUTAMIENTO_H

#include "dijkstra.h"

typedef struct {
    int destino;
    int costo;
    int siguienteSalto;   /* -1 si inalcanzable */
} EntradaRuta;

int  construirTablaRutas(const Red *red, int origen,
                         const ResultadoDijkstra *res,
                         EntradaRuta *tabla);
void mostrarTablaRutas(const Red *red, const EntradaRuta *tabla);
int  simularEnvioPaquete(const Red *red, int origen, int destino,
                         const ResultadoDijkstra *res);

#endif /* ENRUTAMIENTO_H */