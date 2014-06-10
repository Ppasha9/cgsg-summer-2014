/* FILENAME: RENDER.H
 * PROGRAMMER: RK2
 * PURPOSE: Animation render module
 * LAST UPDATE: 10.06.2014
 */

#include "def.h"

#include "mth.h"

/* World to screen coords setting function. */
POINT RK2_RndWorldToScreen( rk2VEC VecSrc, rk2ANIM *Ani );

/* Setting RK2_RndMatrView matrix function. */
VOID RK2_RndBuildMatrView( rk2VEC Loc, rk2VEC Up, rk2VEC Right, rk2VEC Dir, rk2VEC At );

/* END OF 'RENDER.H' FILE */
