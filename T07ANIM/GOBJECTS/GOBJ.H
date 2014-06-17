/* FILENAME: GOBJ.H
 * PROGRAMMER: RK2
 * PURPOSE: Geometry object load functions.
 * LAST UPDATE: 17.06.2014
 */

#ifndef __GOBJ_H_
#define __GOBJ_H_

/***
 * Geometry objects working
 ***/

/* Тип представления текстурных координат */
typedef struct tagrk2UV
{
  FLT U, V; /* компоненты текстурных координат */
} rk2UV;

/* Функиця инициализации текстурных координат */
__inline rk2UV RK2_UVSet( FLT U, FLT V )
{
  rk2UV uv;

  uv.U = U;
  uv.V = V;
  return uv;
} /* End of 'RK2_UVSet' function */

/* Теп представления вершины */
typedef struct tagrk2GVERTEX
{
  rk2VEC P;    /* координаты */
  rk2UV T;     /* текстурные координаты */
  rk2VEC N;    /* нормаль */
  rk2COLOR C;  /* цвет */
} rk2GVERTEX;

/* Тип примитива - сетка треугольников или регулярная сетка вершин */
typedef enum tagrk2GPRIM_TYPE
{
  RK2_PRIM_TRIMESH, RK2_PRIM_GRID
} rk2GPRIM_TYPE;

/* Тип хранения примитива */
typedef struct tagrk2GPRIM
{
  rk2GPRIM_TYPE Type; /* тип примитива */
  INT
    NumOfV,      /* количество вершин */
    NumOfI,      /* количество индексов */
    GW, GH,      /* размер сетки */
    Mtl,         /* номер материала */
    Size;        /* размер выделенной памяти в байтах */
  rk2GVERTEX *V; /* массив вершин */
  INT *I;        /* массив индексов */

  /* Данные для вывода в OpenGL */
  INT
    Buffers[2],    /* Буфер вершин и индексов */
    VertexBuffer;  /* Буфер описания вершин */
} rk2GPRIM;

/* Цвет по-умолчанию */
extern rk2COLOR RK2_DefaultColor;

/* Функция создания примитива */
BOOL RK2_GPrimCreate( rk2GPRIM *P, rk2GPRIM_TYPE Type,
       INT NumOfV_GW, INT NumOfI_GH );

/* Функция создания сферы */
BOOL RK2_GPrimCreateSphere( rk2GPRIM *P,
       rk2VEC C, FLT R, INT M, INT N );

/* Texture Desk creation function */
BOOL RK2_GPrimCreateDesk( rk2GPRIM *P, rk2VEC V0, rk2VEC V1, rk2VEC V2, rk2VEC V3, rk2IMAGE *Texture );

/* Map heights create function. */
BOOL RK2_GPrimCreateHeightField( rk2GPRIM *P, CHAR *FileName, FLT Height, FLT Scale );

/* Функция удаления примитива */
VOID RK2_GPrimFree( rk2GPRIM *P );

/* Функция отрисовки примитива */
VOID RK2_GPrimDraw( rk2GPRIM *P );

/* Максимальная длина имени */
#define MAX_STR 300

/* Тип представления материала */
typedef struct tagrk2GMATERIAL
{
  rk2VEC Ka, Kd, Ks;  /* коэффициенты освещения */
  FLT Phong;       /* коэффициент Фонга */
  FLT Trans;       /* коэффициент прозрачности */
  CHAR
    Name[MAX_STR], /* имя материала */
    MapD[MAX_STR]; /* имя текстуры */
  INT TexNo;       /* идентификатор текстуры в OpenGL */
} rk2GMATERIAL;

/* Геометрический объект - набор примитивов */
typedef struct tagrk2OBJ
{
  INT NumOfPrims, NumOfMtls;  /* размерв массивов */
  rk2GMATERIAL *Mtls;         /* таблица материалов */
  rk2GPRIM *Prims;            /* массив примитивов */
} rk2GOBJ;

/* Функция добавления материала к объекту */
INT RK2_GObjAddMaterial( rk2GOBJ *G,
                         rk2GMATERIAL *Mtl );

/* Функция добавления примитива к объекту */
INT RK2_GObjAddPrim( rk2GOBJ *G, rk2GPRIM *Prim );

/* Функция освобождения геометрического объекта */
VOID RK2_GObjFree( rk2GOBJ *G );

/* Функция рисования */
VOID RK2_GObjDraw( rk2GOBJ *G );

/* Функция загрузки геометрического объекта */
BOOL RK2_GObjLoad( rk2GOBJ *G, CHAR *FileName );

#endif /* __GOBJ_H_ */

/* END OF 'GOBG.H' FILE */
