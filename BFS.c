#include <stdio.h>
#include <stdlib.h>

#define MAX_NODES 128   // Nós representados por caracteres ASCII
#define MAX_LINE  80    // Tamanho máximo de uma linha do arquivo

/* Matriz de adjacência do grafo direcionado */
int graph[MAX_NODES][MAX_NODES];

/* Lista de nós existentes no grafo */
char nodes[MAX_NODES];

/* Vetor de visitados */
int visited[MAX_NODES];

/*
 * Implementação de BFS (Busca em Largura)
 * Percorre o grafo a partir do nó inicial
 */
void bfs(char start) {
    char queue[MAX_NODES];
    int front = 0;
    int rear = 0;

    /* Inicializa a BFS */
    queue[rear++] = start;
    visited[(int)start] = 1;

    while (front < rear) {
        char u = queue[front++];

        printf("Explorou %c\n", u);

        for (int v = 0; v < MAX_NODES; v++) {
            if (graph[u][v] && !visited[v]) {
                visited[v] = 1;
                queue[rear++] = (char)v;
            }
        }
    }
}

/*
 * Inicializa estruturas globais
 */
void initialize() {
    for (int i = 0; i < MAX_NODES; i++) {
        visited[i] = 0;
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

    char line[MAX_LINE];
    int node_count = 0;

    /*
     * Leitura do grafo
     * Formato esperado: "A B 3"
     * Apenas os dois primeiros caracteres são usados
     */
    while (fgets(line, MAX_LINE, file)) {
        char from = line[0];
        char to   = line[2];

        if (!visited[from]) {
            nodes[node_count++] = from;
            visited[from] = -1; // marca como existente
        }
        if (!visited[to]) {
            nodes[node_count++] = to;
            visited[to] = -1;
        }

        graph[from][to] = 1;
    }

    fclose(file);

    /* Limpa marcações temporárias */
    for (int i = 0; i < MAX_NODES; i++) {
        if (visited[i] == -1)
            visited[i] = 0;
    }

    /*
     * Executa BFS a partir do primeiro nó encontrado
     */
    if (node_count > 0) {
        printf("BFS a partir do nó %c:\n", nodes[0]);
        bfs(nodes[0]);
    }

    return 0;
}
