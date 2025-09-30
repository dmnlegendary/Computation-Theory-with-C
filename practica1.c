#include <stdio.h>
#include <stdlib.h>

#define MAX_LEN 200

int longitudCadena(char *cadena) {
    int len = 0;
    while (cadena[len] != '\0') len++;
    return len;
}

void copiarCadena(char *destino, char *origen, int n) {
    for (int i = 0; i < n; i++) {
        destino[i] = origen[i];
    }
    destino[n] = '\0';
}

void invertirCadena(char *cadena, char *invertida, int len) {
    for (int i = 0; i < len; i++) {
        invertida[i] = cadena[len - i - 1];
    }
    invertida[len] = '\0';
}

void obtenerPrefijoSufijo(char *cadena, int n) {
    int len = longitudCadena(cadena);
    if (n > len) n = len;
    char *prefijo = (char *)malloc((len - n + 1) * sizeof(char));
    char *sufijo = (char *)malloc((len - n + 1) * sizeof(char));
    copiarCadena(prefijo, cadena, len - n);
    copiarCadena(sufijo, cadena + n, len - n);
    printf("Prefijo: \"%s\"\n", prefijo);
    printf("Sufijo: \"%s\"\n", sufijo);
    free(prefijo);
    free(sufijo);
}

void obtenerSubcadena(char *cadena, int n, int m) {
    int len = longitudCadena(cadena);
    if (n + m >= len) {
        printf("Subcadena eliminada, solo queda lambda.\n");
        return;
    }
    char *resultado = (char *)malloc((len - n - m + 1) * sizeof(char));
    copiarCadena(resultado, cadena + n, len - n - m);
    printf("Resultado: \"%s\"\n", resultado);
    free(resultado);
}

void obtenerSubsecuencia(char *cadena, int *posiciones, int numPos) {
    int len = longitudCadena(cadena);
    char *resultado = (char *)malloc((len + 1) * sizeof(char));
    int idx = 0;
    for (int i = 0; i < len; i++) {
        int eliminar = 0;
        for (int j = 0; j < numPos; j++) {
            if (i == posiciones[j]) {
                eliminar = 1;
                break;
            }
        }
        if (!eliminar) {
            resultado[idx++] = cadena[i];
        }
    }
    resultado[idx] = '\0';
    printf("Resultado: \"%s\"\n", resultado[0] ? resultado : "lambda");
    free(resultado);
}

void obtenerPotencia(char *cadena, int potencia) {
    int len = longitudCadena(cadena);
    if (potencia == 0) {
        printf("Resultado: {lambda}\n");
        return;
    }
    char *base = (char *)malloc((len + 1) * sizeof(char));
    if (potencia < 0) {
        invertirCadena(cadena, base, len);
    } else {
        copiarCadena(base, cadena, len);
    }
    char *resultado = (char *)malloc((len * abs(potencia) + 1) * sizeof(char));
    resultado[0] = '\0';
    for (int i = 0; i < abs(potencia); i++) {
        copiarCadena(resultado + i * len, base, len);
    }
    resultado[len * abs(potencia)] = '\0';
    printf("Resultado: \"%s\"\n", resultado);
    free(base);
    free(resultado);
}

int main() {
    char *cadena = NULL;
    int opcion;
    do {
        printf("\n=== Menu ===\n");
        printf("1. Ingresar nueva cadena\n");
        printf("2. Prefijo y Sufijo\n");
        printf("3. Subcadena\n");
        printf("4. Subsecuencia\n");
        printf("5. Potencia\n");
        printf("6. Salir\n");
        printf("Seleccione una opcion: ");
        scanf("%d", &opcion);
        getchar();
        
        if (opcion == 6) break;
        
        if (opcion == 1) {
            if (cadena) free(cadena);
            cadena = (char *)malloc(MAX_LEN * sizeof(char));
            printf("Ingrese la cadena: ");
            fgets(cadena, MAX_LEN, stdin);
            for (int i = 0; cadena[i] != '\0'; i++) {
                if (cadena[i] == '\n') {
                    cadena[i] = '\0';
                    break;
                }
            }
        } else if (cadena == NULL) {
            printf("ε\n");
        } else {
            switch (opcion) {
                case 2: {
                    int n;
                    printf("Ingrese n: ");
                    scanf("%d", &n);
                    obtenerPrefijoSufijo(cadena, n);
                    break;
                }
                case 3: {
                    int n, m;
                    printf("Ingrese n y m: ");
                    scanf("%d %d", &n, &m);
                    obtenerSubcadena(cadena, n, m);
                    break;
                }
                case 4: {
                    int numPos, posiciones[MAX_LEN];
                    printf("Ingrese la cantidad de posiciones a eliminar: ");
                    scanf("%d", &numPos);
                    printf("Ingrese las posiciones: ");
                    for (int i = 0; i < numPos; i++) {
                        scanf("%d", &posiciones[i]);
                    }
                    obtenerSubsecuencia(cadena, posiciones, numPos);
                    break;
                }
                case 5: {
                    int potencia;
                    printf("Ingrese la potencia: ");
                    scanf("%d", &potencia);
                    obtenerPotencia(cadena, potencia);
                    break;
                }
                default:
                    printf("Opcion invalida.\n");
            }
        }
    } while (opcion != 6);
    if (cadena) free(cadena);
    return 0;
}