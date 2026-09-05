#include "graph.h"

Graph* graph_create(int num_vertices) {
    if (num_vertices <= 0) {
        return NULL;
    }

    Graph *g = (Graph*) malloc(sizeof(Graph));
    if (!g) {
        return NULL;
    }

    g->num_vertices = num_vertices;
    g->adj_lists = (AdjNode**) malloc(sizeof(AdjNode*) * num_vertices);
    if (!g->adj_lists) {
        free(g);
        return NULL;
    }

    for (int i = 0; i < num_vertices; i++) {
        g->adj_lists[i] = NULL;
    }

    return g;
}

void graph_add_edge(Graph *g, int src, int dest, int weight) {
    if (!g || src < 0 || src >= g->num_vertices || dest < 0 || dest >= g->num_vertices) {
        return;
    }

    AdjNode *new_node = (AdjNode*) malloc(sizeof(AdjNode));
    if (!new_node) {
        return;
    }

    new_node->dest = dest;
    new_node->weight = weight;
    new_node->next = g->adj_lists[src];
    g->adj_lists[src] = new_node;
}

const AdjNode* graph_get_adj(const Graph *g, int u) {
    if (!g || u < 0 || u >= g->num_vertices) {
        return NULL;
    }

    return g->adj_lists[u];
}

void graph_destroy(Graph *g) {
    if (!g) {
        return;
    }

    if (g->adj_lists) {
        for (int i = 0; i < g->num_vertices; i++) {
            AdjNode *curr = g->adj_lists[i];
            while (curr) {
                AdjNode *temp = curr;
                curr = curr->next;
                free(temp);
            }
        }
        free(g->adj_lists);
    }

    free(g);
}
