#ifndef __IMAGEAREA_H_
#define __IMAGEAREA_H_

#include <glib.h>

#include "complex.h"

typedef struct {
  double xmin, xmax, ymin, ymax, min_pixelsize;
  int width, height; /* number of pixels*/
  char **a; /* the matrix */
  GList ***data; /* for every pixel a GList of some data ia_data_t */
  int **N;  /* for every pixel a precision */
  int **type; /* for different colors during drawing */
  int **all_done;
} image_area_t;


typedef struct {
  complex_t z;
  int done;
} ia_data_t;


/* Speicherverwaltung */
ia_data_t *new_ia_data (complex_t z);
image_area_t *new_image_area (int width, int height,
                  double xmin, double xmax, double ymin, double ymax);
void free_image_area (image_area_t *ia);
image_area_t *copy_image_area (image_area_t *ia);
unsigned char *rgbbuf (image_area_t *ia);


/* Koordinaten umrechnen */
int x_coord (image_area_t *ia, complex_t z);
int y_coord (image_area_t *ia, complex_t z);
complex_t z_coord (image_area_t *ia, int x, int y);


/* Punkte setzen und abfragen */
int coord_valid (image_area_t *ia, int x, int y);
int set_point (image_area_t *ia, int x, int y, int value);
char get_point (image_area_t *ia, int x, int y);
int set_c_point (image_area_t *ia, complex_t z, int value);
int set_c_point1 (image_area_t *ia, complex_t z, complex_t *new_inverse,
                  complex_t (*f)(complex_t), complex_t(*f1)(complex_t),
                  ia_data_t **z_list_pos);

#endif
