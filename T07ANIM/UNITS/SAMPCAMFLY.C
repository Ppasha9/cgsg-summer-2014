/* FILENAME: SAMPCAMFLY.C
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

/* Unit flying camera struct definition */
typedef struct tagrk2UNIT_CAMFLY
{
  RK2_UNIT_BASE_FIELDS;           /* Base fields */
  rk2VEC CamPos;                  /* Points of cube */
} rk2UNIT_CAMFLY;

/* Unit flying camera getting coords function.
 * ARGUMENTS:
 *   - Self pointer:
 *       rk2UNIT *Unit;
 *   - Animation context pointer:
 *       rk2ANIM *Ani;
 *   - Last point position:
 *       rk2VEC SrcPoint;
 *   - Time:
 *       DBL Time;
 * RETURNS: 
 *   (rk2VEC) Coords of point.
 */
static rk2VEC UnitCamFlyGetCoordByTime(rk2UNIT_CAMFLY *Unit, rk2ANIM *Ani, rk2VEC SrcPoint, DBL Time )
{
  rk2VEC VecSaved = SrcPoint;
  static INT MinDist = 30, MaxDist = 300;

  SrcPoint.X += cos(Time * 3) * ((INT)Time % 20);
  if (SrcPoint.X < -MaxDist)
    SrcPoint.X = -MaxDist;
  if (SrcPoint.X > MaxDist)
    SrcPoint.X = MaxDist;

  SrcPoint.Y += sin(Time) * ((INT)Time % 10);
  if (SrcPoint.Y < -MaxDist)
    SrcPoint.Y = -MaxDist;
  if (SrcPoint.Y > MaxDist)
    SrcPoint.Y = MaxDist;
  
  SrcPoint.Z += sin(Time * 2) * ((INT)Time % 30);
  if (SrcPoint.Z< -MaxDist)
    SrcPoint.Z = -MaxDist;
  if (SrcPoint.Z > MaxDist)
    SrcPoint.Z = MaxDist;

  if (SrcPoint.X > -MinDist && SrcPoint.X < 0 &&
      SrcPoint.Y > -MinDist && SrcPoint.Y < 0 &&
      SrcPoint.Z > -MinDist && SrcPoint.Z < 0)
    return VecSaved;

  return SrcPoint;
} /* End of 'UnitCamFlyGetCoordByTime' function */

/* Unit flying camera response function.
 * ARGUMENTS:
 *   - Self pointer:
 *       rk2UNIT *Unit;
 *   - Animation context pointer:
 *       rk2ANIM *Ani;
 * RETURNS: None.
 */
static VOID UnitCamFlyResponse( rk2UNIT_CAMFLY *Unit, rk2ANIM *Ani )
{
  /*
  if (!Ani->IsPause)
    Unit->CamPos = UnitCamFlyGetCoordByTime(Unit, Ani, Unit->CamPos, Ani->Time);
  */
} /* End of 'RK2_UnitCamFlyResponse' function */

/* Unit flying camera render function.
 * ARGUMENTS:
 *   - Self pointer:
 *       rk2UNIT *Unit;
 *   - Animation context pointer:
 *       rk2ANIM *Ani;
 * RETURNS: None.
 */
static VOID UnitCamFlyRender( rk2UNIT_CAMFLY *Unit, rk2ANIM *Ani )
{
  /*
  if (Ani->IsPause)
    return;
  RK2_RndCameraSet(&Ani->RndCamera, Unit->CamPos, VecSet(0, 0, 0), VecSet(0, 1, 0));
  */
  /* UnitCamFlyGetCoordByTime(Unit, Ani, Unit->CamPos, Ani->Time + 0.2); */

  /// Ani->RndMatrWorld = MatrDefault();
  /// RK2_RndBuildMatrix();
} /* End of 'RK2_UnitCamFlyRender' function */

/* Unit flying camera create function.
 * ARGUMENTS: None.
 * RETURNS:
 *   (rk2UNIT_CAMFLY *) - pointer for new animation unit.
 */
rk2UNIT *RK2_UnitCamFlyCreate( VOID )
{
  rk2UNIT_CAMFLY *Unit;

  if ((Unit = (rk2UNIT_CAMFLY *)RK2_AnimUnitCreate(sizeof(rk2UNIT_CAMFLY))) == NULL)
    return NULL;

  /* Base fields filling */
  Unit->Render = (VOID *)UnitCamFlyRender;
  Unit->Response = (VOID *)UnitCamFlyResponse;

  Unit->CamPos.X = Unit->CamPos.Y = Unit->CamPos.Z = 70;
  return (rk2UNIT *)Unit;
} /* End of 'RK2_UnitCamFlyCreate' function */

/* END OF 'SAMPCAMFLY.C' FILE */
