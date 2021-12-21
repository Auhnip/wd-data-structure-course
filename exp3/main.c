#include <stdio.h>

#include "tree.h"

#define DEF_FILE "definitions.txt"
#define SERH_FILE "queries.txt"
#define CMD_WHAT "whatis"
#define CMD_NUM "howmany"

BIN_TREE get_bin_tree(FILE *fin);

char *get_line(char *buff, int len, FILE *file);

_Bool search_and_print(const BIN_TREE *tree, FILE *fin);

static char buff[BUFF_SIZE];

int main(void)
{
	FILE *fin = fopen(DEF_FILE, "r");
	if (!fin)
	{
		fprintf(stderr, "open file \"%s\" failed\n", DEF_FILE);
		exit(EXIT_FAILURE);
	}

	BIN_TREE tree = get_bin_tree(fin);

	if (fclose(fin))
	{
		fprintf(stderr, "close file \"%s\" failed\n", DEF_FILE);
		exit(EXIT_FAILURE);
	}

	fin = fopen(SERH_FILE, "r");
	if (!fin)
	{
		fprintf(stderr, "open file \"%s\" failed\n", SERH_FILE);
		exit(EXIT_FAILURE);
	}

	search_and_print(&tree, fin);

	if (fclose(fin))
	{
		fprintf(stderr, "close file \"%s\" failed\n", SERH_FILE);
		exit(EXIT_FAILURE);
	}

	clear_bin_tree(&tree);

	return 0;
}

BIN_TREE get_bin_tree(FILE *fin)
{
	static char parent[BUFF_SIZE];
	static char child[BUFF_SIZE];
	unsigned num;

	BIN_TREE tree;
	init_bin_tree(&tree);

	while (get_line(buff, BUFF_SIZE, fin))
	{
		if (sscanf(buff, "%s %u %s", parent, &num, child) != 3)
		{
			fprintf(stderr, "line: \"%s\" is invalid\n", buff);
			goto clear_resources;
		}

		if (!insert_tuple(&tree, parent, num, child))
		{
			fprintf(stderr, "insert tuple [\"%s\"-%u-\"%s\"] failed\n", parent, num, child);
			goto clear_resources;
		}
	}
	goto ret;

clear_resources:
	clear_bin_tree(&tree);
ret:
	return tree;
}

void print_sub_parts(const NODE *node)
{
	const NODE *child = node->child;

	printf("Part %s subparts are:\n", node->name);
	if (!child)
	{
		printf("\t[empty]\n");
		return;
	}

	while (child)
	{
		printf("\t%u %s\n", child->number, child->name);
		child = child->next;
	}
}

_Bool search_and_print(const BIN_TREE *tree, FILE *fin)
{
	static char word1[BUFF_SIZE];
	static char word2[BUFF_SIZE];
	const NODE *data;
	unsigned num;

	while (get_line(buff, BUFF_SIZE, fin) && sscanf(buff, "%s", word1) == 1)
	{
		if (!strcmp(word1, CMD_WHAT))
		{
			if (sscanf(buff, "%*s %s", word1) != 1 ||
					!(data = find_bin_tree_node(tree, word1)))
			{
				goto invalid_input;
			}

			print_sub_parts(data);
		}
		else if (!strcmp(word1, CMD_NUM))
		{
			if (sscanf(buff, "%*s %s %s", word1, word2) != 2 ||
				!(data = find_bin_tree_node(tree, word1)))
			{
				goto invalid_input;
			}

			for (num = 1; !data && strcmp(word2, data->name); data = data->parent)
			{
				num *= data->number;
			}

			if (!data)
				goto invalid_input;

			printf("%s has %u %s\n", word2, num, word1);
		}
		else
		{
			goto invalid_input;
		}
	}

	return 1;

invalid_input:
	fprintf(stderr, "line: \"%s\" invalid\n", buff);
	return 0;
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