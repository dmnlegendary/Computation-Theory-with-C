#include <stdio.h>
#include <stdlib.h>

#define MAX_STATES 3  // Número de estados del autómata

// Definición de los estados
typedef enum { q0, q1, q2 } State;

// Función para verificar si algún estado activo es de aceptación
int is_accepting(int *active_states) {
    return active_states[q0] || active_states[q2];  // q0 y q2 son estados de aceptación
}

// Función que aplica las transiciones del autómata no determinista
int transition(int *active_states, char input) {
    if (input != 'a' && input != 'b') {
        return 0;  // Si el carácter no es 'a' ni 'b', se rechaza la cadena
    }

    int new_states[MAX_STATES] = {0};  // Inicializamos los nuevos estados como inactivos

    // Evaluamos cada estado activo y propagamos la transición
    if (active_states[q0]) {
        new_states[q0] = 1;  // q0 tiene una transición con 'b' que lo mantiene en sí mismo
        if (input == 'a') new_states[q1] = 1;  // q0 -> q1 con 'a'
    }
    if (active_states[q1] && input == 'b') {
        new_states[q2] = 1;  // q1 -> q2 con 'b'
    }
    if (active_states[q2]) {
        new_states[q2] = 1;  // q2 es de aceptación, no tiene transiciones pero se mantiene
    }

    // Actualizamos los estados activos
    for (int i = 0; i < MAX_STATES; i++) {
        active_states[i] = new_states[i];
    }

    return 1;  // La transición fue válida
}

// Función para leer la entrada dinámicamente
char *read_line() {
    int capacity = 10;
    int length = 0;
    char *buffer = malloc(capacity * sizeof(char));
    if (buffer == NULL) {
        printf("Error al asignar memoria.\n");
        exit(1);
    }

    int c;
    while ((c = getchar()) != EOF && c != '\n') {
        buffer[length++] = (char)c;
        if (length >= capacity - 1) {
            capacity *= 2;
            char *temp = realloc(buffer, capacity * sizeof(char));
            if (temp == NULL) {
                free(buffer);
                printf("Error al realocar memoria.\n");
                exit(1);
            }
            buffer = temp;
        }
    }
    buffer[length] = '\0';
    return buffer;
}

int main() {
    printf("Ingrese una cadena (solo 'a' y 'b'): ");
    char *input = read_line();

    // Inicializamos el conjunto de estados activos
    int active_states[MAX_STATES] = {0};
    active_states[q0] = 1;  // El autómata siempre inicia en q0

    // Procesamos la cadena
    for (int i = 0; input[i] != '\0'; i++) {
        if (!transition(active_states, input[i])) {
            printf("Rejected (Caracter inválido: '%c')\n", input[i]);
            free(input);
            return 0;
        }
    }

    // Verificamos si la cadena es aceptada
    if (is_accepting(active_states))
        printf("Accepted\n");
    else
        printf("Rejected\n");

    free(input);
    return 0;
}