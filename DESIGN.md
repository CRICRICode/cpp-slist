# Exercise 2 — SList: Design

## Scope

L'esercizio consiste nell'implementare una lista semplicemente concatenata come class template e nello studiare come la stessa API cambia comportamento in base alla rappresentazione dello storage. Le tre versioni devono esporre lo stesso comportamento osservabile per le operazioni comuni, pur avendo costi, limiti e failure mode differenti.

L'obiettivo non è soltanto ottenere una lista funzionante, ma poter spiegare ownership, allocazioni, locality, invalidazione, capacità e prevedibilità di ogni scelta. La baseline comprende le tre implementazioni, i test comuni e una review di correttezza; gli extra restano separati fino a baseline stabile.

## Implementazioni previste

- `SList<T>`: nodi collegati tramite puntatori e allocati dinamicamente. È la versione di riferimento per ownership manuale, lifetime dei nodi e Rule of 5.
- `SListArray<T>`: nodi conservati in uno `std::vector`, con collegamenti basati su indici. Mira a migliorare la locality mantenendo capacità dinamica; deve gestire le riallocazioni del vector senza lasciare link interni dangling.
- `FixedSList<T, N>`: nodi conservati in un array fisso di dimensione `N`, con collegamenti basati su indici e free list. Mira a rendere prevedibili memoria e tempo delle operazioni, accettando una capacità massima.

## API comune proposta

La baseline dell'API deve comprendere:

- costruzione di una lista vuota;
- verifica dello stato (`empty`) e del numero di elementi attivi (`size` in O(1));
- accesso al primo elemento, in versione modificabile e di sola lettura;
- inserimento e rimozione in testa;
- svuotamento completo della lista;
- iterazione dalla testa alla fine, sia mutabile sia const;
- copia, copy assignment, move e move assignment con semantica corretta per la rappresentazione scelta.

Le firme C++ corrispondenti sono fissate nella sezione successiva. Operazioni aggiuntive come inserimento in coda, rimozione arbitraria o reverse non fanno parte della baseline finché non ne viene verificata la necessità.

## API signatures

Le firme seguenti descrivono il contratto comune. `List` è un segnaposto: nelle implementazioni reali sarà sostituito da `SList<T>`, `SListArray<T>` oppure `FixedSList<T, N>`.

```cpp
template <typename T>
class List
{
public:
    class Iterator;
    class ConstIterator;

    List();
    ~List();
    List(const List& other);
    List& operator=(const List& other);
    List(List&& other);
    List& operator=(List&& other);

    bool empty() const;
    std::size_t size() const;

    T& front();
    const T& front() const;

    void push_front(const T& value);
    void push_front(T&& value);
    void pop_front();
    void clear();

    Iterator begin();
    Iterator end();
    ConstIterator begin() const;
    ConstIterator end() const;
};
```

Gli iteratori espongono il minimo contratto necessario a traversal manuale e range-based for. Il modo in cui vengono costruiti e lo stato interno che conservano sono dettagli privati della rappresentazione.

```cpp
class Iterator
{
public:
    T& operator*() const;
    Iterator& operator++();
    bool operator==(const Iterator& other) const;
    bool operator!=(const Iterator& other) const;
};

class ConstIterator
{
public:
    const T& operator*() const;
    ConstIterator& operator++();
    bool operator==(const ConstIterator& other) const;
    bool operator!=(const ConstIterator& other) const;
};
```

`push_front(const T&)` inserisce copiando un lvalue; `push_front(T&&)` inserisce spostando un valore spostabile. Entrambe hanno la stessa semantica per la lista e differiscono soltanto nel modo in cui viene inizializzato il valore del nuovo nodo.

### Motivazioni delle scelte API

- La lista è un class template perché deve poter contenere valori di tipo `T` senza duplicare l'implementazione per ogni tipo.
- Le special member functions fanno parte del contratto comune: nella `SList` raw richiedono una gestione esplicita della catena posseduta; nelle versioni basate su `std::vector` o array possono invece sfruttare, quando corretto, la Rule of 0.
- `empty` e `size` permettono al chiamante di osservare lo stato senza esporre nodi o dettagli di storage. `size` è O(1) grazie a `m_size`.
- Le due overload di `front` rispettano la const correctness: una lista mutabile permette di modificare il primo valore, una lista const consente soltanto lettura.
- Le overload di `push_front` distinguono lvalue e rvalue. Il chiamante può evitare una copia quando trasferisce un valore spostabile, senza cambiare la semantica dell'operazione.
- `pop_front` e `clear` appartengono alla baseline perché la lista deve poter rimuovere la testa e rilasciare o restituire correttamente tutti i nodi o slot posseduti.
- `begin` e `end`, nelle versioni mutabile e const, permettono traversal esplicito e range-based for senza esporre la rappresentazione interna.
- Gli iteratori offrono dereferenziazione, pre-incremento e confronto: è il minimo necessario per percorrere una lista semplicemente concatenata in modo idiomatico.
- Inserimento in coda, rimozione arbitraria e reverse restano fuori dalla baseline: aggiungerli ora allargherebbe la superficie da testare e le decisioni di invalidazione senza essere richiesti dalla traccia.

## Ownership e storage

`SList<T>` possiede direttamente i nodi creati dinamicamente: crea, collega, copia, trasferisce e distrugge la catena. I puntatori usati per collegare i nodi non sono ownership esterna.

`SListArray<T>` possiede un `std::vector` che a sua volta possiede lo storage contiguo dei nodi. I collegamenti fra nodi sono indici logici, non puntatori nel buffer del vector. Mantiene una free list di indici per gli slot inattivi: `pop_front` non usa `vector::erase`, ma restituisce lo slot rimosso alla free list; `push_front` riutilizza prima uno slot libero e il vector cresce soltanto quando la free list è vuota.

`FixedSList<T, N>` possiede lo storage incorporato nell'oggetto stesso: non richiede crescita dinamica per i nodi. La lista attiva e la free list sono entrambe gestite dalla struttura; chi usa la lista non possiede i singoli nodi né deve liberarli.

La baseline della `FixedSList` adotta direttamente la rappresentazione `Node<T> m_data[N]`. Di conseguenza, ogni slot costruisce il proprio `T value` quando viene costruita la lista, anche se lo slot è logicamente libero: `T` deve quindi essere default-constructible. `pop_front` restituisce lo slot alla free list senza distruggere il suo `T`; un inserimento successivo gli assegna un nuovo valore. Una versione con slot privi di `T` attivo viene rimandata a un approfondimento futuro su storage e lifetime.

## Invarianti

Ogni lista deve mantenere una catena attiva valida, terminata correttamente. Una lista vuota non ha una testa valida; una lista non vuota ha una testa da cui ogni nodo attivo è raggiungibile una sola volta. Nessun nodo attivo può comparire due volte o formare cicli involontari.

Ogni lista mantiene un contatore `m_size`, pari al numero di nodi attivi. Il numero di nodi raggiungibili dalla testa deve coincidere con `m_size`; il contatore viene aggiornato soltanto dopo un inserimento o una rimozione riusciti, viene azzerato da `clear` e viene trattato coerentemente da copy e move.

Nelle versioni a indici, ogni link deve essere un indice valido oppure la sentinella di fine. In `SListArray`, `m_data.size()` non coincide necessariamente con `m_size`, perché il vector contiene anche gli slot della free list; ogni slot già presente nel vector appartiene esattamente alla catena attiva oppure alla free list. Nella `FixedSList`, ogni slot deve appartenere esattamente alla lista attiva oppure alla free list e deve sempre valere `m_size <= N`.

## Contratti e failure policy

Le operazioni pubbliche non devono produrre Undefined Behaviour per input o stato gestibili. La struttura dati non deve stampare messaggi, aggiornare UI o decidere la logica di gioco: comunica l'esito al chiamante attraverso il contratto della propria API.

`front` e `pop_front` su una lista vuota sollevano `std::out_of_range` in tutte e tre le implementazioni. `FixedSList::push_front` su una lista piena solleva `std::length_error`. Le versioni dinamiche devono lasciare propagare correttamente un eventuale fallimento di allocazione.

## Iterator invalidation

La baseline adotta un contratto preciso, specifico per rappresentazione. Gli iteratori non possiedono i nodi e sono validi solo finché l'operazione eseguita non li invalida secondo le regole seguenti.

- `SList<T>` raw: `push_front` conserva gli iteratori ai nodi già esistenti; `pop_front` invalida soltanto l'iteratore al nodo rimosso.
- `SListArray<T>`: gli iteratori conservano owner e indice, non un puntatore al buffer del vector. `push_front` conserva gli iteratori agli slot attivi anche quando il vector rialloca; `pop_front` invalida soltanto l'iteratore allo slot rimosso.
- `FixedSList<T, N>`: segue la stessa regola di `SListArray`, senza il caso di riallocazione del vector.

`clear`, distruzione, copy assignment e move assignment invalidano tutti gli iteratori associati all'oggetto modificato. Un'operazione di move invalida inoltre gli iteratori associati all'oggetto sorgente.

## Test comuni

I test copriranno lista vuota, singolo elemento, più inserimenti e rimozioni, ordine di traversal, svuotamento, copy/move, self-assignment, iteratori, casi limite e i contratti di errore. Ogni implementazione verrà verificata contro gli stessi comportamenti osservabili.

## Extra dopo baseline

Dopo una baseline corretta e testata, valuteremo benchmark controllati, controlli interni delle invarianti, test di invalidazione più estesi e miglioramenti dell'API che non cambino la semantica già congelata.
