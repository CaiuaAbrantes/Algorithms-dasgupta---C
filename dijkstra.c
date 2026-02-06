#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define MAX_NODES 128
#define MAX_LINE  80
#define INF 1000000

/* Estrutura do grafo */
int graph[MAX_NODES][MAX_NODES];     // adjacência
int weight[MAX_NODES][MAX_NODES];    // pesos

/* Dijkstra */
int dist[MAX_NODES];                 // menor distância
char parent[MAX_NODES];              // predecessor
int visited[MAX_NODES];

/* Lista de nós */
char nodes[MAX_NODES];

/* ---------- Heap (fila de prioridade) ---------- */

typedef struct {
    char node;
    int dist;
} HeapNode;

HeapNode heap[MAX_NODES * MAX_NODES];
int heap_size = 0;

void heap_insert(HeapNode x) {
    int i = heap_size++;
    heap[i] = x;

    while (i > 0) {
        int p = (i - 1) / 2;
        if (heap[p].dist <= heap[i].dist) break;

        HeapNode tmp = heap[i];
        heap[i] = heap[p];
        heap[p] = tmp;
        i = p;
    }
}

HeapNode heap_pop() {
    HeapNode min = heap[0];
    heap[0] = heap[--heap_size];

    int i = 0;
    while (1) {
        int l = 2 * i + 1;
        int r = l + 1;
        int smallest = i;

        if (l < heap_size && heap[l].dist < heap[smallest].dist)
            smallest = l;
        if (r < heap_size && heap[r].dist < heap[smallest].dist)
            smallest = r;

        if (smallest == i) break;

        HeapNode tmp = heap[i];
        heap[i] = heap[smallest];
        heap[smallest] = tmp;
        i = smallest;
    }

    return min;
}

/* ---------- Dijkstra ---------- */

void dijkstra(char start) {
    for (int i = 0; i < MAX_NODES; i++) {
        dist[i] = INF;
        visited[i] = 0;
        parent[i] = 0;
    }

    dist[start] = 0;
    parent[start] = start;

    heap_insert((HeapNode){start, 0});

    while (heap_size > 0) {
        HeapNode current = heap_pop();
        char u = current.node;

        if (visited[u]) continue;
        visited[u] = 1;

        for (int v = 0; v < MAX_NODES; v++) {
            if (graph[u][v]) {
                int new_dist = dist[u] + weight[u][v];
                if (new_dist < dist[v]) {
                    dist[v] = new_dist;
                    parent[v] = u;
                    heap_insert((HeapNode){v, new_dist});
                }
            }
        }
    }
}

/* ---------- Impressão do caminho ---------- */

void print_path(char start, char end) {
    if (start == end) {
        printf("%c ", start);
        return;
    }
    print_path(start, parent[end]);
    printf("%c ", end);
}

/* ---------- Main ---------- */

int main() {
    FILE *file = fopen("data.txt", "r");
    if (!file) {
        perror("Erro ao abrir arquivo");
        return 1;
    }

    char line[MAX_LINE];
    int node_count = 0;

    for (int i = 0; i < MAX_NODES; i++)
        for (int j = 0; j < MAX_NODES; j++)
            graph[i][j] = weight[i][j] = 0;

    while (fgets(line, MAX_LINE, file)) {
        char u = line[0];
        char v = line[2];
        int w = atoi(&line[4]);

        graph[u][v] = 1;
        weight[u][v] = w;

        nodes[node_count++] = u;
        nodes[node_count++] = v;
    }

    fclose(file);

    char start = nodes[0];
    char end   = nodes[1];

    dijkstra(start);

    printf("Menor distância de %c até %c = %d\n", start, end, dist[end]);
    printf("Caminho: ");
    print_path(start, end);
    printf("\n");

    return 0;
}
