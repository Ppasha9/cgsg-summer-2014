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
  Unit->Points[0].X = Unit->PosX;       Unit->Points[0].Y = Unit->PosY;       Unit->Points[0].Z = Unit->PosZ;
  Unit->Points[1].X = Unit->PosX + 3;   Unit->Points[1].Y = Unit->PosY + 2;   Unit->Points[1].Z = Unit->PosZ + 2;
  Unit->Points[2].X = Unit->PosX + 100;  Unit->Points[2].Y = Unit->PosY + 6;   Unit->Points[1].Z = Unit->PosZ + 4;
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
  ;
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
  POINT NPnt;
  INT i;
  for (i = 0; i < 9; i++)
  {
    NPnt = RK2_RndWorldToScreen(Unit->Points[i], Ani);
    Ellipse(Ani->hDC, NPnt.x - 5, NPnt.y - 5, NPnt.x + 5, NPnt.y + 5);
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
