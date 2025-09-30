#include <stdio.h>
#include <stdlib.h>

// Estructura del nodo
struct Nodo {
    char* Nombre; // Nombre del nodo
    char*** Caminos; // Array de arrays de nombres de nodos destino
    int esFinal; // Indica si es un nodo final
    int esInicio; // Indica si es un nodo inicial
    int* CantidadDeCaminos; // Array que almacena la cantidad de caminos para cada entrada
    int numEntradas; // Número de posibles caminos (entradas)
    char** Entradas; // Array de entradas para mantener el mapeo de caracteres
};

// Estructura de la lista
typedef struct {
    struct Nodo* Cabecera; // Puntero al primer nodo de la lista
    int NumNodos; // Número de nodos en la lista
} Lista;

// Función para calcular la longitud de una cadena
int tamano_cadena(const char* cadena) {
    int longitud = 0;
    // Recorre la cadena hasta encontrar el carácter nulo '\0'
    while (cadena[longitud] != '\0') {
        longitud++;
    }
    return longitud; // Retorna la longitud de la cadena
}

// Función para comparar dos cadenas
int comparar_cadenas(const char* cadena1, const char* cadena2) {
    // Compara los caracteres uno por uno
    while (*cadena1 && (*cadena1 == *cadena2)) {
        cadena1++;
        cadena2++;
    }
    // Retorna la diferencia entre los caracteres donde difieren
    return *(unsigned char*)cadena1 - *(unsigned char*)cadena2;
}

// Función para duplicar una cadena
char* duplicar_cadena(const char* cadena) {
    size_t longitud = tamano_cadena(cadena); // Obtiene la longitud de la cadena
    char* duplicado = (char*)malloc((longitud + 1) * sizeof(char)); // Reserva memoria para la nueva cadena
    if (duplicado) {
        // Copia los caracteres de la cadena original a la nueva
        for (size_t i = 0; i <= longitud; i++) {
            duplicado[i] = cadena[i];
        }
    }
    return duplicado; // Retorna la nueva cadena duplicada
}

// Función para dividir una cadena en tokens
char* tonekizar(char* cadena, const char* delimitador) {
    static char* ultima; // Almacena la posición de la última cadena procesada
    if (cadena) {
        ultima = cadena; // Inicializa con la nueva cadena
    }
    if (!ultima) {
        return NULL; // No hay más tokens
    }

    char* token = ultima; // Comienza con la última posición
    while (*ultima) {
        // Recorre la cadena buscando los delimitadores
        for (const char* d = delimitador; *d; d++) {
            if (*ultima == *d) {
                *ultima = '\0'; // Reemplaza el delimitador con '\0'
                ultima++;
                if (ultima == token) {
                    token++;
                } else {
                    return token; // Retorna el token encontrado
                }
            }
        }
        ultima++;
    }
    if (token == ultima) {
        ultima = NULL; // No hay más tokens
    }
    return token; // Retorna el último token
}

// Función para encontrar la primera ocurrencia de una subcadena en una cadena
char* mi_strstr(const char* cadena, const char* subcadena) {
    if (!*subcadena) {
        return (char*)cadena; // Subcadena vacía retorna la cadena completa
    }
    for (const char* p = cadena; *p; p++) {
        if (*p == *subcadena) {
            // Compara los caracteres de la cadena y subcadena
            const char* h = p;
            const char* n = subcadena;
            while (*h && *n && *h == *n) {
                h++;
                n++;
            }
            if (!*n) {
                return (char*)p; // Subcadena completa encontrada
            }
        }
    }
    return NULL; // Subcadena no encontrada
}

// Función para crear un nuevo nodo
struct Nodo* crearNodo(char* nombre, int numEntradas, char** entradas) {
    struct Nodo* nuevoNodo = (struct Nodo*)malloc(sizeof(struct Nodo)); // Reserva memoria para el nodo
    nuevoNodo->Nombre = duplicar_cadena(nombre); // Duplica el nombre del nodo
    nuevoNodo->Caminos = (char***)malloc(numEntradas * sizeof(char**)); // Reserva memoria para los caminos
    nuevoNodo->CantidadDeCaminos = (int*)calloc(numEntradas, sizeof(int)); // Inicializa la cantidad de caminos a cero
    nuevoNodo->esFinal = 0; // Inicializa como nodo no final
    nuevoNodo->esInicio = 0; // Inicializa como nodo no inicial
    nuevoNodo->numEntradas = numEntradas; // Establece el número de entradas
    nuevoNodo->Entradas = (char**)malloc(numEntradas * sizeof(char*)); // Reserva memoria para las entradas
    for (int i = 0; i < numEntradas; i++) {
        nuevoNodo->Entradas[i] = duplicar_cadena(entradas[i]); // Duplica cada entrada
        nuevoNodo->Caminos[i] = NULL; // Inicializa cada camino como NULL
    }
    return nuevoNodo; // Retorna el nuevo nodo creado
}

// Función para agregar un camino a un nodo, evitando duplicados
void agregarCamino(struct Nodo* nodo, char* entrada, char* destino) {
    int indiceEntrada = -1;
    // Busca el índice de la entrada en el nodo
    for (int i = 0; i < nodo->numEntradas; i++) {
        if (comparar_cadenas(nodo->Entradas[i], entrada) == 0) {
            indiceEntrada = i;
            break;
        }
    }
    if (indiceEntrada == -1) {
        return; // Entrada no encontrada
    }

    // Verifica si el destino ya está en los caminos
    for (int i = 0; i < nodo->CantidadDeCaminos[indiceEntrada]; i++) {
        if (comparar_cadenas(nodo->Caminos[indiceEntrada][i], destino) == 0) {
            return; // El destino ya está en el camino, no agregar
        }
    }
    // Reserva memoria para el nuevo camino y lo agrega
    nodo->Caminos[indiceEntrada] = (char**)realloc(nodo->Caminos[indiceEntrada], (nodo->CantidadDeCaminos[indiceEntrada] + 1) * sizeof(char*));
    nodo->Caminos[indiceEntrada][nodo->CantidadDeCaminos[indiceEntrada]] = duplicar_cadena(destino);
    nodo->CantidadDeCaminos[indiceEntrada]++;
}

/*
// Función para imprimir la lista
void imprimirLista(Lista* lista) {
    //printf("Cantidad de nodos en la lista: %d\n", lista->NumNodos);
    // Recorre cada nodo en la lista
    for (int i = 0; i < lista->NumNodos; i++) {
        struct Nodo* nodo = &lista->Cabecera[i];
        //printf("Nodo %s (esFinal: %d, esInicio: %d):\n", nodo->Nombre, nodo->esFinal, nodo->esInicio);
        // Imprime los caminos de cada entrada
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
*/

// Función para contar el número de elementos separados por comas en una línea
int contarElementos(char* linea) {
    int cuenta = 0;
    char* token = tonekizar(linea, ",");
    // Cuenta los tokens en la línea
    while (token) {
        cuenta++;
        token = tonekizar(NULL, ",");
    }
    return cuenta; // Retorna la cantidad de elementos
}

// Función principal para leer el archivo y crear el autómata
void cargarAutomata(char* nombreArchivo, Lista* lista) {
    FILE* archivo = fopen(nombreArchivo, "r");
    if (archivo == NULL) {
        perror("No se pudo abrir el archivo");
        exit(EXIT_FAILURE);
    }

    char linea[100];
    char* token;
    
    // Leer la primera línea (nodos)
    fgets(linea, 100, archivo);
    int numNodos = contarElementos(duplicar_cadena(linea)); // Cuenta el número de nodos
    lista->Cabecera = (struct Nodo*)malloc(numNodos * sizeof(struct Nodo)); // Reserva memoria para los nodos
    lista->NumNodos = numNodos;
    
    // Leer nodos y crear nodos en la lista
    fseek(archivo, 0, SEEK_SET); // Vuelve al inicio del archivo
    fgets(linea, 100, archivo);
    token = tonekizar(linea, ",");
    int indice = 0;
    char** nombresNodos = (char**)malloc(numNodos * sizeof(char*));
    while (token) {
        nombresNodos[indice] = duplicar_cadena(token); // Duplica cada nombre de nodo
        indice++;
        token = tonekizar(NULL, ",");
    }

    // Leer la segunda línea (entradas)
    fgets(linea, 100, archivo);
    int numEntradas = contarElementos(duplicar_cadena(linea)); // Cuenta el número de entradas
    char** entradas = (char**)malloc(numEntradas * sizeof(char*));
    indice = 0;
    token = tonekizar(linea, ",");
    while (token) {
        entradas[indice] = duplicar_cadena(token); // Duplica cada entrada
        indice++;
        token = tonekizar(NULL, ",");
    }
    
    // Agregar "e" como entrada adicional
    entradas = (char**)realloc(entradas, (numEntradas + 1) * sizeof(char*));
    entradas[numEntradas] = duplicar_cadena("e");
    numEntradas++;
    
    for (int i = 0; i < lista->NumNodos; i++) {
        lista->Cabecera[i] = *crearNodo(nombresNodos[i], numEntradas, entradas); // Crea los nodos en la lista
    }
    
    // Leer la tercera línea (estado inicial)
    fgets(linea, 100, archivo);
    token = tonekizar(linea, ",");
    while (token) {
        for (int i = 0; i < lista->NumNodos; i++) {
            if (comparar_cadenas(lista->Cabecera[i].Nombre, token) == 0) {
                lista->Cabecera[i].esInicio = 1; // Marca los nodos iniciales
                break;
            }
        }
        token = tonekizar(NULL, ",");
    }

    // Leer la cuarta línea (nodos finales)
    fgets(linea, 100, archivo);
    token = tonekizar(linea, ",");
    while (token) {
        for (int i = 0; i < lista->NumNodos; i++) {
            if (comparar_cadenas(lista->Cabecera[i].Nombre, token) == 0) {
                lista->Cabecera[i].esFinal = 1; // Marca los nodos finales
                break;
            }
        }
        token = tonekizar(NULL, ",");
    }

    // Leer los caminos
    while (fgets(linea, 100, archivo)) {
        char* origen = tonekizar(linea, ",");
        char* entrada = tonekizar(NULL, ",");
        char* destino = tonekizar(NULL, ",\n"); // Asegurarse de eliminar nueva línea
        for (int i = 0; i < lista->NumNodos; i++) {
            if (comparar_cadenas(lista->Cabecera[i].Nombre, origen) == 0) {
                agregarCamino(&lista->Cabecera[i], entrada, destino); // Agrega los caminos al nodo correspondiente
                break;
            }
        }
    }

    fclose(archivo); // Cierra el archivo

    // Liberar memoria
    for (int i = 0; i < numNodos; i++) {
        free(nombresNodos[i]); // Libera la memoria de los nombres de los nodos
    }
    free(nombresNodos);
    for (int i = 0; i < numEntradas; i++) {
        free(entradas[i]); // Libera la memoria de las entradas
    }
    free(entradas);
}

// Función para buscar un nodo por nombre
struct Nodo* buscarNodo(Lista* lista, char* nombre) {
    // Recorre la lista buscando el nodo por su nombre
    for (int i = 0; i < lista->NumNodos; i++) {
        if (comparar_cadenas(lista->Cabecera[i].Nombre, nombre) == 0) {
            return &lista->Cabecera[i]; // Retorna el nodo encontrado
        }
    }
    return NULL; // Nodo no encontrado
}

// Función recursiva para verificar si una cadena pertenece al lenguaje
void verificarCadena(Lista* lista, struct Nodo* nodo, char* cadena, int posicion, char* recorrido, int* encontrado) {
    if (posicion == tamano_cadena(cadena)) {
        if (nodo->esFinal) {
            printf("Recorrido: %s\n", recorrido);
            printf("La cadena pertenece al lenguaje.\n");
            *encontrado = 1; // Marca que se ha encontrado una cadena válida
        }

        // Verificar transiciones epsilon si la cadena ha terminado
        for (int i = 0; i < nodo->numEntradas; i++) {
            if (comparar_cadenas(nodo->Entradas[i], "e") == 0) {
                for (int j = 0; j < nodo->CantidadDeCaminos[i]; j++) {
                    char nuevoRecorrido[200];
                    snprintf(nuevoRecorrido, sizeof(nuevoRecorrido), "%s -> %s,e", recorrido, nodo->Caminos[i][j]);
                    struct Nodo* siguienteNodo = buscarNodo(lista, nodo->Caminos[i][j]);
                    verificarCadena(lista, siguienteNodo, cadena, posicion, nuevoRecorrido, encontrado);
                }
            }
        }
        return;
    }

    char entrada[2] = {cadena[posicion], '\0'};
    for (int i = 0; i < nodo->numEntradas; i++) {
        if (comparar_cadenas(nodo->Entradas[i], entrada) == 0 || comparar_cadenas(nodo->Entradas[i], "e") == 0) {
            for (int j = 0; j < nodo->CantidadDeCaminos[i]; j++) {
                char nuevoRecorrido[200];
                if (comparar_cadenas(nodo->Entradas[i], "e") == 0) {
                    snprintf(nuevoRecorrido, sizeof(nuevoRecorrido), "%s -> %s,e", recorrido, nodo->Caminos[i][j]);
                    struct Nodo* siguienteNodo = buscarNodo(lista, nodo->Caminos[i][j]);
                    verificarCadena(lista, siguienteNodo, cadena, posicion, nuevoRecorrido, encontrado);
                } else {
                    snprintf(nuevoRecorrido, sizeof(nuevoRecorrido), "%s -> %s,%s", recorrido, nodo->Caminos[i][j], entrada);
                    struct Nodo* siguienteNodo = buscarNodo(lista, nodo->Caminos[i][j]);
                    verificarCadena(lista, siguienteNodo, cadena, posicion + 1, nuevoRecorrido, encontrado);
                }
            }
        }
    }
}

// Función para iniciar la verificación de una cadena
void validacionConCadena(Lista* lista, char* cadena) {
    struct Nodo* nodoInicial = NULL;
    // Encuentra el nodo inicial en la lista
    for (int i = 0; i < lista->NumNodos; i++) {
        if (lista->Cabecera[i].esInicio) {
            nodoInicial = &lista->Cabecera[i];
            break;
        }
    }

    if (nodoInicial == NULL) {
        printf("No hay nodo inicial definido.\n");
        return;
    }

    char recorrido[200] = "";
    int encontrado = 0;
    verificarCadena(lista, nodoInicial, cadena, 0, recorrido, &encontrado);
    if (!encontrado) {
        printf("La cadena no pertenece al lenguaje.\n");
    }
}

int main() {
    Lista lista;
    cargarAutomata("epsilon_nfa4.txt", &lista);
    char cadena[100];
    printf("Ingrese una cadena para verificar (ENTER para cadena vacia): ");

    // Leer entrada con fgets
    if (fgets(cadena, sizeof(cadena), stdin) != NULL) {
        // Eliminar el salto de línea manualmente
        int i = 0;
        while (cadena[i] != '\0') {
            if (cadena[i] == '\n') {
                cadena[i] = '\0';
                break;
            }
            i++;
        }

        validacionConCadena(&lista, cadena);
    } else {
        printf("Error al leer la cadena.\n");
    }

    return 0;
}