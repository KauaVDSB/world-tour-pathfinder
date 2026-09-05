#ifndef MAP_H
#define MAP_H

#include "common.h"

/**
 * @file map.h
 * @brief TAD Tabela de Símbolos para mapeamento bidirecional Cidade <-> ID numérico.
 * 
 * Permite indexação direta O(1) de vértices em estruturas de dados como grafos e filas de prioridade.
 */

typedef struct {
    char **names;       /**< Vetor de strings indexado pelo ID numérico da cidade */
    int capacity;       /**< Capacidade máxima alocada para o conjunto */
    int size;           /**< Quantidade atual de cidades registradas */
} CityMap;

/**
 * @brief Cria e inicializa uma nova tabela de símbolos para cidades.
 * @param capacity Capacidade máxima de cidades esperada.
 * @return Ponteiro para a tabela criada ou NULL em falha de alocação.
 */
CityMap* map_create(int capacity);

/**
 * @brief Insere uma cidade no mapa caso ainda não exista.
 * @param map Ponteiro para a tabela de símbolos.
 * @param name Nome da cidade a ser inserida.
 * @return ID numérico atribuído à cidade (0 <= ID < size) ou -1 em caso de erro.
 */
int map_insert(CityMap *map, const char *name);

/**
 * @brief Obtém o ID numérico associado ao nome da cidade.
 * @param map Ponteiro para a tabela de símbolos.
 * @param name Nome da cidade buscada.
 * @return ID da cidade ou -1 se não for encontrada.
 */
int map_get_id(const CityMap *map, const char *name);

/**
 * @brief Obtém o nome da cidade a partir do seu ID numérico.
 * @param map Ponteiro para a tabela de símbolos.
 * @param id ID numérico da cidade.
 * @return String com o nome da cidade ou NULL se o ID for inválido.
 */
const char* map_get_name(const CityMap *map, int id);

/**
 * @brief Libera toda a memória dinâmica associada à tabela de símbolos.
 * @param map Ponteiro para a tabela a ser destruída.
 */
void map_destroy(CityMap *map);

#endif /* MAP_H */
