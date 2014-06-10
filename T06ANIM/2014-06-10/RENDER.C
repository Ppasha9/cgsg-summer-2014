/* FILENAME: RENDER.C
 * PROGRAMMER: VG4
 * PURPOSE: 3D render handle module.
 * LAST UPDATE: 10.06.2014
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "render.h"

/* Глобальная камера */
vg4CAMERA VG4_RndCam;

/* Параметры проецирования */
DBL
  VG4_RndWs = 30, VG4_RndHs = 30,   /* размеры кадра в точках */
  VG4_RndWp = 4, VG4_RndHp = 3,     /* размеры обрасти проецирования */
  VG4_RndProjDist = 5;              /* расстояние до плоскости проекции */

/* Матрицы */
MATR
  VG4_RndMatrWorld = VG4_UNIT_MATR, /* матрица преобразования мировой СК */
  VG4_RndMatrView = VG4_UNIT_MATR;  /* матрица преобразования видовой СК */

/* Функция преобразования из мировой системы коорлинат в кадр.
 * АРГУМЕНТЫ:
 *   - исходная точка:
 *       VEC P;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (POINT) координаты в кадре.
 */
POINT VG4_RndWorldToScreen( VEC P )
{
  POINT Ps;
  VEC Pp;

  /* преобразование СК */
  Pp = VecMulMatr(P, MatrMulMatr(VG4_RndMatrWorld, VG4_RndMatrView));
  
  /* проецирование */
  Pp.X *= VG4_RndProjDist / Pp.Z;
  Pp.Y *= VG4_RndProjDist / Pp.Z;

  Ps.x = ( Pp.X + VG4_RndWp / 2) / VG4_RndWp * (VG4_RndWs - 1);
  Ps.y = (-Pp.Y + VG4_RndHp / 2) / VG4_RndHp * (VG4_RndHs - 1);
  return Ps;
} /* End of 'VG4_RndWorldToScreen' function */

/* Функция загрузки геометрического объекта.
 * АРГУМЕНТЫ:
 *   - указатель на структуру для загружаемой геометрии:
 *       vg4GOBJ *GObj;
 *   - имя файла (*.OBJ):
 *       CHAR *FileName;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (BOOL) TRUE при успехе.
 */
BOOL VG4_RndGObjLoad( vg4GOBJ *GObj, CHAR *FileName )
{
  INT nv = 0, nf = 0;
  FILE *F;
  static CHAR Buf[1000];

  /* обнулили данные */
  memset(GObj, 0, sizeof(vg4GOBJ));
  if ((F = fopen(FileName, "r")) == NULL)
    return 0;

  /* считаем количество вершин и граней */
  while (fgets(Buf, sizeof(Buf), F) != NULL)
  {
    if (Buf[0] == 'v' && Buf[1] == ' ')
      nv++;
    else if (Buf[0] == 'f' && Buf[1] == ' ')
      nf++;
  }

  /* выделяем память под вершины и грани как единый участок памяти
   * (memory bulk) */
  GObj->V = malloc(nv * sizeof(VEC) + nf * sizeof(INT [3]));
  GObj->F = (INT (*)[3])(GObj->V + nv);
  GObj->NumOfV = nv;
  GObj->NumOfF = nf;

  /* считываем данные */
  nv = 0;
  nf = 0;
  rewind(F);
  while (fgets(Buf, sizeof(Buf), F) != NULL)
  {
    DBL x, y, z;
    INT a, b, c;

    if (Buf[0] == 'v' && Buf[1] == ' ')
    {
      sscanf(Buf + 2, "%lf%lf%lf", &x, &y, &z);
      GObj->V[nv++] = VecSet(x, y, z);
    }
    else if (Buf[0] == 'f' && Buf[1] == ' ')
    {
      sscanf(Buf + 2, "%i%i%i", &a, &b, &c);
      GObj->F[nf][0] = a - 1;
      GObj->F[nf][1] = b - 1;
      GObj->F[nf][2] = c - 1;
      nf++;
    }
  }
  fclose(F);
  return TRUE;
} /* End of 'VG4_RndGObjLoad' function */

/* Функция освобождения геометрического объекта.
 * АРГУМЕНТЫ:
 *   - указатель на структуру для загружаемой геометрии:
 *       vg4GOBJ *GObj;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID VG4_RndGObjFree( vg4GOBJ *GObj )
{
  if (GObj->V != NULL)
    free(GObj->V);
  memset(GObj, 0, sizeof(vg4GOBJ));
} /* End of 'VG4_RndGObjFree' function */

/* Функция отрисовки геометрического объекта.
 * АРГУМЕНТЫ:
 *   - указатель на структуру для загружаемой геометрии:
 *       vg4GOBJ *GObj;
 *   - контекст устройства вывода:
 *       HDC hDC;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID VG4_RndGObjDraw( vg4GOBJ *GObj, HDC hDC )
{
  INT i, j, s = 1;
  POINT pt[3];

  for (i = 0; i < GObj->NumOfV; i++)
  {
    pt[0] = VG4_RndWorldToScreen(GObj->V[i]);
    Ellipse(hDC, pt[0].x - s, pt[0].y - s, pt[0].x + s, pt[0].y + s);
  }
  for (i = 0; i < GObj->NumOfF; i++)
  {
    for (j = 0; j < 3; j++)
      pt[j] = VG4_RndWorldToScreen(GObj->V[GObj->F[i][j]]);
    MoveToEx(hDC, pt[0].x, pt[0].y, NULL);
    LineTo(hDC, pt[1].x, pt[1].y);
    LineTo(hDC, pt[2].x, pt[2].y);
    LineTo(hDC, pt[0].x, pt[0].y);
  }
} /* End of 'VG4_RndGObjDraw' function */

/* END OF 'RENDER.C' FILE */
