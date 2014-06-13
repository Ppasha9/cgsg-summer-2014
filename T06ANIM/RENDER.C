/* FILENAME: RENDER.C
 * PROGRAMMER: RK2
 * PURPOSE: 3D render handle module.
 * LAST UPDATE: 12.06.2014
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "render.h"
#include "mth.h"

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
rk2CAMERA RK2_RndCamera;

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
  RK2_RndCamera.Loc = Loc;
  RK2_RndCamera.At = At;
  RK2_RndCamera.Up = Up;
} /* End of 'RK2_RndCameraSet' funciton */

/* Cameras getting matrix function.
* ARGUMENTS: None.
* RETURNS: 
*   (rk2MATR4x4) Camera matrix.
*/
static rk2MATR4x4 RK2_RndCameraGetMatrix( VOID )
{
  rk2MATR4x4 RetMatr = 
  {
    {
      { RK2_RndCamera.Dir.X  , RK2_RndCamera.Dir.Y  , RK2_RndCamera.Dir.Z  , 0 },
      { RK2_RndCamera.Up.X   , RK2_RndCamera.Up.Y   , RK2_RndCamera.Up.Z   , 0 },
      { RK2_RndCamera.Right.X, RK2_RndCamera.Right.Y, RK2_RndCamera.Right.Z, 0 },
      {                  0,                  0,                 0 , 1 }
    }
  };
  return RetMatr;
} /* End of 'RK2_RndCameraGetMatrix' funciton */

/* Cameras normalizing vectors function.
* ARGUMENTS: None.
* RETURNS: None.
*/
static VOID RK2_RndCameraNormalize(VOID)
{
  RK2_RndCamera.At = VecNormalize(RK2_RndCamera.At);
  RK2_RndCamera.Dir = VecNormalize(RK2_RndCamera.Dir);
  RK2_RndCamera.Up = VecNormalize(RK2_RndCamera.Up);
  RK2_RndCamera.Right = VecNormalize(RK2_RndCamera.Right);
} /* End of 'RK2_RndCameraGetMatrix' funciton */

/* Camera move by Dir-axes properties 
 * ARGUMENTS:
 *   - Distance to move:
 *       DBL Coef;  
 * RETURNS: None.
 */
VOID RK2_RndCameraMoveDir( DBL Coef )
{
  RK2_RndCamera.Dir = VecNormalize(VecSubVec(RK2_RndCamera.At, RK2_RndCamera.Loc));
  RK2_RndCamera.Loc = VecSumVec(RK2_RndCamera.Loc, VecMultNum(RK2_RndCamera.Dir, Coef));
  RK2_RndCamera.At = VecSumVec(RK2_RndCamera.At, VecMultNum(RK2_RndCamera.Dir, Coef));
}  /* End of 'RK2_RndCameraMoveByDir' funciton */

/* Camera move by Right-Axes function.
 * ARGUMENTS:
 *   - Distance to move:
 *       DBL Coef;  
 * RETURNS: None.
 */
VOID RK2_RndCameraMoveRight( DBL Coef )
{
  RK2_RndCamera.Dir = VecSubVec(RK2_RndCamera.At, RK2_RndCamera.Loc);
  RK2_RndCamera.Right = VecCrossVec(RK2_RndCamera.Dir, RK2_RndCamera.Up);
  RK2_RndCamera.Loc = VecSumVec(RK2_RndCamera.Loc, VecMultNum(VecNormalize(RK2_RndCamera.Right), Coef));
  RK2_RndCamera.At = VecSumVec(RK2_RndCamera.At, VecMultNum(VecNormalize(RK2_RndCamera.Right), Coef));
}  /* End of 'RK2_RndCameraMoveByDir' funciton */

/* Camera move by Up-Axes function.
 * ARGUMENTS:
 *   - Distance to move:
 *       DBL Coef;  
 * RETURNS: None.
 */
VOID RK2_RndCameraMoveUp( DBL Coef )
{
  RK2_RndCamera.Loc = VecSumVec(RK2_RndCamera.Loc, VecMultNum(VecNormalize(RK2_RndCamera.Up), Coef));
  RK2_RndCamera.At = VecSumVec(RK2_RndCamera.At, VecMultNum(VecNormalize(RK2_RndCamera.Up), Coef));
}  /* End of 'RK2_RndCameraMoveByDir' funciton */

/* Camera rotation by camera-axes-Up function.
 * ARGUMENTS:
 *   - Angle:
 *       DBL Angle;
 * RETURNS: None.
 */
VOID RK2_RndCameraRotateUp( DBL Angle )
{
  rk2MATR4x4 RotMat;

  RK2_RndCamera.Dir = VecSubVec(RK2_RndCamera.At, RK2_RndCamera.Loc);
  RK2_RndCamera.Right = VecCrossVec(VecNormalize(RK2_RndCamera.Dir), RK2_RndCamera.Up);

  RK2_RndCameraNormalize();

  RotMat = RK2_RndCameraGetMatrix();
  RotMat = MatrMultMatr(MatrRotateY(MatrDefault(), Angle), RotMat);

  RK2_RndCameraSet(RK2_RndCamera.Loc,                                                              /* Location */
                   VecSumVec(RK2_RndCamera.Loc, 
                             VecSet(RotMat.A[0][0], RotMat.A[0][1], RotMat.A[0][2])),              /* At */
                   RK2_RndCamera.Up);                                                              /* Up */
}  /* End of 'RK2_RndCameraRotateUp' funciton */

/* Camera rotation by camera-axes-right function.
* ARGUMENTS:
*   - Angle:
*       DBL Angle;
* RETURNS: None.
*/
VOID RK2_RndCameraRotateRight(DBL Angle)
{
  rk2MATR4x4 RotMat;
  /*
  if (Angle == 0)
    return;
  */
  RK2_RndCamera.Dir = VecSubVec(RK2_RndCamera.At, RK2_RndCamera.Loc);
  RK2_RndCamera.Right = VecCrossVec(VecNormalize(RK2_RndCamera.Dir), RK2_RndCamera.Up);

  RK2_RndCameraNormalize();

  RotMat = RK2_RndCameraGetMatrix();
  RotMat = MatrMultMatr(MatrRotateZ(MatrDefault(), Angle), RotMat);

  RK2_RndCameraSet(RK2_RndCamera.Loc,                                               /* Location */
    VecSumVec(RK2_RndCamera.Loc,
    VecSet(RotMat.A[0][0], RotMat.A[0][1], RotMat.A[0][2])),                        /* At */
    RK2_RndCamera.Up);                                                              /* Up */
}  /* End of 'RK2_RndCameraRotateRight' funciton */

/* Camera rotation by camera-axes-dir function.
* ARGUMENTS:
*   - Angle:
*       DBL Angle;
* RETURNS: None.
*/
VOID RK2_RndCameraRotateDir(DBL Angle)
{
  rk2MATR4x4 RotMat;

  RK2_RndCamera.Dir = VecSubVec(RK2_RndCamera.At, RK2_RndCamera.Loc);
  RK2_RndCamera.Right = VecCrossVec(VecNormalize(RK2_RndCamera.Dir), RK2_RndCamera.Up);
  RK2_RndCameraNormalize();

  RotMat = RK2_RndCameraGetMatrix();
  RotMat = MatrMultMatr(MatrRotateX(MatrDefault(), Angle), RotMat);

  RK2_RndCameraSet(RK2_RndCamera.Loc,                                               /* Location */
    VecSumVec(RK2_RndCamera.Loc,
    VecSet(RotMat.A[0][0], RotMat.A[0][1], RotMat.A[0][2])),                        /* At */
    RK2_RndCamera.Up);                                                              /* Up */
}  /* End of 'RK2_RndCameraRotateDir' funciton */

/* Render setting function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID RK2_RndBuildMatrix( VOID )
{
  RK2_RndMatrView = MatrViewLookAt(RK2_RndCamera.Loc, RK2_RndCamera.At, RK2_RndCamera.Up);
  RK2_RndMatrProjection = MatrProjection(-RK2_RndWp / 2, RK2_RndWp / 2, -RK2_RndHp / 2, RK2_RndHp / 2, RK2_RndProjDist, 1000);
  RK2_RndMatrRes = MatrMultMatr(MatrMultMatr(RK2_RndMatrWorld, RK2_RndMatrView), RK2_RndMatrProjection);
  /* RK2_RndMatrRes = MatrMultMatr(RK2_RndMatrWorld, RK2_RndMatrView); */
} /* End of 'RK2_RndBuildMatrixView' function */

/* END OF 'RENDER.C' FILE */
