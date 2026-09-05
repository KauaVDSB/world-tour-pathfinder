#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "common.h"
#include "map.h"
#include "graph.h"
#include "pathfinder.h"

/**
 * @file scheduler.h
 * @brief Simulador temporal e agendador da turnê de artistas.
 * 
 * Calcula o momento de chegada em cada cidade ao longo da rota ótima
 * e converte horas em dias conforme a especificação do problema.
 */

/**
 * @struct DaySchedule
 * @brief Armazena os eventos de show programados para um dia específico.
 */
typedef struct {
    int day_number;         /**< Número do dia da turnê (1, 2, ...) */
    int *city_ids;          /**< Vetor com os IDs das cidades com show agendado neste dia */
    int count;              /**< Quantidade de apresentações no dia (0 se dia vazio) */
    int capacity;           /**< Capacidade alocada para o vetor */
} DaySchedule;

/**
 * @struct TourSchedule
 * @brief Cronograma completo com todos os dias consecutivos do início ao fim da turnê.
 */
typedef struct {
    DaySchedule *days;      /**< Vetor de dias contíguos do Dia 1 ao Dia final */
    int total_days;         /**< Quantidade total de dias até o destino */
    int total_flight_time;  /**< Tempo total gasto em voos */
    bool cancelled;         /**< true se a turnê foi cancelada (destino inalcançável) */
} TourSchedule;

/**
 * @brief Constrói o cronograma temporal da turnê para um artista.
 * @param path_res Resultado do algoritmo de Dijkstra modificado.
 * @param g Grafo das rotas aéreas.
 * @param show_duration Duração X em horas de cada apresentação.
 * @return Ponteiro para TourSchedule alocado dinamicamente.
 */
TourSchedule* scheduler_build_tour(const PathResult *path_res, const Graph *g, int show_duration);

/**
 * @brief Libera toda a memória associada à estrutura TourSchedule.
 * @param tour Ponteiro para o cronograma a ser destruído.
 */
void tour_schedule_destroy(TourSchedule *tour);

#endif /* SCHEDULER_H */
