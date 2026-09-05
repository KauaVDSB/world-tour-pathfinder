#include "common.h"
#include "parser.h"
#include <assert.h>

/**
 * @file test_parser.c
 * @brief Testes unitários e de integração para o leitor de entrada (parser).
 */

int main(void) {
    printf("=== Executando Bateria de Testes: Parser ===\n");

    FILE *f = fopen("data/entrada.in", "r");
    assert(f != NULL);

    /* Conjunto #1 */
    Dataset *ds1 = parser_read_next_dataset(f, 1);
    assert(ds1 != NULL);
    assert(ds1->id == 1);
    assert(ds1->num_cities == 5);
    assert(strcmp(map_get_name(ds1->map, 0), "Londres") == 0);
    assert(strcmp(map_get_name(ds1->map, 4), "Berlim") == 0);
    assert(ds1->num_artists == 3);
    assert(strcmp(ds1->artists[0].name, "U2") == 0);
    assert(ds1->artists[0].show_duration == 10);
    assert(ds1->artists[0].origin_id == 2); /* Paris */
    assert(ds1->artists[0].dest_id == 1);   /* Milao */
    assert(strcmp(ds1->artists[2].name, "Madonna") == 0);
    assert(ds1->artists[2].show_duration == 1);

    /* Conjunto #2 */
    Dataset *ds2 = parser_read_next_dataset(f, 2);
    assert(ds2 != NULL);
    assert(ds2->id == 2);
    assert(ds2->num_cities == 4);
    assert(strcmp(map_get_name(ds2->map, 0), "Recife") == 0);
    assert(strcmp(map_get_name(ds2->map, 3), "Florianopolis") == 0);
    assert(ds2->num_artists == 3);
    assert(strcmp(ds2->artists[0].name, "ReginaldoRossi") == 0);
    assert(ds2->artists[0].show_duration == 32);

    /* Conjunto #3 deve retornar NULL (EOF) */
    Dataset *ds3 = parser_read_next_dataset(f, 3);
    assert(ds3 == NULL);

    fclose(f);

    /* Liberação da memória */
    dataset_destroy(ds1);
    dataset_destroy(ds2);

    printf("[PASS] test_parser_operations\n");
    printf("=== Todos os testes do parser passaram com sucesso! ===\n");
    return 0;
}
