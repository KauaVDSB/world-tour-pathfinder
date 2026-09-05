#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <limits.h>

/**
 * @file common.h
 * @brief Definições globais, constantes de domínio e macros do projeto.
 * 
 * Disciplina: Algoritmos e Estruturas de Dados II (AED II) - UFAPE
 * Docente: Prof. Daliton da Silva
 * Discente: Kaua Vinicius
 */

/* Tamanho máximo do nome de uma cidade (20 caracteres + '\0') */
#define MAX_CITY_NAME 21

/* Tamanho máximo do nome de um artista (30 caracteres + '\0') */
#define MAX_ARTIST_NAME 31

/* Valor representativo de distância infinita no algoritmo de Dijkstra */
#define INF_TIME 1000000000

/* Código de status de execução */
typedef enum {
    STATUS_OK = 0,
    STATUS_ERROR = 1
} Status;

#endif /* COMMON_H */
