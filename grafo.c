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
@brief Asigna un nombre legible a un router
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

/*
@brief Agrega un enlace bidireccional con costo
*/
int agregarEnlace(Red *red, int i, int j, int costo){
    if (red == NULL || i < 0 ||i >= red->numRouters || j < 0 || j >= red->numRouters){
        return -1;
    }
    if( i == j || costo <=0){
        return -1;
    }

    //Validar si el enlace ya existe
    if( red->matriz[i][j] != INF){
        return -1;
    }

    red->matriz[i][j] = costo;
    red->matriz[j][i] = costo;

    return 0;

}

/*
@brief Elimina el enlace bidireccional entre dos routers conectados.
*/
int  eliminarEnlace(Red *red, int i, int j){
    if (red == NULL || i < 0 ||i >= red->numRouters || j < 0 || j >= red->numRouters || i == j){
        return -1;
    }

    //Validar que exista el enlace
    if(red->matriz[i][j] == INF){
        return -1;
    }

    red->matriz[i][j] = INF;
    red->matriz[j][i] = INF;
    
    return 0;
}

/*
@brief Carga la topologia desde un archivo de texto
 * Formato del archivo:
 * ROUTERS <cantidad>
 * NOMBRE <indice> <nombre>
 * ENLACE <indice1> <indice2> <costo>
 * Los índices empiezan en 0 y NOMBRE/ENLACE deben ir después de ROUTERS.
 */

int  cargarTopologiaDesdeArchivo(Red *red, const char *ruta){
    
    FILE *archivo;
    char linea[MAX_LINEA];
    int redInicializada = 0;
    char clave [MAX_LINEA];
    int i,j,costo;
    int idx;
    char nombre [MAX_NOMBRE];
    int cantRouters;

    //Valida que la ruta y la red existan
    if(red== NULL || ruta == NULL){
        return -1;
    }

    archivo = fopen(ruta, "r");

    if(archivo == NULL){
        return -1;
    }

    while (fgets(linea, sizeof(linea), archivo) != NULL){
        
        if( linea[0] == '#'){
            continue;
        }

        if(sscanf(linea,"%255s", clave) != 1){
            continue;
        }
        
        if(strcmp(clave, "ROUTERS") == 0){
            if(sscanf(linea, "ROUTERS %d", &cantRouters) == 1 && inicializarRed(red, cantRouters) == 0){
                redInicializada = 1;
            }
        }
        else if(strcmp(clave, "NOMBRE") == 0){
           
            if (redInicializada && sscanf(linea, "NOMBRE %d %31s", &idx, nombre) == 2){
                setNombreRouter(red,idx,nombre);
            }
        }
        else if(strcmp(clave, "ENLACE") == 0){
            if(redInicializada && sscanf(linea, "ENLACE %d %d %d", &i, &j, &costo) == 3){
                agregarEnlace(red, i, j, costo);
            }
        }
    }
    fclose(archivo);

   return redInicializada ? 0 : -1;   
}

/*
@brief Carga una topología hardcodeada
*/

int  cargarTopologiaEjemplo(Red *red){

    if( red == NULL){
        return -1;
    }

    if(inicializarRed(red, 5) == -1){
        return -1;
    }

    agregarEnlace(red, 0, 1, 2); 
    agregarEnlace(red, 0, 2, 5); 
    agregarEnlace(red, 1, 2, 1); 
    agregarEnlace(red, 1, 3, 4); 
    agregarEnlace(red, 2, 3, 1); 
    agregarEnlace(red, 3, 4, 3);

    return 0;
}


/*
@brief Muestra la matríz de adyacencia
*/
void mostrarTopologia(const Red *red){
    
    if(red == NULL || red->numRouters == 0){
        printf("La red no esta inicializada\n");        
        return;
    }

    printf("Matríz de adyacencia\n");
    printf("Nro. de routers: %d\n", red->numRouters);
    
    for (int i = 0; i < red->numRouters; i++)
    {
        printf("%8s", red->nombres[i]);
    }
    printf("\n");
    for (int i = 0; i < red->numRouters; i++)
    {
        printf("%10s",red->nombres[i]);
        for (int j = 0; j < red->numRouters; j++)
        {
            if(red->matriz[i][j] == INF){
                printf("%8s", "INF");
            }
            else{
                printf("%8d", red->matriz[i][j]);
            }
        }
        printf("\n");        
    }
    printf("\n");
}

/*
@brief Libera recursos
*/
void liberarRed(Red *red){
    if (red != NULL)
    {
        red->numRouters = 0;
    }
}
