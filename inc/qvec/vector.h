#ifndef QVEC_VECTOR_H
#define QVEC_VECTOR_H

#include <stddef.h>

/**
 * @brief Convenience macro to create a vector.
 *
 * @param t Type of vector elements
 * @param v Qvec pointer
 */
#define QVEC_CREATE(t, v) qvec_create(&(v), sizeof(t));

typedef struct qvec qvec_t;

/* Lifecycle management */

/**
 * @brief Allocate and initialize a new vector.
 *
 * @param v       Output parameter; set to the newly allocated vector on success.
 * @param elem_size Size in bytes of each element.
 * @return 0 on success, -1 on failure with errno set to ENOMEM.
 */
int qvec_create(qvec_t** v, size_t elem_size);

/**
 * @brief Free all memory associated with a vector.
 *
 * @param v Vector to free. Behavior is undefined if v is NULL.
 */
void qvec_free(qvec_t* v);

/**
 * @brief Remove all elements from a vector without freeing its allocation.
 *
 * @param v Vector to clear. Behavior is undefined if v is NULL.
 */
void qvec_clear(qvec_t* v);

/* CRUD operations */

/**
 * @brief Return a pointer to the element at index i.
 *
 * @param v Vector to index into.
 * @param i Zero-based index of the element.
 * @return Pointer to the element on success, NULL if i is out of bounds with errno set to ERANGE.
 */
void* qvec_get(qvec_t const* v, size_t i);

/**
 * @brief Append a copy of data to the end of the vector.
 *
 * @param v    Vector to append to.
 * @param data Pointer to the element to copy. Must be at least elem_size bytes.
 * @return 0 on success, -1 on failure with errno set to ENOMEM.
 */
int qvec_append(qvec_t* v, void* data);

/**
 * @brief Prepend a copy of data to the front of the vector.
 *
 * @param v    Vector to prepend to.
 * @param data Pointer to the element to copy. Must be at least elem_size bytes.
 * @return 0 on success, -1 on failure with errno set to ENOMEM.
 */
int qvec_prepend(qvec_t* v, void* data);

/**
 * @brief Insert a copy of data at index i, shifting subsequent elements right.
 *
 * @param v    Vector to insert into.
 * @param i    Index at which to insert. Must be <= qvec_size(v).
 * @param data Pointer to the element to copy. Must be at least elem_size bytes.
 * @return 0 on success, -1 on failure with errno set to ENOMEM or ERANGE.
 */
int qvec_insert(qvec_t* v, size_t i, void* data);

/**
 * @brief Remove the last element from the vector.
 *
 * @param v Vector to pop from. Behavior is undefined if the vector is empty.
 */
void qvec_pop(qvec_t* v);

/**
 * @brief Remove the element at index i, shifting subsequent elements left.
 *
 * @param v Vector to remove from.
 * @param i Index of the element to remove. Behavior is undefined if i is out of bounds.
 */
void qvec_remove(qvec_t* v, size_t i);

/* Inspection and utility */

/**
 * @brief Return the number of elements in the vector.
 *
 * @param v Vector to inspect.
 * @return Number of elements.
 */
size_t qvec_size(qvec_t const* v);

/**
 * @brief Return a pointer to the underlying data buffer.
 *
 * @param v Vector to inspect.
 * @return Pointer to the raw element buffer, or NULL if the vector has no allocation.
 */
void* qvec_data(qvec_t const* v);

/**
 * @brief Return the current capacity of the vector in number of elements.
 *
 * @param v Vector to inspect.
 * @return Capacity in elements.
 */
size_t qvec_cap(qvec_t const* v);

/**
 * @brief Reserve capacity for at least new_cap elements.
 *
 * Does not shrink the vector. To shrink, use qvec_shrink().
 *
 * @param v       Vector to reserve capacity for.
 * @param new_cap Desired capacity in elements.
 * @return 0 on success, -1 on failure with errno set to ENOMEM.
 */
int qvec_reserve(qvec_t* v, size_t new_cap);

/**
 * @brief Shrink the allocation to exactly fit the current number of elements.
 *
 * @param v Vector to shrink.
 * @return 0 on success, -1 on failure with errno set to ENOMEM.
 */
int qvec_shrink(qvec_t* v);

/**
 * @brief Perform an action for each element of the vector
 *
 * @param v Vector to iterate.
 * @param cb Callback function. Takes a const void* to the element
 */
void qvec_foreach(qvec_t* v, void (*cb)(void const*));

/* Searching and sorting */

/**
 * @brief Search for an element matching key using a linear scan.
 *
 * Analogous to lfind(3). Iterates from index 0 and returns the first match.
 *
 * @param v      Vector to search.
 * @param key    Pointer to the value to search for.
 * @param compar Comparator function; returns 0 if elements are equal.
 * @return Pointer to the matching element in the buffer, or NULL if not found.
 */
void* qvec_find(qvec_t const* v, void const* key, int (*compar)(void const*, void const*));

/**
 * @brief Sort the vector in place using the given comparator.
 *
 * Thin wrapper around qsort(3). The comparator semantics are identical.
 *
 * @param v      Vector to sort.
 * @param compar Comparator function; returns negative, zero, or positive.
 */
void qvec_sort(qvec_t* v, int (*compar)(void const*, void const*));

#endif /* QVEC_VECTOR_H */
