/* FILENAME: ANIM.H
 * PROGRAMMER: RK2
 * PURPOSE: Globe project declaration module.
 * LAST UPDATE: 07.06.2014
 */

#ifndef __ANIM_H_
#define __ANIM_H_

#include "image.h"

/* Максимальное количество элементов анимации */
#define RK2_MAX_UNITS 3000

/* Объявление типа объектов анимации "вперед" */
typedef struct tagrk2UNIT rk2UNIT;

/* Структура хранения контекста анимации */
typedef struct tagrk2ANIM
{
  HWND hWnd;          /* Окно вывода */
  INT W, H;           /* Размер окна вывода */
  HBITMAP hBmFrame;   /* Буфер кадра */
  HDC hDC;            /* Контекст вывода */

  /* Массив элементов анимации и их количество */
  rk2UNIT *Units[RK2_MAX_UNITS]; 
  INT NumOfUnits;

  /* Подсистема синхронизации */
  DBL
    Time,            /* время в секундах со старта анимации */
    GlobalTime,      /* время -"-, но без паузы */
    DeltaTime,       /* межкадровое время в секундах */
    GlobalDeltaTime, /* межкадровое время в секундах без паузы */
    FPS;             /* количество кадров в секунду */
  BOOL
    IsPause;         /* флаг паузы */

  /* Подсистема ввода */
  BYTE Keys[256];     /* Сотояние клавиш клавиатуры и мыши */
} rk2ANIM;

/* Базовые поля объекта анимации:
 * - размер структуры для наследования
 *     INT Size;
 * - указатель на функцию инициализации
 *     VOID (*Init)( rk2UNIT *Unit, rk2ANIM *Ani );
 * - указатель на функцию деинициализации
 *     VOID (*Close)( rk2UNIT *Unit, rk2ANIM *Ani );
 * - указатель на функцию обновления межкадровых параметров
 *     VOID (*Response)( rk2UNIT *Unit, rk2ANIM *Ani );
 * - указатель на функцию построения
 *     VOID (*Render)( rk2UNIT *Unit, rk2ANIM *Ani );
 */
#define RK2_UNIT_BASE_FIELDS \
  INT Size;                                          \
  VOID (*Init)( rk2UNIT *Unit, rk2ANIM *Ani );       \
  VOID (*Close)( rk2UNIT *Unit, rk2ANIM *Ani );      \
  VOID (*Response)( rk2UNIT *Unit, rk2ANIM *Ani );   \
  VOID (*Render)( rk2UNIT *Unit, rk2ANIM *Ani )

/* Базовый тип объекта анимации */
struct tagrk2UNIT
{
  RK2_UNIT_BASE_FIELDS; /* Базовые поля */
};

/***
 * Функции управления анимацией
 ***/

/* Функция инициализации анимации */
VOID RK2_AnimInit( HWND hWnd );

/* Функция деинициализации анимации */
VOID RK2_AnimClose( VOID );

/* Функция обработки изменения размера области вывода */
VOID RK2_AnimResize( INT W, INT H );

/* Функция построения кадра анимации */
VOID RK2_AnimRender( VOID );

/* Функция вывода кадра анимации */
VOID RK2_AnimCopyFrame( VOID );

/* Функция переключения в/из полноэкранного режима
 * с учетом нескольких мониторов */
VOID RK2_AnimFlipFullScreen( VOID );

/* Функция установки паузы анимации */
VOID RK2_AnimSetPause( BOOL NewPauseFlag );

/***
 * Функции обработки объектов анимации
 ***/

/* Функция добавления в систему объекта анимации */
VOID RK2_AnimAddUnit( rk2UNIT *Unit );

/* Функция создания объекта анимации */
rk2UNIT *RK2_AnimUnitCreate( INT Size );

#endif /* __ANIM_H_ */

/* END OF 'ANIM.H' FILE */
