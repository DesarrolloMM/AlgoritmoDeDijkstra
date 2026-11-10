#include <stdio.h>
#include "tests.h"
#include "dijkstra.h"
#include "enrutamiento.h"

static int g_total  = 0;
static int g_fallos = 0;

static void reportar(const char *nombre, int ok, const char *detalle) {
    g_total++;
    if (ok) {
        printf("  [PASS] %-55s\n", nombre);
    } else {
        g_fallos++;
        printf("  [FAIL] %-55s  -> %s\n", nombre, detalle ? detalle : "");
    }
}

static int eq(int a, int b) { return a == b; }

/* ---------------- Caso 1: red lineal ---------------- */
static void test_red_lineal(void) {
    printf("\n--- Caso 1: Red lineal ---\n");
    Red red;
    inicializarRed(&red, 4);
    agregarEnlace(&red, 0, 1, 3);
    agregarEnlace(&red, 1, 2, 2);
    agregarEnlace(&red, 2, 3, 4);

    ResultadoDijkstra res;
    int ok = (dijkstra(&red, 0, &res) == 0);

    reportar("Dijkstra retorna 0", ok, "retorno != 0");
    reportar("R1->R1 = 0", eq(res.distancia[0], 0), "esperado 0");
    reportar("R1->R2 = 3", eq(res.distancia[1], 3), "esperado 3");
    reportar("R1->R3 = 5", eq(res.distancia[2], 5), "esperado 5");
    reportar("R1->R4 = 9", eq(res.distancia[3], 9), "esperado 9");
    reportar("Pred R2 = R1", eq(res.predecesor[1], 0), "esperado 0");
    reportar("Pred R3 = R2", eq(res.predecesor[2], 1), "esperado 1");
    reportar("Pred R4 = R3", eq(res.predecesor[3], 2), "esperado 2");
}

/* ---------------- Caso 2: múltiples caminos ---------------- */
static void test_multiples_caminos(void) {
    printf("\n--- Caso 2: Múltiples caminos ---\n");
    Red red;
    inicializarRed(&red, 3);
    agregarEnlace(&red, 0, 1, 2);
    agregarEnlace(&red, 0, 2, 5);
    agregarEnlace(&red, 1, 2, 1);

    ResultadoDijkstra res;
    dijkstra(&red, 0, &res);

    reportar("R1->R3 = 3 (vía R2)", eq(res.distancia[2], 3), "esperado 3");
    reportar("Pred R3 = R2", eq(res.predecesor[2], 1), "esperado 1");

    EntradaRuta tabla[MAX_ROUTERS];
    construirTablaRutas(&red, 0, &res, tabla);
    reportar("Next hop R3 = R2", eq(tabla[2].siguienteSalto, 1), "esperado 1");
    reportar("Costo R3 en tabla = 3", eq(tabla[2].costo, 3), "esperado 3");
}

/* ---------------- Caso 3: red desconectada ---------------- */
static void test_red_desconectada(void) {
    printf("\n--- Caso 3: Red desconectada ---\n");
    Red red;
    inicializarRed(&red, 5);
    agregarEnlace(&red, 0, 1, 1);
    agregarEnlace(&red, 1, 2, 1);
    agregarEnlace(&red, 3, 4, 1);

    ResultadoDijkstra res;
    dijkstra(&red, 0, &res);

    reportar("R1->R4 = INF", eq(res.distancia[3], INF), "esperado INF");
    reportar("R1->R5 = INF", eq(res.distancia[4], INF), "esperado INF");
    reportar("Pred R4 = -1", eq(res.predecesor[3], -1), "esperado -1");
    reportar("simular R1->R5 = -1",
             eq(simularEnvioPaquete(&red, 0, 4, &res), -1), "esperado -1");
}

/* ---------------- Caso 4: origen == destino ---------------- */
static void test_origen_igual_destino(void) {
    printf("\n--- Caso 4: Origen == Destino ---\n");
    Red red;
    inicializarRed(&red, 3);
    agregarEnlace(&red, 0, 1, 7);
    agregarEnlace(&red, 1, 2, 7);

    ResultadoDijkstra res;
    dijkstra(&red, 1, &res);
    reportar("R2->R2 = 0", eq(res.distancia[1], 0), "esperado 0");
    reportar("Pred R2 = -1", eq(res.predecesor[1], -1), "esperado -1");
    reportar("simular R2->R2 = 0",
             eq(simularEnvioPaquete(&red, 1, 1, &res), 0), "esperado 0");
}

/* ---------------- Caso 5: red con ciclo ---------------- */
static void test_red_con_ciclo(void) {
    printf("\n--- Caso 5: Red con ciclo ---\n");
    Red red;
    inicializarRed(&red, 4);
    agregarEnlace(&red, 0, 1, 1);
    agregarEnlace(&red, 1, 3, 1);
    agregarEnlace(&red, 3, 2, 1);
    agregarEnlace(&red, 2, 0, 1);

    ResultadoDijkstra res;
    dijkstra(&red, 0, &res);
    reportar("R1->R2 = 1", eq(res.distancia[1], 1), "esperado 1");
    reportar("R1->R3 = 1", eq(res.distancia[2], 1), "esperado 1");
    reportar("R1->R4 = 2", eq(res.distancia[3], 2), "esperado 2");
    reportar("simular R1->R4 OK",
             eq(simularEnvioPaquete(&red, 0, 3, &res), 0), "esperado 0");
}

/* ---------------- Caso 6: nodo inexistente / validaciones ---------------- */
static void test_nodo_inexistente(void) {
    printf("\n--- Caso 6: Nodo inexistente y validaciones ---\n");
    Red red;
    inicializarRed(&red, 3);
    agregarEnlace(&red, 0, 1, 1);

    ResultadoDijkstra res;
    reportar("dijkstra(origen=-1) = -1", eq(dijkstra(&red, -1, &res), -1), "esperado -1");
    reportar("dijkstra(origen=99) = -1", eq(dijkstra(&red, 99, &res), -1), "esperado -1");
    reportar("dijkstra(res=NULL) = -1",  eq(dijkstra(&red, 0, NULL), -1), "esperado -1");

    reportar("agregarEnlace(i=-1) = -1",
             eq(agregarEnlace(&red, -1, 1, 5), -1), "esperado -1");
    reportar("agregarEnlace(costo=0) = -1",
             eq(agregarEnlace(&red, 0, 2, 0), -1), "esperado -1");
    reportar("agregarEnlace(costo<0) = -1",
             eq(agregarEnlace(&red, 0, 2, -3), -1), "esperado -1");
    reportar("agregarEnlace(i==j) = -1",
             eq(agregarEnlace(&red, 1, 1, 5), -1), "esperado -1");
    reportar("agregarEnlace duplicado = -1",
             eq(agregarEnlace(&red, 0, 1, 9), -1), "esperado -1");
}

/* ---------------- Extra 1: topología del enunciado ---------------- */
static void test_topologia_enunciado(void) {
    printf("\n--- Extra 1: Topología del enunciado ---\n");
    Red red;
    cargarTopologiaEjemplo(&red);

    ResultadoDijkstra res;
    dijkstra(&red, 0, &res);
    reportar("R1->R2 = 2", eq(res.distancia[1], 2), "esperado 2");
    reportar("R1->R3 = 3", eq(res.distancia[2], 3), "esperado 3");
    reportar("R1->R4 = 4", eq(res.distancia[3], 4), "esperado 4");
    reportar("R1->R5 = 7", eq(res.distancia[4], 7), "esperado 7");

    EntradaRuta tabla[MAX_ROUTERS];
    construirTablaRutas(&red, 0, &res, tabla);
    reportar("Next hop R2 = R2", eq(tabla[1].siguienteSalto, 1), "esperado 1");
    reportar("Next hop R3 = R2", eq(tabla[2].siguienteSalto, 1), "esperado 1");
    reportar("Next hop R4 = R2", eq(tabla[3].siguienteSalto, 1), "esperado 1");
    reportar("Next hop R5 = R2", eq(tabla[4].siguienteSalto, 1), "esperado 1");
}

/* ---------------- Extra 2: simetría no dirigida ---------------- */
static void test_simetria(void) {
    printf("\n--- Extra 2: Simetría ---\n");
    Red red;
    inicializarRed(&red, 4);
    agregarEnlace(&red, 0, 1, 4);
    agregarEnlace(&red, 1, 2, 3);
    agregarEnlace(&red, 2, 3, 2);
    agregarEnlace(&red, 0, 3, 20);

    ResultadoDijkstra a, b;
    dijkstra(&red, 0, &a);
    dijkstra(&red, 3, &b);

    reportar("R1->R4 == R4->R1",
             eq(a.distancia[3], b.distancia[0]), "distintos");
    reportar("R1->R3 == R3->R1",
             eq(a.distancia[2], b.distancia[2]), "distintos");
    reportar("R1->R4 = 9", eq(a.distancia[3], 9), "esperado 9");
}

/* ---------------- Extra 3: eliminar enlace ---------------- */
static void test_eliminar_enlace(void) {
    printf("\n--- Extra 3: Eliminación de enlace ---\n");
    Red red;
    inicializarRed(&red, 3);
    agregarEnlace(&red, 0, 1, 1);
    agregarEnlace(&red, 1, 2, 1);
    agregarEnlace(&red, 0, 2, 10);

    ResultadoDijkstra res;
    dijkstra(&red, 0, &res);
    reportar("Antes R1->R3 = 2", eq(res.distancia[2], 2), "esperado 2");

    reportar("eliminar R1-R2 = 0",
             eq(eliminarEnlace(&red, 0, 1), 0), "esperado 0");

    dijkstra(&red, 0, &res);
    reportar("Después R1->R3 = 10", eq(res.distancia[2], 10), "esperado 10");
    reportar("eliminar inexistente = -1",
             eq(eliminarEnlace(&red, 0, 1), -1), "esperado -1");
}

/* ---------------- Punto de entrada ---------------- */
int ejecutarTodasLasPruebas(void) {
    g_total = 0; g_fallos = 0;

    printf("\n============================================\n");
    printf("      BATERÍA DE PRUEBAS AUTOMÁTICAS\n");
    printf("============================================\n");

    test_red_lineal();
    test_multiples_caminos();
    test_red_desconectada();
    test_origen_igual_destino();
    test_red_con_ciclo();
    test_nodo_inexistente();
    test_topologia_enunciado();
    test_simetria();
    test_eliminar_enlace();

    printf("\n============================================\n");
    printf("  RESUMEN: %d pruebas, %d fallos\n", g_total, g_fallos);
    printf("  RESULTADO: %s\n",
           (g_fallos == 0) ? "TODAS LAS PRUEBAS PASARON" : "HAY FALLOS");
    printf("============================================\n\n");
    return g_fallos;
}