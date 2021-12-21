#include <stdio.h>

#include "graph.h"
#include "vector.h"

#define INPUT_FILE "services.txt"

char *get_line(char *buff, int len, FILE *file);

int main()
{
	char buff[BUFF_SIZE];
	char name1[BUFF_SIZE];
	char name2[BUFF_SIZE];
	int distance, spend;
	VECTOR* path;

	GRAPH* map = graph_init();

	FILE* fin = fopen(INPUT_FILE, "r");
	if (!fin)
	{
		fprintf(stderr, "open file \"%s\" failed\n", INPUT_FILE);
		exit(EXIT_FAILURE);
	}

	while (get_line(buff, BUFF_SIZE, fin))
	{
		if (sscanf(buff, "%s %s %d %d", name1, name2, &spend, &distance) != 4)
			goto input_file_error;
		
		graph_add(map, name1, name2, distance, spend);
	}

	if (fclose(fin))
	{
		fprintf(stderr, "close file \"%s\" failed\n", INPUT_FILE);
	}

	while (puts("Enter a start and destination city: <'quit' to exit>"),
		get_line(buff, BUFF_SIZE, stdin))
	{
		if (sscanf(buff, "%s %s", name1, name2) == 1 && !strcmp("quit", name1))
			break;
		
		path = graph_find_way(map, name1, name2);
		if (!path)
		{
			printf("sorry, \"%s\" has no way to \"%s\"\n", name1, name2);
			continue;
		}

		printf("The cheapest route from %s to %s\n", name1, name2);
		printf("costs %d euros and spans %d kilometers\n",
			graph_get_spend(map, path), graph_get_distance(map, path));
		graph_print_path(map, path);
		putchar('\n');

		vector_destroy(path);
	}
	
	graph_destroy(map);

	return 0;

input_file_error:
	fprintf(stderr, "input error\n");
	exit(EXIT_FAILURE);
}

char *get_line(char *buff, int len, FILE *file)
{
	char *ret = fgets(buff, len, file);
	char *find;
	int ch;

	if (ret)
	{
		find = strchr(buff, '\n');

		if (find)
		{
			*find = '\0';
		}
		else
		{
			while (ch = fgetc(file), ch != '\n' && ch != EOF)
				continue;
		}
	}

	return ret;
}