/* FILENAME: CUBE.C
 * PROGRAMMER: VG4
 * PURPOSE: 3D unit sample.
 * LAST UPDATE: 10.06.2014
 */

#include "anim.h"
#include "render.h"

/* Структура описания объекта анимации */
typedef struct tagvg4UNIT_CUBE
{
  VG4_UNIT_BASE_FIELDS; /* Включение базовых полей */
  vg4GOBJ Cow;
} vg4UNIT_CUBE;

/* Функция инициализации объекта анимации.
 * АРГУМЕНТЫ:
 *   - указатель на "себя" - сам объект анимации:
 *       vg4UNIT_CUBE *Unit;
 *   - указатель на контекст анимации:
 *       vg4ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
static VOID CubeUnitInit( vg4UNIT_CUBE *Unit, vg4ANIM *Ani )
{
  VG4_RndGObjLoad(&Unit->Cow, "cow.object");
} /* End of 'CubeUnitInit' function */

/* Функция деинициализации объекта анимации.
 * АРГУМЕНТЫ:
 *   - указатель на "себя" - сам объект анимации:
 *       vg4UNIT_CUBE *Unit;
 *   - указатель на контекст анимации:
 *       vg4ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
static VOID CubeUnitClose( vg4UNIT_CUBE *Unit, vg4ANIM *Ani )
{
  VG4_RndGObjFree(&Unit->Cow);
} /* End of 'CubeUnitClose' function */

/* Функция обновления межкадровых параметров объекта анимации.
 * АРГУМЕНТЫ:
 *   - указатель на "себя" - сам объект анимации:
 *       vg4UNIT_CUBE *Unit;
 *   - указатель на контекст анимации:
 *       vg4ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
static VOID CubeUnitResponse( vg4UNIT_CUBE *Unit, vg4ANIM *Ani )
{
  VG4_RndWs = Ani->W;
  VG4_RndHs = Ani->H;
  VG4_RndWp = VG4_RndHp * Ani->W / Ani->H;
} /* End of 'CubeUnitResponse' function */

/* Функция построения объекта анимации.
 * АРГУМЕНТЫ:
 *   - указатель на "себя" - сам объект анимации:
 *       vg4UNIT_CUBE *Unit;
 *   - указатель на контекст анимации:
 *       vg4ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
static VOID CubeUnitRender( vg4UNIT_CUBE *Unit, vg4ANIM *Ani )
{
  INT i, s = 5, N = 30 / 30;
  VEC p = {1, 0, 0};
  POINT pt;
  static DBL Delta = 0.1;

  Delta += Ani->JZ * Ani->GlobalDeltaTime;

  VG4_RndMatrView = MatrViewLookAt(VecMulMatr(VecSet(0, 0, Ani->JX * Delta + 15), MatrRotateX(90 * Ani->JY)), VecSet(0, 0, 0), VecSet(0, 1, 0));
  VG4_RndMatrWorld = MatrRotateY(Ani->Time * 30);
  VG4_RndMatrWorld = MatrMulMatr(VG4_RndMatrWorld, MatrScale(0.30, 0.30, 0.30));

  SetDCBrushColor(Ani->hDC, RGB(255, 255, 255));
  SetDCPenColor(Ani->hDC, RGB(255, 255, 255));
  srand(30);
  for (i = 0; i < N; i++)
  {
    p.X = 2.0 * rand() / RAND_MAX - 1;
    p.Y = 2.0 * rand() / RAND_MAX - 1;
    p.Z = 2.0 * rand() / RAND_MAX - 1;
    pt = VG4_RndWorldToScreen(p);
    Ellipse(Ani->hDC, pt.x - s, pt.y - s, pt.x + s, pt.y + s);
  }
  srand(30);
  for (i = 0; i < N; i++)
  {
    p.X = 2.0 * rand() / RAND_MAX - 1;
    p.Y = 2.0 * rand() / RAND_MAX - 1;
    p.Z = 2.0 * rand() / RAND_MAX - 1;
    pt = VG4_RndWorldToScreen(p);
    if (i == 0)
      MoveToEx(Ani->hDC, pt.x, pt.y, NULL);
    else
      LineTo(Ani->hDC, pt.x, pt.y);
  }
  VG4_RndGObjDraw(&Unit->Cow, Ani->hDC);

  VG4_RndMatrWorld = MatrRotateY(Ani->Time * 30);

  VG4_RndMatrWorld = MatrMulMatr(VG4_RndMatrWorld, MatrScale(0.30, 0.30, 0.30));
  VG4_RndMatrWorld = MatrMulMatr(MatrTranslate(0.0, 0.0, 3 * 3.30), VG4_RndMatrWorld);
  VG4_RndGObjDraw(&Unit->Cow, Ani->hDC);
} /* End of 'CubeUnitRender' function */

/* Функция создания 3D объекта анимации.
 * АРГУМЕНТЫ: Нет.
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (vg4UNIT *) указатель на созданный объект анимации.
 */
vg4UNIT * VG4_CubeUnitCreate( VOID )
{
  vg4UNIT_CUBE *Unit;

  if ((Unit = (vg4UNIT_CUBE *)VG4_AnimUnitCreate(sizeof(vg4UNIT_CUBE))) == NULL)
    return NULL;
  /* заполняем поля по-умолчанию */
  Unit->Init = (VOID *)CubeUnitInit;
  Unit->Close = (VOID *)CubeUnitClose;
  Unit->Response = (VOID *)CubeUnitResponse;
  Unit->Render = (VOID *)CubeUnitRender;
  return (vg4UNIT *)Unit;
} /* End of 'VG4_CubeUnitCreate' function */

/* END OF 'CUBE.C' FILE */
