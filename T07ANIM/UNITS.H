/* FILENAME: UNITS.H
 * PROGRAMMER: RK2
 * PURPOSE: Globe project declaration module.
 * LAST UPDATE: 13.06.2014
 */

#ifndef __UNITS_H_
#define __UNITS_H_

#include "anim.h"

/* Unit Animation control create function. */
rk2UNIT *RK2_UnitAnimContolCreate( VOID );

/* Unit Animation control create function. */
rk2UNIT *RK2_UnitAnimInfoCreate( VOID );

/* Unit GObject create function. */
rk2UNIT *RK2_UnitGObjCreate( CHAR *FileName, INT PosX, INT PosY, INT PosZ, DBL RotAngleX, DBL RotAngleY, DBL RotAngleZ );

/* Unit Coord system create function. */
rk2UNIT *RK2_UnitCorSysCreate( VOID );

/* Unit flying camera create function. */
rk2UNIT *RK2_UnitCamFlyCreate( VOID );

#endif /* __UNITS_H_ */

/* END OF 'UNITS.H' FILE */
