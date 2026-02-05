#include <stdio.h>
#include <stdlib.h>
//Algoritimo depth first search
#define MAX_NODES 128   // Quantidade máxima de nós (ASCII)
#define MAX_LINE  80    // Tamanho máximo de uma linha do arquivo

/* 
 * Matriz de adjacência do grafo.
 * matriz[u][v] = 1 indica uma aresta direcionada de u -> v
 */
int graph[MAX_NODES][MAX_NODES];

/* Lista de nós presentes no grafo */
char nodes[MAX_NODES];

/* Vetores auxiliares para DFS */
int visited[MAX_NODES];
int finish_time[MAX_NODES];

/* Relógio global usado para marcar tempo de saída na DFS */
int time_counter = 1;

/*
 * Busca em profundidade (DFS)
 * Marca o tempo de saída de cada nó (finish_time)
 */
void dfs(int u) {
    if (visited[u]) return;

    visited[u] = 1;
    printf("Visitando %c\n", (char)u);

    for (int v = 0; v < MAX_NODES; v++) {
        if (graph[u][v]) {
            dfs(v);
        }
    }

    finish_time[u] = time_counter++;
    printf("Saindo de %c (tempo = %d)\n", (char)u, finish_time[u]);
}

/*
 * Inicializa matrizes e vetores globais
 */
void initialize() {
    for (int i = 0; i < MAX_NODES; i++) {
        visited[i] = 0;
        finish_time[i] = 0;
        for (int j = 0; j < MAX_NODES; j++) {
            graph[i][j] = 0;
        }
    }
}

int main() {
    FILE *file = fopen("data.txt", "r");
    if (!file) {
        perror("Erro ao abrir o arquivo");
        return 1;
    }

    initialize();

    int node_count = 0;
    char line[MAX_LINE];

    /*
     * Leitura do arquivo:
     * Formato esperado: "A B 3"
     * Apenas os dois primeiros caracteres são usados
     */
    while (fgets(line, MAX_LINE, file)) {
        char from = line[0];
        char to   = line[2];

        /* Adiciona os nós à lista, se ainda não existirem */
        if (!visited[from]) {
            nodes[node_count++] = from;
            visited[from] = 1;
        }
        if (!visited[to]) {
            nodes[node_count++] = to;
            visited[to] = 1;
        }

        graph[from][to] = 1;
        printf("Aresta: %c -> %c\n", from, to);
    }

    fclose(file);

    /* Reseta vetor de visitados para a DFS */
    for (int i = 0; i < MAX_NODES; i++) {
        visited[i] = 0;
    }

    /*
     * Executa DFS para calcular tempos de saída
     */
    for (int i = 0; i < node_count; i++) {
        dfs(nodes[i]);
    }

    /*
     * Ordena os nós por tempo de saída (decrescente)
     */
    for (int i = 0; i < node_count; i++) {
        for (int j = i + 1; j < node_count; j++) {
            if (finish_time[(int)nodes[j]] > finish_time[(int)nodes[i]]) {
                char temp = nodes[i];
                nodes[i] = nodes[j];
                nodes[j] = temp;
            }
        }
    }

    /*
     * Cria um novo grafo em forma de caminho
     * baseado na ordem dos tempos de saída
     */
    initialize();

    for (int i = 0; i < node_count - 1; i++) {
        graph[nodes[i]][nodes[i + 1]] = 1;
    }

    printf("\nDFS no grafo ordenado:\n");
    dfs(nodes[0]);

    return 0;
}
