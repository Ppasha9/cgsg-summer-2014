/* FILENAME: SAMPCUBE.C
 * PROGRAMMER: RK2
 * PURPOSE: Animation unit handle module.
 * LAST UPDATE: 09.06.2014
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "../image.h"

#include "../anim.h"
#include "../render.h"

/* Unit GObject struct definition */
typedef struct tagrk2UNIT_OBJ
{
  RK2_UNIT_BASE_FIELDS;           /* Base fields */
  rk2GOBJ GObj;                   /* Geometry Object */
  CHAR FileName[MAX_PATH];        /* File to object */
  rk2VEC VecPos;                  /* Postion of object on world map */
} rk2UNIT_GOBJ;

/* Unit GObject init function.
 * ARGUMENTS:
 *   - Self pointer:
 *       rk2UNIT_GOBJ *Unit;
 *   - Animation context pointer:
 *       rk2ANIM *Ani;
 * RETURNS: None.
 */
static VOID UnitGObjInit( rk2UNIT_GOBJ *Unit, rk2ANIM *Ani )
{
  RK2_GObjLoad(&Unit->GObj, Unit->FileName,Unit->VecPos);
} /* End of 'RK2_UnitGObjInit' function */

/* Unit GObject destructor function.
* ARGUMENTS:
*   - Self pointer:
*       rk2UNIT_GOBJ *Unit;
*   - Animation context pointer:
*       rk2ANIM *Ani;
* RETURNS: None.
*/
static VOID UnitGObjClose(rk2UNIT_GOBJ *Unit, rk2ANIM *Ani)
{
  RK2_GObjFree(&Unit->GObj);
} /* End of 'RK2_UnitGObjClose' function */

/* Unit GObject response function.
 * ARGUMENTS:
 *   - Self pointer:
 *       rk2UNIT *Unit;
 *   - Animation context pointer:
 *       rk2ANIM *Ani;
 * RETURNS: None.
 */
static VOID UnitGObjResponse( rk2UNIT_GOBJ *Unit, rk2ANIM *Ani )
{
} /* End of 'RK2_UnitGObjResponse' function */

/* Unit GObject render function.
 * ARGUMENTS:
 *   - Self pointer:
 *       rk2UNIT *Unit;
 *   - Animation context pointer:
 *       rk2ANIM *Ani;
 * RETURNS: None.
 */
static VOID UnitGObjRender( rk2UNIT_GOBJ *Unit, rk2ANIM *Ani )
{
  /* R2_RndMatrWorld = MatrRotate(Ani->Time * 30, 0, 0, 10); */
  RK2_RndMatrWorld = MatrRotateY(MatrDefault(), Ani->Time * 10);

  /*
  RK2_RndCameraSet(VecSet(10, 10, 10),
                   VecSet(0, 0, 0),
                   VecSet(0, 1, 0));
  */
  RK2_RndBuildMatrix();

  RK2_GObjDraw(&Unit->GObj, Ani->hDC);
} /* End of 'RK2_UnitGObjRender' function */

/* Unit GObject create function.
 * ARGUMENTS:
 *   - Load file name:
 *       CHAR *FileName;
 *   - Postion on world map:
 *       INT PosXPosY;
 * RETURNS:
 *   (rk2UNIT_GOBJ *) - pointer for new animation unit.
 */
rk2UNIT *RK2_UnitGObjCreate( CHAR *FileName, INT PosX, INT PosY, INT PosZ )
{
  rk2UNIT_GOBJ *Unit;

  if ((Unit = (rk2UNIT_GOBJ *)RK2_AnimUnitCreate(sizeof(rk2UNIT_GOBJ))) == NULL)
    return NULL;

  /* Base fields filling */
  Unit->Render = (VOID *)UnitGObjRender;
  Unit->Init = (VOID *)UnitGObjInit;
  Unit->Close = (VOID *)UnitGObjClose;
  Unit->Response = (VOID *)UnitGObjResponse;
  
  strcpy(Unit->FileName, FileName);
  Unit->VecPos = VecSet(PosX, PosY, PosZ);

  return (rk2UNIT *)Unit;
} /* End of 'RK2_UnitGObjCreate' function */

/* END OF 'SAMPCUBE.C' FILE */
