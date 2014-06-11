/* FILENAME: RENDER.C
 * PROGRAMMER: RK2
 * PURPOSE: 3D render handle module.
 * LAST UPDATE: 10.06.2014
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "render.h"
#include "mth.h"

/* Глобальная камера */
rk2CAMERA RK2_RndCam;

/* Parametres of projection */
DBL
  RK2_RndWs = 30, RK2_RndHs = 30,   /* Frame sizes */
  RK2_RndWp = 4, RK2_RndHp = 3,     /* Projection sizes */
  RK2_RndProjDist = 5;              /* distance to frame */

/* Matrix */
rk2MATR4x4
  RK2_RndMatrWorld = RK2_UNIT_MATR_DEFAULT,       /* Matrix world convertion */
  RK2_RndMatrView = RK2_UNIT_MATR_DEFAULT,        /* Matrix view convertion */
  RK2_RndMatrProjection = RK2_UNIT_MATR_DEFAULT,  /* Projection matrix */
  RK2_RndMatrRes = RK2_UNIT_MATR_DEFAULT;         /* Summed matrix */

/* Camera */
rk2CAMERA RK2_Camera;

/* Convertion from worl coodinate system to camera screen function.
 * ARGUMENTS:
 *   - Exisating point:
 *       rk2VEC P;
 * RETURNS:
 *   (rk2VEC) Coords in window.
 */
rk2VEC RK2_RndWorldToScreen( rk2VEC P )
{
  rk2VEC
    Ps, Pp;

  /* Covert system coords */
  Pp = VecMultMatr(P, RK2_RndMatrRes);
  
  /* Projection */
  Pp.X *= RK2_RndProjDist / Pp.Z;
  Pp.Y *= RK2_RndProjDist / Pp.Z;

  Ps.X = (-Pp.X + RK2_RndWp / 2) / RK2_RndWp * (RK2_RndWs - 1);
  Ps.Y = ( Pp.Y + RK2_RndHp / 2) / RK2_RndHp * (RK2_RndHs - 1);
  Ps.Z = Pp.Z;
  return Ps;
} /* End of 'RK2_RndWorldToScreen' function */

/* Render setting function.
 * ARGUMENTS:
 *   - Image properties:
 *       INT W, H;
 * RETURNS: None.
 */
VOID RK2_RndSetRenderProp( INT W, INT H )
{
   RK2_RndWs = W;
   RK2_RndHs = H;
   RK2_RndWs = W;
   RK2_RndHs = H;
} /* End of 'RK2_RndSetRenderProp' function */

/* Setting cameras properties 
 * ARGUMENTS:
 *   - Vector camera new properties:
 *     rk2VEC Loc, At, Up;
 * RETURNS: None.
 */
VOID RK2_RndCameraSet( rk2VEC Loc, rk2VEC At, rk2VEC Up )
{
  RK2_Camera.Loc = Loc;
  RK2_Camera.At = At;
  RK2_Camera.Up = Up;
} /* End of 'RK2_RndCameraSet' funciton */

/* Camera move by Dir-axes properties 
 * ARGUMENTS:
 *   - Vector camera new properties:
 *     rk2VEC Loc, At, Up;
 * RETURNS: None.
 */
VOID RK2_RndCameraMoveDir( DBL Coef )
{
  RK2_Camera.Dir = VecSubVec(RK2_Camera.At, RK2_Camera.Loc);
  RK2_Camera.Loc = VecSumVec(RK2_Camera.Loc, VecMultNum(VecNormalize(RK2_Camera.Dir), Coef));
}  /* End of 'RK2_RndCameraMoveByDir' funciton */

/* Camera move by Right-Axes function.
 * ARGUMENTS:
 *   - Vector camera new properties:
 *     rk2VEC Loc, At, Up;
 * RETURNS: None.
 */
VOID RK2_RndCameraMoveRight( DBL Coef )
{
  RK2_Camera.Dir = VecSubVec(RK2_Camera.At, RK2_Camera.Loc);
  RK2_Camera.Right = VecCrossVec(RK2_Camera.Dir, RK2_Camera.Up);
  RK2_Camera.Loc = VecSumVec(RK2_Camera.Loc, VecMultNum(VecNormalize(RK2_Camera.Right), Coef));
  RK2_Camera.At = VecSumVec(RK2_Camera.At, VecMultNum(VecNormalize(RK2_Camera.Right), Coef));
}  /* End of 'RK2_RndCameraMoveByDir' funciton */

/* Camera move by Up-Axes function.
 * ARGUMENTS:
 *   - Vector camera new properties:
 *     rk2VEC Loc, At, Up;
 * RETURNS: None.
 */
VOID RK2_RndCameraMoveUp( DBL Coef )
{
  RK2_Camera.Loc = VecSumVec(RK2_Camera.Loc, VecMultNum(VecNormalize(RK2_Camera.Up), Coef));
  RK2_Camera.At = VecSumVec(RK2_Camera.At, VecMultNum(VecNormalize(RK2_Camera.Up), Coef));
}  /* End of 'RK2_RndCameraMoveByDir' funciton */

/* Camera rotation by axes Y function.
 * ARGUMENTS:
 *   - Vector camera new properties:
 *     rk2VEC Loc, At, Up;
 * RETURNS: None.
 */
VOID RK2_RndCameraRotateUp( DBL Angle )
{
  RK2_Camera.Dir = VecSubVec(RK2_Camera.At, RK2_Camera.Loc);
  RK2_Camera.Dir.X = RK2_Camera.Dir.X * cos(Angle);
  RK2_Camera.Dir.Y = RK2_Camera.Dir.Y * sin(Angle);

  RK2_Camera.Right = Angle > 0? VecCrossVec(VecNormalize(RK2_Camera.Dir), RK2_Camera.Up);

  RK2_Camera.Dir = VecNeg(VecCrossVec(RK2_Camera.Right, RK2_Camera.Up));
  RK2_Camera.At = VecSubVec(RK2_Camera.Loc, RK2_Camera.Dir);
  
}  /* End of 'RK2_RndCameraMoveByDir' funciton */

/* Render setting function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID RK2_RndBuildMatrix( VOID )
{
  RK2_RndMatrView = MatrViewLookAt(RK2_Camera.Loc, RK2_Camera.At, RK2_Camera.Up);
  RK2_RndMatrProjection = MatrProjection(-RK2_RndWp / 2, RK2_RndWp / 2, -RK2_RndHp / 2, +RK2_RndHp / 2, RK2_RndProjDist, 1000);
  RK2_RndMatrRes = MatrMultMatr(MatrMultMatr(RK2_RndMatrWorld, RK2_RndMatrView), RK2_RndMatrProjection);
  /* RK2_RndMatrRes = MatrMultMatr(RK2_RndMatrWorld, RK2_RndMatrView); */
} /* End of 'RK2_RndBuildMatrixView' function */

/* END OF 'RENDER.C' FILE */
