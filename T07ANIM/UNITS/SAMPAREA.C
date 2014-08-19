/* FILENAME: SAMPAREA.C
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

/* Unit heightmap struct definition */
typedef struct tagrk2UNIT_AREA
{
  RK2_UNIT_BASE_FIELDS;                /* Base fields */
  rk2GOBJ GObj;                        /* Geometry Object */
  CHAR FileTextureName[MAX_PATH];      /* Texture file name */
  rk2VEC 
    VecPos0, VecPos1,                  /* Coords of area */
    VecPos2, VecPos3;
} rk2UNIT_AREA;

/* Unit grass init function.
 * ARGUMENTS:
 *   - Self pointer:
 *       rk2UNIT_AREA *Unit;
 *   - Animation context pointer:
 *       rk2ANIM *Ani;
 * RETURNS: None.
 */
static VOID UnitAreaInit( rk2UNIT_AREA *Unit, rk2ANIM *Ani )
{
  rk2GPRIM GPrim;
  rk2GMATERIAL GMtl;
  strcpy(GMtl.Name, Unit->FileTextureName);
  strcpy(GMtl.MapD, Unit->FileTextureName);
  GMtl.TexNo = 0;
  GMtl.Ka = VecSet(0.3, 0.3, 0.3);
  GMtl.Kd = VecSet(0.3, 0.3, 0.3);
  GMtl.Ks = VecSet(0.3, 0.3, 0.3);
  GMtl.Phong = 50;
  GMtl.Trans = 0.5;
  Ani->WaterLevel = 10;
  /// GMtl.Trans = sin(Ani->Time);
  RK2_GPrimCreateDesk(&GPrim, VecSet(0, 0, 0), VecSet(30, 0, 0), VecSet(0, 0, 30), VecSet(30, 0, 30), NULL);
  GPrim.Mtl = RK2_GObjAddMaterial(&Unit->GObj, &GMtl);
  RK2_GObjAddPrim(&Unit->GObj, &GPrim);
} /* End of 'UnitAreaInit' function */

/* Unit heightmap destructor function.
* ARGUMENTS:
*   - Self pointer:
*       rk2UNIT_AREA *Unit;
*   - Animation context pointer:
*       rk2ANIM *Ani;
* RETURNS: None.
*/
static VOID UnitAreaClose(rk2UNIT_AREA *Unit, rk2ANIM *Ani)
{
  RK2_GObjFree(&Unit->GObj);
} /* End of 'UnitAreaClose' function */

/* Unit heightmap response function.
 * ARGUMENTS:
 *   - Self pointer:
 *       rk2UNIT *Unit;
 *   - Animation context pointer:
 *       rk2ANIM *Ani;
 * RETURNS: None.
 */
static VOID UnitAreaResponse( rk2UNIT_AREA *Unit, rk2ANIM *Ani )
{
} /* End of 'UnitAreaResponse' function */

/* Unit heightmap render function.
 * ARGUMENTS:
 *   - Self pointer:
 *       rk2UNIT *Unit;
 *   - Animation context pointer:
 *       rk2ANIM *Ani;
 * RETURNS: None.
 */
static VOID UnitAreaRender( rk2UNIT_AREA *Unit, rk2ANIM *Ani )
{
  UINT loc;
  INT x, z;
  Ani->RndMatrWorld = MatrDefault();
  RK2_RndBuildMatrix();

  glUseProgram(Ani->ShaderDef);
  for (x = -20; x < 20; x++)
    for (z = -20; z < 20; z++)
    {
      Ani->RndMatrWorld = MatrTranslate(MatrDefault(), x * 30, Ani->WaterLevel, z * 30);
      RK2_RndBuildMatrix();

      RK2_RndSendGlobInfo(Ani->ShaderDef, Ani);
      RK2_GObjDraw(Ani->ShaderDef, Ani, &Unit->GObj);
    }
} /* End of 'UnitAreaRender' function */

/* Unit heightmap create function.
 * ARGUMENTS:
 *   - texture file name:
 *       CHAR *FukeTextureName;
 *   - Coords of texture:
 *       rk2VEC Pos0, Pos1, Pos2, Pos3;
 * RETURNS:
 *   (rk2UNIT_AREA *) - pointer for new animation unit.
 */
rk2UNIT *RK2_UnitAreaCreate( CHAR *FileTextureName, rk2VEC Pos0, rk2VEC Pos1, rk2VEC Pos2, rk2VEC Pos3 )
{
  rk2UNIT_AREA *Unit;

  if ((Unit = (rk2UNIT_AREA *)RK2_AnimUnitCreate(sizeof(rk2UNIT_AREA))) == NULL)
    return NULL;

  /* Base fields filling */
  Unit->Render = (VOID *)UnitAreaRender;
  Unit->Init = (VOID *)UnitAreaInit;
  Unit->Close = (VOID *)UnitAreaClose;
  Unit->Response = (VOID *)UnitAreaResponse;
  strcpy(Unit->FileTextureName, FileTextureName);

  return (rk2UNIT *)Unit;
} /* End of 'RK2_UnitAreaCreate' function */

/* END OF 'SAMPAREA.C' FILE */
