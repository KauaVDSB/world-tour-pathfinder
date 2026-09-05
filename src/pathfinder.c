#include "pathfinder.h"
#include "heap.h"

PathResult* pathfinder_find_shortest_path(const Graph *g, const CityMap *map, int origin_id, int dest_id) {
    if (!g || !map) {
        return NULL;
    }

    PathResult *result = (PathResult*) malloc(sizeof(PathResult));
    if (!result) {
        return NULL;
    }

    result->path = NULL;
    result->path_length = 0;
    result->total_flight_time = 0;
    result->reachable = false;

    int V = g->num_vertices;
    if (origin_id < 0 || origin_id >= V || dest_id < 0 || dest_id >= V) {
        return result;
    }

    int *dist = (int*) malloc(sizeof(int) * V);
    int *hops = (int*) malloc(sizeof(int) * V);
    int *parent = (int*) malloc(sizeof(int) * V);
    bool *visited = (bool*) malloc(sizeof(bool) * V);

    if (!dist || !hops || !parent || !visited) {
        if (dist) free(dist);
        if (hops) free(hops);
        if (parent) free(parent);
        if (visited) free(visited);
        return result;
    }

    for (int i = 0; i < V; i++) {
        dist[i] = INF_TIME;
        hops[i] = INF_TIME;
        parent[i] = -1;
        visited[i] = false;
    }

    dist[origin_id] = 0;
    hops[origin_id] = 0;
    parent[origin_id] = -1;

    MinHeap *heap = heap_create(V * 2);
    if (!heap) {
        free(dist);
        free(hops);
        free(parent);
        free(visited);
        return result;
    }

    HeapNode start_node = { origin_id, 0, 0 };
    heap_push(heap, start_node);

    while (!heap_is_empty(heap)) {
        HeapNode top = heap_pop(heap);
        int u = top.vertex;

        if (visited[u]) {
            continue;
        }
        visited[u] = true;

        if (u == dest_id) {
            break;
        }

        const AdjNode *curr = graph_get_adj(g, u);
        while (curr) {
            int v = curr->dest;
            int weight = curr->weight;

            if (!visited[v]) {
                int new_dist = dist[u] + weight;
                int new_hops = hops[u] + 1;
                bool is_better = false;

                if (new_dist < dist[v]) {
                    is_better = true;
                } else if (new_dist == dist[v]) {
                    if (new_hops < hops[v]) {
                        is_better = true;
                    } else if (new_hops == hops[v]) {
                        /* Critério de desempate 2: Ordem lexicográfica completa da rota */
                        int *curr_path = (int*) malloc(sizeof(int) * (hops[v] + 1));
                        int *cand_path = (int*) malloc(sizeof(int) * (new_hops + 1));

                        if (curr_path && cand_path) {
                            /* Reconstrói caminho atual até v */
                            int temp_curr = v;
                            for (int i = hops[v]; i >= 0; i--) {
                                curr_path[i] = temp_curr;
                                temp_curr = parent[temp_curr];
                            }

                            /* Reconstrói caminho candidato até v (u + v) */
                            cand_path[new_hops] = v;
                            int temp_cand = u;
                            for (int i = hops[u]; i >= 0; i--) {
                                cand_path[i] = temp_cand;
                                temp_cand = parent[temp_cand];
                            }

                            /* Comparação lexicográfica vértice a vértice */
                            for (int i = 0; i <= new_hops; i++) {
                                const char *name_cand = map_get_name(map, cand_path[i]);
                                const char *name_curr = map_get_name(map, curr_path[i]);
                                int cmp = strcmp(name_cand, name_curr);
                                if (cmp < 0) {
                                    is_better = true;
                                    break;
                                } else if (cmp > 0) {
                                    is_better = false;
                                    break;
                                }
                            }
                        }

                        if (curr_path) free(curr_path);
                        if (cand_path) free(cand_path);
                    }
                }

                if (is_better) {
                    dist[v] = new_dist;
                    hops[v] = new_hops;
                    parent[v] = u;
                    HeapNode next_node = { v, new_dist, new_hops };
                    heap_push(heap, next_node);
                }
            }

            curr = curr->next;
        }
    }

    if (dist[dest_id] < INF_TIME) {
        result->reachable = true;
        result->total_flight_time = dist[dest_id];
        result->path_length = hops[dest_id] + 1;
        result->path = (int*) malloc(sizeof(int) * result->path_length);

        if (result->path) {
            int curr = dest_id;
            for (int i = result->path_length - 1; i >= 0; i--) {
                result->path[i] = curr;
                curr = parent[curr];
            }
        }
    }

    heap_destroy(heap);
    free(dist);
    free(hops);
    free(parent);
    free(visited);

    return result;
}

void path_result_destroy(PathResult *res) {
    if (!res) {
        return;
    }

    if (res->path) {
        free(res->path);
    }
    free(res);
}
