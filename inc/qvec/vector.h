#pragma once
#ifndef QVEC_VECTOR_H
#define QVEC_VECTOR_H

// WARNING: do not pass expressions with side effects as @v
#define QVECTOR_CREATE(t, v) qvec_create(&(v), sizeof(t))

#include <stddef.h>

typedef struct qvec qvec_t;

int  qvec_create(qvec_t** v, size_t elem_size);
void qvec_free(qvec_t* v);

int qvec_append(qvec_t* v, void* data);

#endif
