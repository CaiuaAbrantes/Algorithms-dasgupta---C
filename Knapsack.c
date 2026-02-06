#include <stdio.h>

/*
 * Problema da Mochila com repetição (Unbounded Knapsack)
 * Cada item pode ser usado quantas vezes quiser
 */
void knaprep(int valor[], int peso[], int W, int n) {

    int K[W + 1];
    K[0] = 0;

    for (int w = 1; w <= W; w++) {
        int max = 0;

        for (int i = 0; i < n; i++) {
            if (peso[i] <= w) {
                int candidato = K[w - peso[i]] + valor[i];
                if (candidato > max) {
                    max = candidato;
                }
            }
        }

        K[w] = max;
    }

    printf("Resultado (com repeticao): %d\n", K[W]);
}

/*
 * Problema da Mochila sem repetição (0/1 Knapsack)
 * Cada item pode ser usado no máximo uma vez
 */
void knapnorep(int valor[], int peso[], int W, int n) {

    /*
     * K[i][w] representa o maior valor usando
     * os i primeiros itens e capacidade w
     */
    int K[n + 1][W + 1];

    /* Inicialização */
    for (int i = 0; i <= n; i++) {
        for (int w = 0; w <= W; w++) {
            K[i][w] = 0;
        }
    }

    /* Programação dinâmica */
    for (int i = 1; i <= n; i++) {
        for (int w = 1; w <= W; w++) {

            if (peso[i - 1] > w) {
                /* Item não cabe */
                K[i][w] = K[i - 1][w];
            } else {
                /* Escolhe o melhor entre pegar ou não o item */
                int sem_item = K[i - 1][w];
                int com_item = K[i - 1][w - peso[i - 1]] + valor[i - 1];

                K[i][w] = (sem_item > com_item) ? sem_item : com_item;
            }
        }
    }

    printf("Resultado (sem repeticao): %d\n", K[n][W]);
}

int main() {

    int peso[4]  = {6, 3, 4, 2};
    int valor[4] = {30, 14, 16, 9};

    knapnorep(valor, peso, 10, 4);
    // knaprep(valor, peso, 10, 4); // versão com repetição

    return 0;
}
