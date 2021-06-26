#ifndef __zero_h_
#define __zero_h_

#include <stdio.h>

#include "complex.h"

/* errors */
#define H_ZERO_ERR_MAXITER 1
#define H_ZERO_ERR_NAN 2
#define H_ZERO_ERR_ILLEGALN 4
#define H_ZERO_ERR_NODER 8
#define H_ZERO_ERR_OPTIONS 16
#define H_ZERO_ERR_WRONG 32

/* options */
#define H_ZERO_MOD 1    /* modified newton */
#define H_ZERO_NODER 2  /* use no derivative */
#define H_ZERO_LD 4     /* use f/f' not f */

int h_zero (complex_t (*f)(complex_t), complex_t (*f1)(complex_t),
            complex_t (*f2)(complex_t),
            complex_t offset,
            complex_t z_start,
            complex_t *result, int *multiplicity,
            int options);

int n_h_zero (complex_t (*f)(complex_t), complex_t (*f1)(complex_t),
              complex_t offset, int n,
              complex_t z_start, 
              complex_t **result, int **multiplicity,
              int options);

#endif
