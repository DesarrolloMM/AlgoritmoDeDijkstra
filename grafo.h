#ifndef GRAFO_H
#define GRAFO_H

#define MAX_ROUTERS 20
#define INF 999999
#define MAX_NOMBRE  32
#define MAX_LINEA   256

/* Red simulada: matriz de adyacencia + nombres de routers.
 * Convención: matriz[i][j] == INF significa "sin enlace directo".
 *             matriz[i][i] == 0 siempre. */
typedef struct {
    int  numRouters;
    int  matriz[MAX_ROUTERS][MAX_ROUTERS];
    char nombres[MAX_ROUTERS][MAX_NOMBRE];
} Red;

int  inicializarRed(Red *red, int n);
int  setNombreRouter(Red *red, int idx, const char *nombre);
int  agregarEnlace(Red *red, int i, int j, int costo);
int  eliminarEnlace(Red *red, int i, int j);
int  cargarTopologiaDesdeArchivo(Red *red, const char *ruta);
int  cargarTopologiaEjemplo(Red *red);
void mostrarTopologia(const Red *red);
void liberarRed(Red *red);

#endif /* GRAFO_H */