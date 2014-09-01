/* FILENAME: SAMPBOAT.C
 * PROGRAMMER: RK2
 * PURPOSE: Animation unit handle module.
 * LAST UPDATE: 14.08.2014
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "../anim/anim.h"
#include "../gobjects/gobj.h"

/* Boat physics */
typedef struct tagrk2UNIT_BOAT_PHYS
{
  rk2VEC Acc;      /* acceleration */
  rk2VEC Speed;    /* Speed */
  rk2VEC RotSpeed; /* Rotations */
} rk2UNIT_BOAT_PHYS;

/* Unit boat struct definition */
typedef struct tagrk2UNIT_BOAT
{
  RK2_UNIT_BASE_FIELDS;           /* Base fields */
  rk2GOBJ GObj;                   /* Geometry Object */

  rk2VEC CamShift;                 /* Camera position */

  UINT ShaderProg;                /* Shader */
  CHAR 
    ShaderVFileName[MAX_PATH],    /* Shaders file names */
    ShaderFFileName[MAX_PATH];
  rk2CAMERA
    ObjCam;                       /* Object camera */

  rk2MATR4x4 MatrVLA;             /* View Look at matrix */
  rk2UNIT_BOAT_PHYS Phys;         /* Object physics */
} rk2UNIT_BOAT;

/* Unit boat init function.
 * ARGUMENTS:
 *   - Self pointer:
 *       rk2UNIT_BOAT *Unit;
 *   - Animation context pointer:
 *       rk2ANIM *Ani;
 * RETURNS: None.
 */
static VOID UnitBoatInit( rk2UNIT_BOAT *Unit, rk2ANIM *Ani )
{
  if ((Unit->ShaderProg = RK2_ShadProgInit(Unit->ShaderVFileName, Unit->ShaderFFileName)) == 0)
    Unit->ShaderProg = Ani->ShaderDef;

  RK2_RndCameraSet(&Unit->ObjCam, Unit->ObjCam.Loc, VecSumVec(Unit->ObjCam.Loc, VecSet(1, 0, 0)), VecSet(0, 1, 0));
  RK2_RndCameraUpdateInfo(&Unit->ObjCam);

  Unit->CamShift = VecSet(0, 2.0, -18);

  RK2_GObjLoad(&Unit->GObj, "..\\gobjects\\seagul\\seagul.object");
} /* End of 'RK2_UnitBoatInit' function */

/* Unit boat destructor function.
* ARGUMENTS:
*   - Self pointer:
*       rk2UNIT_BOAT *Unit;
*   - Animation context pointer:
*       rk2ANIM *Ani;
* RETURNS: None.
*/
static VOID UnitBoatClose( rk2UNIT_BOAT *Unit, rk2ANIM *Ani )
{
  RK2_GObjFree(&Unit->GObj);
  if (Unit->ShaderProg != Ani->ShaderDef)
    RK2_ShadProgClose(Unit->ShaderProg);
} /* End of 'RK2_UnitBoatClose' function */

/* Unit boat mouse stting response function.
 * ARGUMENTS:
 *   - Self pointer:
 *       rk2UNIT *Unit;
 *   - Animation context pointer:
 *       rk2ANIM *Ani;
 * RETURNS: None.
 */
static VOID UnitBoatCameraSet( rk2UNIT_BOAT *Unit, rk2ANIM *Ani )
{
  rk2MATR4x4 MatrVLACam;
  if (!Ani->IsPause)
  {
    RK2_RndCameraUpdateInfo(&Unit->ObjCam);
    Unit->MatrVLA = MatrTranspose(MatrViewLookAt(VecSet(0, 0, 0), VecNeg(Unit->ObjCam.Dir), Unit->ObjCam.Up));

    MatrVLACam = MatrTranspose(MatrViewLookAt(VecSet(0, 0, 0), VecNeg(Unit->ObjCam.Dir), VecSet(0, 1, 0)));
    RK2_RndCameraSet(&Ani->RndCamera, VecSumVec(Unit->ObjCam.Loc, VecMultMatr(Unit->CamShift, MatrVLACam)),
      Unit->ObjCam.Loc, VecSet(0, 1, 0));

    RK2_RndCameraUpdateInfo(&Ani->RndCamera);
  }
} /* End of 'UnitBoatCameraSet' function */

/* Unit boat response function.
 * ARGUMENTS:
 *   - Self pointer:
 *       rk2UNIT *Unit;
 *   - Animation context pointer:
 *       rk2ANIM *Ani;
 * RETURNS: None.
 */
static VOID UnitBoatResponse( rk2UNIT_BOAT *Unit, rk2ANIM *Ani )
{
  Unit->Phys.Acc = VecSet(0, 0, 0);
  RK2_RndCameraUpdateInfo(&Unit->ObjCam);

  if (!Ani->IsPause)
  {
    if (Ani->Keys['I'])
      Unit->Phys.Speed = VecSumVec(Unit->Phys.Speed, VecSet(0, 0, 0.01));
    if (Ani->Keys['K'])
      Unit->Phys.Speed = VecSumVec(Unit->Phys.Speed, VecSet(0, 0, -0.01));
    if (Ani->JoyY)
      Unit->Phys.Speed = VecSumVec(Unit->Phys.Speed, VecSet(0, 0, -Ani->JoyY * 0.01));

    if (Ani->Keys['Q'])
      RK2_RndCameraRotateDir(&Unit->ObjCam, -5);
    if (Ani->Keys['E'])
      RK2_RndCameraRotateDir(&Unit->ObjCam, 5);

    if (Ani->Keys['A'])
      RK2_RndCameraRotateUp(&Unit->ObjCam, 1);
    if (Ani->Keys['D'])
      RK2_RndCameraRotateUp(&Unit->ObjCam, -1);

    if (Ani->JoyR)
    {
      RK2_RndCameraRotateUp(&Unit->ObjCam, -Ani->JoyR);
      Unit->Phys.RotSpeed.Z = Ani->JoyR * 10;
    }

    if (Ani->Keys['W'])
      RK2_RndCameraRotateRight(&Unit->ObjCam, -0.5);
    if (Ani->Keys['S'])
      RK2_RndCameraRotateRight(&Unit->ObjCam, 0.5);
    if (Ani->JoyZ)
      RK2_RndCameraRotateRight(&Unit->ObjCam, -Ani->JoyZ);

    UnitBoatCameraSet(Unit, Ani);

    Unit->Phys.Speed = VecSumVec(Unit->Phys.Speed, Unit->Phys.Acc);
    Unit->ObjCam.Loc = VecSumVec(Unit->ObjCam.Loc, VecMultMatr(Unit->Phys.Speed, Unit->MatrVLA));
    Unit->ObjCam.At = VecSumVec(Unit->ObjCam.Loc, Unit->ObjCam.Dir);
  }
} /* End of 'RK2_UnitBoatResponse' function */

/* Unit boat render function.
 * ARGUMENTS:
 *   - Self pointer:
 *       rk2UNIT *Unit;
 *   - Animation context pointer:
 *       rk2ANIM *Ani;
 * RETURNS: None.
 */
static VOID UnitBoatRender( rk2UNIT_BOAT *Unit, rk2ANIM *Ani )
{
  // UINT loc;

  Ani->RndMatrWorld = MatrMultMatr(MatrMultMatr(Ani->RndMatrWorld, Unit->MatrVLA),
                                   MatrScale(0.1, 0.1, 0.1));

  Ani->RndMatrWorld = MatrMultMatr(Ani->RndMatrWorld,
                                   MatrTranslate(MatrDefault(),
                                   Unit->ObjCam.Loc.X, Unit->ObjCam.Loc.Y, Unit->ObjCam.Loc.Z));

  RK2_RndCameraUpdateInfo(&Ani->RndCamera);

  RK2_RndBuildMatrix();

  glUseProgram(Unit->ShaderProg);
  RK2_RndShadSendGlobInfo(Unit->ShaderProg, Ani);
  RK2_GObjDraw(Unit->ShaderProg, Ani, &Unit->GObj);

  glBegin(GL_LINES);
    glColor3d(1, 0, 0);
    glVertex3d(Unit->ObjCam.Loc.X, Unit->ObjCam.Loc.Y, Unit->ObjCam.Loc.Z);
    glVertex3d(Unit->ObjCam.Loc.X + Unit->ObjCam.Up.X,
               Unit->ObjCam.Loc.Y + Unit->ObjCam.Up.Y,
               Unit->ObjCam.Loc.Z + Unit->ObjCam.Up.Z);

    glColor3d(0, 1, 0);
    glVertex3d(Unit->ObjCam.Loc.X, Unit->ObjCam.Loc.Y, Unit->ObjCam.Loc.Z);
    glVertex3d(Unit->ObjCam.Loc.X + Unit->ObjCam.Right.X, 
               Unit->ObjCam.Loc.Y + Unit->ObjCam.Right.Y, 
               Unit->ObjCam.Loc.Z + Unit->ObjCam.Right.Z);

    glColor3d(0, 0, 1);
    glVertex3d(Unit->ObjCam.Loc.X, Unit->ObjCam.Loc.Y, Unit->ObjCam.Loc.Z);
    glVertex3d(Unit->ObjCam.Loc.X + Unit->ObjCam.Dir.X,
               Unit->ObjCam.Loc.Y + Unit->ObjCam.Dir.Y,
               Unit->ObjCam.Loc.Z + Unit->ObjCam.Dir.Z);

    glColor3d(1, 1, 1);
    glVertex3d(Unit->ObjCam.Loc.X, Unit->ObjCam.Loc.Y, Unit->ObjCam.Loc.Z);
    glVertex3d(Unit->ObjCam.Loc.X * 0.995, Unit->ObjCam.Loc.Y * 0.995, Unit->ObjCam.Loc.Z * 0.995);
  glEnd();

  glUseProgram(Ani->ShaderDef);
  Ani->RndMatrWorld = MatrDefault();
} /* End of 'RK2_UnitBoatRender' function */

/* Unit boat create function.
 * ARGUMENTS:
 *   - Postion on world map:
 *       INT PosXPosY;
 *   - Angles of rotation:
 *       DBL RotAngleX, RotAngleY, RotAngleZ;
 * RETURNS:
 *   (rk2UNIT_BOAT *) - pointer for new animation unit.
 */
rk2UNIT *RK2_UnitBoatCreate( INT PosX, INT PosY, INT PosZ, 
                             DBL RotAngleX, DBL RotAngleY, DBL RotAngleZ )
{
  rk2UNIT_BOAT *Unit;

  if ((Unit = (rk2UNIT_BOAT *)RK2_AnimUnitCreate(sizeof(rk2UNIT_BOAT))) == NULL)
    return NULL;

  /* Base fields filling */
  Unit->Render = (VOID *)UnitBoatRender;
  Unit->Init = (VOID *)UnitBoatInit;
  Unit->Close = (VOID *)UnitBoatClose;
  Unit->Response = (VOID *)UnitBoatResponse;

  // Unit->VecPos = VecSet(PosX, PosY, PosZ);
  Unit->ObjCam.Loc = VecSet(PosX, PosY, PosZ); // Unit->VecPos;
  // Unit->ObjCam.At = 
  return (rk2UNIT *)Unit;
} /* End of 'RK2_UnitBoatCreate' function */

/* END OF 'SAMPBOAT.C' FILE */
