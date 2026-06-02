#include "qvec/vector.h"

#include <errno.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct qvec {
	void*  data;
	size_t elem_size, size, cap;
};

static int _qvec_ensure_cap(qvec_t* v, size_t add);

int qvec_create(qvec_t** v, size_t elem_size)
{
	*v = malloc(sizeof(**v));
	if (!*v) {
		errno = ENOMEM;
		return -1;
	}

	(*v)->data = NULL;
	(*v)->elem_size = elem_size;
	(*v)->size = 0;
	(*v)->cap = 0;

	return 0;
}

void qvec_clear(qvec_t* v)
{
	v->size = 0;
}

void qvec_free(qvec_t* v)
{
	free(v->data);
	free(v);
}

void* qvec_get(qvec_t const* v, size_t i)
{
	if (i >= v->size) {
		errno = ERANGE;
		return NULL;
	}

	return (char*)v->data + (i * v->elem_size);
}

int qvec_append(qvec_t* v, void* data)
{
	void* dest;

	if (_qvec_ensure_cap(v, 1) != 0)
		return -1;

	// final elem
	dest = (char*)v->data + (v->size * v->elem_size);
	memcpy(dest, data, v->elem_size);
	v->size++;

	return 0;
}

int qvec_prepend(qvec_t* v, void* data)
{
	if (_qvec_ensure_cap(v, 1) != 0)
		return -1;

	memmove((char*)v->data + v->elem_size, v->data, v->size * v->elem_size);

	// insert at first elem
	memcpy(v->data, data, v->elem_size);
	v->size++;

	return 0;
}

int qvec_insert(qvec_t* v, size_t i, void* data)
{
	void*  blk;
	size_t blk_size_b;

	if (i > v->size) {
		errno = ERANGE;
		return -1;
	}

	if (_qvec_ensure_cap(v, 1) != 0)
		return -1;

	blk = (char*)v->data + (i * v->elem_size);
	blk_size_b = (v->size - i) * v->elem_size;
	memmove((char*)blk + v->elem_size, blk, blk_size_b);

	memcpy(blk, data, v->elem_size);
	v->size++;

	return 0;
}

void qvec_pop(qvec_t* v)
{
	v->size--;
}
// implement this later..
void qvec_remove(qvec_t* v, size_t i)
{
}

size_t qvec_size(qvec_t const* v)
{
	return v->size;
}

void* qvec_data(qvec_t const* v)
{
	return v->data;
}

size_t qvec_cap(qvec_t const* v)
{
	return v->cap;
}

int qvec_reserve(qvec_t* v, size_t new_cap)
{
	void* tmp;

	tmp = realloc(v->data, new_cap * v->elem_size);
	if (!tmp) {
		errno = ENOMEM;
		return -1;
	}

	v->data = tmp;
	v->cap = new_cap;

	return 0;
}

int qvec_shrink(qvec_t* v)
{
	void* tmp;

	tmp = realloc(v->data, v->size * v->elem_size);
	if (!tmp) {
		errno = ENOMEM;
		return -1;
	}

	v->data = tmp;
	v->cap = v->size;

	return 0;
}

// implement these two later...
void* qvec_find(qvec_t const* v, void const* key, int (*cmp_cb)(void const*, void const*))
{
	return NULL;
}
void qvec_sort(qvec_t* v, int (*cmp_cb)(void const*, void const*))
{
}

/* Debug functions. Defined in "qvec/debug.h" */
void qvec_debug_print(qvec_t* v, void (*cb)(void const*))
{
	size_t i;
	for (i = 0; i < v->size; ++i) {
		cb((char*)v->data + (i * v->elem_size));
	}
}

static int _qvec_ensure_cap(qvec_t* v, size_t add)
{
	size_t req, new_cap;
	void*  tmp;

	if (add <= v->cap - v->size)
		return 0;

	req = v->size + add;
	for (new_cap = v->cap ? v->cap : 1; new_cap < req; new_cap *= 2)
		;

	tmp = realloc(v->data, new_cap * v->elem_size);
	if (!tmp) {
		errno = ENOMEM;
		return -1;
	}
	v->data = tmp;
	v->cap = new_cap;

	return 0;
}
