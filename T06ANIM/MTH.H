/* FILENAME: MTH.H
 * PROGRAMMER: RK2
 * PURPOSE: Matrix module
 * LAST UPDATE: 10.06.2014
 */

#include <math.h>
#include "def.h"

#ifndef __MTH_H_
#define __MTH_H_

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

/* Matrix adding number function. */
VOID RK2_MatrAddNum( DBL *Dest, DBL *Src, INT SrcW, INT SrcH, DBL Num );

/* Vector create function. */
rk2VEC RK2_Vec( DBL X, DBL Y, DBL Z );

/* Sum vectors counting function. */
rk2VEC RK2_VecSumVec( rk2VEC SrcVec0, rk2VEC SrcVec1 );


#endif /* __MTH_H_ */

/* END OF 'MTH.H' FILE */
