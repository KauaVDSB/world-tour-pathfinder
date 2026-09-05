#ifndef FORMATTER_H
#define FORMATTER_H

#include "common.h"
#include "map.h"
#include "scheduler.h"

/**
 * @file formatter.h
 * @brief Formatador canônico de saída para conformidade estrita com o gabarito oficial.
 * 
 * Disciplina: Algoritmos e Estruturas de Dados II (AED II) - UFAPE
 * Docente: Prof. Daliton da Silva
 * Discente: Kaua Vinicius
 * 
 * Regras de formatação garantidas:
 * 1. "Conjunto #K"
 * 2. Nome do artista na linha inicial
 * 3. Shows diários: "dia: cidade1 cidade2"
 * 4. Dias sem show: "dia:" (sem espaços após os dois pontos)
 * 5. Última linha do artista: total de horas de voo
 * 6. Destino inalcançável: "turne cancelada"
 * 7. Linha em branco separadora após cada artista
 */

/**
 * @brief Imprime o cabeçalho do conjunto no formato "Conjunto #K\n".
 * @param out Fluxo de saída (ex: stdout).
 * @param conjunto_id Número do conjunto atual.
 */
void formatter_print_dataset_header(FILE *out, int conjunto_id);

/**
 * @brief Imprime o cronograma formatado da turnê de um artista.
 * @param out Fluxo de saída (ex: stdout).
 * @param artist_name Nome do artista.
 * @param tour Estrutura TourSchedule contendo os dias calculados.
 * @param map Tabela de símbolos para consulta de nomes de cidades.
 */
void formatter_print_artist_tour(FILE *out, const char *artist_name, const TourSchedule *tour, const CityMap *map);

#endif /* FORMATTER_H */
