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

/* SettingGlobal properties for render function. */
VOID RK2_RndSetRenderProp( INT ScreenW, INT ScreenH );

/* Setting RK2_Camera properties function. */
VOID RK2_RndCameraSet(rk2VEC Loc, rk2VEC Up, rk2VEC Right, rk2VEC Dir, rk2VEC At);

/* Adding RK2_Camera properties function. */
VOID RK2_RndCameraUpdate(rk2VEC DtLoc, rk2VEC DtUp, rk2VEC DtRight, rk2VEC DtDir, rk2VEC DtAt);

/// /* RK2_Camera properties mult function function. */
/// VOID RK2_RndCameraUpdateMult(POINT DtLoc, POINT DtUp, POINT DtRight, POINT DtDir, POINT DtAt);

/* END OF 'RENDER.H' FILE */
