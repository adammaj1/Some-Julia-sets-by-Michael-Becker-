#ifndef __complex_h_
#define __complex_h_

#include <stdio.h>

#ifndef PI
#define PI 3.1415926535897323846264338327
#endif

typedef struct {
  double re;
  double im;
} complex_t;

/* Test auf Gueltigkeit */
int c_isnan (complex_t z);
int c_isinf (complex_t z);
int c_finite (complex_t z);

/* diverse Kleinigkeiten */
complex_t c_conj (complex_t z);
double c_abs2 (complex_t z);
double c_diff2 (complex_t z1, complex_t z2);
double c_abs (complex_t z);
double c_diff (complex_t z1, complex_t z2);
complex_t complex (double re, double im);
complex_t r2c (double r);
complex_t complex_0 ();
complex_t complex_1 ();
complex_t complex_i ();
int c_is_zero (complex_t z);
double c_arg (complex_t z);

/* Grundrechenarten */
complex_t c_add (complex_t z1, complex_t z2);
complex_t c_sub (complex_t z1, complex_t z2);
complex_t c_mult (complex_t z1, complex_t z2);
complex_t rc_mult (double r, complex_t z);
complex_t c_inv (complex_t z);
complex_t c_div (complex_t z1, complex_t z2);
complex_t c_neg (complex_t z);

/* diverse mathematische Funktionen */
complex_t c_pow_n (complex_t z, int n);
complex_t c_sqr (complex_t z);
complex_t c_sqrt (complex_t z);
complex_t c_exp (complex_t z);
complex_t c_log (complex_t z);
complex_t c_sinh (complex_t z);
complex_t c_cosh (complex_t z);
complex_t c_tanh (complex_t z);
complex_t c_sin (complex_t z);
complex_t c_cos (complex_t z);
complex_t c_tan (complex_t z);

/* Ein- und Ausgabe, Speichermanagement */
void print_complex_t (FILE* f, complex_t z);

#endif
