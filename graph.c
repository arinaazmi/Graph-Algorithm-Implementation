/*
 * Our graph implementation.
 *
 * Author: A. Tafliovich.
 */

#include "graph.h"

/*********************************************************************
 ** Helper function provided in the starter code
 *********************************************************************/

void printEdge(Edge *edge) {
  if (edge == NULL)
    printf("NULL");
  else
    printf("(%d -- %d, %d)", edge->fromVertex, edge->toVertex, edge->weight);
}

void printEdgeList(EdgeList *head) {
  while (head != NULL) {
    printEdge(head->edge);
    printf(" --> ");
    head = head->next;
  }
  printf("NULL");
}

void printVertex(Vertex *vertex) {
  if (vertex == NULL) {
    printf("NULL");
  } else {
    printf("%d: ", vertex->id);
    printEdgeList(vertex->adjList);
  }
}

void printGraph(Graph *graph) {
  if (graph == NULL) {
    printf("NULL");
    return;
  }
  printf("Number of vertices: %d. Number of edges: %d.\n\n", graph->numVertices,
         graph->numEdges);

  for (int i = 0; i < graph->numVertices; i++) {
    printVertex(graph->vertices[i]);
    printf("\n");
  }
  printf("\n");
}

/*********************************************************************
 ** Required functions
 *********************************************************************/

/* Returns a newly created Edge from vertex with ID 'fromVertex' to vertex
 * with ID 'toVertex', with weight 'weight'.
 */
Edge *newEdge(int fromVertex, int toVertex, int weight) {
  Edge *edge = malloc(sizeof(Edge));
  if (edge == NULL)
    return NULL;
  edge->fromVertex = fromVertex;
  edge->toVertex = toVertex;
  edge->weight = weight;
  return edge;
}

/* Returns a newly created EdgeList containing 'edge' and pointing to the next
 * EdgeList node 'next'.
 */
EdgeList *newEdgeList(Edge *edge, EdgeList *next) {
  EdgeList *edgeList = malloc(sizeof(EdgeList));
  if (edgeList == NULL)
    return NULL;
  edgeList->edge = edge;
  edgeList->next = next;
  return edgeList;
}

/* Returns a newly created Vertex with ID 'id', value 'value', and adjacency
 * list 'adjList'.
 * Precondition: 'id' is valid for this vertex
 */
Vertex *newVertex(int id, void *value, EdgeList *adjList) {
  Vertex *vertex = malloc(sizeof(Vertex));
  if (vertex == NULL)
    return NULL;
  vertex->id = id;
  vertex->value = value;
  vertex->adjList = adjList;
  return vertex;
}

/* Returns a newly created Graph with space for 'numVertices' vertices.
 * Precondition: numVertices >= 0
 */
Graph *newGraph(int numVertices) {
  Graph *graph = malloc(sizeof(Graph));
  if (graph == NULL)
    return NULL;
  graph->numVertices = numVertices;
  graph->numEdges = 0;
  graph->vertices = malloc(numVertices * sizeof(Vertex *));
  if (graph->vertices == NULL) {
    free(graph);
    return NULL;
  }
  for (int i = 0; i < numVertices; i++) {
    graph->vertices[i] = NULL;
  }
  return graph;
}

/* Frees memory allocated for EdgeList starting at 'head'.
 */
void deleteEdgeList(EdgeList *head) {
  while (head != NULL) {
    EdgeList *next = head->next;
    free(head->edge);
    free(head);
    head = next;
  }
}

/* Frees memory allocated for 'vertex' including its adjacency list.
 */
void deleteVertex(Vertex *vertex) {
  if (vertex == NULL)
    return;
  deleteEdgeList(vertex->adjList);
  free(vertex);
}

/* Frees memory allocated for 'graph'.
 */
void deleteGraph(Graph *graph) {
  if (graph == NULL)
    return;
  for (int i = 0; i < graph->numVertices; i++) {
    deleteVertex(graph->vertices[i]);
  }
  free(graph->vertices);
  free(graph);
}
