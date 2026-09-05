#ifndef PARSER_H
#define PARSER_H

#include "common.h"
#include "map.h"
#include "graph.h"

/**
 * @file parser.h
 * @brief Leitor e analisador léxico/sintático dos conjuntos de dados de entrada.
 * 
 * Suporta múltiplos conjuntos até EOF com validação e alocação dinâmica.
 */

/**
 * @struct ArtistQuery
 * @brief Estrutura que armazena os dados de consulta de turnê de um artista.
 */
typedef struct {
    char name[MAX_ARTIST_NAME];         /**< Nome do artista */
    int show_duration;                  /**< Duração do show em horas (X) */
    char origin_city[MAX_CITY_NAME];    /**< Nome da cidade de origem */
    char dest_city[MAX_CITY_NAME];      /**< Nome da cidade de destino */
    int origin_id;                      /**< ID numérico da cidade de origem */
    int dest_id;                        /**< ID numérico da cidade de destino */
} ArtistQuery;

/**
 * @struct Dataset
 * @brief Encapsula um conjunto completo de cidades, conexões e artistas.
 */
typedef struct {
    int id;                             /**< Identificador sequencial do conjunto (1, 2, ...) */
    int num_cities;                     /**< Número total de cidades (N) */
    CityMap *map;                       /**< Tabela de símbolos (Nome <-> ID) */
    Graph *graph;                       /**< Grafo direcionado ponderado */
    int num_artists;                    /**< Número de artistas (M) */
    ArtistQuery *artists;               /**< Vetor com as consultas dos artistas */
} Dataset;

/**
 * @brief Lê o próximo conjunto de dados do fluxo de entrada fornecido.
 * @param in Ponteiro do arquivo (ex: stdin).
 * @param conjunto_id Número do conjunto atual.
 * @return Ponteiro para o Dataset preenchido, ou NULL se atingir o EOF.
 */
Dataset* parser_read_next_dataset(FILE *in, int conjunto_id);

/**
 * @brief Desaloca completamente a memória utilizada por um Dataset.
 * @param ds Ponteiro para o Dataset a ser destruído.
 */
void dataset_destroy(Dataset *ds);

#endif /* PARSER_H */
