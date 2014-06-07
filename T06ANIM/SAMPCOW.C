/* FILENAME: SAMPCOW.C
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
typedef struct tagrk2UNIT_COW
{
  RK2_UNIT_BASE_FIELDS; /* Включение базовых полей */
  DBL ShiftX, ShiftY;   /* Смещение */
  INT Type;             /* Вид */
} rk2UNIT_COW;

/* Функция инициализации объекта анимации.
 * ARGUMENTS:
 *   - указатель на "себя" - сам объект анимации:
 *       rk2UNIT_COW *Unit;
 *   - указатель на контекст анимации:
 *       rk2ANIM *Ani;
 * RETURNS: None.
 */
static VOID CowUnitInit( rk2UNIT_COW *Unit, rk2ANIM *Ani )
{
} /* End of 'CowUnitInit' function */

/* Функция деинициализации объекта анимации.
 * ARGUMENTS:
 *   - указатель на "себя" - сам объект анимации:
 *       rk2UNIT_COW *Unit;
 *   - указатель на контекст анимации:
 *       rk2ANIM *Ani;
 * RETURNS: None.
 */
static VOID CowUnitClose( rk2UNIT_COW *Unit, rk2ANIM *Ani )
{
} /* End of 'CowUnitClose' function */

/* Функция обновления межкадровых параметров объекта анимации.
 * ARGUMENTS:
 *   - указатель на "себя" - сам объект анимации:
 *       rk2UNIT_COW *Unit;
 *   - указатель на контекст анимации:
 *       rk2ANIM *Ani;
 * RETURNS: None.
 */
static VOID CowUnitResponse( rk2UNIT_COW *Unit, rk2ANIM *Ani )
{
} /* End of 'CowUnitResponse' function */

/* Функция построения объекта анимации.
 * ARGUMENTS:
 *   - указатель на "себя" - сам объект анимации:
 *       rk2UNIT_COW *Unit;
 *   - указатель на контекст анимации:
 *       rk2ANIM *Ani;
 * RETURNS: None.
 */
static VOID CowUnitRender( rk2UNIT_COW *Unit, rk2ANIM *Ani )
{
  DBL
    x = Unit->ShiftX + sin(Ani->Time + Unit->ShiftX) * 30,
    y = Unit->ShiftY + sin(Ani->Time + Unit->ShiftY) * 30;

  if (Unit->Type)
    Rectangle(Ani->hDC, x, y, x + 30, y + 30);
  else
    Ellipse(Ani->hDC, x, y, x + 30, y + 30);
} /* End of 'RK2_AnimUnitRender' function */

/* Функция создания объекта анимации.
 * ARGUMENTS: None.
 * RETURNS:
 *   (rk2UNIT *) указатель на созданный объект анимации.
 */
rk2UNIT * RK2_CowUnitCreate( VOID )
{
  rk2UNIT_COW *Unit;

  if ((Unit = (rk2UNIT_COW *)RK2_AnimUnitCreate(sizeof(rk2UNIT_COW))) == NULL)
    return NULL;
  /* заполняем поля по-умолчанию */
  Unit->Init = (VOID *)CowUnitInit;
  Unit->Close = (VOID *)CowUnitClose;
  Unit->Response = (VOID *)CowUnitResponse;
  Unit->Render = (VOID *)CowUnitRender;
  Unit->ShiftX = 30 * 30.59 * rand() / RAND_MAX;
  Unit->ShiftY = 30 * 30.59 * rand() / RAND_MAX;
  Unit->Type = rand() % 2;
  return (rk2UNIT *)Unit;
} /* End of 'RK2_CowUnitCreate' function */

/* END OF 'SAMPCOW.C' FILE */
