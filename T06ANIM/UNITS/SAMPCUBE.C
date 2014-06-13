/* FILENAME: SAMPCUBE.C
 * PROGRAMMER: RK2
 * PURPOSE: Animation unit handle module.
 * LAST UPDATE: 09.06.2014
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "../anim.h"
#include "../render.h"

/* Unit cube struct definition */
typedef struct tagrk2UNIT_CUBE
{
  RK2_UNIT_BASE_FIELDS;           /* Base fields */
  INT PosX, PosY, PosZ;           /* Position on window */
  DBL Rotation;                   /* Angle of rotation */
  rk2VEC Points[9];               /* Points of cube */
  DBL FacetLen;                   /* Length of facets */
} rk2UNIT_CUBE;

/* Unit cube init function.
 * ARGUMENTS:
 *   - Self pointer:
 *       rk2UNIT_CUBE *Unit;
 *   - Animation context pointer:
 *       rk2ANIM *Ani;
 * RETURNS: None.
 */
static VOID UnitCubeInit( rk2UNIT_CUBE *Unit, rk2ANIM *Ani )
{
  INT i;

  Unit->Points[0].X = Unit->PosX;                   Unit->Points[0].Y = Unit->PosY;                   Unit->Points[0].Z = Unit->PosZ;
  for (i = 1; i < 9; i++)
    Unit->Points[i].X = rand() % 500 - 1000,  Unit->Points[i].Y = rand() % 500 - 1000, Unit->Points[i].Z = rand() % 500 - 1000;
} /* End of 'RK2_UnitCubeInit' function */


/* Unit cube getting coords function.
* ARGUMENTS:
 *   - Self pointer:
 *       rk2UNIT *Unit;
 *   - Animation context pointer:
 *       rk2ANIM *Ani;
 *   - Last point position:
 *       rk2VEC SrcPoint;
 *   - Time:
 *       DBL Time;
 * RETURNS: 
 *   (rk2VEC) Coords of point.
 */
static rk2VEC UnitCubeGetCoordByTime(rk2UNIT_CUBE *Unit, rk2ANIM *Ani, rk2VEC SrcPoint, DBL Time )
{
  rk2VEC VecSaved = SrcPoint;
  static INT MinDist = 30, MaxDist = 300;

  SrcPoint.X += cos(Time * 3) * ((INT)Time % 20);
  if (SrcPoint.X < -MaxDist)
    SrcPoint.X = -MaxDist;
  if (SrcPoint.X > MaxDist)
    SrcPoint.X = MaxDist;

  SrcPoint.Y += sin(Time) * ((INT)Time % 10);
  if (SrcPoint.Y < -MaxDist)
    SrcPoint.Y = -MaxDist;
  if (SrcPoint.Y > MaxDist)
    SrcPoint.Y = MaxDist;
  
  SrcPoint.Z += sin(Time * 2) * ((INT)Time % 30);
  if (SrcPoint.Z< -MaxDist)
    SrcPoint.Z = -MaxDist;
  if (SrcPoint.Z > MaxDist)
    SrcPoint.Z = MaxDist;

  if (SrcPoint.X > -MinDist && SrcPoint.X < 0 &&
      SrcPoint.Y > -MinDist && SrcPoint.Y < 0 &&
      SrcPoint.Z > -MinDist && SrcPoint.Z < 0)
    return VecSaved;

  return SrcPoint;
} /* End of 'UnitCubeGetCoordByTime' function */

/* Unit cube response function.
 * ARGUMENTS:
 *   - Self pointer:
 *       rk2UNIT *Unit;
 *   - Animation context pointer:
 *       rk2ANIM *Ani;
 * RETURNS: None.
 */
static VOID UnitCubeResponse( rk2UNIT_CUBE *Unit, rk2ANIM *Ani )
{
  Unit->Points[0] = UnitCubeGetCoordByTime(Unit, Ani, Unit->Points[0], Ani->Time);
} /* End of 'RK2_UnitCubeResponse' function */

/* Unit cube render function.
 * ARGUMENTS:
 *   - Self pointer:
 *       rk2UNIT *Unit;
 *   - Animation context pointer:
 *       rk2ANIM *Ani;
 * RETURNS: None.
 */
static VOID UnitCubeRender( rk2UNIT_CUBE *Unit, rk2ANIM *Ani )
{
  rk2VEC NPnt;
  INT ls = 2;
  DBL j;
  CHAR Str[MAX_PATH];

  /* RK2_RndCameraSet(Unit->Points[0], VecSet(0, 0, 0), VecSet(0, 1, 0)); */
  /* UnitCubeGetCoordByTime(Unit, Ani, Unit->Points[0], Ani->Time + 0.2); */

  /* Axes */
  SelectObject(Ani->hDC, GetStockObject(DC_BRUSH));
  SelectObject(Ani->hDC, GetStockObject(NULL_PEN));

  RK2_RndMatrWorld = MatrDefault();
  RK2_RndBuildMatrix();

  NPnt = RK2_RndWorldToScreen(Unit->Points[0]);
  Ellipse(Ani->hDC, NPnt.X - 5 / NPnt.Z, NPnt.Y - 5 / NPnt.Z, NPnt.X + 5 / NPnt.Z, NPnt.Y + 5 / NPnt.Z);


  for (j = -50; j <= 50; j += 0.5)
  {
    SetDCBrushColor(Ani->hDC, 0xFF0000);
    NPnt = RK2_RndWorldToScreen(VecSet(j, 0, 0));
    Ellipse(Ani->hDC, NPnt.X - ls, NPnt.Y - ls, NPnt.X + ls, NPnt.Y + ls);
    if (j - (INT)j == 0 && !((INT)j % 5))
      TextOut(Ani->hDC, NPnt.X - 1, NPnt.Y - 1, Str, sprintf(Str, "%d", (INT)j));

    SetDCBrushColor(Ani->hDC, 0x00FF00);
    NPnt = RK2_RndWorldToScreen(VecSet(0, j, 0));
    Ellipse(Ani->hDC, NPnt.X - ls, NPnt.Y - ls, NPnt.X + ls, NPnt.Y + ls);
    if (j - (INT)j == 0 && !((INT)j % 5))
      TextOut(Ani->hDC, NPnt.X - 1, NPnt.Y - 1, Str, sprintf(Str, "%d", (INT)j));

    SetDCBrushColor(Ani->hDC, 0x0000FF);
    NPnt = RK2_RndWorldToScreen(VecSet(0, 0, j));
    Ellipse(Ani->hDC, NPnt.X - ls, NPnt.Y - ls, NPnt.X + ls, NPnt.Y + ls);
    if (j - (INT)j == 0 && !((INT)j % 5))
      TextOut(Ani->hDC, NPnt.X - 1, NPnt.Y - 1, Str, sprintf(Str, "%d", (INT)j));

  }

} /* End of 'RK2_UnitCubeRender' function */

/* Unit cube create function.
 * ARGUMENTS: None.
 * RETURNS:
 *   (rk2UNIT_CUBE *) - pointer for new animation unit.
 */
rk2UNIT *RK2_UnitCubeCreate( VOID )
{
  rk2UNIT_CUBE *Unit;

  if ((Unit = (rk2UNIT_CUBE *)RK2_AnimUnitCreate(sizeof(rk2UNIT_CUBE))) == NULL)
    return NULL;

  /* Base fields filling */
  Unit->Render = (VOID *)UnitCubeRender;
  Unit->Init = (VOID *)UnitCubeInit;
  Unit->Response = (VOID *)UnitCubeResponse;

  Unit->PosX = Unit->PosY = Unit->PosZ = 70;
  return (rk2UNIT *)Unit;
} /* End of 'RK2_UnitCubeCreate' function */

/* END OF 'SAMPCUBE.C' FILE */
