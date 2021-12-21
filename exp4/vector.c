#include "vector.h"

VECTOR* vector_init()
{
	VECTOR* arr = (VECTOR*)malloc(sizeof(VECTOR));
	if (!arr)
	{
		fprintf(stderr, "no memory\n");
		exit(EXIT_FAILURE);
	}
	arr->array = NULL;
	arr->size = 0;
	arr->capacity = 0;
	return arr;
}

static void adjust_arr(VECTOR* arr, size_t space)
{
	if (arr->size + space <= arr->capacity)
		return;
	
	size_t new_cap = (arr->capacity ? arr->capacity << 1 : 1);
	while (new_cap < arr->size + space)
		new_cap <<= 1;

	ELEM_TYPE* new_arr = (ELEM_TYPE*)malloc(sizeof(ELEM_TYPE) * new_cap);
	if (!new_arr)
	{
		fprintf(stderr, "bad alloc\n");
		exit(EXIT_FAILURE);
	}

	memcpy(new_arr, arr->array, arr->size * sizeof(ELEM_TYPE));

	free(arr->array);
	arr->array = new_arr;
	arr->capacity = new_cap;
}

// insert a elements: x
_Bool vector_insert(VECTOR* arr, size_t index, ELEM_TYPE x)
{
	if (index > arr->size)
		return 0;
	
	adjust_arr(arr, 1);

	for (size_t i = arr->size; i != index; --i)
		arr->array[i] = arr->array[i - 1];

	arr->array[index] = x;
	++ arr->size;
	
	return 1;
}

void vector_push_back(VECTOR* arr, ELEM_TYPE x)
{
	adjust_arr(arr, 1);
	arr->array[arr->size] = x;
	++ arr->size;
}

// insert a array: x_arr[len]
_Bool vector_insert_arr(VECTOR* arr, size_t index, const ELEM_TYPE* x_arr, size_t len)
{
	if (index > arr->size)
		return 0;
	
	adjust_arr(arr, len);

	for (size_t i = arr->size - 1; i != index - 1; --i)
		arr->array[i + len] = arr->array[i];
	
	memcpy(arr->array + index, x_arr, len * sizeof(ELEM_TYPE));
	arr->size += len;

	return 1;
}

void vector_assign(VECTOR* lhs, const VECTOR* rhs)
{
	if (lhs == rhs)
		return;
	
	if (lhs->capacity < rhs->capacity)
		adjust_arr(lhs, rhs->capacity - lhs->capacity);
	
	memcpy(lhs->array, rhs->array, sizeof(ELEM_TYPE) * rhs->size);
	lhs->size = rhs->size;
}

_Bool vector_remove(VECTOR* arr, size_t index)
{
	if (index >= arr->size)
		return 0;
	
	for (size_t i = index; i != arr->size - 1; ++i)
		arr->array[i] = arr->array[i + 1];
	--arr->size;

	return 1;
}

void vector_for_each(const VECTOR* arr, void (*func)(const ELEM_TYPE*))
{
	for (size_t i = 0; i < arr->size; ++i)
		func(& arr->array[i]);
}

void vector_destroy(VECTOR* arr)
{
	free(arr->array);
	arr->array = NULL;
	arr->capacity = arr->size = 0;
	free(arr);
}