/* FILENAME: VEC.C
 * PROGRAMMER: RK2
 * PURPOSE: Matrix module
 * LAST UPDATE: 10.06.2014
 */

#include <math.h>

#include "def.h"
#include "mth.h"

/* Vector count length function.
 * ARGUMENTS: 
 *   - Vector:
 *     rk2VEC *Vec
 * RETURNS: 
 *   (DBL) Length of vector.
 */
DBL RK2_VecLen( rk2VEC *Vec )
{
  return sqrt(Vec->X * Vec->X + Vec->Y * Vec->Y + Vec->Z * Vec->Z);
} /* End of 'RK2_VecLen' funciton */
                
/* Sum vectors counting function.
 * ARGUMENTS: 
 *   - Vector:
 *     rk2VEC *SrcVec0, *SrcVec1;
 * RETURNS: 
 *   (rk2VEC) Result vector.
 */
rk2VEC RK2_VecSumVec( rk2VEC SrcVec0, rk2VEC SrcVec1)
{
  rk2VEC VecRet;

  VecRet.X = SrcVec0.X + SrcVec1.X;
  VecRet.X = SrcVec0.X + SrcVec1.X;
  VecRet.X = SrcVec0.X + SrcVec1.X;

  return VecRet;
} /* End of 'RK2_VecSumVec' function */

/* END OF 'VEC.C' FILE */
