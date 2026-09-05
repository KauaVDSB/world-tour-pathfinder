#include "scheduler.h"

static int get_flight_weight(const Graph *g, int u, int v) {
    const AdjNode *curr = graph_get_adj(g, u);
    while (curr) {
        if (curr->dest == v) {
            return curr->weight;
        }
        curr = curr->next;
    }
    return 0;
}

TourSchedule* scheduler_build_tour(const PathResult *path_res, const Graph *g, int show_duration) {
    TourSchedule *tour = (TourSchedule*) malloc(sizeof(TourSchedule));
    if (!tour) {
        return NULL;
    }

    tour->days = NULL;
    tour->total_days = 0;
    tour->total_flight_time = 0;
    tour->cancelled = false;

    if (!path_res || !path_res->reachable || path_res->path_length <= 0) {
        tour->cancelled = true;
        return tour;
    }

    int L = path_res->path_length;
    int *city_days = (int*) malloc(sizeof(int) * L);
    if (!city_days) {
        free(tour);
        return NULL;
    }

    /* Cidade de origem: show no Dia 1 na hora 0 */
    city_days[0] = 1;
    int flight_accumulated = 0;

    for (int i = 1; i < L; i++) {
        int u = path_res->path[i - 1];
        int v = path_res->path[i];
        int flight_time = get_flight_weight(g, u, v);
        flight_accumulated += flight_time;

        /* Momento de chegada = soma dos voos até aqui + (i shows anteriores * X) */
        int arrival_time = flight_accumulated + (i * show_duration);
        city_days[i] = (arrival_time / 24) + 1;
    }

    int max_day = city_days[L - 1];
    tour->total_days = max_day;
    tour->total_flight_time = path_res->total_flight_time;

    tour->days = (DaySchedule*) malloc(sizeof(DaySchedule) * max_day);
    if (!tour->days) {
        free(city_days);
        free(tour);
        return NULL;
    }

    for (int d = 0; d < max_day; d++) {
        tour->days[d].day_number = d + 1;
        tour->days[d].count = 0;
        tour->days[d].capacity = 4;
        tour->days[d].city_ids = (int*) malloc(sizeof(int) * 4);
    }

    for (int i = 0; i < L; i++) {
        int day_idx = city_days[i] - 1;
        DaySchedule *ds = &tour->days[day_idx];

        if (ds->count >= ds->capacity) {
            ds->capacity *= 2;
            int *new_ids = (int*) realloc(ds->city_ids, sizeof(int) * ds->capacity);
            if (new_ids) {
                ds->city_ids = new_ids;
            }
        }

        ds->city_ids[ds->count] = path_res->path[i];
        ds->count++;
    }

    free(city_days);
    return tour;
}

void tour_schedule_destroy(TourSchedule *tour) {
    if (!tour) {
        return;
    }

    if (tour->days) {
        for (int d = 0; d < tour->total_days; d++) {
            if (tour->days[d].city_ids) {
                free(tour->days[d].city_ids);
            }
        }
        free(tour->days);
    }

    free(tour);
}
