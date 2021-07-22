/* by Michael Becker 2005
   all parts of this program are under the GNU GPL */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>

#include "complex.h"
#include "zero.h"


/* approxes zeroes of holomorphic functions with a Newton iteration which is
   modified in such a way it convergers quadratically also for zeros with
   multiplicity >1 */



/* some global variables may be set by the user */
int h_zero_maxiter=100;      /* maximum number of iterations */
int h_zero_randstart=1;      /* start with a random number in 
                                [-10,10]x[-10,10], not with z_start */
int h_zero_randregion=20; 
double h_zero_max=10000;     /* when |z|>... stop iteration */
int h_zero_restart_on_nan=1; /* if f(z) or f'(z) is nan, or h_zero_max reached,
                                restart iteration. Only effective if 
                                randstart==1 */


/* f: the function
   f1 : its first derivation
   offset: search zero of f(z)-offset, i.e. inverse image of "offset".
   z_start: start value of iteration
   result: the result of the iteration
   mulitplicity: the multiplicity of the zero
   return value: see below!
*/


int h_zero (complex_t (*f)(complex_t), complex_t (*f1)(complex_t),
            complex_t (*f2)(complex_t),
            complex_t offset,
            complex_t z_start,
            complex_t *result, int *multiplicity,
            int options)
{
  complex_t z, zo, zn; /* z, z-new, z-old */
  complex_t y, y1, yo, yn; /* f(z), f'(z), f(z-old), f(z-new) */
  complex_t dz=complex_1(), dzo=complex_1(), a;
  int err, k=1, kold=-1; /* multiplicity */
  int count=0, no_der, mult_found=0;
  double tmp, ln2=0.69314718;

  /* get some random start point */
  void c_random_start (complex_t *z) {
    z->re=(rand()%(20*h_zero_randregion)-(10*h_zero_randregion))/10.0;
    z->im=(rand()%(20*h_zero_randregion)-(10*h_zero_randregion))/10.0;
    return;
  }


  /* get a start point */
  if (h_zero_randstart) c_random_start (&z_start);
  zo = z = z_start;
  zo.re+=1.0;
  /* and its function value */

  /* now start the iteration */
  while ((c_diff(z, zo)>4*DBL_EPSILON)
         && (count<h_zero_maxiter)) {
    /* 2*DBL_EPSILON because c_diff may be up to sqrt(2)*DBL_EPSILON without
       being able to change any more */
    count++;

    /* we have: z, zo, yo, dzo */
    y=c_sub(f(z),offset);
    y1=f1(z);

#ifdef ZERO_DEBUG
    fprintf (stderr, "%i : ", count);
    print_complex_t (stderr, z);
    fprintf (stderr, " -> ");
    print_complex_t (stderr, y);
    fprintf (stderr, " : ");
    print_complex_t (stderr, y1);
    fprintf (stderr, " : %i\n", k);
#endif

    dz = c_div (y, y1);

    /* a is an aux variable for calculating the multiplicity */
    a  = c_div (dz, dzo);

    if (c_isnan(z) || c_isnan(y1) || c_isnan(dz) || c_isnan(a) ||
        (c_abs(z)>h_zero_max)) {
      if (h_zero_restart_on_nan && h_zero_randstart) {
        c_random_start (&zn);
        z=zn;
        z.re+=1;
#ifdef ZERO_DEBUG
        fprintf (stderr, "restarting\n");
#endif
        continue; /* restart iteration */
      }
      else return H_ZERO_ERR_NAN;
    }

    /* now we try to guess the multiplication of the zero */
    if ((count>1) && !mult_found)
      if ((fabs(a.re)>0.0001) && (fabs(a.im)<0.1)) {
        tmp=1.0/(1-a.re);

        if (tmp>=0) k = tmp+0.5;
        else k = tmp-0.5; /* C casting is nonsense */
        if ((k<=0) && !(options & H_ZERO_LD)) k=1;
      }

    /* now get the new z */
    if ((count>10) && (k==kold) && 
        (options & H_ZERO_MOD)) {
      mult_found=1;
      zn = c_sub(z, rc_mult(k,dz));
    }
    else {
      zn = c_sub(z, dz);
      kold=k;
    }

    dzo=dz;
    zo=z;
    z=zn;
    yo=y;
  } /* while */

  if (count<h_zero_maxiter) {
    *result=z;
    if (count>=3) *multiplicity=k;
    else *multiplicity=1; /* probably linear function */
    err=0;
  }
  else {
    err=H_ZERO_ERR_MAXITER;
  }

  if ((err==0) && (c_abs(c_sub(f(*result),offset))>0.00001)) {
    fprintf (stderr, "-----------\n");
  }

  if (*multiplicity>10) *multiplicity=1;

#ifdef ZERO_DEBUG
  if (err==0) {
    fprintf (stderr, "found zero ");
    print_complex_t (stderr, *result);
    fprintf (stderr, " with multiplcity %i\n", *multiplicity);
  }
  fprintf (stderr, "zero returns %i\n", err);
#endif
  return err;
}


/* as h_zero, but searches n zeros. returns arrays result and multiplicity 
   of size (n+1). multiplicity ends with a 0-entry. the corresponding
   entry in result is then invalid.  */

int n_h_zero (complex_t (*f)(complex_t), complex_t (*f1)(complex_t),
              complex_t offset, int n,
              complex_t z_start, 
              complex_t **result, int **multiplicity,
              int options)
{
  int no_zeros=0; /* number of zeros found */
  int pos=0;      /* next position in array */
  int err;        /* err code of h_zero */
  complex_t c_null = complex_0();
  double diff;

  /* f_n(z)=f(z)/prod(z-z_i)-offset */
  complex_t f_n (complex_t z) {
    complex_t p={1,0};
    int i;
    for (i=0; (*multiplicity)[i]; i++) {
      p=c_mult (p, c_pow_n(c_sub(z, (*result)[i]), (*multiplicity)[i]));
    }
    return c_div (c_sub(f(z),offset), p);
  }

  /* f_n'(z) = (f'(z)-f(z)*sum(k_i*1/(z-z_i)))/prod((z-z_i)^k_i) */
  complex_t f1_n (complex_t z) {
    complex_t p={1,0}, s={0,0}, diff;
    int i;
    for (i=0; (*multiplicity)[i]; i++) {
      p=c_mult (p, c_pow_n(c_sub(z, (*result)[i]), (*multiplicity)[i]));
      diff = c_sub(z, (*result)[i]);
      s=c_add(s, rc_mult((*multiplicity)[i], (c_inv(c_sub(z, (*result)[i])))));
    }
    return c_div(c_sub(f1(z), c_mult(c_sub(f(z), offset), s)), p);
  }

  if (n<=0) return H_ZERO_ERR_ILLEGALN;

#if 0
  fprintf (stderr, "offset is ");
  print_complex_t (stderr, offset);
  fprintf (stderr, "\n");
#endif

  *result = (complex_t*)calloc(2*n+1, sizeof(complex_t));
  *multiplicity = (int*)calloc(2*n+1, sizeof(int));

  while (no_zeros<n) {
    if (0==((err=h_zero(f_n, f1_n, NULL, c_null, z_start,
                  &((*result)[pos]), &((*multiplicity)[pos]), options )))) {
      if ((*multiplicity)[pos]>0) no_zeros+=abs((*multiplicity)[pos]);

      /* check again */
      diff = c_diff(f((*result)[pos]),offset);
      if ((diff>100*DBL_EPSILON) || (diff<0)) {
        err|=H_ZERO_ERR_WRONG;
        break;
      }

      pos++;
    }
    else break;
  }

  if (err) {
    free (*result);       *result=NULL;
    free (*multiplicity); *multiplicity=NULL;
  }

#if 0
  fprintf (stderr, "n_h_zero return %i\n", err);
#endif
  return err;
}


