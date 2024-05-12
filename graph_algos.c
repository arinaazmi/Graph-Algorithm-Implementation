/*
 * Graph algorithms.
 *
 * Author (of starter code): A. Tafliovich.
 */

#include <limits.h>

#include "graph.h"
#include "minheap.h"

#define NOTHING -1
#define DEBUG 0

typedef struct records {
  int numVertices;   // total number of vertices in the graph
                     // vertex IDs are 0, 1, ..., numVertices-1
  MinHeap *heap;     // priority queue
  bool *finished;    // finished[id] is true iff vertex id is finished
                     //   i.e. no longer in the PQ
  int *predecessors; // predecessors[id] is the predecessor of vertex id
  Edge *tree;        // keeps edges for the resulting tree
  int numTreeEdges;  // current number of edges in mst
} Records;

/*************************************************************************
 ** Provided helper functions -- part of starter code to help you debug!
 *************************************************************************/
void printRecords(Records *records) {
  if (records == NULL)
    return;

  int numVertices = records->numVertices;
  printf("Reporting on algorithm's records on %d vertices...\n", numVertices);

  printf("The PQ is:\n");
  printHeap(records->heap);

  printf("The finished array is:\n");
  for (int i = 0; i < numVertices; i++)
    printf("\t%d: %d\n", i, records->finished[i]);

  printf("The predecessors array is:\n");
  for (int i = 0; i < numVertices; i++)
    printf("\t%d: %d\n", i, records->predecessors[i]);

  printf("The TREE edges are:\n");
  for (int i = 0; i < records->numTreeEdges; i++)
    printEdge(&records->tree[i]);

  printf("... done.\n");
}

/*************************************************************************
 ** Suggested helper functions -- part of starter code
 *************************************************************************/

/* Creates, populates, and returns a MinHeap to be used by Prim's and
 * Dijkstra's algorithms on Graph 'graph' starting from vertex with ID
 * 'startVertex'.
 * Precondition: 'startVertex' is valid in 'graph'
 */
MinHeap *initHeap(Graph *graph, int startVertex) {
  MinHeap *heap = newHeap(graph->numVertices);
  for (int i = 0; i < graph->numVertices; i++) {
    if (i == startVertex) {
      insert(heap, 0, i);
    } else {
      insert(heap, INT_MAX, i);
    }
  }
  return heap;
}

void deleteRecords(Records *records) {
  if (records == NULL)
    return;
  deleteHeap(records->heap);
  free(records->finished);
  free(records->predecessors);
  free(records);
}

/* Creates, populates, and returns all records needed to run Prim's and
 * Dijkstra's algorithms on Graph 'graph' starting from vertex with ID
 * 'startVertex'.
 * Precondition: 'startVertex' is valid in 'graph'
 */
Records *initRecords(Graph *graph, int startVertex) {
  Records *records = malloc(sizeof(Records));

  if (!records) {
    perror("Unable to allocate memory for records");
    exit(EXIT_FAILURE);
  }

  records->numVertices = graph->numVertices;
  records->heap = initHeap(graph, startVertex);
  records->finished = malloc(graph->numVertices * sizeof(bool));
  records->predecessors = malloc(graph->numVertices * sizeof(int));
  records->tree = malloc((graph->numVertices - 1) * sizeof(Edge));
  records->numTreeEdges = 0;

  // error handling
  if (!records->heap || !records->predecessors || !records->finished ||
      !records->tree) {
    perror("Unable to allocate memory for records' components");
    // Free already allocated resources before exiting
    deleteRecords(records);
    exit(EXIT_FAILURE);
  }

  for (int i = 0; i < graph->numVertices; i++) {
    records->finished[i] = false;
    records->predecessors[i] = NOTHING;
  }
  return records;
}

/* Returns true iff 'heap' is NULL or is empty. */
bool isEmpty(MinHeap *heap) { return (heap == NULL || heap->size <= 0); }

/* Add a new edge to records at index ind. */
void addTreeEdge(Records *records, int ind, int fromVertex, int toVertex,
                 int weight) {
  Edge edge = {fromVertex, toVertex, weight};
  records->tree[ind] = edge;
}

/*************************************************************************
 ** Required functions
 *************************************************************************/

/* Runs Prim's algorithm on Graph 'graph' starting from vertex with ID
 * 'startVertex', and return the resulting MST: an array of Edges.
 * Returns NULL is 'startVertex' is not valid in 'graph'.
 * Precondition: 'graph' is connected.
 */
Edge *getMSTprim(Graph *graph, int startVertex) {

  // invalid entries
  if (graph == NULL || startVertex < 0 || startVertex >= graph->numVertices) {
    return NULL;
  }
  // Initialize records
  Records *records = initRecords(graph, startVertex);
  // while PQ is not empty
  while (!isEmpty(records->heap)) {
    int weight = getMin(records->heap).priority;
    HeapNode node = extractMin(records->heap);
    int u = node.id;
    records->finished[u] = true;

    if (records->predecessors[u] != NOTHING) {
      addTreeEdge(records, records->numTreeEdges++, u, records->predecessors[u],
                  weight);
    }
    Vertex *vertex = graph->vertices[u];
    EdgeList *adjList = vertex->adjList;
    while (adjList != NULL) {
      Edge *edge = adjList->edge;
      int toVertex = edge->toVertex;
      if (!records->finished[toVertex] &&
          edge->weight < getPriority(records->heap, toVertex)) {
        decreasePriority(records->heap, toVertex, edge->weight);
        records->predecessors[toVertex] = u;
      }
      adjList = adjList->next;
    }
  }
  Edge *mst = records->tree;

  deleteRecords(records);
  return mst;
}

/* Runs Dijkstra's algorithm on Graph 'graph' starting from vertex with ID
 * 'startVertex', and return the resulting distance tree: an array of edges.
 * Returns NULL if 'startVertex' is not valid in 'graph'.
 * Precondition: 'graph' is connected.
 */
Edge *getDistanceTreeDijkstra(Graph *graph, int startVertex) {
  if (graph == NULL || startVertex < 0 || startVertex >= graph->numVertices) {
    return NULL;
  }

  Records *records = initRecords(graph, startVertex);
  while (!isEmpty(records->heap)) {
    HeapNode node = extractMin(records->heap);
    int u = node.id;
    records->finished[u] = true;
    if (records->predecessors[u] != NOTHING) {
      addTreeEdge(records, records->numTreeEdges++, u, records->predecessors[u],
                  node.priority);
    }
    Vertex *vertex = graph->vertices[u];
    EdgeList *adjList = vertex->adjList;
    while (adjList != NULL) {
      Edge *edge = adjList->edge;
      int toVertex = edge->toVertex;
      if (!records->finished[toVertex] &&
          node.priority + edge->weight < getPriority(records->heap, toVertex)) {
        decreasePriority(records->heap, toVertex, node.priority + edge->weight);
        records->predecessors[toVertex] = u;
      }
      adjList = adjList->next;
    }
  }
  Edge *distTree = records->tree;

  deleteRecords(records);
  return distTree;
}

/* Creates and returns an array 'paths' of shortest paths from every vertex
 * in the graph to vertex 'startVertex', based on the information in the
 * distance tree 'distTree' produced by Dijkstra's algorithm on a graph with
 * 'numVertices' vertices and with the start vertex 'startVertex'.  paths[id]
 * is the list of edges of the form
 *   [(id -- id_1, w_0), (id_1 -- id_2, w_1), ..., (id_n -- start, w_n)]
 *   where w_0 + w_1 + ... + w_n = distance(id)
 * Returns NULL if 'startVertex' is not valid in 'distTree'.
 */
EdgeList **getShortestPaths(Edge *distTree, int numVertices, int startVertex) {
  if (distTree == NULL || startVertex < 0 || startVertex >= numVertices) {
    return NULL;
  }

  EdgeList **paths = malloc(numVertices * sizeof(EdgeList *));
  if (paths == NULL)
    return NULL;

  for (int i = 0; i < numVertices; i++) {
    if (i == startVertex) {
      paths[i] = NULL;
    }
  }
  return paths;
}
