# Exercise 2 — SList

Implementazione comparativa di tre liste semplicemente concatenate in C++17. Il progetto non studia soltanto l'API di una lista: mette a confronto ownership, lifetime, layout di memoria, invalidazione degli iteratori, free list e prevedibilità dei costi.

La specifica progettuale completa è in [DESIGN.md](DESIGN.md).

## Implementazioni

| Tipo | Storage dei nodi | Allocazioni dei nodi | Caratteristica principale |
|---|---|---:|---|
| `SList<T>` | nodi separati allocati dinamicamente | una per inserimento | ownership manuale e Rule of 5 |
| `SListArray<T>` | `std::vector<Node>` con link a indici | crescita dinamica del vector | capacità dinamica e migliore locality potenziale |
| `FixedSList<T, N>` | `std::array<Node, N>` incorporato | nessuna dopo la costruzione | capacità fissa e costi più prevedibili |

Le tre versioni espongono la stessa API essenziale: `empty`, `size`, `front`, `push_front`, `pop_front`, `clear`, iterazione mutabile/const e semantiche di copy/move appropriate alla rappresentazione.

## Contratti principali

- `front()` e `pop_front()` su lista vuota sollevano `std::out_of_range`.
- `FixedSList::push_front()` su lista piena solleva `std::length_error`.
- `FixedSList::try_push_front()` restituisce `false` se la lista è piena e non modifica lo stato della lista.
- `FixedSList::full()` indica se tutti gli slot sono attivi.
- `Validate()` controlla le invarianti interne; `false` indica una struttura corrotta o incoerente, non un normale stato di lista piena/vuota.

`FixedSList<T, N>` richiede che `T` sia default-constructible: ogni slot dell'array contiene un `T` già costruito, anche quando è logicamente libero.

## Build e avvio

Il progetto è header-only perché tutte le classi sono template.

### Test funzionali e regressione

```bash
g++ -std=c++17 -Wall -Wextra -Wpedantic main.cpp -o main
./main
```

`main.cpp` contiene soltanto i test funzionali e di regressione.

### Benchmark

Per raccogliere tempi confrontabili, abilita le ottimizzazioni:

```bash
g++ -std=c++17 -O2 -Wall -Wextra -Wpedantic benchmark.cpp -o benchmark
./benchmark
```

## Benchmark

`Benchmark.h` espone il template `BenchmarkList`, quindi la sua definizione rimane nell'header ed è visibile alla translation unit che lo istanzia.

Ogni benchmark ripete tre fasi:

1. `build`: inserimenti tramite `push_front`;
2. `traversal`: iterazione const con checksum;
3. `pop-all`: rimozione di tutti gli elementi tramite `pop_front`.

Gli scenari confrontano `SList`, `SListArray`, `SListArray` con `reserve` e `FixedSList`. I risultati non stabiliscono che un contenitore sia universalmente più veloce: dipendono da workload, dimensione/tipo di `T`, allocator, compilatore, flag di ottimizzazione, cache e hardware. Inoltre il benchmark misura le operazioni dopo la costruzione del container; non include il costo iniziale di creare e inizializzare tutti gli slot di `FixedSList`.

### Risultati osservati

Rilevazione eseguita con `elementCount = 10'000`, `repetitions = 100` e build ottimizzata (`g++ -O2`) su ambiente Windows. I valori sono tempi aggregati in microsecondi.

| Implementazione | Build (µs) | Traversal (µs) | Pop-all (µs) |
|---|---:|---:|---:|
| `SList raw` | 26887 | 2088 | 12888 |
| `SListArray` | 24774 | 2247 | 1627 |
| `SListArray reserved` | 6561 | 2159 | 1505 |
| `FixedSList` | 1469 | 1445 | 1498 |

In questo scenario il costo delle allocazioni e deallocazioni per nodo penalizza la lista raw; `reserve` evita le riallocazioni previste del vector durante il popolamento; `FixedSList` non deve allocare nodi dinamicamente. La tabella descrive questa misurazione, non una classifica assoluta dei container.

## Struttura

```text
cpp-slist/
├── SList.h
├── SListArray.h
├── FixedSList.h
├── Benchmark.h
├── main.cpp
├── benchmark.cpp
├── DESIGN.md
└── README.md
```

## Limiti volutamente fuori scope

- inserimento in coda;
- rimozione arbitraria;
- iteratori completi STL (`operator->`, post-incremento, categorie di iteratore);
- slot senza `T` attivo in `FixedSList`.

Questi aspetti richiederebbero nuovi contratti di invalidazione o una gestione esplicita di storage e lifetime, e sono quindi rimandati a un approfondimento successivo.
