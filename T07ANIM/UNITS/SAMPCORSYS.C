/* FILENAME: SAMPCORSYS.C
 * PROGRAMMER: RK2
 * PURPOSE: Animation unit handle module.
 * LAST UPDATE: 09.06.2014
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "../anim/anim.h"
#include "units.h"

/* Unit Coord system struct definition */
typedef struct tagrk2UNIT_CORSYS
{
  RK2_UNIT_BASE_FIELDS;           /* Base fields */
} rk2UNIT_CORSYS;

/* Unit Coord system render function.
 * ARGUMENTS:
 *   - Self pointer:
 *       rk2UNIT *Unit;
 *   - Animation context pointer:
 *       rk2ANIM *Ani;
 * RETURNS: None.
 */
static VOID UnitCorSysRender( rk2UNIT_CORSYS *Unit, rk2ANIM *Ani )
{
  Ani->RndMatrWorld = MatrDefault(); /* MatrRotateY(MatrDefault(), Ani->Time * 10); */
  RK2_RndBuildMatrix();

  glUseProgram(0);
  glLoadMatrixf(&Ani->RndMatrRes.A[0][0]);

  glBegin(GL_LINES);
    glColor3d(1, 0, 0);
    glVertex3d(0, 0, 0);
    glVertex4d(1, 0, 0, 0);

    glColor3d(0, 1, 0);
    glVertex3d(0, 0, 0);
    glVertex4d(0, 1, 0, 0);

    glColor3d(0, 0, 1);
    glVertex3d(0, 0, 0);
    glVertex4d(0, 0, 1, 0);
  glEnd();
} /* End of 'RK2_UnitCorSysRender' function */

/* Unit Coord system create function.
 * ARGUMENTS: None.
 * RETURNS:
 *   (rk2UNIT_CORSYS *) - pointer for new animation unit.
 */
rk2UNIT *RK2_UnitCorSysCreate( VOID )
{
  rk2UNIT_CORSYS *Unit;

  if ((Unit = (rk2UNIT_CORSYS *)RK2_AnimUnitCreate(sizeof(rk2UNIT_CORSYS))) == NULL)
    return NULL;

  /* Base fields filling */
  Unit->Render = (VOID *)UnitCorSysRender;

  return (rk2UNIT *)Unit;
} /* End of 'RK2_UnitCorSysCreate' function */

/* END OF 'SAMPCORSYS.C' FILE */
