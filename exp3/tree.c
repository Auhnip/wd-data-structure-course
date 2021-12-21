#include "tree.h"

#include <string.h>

#define get_key(X) ((X).name)

void init_bin_tree(BIN_TREE* tree)
{
	tree->root = NULL;
	tree->size = 0;
}

static NODE* find_bin_tree_node_util(BIN_NODE* p_node, const char* key)
{
	if (!p_node)
		return NULL;
	
	int cmp_ret = strcmp(get_key(p_node->data), key);

	if (cmp_ret < 0)
	{
		return find_bin_tree_node_util(p_node->left, key);
	}
	else if (cmp_ret > 0)
	{
		return find_bin_tree_node_util(p_node->right, key);
	}
	
	return & p_node->data;
}

const NODE* find_bin_tree_node(const BIN_TREE* tree, const char* key)
{
	BIN_NODE* p_node = tree->root;

	return find_bin_tree_node_util(p_node, key);
}

static BIN_NODE* create_new_node(const NODE* data)
{
	BIN_NODE* new_bin_node = (BIN_NODE*)malloc(sizeof(BIN_NODE));
	if (!new_bin_node)
	{
		fprintf(stderr, "bad alloc\n");
		exit(EXIT_FAILURE);
	}

	new_bin_node->data = *data;
	new_bin_node->left = new_bin_node->right = NULL;
}

static BIN_NODE* insert_bin_tree_node_util(BIN_NODE* p_node, const NODE* data)
{
	if (!p_node)
	{
		return create_new_node(data);
	}

	int cmp_ret = strcmp(get_key(p_node->data), get_key(*data));
	
	if (cmp_ret < 0)
	{
		p_node->left = insert_bin_tree_node_util(p_node->left, data);
	}
	else if (cmp_ret > 0)
	{
		p_node->right = insert_bin_tree_node_util(p_node->right, data);
	}
	// if key equals, do nothing

	return p_node;
}

void insert_bin_tree_node(BIN_TREE* tree, const NODE* data)
{
	tree->root = insert_bin_tree_node_util(tree->root, data);
	++ tree->size;
}

_Bool insert_tuple(BIN_TREE* tree, const char* parent, unsigned num, const char* child)
{
	static NODE data = {"", 0, NULL, NULL, NULL, 0, NULL};

	if (!tree->size)
	{
		strncpy(data.name, parent, BUFF_SIZE);
		data.number = 0;

		insert_bin_tree_node(tree, &data);
	}

	NODE* find_prt = find_bin_tree_node_util(tree->root, parent);
	if (!find_prt)
		return 0;
	
	strncpy(data.name, child, BUFF_SIZE);
	data.number = num;

	insert_bin_tree_node(tree, &data);
	// p_child will never be NULL
	NODE* p_child = find_bin_tree_node_util(tree->root, child);
	
	if (!find_prt->last_child)
	{
		find_prt->last_child = find_prt->child = p_child;
	}
	else
	{
		find_prt->last_child->next = p_child;
		find_prt->last_child = p_child;
	}
	p_child->parent = find_prt;
	++ find_prt->child_num;

	return 1;
}

static void clear_bin_tree_util(BIN_NODE* p_node)
{
	if (!p_node)
		return;

	clear_bin_tree_util(p_node->left);
	clear_bin_tree_util(p_node->right);

	free(p_node);
}

void clear_bin_tree(BIN_TREE* tree)
{
	clear_bin_tree_util(tree->root);
	tree->size = 0;
}

static void print_node(const NODE* p_data)
{
	printf("|--[%s, %u--child_num=%u]\n", p_data->name, p_data->number, p_data->child_num);
}

static void print_bin_tree_util(BIN_NODE* p_node, unsigned level)
{
	if (!p_node)
		return;

	unsigned n = level;
	while (n--)
		putchar('\t');
	
	print_node(& p_node->data);
	print_bin_tree_util(p_node->left, level + 1);
	print_bin_tree_util(p_node->right, level + 1);
}

void print_bin_tree(const BIN_TREE* tree)
{
	print_bin_tree_util(tree->root, 0);
}