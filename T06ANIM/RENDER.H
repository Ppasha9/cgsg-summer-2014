/* FILENAME: RENDER.H
 * PROGRAMMER: RK2
 * PURPOSE: 3D render declaration module.
 * LAST UPDATE: 12.06.2014
 */

#ifndef __RENDER_H_
#define __RENDER_H_

#include "image.h"
#include "mth.h"

/* Camera properties */
typedef struct
{
  rk2VEC
    Loc,             /* Location */
    Dir, Up, Right,  /* Orient vectors */
    At;              /* Point to view */
} rk2CAMERA;

/* Тип представления объекта - набора вершин и треугольников и ребер */

/* Глобальная камера */
extern rk2CAMERA RK2_RndCamera;

/* Projection parametres */
extern DBL
  RK2_RndWs, RK2_RndHs, /* Frame sizes */
  RK2_RndWp, RK2_RndHp, /* Projection sizes */
  RK2_RndProjDist;      /* Distance to drawing screen */

/* Matrixes */
extern rk2MATR4x4
  RK2_RndMatrWorld, /* Matrix of convertion World CS */
  RK2_RndMatrView;  /* Matrix of view World CS */

/* Convertion from World system to screen function */
rk2VEC RK2_RndWorldToScreen( rk2VEC P );

/* Render setting function. */
VOID RK2_RndSetRenderProp( INT W, INT H );

/* Setting cameras properties */
VOID RK2_RndCameraSet( rk2VEC Loc, rk2VEC At, rk2VEC Up );

/* Setting cameras properties */
VOID RK2_RndCameraMoveDir( DBL Coef );

/* Camera move by Right-Axes function. */
VOID RK2_RndCameraMoveRight( DBL Coef );

/* Camera move by Up-Axes function. */
VOID RK2_RndCameraMoveUp( DBL Coef );

/* Camera rotation by axes Y function. */
VOID RK2_RndCameraRotateUp( DBL Angle );

/* Camera rotation by camera-axes-right function. */
VOID RK2_RndCameraRotateRight(DBL Angle);

/* Camera rotation by camera-axes-dir function. */
VOID RK2_RndCameraRotateDir(DBL Angle);


/* Render setting function. */
VOID RK2_RndBuildMatrix( VOID );

#endif /* __RENDER_H_ */

/* END OF 'RENDER.H' FILE */
