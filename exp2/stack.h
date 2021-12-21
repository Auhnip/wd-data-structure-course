#ifndef STACK_H_
#define STACK_H_

#define CAR_NAME_LENGTH 64

typedef struct car
{
	char name[CAR_NAME_LENGTH];
	unsigned int times;
} CAR;

typedef struct node
{
	CAR car;
	struct node* next;
} NODE;

typedef struct stack
{
	NODE* top;
	unsigned int size;
	unsigned int max_size;
} STACK;

void init_stack(STACK* stack, unsigned int size);

int push_stack(STACK* stack, const CAR* car);

int pop_stack(STACK* stack);

/*
 * pop the top elements of 'src' stack and push it to 'dest' stack
 */
int top_move_stack(STACK* dest, STACK* src);

/*
 * if stack is empty, return 1; else return 0;
 */
int is_empty_stack(STACK* stack);

/*
 * if stack is full, return 1; else return 0;
 */
int is_full_stack(STACK* stack);

/*
 * return a pointer which point to the top elements of stack
 */
CAR* top_stack(STACK* stack);

void print_stack(STACK* stack);

void clear_stack(STACK* stack);

#endif // STACK_H_