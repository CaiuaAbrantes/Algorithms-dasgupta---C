#include <stdio.h>
#include <stdlib.h>

#define MAX_NODES 128   // Nós representados por caracteres ASCII
#define MAX_LINE  80    // Tamanho máximo de uma linha do arquivo

/* 
 * Matriz de adjacência do grafo não-direcionado
 * graph[u][v] = 1 indica uma aresta entre u e v
 */
int graph[MAX_NODES][MAX_NODES];

/* Lista de nós presentes no grafo */
char nodes[MAX_NODES];

/*
 * Vetor que indica o grupo (componente conexa) de cada nó
 * 0  -> nó ainda não visitado
 * >0 -> identificador da componente
 */
int component[MAX_NODES];

/* Contador global de componentes */
int current_component = 0;

/*
 * DFS para marcar todos os nós da mesma componente conexa
 */
void dfs(int u) {
    if (component[u] != 0) return;

    component[u] = current_component;
    printf("Visitando %c (componente %d)\n", (char)u, current_component);

    for (int v = 0; v < MAX_NODES; v++) {
        if (graph[u][v]) {
            dfs(v);
        }
    }
}

/*
 * Inicializa estruturas globais
 */
void initialize() {
    for (int i = 0; i < MAX_NODES; i++) {
        component[i] = 0;
        for (int j = 0; j < MAX_NODES; j++) {
            graph[i][j] = 0;
        }
    }
}

int main() {
    FILE *file = fopen("componente.txt", "r");
    if (!file) {
        perror("Erro ao abrir o arquivo");
        return 1;
    }

    initialize();

    int node_count = 0;
    char line[MAX_LINE];

    /*
     * Leitura do grafo
     * Formato esperado: "A B 3"
     * Apenas os dois primeiros caracteres são usados
     */
    while (fgets(line, MAX_LINE, file)) {
        char u = line[0];
        char v = line[2];

        /* Adiciona nós à lista, se ainda não existirem */
        if (component[u] == 0) {
            nodes[node_count++] = u;
            component[u] = -1; // marca como existente
        }
        if (component[v] == 0) {
            nodes[node_count++] = v;
            component[v] = -1;
        }

        /* Grafo NÃO direcionado */
        graph[u][v] = 1;
        graph[v][u] = 1;

        printf("Aresta: %c -- %c\n", u, v);
    }

    fclose(file);

    /* Limpa marcas temporárias */
    for (int i = 0; i < MAX_NODES; i++) {
        if (component[i] == -1)
            component[i] = 0;
    }

    /*
     * Identificação das componentes conexas
     */
    for (int i
