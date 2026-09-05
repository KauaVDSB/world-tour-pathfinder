#ifndef GRAPH_H
#define GRAPH_H

#include "common.h"

/**
 * @file graph.h
 * @brief TAD Grafo Direcionado e Ponderado representado por Lista de Adjacência.
 * 
 * Modela as rotas aéreas entre cidades, onde os pesos das arestas representam
 * o tempo de voo direto em horas.
 */

/**
 * @struct AdjNode
 * @brief Nó da lista encadeada de adjacências de um vértice.
 */
typedef struct AdjNode {
    int dest;               /**< ID numérico do vértice de destino */
    int weight;             /**< Peso da aresta (tempo de voo em horas) */
    struct AdjNode *next;   /**< Ponteiro para o próximo nó adjacente */
} AdjNode;

/**
 * @struct Graph
 * @brief Estrutura do Grafo direcionado.
 */
typedef struct {
    int num_vertices;       /**< Quantidade total de vértices |V| */
    AdjNode **adj_lists;    /**< Vetor de ponteiros para as listas encadeadas de adjacência */
} Graph;

/**
 * @brief Cria e inicializa um grafo direcionado com V vértices.
 * @param num_vertices Número de vértices do grafo.
 * @return Ponteiro para a estrutura Graph alocada ou NULL em falha.
 */
Graph* graph_create(int num_vertices);

/**
 * @brief Adiciona uma aresta direcionada ponderada u -> v com peso w.
 * @param g Ponteiro para o grafo.
 * @param src ID do vértice de origem (u).
 * @param dest ID do vértice de destino (v).
 * @param weight Tempo de voo em horas (w >= 0).
 */
void graph_add_edge(Graph *g, int src, int dest, int weight);

/**
 * @brief Obtém o início da lista de adjacências do vértice u.
 * @param g Ponteiro para o grafo.
 * @param u ID do vértice.
 * @return Ponteiro para o primeiro AdjNode ou NULL se não houver vizinhos.
 */
const AdjNode* graph_get_adj(const Graph *g, int u);

/**
 * @brief Libera toda a memória alocada para as listas de adjacência e a estrutura do grafo.
 * @param g Ponteiro para o grafo a ser destruído.
 */
void graph_destroy(Graph *g);

#endif /* GRAPH_H */
