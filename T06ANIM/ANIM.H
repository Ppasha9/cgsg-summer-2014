/* FILENAME: ANIM.H
 * PROGRAMMER: RK2
 * PURPOSE: Globe project declaration module.
 * LAST UPDATE: 08.06.2014
 */

#ifndef __ANIM_H_
#define __ANIM_H_

#include "image.h"

/* Max of animation units */
#define RK2_MAX_UNITS 3000

/* definition unit base struct */
typedef struct tagrk2UNIT rk2UNIT;

/* animation context declaration */
typedef struct tagrk2ANIM
{
  HWND hWnd;          /* Work window */
  INT W, H;           /* Window size */
  HBITMAP hBmFrame;   /* Frame buffer */
  HDC hDC;            /* Output context */

  /* Array of units and number of elements */
  rk2UNIT *Units[RK2_MAX_UNITS]; 
  INT NumOfUnits;

  /* Syncronisation system */
  DBL
    Time,            /* time from beginning animation */
    GlobalTime,      /* time from beginning animation without pause */
    DeltaTime,       /* time between frames in seconds */
    GlobalDeltaTime, /* time between frames in seconds without pause */
    FPS;             /* Frames per second */
  BOOL
    IsPause;         /* Pause flag */

  /* Input system */
  BYTE Keys[256];     /* Keyboard and mouse state */
} rk2ANIM;

/* extern rk2ANIM RK2_Anim; *//* Global animation context */

/* Base fields for animation unit struct:
 * - Size of struct:
 *     INT Size;
 * - Pointer for init unit function:
 *     VOID (*Init)( rk2UNIT *Unit, rk2ANIM *Ani );
 * - Pointer for destructor unit function:
 *     VOID (*Close)( rk2UNIT *Unit, rk2ANIM *Ani );
 * - Pointer for response unit function:
 *     VOID (*Response)( rk2UNIT *Unit, rk2ANIM *Ani );
 * -Pointer for rendering unit function:
 *     VOID (*Render)( rk2UNIT *Unit, rk2ANIM *Ani );
 */
#define RK2_UNIT_BASE_FIELDS \
  INT Size;                                          \
  VOID (*Init)( rk2UNIT *Unit, rk2ANIM *Ani );       \
  VOID (*Close)( rk2UNIT *Unit, rk2ANIM *Ani );      \
  VOID (*Response)( rk2UNIT *Unit, rk2ANIM *Ani );   \
  VOID (*Render)( rk2UNIT *Unit, rk2ANIM *Ani )

/* Base type of animation unit */
struct tagrk2UNIT
{
  RK2_UNIT_BASE_FIELDS; /* Base fields */
};

/***
 * Animation controll functions
 ***/

/* Animation init function */
VOID RK2_AnimInit(HWND hWnd);

/* Animation destructot function */
VOID RK2_AnimClose(VOID);

/* Resize window animation function */
VOID RK2_AnimResize(INT W, INT H);

/* Anmation rendering function */
VOID RK2_AnimRender(VOID);

/* Drawing to frame animation function */
VOID RK2_AnimCopyFrame(VOID);

/* Adding object to animation list function */
VOID RK2_AnimAddUnit(rk2UNIT *Unit);

/* Switching to/from fullscreen function */
VOID RK2_AnimFlipFullScreen(VOID);

/* Setting animation pause function */
VOID RK2_AnimSetPause(BOOL NewPauseFlag);
/***
 * Working with animation units functions
 ***/

/* Функция добавления в систему объекта анимации */
VOID RK2_AnimAddUnit( rk2UNIT *Unit );

/* Функция создания объекта анимации */
rk2UNIT *RK2_AnimUnitCreate( INT Size );

#endif /* __ANIM_H_ */

/* END OF 'ANIM.H' FILE */
