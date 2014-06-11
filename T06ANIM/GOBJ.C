/* FILENAME: GOBG.C
 * PROGRAMMER: RK2
 * PURPOSE: Geometry *.object* files module
 * LAST UPDATE: 10.06.2014
 */

#include <stdio.h>
#include <stdlib.h>

#include "image.h"
#include "render.h"

/* Geometry object function Load.
 * ARGUMENTS:
 *   - óPointer for new struct:
 *       rk2GOBJ *GObj;
 *   - File Name:
 *       CHAR *FileName;
 * RETURNS:
 *   (BOOL) TRUE if succcesful.
 */
BOOL RK2_GObjLoad( rk2GOBJ *GObj, CHAR *FileName )
{
  INT nv = 0, nf = 0;
  FILE *F;
  static CHAR Buf[1000];

  /* clearing data */
  memset(GObj, 0, sizeof(rk2GOBJ));
  if ((F = fopen(FileName, "r")) == NULL)
    return 0;

  /* counting elements */
  while (fgets(Buf, sizeof(Buf), F) != NULL)
  {
    if (Buf[0] == 'v' && Buf[1] == ' ')
      nv++;
    else if (Buf[0] == 'f' && Buf[1] == ' ')
      nf++;
  }

  /* malloc memory */
  GObj->V = malloc(nv * sizeof(rk2VEC) + nf * sizeof(INT [3]));
  GObj->F = (INT (*)[3])(GObj->V + nv);
  GObj->NumOfV = nv;
  GObj->NumOfF = nf;

  /* Data writing */
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
} /* End of 'RK2_RndGObjLoad' function */

/* Free Geometric object unit function.
 * ARGUMENTS:
 *   - Pointer for object:
 *       rk2GOBJ *GObj;
 * RETURNS: None.
 */
VOID RK2_GObjFree( rk2GOBJ *GObj )
{
  if (GObj->V != NULL)
    free(GObj->V);
  memset(GObj, 0, sizeof(rk2GOBJ));
} /* End of 'RK2_GObjFree' function */


/* Geometric object drawing function.
 * ARGUMENTS:
 *   - Pointer for object:
 *       rk2GOBJ *GObj;
 *   - output ontext:
 *       HDC hDC;
 * RETURNS: None.
 */
VOID RK2_GObjDraw( rk2GOBJ *GObj, HDC hDC )
{
  INT i, j, s = 1;
  rk2VEC pt[3];

  for (i = 0; i < GObj->NumOfV; i++)
  {
    pt[0] = RK2_RndWorldToScreen(GObj->V[i]);
    if (pt[0].Z >= 0)
      Ellipse(hDC, pt[0].X - s, pt[0].Y - s, pt[0].X + s, pt[0].Y + s);
  }
  for (i = 0; i < GObj->NumOfF; i++)
  {
    for (j = 0; j < 3; j++)
      pt[j] = RK2_RndWorldToScreen(GObj->V[GObj->F[i][j]]);
    if (pt[0].Z >= 0)
      MoveToEx(hDC, pt[0].X, pt[0].Y, NULL);
    if (pt[1].Z >= 0)
      LineTo(hDC, pt[1].X, pt[1].Y);
    if (pt[2].Z >= 0)
      LineTo(hDC, pt[2].X, pt[2].Y);
    if (pt[0].Z >= 0)
      LineTo(hDC, pt[0].X, pt[0].Y);
  }
} /* End of 'RK2_GObjDraw' function */

/* END OF 'GOBG.C' FILE */
