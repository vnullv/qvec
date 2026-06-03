#include "qvec/vector.h"

#include <stdio.h>

void print_int(void const* v)
{
	if (!v)
		return;
	printf("%d\n", *(int*)v);
}

int int_compar(void const* a, void const* b)
{
	int x = *(int const*)a;
	int y = *(int const*)b;

	if (x == y)
		return 0;

	return (x < y) ? -1 : 1;
}

int main(void)
{
	qvec_t* v;
	QVEC_CREATE(int, v);

	int x = 199;
	qvec_append(v, &x);
	qvec_append(v, &x);
	qvec_append(v, &x);
	qvec_append(v, &x);
	qvec_append(v, &x);
	qvec_append(v, &x);

	qvec_remove(v, 2);

	x = 2;
	qvec_insert(v, qvec_size(v), &x);

	qvec_foreach(v, print_int);
	qvec_free(v);
}
