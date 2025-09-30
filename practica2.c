#include <stdio.h>
#include <stdlib.h>

#define MAX_CADENAS 100
#define MAX_LEN 200

int longitudCadena(char *cadena) {
    int len = 0;
    while (cadena[len] != '\0') len++;
    return len;
}

void copiarCadena(char *destino, char *origen) {
    int i = 0;
    while (origen[i] != '\0') {
        destino[i] = origen[i];
        i++;
    }
    destino[i] = '\0';
}

void invertirCadena(char *cadena) {
    int len = longitudCadena(cadena);
    for (int i = 0; i < len / 2; i++) {
        char temp = cadena[i];
        cadena[i] = cadena[len - i - 1];
        cadena[len - i - 1] = temp;
    }
}

void unionLenguajes(char **L1, int n1, char **L2, int n2) {
    printf("Union de L1 y L2:\n");
    for (int i = 0; i < n1; i++) printf("%s\n", L1[i]);
    for (int i = 0; i < n2; i++) printf("%s\n", L2[i]);
}

void concatenacionLenguajes(char **L1, int n1, char **L2, int n2) {
    printf("Concatenacion de L1 y L2:\n");
    for (int i = 0; i < n1; i++) {
        for (int j = 0; j < n2; j++) {
            int len1 = longitudCadena(L1[i]);
            int len2 = longitudCadena(L2[j]);
            char *concatenado = (char *)malloc((len1 + len2 + 1) * sizeof(char));
            copiarCadena(concatenado, L1[i]);
            copiarCadena(concatenado + len1, L2[j]);
            printf("%s\n", concatenado);
            free(concatenado);
        }
    }
}

void concatenacionL2L1(char **L1, int n1, char **L2, int n2) {
    printf("Concatenacion de L2 y L1:\n");
    for (int i = 0; i < n2; i++) {
        for (int j = 0; j < n1; j++) {
            int len1 = longitudCadena(L1[j]);
            int len2 = longitudCadena(L2[i]);
            char *concatenado = (char *)malloc((len1 + len2 + 1) * sizeof(char));
            copiarCadena(concatenado, L2[i]);
            copiarCadena(concatenado + len2, L1[j]);
            printf("%s\n", concatenado);
            free(concatenado);
        }
    }
}

void potenciaLenguaje(char **L, int n, int potencia) {
    printf("Potencia %d de L:\n", potencia);
    if (potencia == 0) {
        printf("Lambda\n");
        return;
    }
    for (int i = 0; i < n; i++) {
        int len = longitudCadena(L[i]);
        char *cadena = (char *)malloc((len + 1) * sizeof(char));
        copiarCadena(cadena, L[i]);
        
        int abs_potencia = potencia;
        if (potencia < 0) {
            invertirCadena(cadena);
            abs_potencia = -potencia;
        }
        
        char *resultado = (char *)malloc((len * abs_potencia + 1) * sizeof(char));
        resultado[0] = '\0';
        for (int j = 0; j < abs_potencia; j++) {
            copiarCadena(resultado + j * len, cadena);
        }
        resultado[len * abs_potencia] = '\0';
        printf("%s\n", resultado);
        free(resultado);
        free(cadena);
    }
}

int main() {
    char *L1[MAX_CADENAS], *L2[MAX_CADENAS];
    int n1 = 0, n2 = 0, opcion;
    do {
        printf("\n=== Menu ===\n");
        printf("1. Ingresar lenguaje L1\n");
        printf("2. Ingresar lenguaje L2\n");
        printf("3. Union L1 U L2\n");
        printf("4. Concatenacion L1L2\n");
        printf("5. Concatenacion L2L1\n");
        printf("6. Potencia de L1 o L2\n");
        printf("7. Salir\n");
        printf("Seleccione una opcion: ");
        scanf("%d", &opcion);
        getchar();
        if (opcion == 7) break;
        if (opcion == 1 || opcion == 2) {
            int *n = (opcion == 1) ? &n1 : &n2;
            char **L = (opcion == 1) ? L1 : L2;
            printf("Ingrese el numero de cadenas: ");
            scanf("%d", n);
            getchar();
            for (int i = 0; i < *n; i++) {
                L[i] = (char *)malloc(MAX_LEN * sizeof(char));
                printf("Ingrese la cadena %d: ", i + 1);
                fgets(L[i], MAX_LEN, stdin);
                for (int j = 0; L[i][j] != '\0'; j++) {
                    if (L[i][j] == '\n') {
                        L[i][j] = '\0';
                        break;
                    }
                }
            }
        } else if (opcion == 3) {
            unionLenguajes(L1, n1, L2, n2);
        } else if (opcion == 4) {
            concatenacionLenguajes(L1, n1, L2, n2);
        } else if (opcion == 5) {
            concatenacionL2L1(L1, n1, L2, n2);
        } else if (opcion == 6) {
            int p, lang;
            printf("Ingrese 1 para L1 o 2 para L2: ");
            scanf("%d", &lang);
            printf("Ingrese la potencia: ");
            scanf("%d", &p);
            potenciaLenguaje(lang == 1 ? L1 : L2, lang == 1 ? n1 : n2, p);
        } else {
            printf("Opcion invalida.\n");
        }
    } while (opcion != 7);
    for (int i = 0; i < n1; i++) free(L1[i]);
    for (int i = 0; i < n2; i++) free(L2[i]);
    return 0;
}