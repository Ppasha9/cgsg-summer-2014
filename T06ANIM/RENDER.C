/* FILENAME: RENDER.C
 * PROGRAMMER: RK2
 * PURPOSE: Animation render module
 * LAST UPDATE: 10.06.2014
 */

#include "def.h"

#include "mth.h"

/* Camera render struct */
typedef struct tagrk2CAMERA
{
  rk2VEC Loc, Dir, Up, Right, At; /* Camera properties */
} rk2CAMERA;

/* Gloabal Matrixes */
rk2MATR4x4 RK2_RndMatrWorld = 
{
  {
    {1, 0, 0, 0},
    {0, 1, 0, 0},
    {0, 0, 1, 0},
    {0, 0, 0, 1}
  }
}, 
RK2_RndMatrView = 
{
  {
    {1, 0, 0, 0},
    {0, 1, 0, 0},
    {0, 0, 1, 0},
    {0, 0, 0, 1}
  }
};

DBL 
  RK2_ImageW = 30, RK2_ImageH = 30,   /* Screen sizes */
  RK2_ImageWp = 3, RK2_ImageHp = 4; /* Screen sizes */

/* User camera */
rk2CAMERA RK2_Camera;

/* World to screen coords setting function.
 * ARGUMENTS:
 *   - vector:
 *       rk2CAMERA *Camera;
 * RETURNS:
 *   (rk2VEC) Vector back.
 */
rk2VEC RK2_RndWorldToScreen( rk2VEC VecSrc )
{
  rk2VEC Ps, VecN;

  rk2MATR4x4 TmpMatr;

  /* getting coords */
  RK2_MatrMultMatr(&TmpMatr.Arr[0][0], &RK2_RndMatrWorld.Arr[0][0], 4, 4, &RK2_RndMatrView.Arr[0][0], 4, 4);
  VecN = RK2_VecMultMatr4x4(VecSrc, &TmpMatr);

  /* Proection */
  Ps.X = (VecN.X + RK2_ImageW / 2) / RK2_ImageWp * (RK2_ImageW - 1);
  Ps.Y = (-VecN.Y + RK2_ImageH / 2) / RK2_ImageHp * (RK2_ImageH - 1);
  Ps.Z = VecN.Z;
  return Ps;
} /* End of 'RK2_RndWorldToScreen' function */

/* Setting RK2_RndMatrView matrix function.
 * ARGUMENTS: None.
 * RETURNS:
 *   (VOID) None.
 */
VOID RK2_RndBuildMatrView( VOID )
{
  /*
  RK2_RndMatrView.Arr[0][0] = Right.X;
  RK2_RndMatrView.Arr[0][1] = Right.Y;
  RK2_RndMatrView.Arr[0][2] = Right.Z;
  RK2_RndMatrView.Arr[0][3] = -Loc.X * Right.X - Loc.Y * Right.Y - Loc.Z * Right.Z;

  RK2_RndMatrView.Arr[1][0] = Up.X;
  RK2_RndMatrView.Arr[1][1] = Up.Y;
  RK2_RndMatrView.Arr[1][2] = Up.Z;
  RK2_RndMatrView.Arr[1][2] = -Loc.X * Up.X - Loc.Y * Up.Y - Loc.Z * Up.Z;

  RK2_RndMatrView.Arr[2][0] = -Dir.X;
  RK2_RndMatrView.Arr[2][1] = -Dir.Y;
  RK2_RndMatrView.Arr[2][2] = -Dir.Z;
  RK2_RndMatrView.Arr[2][2] = Loc.X * Dir.X + Loc.Y * Dir.Y + Loc.Z * Dir.Z;

  RK2_RndMatrView.Arr[3][0] = 0;
  RK2_RndMatrView.Arr[3][1] = 0;
  RK2_RndMatrView.Arr[3][2] = 0;
  RK2_RndMatrView.Arr[3][2] = 1;
  */

  RK2_RndMatrView.Arr[0][0] = RK2_Camera.Right.X;
  RK2_RndMatrView.Arr[0][1] = RK2_Camera.Right.Y;
  RK2_RndMatrView.Arr[0][2] = RK2_Camera.Right.Z;
  RK2_RndMatrView.Arr[0][3] = -RK2_Camera.Loc.X * RK2_Camera.Right.X - RK2_Camera.Loc.Y * RK2_Camera.Right.Y - RK2_Camera.Loc.Z * RK2_Camera.Right.Z;

  RK2_RndMatrView.Arr[1][0] = RK2_Camera.Up.X;
  RK2_RndMatrView.Arr[1][1] = RK2_Camera.Up.Y;
  RK2_RndMatrView.Arr[1][2] = RK2_Camera.Up.Z;
  RK2_RndMatrView.Arr[1][2] = -RK2_Camera.Loc.X * RK2_Camera.Up.X - RK2_Camera.Loc.Y * RK2_Camera.Up.Y - RK2_Camera.Loc.Z * RK2_Camera.Up.Z;

  RK2_RndMatrView.Arr[2][0] = -RK2_Camera.Dir.X;
  RK2_RndMatrView.Arr[2][1] = -RK2_Camera.Dir.Y;
  RK2_RndMatrView.Arr[2][2] = -RK2_Camera.Dir.Z;
  RK2_RndMatrView.Arr[2][2] = RK2_Camera.Loc.X * RK2_Camera.Dir.X + RK2_Camera.Loc.Y * RK2_Camera.Dir.Y + RK2_Camera.Loc.Z * RK2_Camera.Dir.Z;

  RK2_RndMatrView.Arr[3][0] = 0;
  RK2_RndMatrView.Arr[3][1] = 0;
  RK2_RndMatrView.Arr[3][2] = 0;
  RK2_RndMatrView.Arr[3][2] = 1;

} /* End of 'RK2_RndBuildMatrView' function */

/* Setting RK2_Camera properties function.
 * ARGUMENTS:
 *   - Camera properties:
 *       rk2VEC Loc, rk2VEC Up, rk2VEC Right, rk2VEC Dir, rk2VEC At;
 * RETURNS:
 *   (VOID) None.
 */
VOID RK2_RndSetCamera( rk2VEC Loc, rk2VEC Up, rk2VEC Right, rk2VEC Dir, rk2VEC At )
{
  RK2_Camera.At = At;
  RK2_Camera.Up = Up;
  RK2_Camera.Right = Right;
  RK2_Camera.Dir = Dir;
} /* End of 'RK2_RndSetCamera' function */

/* Adding RK2_Camera properties function.
 * ARGUMENTS:
 *   - Camera properties:
 *       rk2VEC Loc, rk2VEC Up, rk2VEC Right, rk2VEC Dir, rk2VEC At;
 * RETURNS:
 *   (VOID) None.
 */
VOID RK2_RndUpdateCamera( rk2VEC DtLoc, rk2VEC DtUp, rk2VEC DtRight, rk2VEC DtDir, rk2VEC DtAt )
{
  RK2_Camera.At = RK2_VecSumVec(RK2_Camera.At, DtAt);
  RK2_Camera.Dir = RK2_VecSumVec(RK2_Camera.Dir, DtDir);
  RK2_Camera.Loc = RK2_VecSumVec(RK2_Camera.Loc, DtLoc);
  RK2_Camera.Right = RK2_VecSumVec(RK2_Camera.Right, DtRight);
  RK2_Camera.Up = RK2_VecSumVec(RK2_Camera.Up, DtUp);
} /* End of 'RK2_RndUpdateCamera' function */

/* Setting Global properties for render function.
 * ARGUMENTS:
 *   - Anim properties:
 *       INT ScreenW, INT ScreenH;
 * RETURNS:
 *   (VOID) None.
 */
VOID RK2_RndSetRenderProp( INT ScreenW, INT ScreenH )
{
  
  RK2_ImageW = ScreenW;
  RK2_ImageH = ScreenH;
  RK2_ImageWp = RK2_ImageHp * ScreenW / ScreenH;
  /* RK2_ImageHp = ScreenH; */
  
} /* End of 'RK2_RndSetRenderProp' function */

/* END OF 'RENDER.C' FILE */
