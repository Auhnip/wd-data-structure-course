#include "stack.h"
#include <stdio.h>
#include <stdlib.h>

void init_stack(STACK* stack, unsigned int size)
{
	stack->top = NULL;
	stack->size = 0;
	stack->max_size = size;
}

int push_stack(STACK* stack, const CAR* car)
{
	if (stack->size == stack->max_size)
	{
		// stack is full
		return 0;
	}

	NODE* new_node = (NODE*)malloc(sizeof(NODE));
	if (!new_node)
	{
		fprintf(stderr, "memory not enough\n");
		exit(EXIT_FAILURE);
	}

	new_node->car = *car;
	new_node->next = stack->top;
	stack->top = new_node;

	++ stack->size;

	return 1;
}

int pop_stack(STACK* stack)
{
	if (!stack->size)
	{
		// stack is empty
		return 0;
	}

	NODE* not_need = stack->top;
	stack->top = not_need->next;
	free(not_need);

	-- stack->size;

	return 1;
}

int top_move_stack(STACK* dest, STACK* src)
{
	if (!src->size || dest->size == dest->max_size)
	{
		// src is empty or dest is full
		return 0;
	}
	
	NODE* top = src->top;
	src->top = src->top->next;
	-- src->size;

	top->next = dest->top;
	dest->top = top;
	++ dest->size;

	return 1;
}

int is_empty_stack(STACK* stack)
{
	return ! stack->size;
}

int is_full_stack(STACK* stack)
{
	return stack->size == stack->max_size;
}

CAR* top_stack(STACK* stack)
{
	if (is_empty_stack(stack))
		return NULL;
	
	return & stack->top->car;
}

void print_stack(STACK* stack)
{
	printf("<stack> [size = %u/%u | ", stack->size, stack->max_size);
	for (NODE* cur = stack->top; cur; cur = cur->next)
	{
		printf("[%s - %u]", (cur->car).name, (cur->car).times);
	}
	printf(" ]\n");
}

void clear_stack(STACK* stack)
{
	NODE* cur = stack->top;
	NODE* next;

	while (cur)
	{
		next = cur->next;
		free(cur);
		cur = next;
	}

	stack->top = NULL;
	stack->size = 0;
}