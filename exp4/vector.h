#ifndef VECTOR_H_
#define VECTOR_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFF_SIZE 64

#define vector_get(X, Y) ((X)->array[(Y)])
#define vector_size(X) ((X)->size)

typedef int ELEM_TYPE;

typedef struct
{
	ELEM_TYPE* array;
	size_t size;
	size_t capacity;
} VECTOR;

VECTOR* vector_init();

_Bool vector_insert(VECTOR* arr, size_t index, ELEM_TYPE x);

void vector_push_back(VECTOR* arr, ELEM_TYPE x);

_Bool vector_insert_arr(VECTOR* arr, size_t index, const ELEM_TYPE* x_arr, size_t len);

void vector_assign(VECTOR* lhs, const VECTOR* rhs);

_Bool vector_remove(VECTOR* arr, size_t index);

void vector_for_each(const VECTOR* arr, void (*func)(const ELEM_TYPE*));

void vector_destroy(VECTOR* arr);

#endif