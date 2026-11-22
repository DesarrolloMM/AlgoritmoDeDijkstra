/*1. INCLUSIONES DE CABECERAS
    Cabeceras estándar para I/O y cadenas, junto con los módulos del proyecto.
  */
#include <stdio.h>
#include <string.h>
#include "grafo.h"
#include "dijkstra.h"
#include "enrutamiento.h"
#include "tests.h"

/* Ancho fijo para mantener bordes y títulos alineados en consola */
#define ANCHO_BANNER 54

/*
    2. FUNCIONES AUXILIARES DE FORMATO (INICIO / FIN / SEPARADORES)
    Encargan la presentación visual en terminal sin mezclar lógica de red.
*/

/* Imprime una línea horizontal repetida del ancho del banner */
static void imprimirSeparador(char caracter) {
    for (int i = 0; i < ANCHO_BANNER; i++) {
        putchar(caracter);
    }
    putchar('\n');
}

/* Centra un texto dentro del ancho definido para el banner */
static void imprimirCentrado(const char *texto) {
    int longitud = (int)strlen(texto);
    int margenIzq = (longitud < ANCHO_BANNER) ? (ANCHO_BANNER - longitud) / 2 : 0;

    for (int i = 0; i < margenIzq; i++) {
        putchar(' ');
    }
    printf("%s\n", texto);
}

/* Marca visual explícita del PRINCIPIO de la simulación */
static void mostrarInicioSimulacion(void) {
    putchar('\n');
    imprimirSeparador('=');
    imprimirCentrado("SIMULADOR DE ENRUTAMIENTO - DIJKSTRA");
    imprimirCentrado("PRINCIPIO DE LA SIMULACION");
    imprimirSeparador('=');
}

/* Marca visual explícita del FIN de la simulación*/
static void mostrarFinSimulacion(void) {
    putchar('\n');
    imprimirSeparador('=');
    imprimirCentrado("FIN DE LA SIMULACION");
    imprimirCentrado("Recursos liberados correctamente.");
    imprimirSeparador('=');
    putchar('\n');
}

/* 3. FUNCIONES AUXILIARES DE ENTRADA POR TECLADO
Evitan que entradas no numéricas o saltos de línea residuales rompan el CLI.
 */

/* Limpia caracteres restantes en stdin hasta el fin de línea o EOF */
static void limpiarBuffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {
        /* Descartar caracteres sobrantes */
    }
}

/* Lee un entero validando que scanf haya convertido un número */
static int leerEntero(const char *msg, int *out) {
    printf("%s", msg);
    if (scanf("%d", out) != 1) {
        limpiarBuffer();
        printf("Entrada invalida: debe ingresar un numero entero.\n");
        return -1;
    }
    limpiarBuffer();
    return 0;
}

/* Lee una línea de texto (por ejemplo, la ruta del archivo de topología) */
static void leerCadena(const char *msg, char *destino, size_t tam) {
    printf("%s", msg);
    if (fgets(destino, (int)tam, stdin) != NULL) {
        size_t len = strlen(destino);
        if (len > 0 && destino[len - 1] == '\n') {
            destino[len - 1] = '\0';
        } else {
            limpiarBuffer();
        }
    } else {
        destino[0] = '\0';
        limpiarBuffer();
    }
}

/*
    4. MENÚ PRINCIPAL (NUMERACIÓN DEL ANEXO 5)
    Mantiene el orden exacto de opciones 1..7 y 0 del proyecto.
*/
static void mostrarMenu(void) {
    putchar('\n');
    imprimirSeparador('-');
    imprimirCentrado("MENU DE OPCIONES");
    imprimirSeparador('-');
    printf("  1. Cargar topologia desde archivo\n");
    printf("  2. Cargar topologia de ejemplo\n");
    printf("  3. Agregar enlace manualmente\n");
    printf("  4. Mostrar topologia actual\n");
    printf("  5. Calcular tabla de rutas desde un router\n");
    printf("  6. Simular envio de paquete\n");
    printf("  7. Ejecutar pruebas automaticas\n");
    printf("  0. Salir\n");
    imprimirSeparador('-');
    printf("Opcion: ");
}

/*
    5. ACCIONES DE CADA OPCIÓN DEL MENÚ
    Delegan el trabajo real a las funciones de grafo.c, dijkstra.c y enrutamiento.c.
 */

/* Opción 1: Lee ruta y llama a cargarTopologiaDesdeArchivo() */
static void accionCargarArchivo(Red *red) {
    char ruta[MAX_LINEA];
    leerCadena("Ingrese la ruta del archivo de topologia: ", ruta, sizeof(ruta));

    if (ruta[0] == '\0') {
        printf("No se ingreso ninguna ruta.\n");
        return;
    }

    if (cargarTopologiaDesdeArchivo(red, ruta) == 0) {
        printf("Topologia cargada desde '%s' (%d routers).\n", ruta, red->numRouters);
    } else {
        printf("Error al cargar la topologia desde '%s'.\n", ruta);
    }
}

/* Opción 2: Carga la red de prueba hardcodeada con cargarTopologiaEjemplo() */
static void accionCargarEjemplo(Red *red) {
    if (cargarTopologiaEjemplo(red) == 0) {
        printf("Topologia de ejemplo cargada (%d routers).\n", red->numRouters);
    } else {
        printf("Error al cargar la topologia de ejemplo.\n");
    }
}

/* Opción 3: Solicita índices y costo, y llama a agregarEnlace() */
static void accionAgregarEnlace(Red *red) {
    if (red->numRouters == 0) {
        printf("La red no tiene routers cargados aun.\n");
        return;
    }

    int origen, destino, costo;
    if (leerEntero("Indice router 1: ", &origen) != 0) return;
    if (leerEntero("Indice router 2: ", &destino) != 0) return;
    if (leerEntero("Costo del enlace: ", &costo) != 0) return;

    if (agregarEnlace(red, origen, destino, costo) == 0) {
        printf("Enlace agregado entre %s y %s (costo %d).\n",
               red->nombres[origen], red->nombres[destino], costo);
    } else {
        printf("No se pudo agregar el enlace (verifique indices, duplicados o costo > 0).\n");
    }
}

/* Opción 4: Imprime la matriz de adyacencia con mostrarTopologia() */
static void accionMostrarTopologia(const Red *red) {
    mostrarTopologia(red);
}

/* Opción 5: Ejecuta dijkstra(), arma la tabla y la muestra con mostrarTablaRutas() */
static void accionCalcularTablaRutas(const Red *red) {
    if (red->numRouters == 0) {
        printf("La red no tiene routers cargados aun.\n");
        return;
    }

    int origen;
    if (leerEntero("Indice del router origen: ", &origen) != 0) return;

    if (origen < 0 || origen >= red->numRouters) {
        printf("Indice fuera de rango (0 a %d).\n", red->numRouters - 1);
        return;
    }

    ResultadoDijkstra res;
    if (dijkstra(red, origen, &res) != 0) {
        printf("Error al ejecutar el algoritmo de Dijkstra.\n");
        return;
    }

    EntradaRuta tabla[MAX_ROUTERS];
    if (construirTablaRutas(red, origen, &res, tabla) == 0) {
        printf("\nOrigen: %s\n", red->nombres[origen]);
        mostrarTablaRutas(red, tabla);
    } else {
        printf("Error al construir la tabla de rutas.\n");
    }
}

/* Opción 6: Ejecuta dijkstra() y muestra el recorrido con simularEnvioPaquete() */
static void accionSimularPaquete(const Red *red) {
    if (red->numRouters == 0) {
        printf("La red no tiene routers cargados aun.\n");
        return;
    }

    int origen, destino;
    if (leerEntero("Indice router origen: ", &origen) != 0) return;
    if (leerEntero("Indice router destino: ", &destino) != 0) return;

    if (origen < 0 || origen >= red->numRouters ||
        destino < 0 || destino >= red->numRouters) {
        printf("Indices fuera de rango (0 a %d).\n", red->numRouters - 1);
        return;
    }

    ResultadoDijkstra res;
    if (dijkstra(red, origen, &res) != 0) {
        printf("Error al calcular las rutas.\n");
        return;
    }

    simularEnvioPaquete(red, origen, destino, &res);
}

/* Opción 7: Ejecuta toda la batería de tests de tests.c */
static void accionEjecutarPruebas(void) {
    int fallos = ejecutarTodasLasPruebas();
    printf("Pruebas con fallos: %d\n", fallos);
}

/*
    6. FUNCIÓN PRINCIPAL (main)
    Controla: modo --test, principio de simulación, bucle del menú y fin.
*/
int main(int argc, char *argv[]) {
    /* Soporte para ejecución directa de pruebas (ej. make test -> ./simulador_rutas --test) */
    if (argc > 1 && strcmp(argv[1], "--test") == 0) {
        return ejecutarTodasLasPruebas();
    }

    /* 6.1 Principio de la simulación: banner inicial e inicialización vacía de la red */
    mostrarInicioSimulacion();

    Red red;
    inicializarRed(&red, 0);

    /* 6.2 Bucle interactivo del menú hasta elegir 0 */
    int opcion = -1;
    do {
        mostrarMenu();

        if (scanf("%d", &opcion) != 1) {
            limpiarBuffer();
            printf("Entrada invalida. Ingrese un numero del menu.\n");
            opcion = -1;
            continue;
        }
        limpiarBuffer();

        switch (opcion) {
            case 1:
                accionCargarArchivo(&red);
                break;
            case 2:
                accionCargarEjemplo(&red);
                break;
            case 3:
                accionAgregarEnlace(&red);
                break;
            case 4:
                accionMostrarTopologia(&red);
                break;
            case 5:
                accionCalcularTablaRutas(&red);
                break;
            case 6:
                accionSimularPaquete(&red);
                break;
            case 7:
                accionEjecutarPruebas();
                break;
            case 0:
                printf("Saliendo del simulador...\n");
                break;
            default:
                printf("Opcion no valida. Intente nuevamente.\n");
                break;
        }
    } while (opcion != 0);

    /* 6.3 Fin de la simulación: liberación de recursos y banner de cierre */
    liberarRed(&red);
    mostrarFinSimulacion();

    return 0;
}