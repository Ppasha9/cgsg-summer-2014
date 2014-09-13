/* FILENAME: ANIM.H
 * PROGRAMMER: RK2
 * PURPOSE: Globe project declaration module.
 * LAST UPDATE: 08.06.2014
 */

#ifndef __ANIM_H_
#define __ANIM_H_

#include "image.h"

/* Clock rendering function. */
VOID RK2_ClockRender( HDC hDC, INT W, INT H );


/* Switching to/from fullscreen function. */
VOID RK2_AnimFlipFullScreen( HWND hWnd );

#endif /* __ANIM_H_ */

/* END OF 'ANIM.H' FILE */
