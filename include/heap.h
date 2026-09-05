#ifndef HEAP_H
#define HEAP_H

#include "common.h"

/**
 * @file heap.h
 * @brief Min-Heap / Fila de Prioridade para o Algoritmo de Dijkstra.
 */

/**
 * @struct HeapNode
 * @brief Elemento armazenado na Min-Heap.
 */
typedef struct {
    int vertex;     /**< ID do vértice */
    int dist;       /**< Custo acumulado de voo */
    int hops;       /**< Quantidade de arestas/saltos percorridos */
} HeapNode;

/**
 * @struct MinHeap
 * @brief Estrutura da Min-Heap com alocação dinâmica.
 */
typedef struct {
    HeapNode *data; /**< Vetor dinâmico de nós */
    int size;       /**< Quantidade atual de elementos */
    int capacity;   /**< Capacidade máxima alocada */
} MinHeap;

/**
 * @brief Cria e inicializa uma Min-Heap com capacidade especificada.
 * @param capacity Capacidade inicial da fila de prioridade.
 * @return Ponteiro para a MinHeap ou NULL em caso de erro.
 */
MinHeap* heap_create(int capacity);

/**
 * @brief Insere um novo nó na Min-Heap mantendo a propriedade de heap mínima.
 * @param heap Ponteiro para a MinHeap.
 * @param node Nó a ser inserido.
 */
void heap_push(MinHeap *heap, HeapNode node);

/**
 * @brief Remove e retorna o elemento de menor prioridade (menor custo / menores saltos).
 * @param heap Ponteiro para a MinHeap.
 * @return HeapNode com o menor elemento.
 */
HeapNode heap_pop(MinHeap *heap);

/**
 * @brief Verifica se a fila de prioridade está vazia.
 * @param heap Ponteiro para a MinHeap.
 * @return true se estiver vazia, false caso contrário.
 */
bool heap_is_empty(const MinHeap *heap);

/**
 * @brief Libera toda a memória associada à Min-Heap.
 * @param heap Ponteiro para a MinHeap a ser destruída.
 */
void heap_destroy(MinHeap *heap);

#endif /* HEAP_H */
