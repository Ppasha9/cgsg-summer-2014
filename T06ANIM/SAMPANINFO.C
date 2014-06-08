/* FILENAME: SAMPANINFO.C
 * PROGRAMMER: RK2
 * PURPOSE: Animation unit handle module.
 * LAST UPDATE: 08.06.2014
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "anim.h"

/* Animation info unit render function.
 * ARGUMENTS:
 *   - Self pointer:
 *       rk2UNIT *Unit;
 *   - Animation context pointer:
 *       rk2ANIM *Ani;
 * RETURNS: None.
 */
static VOID InfoUnitRender( rk2UNIT *Unit, rk2ANIM *Ani )
{
  static CHAR Buf[1000];

  SetBkMode(Ani->hDC, TRANSPARENT);
  SetTextColor(Ani->hDC, RGB(255, 255, 155));
  TextOut(Ani->hDC, 10, 10, Buf, sprintf(Buf, "FPS: %.3f", Ani->FPS));
} /* End of 'RK2_AnimUnitRender' function */


/* Animation info unit create function.
 * ARGUMENTS: None.
 * RETURNS:
 *   (rk2UNIT *) - pointer for new animation unit.
 */
rk2UNIT *RK2_InfoUnitCreate( VOID )
{
  rk2UNIT *Unit;

  if ((Unit = RK2_AnimUnitCreate(sizeof(rk2UNIT))) == NULL)
    return NULL;
  /* Base fields filling */
  Unit->Render = (VOID *)InfoUnitRender;
  return Unit;
} /* End of 'RK2_InfoUnitCreate' function */

/* END OF 'SAMPANINFO.C' FILE */
