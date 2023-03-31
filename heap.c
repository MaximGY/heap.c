#include "heap.h"
#include <stdlib.h>

// Represent a heap
typedef struct Heap {
    HeapEntry* entries;
    size_t size;
    size_t capacity;
} Heap;

// Represents an entry in a heap
typedef struct HeapEntry {
    void* data;
    int priority;
} HeapEntry;

// Helpers
// -----------------------------------
void swap(HeapEntry* a, HeapEntry* b) {
    HeapEntry temp = *a;
    *a = *b;
    *b = temp;
}

size_t parent(size_t i) {
    return (i - 1) / 2;
}

size_t left_child(size_t i) {
    return (i * 2) + 1;
}

size_t right_child(size_t i) {
    return (i * 2) + 2;
}

size_t smallest_child(Heap* heap, size_t i) {
    size_t rchild = right_child(i);
    size_t lchild = left_child(i);
    return rchild < heap->size && heap->entries[lchild].priority > heap->entries[rchild].priority ? rchild : lchild;
}

void sift_up(Heap* heap, size_t i) {
    size_t pi = parent(i);
    while (i > 0 && heap->entries[pi].priority > heap->entries[i].priority) {
        swap(heap->entries + i, heap->entries + pi);
        i = pi;
        pi = parent(pi);
    }
}

void sift_down(Heap* heap, size_t i) {
    size_t si = smallest_child(heap, i);
    while (left_child(i) < heap->size && heap->entries[si].priority < heap->entries[i].priority) {
        swap(heap->entries + si, heap->entries + i);
        i = si;
        si = smallest_child(heap, si);
    }
}

// -----------------------------------

Heap* heap_new() {
    Heap* heap;

    if (!(heap = (Heap*) malloc(sizeof(Heap)))) return NULL;
    if (!(heap->entries = malloc(sizeof(HeapEntry)))) {
        free(heap);
        return NULL;
    };

    heap->size = 0;
    heap->capacity = 1;

    return heap;
}

void heap_free(Heap* heap) {
    free(heap->entries);
    free(heap);
}

// Add an entry into the heap
void heap_insert(Heap* heap, void* data, int priority) {
    // When heap is full, double its capacity (overhead is O(1), overall)
    if (heap->size == heap->capacity) {
        heap->entries = realloc(heap->entries, heap->capacity * 2 * sizeof(HeapEntry));
        heap->capacity *= 2;
    }

    heap->entries[heap->size++] = (HeapEntry) { .data = data, .priority = priority };
    sift_up(heap, heap->size - 1);
}

// Pop the top entry in the heap
void* heap_pop(Heap* heap) {
    if (!heap->size) return NULL;

    swap(heap->entries, heap->entries + heap->size - 1);
    heap->size--;
    sift_down(heap, 0);

    return heap->entries[heap->size].data;
}

// Bump up the priority of an entry in the heap
//? Not great since finding an element in the heap is O(n).
//? However, caching the position of each node gets expensive.
//? WAAAY too expensive (using a hashmap, for example...)
void heap_bump(Heap* heap, void* data, int new_priority) {
    size_t i = 0;

    while (heap->entries[i].data != data) i++;

    heap->entries[i].priority = new_priority;
    sift_up(heap, i);
}
