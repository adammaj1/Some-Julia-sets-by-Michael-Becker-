/* by Michael Becker 2005
   all parts of this program are under the GNU GPL */

#include <stdio.h>
#include <math.h>
#include "complex.h"

/*
complex arithmetic
some functions (abs, inv and div) according to "numerical recipes in C".
*/

/***********************************************************************/
/* several small things */

/* test for validy */
int c_isnan (complex_t z)
{
  return (isnan(z.re) || isnan(z.im));
}
int c_isinf (complex_t z)
{
  return (isinf(z.re) || (isinf(z.im)));
}
int c_finite (complex_t z)
{
  return (finite(z.re) && finite(z.im));
}



// new 
int d_is_0(double x){

	
	return (fpclassify(x)==FP_ZERO) ;
		
}

// new 
int c_is_0(complex_t z)
{
	
	
	return (d_is_0(z.re) && d_is_0(z.im));
	

}


int c_clasify(complex_t z){

	int r = 0;
	if (! d_is_0(z.re)) {r+=1;}
	if (! d_is_0(z.im)) {r+=1;}
	return r;
	
	
}

/* complex conjugated */
complex_t c_conj (complex_t z)
{
  complex_t res;
  res.re =  z.re;
  res.im = -z.im;
  return res;
}

/* abs square */
double c_abs2 (complex_t z)
{
  return z.re*z.re + z.im*z.im;
}

/* abs square of the difference */
double c_diff2 (complex_t z1, complex_t z2)
{
  double dx=z2.re-z1.re, dy=z2.im-z1.im;
  return (dx*dx+dy*dy);
}

/* absolute valute */
double c_abs (complex_t z)
{
  double x=fabs(z.re),y=fabs(z.im), res, tmp;
  if (x==0.0) res=y;
  else if (y==0.0) res=x;
  else if (x>y) {
    tmp=y/x;
    res=x*sqrt(1.0+tmp*tmp);
  } else {
    tmp=x/y;
    res=y*sqrt(1.0+tmp*tmp);
  }
  return res;
#if 0
  return sqrt(c_abs2(z));
#endif
}

/* abs of the difference */
double c_diff (complex_t z1, complex_t z2)
{
  return c_abs(c_sub(z1,z2));
}

/* (real,real)->complex cast */
complex_t complex (double re, double im)
{
  complex_t z;
  z.re=re;
  z.im=im;
  return z;
}

/* real->complex cast */
complex_t r2c (double r)
{
  complex_t res;
  res.re = r;
  res.im =0.0;
  return res;
}

/* null, one and i -element */
complex_t complex_0 ()
{
  complex_t res;
  res.re = res.im = 0;
  return res;
}

complex_t complex_1 ()
{
  complex_t res;
  res.re = 1;
  res.im = 0;
  return res;
}

complex_t complex_i ()
{
  complex_t res;
  res.re=0;
  res.im=1;
  return res;
}

/* test for null */
int c_is_zero (complex_t z)
{
  return ((z.re==0) && (z.im==0));
}

/* argument (angel, between -pi (ex-) and +pi (inclusively).
   for z=0  0 is returned */
double c_arg (complex_t z)
{
  if (z.re>0) return (atan(z.im/z.re));
  if (z.im>0) return (atan(-z.re/z.im) + PI/2);
  if (z.im<0) return (atan(-z.re/z.im) - PI/2);
  if (z.re<0) return PI;
  else return 0;
}

/***********************************************************************/
/* basic operations */

/* addition */
complex_t c_add (complex_t z1, complex_t z2)
{
  complex_t res;
  res.re = z1.re + z2.re;
  res.im = z1.im + z2.im;
  return (res);
}

/* subtraction */
complex_t c_sub (complex_t z1, complex_t z2)
{
  complex_t res;
  res.re = z1.re - z2.re;
  res.im = z1.im - z2.im;
  return (res);
}

/* multiplication */
complex_t c_mult (complex_t z1, complex_t z2)
{
  complex_t res;
  res.re = z1.re*z2.re - z1.im*z2.im;
  res.im = z1.re*z2.im + z1.im*z2.re;
  return res;
}

/* real-complex multiplication */
complex_t rc_mult (double r, complex_t z)
{
  complex_t res;
  res.re = r*z.re;
  res.im = r*z.im;
  return res;
}

/* multiplicative inverse */
complex_t c_inv (complex_t z)
{
  complex_t res;
#if 0
  double r, d;

  if (fabs(z.re) >= fabs(z.im)) {
    r=z.im/z.re;
    d=z.re+r*z.im;
    res.re=1.0/d;
    res.im=-r/d;
  } else {
    r=z.re/z.im;
    d=z.im+r*z.re;
    res.re=r/d;
    res.im=-1.0/d;
  }
#endif

  double a=c_abs2(z);
  res.re=z.re/a;
  res.im=-z.im/a;

  return res;
}

/* division */
complex_t c_div (complex_t z1, complex_t z2)
{
#if 0
  complex_t res;
  double r, d;

  if (fabs(z2.re) >= fabs(z2.im)) {
    r=z2.im/z2.re;
    d=z2.re+r*z2.im;
    res.re=(z1.re+r*z1.im)/d;
    res.im=(z1.im-r*z1.re)/d;
  } else {
    r=z2.re/z2.im;
    d=z2.im+r*z2.re;
    res.re=(z1.re*r+z1.im)/d;
    res.im=(z1.im*r-z1.re)/d;
  }
  return res;
#endif
  return c_mult(z1,c_inv(z2));
}

/* negative */
complex_t c_neg (complex_t z)
{
  complex_t res;
  res.re = -z.re;
  res.im = -z.im;
  return res;
}

/************************************************************************/
/* several mathematical functions */

/* to the power of a natural number */
complex_t c_pow_n (complex_t z, int n)
{
  complex_t res;
  int sign= (n<0 ? (n=-n, -1) : 1);
  res=complex_1();
  while (n--) res=c_mult(res,z);
  if (sign<0) res=c_inv(res);
  return res;
}

/* square */
complex_t c_sqr (complex_t z)
{
  return c_mult (z, z);
}

/* square root with re>0 or (re=0 and im>=0) */
complex_t c_sqrt (complex_t z)
{
  complex_t res;
  double x=fabs(z.re), y=fabs(z.im), w, r;

  if ((z.re==0.0) && (z.im==0.0)) return complex_0();
  if (x>=y) {
    r=y/x;
    w=sqrt(x)*sqrt(0.5*(1.0+sqrt(1.0+r*r)));
  } else {
    r=x/y;
    w=sqrt(y)*sqrt(0.5*(r+sqrt(1.0+r*r)));
  }
  if (z.re>=0.0) {
    res.re=w;
    res.im=z.im/(2.0*w);
  } else {
    res.im=(z.im >= 0) ? w : -w;
    res.re=z.im/(2.0*res.im);
  }
  return res;
}

/* exponential function */
complex_t c_exp (complex_t z)
{
  complex_t res;
  double tmp=exp(z.re);
  res.re = tmp*cos(z.im);
  res.im = tmp*sin(z.im);
  return res;
}

/* natural log. for the branch of the log compare c_arg */
complex_t c_log (complex_t z)
{
  complex_t res;
  res.re = log(c_abs(z));
  res.im = c_arg(z);
  return res;
}

/* trigonometric functions */
complex_t c_sinh (complex_t z)
{
  return rc_mult (0.5, c_sub(c_exp(z), c_exp(c_neg(z))));
}
complex_t c_cosh (complex_t z)
{
  return rc_mult (0.5, c_add(c_exp(z), c_exp(c_neg(z))));
}
complex_t c_tanh (complex_t z)
{
  return c_div(c_sinh(z), c_cosh(z));
}

complex_t c_sin (complex_t z)
{
  return c_div(c_sinh(c_mult(complex_i(), z)), complex_i());
}
complex_t c_cos (complex_t z)
{
  return c_cosh(c_mult(complex_i(), z));
}
complex_t c_tan (complex_t z)
{
  return c_div(c_sin(z), c_cos(z));
}

/*************************************************************************/
/* input, output, memory management */

void print_complex_t (FILE *f, complex_t z)
{
	int i =  c_clasify(z);
	switch(i) {
  		
  		case 0: fprintf (f, "0.0"); break;
  		case 1: if (d_is_0(z.re)) 
  				{fprintf (f, "%+f*i",  z.im);}
  				else {fprintf (f, "%+f", z.re);}
  			break;
  		case 2: fprintf (f, "%+f%+f*i", z.re, z.im);
  		
  		}
 
}
