#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Estructura del nodo
struct Nodo {
    char* Nombre;
    char*** Caminos; // Array de arrays de nombres de nodos destino
    int esFinal;
    int esInicio; // Nuevo campo para indicar si es un nodo inicial
    int* CantidadDeCaminos; // Array que almacena la cantidad de caminos para cada entrada
    int numEntradas; // Número de posibles caminos (entradas)
    char** Entradas; // Array de entradas para mantener el mapeo de caracteres
};

// Estructura de la lista
typedef struct {
    struct Nodo* Cabecera;
    int NumNodos;
} Lista;

// Función para crear un nuevo nodo
struct Nodo* crearNodo(char* nombre, int numEntradas, char** entradas) {
    struct Nodo* nuevoNodo = (struct Nodo*)malloc(sizeof(struct Nodo));
    nuevoNodo->Nombre = strdup(nombre);
    nuevoNodo->Caminos = (char***)malloc(numEntradas * sizeof(char**));
    nuevoNodo->CantidadDeCaminos = (int*)calloc(numEntradas, sizeof(int));
    nuevoNodo->esFinal = 0;
    nuevoNodo->esInicio = 0;
    nuevoNodo->numEntradas = numEntradas;
    nuevoNodo->Entradas = (char**)malloc(numEntradas * sizeof(char*));
    for (int i = 0; i < numEntradas; i++) {
        nuevoNodo->Entradas[i] = strdup(entradas[i]);
        nuevoNodo->Caminos[i] = NULL;
    }
    return nuevoNodo;
}

// Función para agregar un camino a un nodo, evitando duplicados
void agregarCamino(struct Nodo* nodo, char* entrada, char* destino) {
    int entradaIdx = -1;
    for (int i = 0; i < nodo->numEntradas; i++) {
        if (strcmp(nodo->Entradas[i], entrada) == 0) {
            entradaIdx = i;
            break;
        }
    }
    if (entradaIdx == -1) {
        return; // Entrada no encontrada
    }

    for (int i = 0; i < nodo->CantidadDeCaminos[entradaIdx]; i++) {
        if (strcmp(nodo->Caminos[entradaIdx][i], destino) == 0) {
            return; // El destino ya está en el camino, no agregar
        }
    }
    nodo->Caminos[entradaIdx] = (char**)realloc(nodo->Caminos[entradaIdx], (nodo->CantidadDeCaminos[entradaIdx] + 1) * sizeof(char*));
    nodo->Caminos[entradaIdx][nodo->CantidadDeCaminos[entradaIdx]] = strdup(destino);
    nodo->CantidadDeCaminos[entradaIdx]++;
}

// Función para imprimir la lista
void imprimirLista(Lista* lista) {
    //printf("Cantidad de nodos en la lista: %d\n", lista->NumNodos);
    for (int i = 0; i < lista->NumNodos; i++) {
        struct Nodo* nodo = &lista->Cabecera[i];
        //printf("Nodo %s (esFinal: %d, esInicio: %d):\n", nodo->Nombre, nodo->esFinal, nodo->esInicio);
        for (int j = 0; j < nodo->numEntradas; j++) {
            if (nodo->CantidadDeCaminos[j] > 0) {
                //printf("  Camino %s:", nodo->Entradas[j]);
                for (int k = 0; k < nodo->CantidadDeCaminos[j]; k++) {
                    //printf(" %s", nodo->Caminos[j][k]);
                }
                //printf("\n");
            }
        }
    }
}

// Función para contar el número de elementos separados por comas en una línea
int contarElementos(char* linea) {
    int count = 0;
    char* token = strtok(linea, ",");
    while (token) {
        count++;
        token = strtok(NULL, ",");
    }
    return count;
}

// Función para verificar si un nodo combinado ya existe en la lista
int nodoExiste(Lista* lista, char* nombre) {
    for (int i = 0; i < lista->NumNodos; i++) {
        if (strcmp(lista->Cabecera[i].Nombre, nombre) == 0) {
            return 1;
        }
    }
    return 0;
}

// Función para generar combinaciones de nodos
void generarCombinaciones(Lista* lista, char** elementos, int numElementos, int inicio, char* combinacion, int profundidad, int numEntradas, char** entradas) {
    if (profundidad > 0 && !nodoExiste(lista, combinacion)) {
        struct Nodo* nuevoNodo = crearNodo(combinacion, numEntradas, entradas);
        for (int i = 0; i < lista->NumNodos; i++) {
            if (strstr(combinacion, lista->Cabecera[i].Nombre) != NULL) {
                for (int entrada = 0; entrada < lista->Cabecera[i].numEntradas; entrada++) {
                    for (int k = 0; k < lista->Cabecera[i].CantidadDeCaminos[entrada]; k++) {
                        agregarCamino(nuevoNodo, lista->Cabecera[i].Entradas[entrada], lista->Cabecera[i].Caminos[entrada][k]);
                    }
                }
                if (lista->Cabecera[i].esFinal) {
                    nuevoNodo->esFinal = 1;
                }
            }
        }
        lista->Cabecera = (struct Nodo*)realloc(lista->Cabecera, (lista->NumNodos + 1) * sizeof(struct Nodo));
        lista->Cabecera[lista->NumNodos] = *nuevoNodo;
        lista->NumNodos++;
    }
    for (int i = inicio; i < numElementos; i++) {
        if (strcmp(elementos[i], "Vacio\n") != 0) { // Ignorar "Vacio"
            int newLen = strlen(combinacion) + strlen(elementos[i]) + 1;
            char* nuevaCombinacion = (char*)malloc(newLen * sizeof(char));
            snprintf(nuevaCombinacion, newLen, "%s%s",
            combinacion, elementos[i]);
            generarCombinaciones(lista, elementos, numElementos, i + 1, nuevaCombinacion, profundidad + 1, numEntradas, entradas);
            free(nuevaCombinacion);
        }
    }
}

// Función principal para leer el archivo y crear el autómata
void cargar_automata(char* nombreArchivo, Lista* lista) {
    FILE* archivo = fopen(nombreArchivo, "r");
    if (archivo == NULL) {
        perror("No se pudo abrir el archivo");
        exit(EXIT_FAILURE);
    }

    char linea[100];
    char* token;
    
    // Leer la primera línea (nodos)
    fgets(linea, 100, archivo);
    int numNodos = contarElementos(strdup(linea));
    lista->Cabecera = (struct Nodo*)malloc(numNodos * sizeof(struct Nodo));
    lista->NumNodos = numNodos;
    
    // Leer nodos y crear nodos en la lista
    fseek(archivo, 0, SEEK_SET);
    fgets(linea, 100, archivo);
    token = strtok(linea, ",");
    int idx = 0;
    char** nombresNodos = (char**)malloc(numNodos * sizeof(char*));
    while (token) {
        nombresNodos[idx] = strdup(token);
        idx++;
        token = strtok(NULL, ",");
    }

    // Leer la segunda línea (entradas)
    fgets(linea, 100, archivo);
    int numEntradas = contarElementos(strdup(linea));
    char** entradas = (char**)malloc(numEntradas * sizeof(char*));
    idx = 0;
    token = strtok(linea, ",");
    while (token) {
        entradas[idx] = strdup(token);
        idx++;
        token = strtok(NULL, ",");
    }
    
    for (int i = 0; i < lista->NumNodos; i++) {
        lista->Cabecera[i] = *crearNodo(nombresNodos[i], numEntradas, entradas);
    }
    
    // Leer la tercera línea (estado inicial)
    fgets(linea, 100, archivo);
    token = strtok(linea, ",");
    while (token) {
        for (int i = 0; i < lista->NumNodos; i++) {
            if (strcmp(lista->Cabecera[i].Nombre, token) == 0) {
                lista->Cabecera[i].esInicio = 1;
                break;
            }
        }
        token = strtok(NULL, ",");
    }

    // Leer la cuarta línea (nodos finales)
    fgets(linea, 100, archivo);
    token = strtok(linea, ",");
    while (token) {
        for (int i = 0; i < lista->NumNodos; i++) {
            if (strcmp(lista->Cabecera[i].Nombre, token) == 0) {
                lista->Cabecera[i].esFinal = 1;
                break;
            }
        }
        token = strtok(NULL, ",");
    }

    // Leer los caminos
    while (fgets(linea, 100, archivo)) {
        char* origen = strtok(linea, ",");
        char* entrada = strtok(NULL, ",");
        char* destino = strtok(NULL, ",\n"); // Asegurarse de eliminar nueva línea
        for (int i = 0; i < lista->NumNodos; i++) {
            if (strcmp(lista->Cabecera[i].Nombre, origen) == 0) {
                agregarCamino(&lista->Cabecera[i], entrada, destino);
                break;
            }
        }
    }

    fclose(archivo);

    // Generar combinaciones de nodos
    char combinacion[100] = "";
    generarCombinaciones(lista, nombresNodos, numNodos, 0, combinacion, 0, numEntradas, entradas);

    // Liberar memoria
    for (int i = 0; i < numNodos; i++) {
        free(nombresNodos[i]);
    }
    free(nombresNodos);
    for (int i = 0; i < numEntradas; i++) {
        free(entradas[i]);
    }
    free(entradas);
}

// Función para buscar un nodo por nombre
struct Nodo* buscarNodo(Lista* lista, char* nombre) {
    for (int i = 0; i < lista->NumNodos; i++) {
        if (strcmp(lista->Cabecera[i].Nombre, nombre) == 0) {
            return &lista->Cabecera[i];
        }
    }
    return NULL;
}

// Función recursiva para verificar si una cadena pertenece al lenguaje
void verificarCadena(Lista* lista, struct Nodo* nodo, char* cadena, int posicion, char* recorrido, int* encontrado) {
    if (posicion == strlen(cadena)) {
        if (nodo->esFinal) {
            printf("Recorrido: %s\n", recorrido);
            printf("La cadena pertenece al lenguaje.\n");
            *encontrado = 1;
        }
        return;
    }

    char entrada[2] = {cadena[posicion], '\0'};
    for (int i = 0; i < nodo->numEntradas; i++) {
        if (strcmp(nodo->Entradas[i], entrada) == 0) {
            for (int j = 0; j < nodo->CantidadDeCaminos[i]; j++) {
                char nuevoRecorrido[200];
                snprintf(nuevoRecorrido, sizeof(nuevoRecorrido), "%s -> %s,%s", recorrido, nodo->Caminos[i][j], entrada);
                struct Nodo* siguienteNodo = buscarNodo(lista, nodo->Caminos[i][j]);
                verificarCadena(lista, siguienteNodo, cadena, posicion + 1, nuevoRecorrido, encontrado);
            }
        }
    }
}

// Función para iniciar la verificación de una cadena
void buscar_caminos(Lista* lista, char* cadena){
    struct Nodo* nodoInicial = NULL;
    for (int i = 0; i < lista->NumNodos; i++) {
        if (lista->Cabecera[i].esInicio) {
            nodoInicial = &lista->Cabecera[i];
            break;
        }
    }

    if (nodoInicial == NULL){
        printf("No hay nodo inicial definido.\n");
        return;
    }

    char recorrido[300] = "";
    int encontrado = 0;
    verificarCadena(lista, nodoInicial, cadena, 0, recorrido, &encontrado);
    if (!encontrado) {
        printf("La cadena no pertenece al lenguaje.\n");
    }
}

void main(){
    Lista lista;
    cargar_automata("nfa5.txt", &lista);
    char cadena[100];
    printf("Ingrese la cadena a evaluar en el automata: ");
    scanf("%s", cadena);
    buscar_caminos(&lista, cadena);
}