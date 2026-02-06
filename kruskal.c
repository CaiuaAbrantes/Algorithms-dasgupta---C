#include <stdio.h>
#include <stdlib.h>

#define MAX_NO 128
#define MAX_COL 80

/* ---------- Estruturas ---------- */

typedef struct {
    char u;
    char v;
    int peso;
} Aresta;

/* ---------- Variáveis Globais ---------- */

Aresta heap[MAX_NO];     // Min-heap de arestas
Aresta mst[MAX_NO];      // Arestas da MST
int heap_size = 0;

char vertices[MAX_NO];
int visitado[MAX_NO];

char parent[MAX_NO];
int rank[MAX_NO];

char linha[MAX_COL];

/* ---------- Union-Find ---------- */

void makeset(char x) {
    parent[x] = x;
    rank[x] = 0;
}

char find(char x) {
    if (x != parent[x]) {
        parent[x] = find(parent[x]); // compressão de caminho
    }
    return parent[x];
}

void unite(char x, char y) {
    char rx = find(x);
    char ry = find(y);

    if (rx == ry) return;

    if (rank[rx] < rank[ry]) {
        parent[rx] = ry;
    } else if (rank[rx] > rank[ry]) {
        parent[ry] = rx;
    } else {
        parent[ry] = rx;
        rank[rx]++;
    }
}

/* ---------- Min-Heap ---------- */

void heap_insert(Aresta e) {
    int i = heap_size++;
    heap[i] = e;

    while (i > 0) {
        int pai = (i - 1) / 2;
        if (heap[pai].peso <= heap[i].peso) break;

        Aresta tmp = heap[pai];
        heap[pai] = heap[i];
        heap[i] = tmp;

        i = pai;
    }
}

Aresta heap_extract_min() {
    Aresta min = heap[0];
    heap[0] = heap[--heap_size];

    int i = 0;
    while (1) {
        int esq = 2 * i + 1;
        int dir = 2 * i + 2;
        int menor = i;

        if (esq < heap_size && heap[esq].peso < heap[menor].peso)
            menor = esq;
        if (dir < heap_size && heap[dir].peso < heap[menor].peso)
            menor = dir;

        if (menor == i) break;

        Aresta tmp = heap[i];
        heap[i] = heap[menor];
        heap[menor] = tmp;

        i = menor;
    }

    return min;
}

/* ---------- Kruskal ---------- */

int kruskal(char vertices[], int n) {
    for (int i = 0; i < n; i++) {
        makeset(vertices[i]);
    }

    int mst_size = 0;

    while (heap_size > 0) {
        Aresta e = heap_extract_min();

        if (find(e.u) != find(e.v)) {
            mst[mst_size++] = e;
            unite(e.u, e.v);
        }
    }

    return mst_size;
}

/* ---------- Main ---------- */

int main() {
    FILE *f = fopen("data.txt", "r");
    if (!f) {
        printf("Erro ao abrir arquivo.\n");
        return 1;
    }

    int n_vertices = 0;

    while (fgets(linha, MAX_COL, f)) {
        char u, v;
        int peso;

        sscanf(linha, " %c %c %d", &u, &v, &peso);

        if (!visitado[u]) {
            vertices[n_vertices++] = u;
            visitado[u] = 1;
        }
        if (!visitado[v]) {
            vertices[n_vertices++] = v;
            visitado[v] = 1;
        }

        Aresta e = {u, v, peso};
        heap_insert(e);
    }

    fclose(f);

    int mst_size = kruskal(vertices, n_vertices);

    printf("Arestas da Árvore Geradora Mínima:\n");
    for (int i = 0; i < mst_size; i++) {
        printf("%c - %c (%d)\n", mst[i].u, mst[i].v, mst[i].peso);
    }

    return 0;
}
