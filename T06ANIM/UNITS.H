/* FILENAME: UNITS.H
 * PROGRAMMER: RK2
 * PURPOSE: Globe project declaration module.
 * LAST UPDATE: 09.06.2014
 */

#ifndef __UNITS_H_
#define __UNITS_H_

#include "anim.h"

/* Unit Animation control create function. */
rk2UNIT *RK2_UnitAnimContolCreate(VOID);

/* Anim Info Unit create function. */
rk2UNIT *RK2_InfoUnitCreate( VOID );

/* Cow Unit create function. */
rk2UNIT *RK2_CowUnitCreate(VOID);

/* Creating unit clock function. */
rk2UNIT *RK2_UnitClockCreate( VOID );

/* Creating unit pict animation function. */
rk2UNIT *RK2_UnitPictCreate( CHAR *FileName, INT PosX, INT PosY, DWORD DrawingType );

/* Creating unit pict animation function. */
rk2UNIT *RK2_UnitLogoCreate( INT PosX, INT PosY );

#endif /* __UNITS_H_ */

/* END OF 'UNITS.H' FILE */
