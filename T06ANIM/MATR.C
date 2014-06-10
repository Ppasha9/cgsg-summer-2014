/* FILENAME: MATR.C
 * PROGRAMMER: RK2
 * PURPOSE: Matrix module
 * LAST UPDATE: 10.06.2014
 */

#include "def.h"
#include "mth.h"

/* Matr mult with matr function.
 * ARGUMENTS:
 *   - Dest matr:
 *       rk2MATR4x4 *Dest;
 *   - Source Matrs:
 *       rk2MATR4x4 *SrcA, *SrcB;
 *   - Matr sizes:
 *       INT SrcAw, SrcAh, SrcBw, SrcBh;
 * RETURNS:
 *   (VOID) None.
 */
VOID RK2_MatrMultMatr( DBL *Dest, DBL *SrcA, INT SrcAw, INT SrcAh, DBL *SrcB, INT SrcBw, INT SrcBh )
{
  INT j, i;
  for (j = 0; j < SrcAh; j++)
    for (i = 0; i < SrcBw; i++)
    {
      DBL Sum = 0;
      INT k;
      for (k = 0; k < SrcAw; k++)
        Sum += SrcA[j * SrcAw + k] * SrcB[k * SrcBw + i];
      Dest[j * SrcBw + i] = Sum;
    }
} /* End of 'RK2_MatrMultMatr' function */

/* Vec mult with matr function.
 * ARGUMENTS:
 *   - Vector:
 *       rk2VEC Vec;
 *   - Matr:
 *       rk2MATR4x4 *SrcMatr;
 * RETURNS:
 *   (rk2VEC) Result vector.
 */
rk2VEC RK2_VecMultMatr4x4( rk2VEC Vec, rk2MATR4x4 *SrcMatr )
{
  rk2MATR4x4 Tmp, Res;

  Tmp.Arr[0][0] = Vec.X;
  Tmp.Arr[0][1] = Vec.Y;
  Tmp.Arr[0][2] = Vec.Z;
  Tmp.Arr[0][3] = Vec.At;
  RK2_MatrMultMatr(&Res.Arr[0][0], &Tmp.Arr[0][0], 4, 1, &SrcMatr->Arr[0][0], 4, 4);
  Vec.X = Res.Arr[0][0];
  Vec.Y = Res.Arr[0][1];
  Vec.Z = Res.Arr[0][2];
  Vec.At = Res.Arr[0][3];

  return Vec;
} /* End of 'RK2_VecMultMatr4x4' function */

/* END OF 'MATR.C' FILE */
