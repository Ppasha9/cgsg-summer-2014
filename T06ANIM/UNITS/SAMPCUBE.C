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
  Unit->Points[0].X = Unit->PosX;                   Unit->Points[0].Y = Unit->PosY;                   Unit->Points[0].Z = Unit->PosZ;
  Unit->Points[1].X = Unit->PosX + Unit->FacetLen;  Unit->Points[1].Y = Unit->PosY + Unit->FacetLen;  Unit->Points[1].Z = Unit->PosZ;
  /*
  Unit->Points[1].X = Unit->PosX + Unit->FacetLen;  Unit->Points[1].Y = Unit->PosY + Unit->FacetLen;  Unit->Points[1].Z = Unit->PosZ;
  Unit->Points[1].X = Unit->PosX + Unit->FacetLen;  Unit->Points[1].Y = Unit->PosY + Unit->FacetLen;  Unit->Points[1].Z = Unit->PosZ;
  Unit->Points[1].X = Unit->PosX + Unit->FacetLen;  Unit->Points[1].Y = Unit->PosY + Unit->FacetLen;  Unit->Points[1].Z = Unit->PosZ;
  Unit->Points[1].X = Unit->PosX + Unit->FacetLen;  Unit->Points[1].Y = Unit->PosY + Unit->FacetLen;  Unit->Points[1].Z = Unit->PosZ;
  Unit->Points[1].X = Unit->PosX + Unit->FacetLen;  Unit->Points[1].Y = Unit->PosY + Unit->FacetLen;  Unit->Points[1].Z = Unit->PosZ;
  Unit->Points[1].X = Unit->PosX + Unit->FacetLen;  Unit->Points[1].Y = Unit->PosY + Unit->FacetLen;  Unit->Points[1].Z = Unit->PosZ;
  Unit->Points[1].X = Unit->PosX + Unit->FacetLen;  Unit->Points[1].Y = Unit->PosY + Unit->FacetLen;  Unit->Points[1].Z = Unit->PosZ;
  */
} /* End of 'RK2_UnitCubeInit' function */

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
  INT i;
  for (i = 0; i < 9; i++)
  {
    Unit->Points[i].X += cos(Ani->Time);
    Unit->Points[i].Y += cos(Ani->Time);
    Unit->Points[i].Z += cos(Ani->Time) / 100;
  }
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
  INT i, ls = 2;
  DBL j;
  CHAR Str[MAX_PATH];

  for (i = 0; i < 9; i++)
  {
    NPnt = RK2_RndWorldToScreen(Unit->Points[i]);
    Ellipse(Ani->hDC, NPnt.X - 5 / NPnt.Z, NPnt.Y - 5 / NPnt.Z, NPnt.X + 5 / NPnt.Z, NPnt.Y + 5 / NPnt.Z);
  }
  
  /* Axes */
  SelectObject(Ani->hDC, GetStockObject(DC_BRUSH));
  SelectObject(Ani->hDC, GetStockObject(NULL_PEN));

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

  Unit->PosX = Unit->PosY = Unit->PosZ = 0;
  return (rk2UNIT *)Unit;
} /* End of 'RK2_UnitCubeCreate' function */

/* END OF 'SAMPCUBE.C' FILE */
