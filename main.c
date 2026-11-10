#include <stdio.h>
#include <string.h>
#include "grafo.h"
#include "dijkstra.h"
#include "enrutamiento.h"
#include "tests.h"

static void limpiarBuffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) { /* descartar */ }
}

static int leerEntero(const char *msg, int *out) {
    printf("%s", msg);
    if (scanf("%d", out) != 1) { limpiarBuffer(); return -1; }
    limpiarBuffer();
    return 0;
}

static void mostrarMenu(void) {
    printf("\n===== SIMULADOR DE ENRUTAMIENTO - DIJKSTRA =====\n");
    printf("1. Cargar topologia desde archivo\n");
    printf("2. Cargar topologia de ejemplo\n");
    printf("3. Agregar enlace manualmente\n");
    printf("4. Mostrar topologia actual\n");
    printf("5. Calcular tabla de rutas desde un router\n");
    printf("6. Simular envio de paquete\n");
    printf("7. Ejecutar pruebas automaticas\n");
    printf("0. Salir\n");
    printf("Opcion: ");
}

static void opcionCargarArchivo(Red *red) {
    char ruta[MAX_LINEA];
    printf("Ruta del archivo: ");
    if (scanf("%255s", ruta) != 1) { limpiarBuffer(); return; }
    limpiarBuffer();

    if (cargarTopologiaDesdeArchivo(red, ruta) == 0)
        printf("Topologia cargada desde '%s'.\n", ruta);
    else
        printf("No se pudo cargar la topologia.\n");
}

static void opcionAgregarEnlace(Red *red) {
    if (red->numRouters == 0) { printf("Primero cargue una topologia.\n"); return; }
    int i, j, c;
    if (leerEntero("Indice router 1: ", &i) != 0) return;
    if (leerEntero("Indice router 2: ", &j) != 0) return;
    if (leerEntero("Costo: ", &c) != 0) return;

    if (agregarEnlace(red, i, j, c) == 0)
        printf("Enlace %d-%d agregado (costo %d).\n", i, j, c);
    else
        printf("Error: indices invalidos, costo <= 0 o enlace ya existente.\n");
}

static void opcionTablaRutas(const Red *red) {
    if (red->numRouters == 0) { printf("Primero cargue una topologia.\n"); return; }
    int origen;
    if (leerEntero("Indice del router origen: ", &origen) != 0) return;
    if (origen < 0 || origen >= red->numRouters) {
        printf("Indice fuera de rango (0..%d).\n", red->numRouters - 1);
        return;
    }

    ResultadoDijkstra res;
    if (dijkstra(red, origen, &res) != 0) { printf("Error en Dijkstra.\n"); return; }

    EntradaRuta tabla[MAX_ROUTERS];
    construirTablaRutas(red, origen, &res, tabla);

    printf("\nTabla de rutas desde %s:\n", red->nombres[origen]);
    mostrarTablaRutas(red, tabla);
}

static void opcionSimularPaquete(const Red *red) {
    if (red->numRouters == 0) { printf("Primero cargue una topologia.\n"); return; }
    int o, d;
    if (leerEntero("Router origen: ", &o) != 0) return;
    if (leerEntero("Router destino: ", &d) != 0) return;
    if (o < 0 || o >= red->numRouters || d < 0 || d >= red->numRouters) {
        printf("Indices fuera de rango.\n");
        return;
    }

    ResultadoDijkstra res;
    dijkstra(red, o, &res);
    simularEnvioPaquete(red, o, d, &res);
}

int main(int argc, char *argv[]) {
    /* Modo no interactivo: solo pruebas (útil para CI / make test) */
    if (argc > 1 && strcmp(argv[1], "--test") == 0)
        return ejecutarTodasLasPruebas();

    Red red;
    inicializarRed(&red, 0);

    int opcion;
    do {
        mostrarMenu();
        if (scanf("%d", &opcion) != 1) { limpiarBuffer(); continue; }
        limpiarBuffer();

        switch (opcion) {
            case 1: opcionCargarArchivo(&red); break;
            case 2:
                cargarTopologiaEjemplo(&red);
                printf("Topologia de ejemplo cargada (5 routers).\n");
                break;
            case 3: opcionAgregarEnlace(&red); break;
            case 4: mostrarTopologia(&red);    break;
            case 5: opcionTablaRutas(&red);    break;
            case 6: opcionSimularPaquete(&red); break;
            case 7: {
                int fallos = ejecutarTodasLasPruebas();
                printf("Pruebas fallidas: %d\n", fallos);
                break;
            }
            case 0: printf("Saliendo...\n"); break;
            default: printf("Opcion invalida.\n"); break;
        }
    } while (opcion != 0);

    liberarRed(&red);
    return 0;
}