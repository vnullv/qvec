#include "qvec/debug.h"
#include "qvec/vector.h"

#include <stdio.h>

void print_int(void const* v)
{
	if (!v)
		return;
	printf("%d\n", *(int*)v);
}

int main(void)
{
	qvec_t* v;
	QVECTOR_CREATE(int, v);

	int x = 199;
	qvec_append(v, &x);
	qvec_append(v, &x);
	qvec_append(v, &x);
	qvec_append(v, &x);
	qvec_append(v, &x);
	qvec_append(v, &x);

	x = 2;
	qvec_insert(v, 6, &x);

	qvec_debug_print(v, print_int);
	qvec_free(v);
}
