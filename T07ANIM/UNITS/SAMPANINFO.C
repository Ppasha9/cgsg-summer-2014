/* FILENAME: SAMPANINFO.C
 * PROGRAMMER: RK2
 * PURPOSE: Animation unit handle module.
 * LAST UPDATE: 12.06.2014
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "../anim/anim.h"

/* Unit Animation control render function.
 * ARGUMENTS:
 *   - Self pointer:
 *       rk2UNIT *Unit;
 *   - Animation context pointer:
 *       rk2ANIM *Ani;
 * RETURNS: None.
 */
static VOID UnitAnimInfoRender(rk2UNIT *Unit, rk2ANIM *Ani)
{
  static CHAR Buf[1000];

  sprintf(Buf, "FPS: %.3f", Ani->FPS);
  SetWindowText(Ani->hWnd, Buf);
} /* End of 'RK2_AnimUnitRender' function */

/* Unit Animation control create function.
 * ARGUMENTS: None.
 * RETURNS:
 *   (rk2UNIT *) - pointer for new animation unit.
 */
rk2UNIT *RK2_UnitAnimInfoCreate( VOID )
{
  rk2UNIT *Unit;

  if ((Unit = RK2_AnimUnitCreate(sizeof(rk2UNIT))) == NULL)
    return NULL;

  /* Base fields filling */
  Unit->Render = (VOID *)UnitAnimInfoRender;
  return Unit;
} /* End of 'RK2_UnitAnimContolCreate' function */

/* END OF 'SAMPANINFO.C' FILE */
