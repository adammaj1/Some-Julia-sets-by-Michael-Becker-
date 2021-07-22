/* by Michael Becker 2005
   all parts of this program are under the GNU GPL */

#include <malloc.h>
#include <math.h>
#include "complex.h"
#include "zero.h"
#include "cpoly.h"

/* function to work with complex polynomials. */

/*****************************************************************/
/* auxilliary functions */

int imax (int a, int b)
{
  if (a>b) return a;
  return b;
}

/*****************************************************************/
/* memory management */
cpoly_t *new_cpoly (int deg)
{
  int i;
  cpoly_t *res=(cpoly_t*)malloc(sizeof(cpoly_t));
  if (deg<0) deg=0;
  res->deg=deg;
  res->a=(complex_t*)malloc(sizeof(complex_t)*(deg+1));
  for (i=0; i<=deg; i++) 
  	res->a[i]=complex_0();
  return res;
}

void free_cpoly (cpoly_t *p)
{
  free (p->a);
  free (p);
  return;
}

cpoly_t *copy_cpoly (cpoly_t *p)
{
  int i;
  cpoly_t *res=new_cpoly (p->deg);
  for (i=0; i<=p->deg; i++) res->a[i]=p->a[i];
  return res;
}

/*****************************************************************/
/* basic operations */

/* the value of the polynomial p at point z. */
complex_t cpoly_val (cpoly_t *p, complex_t z)
{
  int i=p->deg-1;
  complex_t res=p->a[p->deg];
  for (; i>=0; i--)
    res=c_add(p->a[i],c_mult(res,z));
  return res;
}

/* value of rational function p/q at point z. */
complex_t rational_val (cpoly_t *p, cpoly_t *q, complex_t z)
{
  return c_div(cpoly_val(p,z), cpoly_val(q,z));
}

/* mutiplicate complex c and polynomial p. */
cpoly_t *c_mult_poly (complex_t c, cpoly_t *p)
{
  int i;
  cpoly_t *res=new_cpoly(p->deg);
  for (i=0; i<=res->deg; i++) res->a[i]=c_mult(c,p->a[i]);
  return res;
}

/* add two polynomials */
cpoly_t *cpoly_add (cpoly_t *p1, cpoly_t *p2)
{
  int i;
  cpoly_t *res;

  if (p2->deg>p1->deg) {res=p1; p1=p2; p2=res;}
  /* now p1 has higher degree */

  res = copy_cpoly (p1);
  for (i=0; i<=p2->deg; i++) res->a[i]=c_add(p1->a[i], p2->a[i]);
  /* Other coefficients remain unchanged */

  /* get new degree */
  while ((c_abs(res->a[res->deg])<2*DBL_EPSILON) && (res->deg>0))
    res->deg--;

  return res;
}

/* first derivativ of polynomial p */
cpoly_t *cpoly_diff (cpoly_t *p)
{
  int i;
  cpoly_t *res=new_cpoly (p->deg-1);
  for (i=0; i<p->deg; i++)
    res->a[i]=rc_mult(i+1.0, p->a[i+1]);
  return res;
}

/*****************************************************************/
/* polynomial divisions */

void cpoly_div (cpoly_t *p, cpoly_t *q, cpoly_t **d, cpoly_t **r)
{
  complex_t z;
  int i;

  *r=copy_cpoly(p);
  *d=new_cpoly(p->deg-q->deg);

  while ((*r)->deg>=q->deg) {
    z=c_div((*r)->a[(*r)->deg], q->a[q->deg]);
    (*d)->a[(*r)->deg-q->deg]=z;

    for (i=0; i<=q->deg; i++)
      (*r)->a[i+(*r)->deg-q->deg]=
        c_sub((*r)->a[i+(*r)->deg-q->deg], c_mult(z, q->a[i]));
    (*r)->deg--;
  }

  if ((*r)->deg<0) (*r)->deg=0;

  return;
}



/*****************************************************************/
/* searching roots of rational functions */

/* searches for the inverse images of 'offset' under polynomial cp.
   returns them in the array result. multiplicity contains the
   multiplicities. The arrays stop with multiplicity[..]==0. */
int cpoly_zero (cpoly_t *cp, complex_t offset, 
                 complex_t **result, int **multiplicity)
{
  cpoly_t *p, *p1, *linear_fact, *tmp_d, *tmp_r;
  int no_zeros=0, i, err=0, pos=0;
  complex_t z_start={0.3141516, -0.278765}, c_null=complex_0();

  complex_t f (complex_t z) {
    return cpoly_val (p,z);
  }
  complex_t f1 (complex_t z) {
    return cpoly_val (p1,z);
  }


  /* we search zeros of cp-offset. */
  p  = copy_cpoly (cp); p->a[0]=c_sub(p->a[0],offset);
  p1 = cpoly_diff (p);

  /* for thee polynomial division we need lineaf factors (z-z_0) this are
     stored in linear_fact. */
  linear_fact = new_cpoly(1);
  linear_fact->a[1]=complex_1();

  *result = (complex_t*)calloc(p->deg+1, sizeof(complex_t));
  *multiplicity = (int*)calloc(p->deg+1, sizeof(int));

  while (no_zeros<cp->deg) { /* still zeros left */
    if ((0==((err=h_zero(f, f1, NULL, c_null, z_start,
                        &((*result)[pos]), &((*multiplicity)[pos]),
                        0 )))) &&
        ((*multiplicity)[pos]>0)) {

      no_zeros+=(*multiplicity)[pos];
      /* make some polynomial divisions */
      linear_fact->a[0]=c_neg((*result)[pos]);
      for (i=0; i<(*multiplicity)[pos]; i++) {
        cpoly_div (p, linear_fact, &tmp_d, &tmp_r);
        free_cpoly (tmp_r);
        free_cpoly (p);
        p=tmp_d;
      }
      free_cpoly (p1);
      p1 = cpoly_diff(p);

      pos++;
    }
    else break;
  }
  free_cpoly (p);
  free_cpoly (p1);
  free_cpoly (linear_fact);

  if (err) {
    free (*result);       *result=NULL;
    free (*multiplicity); *multiplicity=NULL;
  }
  return err;
}

/* searches inverse images of offset under p/q. */
int rational_zero (cpoly_t *p, cpoly_t *q, complex_t offset,
                   complex_t **result, int **multiplicity)
{
  cpoly_t *a, *tmp1;
  int err, i;
  double k;

  /* we look for zeros of p-offset*q. */
  tmp1 = c_mult_poly (c_neg(offset), q);
  a = cpoly_add (p, tmp1);
  free_cpoly(tmp1);

  err = cpoly_zero (a, complex_0(), result, multiplicity);

  free_cpoly (a);

  if (err==0) {
    for (i=0; (*multiplicity)[i]; i++) {
      k=c_abs(c_sub(rational_val(p, q, (*result)[i]), offset));
      if ((k/DBL_EPSILON>100000) || (k<0)) {
        err = -1;
        continue;
      }

      if (c_abs(c_sub(rational_val(p, q, (*result)[i]), offset))>0.001) {
        fprintf (stderr, "-----------------------\n");
        print_complex_t (stderr, c_sub(rational_val(p, q, (*result)[i]), offset));
        fprintf (stderr, "\n%f\n", k);
        err = -2;
      }

    }
  }

  return err;
}


/*****************************************************************/
/* io */

void fprint_cpoly (FILE *f, char *comment, cpoly_t *p)
{
  int i;
   fprintf (f, "Polynomial %s \t", comment);
  for (i=p->deg; i>=0; i--) {
  
  	complex_t c = p->a[i];
  
  	if (! c_is_0(c)){
    		print_complex_t (f, c);
    		if (i!=0) {fprintf (f, "*z^%i\t", i);}
    		}
  }
   fprintf (f, "\n");
  return;
}

/*****************************************************************/
/*****************************************************************/
