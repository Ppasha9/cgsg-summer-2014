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
                
/* END OF 'VEC.C' FILE */
