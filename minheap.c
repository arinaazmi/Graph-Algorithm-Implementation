/*
 * Our min-heap implementation.
 *
 * Author (starter code): A. Tafliovich.
 */

#include "minheap.h"
#include <math.h>

#define ROOT_INDEX 1
#define NOTHING -1

/*************************************************************************
 ** Suggested helper functions -- part of starter code
 *************************************************************************/

/* Returns True if 'maybeIdx' is a valid index in minheap 'heap', and 'heap'
 * stores an element at that index. Returns False otherwise.
 */
bool isValidIndex(MinHeap *heap, int maybeIdx) {
  if (heap != NULL && maybeIdx >= ROOT_INDEX && maybeIdx <= heap->size) {
    return true;
  }
  return false;
}

/* Returns the index of the left child of a node at index 'nodeIndex' in
 * minheap 'heap', if such exists.  Returns NOTHING if there is no such left
 * child.
 */
int leftIdx(MinHeap *heap, int nodeIndex) {
  if (isValidIndex(heap, nodeIndex)) {
    int leftId = 2 * nodeIndex;
    if (leftId <= heap->size) {
      return leftId;
    }
  }
  return NOTHING;
}

/* Returns the index of the right child of a node at index 'nodeIndex' in
 * minheap 'heap', if such exists.  Returns NOTHING if there is no such right
 * child.
 */
int rightIdx(MinHeap *heap, int nodeIndex) {
  if (isValidIndex(heap, nodeIndex)) {
    int rightId = 2 * nodeIndex + 1;
    if (rightId <= heap->size) {
      return rightId;
    }
  }
  return NOTHING;
}

/* Returns the index of the parent of a node at index 'nodeIndex' in minheap
 * 'heap', if such exists.  Returns NOTHING if there is no such parent.
 */
int parentIdx(MinHeap *heap, int nodeIndex) {
  if (isValidIndex(heap, nodeIndex)) {
    int parentId = floor(nodeIndex / 2);
    if (parentId >= ROOT_INDEX) {
      return parentId;
    }
  }
  return NOTHING;
}

/* Swaps contents of heap->arr[index1] and heap->arr[index2] if both 'index1'
 * and 'index2' are valid indices for minheap 'heap'. Has no effect
 * otherwise.
 */
void swap(MinHeap *heap, int index1, int index2) {
  if (isValidIndex(heap, index1) && isValidIndex(heap, index2)) {

    int id1 = heap->arr[index1].id;
    int id2 = heap->arr[index2].id;

    HeapNode temp = heap->arr[index1];
    heap->arr[index1] = heap->arr[index2];
    heap->arr[index2] = temp;

    heap->indexMap[id1] = index2;
    heap->indexMap[id2] = index1;
  }
}

/* Bubbles up the element newly inserted into minheap 'heap' at index
 * 'nodeIndex', if 'nodeIndex' is a valid index for heap. Has no effect
 * otherwise.
 */
void bubbleUp(MinHeap *heap, int nodeIndex) {
  if (isValidIndex(heap, nodeIndex)) {
    int parentId = parentIdx(heap, nodeIndex);

    while (nodeIndex > ROOT_INDEX &&
           heap->arr[nodeIndex].priority < heap->arr[parentId].priority) {
      swap(heap, nodeIndex, parentId);
      nodeIndex = parentId;
      parentId = parentIdx(heap, nodeIndex);
    }
  }
}

/* Bubbles down the element newly inserted into minheap 'heap' at the root,
 * if it exists. Has no effect otherwise.
 */
void bubbleDown(MinHeap *heap) {
  if (isValidIndex(heap, ROOT_INDEX)) {

    int nodeIndex = ROOT_INDEX;
    while (isValidIndex(heap, nodeIndex)) {
      int leftId = leftIdx(heap, nodeIndex);
      int rightId = rightIdx(heap, nodeIndex);
      int minId = NOTHING;
      if (isValidIndex(heap, leftId)) {
        minId = leftId;
        if (isValidIndex(heap, rightId) &&
            heap->arr[rightId].priority < heap->arr[leftId].priority) {
          minId = rightId;
        }
      }
      if (minId != NOTHING &&
          heap->arr[nodeIndex].priority > heap->arr[minId].priority) {
        swap(heap, nodeIndex, minId);
        nodeIndex = minId;
      } else {
        break;
      }
    }
  }
}

/* Returns node at index 'nodeIndex' in minheap 'heap'.
 * Precondition: 'nodeIndex' is a valid index in 'heap'
 *               'heap' is non-empty
 */
HeapNode nodeAt(MinHeap *heap, int nodeIndex) { return heap->arr[nodeIndex]; }

/* Returns priority of node at index 'nodeIndex' in minheap 'heap'.
 * Precondition: 'nodeIndex' is a valid index in 'heap'
 *               'heap' is non-empty
 */
int priorityAt(MinHeap *heap, int nodeIndex) {
  return heap->arr[nodeIndex].priority;
}

/* Returns ID of node at index 'nodeIndex' in minheap 'heap'.
 * Precondition: 'nodeIndex' is a valid index in 'heap'
 *               'heap' is non-empty
 */
int idAt(MinHeap *heap, int nodeIndex) { return heap->arr[nodeIndex].id; }

/* Returns index of node with ID 'id' in minheap 'heap'.
 * Precondition: 'id' is a valid ID in 'heap'
 *               'heap' is non-empty
 */
int indexOf(MinHeap *heap, int id) { return heap->indexMap[id]; }

HeapNode getMin(MinHeap *heap) { return heap->arr[ROOT_INDEX]; }

/*********************************************************************
 * Required functions
 ******************************************************************** */

/*
 Removes and returns the node with minimum priority in minheap 'heap'.
  Precondition: heap is non-empty
*/

HeapNode extractMin(MinHeap *heap) {
  HeapNode min = getMin(heap);
  // swap(heap, ROOT_INDEX, heap->size);
  heap->arr[ROOT_INDEX] = heap->arr[heap->size]; // Move last element to root.
  heap->indexMap[heap->arr[ROOT_INDEX].id] = ROOT_INDEX;

  heap->indexMap[min.id] = NOTHING;
  heap->arr[heap->size].priority = 0;
  heap->arr[heap->size].id = 0;

  heap->size--;
  if (heap->size > 0) {
    bubbleDown(heap);
  }
  return min;
}

/* Inserts a new node with priority 'priority' and ID 'id' into minheap 'heap'.
 * Precondition: 'id' is unique within this minheap
 *               0 <= 'id' < heap->capacity
 *               heap->size < heap->capacity
 */
void insert(MinHeap *heap, int priority, int id) {
  if (heap->size < heap->capacity) {
    heap->size++;
    heap->arr[heap->size].priority = priority;
    heap->arr[heap->size].id = id;
    heap->indexMap[id] = heap->size;
    bubbleUp(heap, heap->size);
  }
}

/* Returns priority of the node with ID 'id' in 'heap'.
 * Precondition: 'id' is a valid node ID in 'heap'.
 */
int getPriority(MinHeap *heap, int id) {
  return heap->arr[heap->indexMap[id]].priority;
}

/* Sets priority of node with ID 'id' in minheap 'heap' to 'newPriority', if
 * such a node exists in 'heap' and its priority is larger than
 * 'newPriority', and returns True. Has no effect and returns False, otherwise.
 * Note: this function bubbles up the node until the heap property is restored.
 */
bool decreasePriority(MinHeap *heap, int id, int newPriority) {
  int index = heap->indexMap[id];
  if (isValidIndex(heap, index) && heap->arr[index].priority > newPriority) {
    heap->arr[index].priority = newPriority;
    bubbleUp(heap, index);
    return true;
  }
  return false;
}

/* Returns a newly created empty minheap with initial capacity 'capacity'.
 * Precondition: capacity >= 0
 */
MinHeap *newHeap(int capacity) {
  MinHeap *heap = (MinHeap *)malloc(sizeof(MinHeap));
  heap->size = 0;
  heap->capacity = capacity;
  heap->arr = (HeapNode *)malloc((capacity + 1) * sizeof(HeapNode));
  heap->indexMap = (int *)malloc((capacity + 1) * sizeof(int));
  for (int i = 0; i <= capacity; i++) {
    heap->indexMap[i] = NOTHING;
  }
  return heap;
}

/* Frees all memory allocated for minheap 'heap'.
 */
void deleteHeap(MinHeap *heap) {
  free(heap->arr);
  free(heap->indexMap);
  free(heap);
}

/*********************************************************************
 ** Helper function provided in the starter code
 *********************************************************************/
void printHeap(MinHeap *heap) {
  printf("MinHeap with size: %d\n\tcapacity: %d\n\n", heap->size,
         heap->capacity);
  printf("index: priority [ID]\t ID: index\n");
  for (int i = 0; i < heap->capacity; i++)
    printf("%d: %d [%d]\t\t%d: %d\n", i, priorityAt(heap, i), idAt(heap, i), i,
           indexOf(heap, i));
  printf("%d: %d [%d]\t\t\n", heap->capacity, priorityAt(heap, heap->capacity),
         idAt(heap, heap->capacity));
  printf("\n\n");
}
