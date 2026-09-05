#include "common.h"
#include "map.h"
#include "graph.h"
#include "pathfinder.h"
#include "scheduler.h"
#include <assert.h>

/**
 * @file test_scheduler.c
 * @brief Testes unitários para a simulação cronológica da turnê.
 */

int main(void) {
    printf("=== Executando Bateria de Testes: Scheduler ===\n");

    /* Cenário Reginaldo Rossi: Recife -> Rio -> Brasilia, X = 32 */
    CityMap *map = map_create(4);
    int recife = map_insert(map, "Recife");
    int rio = map_insert(map, "Rio");
    int brasilia = map_insert(map, "Brasilia");
    map_insert(map, "Florianopolis");

    Graph *g = graph_create(4);
    graph_add_edge(g, recife, rio, 18);
    graph_add_edge(g, rio, brasilia, 8);

    PathResult *path_res = pathfinder_find_shortest_path(g, map, recife, brasilia);
    assert(path_res->reachable);
    assert(path_res->total_flight_time == 26);
    assert(path_res->path_length == 3);

    TourSchedule *tour = scheduler_build_tour(path_res, g, 32);
    assert(tour != NULL);
    assert(!tour->cancelled);
    assert(tour->total_days == 4);
    assert(tour->total_flight_time == 26);

    /* Dia 1: Recife */
    assert(tour->days[0].day_number == 1);
    assert(tour->days[0].count == 1);
    assert(tour->days[0].city_ids[0] == recife);

    /* Dia 2: Vazio (sem show) */
    assert(tour->days[1].day_number == 2);
    assert(tour->days[1].count == 0);

    /* Dia 3: Rio */
    assert(tour->days[2].day_number == 3);
    assert(tour->days[2].count == 1);
    assert(tour->days[2].city_ids[0] == rio);

    /* Dia 4: Brasilia */
    assert(tour->days[3].day_number == 4);
    assert(tour->days[3].count == 1);
    assert(tour->days[3].city_ids[0] == brasilia);

    tour_schedule_destroy(tour);
    path_result_destroy(path_res);
    graph_destroy(g);
    map_destroy(map);

    printf("[PASS] test_scheduler_reginaldo_rossi\n");
    printf("=== Todos os testes do scheduler passaram com sucesso! ===\n");
    return 0;
}
