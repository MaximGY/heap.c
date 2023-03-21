// Represent a heap
typedef struct Heap Heap;

// Create a new heap
Heap* heap_new();

// Destroy an instance of heap
void heap_free(Heap* heap);

// Add an entry to the heap
void heap_insert(Heap* heap, void* data, int priority);

// Return the entry at the top of the heap
void* heap_pop(Heap* heap);

// Bump up the priority of an entry in the heap
void heap_bump(Heap* heap, void* data, int new_priority);
