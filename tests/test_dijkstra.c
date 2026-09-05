#include "common.h"
#include "map.h"
#include "graph.h"
#include "heap.h"
#include "pathfinder.h"
#include <assert.h>

/**
 * @file test_dijkstra.c
 * @brief Testes unitários para a Fila de Prioridade (MinHeap) e Dijkstra Customizado.
 */

static void test_heap_operations(void) {
    MinHeap *heap = heap_create(4);
    assert(heap != NULL);
    assert(heap_is_empty(heap));

    heap_push(heap, (HeapNode){ 1, 10, 2 });
    heap_push(heap, (HeapNode){ 2, 5, 1 });
    heap_push(heap, (HeapNode){ 3, 5, 3 });
    heap_push(heap, (HeapNode){ 4, 2, 4 });

    assert(!heap_is_empty(heap));

    /* Menor custo primário: nó 4 (dist 2) */
    HeapNode n1 = heap_pop(heap);
    assert(n1.vertex == 4 && n1.dist == 2);

    /* Empate em dist 5: nó 2 (hops 1) vs nó 3 (hops 3) -> nó 2 deve sair primeiro */
    HeapNode n2 = heap_pop(heap);
    assert(n2.vertex == 2 && n2.dist == 5 && n2.hops == 1);

    HeapNode n3 = heap_pop(heap);
    assert(n3.vertex == 3 && n3.dist == 5 && n3.hops == 3);

    HeapNode n4 = heap_pop(heap);
    assert(n4.vertex == 1 && n4.dist == 10);

    assert(heap_is_empty(heap));
    heap_destroy(heap);
    printf("[PASS] test_heap_operations\n");
}

static void test_tie_breakers(void) {
    /* Teste do critério 1: Menor número de hops */
    CityMap *map1 = map_create(5);
    int a = map_insert(map1, "A");
    int b = map_insert(map1, "B");
    int c = map_insert(map1, "C");
    int d = map_insert(map1, "D");
    int e = map_insert(map1, "E");

    Graph *g1 = graph_create(5);
    /* Rota 1: A -> B -> C -> D (dist 20, 3 hops) */
    graph_add_edge(g1, a, b, 5);
    graph_add_edge(g1, b, c, 5);
    graph_add_edge(g1, c, d, 10);
    /* Rota 2: A -> E -> D (dist 20, 2 hops) */
    graph_add_edge(g1, a, e, 10);
    graph_add_edge(g1, e, d, 10);

    PathResult *res1 = pathfinder_find_shortest_path(g1, map1, a, d);
    assert(res1->reachable);
    assert(res1->total_flight_time == 20);
    assert(res1->path_length == 3); /* A, E, D */
    assert(res1->path[1] == e);
    path_result_destroy(res1);
    graph_destroy(g1);
    map_destroy(map1);

    /* Teste do critério 2: Ordem lexicográfica em igualdade de custo e hops */
    CityMap *map2 = map_create(4);
    int o = map_insert(map2, "Origem");
    int c_alfa = map_insert(map2, "Alfa");
    int c_beta = map_insert(map2, "Beta");
    int dest = map_insert(map2, "Destino");

    Graph *g2 = graph_create(4);
    /* Origem -> Alfa -> Destino (dist 10, hops 2) */
    graph_add_edge(g2, o, c_alfa, 5);
    graph_add_edge(g2, c_alfa, dest, 5);
    /* Origem -> Beta -> Destino (dist 10, hops 2) */
    graph_add_edge(g2, o, c_beta, 5);
    graph_add_edge(g2, c_beta, dest, 5);

    PathResult *res2 = pathfinder_find_shortest_path(g2, map2, o, dest);
    assert(res2->reachable);
    assert(res2->total_flight_time == 10);
    assert(res2->path_length == 3);
    assert(res2->path[1] == c_alfa); /* "Alfa" < "Beta" */
    path_result_destroy(res2);
    graph_destroy(g2);
    map_destroy(map2);

    printf("[PASS] test_tie_breakers\n");
}

int main(void) {
    printf("=== Executando Bateria de Testes: Heap & Pathfinder ===\n");
    test_heap_operations();
    test_tie_breakers();
    printf("=== Todos os testes do Dijkstra passaram com sucesso! ===\n");
    return 0;
}
