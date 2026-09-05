#ifndef PATHFINDER_H
#define PATHFINDER_H

#include "common.h"
#include "map.h"
#include "graph.h"

/**
 * @file pathfinder.h
 * @brief Motor do Algoritmo de Dijkstra modificado com critérios de desempate estritos.
 * 
 * Aplica relaxamento com 3 regras de decisão em cascata:
 * 1. Menor tempo total de voo
 * 2. Menor número de cidades visitadas (saltos)
 * 3. Ordem lexicográfica estrita da sequência de cidades
 */

/**
 * @struct PathResult
 * @brief Resultado da busca de caminho mínimo entre origem e destino.
 */
typedef struct {
    int *path;              /**< Vetor de IDs das cidades ordenadas da origem ao destino */
    int path_length;        /**< Quantidade de cidades visitadas no trajeto */
    int total_flight_time;  /**< Soma do tempo de voo de todas as arestas utilizadas */
    bool reachable;         /**< true se o destino for alcançável a partir da origem */
} PathResult;

/**
 * @brief Calcula a rota ótima para uma consulta de artista aplicando os critérios de desempate.
 * @param g Grafo direcionado ponderado.
 * @param map Tabela de símbolos das cidades.
 * @param origin_id ID numérico da cidade de partida.
 * @param dest_id ID numérico da cidade de chegada.
 * @return Ponteiro para PathResult alocado dinamicamente.
 */
PathResult* pathfinder_find_shortest_path(const Graph *g, const CityMap *map, int origin_id, int dest_id);

/**
 * @brief Libera toda a memória associada a um PathResult.
 * @param res Ponteiro para o resultado a ser destruído.
 */
void path_result_destroy(PathResult *res);

#endif /* PATHFINDER_H */
