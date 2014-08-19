/* FILENAME: SAMPGRASS.C
 * PROGRAMMER: RK2
 * PURPOSE: Animation unit handle module.
 * LAST UPDATE: 14.08.2014
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "../image/image.h"

#include "../anim/anim.h"
#include "../gobjects/gobj.h"

/* Unit heightmap struct definition */
typedef struct tagrk2UNIT_HEIGHTMAP
{
  RK2_UNIT_BASE_FIELDS;                /* Base fields */
  rk2GOBJ GObj;                        /* Geometry Object */
  CHAR 
    FileTextureName[MAX_PATH],      /* Texture file name */
    FileMapName[MAX_PATH];          /* Map File name */

  INT TreesCnt;
  rk2VEC TreesPos[RK2_MAX_UNITS];  /* Trees positions */
  INT TreesObjCnt;                 /* Count of models trees */
  rk2GOBJ TrObjs[RK2_MAX_UNITS];   /* Tree models */

  UINT ShaderProg;                 /* Program shader */
} rk2UNIT_HEIGHTMAP;

/* Unit heightmap init function.
 * ARGUMENTS:
 *   - Self pointer:
 *       rk2UNIT_HEIGHTMAP *Unit;
 *   - Animation context pointer:
 *       rk2ANIM *Ani;
 * RETURNS: None.
 */
static VOID UnitHeightMapInit( rk2UNIT_HEIGHTMAP *Unit, rk2ANIM *Ani )
{
  rk2GPRIM GPrim;
  rk2GMATERIAL GMtl;
  strcpy(GMtl.Name, Unit->FileTextureName);
  strcpy(GMtl.MapD, Unit->FileTextureName);
  GMtl.TexNo = 0;
  GMtl.Ka = VecSet(0.3, 0.3, 0.3);
  GMtl.Kd = VecSet(0.3, 0.3, 0.3);
  GMtl.Ks = VecSet(0.3, 0.3, 0.3);
  GMtl.Phong = 10;
  GMtl.Trans = 1;
  RK2_GPrimCreateHeightField(&GPrim, Unit->FileMapName, 80, 2, Unit->FileTextureName);
  GPrim.Mtl = RK2_GObjAddMaterial(&Unit->GObj, &GMtl);
  RK2_GObjAddPrim(&Unit->GObj, &GPrim);
  if ((Unit->ShaderProg = RK2_ShadProgInit("shaders\\default.vert", "shaders\\grass.frag")) == 0)
    Unit->ShaderProg = Ani->ShaderDef;

} /* End of 'UnitHeightMapInit' function */

/* Unit heightmap destructor function.
* ARGUMENTS:
*   - Self pointer:
*       rk2UNIT_HEIGHTMAP *Unit;
*   - Animation context pointer:
*       rk2ANIM *Ani;
* RETURNS: None.
*/
static VOID UnitHeightMapClose(rk2UNIT_HEIGHTMAP *Unit, rk2ANIM *Ani)
{
  RK2_GObjFree(&Unit->GObj);
  if (Unit->ShaderProg != Ani->ShaderDef)
    RK2_ShadProgClose(Unit->ShaderProg);
} /* End of 'UnitHeightMapClose' function */

/* Unit heightmap response function.
 * ARGUMENTS:
 *   - Self pointer:
 *       rk2UNIT *Unit;
 *   - Animation context pointer:
 *       rk2ANIM *Ani;
 * RETURNS: None.
 */
static VOID UnitHeightMapResponse( rk2UNIT_HEIGHTMAP *Unit, rk2ANIM *Ani )
{
  if (Ani->Keys['M'] && !Ani->KeysClicked['M'])
  {
    RK2_ShadProgClose(Unit->ShaderProg);
    if ((Unit->ShaderProg = RK2_ShadProgInit("__shaders\\default.vert", "shaders\\grass.frag")) == 0)
      Unit->ShaderProg = Ani->ShaderDef;
  }
} /* End of 'UnitHeightMapResponse' function */

/* Unit heightmap render function.
 * ARGUMENTS:
 *   - Self pointer:
 *       rk2UNIT *Unit;
 *   - Animation context pointer:
 *       rk2ANIM *Ani;
 * RETURNS: None.
 */
static VOID UnitHeightMapRender( rk2UNIT_HEIGHTMAP *Unit, rk2ANIM *Ani )
{
  UINT loc;
  RK2_RndBuildMatrix();

  glUseProgram(Unit->ShaderProg);
  RK2_RndSendGlobInfo(Unit->ShaderProg, Ani);

  RK2_GObjDraw(Unit->ShaderProg, Ani, &Unit->GObj);
  glUseProgram(Ani->ShaderDef);
} /* End of 'UnitHeightMapRender' function */

/* Unit heightmap create function.
 * ARGUMENTS:
 *   - File names:
 *     CHAR *FileMapName, *FileTextName
 * RETURNS:
 *   (rk2UNIT_HEIGHTMAP *) - pointer for new animation unit.
 */
rk2UNIT *RK2_UnitHeightMapCreate( CHAR *FileMapName, CHAR *FileTextName )
{
  rk2UNIT_HEIGHTMAP *Unit;

  if ((Unit = (rk2UNIT_HEIGHTMAP *)RK2_AnimUnitCreate(sizeof(rk2UNIT_HEIGHTMAP))) == NULL)
    return NULL;

  /* Base fields filling */
  Unit->Render = (VOID *)UnitHeightMapRender;
  Unit->Init = (VOID *)UnitHeightMapInit;
  Unit->Close = (VOID *)UnitHeightMapClose;
  Unit->Response = (VOID *)UnitHeightMapResponse;

  strcpy(Unit->FileMapName,FileMapName);
  if (FileTextName != NULL)
    strcpy(Unit->FileTextureName, FileTextName);
  else
    strcpy(Unit->FileTextureName, Unit->FileMapName);

  return (rk2UNIT *)Unit;
} /* End of 'RK2_UnitGrassCreate' function */

/* END OF 'SAMPGRASS.C' FILE */
