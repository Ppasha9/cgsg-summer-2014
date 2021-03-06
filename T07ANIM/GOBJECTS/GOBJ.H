/* FILENAME: GOBJ.H
 * PROGRAMMER: RK2
 * PURPOSE: Geometry object load functions.
 * LAST UPDATE: 17.06.2014
 */

#ifndef __GOBJ_H_
#define __GOBJ_H_

#include "../anim/anim.h"

#include "../mth/mth.h"
#include "../image/image.h"

/***
 * Geometry objects working
 ***/

/* ��� ������������� ���������� ��������� */
typedef struct tagrk2UV
{
  FLT U, V; /* ���������� ���������� ��������� */
} rk2UV;

/* ������� ������������� ���������� ��������� */
__inline rk2UV RK2_UVSet( FLT U, FLT V )
{
  rk2UV uv;

  uv.U = U;
  uv.V = V;
  return uv;
} /* End of 'RK2_UVSet' function */

/* ��� ������������� ������� */
typedef struct tagrk2GVERTEX
{
  rk2VEC P;    /* ���������� */
  rk2UV T;     /* ���������� ���������� */
  rk2VEC N;    /* ������� */
  rk2COLOR C;  /* ���� */
} rk2GVERTEX;

/* ��� ��������� - ����� ������������� ��� ���������� ����� ������ */
typedef enum tagrk2GPRIM_TYPE
{
  RK2_PRIM_TRIMESH, RK2_PRIM_GRID
} rk2GPRIM_TYPE;

/* ��� �������� ��������� */
typedef struct tagrk2GPRIM
{
  rk2GPRIM_TYPE Type; /* ��� ��������� */
  INT
    NumOfV,      /* ���������� ������ */
    NumOfI,      /* ���������� �������� */
    GW, GH,      /* ������ ����� */
    Mtl,         /* ����� ��������� */
    Size;        /* ������ ���������� ������ � ������ */
  rk2GVERTEX *V; /* ������ ������ */
  INT *I;        /* ������ �������� */

  /* ������ ��� ������ � OpenGL */
  INT
    Buffers[2],    /* ����� ������ � �������� */
    VertexBuffer;  /* ����� �������� ������ */
} rk2GPRIM;

/* ���� ��-��������� */
extern rk2COLOR RK2_DefaultColor;

/* ������� �������� ��������� */
BOOL RK2_GPrimCreate( rk2GPRIM *P, rk2GPRIM_TYPE Type,
       INT NumOfV_GW, INT NumOfI_GH );

/* ������� �������� ����� */
BOOL RK2_GPrimCreateSphere( rk2GPRIM *P,
       rk2VEC C, FLT R, INT M, INT N );

/* Texture Desk creation function */
BOOL RK2_GPrimCreateDesk( rk2GPRIM *P, rk2VEC V0, rk2VEC V1, rk2VEC V2, rk2VEC V3, rk2IMAGE *Texture );

/* Map heights create function. */
BOOL RK2_GPrimCreateHeightField( rk2GPRIM *P, CHAR *FileName, FLT Height, FLT Scale, CHAR *FileTextureName );
/* ������� �������� ��������� */
VOID RK2_GPrimFree( rk2GPRIM *P );

/* ������� ��������� ��������� */
VOID RK2_GPrimDraw( rk2GPRIM *P );

/* ������������ ����� ����� */
#define MAX_STR 300

/* ��� ������������� ��������� */
typedef struct tagrk2GMATERIAL
{
  rk2VEC Ka, Kd, Ks;  /* ������������ ��������� */
  FLT Phong;       /* ����������� ����� */
  FLT Trans;       /* ����������� ������������ */
  CHAR
    Name[MAX_STR], /* ��� ��������� */
    MapD[MAX_STR]; /* ��� �������� */
  INT TexNo;       /* ������������� �������� � OpenGL */
} rk2GMATERIAL;

/* �������������� ������ - ����� ���������� */
typedef struct tagrk2OBJ
{
  INT NumOfPrims, NumOfMtls;  /* ������� �������� */
  rk2GMATERIAL *Mtls;         /* ������� ���������� */
  rk2GPRIM *Prims;            /* ������ ���������� */
} rk2GOBJ;

/* ������� ���������� ��������� � ������� */
INT RK2_GObjAddMaterial( rk2GOBJ *G,
                         rk2GMATERIAL *Mtl );

/* ������� ���������� ��������� � ������� */
INT RK2_GObjAddPrim( rk2GOBJ *G, rk2GPRIM *Prim );

/* ������� ������������ ��������������� ������� */
VOID RK2_GObjFree( rk2GOBJ *G );

/* ������� ��������� */
VOID RK2_GObjDraw( UINT ShadProg, rk2ANIM *Ani, rk2GOBJ *G );

/* ������� �������� ��������������� ������� */
BOOL RK2_GObjLoad( rk2GOBJ *G, CHAR *FileName );

#endif /* __GOBJ_H_ */

/* END OF 'GOBG.H' FILE */
