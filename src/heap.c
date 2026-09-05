#include "heap.h"

static bool heap_less(HeapNode a, HeapNode b) {
    if (a.dist != b.dist) {
        return a.dist < b.dist;
    }
    return a.hops < b.hops;
}

static void heap_swap(HeapNode *a, HeapNode *b) {
    HeapNode temp = *a;
    *a = *b;
    *b = temp;
}

static void heap_swim(MinHeap *heap, int k) {
    while (k > 0) {
        int parent = (k - 1) / 2;
        if (heap_less(heap->data[k], heap->data[parent])) {
            heap_swap(&heap->data[k], &heap->data[parent]);
            k = parent;
        } else {
            break;
        }
    }
}

static void heap_sink(MinHeap *heap, int k) {
    while (2 * k + 1 < heap->size) {
        int left = 2 * k + 1;
        int right = 2 * k + 2;
        int smallest = left;

        if (right < heap->size && heap_less(heap->data[right], heap->data[left])) {
            smallest = right;
        }

        if (heap_less(heap->data[smallest], heap->data[k])) {
            heap_swap(&heap->data[k], &heap->data[smallest]);
            k = smallest;
        } else {
            break;
        }
    }
}

MinHeap* heap_create(int capacity) {
    if (capacity <= 0) {
        capacity = 16;
    }

    MinHeap *heap = (MinHeap*) malloc(sizeof(MinHeap));
    if (!heap) {
        return NULL;
    }

    heap->data = (HeapNode*) malloc(sizeof(HeapNode) * capacity);
    if (!heap->data) {
        free(heap);
        return NULL;
    }

    heap->size = 0;
    heap->capacity = capacity;
    return heap;
}

void heap_push(MinHeap *heap, HeapNode node) {
    if (!heap) {
        return;
    }

    /* Redimensiona caso atinja a capacidade */
    if (heap->size >= heap->capacity) {
        int new_capacity = heap->capacity * 2;
        HeapNode *new_data = (HeapNode*) realloc(heap->data, sizeof(HeapNode) * new_capacity);
        if (!new_data) {
            return;
        }
        heap->data = new_data;
        heap->capacity = new_capacity;
    }

    heap->data[heap->size] = node;
    heap_swim(heap, heap->size);
    heap->size++;
}

HeapNode heap_pop(MinHeap *heap) {
    HeapNode min_node = { -1, INF_TIME, INF_TIME };
    if (!heap || heap->size == 0) {
        return min_node;
    }

    min_node = heap->data[0];
    heap->size--;
    if (heap->size > 0) {
        heap->data[0] = heap->data[heap->size];
        heap_sink(heap, 0);
    }

    return min_node;
}

bool heap_is_empty(const MinHeap *heap) {
    return (!heap || heap->size == 0);
}

void heap_destroy(MinHeap *heap) {
    if (!heap) {
        return;
    }

    if (heap->data) {
        free(heap->data);
    }
    free(heap);
}
