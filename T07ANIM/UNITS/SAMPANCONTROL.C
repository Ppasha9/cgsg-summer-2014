/* FILENAME: SAMPANINFO.C
 * PROGRAMMER: RK2
 * PURPOSE: Animation unit handle module.
 * LAST UPDATE: 12.06.2014
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "../anim/anim.h"

/* Unit Animation control response function.
 * ARGUMENTS:
 *   - Self pointer:
 *       rk2UNIT *Unit;
 *   - Animation context pointer:
 *       rk2ANIM *Ani;
 * RETURNS: None.
 */
static VOID UnitAnimContolResponse(rk2UNIT *Unit, rk2ANIM *Ani)
{
  if (!Ani->KeysClicked['F'] && Ani->Keys['F'])
    RK2_AnimFlipFullScreen();
  if (!Ani->KeysClicked['P'] && Ani->Keys['P'])
    RK2_AnimSetPause();
  if (Ani->IsPause)
  {
    if (Ani->Keys['O'])
      RK2_RndCameraMoveDir(&Ani->RndCamera, 1.5);
    if (Ani->Keys['L'])
      RK2_RndCameraMoveDir(&Ani->RndCamera, -1.5);

    if (Ani->JoyY)
      RK2_RndCameraMoveDir(&Ani->RndCamera, -Ani->JoyY);

    if (Ani->Keys['A'])
      RK2_RndCameraMoveRight(&Ani->RndCamera, -0.5);
    if (Ani->Keys['D'])
      RK2_RndCameraMoveRight(&Ani->RndCamera, 0.5);
  
    if (Ani->JoyX)
      RK2_RndCameraMoveRight(&Ani->RndCamera, Ani->JoyX);

    if (Ani->Keys['W'])
      RK2_RndCameraMoveUp(&Ani->RndCamera, 0.3);
    if (Ani->Keys['S'])
     RK2_RndCameraMoveUp(&Ani->RndCamera, -0.3);

    if (Ani->JoyR)
      RK2_RndCameraRotateUp(&Ani->RndCamera, -Ani->JoyR);

    if (Ani->Keys['Q'])
      RK2_RndCameraRotateUp(&Ani->RndCamera, 1);
    if (Ani->Keys['E'])
     RK2_RndCameraRotateUp(&Ani->RndCamera, -1);

    if (Ani->JoyZ)
     RK2_RndCameraRotateRight(&Ani->RndCamera, -Ani->JoyZ);
    RK2_RndCameraRotateUp(&Ani->RndCamera, (DBL)Ani->MouseWheel);

    if (Ani->Keys['I'])
      RK2_RndCameraRotateRight(&Ani->RndCamera, 1);
    if (Ani->Keys['K'])
      RK2_RndCameraRotateRight(&Ani->RndCamera, -1);
  }
  if (Ani->Keys['R'])
  {
    RK2_ShadProgClose(Ani->ShaderDef);
    Ani->ShaderDef = RK2_ShadProgInit("shaders\\default.vert", "shaders\\default.frag");
  }

  if (Ani->Keys[VK_ESCAPE])
    DestroyWindow(Ani->hWnd);

} /* End of 'RK2_AnimUnitRender' function */

/* Unit Animation control create function.
 * ARGUMENTS: None.
 * RETURNS:
 *   (rk2UNIT *) - pointer for new animation unit.
 */
rk2UNIT *RK2_UnitAnimContolCreate( VOID )
{
  rk2UNIT *Unit;

  if ((Unit = RK2_AnimUnitCreate(sizeof(rk2UNIT))) == NULL)
    return NULL;

  /* Base fields filling */
  Unit->Response = (VOID *)UnitAnimContolResponse;

  return Unit;
} /* End of 'RK2_UnitAnimContolCreate' function */

/* END OF 'SAMPANINFO.C' FILE */
