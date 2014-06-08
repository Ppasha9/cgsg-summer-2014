/* FILENAME: SAMPCOW.C
 * PROGRAMMER: RK2
 * PURPOSE: Animation unit handle module.
 * LAST UPDATE: 08.06.2014
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "anim.h"

/* Unit cow struct definition */
typedef struct tagrk2UNIT_COW
{
  RK2_UNIT_BASE_FIELDS; /* Base fields */
  DBL ShiftX, ShiftY;   /* shift from screen left top point */
  INT Type;             /* Type */
} rk2UNIT_COW;

/* Unit Cow initialization function.
 * ARGUMENTS:
 *   - Self pointer:
 *       rk2UNIT_COW *Unit;
 *   - Animation context pointer:
 *       rk2ANIM *Ani;
 * RETURNS: None.
 */
static VOID CowUnitInit( rk2UNIT_COW *Unit, rk2ANIM *Ani )
{
  Unit->ShiftX = (DBL)Ani->W * rand() / RAND_MAX;
  Unit->ShiftY = (DBL)Ani->H * rand() / RAND_MAX;
  Unit->Type = rand() % 2;
} /* End of 'CowUnitInit' function */

/* Unit Cow destructor function.
 * ARGUMENTS:
 *   - Self pointer:
 *       rk2UNIT_COW *Unit;
 *   - Animation context pointer:
 *       rk2ANIM *Ani;
 * RETURNS: None.
 */
static VOID CowUnitClose( rk2UNIT_COW *Unit, rk2ANIM *Ani )
{
} /* End of 'CowUnitClose' function */

/* Unit Cow update unit function.
 * ARGUMENTS:
 *   - Self pointer:
 *       rk2UNIT_COW *Unit;
 *   - Animation context pointer:
 *       rk2ANIM *Ani;
 * RETURNS: None.
 */
static VOID CowUnitResponse( rk2UNIT_COW *Unit, rk2ANIM *Ani )
{
} /* End of 'CowUnitResponse' function */

/* Unit Cow rendering function.
 * ARGUMENTS:
 *   - Self pointer:
 *       rk2UNIT_COW *Unit;
 *   - Animation context pointer:
 *       rk2ANIM *Ani;
 * RETURNS: None.
 */
static VOID CowUnitRender( rk2UNIT_COW *Unit, rk2ANIM *Ani )
{
  DBL
    x = Unit->ShiftX + sin(Ani->Time + Unit->ShiftX) * 30,
    y = Unit->ShiftY + sin(Ani->Time + Unit->ShiftY) * 30;

  if (Unit->Type)
    Rectangle(Ani->hDC, x, y, x + 30, y + 30);
  else
    Ellipse(Ani->hDC, x, y, x + 30, y + 30);
} /* End of 'RK2_AnimUnitRender' function */

/* Creating unit cow animation function.
 * ARGUMENTS: None.
 * RETURNS:
 *   (rk2UNIT *) Pointer for created animation object.
 */
rk2UNIT *RK2_CowUnitCreate( VOID )
{
  rk2UNIT_COW *Unit;

  if ((Unit = (rk2UNIT_COW *)RK2_AnimUnitCreate(sizeof(rk2UNIT_COW))) == NULL)
    return NULL;
  /* Filling standart pointers */
  Unit->Init = (VOID *)CowUnitInit;
  Unit->Close = (VOID *)CowUnitClose;
  Unit->Response = (VOID *)CowUnitResponse;
  Unit->Render = (VOID *)CowUnitRender;

  return (rk2UNIT *)Unit;
} /* End of 'RK2_CowUnitCreate' function */

/* END OF 'SAMPCOW.C' FILE */
