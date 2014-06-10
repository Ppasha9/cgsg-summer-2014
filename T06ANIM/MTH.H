/* FILENAME: MTH.H
 * PROGRAMMER: RK2
 * PURPOSE: Matrix module
 * LAST UPDATE: 10.06.2014
 */

#include <math.h>
#include "def.h"


/* Matrix 4x4 type */
typedef struct tagrk2MATR4x4
{
  DBL Arr[4][4];
} rk2MATR4x4;

/* Vector type */
typedef struct tagrk2VEC
{
  DBL X, Y, Z, At;
} rk2VEC;

/* Matr mult with matr function. */
VOID RK2_MatrMultMatr( DBL *Dest, DBL *SrcA, INT SrcAw, INT SrcAh, DBL *SrcB, INT SrcBw, INT SrcBh );

/* Vec mult with matr function. */
rk2VEC RK2_VecMultMatr4x4( rk2VEC Vec, rk2MATR4x4 *SrcMatr );

/* END OF 'MTH.H' FILE */
