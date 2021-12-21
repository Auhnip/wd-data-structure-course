#ifndef GRAPH_H_
#define GRAPH_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#include "vector.h"

#define LIST_SIZE 64
#define BUFF_SIZE 64

typedef struct graph
{
	struct
	{
		int distance;
		int spend;
	} edges[LIST_SIZE][LIST_SIZE];
	char points[LIST_SIZE][BUFF_SIZE];
	int edge_num;
	int point_num;
} GRAPH;

GRAPH* graph_init();

void graph_add(GRAPH* graph, const char* start, const char* end, int distance, int spend);

const char* graph_get_point(const GRAPH* graph, int index);

int graph_get_spend(const GRAPH* graph, VECTOR* path);

int graph_get_distance(const GRAPH* graph, VECTOR* path);

VECTOR* graph_find_way(GRAPH* graph, const char* start, const char* end);

void graph_print_path(GRAPH* graph, VECTOR* path);

void graph_destroy(GRAPH* graph);

#endif