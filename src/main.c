#include "common.h"
#include "parser.h"
#include "pathfinder.h"
#include "scheduler.h"
#include "formatter.h"

/**
 * @file main.c
 * @brief Ponto de entrada do sistema Turnê Mundial de Artistas.
 * 
 * Disciplina: Algoritmos e Estruturas de Dados II (AED II) - UFAPE
 * Docente: Prof. Daliton da Silva
 * Discente: Kaua Vinicius
 * 
 * Pipeline de execução:
 * 1. Parser: Leitura de múltiplos conjuntos de dados até EOF.
 * 2. Pathfinder: Dijkstra customizado com desempates (Custo -> Hops -> Lexicográfico).
 * 3. Scheduler: Simulação temporal de chegadas e conversão em dias de apresentação.
 * 4. Formatter: Saída estrita no formato canônico da disciplina.
 * 5. Clean-up: Desalocação completa de memória a cada ciclo (zero memory leaks).
 */

int main(void) {
    int conjunto_id = 1;
    Dataset *ds = NULL;

    while ((ds = parser_read_next_dataset(stdin, conjunto_id)) != NULL) {
        /* Cabeçalho do conjunto */
        formatter_print_dataset_header(stdout, ds->id);

        /* Processamento individual para cada artista do conjunto */
        for (int i = 0; i < ds->num_artists; i++) {
            ArtistQuery *artist = &ds->artists[i];

            /* Executa o Dijkstra modificado */
            PathResult *path_res = pathfinder_find_shortest_path(
                ds->graph,
                ds->map,
                artist->origin_id,
                artist->dest_id
            );

            /* Constrói o cronograma diário da turnê */
            TourSchedule *tour = scheduler_build_tour(
                path_res,
                ds->graph,
                artist->show_duration
            );

            /* Emite a saída rigorosamente formatada */
            formatter_print_artist_tour(stdout, artist->name, tour, ds->map);

            /* Liberação da memória da consulta */
            tour_schedule_destroy(tour);
            path_result_destroy(path_res);
        }

        /* Liberação completa do conjunto atual */
        dataset_destroy(ds);
        conjunto_id++;
    }

    return STATUS_OK;
}
