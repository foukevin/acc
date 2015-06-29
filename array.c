#include "array.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

static void array_resize(array_t *array, size_t new_cap)
{
	printf("changing array capacity to %d\n", new_cap);
	array->store = realloc(array->store, new_cap * array->elem_size);
	array->capacity = new_cap;
	assert(array->store);
}

array_t *new_array(size_t cap, size_t elem_size)
{
	array_t *new_array;

	new_array = malloc(sizeof(*new_array));
	assert(new_array);
	new_array->store = malloc(cap * elem_size);
	assert(new_array->store);

	new_array->elem_size = elem_size;
	new_array->capacity = cap;
	new_array->size = 0;

	return new_array;
}

void delete_array(array_t *array)
{
	assert(array);

	free(array->store);
	free(array);
}

void *array_get(const array_t *array, size_t index)
{
	assert(index < array->size);

	return &array->store[array->elem_size * index];
}

void array_append(array_t *array, void *elem)
{
	assert(array);
	assert(elem);

	char *dst;

	if (array->size == array->capacity)
		array_resize(array, array->capacity * 2);

	dst = &array->store[array->elem_size * array->size];
	memcpy(dst, elem, array->elem_size);
	++array->size;
}
