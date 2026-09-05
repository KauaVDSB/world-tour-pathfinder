#include "parser.h"

Dataset* parser_read_next_dataset(FILE *in, int conjunto_id) {
    if (!in) {
        return NULL;
    }

    int num_cities = 0;
    if (fscanf(in, "%d", &num_cities) != 1) {
        return NULL; /* EOF ou erro de leitura */
    }

    if (num_cities <= 0) {
        return NULL;
    }

    Dataset *ds = (Dataset*) malloc(sizeof(Dataset));
    if (!ds) {
        return NULL;
    }

    ds->id = conjunto_id;
    ds->num_cities = num_cities;
    ds->artists = NULL;
    ds->num_artists = 0;

    /* Cria a tabela de símbolos para N cidades */
    ds->map = map_create(num_cities);
    if (!ds->map) {
        free(ds);
        return NULL;
    }

    /* Leitura dos nomes das N cidades */
    for (int i = 0; i < num_cities; i++) {
        char city_name[MAX_CITY_NAME];
        if (fscanf(in, "%20s", city_name) != 1) {
            dataset_destroy(ds);
            return NULL;
        }
        map_insert(ds->map, city_name);
    }

    /* Cria o grafo direcionado com N vértices */
    ds->graph = graph_create(num_cities);
    if (!ds->graph) {
        dataset_destroy(ds);
        return NULL;
    }

    /* Leitura das conexões para cada uma das N cidades */
    for (int i = 0; i < num_cities; i++) {
        int num_conns = 0;
        if (fscanf(in, "%d", &num_conns) != 1) {
            dataset_destroy(ds);
            return NULL;
        }

        for (int c = 0; c < num_conns; c++) {
            char dest_name[MAX_CITY_NAME];
            int flight_hours = 0;
            if (fscanf(in, "%20s %d", dest_name, &flight_hours) != 2) {
                dataset_destroy(ds);
                return NULL;
            }

            int dest_id = map_get_id(ds->map, dest_name);
            if (dest_id != -1) {
                graph_add_edge(ds->graph, i, dest_id, flight_hours);
            }
        }
    }

    /* Leitura da quantidade de artistas M */
    int num_artists = 0;
    if (fscanf(in, "%d", &num_artists) != 1) {
        dataset_destroy(ds);
        return NULL;
    }

    ds->num_artists = num_artists;
    if (num_artists > 0) {
        ds->artists = (ArtistQuery*) malloc(sizeof(ArtistQuery) * num_artists);
        if (!ds->artists) {
            dataset_destroy(ds);
            return NULL;
        }

        for (int j = 0; j < num_artists; j++) {
            if (fscanf(in, "%30s %d %20s %20s",
                       ds->artists[j].name,
                       &ds->artists[j].show_duration,
                       ds->artists[j].origin_city,
                       ds->artists[j].dest_city) != 4) {
                dataset_destroy(ds);
                return NULL;
            }

            ds->artists[j].origin_id = map_get_id(ds->map, ds->artists[j].origin_city);
            ds->artists[j].dest_id = map_get_id(ds->map, ds->artists[j].dest_city);
        }
    }

    return ds;
}

void dataset_destroy(Dataset *ds) {
    if (!ds) {
        return;
    }

    if (ds->map) {
        map_destroy(ds->map);
    }

    if (ds->graph) {
        graph_destroy(ds->graph);
    }

    if (ds->artists) {
        free(ds->artists);
    }

    free(ds);
}
