/*
 ============================================================================
 Name		 : gcolor.c
 Author		 : Asim Zoulkarni
 Version	 : Standalone Adjacency List-based Implementation
 Copyright	 : All rights reserved
 Description : Approximate Graph Coloring in C, Ansi-style
 ============================================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

#ifndef ATTACH
#define ATTACH 0
#else
#undef ATTACH
#define ATTACH 1
#endif
#define tokenize(x) strtok(x, " \n\r")
#define min(a,b) \
    ({ __typeof__ (a) _a = (a); \
     __typeof__ (b) _b = (b); \
     _a < _b ? _a : _b; })

void *scalloc (int len, size_t size, const char fun) {
    void *p = fun ? malloc (len * size) : calloc (len, size);
    if (!p) {
        fprintf (stderr, "%calloc: Out of memory", fun ? 'm' : 'c');
        exit (1);
    }
    return p;
}

void *reshape (void *p, size_t size) {
    if (!(p = realloc (p, size))) {
        perror ("realloc: Out of memory");
        exit (1);
    }
    return p;
}

typedef struct node {
    int label;
    int index;
    struct node* next;
} Node;

typedef struct queue {
    Node *front, *rear;
} Queue;

typedef struct vertex {
    int label;
    int index;
    int degree;
    int color;
    int deleted : 1;
    Node* adjList;
} Vertex;

typedef struct Graph {
    int numVertices;
    Vertex* vertex;
    Node *adjArray;
    struct Graph *ancestor;
} Graph;

clock_t begin;
void timeTaken (void) {
#if ATTACH
    return;
#endif
    clock_t end = clock ();
    printf("Time taken = %lf\n", ((double) end - begin) / CLOCKS_PER_SEC);
}

int deg_comparator (const void *a, const void *b) {
    return ((Vertex*) b)->degree - ((Vertex*) a)->degree;
}

int col_comparator (const void *a, const void *b) {
    return ((Vertex*) a)->color - ((Vertex*) b)->color;
}

int lab_comparator (const void *a, const void *b) {
    return ((Node*) a)->label - ((Node*) b)->label;
}

Node *createNode (int label, int index) {
    Node* newNode = scalloc (1, sizeof (Node), 0);
    newNode->label = label;
    newNode->index = index;
    return newNode;
}

void enQueue(Queue* q, int color, int vertex) {
    Node* temp = createNode(color, vertex);
    if (NULL == q->rear) {
        q->front = q->rear = temp;
        return;
    }
    q->rear->next = temp;
    q->rear = temp;
}

Node deQueue(Queue* q) {
    Node def = {0, 0, NULL}, val;
    if (NULL == q->front)
        return def;
    Node* temp = q->front;
    val = q ? *temp : def;
    q->front = q->front->next;
    q->rear = q->front ? q->rear : q->front;
    free(temp);
    return val;
}

Graph *createGraph (int n) {
    Graph *graph = scalloc (1, sizeof (Graph), 0);
    graph->vertex = scalloc (n, sizeof (Vertex), 0);
    return graph;
}

void deleteGraph (Graph **graph) {
    int i = 0;
    for (; i < (*graph)->numVertices; i++) {
        Node *next, *p = (*graph)->vertex[i].adjList;
        for (; p; next = p->next, free (p), p = next);
    }
    free ((*graph)->vertex);
    free (*graph);
}

int binarySearch (Node arr[], int l, int r, int x) {
    if (r >= l) {
        int mid = l + (r - l) / 2;
        if (arr[mid].label == x)
            return mid;
        if (arr[mid].label > x)
            return binarySearch (arr, l, mid - 1, x);
        return binarySearch (arr, mid + 1, r, x);
    }
    return -1;
}

int reshapeAdjList (Graph *graph, int n, int m) {
    graph->vertex = reshape (graph->vertex, m * sizeof (Vertex));
    memset (n + graph->vertex, 0, (m > n) * (m - n) * sizeof (Vertex));
    return m;
}

void addEdge (Graph* graph, int s, int d, int idx) {
    Node* newNode = createNode (d, idx);
    newNode->next = graph->vertex[s].adjList;
    graph->vertex[s].adjList = newNode;
    graph->vertex[s].degree++;
}

void printColors (int colors, Graph* G) {
    printf ("%d", colors);
    if (G && G->numVertices && colors) {
        qsort (G->vertex, G->numVertices, sizeof (Vertex), col_comparator);
        int i, current = -1;
        for (i = 0; i < G->numVertices; i++) {
            if (G->vertex[i].color == current) {
                printf (" %d", !ATTACH + G->vertex[i].label);
            } else {
                printf ("\n%d", !ATTACH + G->vertex[i].label);
                current = G->vertex[i].color;
            }
        }
    }
    putchar ('\n');
}

Graph *subgraphInduced (Graph *G, int N) {
    Node *adj = G->adjArray;
    qsort (adj, N, sizeof (Node), lab_comparator);
    Graph *subgraph = scalloc (1, sizeof (Graph), 0);
    subgraph->vertex = scalloc (N, sizeof (Vertex), 0);
    subgraph->ancestor = G->ancestor;
    subgraph->numVertices = N;
    int i = 0;
    for (i = 0; i < N; i++) {
        Node *p = G->vertex[adj[i].index].adjList;
        for (subgraph->vertex[i].label = adj[i].label; p; p = p->next) {
            int idx = binarySearch (adj, 0, N - 1, p->label);
            if (idx != -1)
                addEdge (subgraph, i, p->label, idx);
        }
    }
    return subgraph;
}

int BFS (Graph *G, int V, int offset) {
    int color = 0, failure = 0;
    Queue* q = scalloc (1, sizeof (Queue), 0);
    G->vertex[V].color = offset + color;
    G->ancestor->vertex[G->vertex[V].label].color = offset + color;
    for (enQueue (q, color, V); q->front;) {
        Node vtx = deQueue (q), *p;
        for (p = G->vertex[vtx.index].adjList; p; p = p->next) {
            if (0 == G->vertex[p->index].color) {
                failure |= 1 << 1;
                G->vertex[p->index].color = offset + 1 - vtx.label;
                G->ancestor->vertex[p->label].color = offset + 1 - vtx.label;
                enQueue (q, 1 - vtx.label, p->index);
            } else if (offset + vtx.label == G->vertex[p->index].color) {
                for (; q->front; deQueue (q));
                return failure |= 1;
            }
        }
    }
    return failure;
}

int twoColor (Graph *G, int i) {
    int j, failure = 0;
    for (j = 0; j < G->numVertices; j++) {
        if (0 == G->vertex[j].color) {
            failure = BFS (G, j, i);
            if (failure & 1)
                return failure;
        }
    }
    return failure;
}

int greedyColor (Graph *G, int i) {
    int j, k, n = G->numVertices, N = G->ancestor->numVertices;
    int *freq = scalloc (1 + N, sizeof (int), 1);
    for (j = 0; j < n; j++) {
        if (0 == G->vertex[j].color) {
            Node *p = G->ancestor->vertex[G->vertex[j].label].adjList;
            memset (1 + freq, 0, N * sizeof (int));
            for (; p; ++freq[G->ancestor->vertex[p->label].color], p = p->next);
            for (k = 1; k <= i && k <= N && freq[k]; k++);
            G->ancestor->vertex[G->vertex[j].label].color = k;
            G->vertex[j].color = k;
            i += k == 1 + i;
        }
    }
    free (freq);
    return i;
}

int kColor (int k, Graph *G, int i) {
    static int failure = 0;
    failure ^= G == G->ancestor ? failure : 0;

    if (k <= 2) {
        failure |= twoColor (G, i);
        return failure & 1 ? 0 : 1 + (failure >> 1);
    }

    if (k >= G->numVertices) {
        int j;
        for (j = i; j < G->numVertices; G->vertex[j].color = j, j++);
        return failure & 1 ? 0 : j;
    }

    int j, m, n = G->numVertices;
    for (j = 0; j < n; G->vertex[j].index = j, j++);
    Vertex* sortedVtx = scalloc (n, sizeof (Vertex), 1);
    memcpy (sortedVtx, G->vertex, n * sizeof (Vertex));
    qsort (sortedVtx, n, sizeof (Vertex), deg_comparator);
    G->adjArray = scalloc (sortedVtx[0].degree, sizeof (Node), 1);

    double exp = 1 - 1. / (k - 1);
    int fun = (int) ceil (pow (n, exp));
    for (j = 0; j < n && sortedVtx[j].degree >= fun; j++) {
        if (G->ancestor->vertex[sortedVtx[j].label].deleted)
            continue;

        Node *p = G->vertex[sortedVtx[j].index].adjList;
        for (m = 0; p; p = p->next)
            if (!G->ancestor->vertex[p->label].deleted)
                G->adjArray[m++] = *p;

        Graph *H = subgraphInduced (G, m);
        i = 1 + kColor (k - 1, H, i);

        if (failure & 1) {
            deleteGraph (&H);
            break;
        }

        G->ancestor->vertex[sortedVtx[j].label].color = i;
        G->vertex[sortedVtx[j].index].color = i;
        G->ancestor->vertex[sortedVtx[j].label].deleted = 1;
        for (m = 0; m < H->numVertices; m++)
            G->ancestor->vertex[G->adjArray[m].label].deleted = 1;
        deleteGraph (&H);
    }

    free (G->adjArray);
    free (sortedVtx);
    return failure & 1 ? 0 : greedyColor (G, i);
}

Graph *parseGraph (FILE *ifp, const char attach) {
    ifp = !ifp ? stdin : ifp;
    int src, n = 0x800, len = 0x80000;
    Graph *graph = createGraph (n);
    char *line = scalloc (len, sizeof (char), 1);

    switch (attach) {
        case 0:
            while (fgets (line, len, ifp)) {
                int src, dst;
                char *pch = tokenize (line);
                if (1 != sscanf (pch, "%u", &src))
                    continue;

                graph->vertex[src - 1].label = src - 1;
                if (++graph->numVertices == n)
                    n = reshapeAdjList (graph, n, n << 1);

                for (pch = tokenize (NULL); pch; pch = tokenize (NULL))
                    if (1 == sscanf (pch, "%u", &dst))
                        addEdge (graph, src - 1, dst - 1, dst - 1);
            }
            break;
        default:
            for (src = 0; fgets (line, len, ifp); src++) {
                int dst;
                char *pch;

                graph->vertex[src].label = src;
                if (++graph->numVertices == n)
                    n = reshapeAdjList (graph, n, n << 1);

                for (pch = tokenize (line); pch; pch = tokenize (NULL))
                    if (1 == sscanf (pch, "%u", &dst))
                        addEdge (graph, src, dst, dst);
            }
            break;
    }

    fclose (ifp);
    free (line);
    n = reshapeAdjList (graph, n, graph->numVertices);
    return graph->ancestor = graph;
}

int main (int argc, char *argv[]) {
    atexit (timeTaken);
    begin = clock ();

    FILE *ifp = argc < 2 ? stdin : fopen (argv[1], "r");
    Graph *graph = parseGraph (ifp, ATTACH);

    int i, j = 1, colors;
    do {
        j = min (j << 1, graph->numVertices);
        colors = kColor (j, graph, 1);
        for (i = 0; !colors && i < graph->numVertices; graph->vertex[i++].color = 0);
    } while (!colors && j < graph->numVertices);

    printColors (colors, graph);
    deleteGraph (&graph);
    return 0;
}
