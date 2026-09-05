#include "map.h"

CityMap* map_create(int capacity) {
    if (capacity <= 0) {
        return NULL;
    }

    CityMap *map = (CityMap*) malloc(sizeof(CityMap));
    if (!map) {
        return NULL;
    }

    map->names = (char**) malloc(sizeof(char*) * capacity);
    if (!map->names) {
        free(map);
        return NULL;
    }

    for (int i = 0; i < capacity; i++) {
        map->names[i] = NULL;
    }

    map->capacity = capacity;
    map->size = 0;
    return map;
}

int map_insert(CityMap *map, const char *name) {
    if (!map || !name) {
        return -1;
    }

    /* Verifica se a cidade já está cadastrada */
    int existing_id = map_get_id(map, name);
    if (existing_id != -1) {
        return existing_id;
    }

    /* Verifica se há capacidade disponível */
    if (map->size >= map->capacity) {
        return -1;
    }

    size_t len = strlen(name);
    char *name_copy = (char*) malloc(len + 1);
    if (!name_copy) {
        return -1;
    }
    strcpy(name_copy, name);

    int new_id = map->size;
    map->names[new_id] = name_copy;
    map->size++;

    return new_id;
}

int map_get_id(const CityMap *map, const char *name) {
    if (!map || !name) {
        return -1;
    }

    for (int i = 0; i < map->size; i++) {
        if (map->names[i] && strcmp(map->names[i], name) == 0) {
            return i;
        }
    }

    return -1;
}

const char* map_get_name(const CityMap *map, int id) {
    if (!map || id < 0 || id >= map->size) {
        return NULL;
    }

    return map->names[id];
}

void map_destroy(CityMap *map) {
    if (!map) {
        return;
    }

    if (map->names) {
        for (int i = 0; i < map->size; i++) {
            if (map->names[i]) {
                free(map->names[i]);
            }
        }
        free(map->names);
    }

    free(map);
}
