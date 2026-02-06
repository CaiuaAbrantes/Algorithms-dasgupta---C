//Longest Increasing Subsequence#include <stdio.h>

#define N 8

int matriz[N][N];   // matriz de adjacência do DAG
int L[N];           // tamanho da LIS terminando em i
int prev[N];        // predecessor para reconstrução da sequência

/*
 * Cria um DAG onde existe aresta i -> j
 * se i < j e lista[i] < lista[j]
 */
void criar_grafo(int lista[]) {
    for (int i = 0; i < N - 1; i++) {
        for (int j = i + 1; j < N; j++) {
            if (lista[i] < lista[j]) {
                matriz[i][j] = 1;
            }
        }
    }
}

/*
 * Calcula a Longest Increasing Subsequence (LIS)
 * usando programação dinâmica sobre o DAG
 */
void LIS(int lista[]) {
    int maior = 0;
    int fim = 0;

    for (int i = 0; i < N; i++) {
        int max = 0;
        prev[i] = -1;

        for (int j = i - 1; j >= 0; j--) {
            if (matriz[j][i] && L[j] >= max) {
                max = L[j];
                prev[i] = j;
            }
        }

        L[i] = max + 1;

        if (L[i] > maior) {
            maior = L[i];
            fim = i;
        }
    }

    /* Reconstrução da sequência */
    printf("LIS: ");
    while (fim != -1) {
        printf("%d ", lista[fim]);
        fim = prev[fim];
    }
    printf("\n");
}

int main() {

    int lista[N] = {5, 2, 8, 6, 3, 6, 9, 7};

    /* Inicialização */
    for (int i = 0; i < N; i++) {
        L[i] = 0;
        for (int j = 0; j < N; j++) {
            matriz[i][j] = 0;
        }
    }

    criar_grafo(lista);
    LIS(lista);

    return 0;
}
