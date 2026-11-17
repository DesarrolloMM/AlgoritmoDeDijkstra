
Se utiliza Notebook LM para planeamiento de fases, ejemplos y solucion de dudas,
además de busquedas en la web y el motor google, que utiliza IA. 

Se utiliza copilot para revision de comentarios, sugrencias de código y correccion de errores

---------------Fases---------------

Fase 1: 
Representación de la Red en memoria (grafo.h / grafo.c)
Comenzar por lo más simple y tangible: hacer que la red pueda existir en la memoria RAM y se pueda visualizarla por pantalla.
Estructuras básicas: Definir la estructura Red con la matriz de adyacencia matriz[MAX_ROUTERS][MAX_ROUTERS] y el arreglo de 
nombres.

Funciones mínimas: Implementar en grafo.c: 
inicializarRed(): llena la matriz poniendo 0 en la diagonal principal y INF en el resto.
setNombreRouter() y agregarEnlace(): asignan nombres y cargan los costos de los enlaces.
mostrarTopologia(): imprime la matriz formateada en consola.

Fase 2:
El núcleo algorítmico (dijkstra.h / dijkstra.c)Con el grafo funcionando en memoria, cálculo del camino máscorto.
Variables de estado: Crear la estructura ResultadoDijkstra con los arreglos distancia[], predecesor[] y visitado[].
Implementación del bucle: Inicializar el nodo origen con distancia[origen] = 0 y los demás en INF.
En cada iteración, seleccionar el nodo no visitado con la menor distancia acumulada.
Comparar si los resultados coinciden con la tabla teórica dada en el proyecto.

Fase 3:
Tabla de enrutamiento y reconstrucción de rutas (enrutamiento.h / enrutamiento.c) Una vez que predecesor[] se calcula correctamente, transformar esos datos abstractos en información útil de red.
Lógica del Next Hop (Siguiente salto): Para encontrar por qué vecino debe salir un paquete hacia un destino, retroceder siguiendo los valores de predecesor[] desde  "d" hasta hallar el nodo cuyo predecesor directo sea
el origen. Ese nodo es el next hop.
Simulación de paquetes: Recorrer la cadena de predecesores desde el destino hasta el origen, invertir la secuencia e imprimir por pantalla para mostrar el trayecto completo del paquete.

Fase 4: 
Lectura del archivo de topología (grafo.c). Abrir el archivo topologia.txt usando fopen() y leer línea por línea.
Analizar el contenido para identificar las palabras clave (ROUTERS, NOMBRE, ENLACE) y llamar a las funciones setNombreRouter(), agregarEnlace().

Fase 5:
Menú interactivo, pruebas y Valgrind (main.c, tests.c, Makefile)Batería de tests: Integrr las pruebas unitarias proporcionadas.
Menú de usuario: Construye la interfaz por consola mediante un bucle switch.
Control de memoria: Compilar con las opciones estrictas de GCC (-Wall -Wextra -Werror -std=c11 -g -O0) y ejecutar valgrind --leak-check=full para verificar la ausencia de fugas de memoria