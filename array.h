#ifndef ARRAY_H
#define ARRAY_H

#include <stddef.h> /* size_t */

typedef struct array {
	char *store;
	size_t elem_size;
	size_t capacity;
	size_t size;
} array_t;

array_t *new_array(size_t cap, size_t elem_size);
void delete_array(array_t *array);
void *array_get(const array_t *array, size_t index);
void array_append(array_t *array, void *elem);

#endif /* ARRAY_H */
