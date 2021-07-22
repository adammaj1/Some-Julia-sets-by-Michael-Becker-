/* by Michael Becker 2005
   all parts of this program are under the GNU GPL */

#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <stdlib.h>
#include <gtk/gtk.h>
#include <unistd.h>
#include <math.h>

#include "complex.h"
#include "zero.h"
#include "cpoly.h"
#include "imagearea.h"

int max_depth = 30;
double stretch_precision = 5;
double rot_precision = 2;
int max_list_len=1000;
int max_fail_len=50;
double xmin=-1.5, xmax=1.5, ymin=-1.5, ymax=1.5;
int width=750, height=750;
//double xmin=-0.55, xmax=0.55, ymin=-1.1, ymax=1.1;
//int width=500, height=1000;
int h_zero_options=0;
int use_cpoly_zero=1;
int use_f_inv=0;
cpoly_t *P, *Q, *P1, *Q1;
int draw_step=20000;


int draw_axis=0; 
double y_axis=-0.0; /* x-coordinate of y-axis */
double x_axis=-0.0; /* y-coordinate of x-axis */


extern int h_zero_maxiter;

/* Klassische Julia-Mengen */

int no_of_zeros = 2;
//complex_t c={0.252, 0.0};  // sehr schwierig
complex_t c={-0.2, -0.7};  // leicht, aber sehr schoen 
//complex_t c={0.4, -0.167};
//complex_t c={0.35, -0.4};
//complex_t c={0.35, -0.35}; // sehr schwierig
//complex_t c={-1.5, -0.1};
complex_t d={0.5, 0.5};

#if 0
complex_t f (complex_t z)
{
  return c_add(c_mult(z,z),c);
}

complex_t f1 (complex_t z)
{
  return rc_mult (2, z);
}
#endif


/* the following functions can be used, if no rational function is used,
   but another function. (Here e.g. exp(x). The first derivative and the
   inverse function in this case is to be programmed manually. */
complex_t f (complex_t z)
{
  return c_exp(z);
}
complex_t f1 (complex_t z)
{
  return c_exp(z);
}

int f_inv (complex_t z, complex_t **inv, int **mult)
{
  *inv  = (complex_t*)calloc(6, sizeof(complex_t));
  *mult = (int*)calloc(6, sizeof(int));

  (*inv)[0] = c_log(z); (*mult)[0]=1;
  (*inv)[1] = (*inv)[0]; (*inv)[1].im-=2*PI; (*mult)[1]=1;
  (*inv)[2] = (*inv)[0]; (*inv)[2].im+=2*PI; (*mult)[2]=1;
  (*inv)[3] = (*inv)[0]; (*inv)[3].im-=4*PI; (*mult)[3]=1;
  (*inv)[4] = (*inv)[0]; (*inv)[4].im+=4*PI; (*mult)[4]=1;

  return 0;
}



int draw_imagearea (GtkWidget *widget, image_area_t *ia)
{
  unsigned char *buf;

  buf=rgbbuf (ia);
  //gtk_drawing_area_size (GTK_DRAWING_AREA (widget), ia->width, ia->height);
  gdk_draw_rgb_image (widget->window, widget->style->fg_gc[GTK_STATE_NORMAL],
                      0, 0, ia->width, ia->height,
                      GDK_RGB_DITHER_NONE, buf, ia->width * 3);

  free(buf);
  //while (gtk_events_pending()) gtk_main_iteration();

  return 0;
}


/* returns the first derivative of the rational function P/Q (global
   variables) at the point z. P1 and Q1 are the first derivatives of
   the polynomials P and Q. */
complex_t cpoly_f1 (complex_t z)
{
  complex_t p, p1, q, q1;
  p  = cpoly_val(P ,z);
  p1 = cpoly_val(P1,z);
  q  = cpoly_val(Q ,z);
  q1 = cpoly_val(Q1,z);

  return c_div(c_sub(c_mult(p1,q),c_mult(p,q1)),c_sqr(q));
}


/* event handler for GTK */
gboolean on_darea_expose (GtkWidget *widget,
                 GdkEventExpose *event,
                 gpointer user_data)
{
  image_area_t *ia=(image_area_t*)user_data;

  draw_imagearea (widget, ia);
  while (gtk_events_pending()) gtk_main_iteration();

  return 0;
}


/* draws axis with midpoint at the coordinates x_axis;y_axis 
   (global variables) */
int draw_some_axis (image_area_t *ia)
{
  complex_t c=complex(y_axis, x_axis);
  int i, x, y;

  fprintf (stderr, "drawing axis...");
  x=x_coord(ia, c);
  y=y_coord(ia, c);

  fprintf (stderr, "%i : %i\n", x, y);

  for (i=0; i<ia->width; i++)
    set_point (ia, i, y, 6);
  for (i=0; i<ia->height; i++)
    set_point (ia, x, i, 6);

  return 0;
}


/* calculate the necessary precision for the pixel with coordinates i,j,
   using the derivate of the inverse function. */
int calc_precision (image_area_t *ia, int i, int j)
{
  complex_t z, *inv;
  int *mult, k, zero_err;
  double tmp, max_df, eps1, eps2, max_df2;
#if 0
  int p, p1, p2, q, q1, q2, a;
  double arg, x;
#endif

  /* search inverse images of z */
  z = z_coord(ia, i, j);

  if (use_cpoly_zero)
    zero_err = rational_zero (P, Q, z, &inv, &mult);
  else if (use_f_inv)
    zero_err = f_inv (z, &inv, &mult);
  else 
    zero_err = n_h_zero (f, f1, z, no_of_zeros, z, &inv, &mult, h_zero_options);

  if (zero_err) return -1;

  /* calculate the maximum modulus of the first derivative of the
     inverse functions */
  max_df=0;
  for (k=0; mult[k]; k++) {
    if (mult[k]>0) {
      if (use_cpoly_zero) tmp=1.0/c_abs(cpoly_f1(inv[k]));
      else tmp = 1.0/c_abs(f1(inv[k]));
      if (tmp>max_df) max_df=tmp;
    }
  }
 
  eps1 = ia->min_pixelsize/stretch_precision/max_df;

#if 0
  /* for eps2 we make a chain fraction development of f'(z)  */
  //arg = 2*PI/(c_arg((use_cpoly_zero ? cpoly_f1(z) : f1(z)))+PI);
  arg = (c_arg((use_cpoly_zero ? cpoly_f1(z) : f1(z)))+PI)/2/PI;

  if (arg>=1) arg-=1;

  x=arg;
  if (x!=0) {
    p1=0; p2=1;
    q1=1; q2=0;
    do {
      a=1/x;
      x = 1.0/x-a;
      p = p1*a+p2;
      q = q1*a+q2;
      p2=p1; p1=p;
      q2=q1; q1=q;
    } while (fabs((1.0*p)/q-arg)>1.0/rot_precision);
    eps2 = ia->min_pixelsize/q;
  }
  else {
    eps2=1.0;
  }
#endif
  /* calculate the necessary precision. */

  /* eps2 is as eps1, but we don't test, whether the derivation is big, but
     whether it is near 1. */
  max_df2=fabs(1.0/(c_abs(use_cpoly_zero ? cpoly_f1(z) : f1(z))-1));
  eps2 = ia->min_pixelsize/rot_precision/max_df2;


  //  if ((eps1<eps2) || 
  //      (c_diff( (use_cpoly_zero ? cpoly_f1(z) : f1(z)) , complex_1())>0.5)) {
  if (eps1<eps2) {
    ia->N[i][j] = 1/eps1+1;
    ia->type[i][j]=0;
  }
  else {
    //    ia->N[i][j]= q*rot_precision/eps1;
    ia->N[i][j]=1/eps2+1;
    ia->type[i][j]=1;
  }
  return 0;
}


/* calculate the necessary precisions for all pixels in the image. This
   is done only once at the start of the program. */
int get_precisions (image_area_t *ia)
{
  int i,j;

  fprintf (stderr, "calculating precisions...");

  for (i=0; i<ia->width; i++) {
    for (j=0; j<ia->height; j++) {
      calc_precision (ia, i, j);
    } /* for (j) */
  }

  fprintf (stderr, "done.\n");
  return 0;
}


/* this is a simple back iteration, which is no more used in the program. */
int simple_recurse (GtkWidget *darea, image_area_t *ia, 
                    complex_t z, int depth)
{
  int x=x_coord(ia,z) ,y=y_coord(ia,z), *mult, i, zero_err;
  static int count=0;
  complex_t *inv;

  if (depth>max_depth) return 0;

  if (depth>5) {
    if (get_point(ia, x, y)==0) set_point (ia, x, y, 4);
  }

  if (use_cpoly_zero)
    zero_err = rational_zero (P, Q, z, &inv, &mult);
  else if (use_f_inv)
    zero_err = f_inv (z, &inv, &mult);
  else 
    zero_err = n_h_zero (f, f1, z, no_of_zeros, z, &inv, &mult, h_zero_options);

  if (!zero_err) {
    count++;
    if (count%draw_step==0) {
      draw_imagearea (darea, ia);
      fprintf (stderr, ".");
      count=0;
    }

    for (i=0; mult[i]; i++) {
      if (mult[i]>0) simple_recurse (darea, ia, inv[i], depth+1);
    }

    free (inv);
    free (mult);
  }

  return zero_err;
}


/* this is the recursive back iteration, which is used at the start of the
   program to get enough values in the image, to start the non-recursive
   back iteration (function back_iter) */
int complicated_recurse (GtkWidget *darea, image_area_t *ia, 
                         complex_t z, int depth)
{
  int x=x_coord(ia,z) ,y=y_coord(ia,z), x1, y1, *mult, i, prepend;
  int outofrange, listlen, zero_err;
  static int count=0;
  complex_t *inv;
  double max_df=0.001, tmp, eps;
  GList *l;

  if (depth==max_depth) fprintf (stderr, ".");
  if(count>5000) return 0;

  if (depth>max_depth) return 0;

  if (coord_valid(ia, x, y)) {
    //    if (depth>5) set_point (ia, x, y, 1);
    if (depth>max_depth-2) set_point (ia, x, y, 2);
    outofrange=0;
  }
  else outofrange=1;


  /* get reverse images of z */
  if (use_cpoly_zero)
    zero_err = rational_zero (P, Q, z, &inv, &mult);
  else if (use_f_inv)
    zero_err = f_inv (z, &inv, &mult);
  else 
    zero_err = n_h_zero (f, f1, z, no_of_zeros, z, &inv, &mult,
                         h_zero_options);

  if (zero_err) {
    fprintf (stderr, "zero failed. error code %i at ", zero_err);
    print_complex_t (stderr, z);
    fprintf (stderr, "\n");
    return -1;
  }

  /* draw the reverse images and get the maximum modulus of the derivative
     of the inverse functions at this points. */
  for (i=0; mult[i]; i++) {
    if (mult[i]>0) {
      x1=x_coord(ia,inv[i]);
      y1=y_coord(ia,inv[i]);
      if ((depth>max_depth-2) && (get_point(ia, x1, y1)==0)) 
        set_point(ia, x1, y1, 2);
      tmp = 1.0/c_abs(use_cpoly_zero ? cpoly_f1(inv[i]) : f1(inv[i]));
      if (tmp>max_df) max_df=tmp;
    }
  }

  /* check, whether the point z is already near (modulus < eps) at a point
     we already had. If it is near, we set prepend = 0. */
  if ((!outofrange) && (!ia->all_done[x][y])) {
    eps = 0.5*ia->min_pixelsize/max_df;
    prepend=1;
    listlen=0;
    for (l=ia->data[x][y]; l; l=l->next) {
      listlen++;
      if (c_diff(z, ((ia_data_t*)(l->data))->z)<eps) {
        prepend=0;
        break;
      }
    }

    /* if necessary prepend the point to the list for coodinates (x,y)
       an recursively call this function with the inverse images. */
    if (prepend && listlen<max_list_len) {
      count++;
      if (count%draw_step==0) {
        draw_imagearea (darea, ia);
        fprintf (stderr, ".");
      }
      if (depth>max_depth-2)
        ia->data[x][y] = g_list_prepend (ia->data[x][y], new_ia_data (z));
      //      ((ia_data_t*)(ia->data[x][y]->data))->done=1;
      for (i=0; mult[i]; i++) {
        if (mult[i]>0) complicated_recurse (darea, ia, inv[i], depth+1);
      }
    } /* if (prepend) */
#if 0
    else {
      if (listlen>=max_list_len) {
        ia->all_done[x][y]=1;
        for (l=ia->data[x][y]; l; l=l->next) free(l->data);
        g_list_free (ia->data[x][y]);
      }
    }
#endif
  } /* if ((!outofrange) ... ) */

  /* fre some memory */
  free (inv);
  free (mult);
  return 0;
}

/* this is the main back iteration of the program. The function runs through
   the whole image with all its lists and calculates the reverse images of
   all points, for which this was still not done. */
int back_iter (GtkWidget *darea, image_area_t *ia)
{
  int count, count1, count2, x, y, i, *mult, x1, y1, prepend, listlen,
    deleted, zero_err;
  complex_t *inv;
  GList *l, *m;
  ia_data_t *id;

  count1=0;
  count2=0;
  do {
    count2++;
    fprintf (stderr, "%4i : ", count2);
    if (count2%100==0) draw_imagearea (darea, ia);
    count=0;
    deleted=0;

    /* run through the whole image and  all data lists. */
    for (x=0; x<ia->width; x++)
      for (y=0; y<ia->height; y++) {
        if (get_point(ia, x, y)==5) set_point (ia, x, y, 4);
        else {
          for (l=ia->data[x][y]; l; l ? l=l->next : l) {
            if ( ((id=(ia_data_t*)(l->data)))->done==0 ) {
              count++;
              count1++;
              if (count1%draw_step==0) {
                draw_imagearea (darea, ia);
                count1=0;
              }

              /* mark the point as 'done' and draw it. */
              id->done=1;
              set_point (ia, x, y, 1);

              /* get inverse images of the point. */
              if (use_cpoly_zero)
                zero_err = rational_zero (P, Q, id->z, &inv, &mult);
              else if (use_f_inv)
                zero_err = f_inv (id->z, &inv, &mult);
              else 
                zero_err = n_h_zero (f, f1, id->z, no_of_zeros, id->z, 
                                     &inv, &mult, h_zero_options);
              if (zero_err) {
                continue;
              }


              for (i=0; mult[i]; i++) {
                if (mult[i]<=0) continue;

                /* sometimes the zero-algorithms don't work well. so we check
                   here, whether we really have  an inverse image. */
                if (use_cpoly_zero && 
                    (c_abs(c_sub(rational_val(P, Q, inv[i]), id->z))>0.001)) {
                  //fprintf (stderr, "-----------------------\n");
                  //print_complex_t (stderr, id->z);
                  //fprintf (stderr, "\n");
                  continue;
                }
                if (!use_cpoly_zero && 
                    (c_abs(c_sub(f(inv[i]), id->z))>0.001)) {
                  fprintf (stderr, "-----------------------\n");
                  print_complex_t (stderr, id->z);
                  fprintf (stderr, "\n");
                  continue;
                }

                /* x1 and y1 are the coordinates of the inverse image in the
                   image. */
                x1=x_coord(ia,inv[i]);
                y1=y_coord(ia,inv[i]);

                if (coord_valid (ia, x1, y1) && (ia->all_done[x1][y1]<=max_fail_len)) {

                  /* test whether inv[i] is to be prepended */
                  if (ia->N[x1][y1]<=0) /* something went wrong */
                    calc_precision (ia, x1, y1);
                  prepend=1;
                  listlen=0;
                  for (m=ia->data[x1][y1]; m; m=m->next) {
                    listlen++;
                    if (c_diff(inv[i], 
                               ((ia_data_t*)(m->data))->z)<1.0/ia->N[x1][y1]) {
                      prepend=0;
                      break;
                    }
                  } /* for (m) */
                  if (prepend && (listlen<max_list_len)) {
                    /* if we are here, the inverse image has to be prepended
                       to the list, and worked with in the next run. */
                    ia->all_done[x1][y1]=0;
                    ia->data[x1][y1] = g_list_prepend (ia->data[x1][y1], 
                                                       new_ia_data (inv[i]));
                    if (ia->type[x1][y1]==0) {
                      set_point (ia, x1, y1, 2);
                    }
                    else {
                      set_point (ia, x1, y1, 3);
                    }
                  }
                  else { /* check whether we can free some memory */
                    ia->all_done[x1][y1]++;
                    if ((ia->all_done[x1][y1]>max_fail_len) ||
                        (listlen>=max_list_len)) {
                      //                    ia->all_done[x1][y1]=1;
                      set_point (ia, x1, y1, 5);
                      for (l=ia->data[x1][y1]; l; l=l->next) free(l->data);
                      g_list_free (ia->data[x1][y1]);
                      ia->data[x1][y1]=NULL;
                      ia->all_done[x1][y1]=max_fail_len+2;
                      deleted+=listlen;
                    }
                  }
                }
              } /* for (i) */
              free (mult);
              free (inv);
            } /* if (done==0) */
          } /* for (l) */
        } /* else */
      } /* for y */

    fprintf (stderr, "%i new, %i deleted\n", count, deleted);
  } /* do */
  while (count);

  return 0;
}


/* make the still colored pictures black and white */
int paint_white (image_area_t *ia) 
{
  int x, y;

  for (x=0; x<ia->width; x++)
    for (y=0; y<ia->height; y++) 
      if ((ia->a[x][y]) && (ia->a[x][y]<6)) ia->a[x][y]=1;
  return 0;
}


int main (int argc, char *argv[])
{
  GtkWidget *window, *darea;
  image_area_t *ia=new_image_area (width, height ,xmin, xmax, ymin, ymax);
  complex_t z=complex(0.04, -0.18); // ???
  //complex_t e;
  //double u, t;
  //int i;
  
  
  
  
   /* 
   This is called in all GTK applications. Arguments are parsed
   from the command line and are returned to the application. 
   */
  gtk_init (&argc, &argv);
  gdk_rgb_init();
  window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  darea = gtk_drawing_area_new();
  gtk_drawing_area_size (GTK_DRAWING_AREA (darea), width, height);
  gtk_container_add (GTK_CONTAINER (window), darea);
  gtk_widget_show_all (window);
  gtk_signal_connect (GTK_OBJECT (darea), "expose_event",  
                    GTK_SIGNAL_FUNC (on_darea_expose), ia);

  gtk_widget_show_all (window);

  if (draw_axis) draw_some_axis(ia);


#if 0
  P=new_cpoly (3);
  P->a[3]=rc_mult(2,d);
  P->a[2]=r2c(2);
  P->a[1]=complex( 0.0, 0);
  P->a[0]=complex( 0.0, 0);

  Q=new_cpoly (3);
  Q->a[3]=complex( 1.0, 0);
  Q->a[2]=rc_mult(3,d);
  Q->a[1]=complex( 1.0, 0);
  Q->a[0]=c_neg(d);
#endif

  /* classical Julia sets */

#if 0
  //u=2*PI*(sqrt(5)-1)/2;
  u=2*PI*0.1;

  P=new_cpoly (2);
  P->a[2]=complex(1, 0);
  P->a[1]=complex(0, 0);
  P->a[0]=complex(-0.22, -0.7);

  Q=new_cpoly (0);
  Q->a[0]=complex_1(1,0);
#endif

 // t=0.6;
 // u=0;
  //  e=complex (-0.1464,0.35356);
 // e=complex (1.2,0.3);
  //  e=complex (1.2);


  // rational map 
  P=new_cpoly (10);
  P->a[10]=complex(1, 0);
  P->a[1]=complex(-1,0.0);
  fprint_cpoly(stdout, "P",  P); 


  Q=new_cpoly (0);
  //  Q->a[2]=complex(1,  0);
  Q->a[0]=complex(1,0);
  fprint_cpoly(stdout, "Q", Q); 

  P1 = cpoly_diff (P);
  fprint_cpoly(stdout,"P1 = diff(P) = ", P1); 
  
  Q1 = cpoly_diff (Q);
  fprint_cpoly(stdout, "Q1 = diff(Q) = ", Q1); 
  fprintf(stdout, "\nrational map R = P/Q \n"); 
  


  /* komplizierte rueckwaerts-iteration */

  h_zero_maxiter = 1000;
  h_zero_options= H_ZERO_MOD;

  complicated_recurse (darea, ia, z, 0);
  draw_imagearea (darea, ia);
  fprintf (stderr, "complicated recursing done\n");

  back_iter (darea, ia);
  fprintf (stderr, "back iteration done\n");


  /* make the still colored pictures black and white  */
  paint_white (ia);
  draw_imagearea (darea, ia);
  fprintf (stderr, "white image ready!\n");
  
  
  /* All GTK applications must have a gtk_main(). Control ends here
     * and waits for an event to occur (like a key press or
     * mouse event). */
  gtk_main();
  
  
  return 0;
}

