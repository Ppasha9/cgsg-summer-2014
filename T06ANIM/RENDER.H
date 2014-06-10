/* FILENAME: RENDER.H
 * PROGRAMMER: RK2
 * PURPOSE: Animation render module
 * LAST UPDATE: 10.06.2014
 */

#include "def.h"

#include "mth.h"

/* World to screen coords setting function. */
rk2VEC RK2_RndWorldToScreen( rk2VEC VecSrc );

/* Setting RK2_RndMatrView matrix function. */
VOID RK2_RndBuildMatrView( VOID );

/* Setting RK2_Camera properties function. */
VOID RK2_RndSetCamera( rk2VEC Loc, rk2VEC Up, rk2VEC Right, rk2VEC Dir, rk2VEC At );

/* SettingGlobal properties for render function. */
VOID RK2_RndSetRenderProp( INT ScreenW, INT ScreenH );

/* END OF 'RENDER.H' FILE */
