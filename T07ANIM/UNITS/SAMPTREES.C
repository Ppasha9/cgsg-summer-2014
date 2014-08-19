/* FILENAME: SAMPTREES.C
 * PROGRAMMER: RK2
 * PURPOSE: Animation unit handle module.
 * LAST UPDATE: 14.08.2014
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "../image.h"

#include "../anim.h"
#include "../render.h"
#include "../shader.h"
#include "../gobjects/gobj.h"

/* Unit Trees struct definition */
typedef struct tagrk2UNIT_OBJ
{
  RK2_UNIT_BASE_FIELDS;           /* Base fields */
  CHAR FileName[MAX_PATH];        /* File to object */

  UINT ShaderProg;                /* Shader */
} rk2UNIT_GOBJ;

/* Unit Trees init function.
 * ARGUMENTS:
 *   - Self pointer:
 *       rk2UNIT_GOBJ *Unit;
 *   - Animation context pointer:
 *       rk2ANIM *Ani;
 * RETURNS: None.
 */
static VOID UnitTreesInit( rk2UNIT_GOBJ *Unit, rk2ANIM *Ani )
{
  INT i;
  INT x, y;

  CHAR Buf[1000];
  Unit->ShaderProg = Ani->ShaderDef;

  Unit->TreesObjCnt = 3;
  for (i = 0; i < Unit->TreesObjCnt; i++)
  {
    sprintf(Buf, "..\\gobjects\\Trees\\Tree%d.object", i);
    RK2_GObjLoad(&Unit->TrObjs[i], Buf);
  }
  i = 0;
  while (i < Unit->TreesCnt)
  {
    x = (rand() / RAND_MAX - 1) * 100;
    y = (rand() / RAND_MAX - 1) * 100;
    Unit->VecPos[i] = VecSet(x, y);
    i++;
  }
} /* End of 'RK2_UnitGObjInit' function */

/* Unit Trees destructor function.
* ARGUMENTS:
*   - Self pointer:
*       rk2UNIT_GOBJ *Unit;
*   - Animation context pointer:
*       rk2ANIM *Ani;
* RETURNS: None.
*/
static VOID UnitTreesClose( rk2UNIT_GOBJ *Unit, rk2ANIM *Ani )
{
  if (Unit->ShaderProg != Ani->ShaderDef)
    RK2_ShadProgClose(Unit->ShaderProg);
} /* End of 'RK2_UnitGObjClose' function */

/* Unit Trees response function.
 * ARGUMENTS:
 *   - Self pointer:
 *       rk2UNIT *Unit;
 *   - Animation context pointer:
 *       rk2ANIM *Ani;
 * RETURNS: None.
 */
static VOID UnitTreesResponse( rk2UNIT_GOBJ *Unit, rk2ANIM *Ani )
{
} /* End of 'RK2_UnitGObjResponse' function */

/* Unit Trees render function.
 * ARGUMENTS:
 *   - Self pointer:
 *       rk2UNIT *Unit;
 *   - Animation context pointer:
 *       rk2ANIM *Ani;
 * RETURNS: None.
 */
static VOID UnitTreesRender( rk2UNIT_GOBJ *Unit, rk2ANIM *Ani )
{
  UINT loc;
  
  RK2_RndBuildMatrix();

  if (Unit->ShaderProg)
  {
    glUseProgram(Unit->ShaderProg);
    loc = glGetUniformLocation(Unit->ShaderProg, "Matr");
    if (loc != -1)
      glUniformMatrix4fv(loc, 1, FALSE, &Ani->RndMatrRes.A[0][0]);

    loc = glGetUniformLocation(Unit->ShaderProg, "Time");
    if (loc != -1)
      glUniform1f(loc, Ani->Time);

    loc = glGetUniformLocation(Unit->ShaderProg, "UnitPos");
    if (loc != -1)
      glUniformMatrix4fv(loc, 1, FALSE, &Unit->VecPos[0].X);

    loc = glGetUniformLocation(Unit->ShaderProg, "Trans");
    if (loc != -1)
      glUniform1f(loc, 1.0);
  }

  // RK2_GObjDraw(&Unit->GObj);
  glUseProgram(0);
} /* End of 'RK2_UnitGObjRender' function */

/* Unit Trees create function.
 * ARGUMENTS:
 *   - Count of trees:
 *       INT TreesCnt;
 * RETURNS:
 *   (rk2UNIT_GOBJ *) - pointer for new animation unit.
 */
rk2UNIT *RK2_UnitTreesCreate( INT TreesCnt )
{
  rk2UNIT_GOBJ *Unit;

  if ((Unit = (rk2UNIT_GOBJ *)RK2_AnimUnitCreate(sizeof(rk2UNIT_GOBJ))) == NULL)
    return NULL;

  /* Base fields filling */
  Unit->Render = (VOID *)UnitTreesRender;
  Unit->Init = (VOID *)UnitTreesInit;
  Unit->Close = (VOID *)UnitTreesClose;
  Unit->Response = (VOID *)UnitTreesResponse;

  Unit->TreesCnt = TreesCnt;

  return (rk2UNIT *)Unit;
} /* End of 'RK2_UnitGObjCreate' function */

/* END OF 'SAMPCUBE.C' FILE */
