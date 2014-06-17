/* FILENAME: UNITS.H
 * PROGRAMMER: RK2
 * PURPOSE: Globe project declaration module.
 * LAST UPDATE: 17.06.2014
 */

#ifndef __UNITS_H_
#define __UNITS_H_

#include "anim.h"

/* Unit Animation control create function. */
rk2UNIT *RK2_UnitAnimContolCreate( VOID );

/* Unit Animation control create function. */
rk2UNIT *RK2_UnitAnimInfoCreate( VOID );

/* Unit GObject create function. */
rk2UNIT *RK2_UnitGObjCreate( CHAR *FileName, 
                             INT PosX, INT PosY, INT PosZ, 
                             DBL RotAngleX, DBL RotAngleY, DBL RotAngleZ,
                             CHAR *ShaderVFileName, CHAR *ShaderFFileName );

/* Unit Coord system create function. */
rk2UNIT *RK2_UnitCorSysCreate( VOID );

/* Unit flying camera create function. */
rk2UNIT *RK2_UnitCamFlyCreate( VOID );

/* Unit heightmap create function. */
rk2UNIT *RK2_UnitHeightMapCreate( VOID );

/* Unit heightmap create function. */
rk2UNIT *RK2_UnitAreaCreate( CHAR *FileTextureName, rk2VEC Pos0, rk2VEC Pos1, rk2VEC Pos2, rk2VEC Pos3 );


#endif /* __UNITS_H_ */

/* END OF 'UNITS.H' FILE */
