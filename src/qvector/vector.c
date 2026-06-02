#include "qvec/vector.h"

#include <stddef.h>
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
    if (!*v)
	return -1;

    (*v)->data = NULL;
    (*v)->elem_size = elem_size;
    (*v)->size = 0;
    (*v)->cap = 0;

    return 0;
}

void qvec_free(qvec_t* v)
{
    free(v->data);
    free(v);
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
    if (!tmp)
	return -1;
    v->data = tmp;
    v->cap = new_cap;

    return 0;
}

/* Debug functions. Defined in "qvec/debug.h" */
void qvec_debug_print(qvec_t* v, void (*cb)(void const*))
{
    size_t i;
    for (i = 0; i < v->size; ++i) {
	cb((char*)v->data + (i * v->elem_size));
    }
}
