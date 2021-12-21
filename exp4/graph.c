#include "graph.h"

GRAPH* graph_init()
{
	GRAPH* ret = (GRAPH*)malloc(sizeof(GRAPH));
	if (!ret)
	{
		fprintf(stderr, "no memory\n");
		exit(EXIT_FAILURE);
	}
	for (int i = 0; i < LIST_SIZE; ++i)
	{
		for (int j = 0; j < LIST_SIZE; ++j)
		{
			ret->edges[i][j].spend = ret->edges[i][j].distance = INT_MAX;
		}
	}
	ret->edge_num = ret->point_num = 0;

	return ret;
}

static int find_point_index(GRAPH* graph, const char* name)
{
	int index = -1;

	for (int i = 0; i < graph->point_num; ++i)
	{
		if (!strcmp(graph->points[i], name))
		{
			index = i;
			break;
		}
	}
	if (! ~index)
	{
		if (graph->point_num == LIST_SIZE)
		{
			fprintf(stderr, "point number max\n");
			return index;
		}

		strncpy(graph->points[graph->point_num], name, BUFF_SIZE);
		index = graph->point_num;
		++graph->point_num;
	}

	return index;
}

void graph_add(GRAPH* graph, const char* start, const char* end, int distance, int spend)
{
	int start_index, end_index;

	start_index = find_point_index(graph, start);
	if (! ~start_index)
		return;
	
	end_index = find_point_index(graph, end);
	if (! ~end_index)
		return;
	
	graph->edges[start_index][end_index].distance = distance;
	graph->edges[start_index][end_index].spend = spend;
	++graph->edge_num;
}

const char* graph_get_point(const GRAPH* graph, int index)
{
	return graph->points[index];
}

int graph_get_spend(const GRAPH* graph, VECTOR* path)
{
	int spend = 0;
	for (size_t i = 1, current = path->array[0]; i < path->size; ++i)
	{
		spend += graph->edges[current][path->array[i]].spend;
		current = path->array[i];
	}
	return spend;
}

int graph_get_distance(const GRAPH* graph, VECTOR* path)
{
	int distance = 0;
	for (size_t i = 1, current = path->array[0]; i < path->size; ++i)
	{
		distance += graph->edges[current][path->array[i]].distance;
		current = path->array[i];
	}
	return distance;
}

VECTOR* graph_find_way(GRAPH* graph, const char* start, const char* end)
{
	// get index of start point and end point
	int start_index = find_point_index(graph, start);
	if (! ~start_index)
		return NULL;
	
	int end_index = find_point_index(graph, end);
	if (! ~end_index)
		return NULL;
	
	// create neccessary variables
	VECTOR* ret_val = NULL;
	int * each_min_spend = (int*)malloc(sizeof(int) * graph->point_num);
	int * reached = (int*)malloc(sizeof(int) * graph->point_num);
	VECTOR** each_path = (VECTOR**)malloc(sizeof(VECTOR*) * graph->point_num);
	if (!each_min_spend || !reached || !each_path)
	{
		fprintf(stderr, "no memory\n");
		goto free_resources;
	}

	// initialize neccessary variables
	memset(each_min_spend, INT_MAX, sizeof(int) * graph->point_num);
	memset(reached, 0, sizeof(int) * graph->point_num);
	for (int i = 0; i < graph->point_num; ++i)
		each_path[i] = vector_init();

	for (int i = 0; i < graph->point_num; ++i)
	{
		each_min_spend[i] = graph->edges[start_index][i].spend;
		if (each_min_spend[i] != INT_MAX)
		{
			vector_push_back(each_path[i], start_index);
			vector_push_back(each_path[i], i);
		}
	}
	reached[start_index] = 1;

	// get path that costs minimal
	for (int min_index, min; min_index != end_index; )
	{
		min_index = -1;
		min = INT_MAX;
		for (int i = 0; i < graph->point_num; ++i)
			if (!reached[i] && each_min_spend[i] < min)
			{
				min = each_min_spend[i];
				min_index = i;
			}
		
		// no way to destination
		if (min == INT_MAX)
			goto free_resources;

		reached[min_index] = 1;
		for (int i = 0; i < graph->point_num; ++i)
		{
			// if i isn't reached and min_index has a way to i
			// and it will costs lower go to i through min_index
			if (!reached[i] && graph->edges[min_index][i].spend != INT_MAX &&
				graph->edges[min_index][i].spend + each_min_spend[min_index] < each_min_spend[i])
			{
				each_min_spend[i] = graph->edges[min_index][i].spend + each_min_spend[min_index];
				vector_assign(each_path[i], each_path[min_index]);
				vector_push_back(each_path[i], i);
			}
		}
	}

	ret_val = vector_init();
	vector_assign(ret_val, each_path[end_index]);

free_resources:
	for (int i = 0; i < graph->point_num; ++i)
		vector_destroy(each_path[i]);
	free(each_path);
	free(each_min_spend);
	free(reached);

	return ret_val;
}

void graph_print_path(GRAPH* graph, VECTOR* path)
{
	for (size_t i = 0; i < vector_size(path); ++i)
	{
		printf("%s", graph->points[vector_get(path, i)]);
		if (i != vector_size(path) - 1)
			printf(" to ");
	}
}

void graph_destroy(GRAPH* graph)
{
	free(graph);
}