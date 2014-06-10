/* FILENAME: RENDER.H
 * PROGRAMMER: VG4
 * PURPOSE: 3D render declaration module.
 * LAST UPDATE: 10.06.2014
 */

#ifndef __RENDER_H_
#define __RENDER_H_

#include "vec.h"

/* Тип представления камеры в пространстве */
typedef struct
{
  VEC
    Loc,             /* точка расположения */
    Dir, Up, Right,  /* ориентационные вектора */
    At;              /* точка наблюдения */
} vg4CAMERA;

/* Тип представления объекта - набора вершин и треугольников и ребер */
typedef struct tagvg4GOBJ
{
  VEC *V;      /* массив вершин объекта */
  INT NumOfV;  /* количество вершин */
  INT (*F)[3]; /* массив граней объекта */
  INT NumOfF;  /* количество граней */
} vg4GOBJ;

/* Глобальная камера */
extern vg4CAMERA VG4_RndCam;

/* Параметры проецирования */
extern DBL
  VG4_RndWs, VG4_RndHs, /* размеры кадра в точках */
  VG4_RndWp, VG4_RndHp, /* размеры обрасти проецирования */
  VG4_RndProjDist;      /* расстояние до плоскости проекции */

/* Матрицы */
extern MATR
  VG4_RndMatrWorld, /* матрица преобразования мировой СК */
  VG4_RndMatrView;  /* матрица преобразования видовой СК */

/* Функция преобразования из мировой системы коорлинат в кадр */
POINT VG4_RndWorldToScreen( VEC P );

/* Функция загрузки геометрического объекта */
BOOL VG4_RndGObjLoad( vg4GOBJ *GObj, CHAR *FileName );

/* Функция освобождения геометрического объекта */
VOID VG4_RndGObjFree( vg4GOBJ *GObj );

/* Функция отрисовки геометрического объекта */
VOID VG4_RndGObjDraw( vg4GOBJ *GObj, HDC hDC );

#endif /* __RENDER_H_ */

/* END OF 'RENDER.H' FILE */
