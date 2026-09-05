# Turnê Mundial de Artistas (World Tour Pathfinder)

[![C99 Standard](https://img.shields.io/badge/C-C99-blue.svg)](https://en.wikipedia.org/wiki/C99)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](LICENSE)
[![Build Status](https://img.shields.io/badge/build-passing-brightgreen.svg)]()

Sistema de otimização e agendamento de rotas aéreas para turnês internacionais de artistas utilizando o **Algoritmo de Dijkstra Modificado** com critérios de desempate em cascata e simulação temporal contínua.

---

## 1. Identificação Acadêmica

- **Instituição:** Universidade Federal do Agreste de Pernambuco (UFAPE)
- **Curso:** Bacharelado em Ciência da Computação (BCC)
- **Disciplina:** Algoritmos e Estruturas de Dados II (AED II)
- **Docente:** Prof. Daliton da Silva
- **Discente:** Kaua Vinicius

---

## 2. Visão Geral e Arquitetura

O projeto foi concebido seguindo princípios de **Clean Code**, **Modularização**, **SOLID** e gerenciamento estrito de recursos em C99, garantindo ausência total de vazamento de memória (*zero memory leaks*).

### Estrutura de Diretórios

```
world-tour-pathfinder/
├── data/
│   ├── entrada.in          # Casos de teste oficiais da disciplina
│   └── saida.out           # Gabarito oficial para validação de diff
├── include/
│   ├── common.h            # Constantes de domínio, limites e macros globais
│   ├── map.h               # TAD Tabela de Símbolos (Cidade <-> ID numérico)
│   ├── graph.h             # TAD Grafo Direcionado Ponderado (Lista de Adjacência)
│   ├── heap.h              # Min-Heap / Fila de Prioridade genérica
│   ├── pathfinder.h        # Motor do Algoritmo de Dijkstra com 3 desempates
│   ├── scheduler.h         # Simulador temporal e agendamento de apresentações
│   └── formatter.h         # Formatador canônico de saída (zero trailing spaces)
├── src/
│   ├── map.c               # Implementação da Tabela de Símbolos
│   ├── graph.c             # Implementação do Grafo com Lista de Adjacência
│   ├── heap.c              # Implementação da Min-Heap
│   ├── parser.c            # Leitor de múltiplos datasets até EOF
│   ├── pathfinder.c        # Relaxamento prioritário e desempate lexicográfico
│   ├── scheduler.c         # Cálculo de momentos de chegada e conversão em dias
│   ├── formatter.c         # Emissão padronizada da saída
│   └── main.c              # Ponto de entrada e orquestração do pipeline
├── tests/
│   ├── test_graph.c        # Testes unitários para Map e Graph
│   ├── test_parser.c       # Testes unitários para o leitor e EOF
│   ├── test_dijkstra.c     # Testes para Heap e critérios de desempate
│   ├── test_scheduler.c    # Testes para a conversão de horas em dias
│   └── test_integration.c  # Validação cega ponta a ponta contra saida.out
├── Makefile                # Automação de compilação e testes (Linux/GCC)
├── .gitignore              # Configuração de exclusão de binários e artefatos
└── README.md               # Documentação técnica e acadêmica do projeto
```

---

## 3. Decisões de Projeto e Modelagem Teórica

### A. Representação do Grafo: Lista de Adjacência
- **Vértices ($V$):** As cidades são identificadas inicialmente por strings de até 20 caracteres sem espaços e mapeadas em tempo $O(1)$ para índices inteiros contíguos de $0$ a $N-1$ através do TAD `CityMap`.
- **Arestas Direcionadas ($E$):** Cada conexão direta representa um voo de $u \to v$ com peso $W(u, v)$ correspondente ao **tempo de voo em horas** ($W \ge 0$).
- **Justificativa da Lista de Adjacência:** Em malhas aéreas onde o grafo é tipicamente esparso ($|E| \ll |V|^2$), a Lista de Adjacência consome espaço linear $O(V + E)$ em oposição a $O(V^2)$ da Matriz de Adjacência. Além disso, a iteração sobre os vizinhos diretos de um vértice durante o relaxamento de Dijkstra ocorre em $O(\text{grau}(u))$.

### B. Algoritmo de Dijkstra e Critérios de Desempate em Cascata
Para determinar a rota ótima da cidade de origem até o destino, o sistema utiliza o Algoritmo de Dijkstra auxiliado por uma **Min-Heap** com as seguintes regras de decisão em ordem estrita de precedência:

1. **Critério Primário — Menor Tempo Total de Voo:** Minimiza a soma dos pesos das arestas percorridas ($\sum W$).
2. **Critério de Desempate 1 — Menor Número de Cidades Visitadas:** Em caso de empate no tempo total de voo, prioriza o trajeto com menor contagem de saltos/arestas.
3. **Critério de Desempate 2 — Ordem Lexicográfica Estrita da Rota:** Se persistir o empate em tempo e número de cidades, compara-se lexicograficamente a sequência de nomes das cidades ao longo de todo o percurso, da origem ao destino ($P_{\text{novo}} <_{\text{lex}} P_{\text{atual}}$).

### C. Simulação Temporal da Turnê (Scheduler)
- Cada show consome $X$ horas.
- O início da turnê ocorre na **Hora 0 (meia-noite do Dia 1)** na cidade de origem. O show na origem é sempre no **Dia 1**.
- O dia de cada apresentação é determinado pelo momento da **CHEGADA** na cidade:
  $$\text{Chegada}_k = \sum_{i=1}^{k-1} W(c_{i-1}, c_i) + (k - 1) \cdot X$$
  $$\text{Dia} = \left\lfloor \frac{\text{Chegada}_k}{24} \right\rfloor + 1$$
- Intervalos onde a viagem ou a permanência anterior impedem apresentações em determinado dia geram linhas de dia ocioso no formato `dia:` (sem espaços extras).

---

## 4. Tratamento de Entradas e Casos de Borda

### Ressalva sobre Nomes Compostos (`Reginaldo Rossi` vs `ReginaldoRossi`)
> [!NOTE]
> No texto descritivo do enunciado da disciplina consta a menção `Reginaldo Rossi 32...` com espaço separador. Contudo, o arquivo oficial de avaliação `data/entrada.in` e o respectivo gabarito `data/saida.out` fornecidos pelo docente utilizam estritamente o identificador contíguo **`ReginaldoRossi`**, respeitando a especificação formal de strings alfanuméricas contíguas de até 20 caracteres. O sistema foi calibrado para ler e processar tokens conforme o arquivo oficial. Caso em execuções futuras seja fornecida uma entrada contendo espaços no nome do artista, o ajuste deve ser feito em `src/parser.c` (ou no arquivo de entrada substituindo o espaço pelo formato unificado).

### Linhas em Branco e Transição Inter-Conjuntos
O arquivo `data/entrada.in` contém uma linha em branco separando os conjuntos (Linha 11) e no encerramento (Linha 21). O nosso leitor em `src/parser.c` utiliza especificadores `%d` e `%s` que consomem nativamente todos os delimitadores de whitespace (`\r`, `\n`, `\t`, ` `), garantindo transição transparente entre instâncias e encerramento seguro ao detectar `EOF`.

---

## 5. Análise de Complexidade Assintótica

| Módulo / Operação | Complexidade Temporal | Complexidade Espacial | Justificativa Teórica |
| :--- | :---: | :---: | :--- |
| **Construção do Grafo** | $O(V + E)$ | $O(V + E)$ | Alocação das listas de adjacência e inserção $O(1)$ na cabeça. |
| **Dijkstra com Min-Heap** | $O((V + E) \log V + V \cdot L)$ | $O(V + E)$ | Extração do mínimo e inserção na heap em $O(\log V)$; desempate lexicográfico percorre o caminho de tamanho $L \le V$. |
| **Scheduler da Turnê** | $O(L + D)$ | $O(D)$ | Varredura linear do caminho de $L$ cidades e alocação de $D$ dias. |
| **Formatador de Saída** | $O(D + L)$ | $O(1)$ | Impressão sequencial formatada sem alocações adicionais. |
| **Complexidade Global** | $O(K \cdot M \cdot (V + E) \log V)$ | $O(V + E)$ | Para $K$ conjuntos com $M$ artistas cada. |

---

## 6. Instruções de Compilação, Execução e Testes

### Pré-requisitos
- Compilador C compatível com C99 (`gcc` ou `clang`).
- GNU Make (opcional, para uso do Makefile em ambientes Unix/Linux).

### Compilação Direta com Flags Estritas
```bash
gcc -Wall -Wextra -pedantic -std=c99 -O2 -Iinclude src/*.c -o turne -lm
```

### Execução com Redirecionamento de I/O
```bash
./turne < data/entrada.in > data/meu_resultado.out
```

### Validação Cega de Diff (Comparação Byte a Byte)
No Linux/macOS:
```bash
diff -u data/saida.out data/meu_resultado.out
```
*Se o comando não emitir nenhuma linha de saída, a conformidade é de 100%.*

No Windows PowerShell:
```powershell
Get-Content data/entrada.in | ./turne.exe | Set-Content -NoNewline -Encoding utf8 data/meu_resultado.out
Compare-Object (Get-Content data/saida.out) (Get-Content data/meu_resultado.out)
```

### Execução da Bateria de Testes Unitários e Integração
```bash
# Teste de Map e Grafo
gcc -Wall -Wextra -pedantic -std=c99 -O2 -Iinclude src/map.c src/graph.c tests/test_graph.c -o test_graph -lm && ./test_graph

# Teste do Parser
gcc -Wall -Wextra -pedantic -std=c99 -O2 -Iinclude src/map.c src/graph.c src/parser.c tests/test_parser.c -o test_parser -lm && ./test_parser

# Teste do Dijkstra e Desempates
gcc -Wall -Wextra -pedantic -std=c99 -O2 -Iinclude src/map.c src/graph.c src/heap.c src/pathfinder.c tests/test_dijkstra.c -o test_dijkstra -lm && ./test_dijkstra

# Teste do Scheduler
gcc -Wall -Wextra -pedantic -std=c99 -O2 -Iinclude src/map.c src/graph.c src/heap.c src/pathfinder.c src/scheduler.c tests/test_scheduler.c -o test_scheduler -lm && ./test_scheduler

# Teste de Integração Ponta a Ponta
gcc -Wall -Wextra -pedantic -std=c99 -O2 -Iinclude src/map.c src/graph.c src/parser.c src/heap.c src/pathfinder.c src/scheduler.c src/formatter.c tests/test_integration.c -o test_integration -lm && ./test_integration
```

### Verificação de Vazamento de Memória (Valgrind)
No ambiente Linux/WSL:
```bash
valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./turne < data/entrada.in
```
*Resultado esperado: `All heap blocks were freed -- no leaks are possible`.*

---

## 7. Histórico de Commits (Conventional Commits)

O repositório foi construído de maneira estritamente incremental seguindo o padrão Conventional Commits:

| Commit | Mensagem | Escopo e Entregas |
| :---: | :--- | :--- |
| `1` | `chore: initial project structure and build configuration` | Inicialização do repositório, licença MIT e gitignore base. |
| `2` | `chore: setup project structure, build configuration and datasets` | Estrutura de diretórios, dados oficiais `data/entrada.in` e `data/saida.out`, cabeçalho `common.h` e Makefile. |
| `3` | `feat(graph): implement symbol table map and weighted directed graph` | TAD `CityMap` para indexação $O(1)$ e Grafo direcionado ponderado com Lista de Adjacência e desalocação em profundidade. |
| `4` | `feat(parser): implement multi-dataset input reader and tokenization` | Leitor sequencial de instâncias com suporte a múltiplos datasets até EOF e descarte robusto de quebras de linha. |
| `5` | `feat(dijkstra): implement priority queue and shortest path with custom tie-breakers` | Min-Heap e Dijkstra modificado com 3 regras de desempate (custo de voo $\to$ número de cidades $\to$ ordem lexicográfica). |
| `6` | `feat(tour): implement timeline simulation and day calculation` | Cálculo de chegadas cronológicas $\text{Chegada}_k = \sum W + (k-1)X$ e conversão precisa de horas para dias. |
| `7` | `feat(output): format output matching strict requirements and main driver` | Formatador canônico sem trailing spaces, pipeline em `src/main.c` e liberação completa de memória. |
| `8` | `test: validate with entrada.in and saida.out` | Teste de integração automatizado com validação cega byte a byte (Diff = 0). |
| `9` | `docs: add comprehensive academic README and architecture documentation` | Documentação completa com modelagem matemática, complexidade assintótica e guia de execução. |
