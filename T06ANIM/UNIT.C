/* FILENAME: UNIT.C
 * PROGRAMMER: RK2
 * PURPOSE: Animation unit handle module.
 * LAST UPDATE: 07.06.2014
 */

#include <stdlib.h>
#include <string.h>

#include "anim.h"

/* Функция по-умолчанию инициализации объекта анимации.
 * ARGUMENTS:
 *   - указатель на "себя" - сам объект анимации:
 *       rk2UNIT *Unit;
 *   - указатель на контекст анимации:
 *       rk2ANIM *Ani;
 * RETURNS: None.
 */
static VOID RK2_AnimUnitInit( rk2UNIT *Unit, rk2ANIM *Ani )
{
} /* End of 'RK2_AnimUnitInit' function */

/* Функция по-умолчанию деинициализации объекта анимации.
 * ARGUMENTS:
 *   - указатель на "себя" - сам объект анимации:
 *       rk2UNIT *Unit;
 *   - указатель на контекст анимации:
 *       rk2ANIM *Ani;
 * RETURNS: None.
 */
static VOID RK2_AnimUnitClose( rk2UNIT *Unit, rk2ANIM *Ani )
{
} /* End of 'RK2_AnimUnitClose' function */

/* Функция по-умолчанию обновления межкадровых параметров объекта анимации.
 * ARGUMENTS:
 *   - указатель на "себя" - сам объект анимации:
 *       rk2UNIT *Unit;
 *   - указатель на контекст анимации:
 *       rk2ANIM *Ani;
 * RETURNS: None.
 */
static VOID RK2_AnimUnitResponse( rk2UNIT *Unit, rk2ANIM *Ani )
{
} /* End of 'RK2_AnimUnitResponse' function */

/* Функция по-умолчанию построения объекта анимации.
 * ARGUMENTS:
 *   - указатель на "себя" - сам объект анимации:
 *       rk2UNIT *Unit;
 *   - указатель на контекст анимации:
 *       rk2ANIM *Ani;
 * RETURNS: None.
 */
static VOID RK2_AnimUnitRender( rk2UNIT *Unit, rk2ANIM *Ani )
{
} /* End of 'RK2_AnimUnitRender' function */

/* Функция создания объекта анимации.
 * ARGUMENTS:
 *   - размер структуры объекта анимации:
 *       INT Size;
 * RETURNS:
 *   (rk2UNIT *) указатель на созданный объект анимации.
 */
rk2UNIT * RK2_AnimUnitCreate( INT Size )
{
  rk2UNIT *Unit;

  if (Size < sizeof(rk2UNIT) ||
      (Unit = malloc(Size)) == NULL)
    return NULL;
  memset(Unit, 0, Size);
  /* заполняем поля по-умолчанию */
  Unit->Size = Size;
  Unit->Init = RK2_AnimUnitInit;
  Unit->Close = RK2_AnimUnitClose;
  Unit->Response = RK2_AnimUnitResponse;
  Unit->Render = RK2_AnimUnitRender;
  return Unit;
} /* End of 'RK2_AnimUnitCreate' function */

/* END OF 'UNIT.C' FILE */

