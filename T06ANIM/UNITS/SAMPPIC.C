/* FILENAME: SAMPPIC.C
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
typedef struct tagrk2UNIT_PIC
{
  RK2_UNIT_BASE_FIELDS;           /* Base fields */
  rk2IMAGE Img;                   /* Image */
  INT PosX, PosY;                 /* Position on window */
  CHAR FileName[MAX_PATH];        /* Source picture file name */
  DWORD DrawingType;              /* Type of blit: SRCCOPY, SRCAND, etc. */
} rk2UNIT_PIC;

/* Unit Pict initialization function.
 * ARGUMENTS:
 *   - Self pointer:
 *       rk2UNIT_PIC *Unit;
 *   - Animation context pointer:
 *       rk2ANIM *Ani;
 * RETURNS: None.
 */
static VOID UnitPictInit( rk2UNIT_PIC *Unit, rk2ANIM *Ani )
{
  ImageLoad(&Unit->Img, Unit->FileName);
} /* End of 'UnitPictInit' function */

/* Unit Pict destructor function.
 * ARGUMENTS:
 *   - Self pointer:
 *       rk2UNIT_PIC *Unit;
 *   - Animation context pointer:
 *       rk2ANIM *Ani;
 * RETURNS: None.
 */
static VOID UnitPictClose( rk2UNIT_PIC *Unit, rk2ANIM *Ani )
{
} /* End of 'UnitPictClose' function */

/* Unit Pict update unit function.
 * ARGUMENTS:
 *   - Self pointer:
 *       rk2UNIT_PIC *Unit;
 *   - Animation context pointer:
 *       rk2ANIM *Ani;
 * RETURNS: None.
 */
static VOID UnitPictResponse( rk2UNIT_PIC *Unit, rk2ANIM *Ani )
{
} /* End of 'UnitPictResponse' function */

/* Unit Pict rendering function.
 * ARGUMENTS:
 *   - Self pointer:
 *       rk2UNIT_PIC *Unit;
 *   - Animation context pointer:
 *       rk2ANIM *Ani;
 * RETURNS: None.
 */
static VOID UnitPictRender( rk2UNIT_PIC *Unit, rk2ANIM *Ani )
{
  ImageDraw(Ani->hDC, &Unit->Img, Unit->DrawingType, Unit->PosX, Unit->PosY,
            Unit->PosX + Unit->Img.W, Unit->PosY + Unit->Img.H);
} /* End of 'RK2_AnimUnitRender' function */

/* Creating unit pict animation function.
 * ARGUMENTS:
 *   - Source file name:
 *      CHAR *FileName;
 *   - Position on window:
 *      INT PosX, PosY;
 *   - Type of draw: SRCCOPY, SRCAND, etc...
 *      DWORD DrawingType;
 * RETURNS:
 *   (rk2UNIT *) Pointer for created animation object.
 */
rk2UNIT *RK2_UnitPictCreate( CHAR *FileName, INT PosX, INT PosY, DWORD DrawingType )
{
  rk2UNIT_PIC *Unit;

  if ((Unit = (rk2UNIT_PIC *)RK2_AnimUnitCreate(sizeof(rk2UNIT_PIC))) == NULL)
    return NULL;
  
  /* Filling standart pointers */
  Unit->Init = (VOID *)UnitPictInit;
  Unit->Close = (VOID *)UnitPictClose;
  Unit->Response = (VOID *)UnitPictResponse;
  Unit->Render = (VOID *)UnitPictRender;

  strcpy(Unit->FileName, FileName);
  Unit->PosX = PosX;
  Unit->PosY = PosY;
  Unit->DrawingType = DrawingType;

  return (rk2UNIT *)Unit;
} /* End of 'RK2_UnitPictCreate' function */

/* END OF 'SAMPPIC.C' FILE */
