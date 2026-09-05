#include "common.h"
#include "parser.h"
#include "pathfinder.h"
#include "scheduler.h"
#include "formatter.h"
#include <assert.h>

/**
 * @file test_integration.c
 * @brief Teste de integração ponta a ponta e validação estrita contra o gabarito oficial.
 */

int main(void) {
    printf("=== Executando Teste de Integração: Validação Cega contra data/saida.out ===\n");

    FILE *in = fopen("data/entrada.in", "r");
    assert(in != NULL);

    /* Executa o pipeline e grava a saída em data/meu_resultado.out */
    FILE *out = fopen("data/meu_resultado.out", "w");
    assert(out != NULL);

    int conjunto_id = 1;
    Dataset *ds = NULL;

    while ((ds = parser_read_next_dataset(in, conjunto_id)) != NULL) {
        formatter_print_dataset_header(out, ds->id);

        for (int i = 0; i < ds->num_artists; i++) {
            ArtistQuery *artist = &ds->artists[i];

            PathResult *path_res = pathfinder_find_shortest_path(
                ds->graph,
                ds->map,
                artist->origin_id,
                artist->dest_id
            );

            TourSchedule *tour = scheduler_build_tour(
                path_res,
                ds->graph,
                artist->show_duration
            );

            formatter_print_artist_tour(out, artist->name, tour, ds->map);

            tour_schedule_destroy(tour);
            path_result_destroy(path_res);
        }

        dataset_destroy(ds);
        conjunto_id++;
    }

    fclose(in);
    fclose(out);

    /* Comparação byte a byte contra data/saida.out */
    FILE *f_expected = fopen("data/saida.out", "rb");
    FILE *f_actual = fopen("data/meu_resultado.out", "rb");
    assert(f_expected != NULL);
    assert(f_actual != NULL);

    fseek(f_expected, 0, SEEK_END);
    long len_expected = ftell(f_expected);
    fseek(f_expected, 0, SEEK_SET);

    fseek(f_actual, 0, SEEK_END);
    long len_actual = ftell(f_actual);
    fseek(f_actual, 0, SEEK_SET);

    printf("Tamanho esperado: %ld bytes | Tamanho gerado: %ld bytes\n", len_expected, len_actual);
    assert(len_expected == len_actual);

    int byte_exp, byte_act;
    long pos = 0;
    while ((byte_exp = fgetc(f_expected)) != EOF) {
        byte_act = fgetc(f_actual);
        if (byte_exp != byte_act) {
            fprintf(stderr, "Divergência detectada no byte %ld: esperado '%c' (0x%02X), obtido '%c' (0x%02X)\n",
                    pos, byte_exp, byte_exp, byte_act, byte_act);
            assert(byte_exp == byte_act);
        }
        pos++;
    }

    fclose(f_expected);
    fclose(f_actual);

    printf("[PASS] Comparação estrita byte-a-byte idêntica (Diff = 0)!\n");
    printf("=== Teste de Integração Concluído com Sucesso! ===\n");
    return 0;
}
