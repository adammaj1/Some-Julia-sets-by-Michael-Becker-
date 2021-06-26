/* by Michael Becker 2005
   all parts of this program are under the GNU GPL */

#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <stdlib.h>

#include <math.h>

#include "zero.h"
#include "imagearea.h"

/**************************************************************************/
/* Speicherverwaltung / memory management */


ia_data_t *new_ia_data (complex_t z)
{
  ia_data_t *res=(ia_data_t*)malloc(sizeof(ia_data_t));
  res->z = z;
  res->done=0;
  return res;
}


image_area_t *new_image_area (int width, int height,
      double xmin, double xmax, double ymin, double ymax)
{
  double d_min (double a, double b)
    {
      return (a<b ? a : b);
    }

  image_area_t *res=(image_area_t*)malloc(sizeof(image_area_t));
  int i;
  res->width  = width;
  res->height = height;
  res->xmin = xmin;
  res->xmax = xmax;
  res->ymin = ymin;
  res->ymax = ymax;
  res->min_pixelsize = d_min ((xmax-xmin)/width, (ymax-ymin)/height);

  res->a=(char**)calloc(width, sizeof(char*));
  for (i=0; i<width; i++)
    res->a[i]=(char*)calloc(height, sizeof(char));

  res->data=(GList***)calloc(width, sizeof(GList**));
  for (i=0; i<width; i++)
    res->data[i]=(GList**)calloc(height, sizeof(GList*));

  res->N=(int**)calloc(width, sizeof(int*));
  for (i=0; i<width; i++) {
    res->N[i]=(int*)calloc(height, sizeof(int));
  }

  res->type=(int**)calloc(width, sizeof(int*));
  for (i=0; i<width; i++)
    res->type[i]=(int*)calloc(height, sizeof(int));

  res->all_done=(int**)calloc(width, sizeof(int*));
  for (i=0; i<width; i++)
    res->all_done[i]=(int*)calloc(height, sizeof(int));

  return res;
}


void free_image_area (image_area_t *ia)
{
  int i,j;
  GList *p;

  for (i=0; i<ia->width; i++) free(ia->a[i]);
  for (i=0; i<ia->width; i++) {
    for (j=0; j<ia->height; j++) {
      for (p=ia->data[i][j]; p; p=p->next) free(p->data);
      g_list_free(ia->data[i][j]);
    }
    free (ia->data[i]);
  }
  free (ia->a);
  free (ia);
  return;
}


/*
image_area_t *copy_image_area (image_area_t *ia)
{
  image_area_t *res=new_image_area(ia->width, ia->height, 
				   ia->xmin, ia->xmax, ia->ymin, ia->ymax);
  int i;

  for (i=0; i<res->width; i++)
    memcpy (res->a[i], ia->a[i], sizeof(char)*(ia->height));
    // data-Listen kopieren
  return res;
}
*/


char *imagearea_default_color="\xFF\xFF\x00";
int imagearea_color_number=7;
char *imagearea_color_table[]={
  "\0\0\0",        /* black */
  "\xFF\xFF\xFF",  /* white */
  "\xFF\0\0",      /* red */
  "\0\xFF\0",      /* green */
  "\x66\x66\xFF",  /* blue */
  "\xFF\xFF\x00",  /* yellow */
  "\x99\x99\x99",
  NULL
};


unsigned char *rgbbuf (image_area_t *ia)
{
  unsigned char *res, *k;
  int i,j;

  res = (unsigned char*)malloc(sizeof(unsigned char)*(ia->width*ia->height*3));
  k=res;
  for (j=0; j<ia->height; j++)
    for (i=0; i<ia->width; i++)
      {
        if ((int)(unsigned char)(ia->a[i][j])<imagearea_color_number)
          memcpy (k, imagearea_color_table[(int)(unsigned char)(ia->a[i][j])],
                  3);
        else
          memcpy (k, imagearea_default_color, 3);
        k+=3;
      }
  return res;
}


/*************************************************************************/
/* calculate coordinates from complex number and vice versa */

int x_coord (image_area_t *ia, complex_t z)
{
  return (z.re-ia->xmin)/(ia->xmax-ia->xmin)*ia->width;
}
int y_coord (image_area_t *ia, complex_t z)
{
  return ia->height-1-(z.im-ia->ymin)/(ia->ymax-ia->ymin)*ia->height;
}

complex_t z_coord (image_area_t *ia, int x, int y)
{
  static complex_t res;
  y=ia->height-1-y;
  res.re = (x*(ia->xmax-ia->xmin)/ia->width )+ia->xmin;
  res.im = (y*(ia->ymax-ia->ymin)/ia->height)+ia->ymin;
  return res;
}



/**************************************************************************/
/* set and get color of points. */

/* are coordingates in range? */
int coord_valid (image_area_t *ia, int x, int y)
{
  return ((x>=0) && (x<ia->width) && (y>=0) && (y<ia->height));
}


/* safe routines */
/* return -1 if out of range, 0 otherwise */
int set_point (image_area_t *ia, int x, int y, int value)
{
  if (coord_valid(ia, x, y)) {
    ia->a[x][y]=value;
    return 0;
  }
  return -1;
}

/* return 0 (not set), if x,y is out of range */
char get_point (image_area_t *ia, int x, int y)
{
  if (coord_valid(ia, x, y)) {
    return (ia->a[x][y]);
  }
  return 0;
}

int set_c_point (image_area_t *ia, complex_t z, int value)
{
  int x=x_coord(ia,z) ,y=y_coord(ia,z);
  return set_point (ia, x, y, value);
}

/* return-value:
   &1 : if inverse was not found
   &2 : if z was prepended to list
   &4 : if z was out of range
*/
#if 0
int set_c_point1 (image_area_t *ia, complex_t z, complex_t *new_inverse,
                  complex_t (*f)(complex_t), complex_t(*f1)(complex_t),
                  ia_data_t **z_list_pos)
{
  int x=x_coord(ia,z) ,y=y_coord(ia,z), inv_found, prepend=0, outofrange,
      mult;
  complex_t df={0,0};
  GList *l;
  double eps;

  if (get_point(ia,x,y)==0) outofrange=set_point(ia,x,y,1);
  if (0==h_zero(f, f1, z, z, new_inverse, &mult)) {
    df = c_inv(f1(*new_inverse));
    inv_found=1;
  }
  else {
    df.re=1;
    inv_found=0;
  }

  if (outofrange) outofrange=1;
  else {
    prepend=1;
    eps = 0.5*ia->min_pixelsize/c_abs(df);
    for (l=ia->data[x][y]; l; l=l->next) {
      if (c_diff(z, ((ia_data_t*)(l->data))->z)<eps) {
        prepend=0;
        break;
      }
    }
    if (prepend) {
      ia->data[x][y] = g_list_prepend (ia->data[x][y], new_ia_data (z));
      *z_list_pos = (ia_data_t*)(ia->data[x][y]->data);
    }
  }

  return (inv_found | (prepend*2) | (outofrange*4));
}

#endif



