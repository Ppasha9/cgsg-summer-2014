/* FILENAME: SAMPCLOCK.C
 * PROGRAMMER: RK2
 * PURPOSE: Animation unit handle module.
 * LAST UPDATE: 07.06.2014
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "anim.h"

/* Структура описания объекта анимации */
typedef struct tagrk2UNIT_CLOCK
{
  RK2_UNIT_BASE_FIELDS; /* Включение базовых полей */
} rk2UNIT_CLOCK;

/* Функция инициализации объекта анимации.
 * ARGUMENTS:
 *   - указатель на "себя" - сам объект анимации:
 *       rk2UNIT_CLOCK *Unit;
 *   - указатель на контекст анимации:
 *       rk2ANIM *Ani;
 * RETURNS: None.
 */
static VOID UnitClockInit( rk2UNIT_CLOCK *Unit, rk2ANIM *Ani )
{
} /* End of 'UnitClockInit' function */

/* Функция деинициализации объекта анимации.
 * ARGUMENTS:
 *   - указатель на "себя" - сам объект анимации:
 *       rk2UNIT_CLOCK *Unit;
 *   - указатель на контекст анимации:
 *       rk2ANIM *Ani;
 * RETURNS: None.
 */
static VOID UnitClockClose( rk2UNIT_CLOCK *Unit, rk2ANIM *Ani )
{
} /* End of 'UnitClockClose' function */

/* Функция обновления межкадровых параметров объекта анимации.
 * ARGUMENTS:
 *   - указатель на "себя" - сам объект анимации:
 *       rk2UNIT_CLOCK *Unit;
 *   - указатель на контекст анимации:
 *       rk2ANIM *Ani;
 * RETURNS: None.
 */
static VOID UnitClockResponse( rk2UNIT_CLOCK *Unit, rk2ANIM *Ani )
{
} /* End of 'UnitClockResponse' function */

/* Drawing clock arrow function
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
static VOID UnitClockDrawArrow( HDC hDC, INT X, INT Y, DBL LenArrow, DBL Angle, DWORD Color )
{
  DBL X0, Y0, X1, Y1, ancos = cos(Angle - 3.14 / 2), ansin = sin(Angle - 3.14 / 2);
  
  
  POINT pts[] = 
  {
    {X, Y}, 
    {X + ancos * LenArrow, Y + ansin * LenArrow},
    {X + ansin * LenArrow / 6, Y - ancos * LenArrow / 6},
    {X - ansin * LenArrow / 6, Y + ancos * LenArrow / 6}
  };
  /*
  POINT pts[] = 
  {
    {10, 10},
    {20, 20},
    {30, 50},
    {200, 400}
  };
  */
  Polygon(hDC, pts, sizeof(pts) / sizeof(pts[0]));

///  Polygon(hDC, pts, 4);
} /* End of 'UnitClockDrawArrow' function */

/* Функция построения объекта анимации.
 * ARGUMENTS:
 *   - указатель на "себя" - сам объект анимации:
 *       rk2UNIT_CLOCK *Unit;
 *   - указатель на контекст анимации:
 *       rk2ANIM *Ani;
 * RETURNS: None.
 */
static VOID UnitClockRender( rk2UNIT_CLOCK *Unit, rk2ANIM *Ani )
{
  SYSTEMTIME SysTime;
  CHAR Str[1000];
  GetSystemTime(&SysTime);
  
  UnitClockDrawArrow(Ani->hDC, Ani->W / 2, Ani->H / 2, (Ani->W + Ani->H) / 4, (SysTime.wSecond * 1000 + SysTime.wMilliseconds) / 60000.0 * 3.14 * 2, 0xF00F0F);
  UnitClockDrawArrow(Ani->hDC, Ani->W / 2, Ani->H / 2, (Ani->W + Ani->H) / 6, (SysTime.wMinute * 60 + SysTime.wSecond) / 3600.0 * 3.14 * 2, 0x00FFFF);
  UnitClockDrawArrow(Ani->hDC, Ani->W / 2, Ani->H / 2, (Ani->W + Ani->H) / 8, (SysTime.wHour * 60 + SysTime.wMinute) / 12.0 * 60 * 3.14 * 2, 0xFFF0FF);
  TextOut(Ani->hDC, 0, 30, Str, sprintf(Str, "%d:%d:%d", SysTime.wHour, SysTime.wMinute, SysTime.wSecond));
} /* End of 'RK2_UnitClockRender' function */

/* Creating unit clock function
 * ARGUMENTS: None.
 * RETURNS:
 *   (rk2UNIT *) pointer to new unit.
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
