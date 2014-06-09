/* FILENAME: SAMPLOGO.C
 * PROGRAMMER: RK2
 * PURPOSE: Animation unit handle module.
 * LAST UPDATE: 09.06.2014
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "../anim.h"
#include "../image.h"

/* Unit picture struct definition */
typedef struct tagrk2UNIT_LOGO
{
  RK2_UNIT_BASE_FIELDS;           /* Base fields */
  rk2IMAGE ImgAnd, ImgXor;        /* Logo Images */
  INT PosX, PosY;                 /* Position on window */
} rk2UNIT_LOGO;

/* Unit Pict initialization function.
 * ARGUMENTS:
 *   - Self pointer:
 *       rk2UNIT_LOGO *Unit;
 *   - Animation context pointer:
 *       rk2ANIM *Ani;
 * RETURNS: None.
 */
static VOID UnitLogoInit( rk2UNIT_LOGO *Unit, rk2ANIM *Ani )
{
  ImageLoad(&Unit->ImgAnd, "PICS\\logo_and.bmp");
  ImageLoad(&Unit->ImgXor, "PICS\\logo_xor.bmp");
} /* End of 'UnitLogoInit' function */

/* Unit Pict destructor function.
 * ARGUMENTS:
 *   - Self pointer:
 *       rk2UNIT_LOGO *Unit;
 *   - Animation context pointer:
 *       rk2ANIM *Ani;
 * RETURNS: None.
 */
static VOID UnitLogoClose( rk2UNIT_LOGO *Unit, rk2ANIM *Ani )
{
} /* End of 'UnitLogoClose' function */

/* Unit Pict update unit function.
 * ARGUMENTS:
 *   - Self pointer:
 *       rk2UNIT_LOGO *Unit;
 *   - Animation context pointer:
 *       rk2ANIM *Ani;
 * RETURNS: None.
 */
static VOID UnitLogoResponse( rk2UNIT_LOGO *Unit, rk2ANIM *Ani )
{
  Unit->PosX += Ani->JoyX * 10;
  Unit->PosY += Ani->JoyY * 10;
} /* End of 'UnitLogoResponse' function */

/* Unit Pict rendering function.
 * ARGUMENTS:
 *   - Self pointer:
 *       rk2UNIT_LOGO *Unit;
 *   - Animation context pointer:
 *       rk2ANIM *Ani;
 * RETURNS: None.
 */
static VOID UnitLogoRender( rk2UNIT_LOGO *Unit, rk2ANIM *Ani )
{
  ImageDraw(Ani->hDC, &Unit->ImgAnd, SRCAND, Unit->PosX, Unit->PosY,
            Unit->PosX + Unit->ImgAnd.W, Unit->PosY + Unit->ImgAnd.H);
  ImageDraw(Ani->hDC, &Unit->ImgXor, SRCINVERT, Unit->PosX, Unit->PosY,
            Unit->PosX + Unit->ImgXor.W, Unit->PosY + Unit->ImgXor.H);
} /* End of 'RK2_AnimUnitRender' function */

/* Creating unit pict animation function.
 * ARGUMENTS:
 *   - Position on window:
 *      INT PosX, PosY;
 * RETURNS:
 *   (rk2UNIT *) Pointer for created animation object.
 */
rk2UNIT *RK2_UnitLogoCreate( INT PosX, INT PosY )
{
  rk2UNIT_LOGO *Unit;

  if ((Unit = (rk2UNIT_LOGO *)RK2_AnimUnitCreate(sizeof(rk2UNIT_LOGO))) == NULL)
    return NULL;
  
  /* Filling standart pointers */
  Unit->Init = (VOID *)UnitLogoInit;
  Unit->Close = (VOID *)UnitLogoClose;
  Unit->Response = (VOID *)UnitLogoResponse;
  Unit->Render = (VOID *)UnitLogoRender;

  Unit->PosX = PosX;
  Unit->PosY = PosY;

  return (rk2UNIT *)Unit;
} /* End of 'RK2_UnitLogoCreate' function */

/* END OF 'SAMPPIC.C' FILE */
