#include <stdio.h>
#include <stdlib.h>

#define MAX_STATES 100
#define MAX_ALPHABET 100
#define MAX_TRANSITIONS 200
#define MAX_LENGTH 100

// Estructuras
typedef struct {
    int from;
    char symbol;
    int to;
} Transition;

// Variables globales del autómata
typedef struct {
    int states[MAX_STATES];
    char alphabet[MAX_ALPHABET];
    int start_state;
    int valid_states[MAX_STATES];
    Transition transitions[MAX_TRANSITIONS];
    int num_states, num_symbols, num_valid_states, num_transitions;
} Automaton;

Automaton automaton;
char stored_string[MAX_LENGTH] = "";

// Función para leer el autómata desde un archivo
void loadAutomaton(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Error: No se pudo abrir el archivo.\n");
        return;
    }

    // Leer estados
    automaton.num_states = 0;
    while (fscanf(file, "%d,", &automaton.states[automaton.num_states]) == 1) {
        automaton.num_states++;
    }
    
    // Leer alfabeto
    automaton.num_symbols = 0;
    while (fscanf(file, " %c,", &automaton.alphabet[automaton.num_symbols]) == 1) {
        automaton.num_symbols++;
    }
    
    // Leer estado inicial
    fscanf(file, "%d", &automaton.start_state);
    
    // Leer estados de aceptación
    automaton.num_valid_states = 0;
    while (fscanf(file, "%d,", &automaton.valid_states[automaton.num_valid_states]) == 1) {
        automaton.num_valid_states++;
    }
    
    // Leer transiciones
    automaton.num_transitions = 0;
    while (fscanf(file, "%d,%c,%d", &automaton.transitions[automaton.num_transitions].from,
                                       &automaton.transitions[automaton.num_transitions].symbol,
                                       &automaton.transitions[automaton.num_transitions].to) == 3) {
        automaton.num_transitions++;
    }
    
    fclose(file);
    printf("Automata cargado correctamente.\n");
}

// Función para ingresar una cadena y almacenarla
void inputString() {
    printf("Ingrese la cadena: ");
    scanf("%s", stored_string);
    printf("Cadena almacenada: %s\n", stored_string);
}

// Función para verificar si un estado es de aceptación
int isAccepting(int state) {
    for (int i = 0; i < automaton.num_valid_states; i++) {
        if (automaton.valid_states[i] == state) {
            return 1;
        }
    }
    return 0;
}

// Función recursiva para encontrar caminos de aceptación
void findPaths(int current_state, int index, char *path) {
    if (stored_string[index] == '\0') {
        if (isAccepting(current_state)) {
            printf("%s\n", path);
        }
        return;
    }

    for (int i = 0; i < automaton.num_transitions; i++) {
        if (automaton.transitions[i].from == current_state &&
            automaton.transitions[i].symbol == stored_string[index]) {
            char new_path[MAX_LENGTH];
            sprintf(new_path, "%s -> %d", path, automaton.transitions[i].to);
            findPaths(automaton.transitions[i].to, index + 1, new_path);
        }
    }
}

// Función para ejecutar el autómata
void runAutomaton() {
    int length = 0;
    while (stored_string[length] != '\0') {
        length++;
    }
    
    if (length == 0) {
        printf("No hay cadena almacenada.\n");
        return;
    }
    printf("Caminos de aceptacion:\n");
    char initial_path[MAX_LENGTH];
    sprintf(initial_path, "%d", automaton.start_state);
    findPaths(automaton.start_state, 0, initial_path);
}

// Menú principal
void menu() {
    int option;
    char filename[100];
    do {
        printf("\nMenu:\n");
        printf("1. Cargar automata desde archivo\n");
        printf("2. Ingresar cadena\n");
        printf("3. Ejecutar autómata\n");
        printf("4. Salir\n");
        printf("Seleccione una opción: ");
        scanf("%d", &option);
        
        switch (option) {
            case 1:
                printf("Ingrese el nombre del archivo: ");
                scanf("%s", filename);
                loadAutomaton(filename);
                break;
            case 2:
                inputString();
                break;
            case 3:
                runAutomaton();
                break;
            case 4:
                printf("Saliendo...\n");
                break;
            default:
                printf("Opcion no valida.\n");
        }
    } while (option != 4);
}

// Función principal
int main() {
    menu();
    return 0;
}