#ifndef TREE_H_
#define TREE_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFF_SIZE 64

typedef struct node
{
	char name[BUFF_SIZE];
	unsigned int number;
	struct node* next;
	struct node* child;
	struct node* last_child;
	unsigned int child_num;
	struct node* parent;
} NODE;

typedef struct bin_node
{
	NODE data;
	struct bin_node* left;
	struct bin_node* right;
} BIN_NODE;

typedef struct bin_tree
{
	BIN_NODE* root;
	unsigned size;
} BIN_TREE;

void init_bin_tree(BIN_TREE* tree);

const NODE* find_bin_tree_node(const BIN_TREE* tree, const char* key);

// void insert_bin_tree_node(BIN_TREE* tree, const NODE* data);

_Bool insert_tuple(BIN_TREE* tree, const char* parent, unsigned num, const char* child);

void clear_bin_tree(BIN_TREE* tree);

void print_bin_tree(const BIN_TREE* tree);

#endif