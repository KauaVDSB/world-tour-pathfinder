#include "common.h"
#include "map.h"
#include "graph.h"
#include <assert.h>

/**
 * @file test_graph.c
 * @brief Testes unitários para o TAD Tabela de Símbolos (CityMap) e Grafo (Graph).
 */

static void test_map_operations(void) {
    CityMap *map = map_create(5);
    assert(map != NULL);

    int id0 = map_insert(map, "Londres");
    int id1 = map_insert(map, "Milao");
    int id2 = map_insert(map, "Paris");

    assert(id0 == 0);
    assert(id1 == 1);
    assert(id2 == 2);

    /* Inserção duplicada deve retornar o mesmo ID */
    assert(map_insert(map, "Milao") == 1);

    /* Busca por nome */
    assert(map_get_id(map, "Londres") == 0);
    assert(map_get_id(map, "Paris") == 2);
    assert(map_get_id(map, "Inexistente") == -1);

    /* Busca por ID */
    assert(strcmp(map_get_name(map, 0), "Londres") == 0);
    assert(strcmp(map_get_name(map, 1), "Milao") == 0);
    assert(map_get_name(map, 99) == NULL);

    map_destroy(map);
    printf("[PASS] test_map_operations\n");
}

static void test_graph_operations(void) {
    Graph *g = graph_create(3);
    assert(g != NULL);
    assert(g->num_vertices == 3);

    /* Adiciona arestas: 0 -> 1 (peso 5), 0 -> 2 (peso 2) */
    graph_add_edge(g, 0, 1, 5);
    graph_add_edge(g, 0, 2, 2);

    /* Verifica adjacências de 0 */
    const AdjNode *adj = graph_get_adj(g, 0);
    assert(adj != NULL);

    int count = 0;
    bool found_1 = false;
    bool found_2 = false;

    while (adj) {
        count++;
        if (adj->dest == 1 && adj->weight == 5) found_1 = true;
        if (adj->dest == 2 && adj->weight == 2) found_2 = true;
        adj = adj->next;
    }

    assert(count == 2);
    assert(found_1 && found_2);

    /* Verifica que 1 não possui aresta para 0 (grafo direcionado) */
    assert(graph_get_adj(g, 1) == NULL);

    graph_destroy(g);
    printf("[PASS] test_graph_operations\n");
}

int main(void) {
    printf("=== Executando Bateria de Testes: Map & Graph ===\n");
    test_map_operations();
    test_graph_operations();
    printf("=== Todos os testes passaram com sucesso! ===\n");
    return 0;
}
