/* FILENAME: SAMPBOAT.C
 * PROGRAMMER: RK2
 * PURPOSE: Animation unit handle module.
 * LAST UPDATE: 17.06.2014
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

  rk2VEC VecPos;                  /* Postion of object on world map */
  DBL
    RotAngleX,                    /* Angles of rotation unit */
    RotAngleY,
    RotAngleZ;
  DBL
    LevelDif;                     /* Add to water level constant */
  rk2VEC CamDist;                 /* Distance from camera to boat */
  UINT ShaderProg;                /* Shader */
  CHAR 
    ShaderVFileName[MAX_PATH],    /* Shaders file names */
    ShaderFFileName[MAX_PATH];
  rk2CAMERA 
    Camera;                       /* Camera */
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

  Unit->Camera.Loc = Unit->VecPos;
  Unit->Camera.Up = VecSet(0, 1, 0);
  Unit->Camera.At = VecSet(0, 0, 0);
  Unit->CamDist = VecSet(0, 30, -50);
  Unit->LevelDif = 0.5;
  RK2_GObjLoad(&Unit->GObj, "..\\gobjects\\seagul.object");
  /* , Unit->VecPos); */
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
  rk2CAMERA DefCam;
  rk2MATR4x4 DefMatr;

  if (!Ani->IsPause)
  {
    DefMatr = MatrRotateY(MatrDefault(), Unit->RotAngleY);
    DefMatr = MatrMultMatr(DefMatr, MatrRotateX(MatrDefault(), Unit->RotAngleX));
    DefMatr = MatrMultMatr(DefMatr, MatrRotateZ(MatrDefault(), Unit->RotAngleZ + Unit->Phys.RotSpeed.Z));

    RK2_RndCameraSet(&DefCam, VecSumVec(Unit->VecPos, VecMultMatr(Unit->CamDist, DefMatr)),
                     Unit->VecPos, VecMultMatr(VecSet(0, 1, 0), DefMatr));
    /// RK2_RndCameraUpdateInfo(&DefCam);
    /// RK2_RndCameraNormalize(&DefCam);
    Unit->Camera = DefCam;
  }
  else 
    Unit->Camera = Ani->RndCamera;
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
  rk2MATR4x4 DefMatr;

  Unit->Phys.RotSpeed.Z = 0;
  Unit->Phys.Speed.X = 0;
  Unit->Phys.Speed.Y = 0;
  Unit->Phys.Acc = VecSet(0, 0, 0);
  if (!Ani->IsPause)
  {
    if (Ani->Keys['A'])
      Unit->RotAngleY += 0.3;
    if (Ani->Keys['D'])
      Unit->RotAngleY -= 0.3;
    if (Ani->JoyR)
    {
      Unit->RotAngleY -= Ani->JoyR;
      Unit->Phys.RotSpeed.Z = Ani->JoyR * 10;
    }

    if (Ani->Keys['W'])
      Unit->Phys.Acc = VecSumVec(Unit->Phys.Acc, VecSet(0, 0, 0.01));
    if (Ani->Keys['S'])
      Unit->Phys.Acc = VecSumVec(Unit->Phys.Acc, VecSet(0, 0, -0.01));
    if (Ani->JoyY)
      Unit->Phys.Acc = VecSumVec(Unit->Phys.Acc, VecSet(0, 0, -Ani->JoyY * 0.01));

    if (Ani->Keys['I'])
      Unit->RotAngleX += 0.3;
    if (Ani->Keys['K'])
      Unit->RotAngleX -= 0.3;
    if (Ani->JoyZ)
    {
      Unit->RotAngleX -= Ani->JoyZ;
      /// Unit->Phys.RotSpeed.Z = Ani->JoyR * 10;
    }
  }
  /*
  Unit->Phys.Speed.X *= Unit->Phys.Speed.Z;
  Unit->Phys.Speed.Y *= Unit->Phys.Speed.Z;
  */
  Unit->Phys.Speed = VecSumVec(Unit->Phys.Speed, Unit->Phys.Acc);

  DefMatr = MatrRotateY(MatrDefault(), Unit->RotAngleY);
  DefMatr = MatrMultMatr(DefMatr, MatrRotateX(MatrDefault(), Unit->RotAngleX));
  DefMatr = MatrMultMatr(DefMatr, MatrRotateZ(MatrDefault(), Unit->RotAngleZ));
  /*
  DefMatr = MatrRotateY(MatrDefault(), Unit->RotAngleY);
  DefMatr = MatrRotateX(DefMatr, Unit->RotAngleX);
  DefMatr = MatrRotateZ(DefMatr, Unit->RotAngleZ + Unit->Phys.RotSpeed.Z);
  */
  Unit->VecPos = VecSumVec(Unit->VecPos, VecMultMatr(Unit->Phys.Speed, DefMatr));

  UnitBoatCameraSet(Unit, Ani);
  Ani->RndCamera = Unit->Camera;
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
  UINT loc;
  Ani->RndCamera = Unit->Camera;
  Ani->RndMatrWorld = MatrScale(0.7, 0.7, 0.7);
  
  Ani->RndMatrWorld = MatrMultMatr(Ani->RndMatrWorld, MatrRotateY(MatrDefault(), Unit->RotAngleY));
  Ani->RndMatrWorld = MatrMultMatr(Ani->RndMatrWorld, MatrRotateX(MatrDefault(), Unit->RotAngleX));
  Ani->RndMatrWorld = MatrMultMatr(Ani->RndMatrWorld, MatrRotateZ(MatrDefault(), Unit->RotAngleZ + Unit->Phys.RotSpeed.Z));
  Ani->RndMatrWorld = MatrMultMatr(Ani->RndMatrWorld, MatrTranslate(MatrDefault(), Unit->VecPos.X, Unit->VecPos.Y + Unit->LevelDif, Unit->VecPos.Z));

  RK2_RndCameraUpdateInfo(&Unit->Camera);
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
      glUniformMatrix4fv(loc, 1, FALSE, &Unit->VecPos.X);

    loc = glGetUniformLocation(Unit->ShaderProg, "Trans");
    if (loc != -1)
      glUniform1f(loc, 1.0);
  }

  RK2_GObjDraw(&Unit->GObj);

  RK2_RndCameraNormalize(&Unit->Camera);
  glBegin(GL_LINES);
    glColor3d(1, 0, 0);
    glVertex3d(Unit->VecPos.X, Unit->VecPos.Y, Unit->VecPos.Z);
    glVertex3d(Unit->VecPos.X + Unit->Camera.Up.X, 
               Unit->VecPos.Y + Unit->Camera.Up.Y, 
               Unit->VecPos.Z + Unit->Camera.Up.Z);

    glColor3d(0, 1, 0);
    glVertex3d(Unit->VecPos.X, Unit->VecPos.Y, Unit->VecPos.Z);
    glVertex3d(Unit->VecPos.X + Unit->Camera.Right.X, 
               Unit->VecPos.Y + Unit->Camera.Right.Y, 
               Unit->VecPos.Z + Unit->Camera.Right.Z);

    glColor3d(0, 0, 1);
    glVertex3d(Unit->VecPos.X, Unit->VecPos.Y, Unit->VecPos.Z);
    glVertex3d(Unit->VecPos.X + Unit->Camera.Dir.X, 
               Unit->VecPos.Y + Unit->Camera.Dir.Y, 
               Unit->VecPos.Z + Unit->Camera.Dir.Z);
  glEnd();
  glUseProgram(0);
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

  Unit->VecPos = VecSet(PosX, PosY, PosZ);

  Unit->RotAngleX = RotAngleX;
  Unit->RotAngleY = RotAngleY;
  Unit->RotAngleZ = RotAngleZ;

  return (rk2UNIT *)Unit;
} /* End of 'RK2_UnitBoatCreate' function */

/* END OF 'SAMPBOAT.C' FILE */
