#include "formatter.h"

void formatter_print_dataset_header(FILE *out, int conjunto_id) {
    if (!out) {
        return;
    }
    fprintf(out, "Conjunto #%d\n", conjunto_id);
}

void formatter_print_artist_tour(FILE *out, const char *artist_name, const TourSchedule *tour, const CityMap *map) {
    if (!out || !artist_name || !tour || !map) {
        return;
    }

    /* Linha 1: Nome do artista */
    fprintf(out, "%s\n", artist_name);

    /* Inacessibilidade: imprime 'turne cancelada' seguido de linha em branco */
    if (tour->cancelled) {
        fprintf(out, "turne cancelada\n\n");
        return;
    }

    /* Linhas diárias: loop do Dia 1 até o Dia final */
    for (int d = 0; d < tour->total_days; d++) {
        const DaySchedule *day = &tour->days[d];

        if (day->count == 0) {
            /* Dia sem apresentação: estritamente "numeroDia:" sem espaço ao final */
            fprintf(out, "%d:\n", day->day_number);
        } else {
            /* Dia com apresentação: "numeroDia: cidade1 cidade2" */
            fprintf(out, "%d:", day->day_number);
            for (int c = 0; c < day->count; c++) {
                const char *city_name = map_get_name(map, day->city_ids[c]);
                if (city_name) {
                    fprintf(out, " %s", city_name);
                }
            }
            fprintf(out, "\n");
        }
    }

    /* Última linha do artista: apenas o total de horas gastas em voo + linha em branco */
    fprintf(out, "%d\n\n", tour->total_flight_time);
}
