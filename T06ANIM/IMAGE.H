/* FILENAME: IMAGE.H
 * PROGRAMMER: RK2
 * PURPOSE: Image handle declaration module.
 * LAST UPDATE: 06.06.2014
 */

#ifndef __IMAGE_H_
#define __IMAGE_H_

#include "def.h"

/* Image struct definition */
typedef struct tagIMAGE
{
  INT W, H;    /* Picture size */
  HBITMAP hBm; /* Descriptor of loaded picture */
  DWORD *Bits; /* Image row */
} IMAGE;

/* Loading picture from file function. */
BOOL ImageLoad( IMAGE *Img, CHAR *FileName );

/* Picture memory free function. */
VOID ImageFree( IMAGE *Img );

/* Getting image point color function. */
DWORD ImageGetP( IMAGE *Img, INT X, INT Y );

#endif /* __IMAGE_H_ */

/* END OF 'IMAGE.H' FILE */
