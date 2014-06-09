/* FILENAME: SAMPCLOCK.C
 * PROGRAMMER: RK2
 * PURPOSE: Animation unit handle module.
 * LAST UPDATE: 09.06.2014
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "../anim.h"

/* Clock unit struct definition. */
typedef struct tagrk2UNIT_CLOCK
{
  RK2_UNIT_BASE_FIELDS; /* Include bas fields */
  INT PosX, PosY;       /* Clock coordinates */
  DBL Rotate;           /* Clock rotate angle */
} rk2UNIT_CLOCK;

/* Clock unit initialization function.
 * ARGUMENTS:
 *   - Self pointer:
 *       rk2UNIT_CLOCK *Unit;
 *   - Animation context pointer:
 *       rk2ANIM *Ani;
 * RETURNS: None.
 */
static VOID UnitClockInit( rk2UNIT_CLOCK *Unit, rk2ANIM *Ani )
{
  Unit->PosX = Ani->W / 2;
  Unit->PosY = Ani->H / 2;
} /* End of 'UnitClockInit' function */

/* Clock unit destructor function.
 * ARGUMENTS:
 *   - Self pointer:
 *       rk2UNIT_CLOCK *Unit;
 *   - Animation context pointer:
 *       rk2ANIM *Ani;
 * RETURNS: None.
 */
static VOID UnitClockClose( rk2UNIT_CLOCK *Unit, rk2ANIM *Ani )
{
} /* End of 'UnitClockClose' function */

/* Clock Unit changing parametres function.
 * ARGUMENTS:
 * ARGUMENTS:
 *   - Self pointer:
 *       rk2UNIT_CLOCK *Unit;
 *   - Animation context pointer:
 *       rk2ANIM *Ani;
 * RETURNS: None.
 */
static VOID UnitClockResponse( rk2UNIT_CLOCK *Unit, rk2ANIM *Ani )
{
  Unit->PosX += Ani->JoyR * 20;
  Unit->PosY += Ani->JoyZ * 20;
  Unit->Rotate -= Ani->JoyButs[6] * 0.1;
  Unit->Rotate += Ani->JoyButs[7] * 0.1;
} /* End of 'UnitClockResponse' function */

/* Drawing clock arrow function.
 * ARGUMENTS:
 *   - handle to draw:
 *       HDC hDC;
 *   - Position of arrow:
 *       INT X, Y;
 *   - Arrows length:
 *       DBL LenArrow;
 *   - Angle to draw:
 *       DBL Angle;
 * RETURNS: None.
 */
static VOID UnitClockDrawArrow( HDC hDC, INT X, INT Y, DBL LenArrowX, DBL LenArrowY, DBL Angle, DWORD Color )
{
  DBL ancos = cos(Angle + MATH_PI * 3 / 2), ansin = sin(Angle + MATH_PI * 3 / 2);
    
  POINT pts[] = 
  {
    {X, Y}, 
    {X + ansin * LenArrowY, Y - ancos * LenArrowY},
    {X + ancos * LenArrowX, Y + ansin * LenArrowX},
    {X - ansin * LenArrowY, Y + ancos * LenArrowY},
    {X - ancos * LenArrowY, Y - ansin * LenArrowY},
    {X + ansin * LenArrowY, Y - ancos * LenArrowY},
    {X, Y}
  };

  /* Angle += MATH_PI * 3 / 2; */

  /* Setting colors to draw */
  SelectObject(hDC, GetStockObject(DC_BRUSH));
  SelectObject(hDC, GetStockObject(NULL_PEN));
  SetDCBrushColor(hDC, Color);
  SetDCPenColor(hDC, 0x0);

  Polygon(hDC, pts, sizeof(pts) / sizeof(pts[0]));

} /* End of 'UnitClockDrawArrow' function */

/* Clock unit rendering function.
 * ARGUMENTS:
 *   - Self pointer:
 *       rk2UNIT_CLOCK *Unit;
 *   - Animation context pointer:
 *       rk2ANIM *Ani;
 * RETURNS: None.
 */
static VOID UnitClockRender( rk2UNIT_CLOCK *Unit, rk2ANIM *Ani )
{
  static SYSTEMTIME SysTime;
  CHAR Str[1000];
  DBL Sum = (Unit->PosX + Unit->PosY) / 2;
  
  GetLocalTime(&SysTime);

  /* Hour arrow */
  UnitClockDrawArrow(Ani->hDC, Unit->PosX, Unit->PosY, Sum / 6, Sum / 48,
                    ((SysTime.wHour % 12) * 60 + SysTime.wMinute) / 12.0 / 60 * MATH_PI * 2 + Unit->Rotate, 
                    0xFFF0FF);
  /* Minute arrow */
  UnitClockDrawArrow(Ani->hDC, Unit->PosX, Unit->PosY, Sum / 4, Sum / 64,
                    (SysTime.wMinute * 60 + SysTime.wSecond) / 3600.0 * MATH_PI * 2 + Unit->Rotate,
                    0xF03030);

  /* Second arrow */
  UnitClockDrawArrow(Ani->hDC, Unit->PosX, Unit->PosY, Sum / 2.5, Sum / 96,
                    (SysTime.wSecond * 1000 + SysTime.wMilliseconds) / 60000.0 * MATH_PI * 2 + Unit->Rotate, 
                    0xF00F0F);

  TextOut(Ani->hDC, 30, 30, Str, sprintf(Str, "%2d:%2d:%2d", SysTime.wHour, SysTime.wMinute, SysTime.wSecond));
} /* End of 'RK2_UnitClockRender' function */

/* Creating unit clock function.
 * ARGUMENTS: None.
 * RETURNS:
 *   (rk2UNIT *) - pointer to new unit.
 */
rk2UNIT *RK2_UnitClockCreate( VOID )
{
  rk2UNIT_CLOCK *Unit;

  if ((Unit = (rk2UNIT_CLOCK *)RK2_AnimUnitCreate(sizeof(rk2UNIT_CLOCK))) == NULL)
    return NULL;
  /* filling fields standart */
  Unit->Init = (VOID *)UnitClockInit;
  Unit->Close = (VOID *)UnitClockClose;
  Unit->Response = (VOID *)UnitClockResponse;
  Unit->Render = (VOID *)UnitClockRender;
  return (rk2UNIT *)Unit;
} /* End of 'RK2_UnitClockCreate' function */

/* END OF 'SAMPCLOCK.C' FILE */
