#ifndef __cpoly_h_
#define __cpoly_h_

#include <stdio.h>
#include "complex.h"

#ifndef DBL_EPSILON
#define DBL_EPSILON 0.000000001
#endif

typedef struct {
  int deg;
  complex_t *a;
} cpoly_t;


/* memory management */
cpoly_t *new_cpoly (int deg);
void free_cpoly (cpoly_t *p);
cpoly_t *copy_cpoly (cpoly_t *p);

/* basic operations */
complex_t cpoly_val (cpoly_t *p, complex_t z);
complex_t rational_val (cpoly_t *p, cpoly_t *q, complex_t z);
cpoly_t *c_mult_poly (complex_t c, cpoly_t *p);
cpoly_t *cpoly_add (cpoly_t *p1, cpoly_t *p2);
cpoly_t *cpoly_diff (cpoly_t *p);

/* polynomial divisions */
void cpoly_div (cpoly_t *p, cpoly_t *q, cpoly_t **d, cpoly_t **r);

/* searching roots of rational functions */
int cpoly_zero (cpoly_t *cp, complex_t offset, 
                complex_t **result, int **multiplicity);

/* io */
void fprint_cpoly (FILE *f, char *comment, cpoly_t *p);
int rational_zero (cpoly_t *p, cpoly_t *q, complex_t offset,
                   complex_t **result, int **multiplicity);

#endif
