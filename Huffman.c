#include <stdio.h>
#include <stdlib.h>

#define MAX_NODES 128
#define MAX_HEAP  64

/*
 * Nó da árvore de Huffman
 * - símbolo: caractere (ou '*' para nó interno)
 * - freq: frequência do símbolo
 */
typedef struct {
    char symbol;
    int freq;
} Node;

/* Heap mínimo (fila de prioridade) */
Node heap[MAX_HEAP];
int heap_size = 0;

/* Vetor auxiliar para armazenar a construção da árvore */
Node huffman_tree[MAX_HEAP * 2];

/* ---------- Heap mínimo ---------- */

void heap_insert(Node x) {
    int i = heap_size++;
    heap[i] = x;

    while (i > 0) {
        int p = (i - 1) / 2;
        if (heap[p].freq <= heap[i].freq) break;

        Node tmp = heap[i];
        heap[i] = heap[p];
        heap[p] = tmp;
        i = p;
    }
}

Node heap_pop() {
    Node min = heap[0];
    heap[0] = heap[--heap_size];

    int i = 0;
    while (1) {
        int l = 2 * i + 1;
        int r = l + 1;
        int smallest = i;

        if (l < heap_size && heap[l].freq < heap[smallest].freq)
            smallest = l;
        if (r < heap_size && heap[r].freq < heap[smallest].freq)
            smallest = r;

        if (smallest == i) break;

        Node tmp = heap[i];
        heap[i] = heap[smallest];
        heap[smallest] = tmp;
        i = smallest;
    }

    return min;
}

/* ---------- Algoritmo de Huffman ---------- */

void build_huffman(int n) {
    int idx = 0;

    while (heap_size > 1) {
        Node a = heap_pop();
        Node b = heap_pop();

        huffman_tree[idx++] = a;
        huffman_tree[idx++] = b;

        Node parent;
        parent.symbol = '*';               // nó interno
        parent.freq = a.freq + b.freq;

        heap_insert(parent);
    }

    /* Raiz da árvore */
    Node root = heap_pop();
    huffman_tree[idx] = root;
}

/* ---------- Main ---------- */

int main() {
    FILE *file = fopen("leave.txt", "r");
    if (!file) {
        perror("Erro ao abrir arquivo");
        return 1;
    }

    char line[MAX_NODES];

    /*
     * Leitura das folhas (símbolo + frequência)
     */
    while (fgets(line, MAX_NODES, file)) {
        Node n;
        n.symbol = line[0];
        n.freq = atoi(&line[2]);
        heap_insert(n);
    }

    fclose(file);

    int leaf_count = heap_size;

    build_huffman(leaf_count);

    printf("Nós usados na construção da árvore de Huffman:\n");
    for (int i = 0; i < 2 * leaf_count - 1; i++) {
        printf("Símbolo: %c | Frequência: %d\n",
               huffman_tree[i].symbol,
               huffman_tree[i].freq);
    }

    return 0;
}
