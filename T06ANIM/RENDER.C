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
RK2_RndMatrView;

/* World to screen coords setting function.
 * ARGUMENTS:
 *   - Camera:
 *       rk2CAMERA *Camera;
 * RETURNS:
 *   (VOID) None.
 */
POINT RK2_RndWorldToScreen( rk2VEC VecSrc, rk2ANIM *Ani )
{
  POINT Ps;
  rk2VEC VecN;

  /* getting coords */
  VecN = RK2_VecMultMatr4x4(RK2_VecMultMatr4x4(VecSrc, &RK2_RndMatrWorld), &RK2_RndMatrView);

  /* Proection */
  Ps.x = (VecN.X + Ani->W / 2) / Ani->W * (Ani->W - 1);
  Ps.y = (-VecN.Y + Ani->H / 2) / Ani->H * (Ani->H - 1);
  return Ps;
} /* End of 'RK2_RndWorldToScreen' function */

/* Setting RK2_RndMatrView matrix function.
 * ARGUMENTS:
 *   - Camera properties:
 *       rk2VEC Loc, rk2VEC Up, rk2VEC Right, rk2VEC Dir, rk2VEC At;
 * RETURNS:
 *   (VOID) None.
 */
VOID RK2_RndBuildMatrView( rk2VEC Loc, rk2VEC Up, rk2VEC Right, rk2VEC Dir, rk2VEC At )
{
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
} /* End of 'RK2_RndBuildMatrView' function */

/* END OF 'RENDER.C' FILE */
