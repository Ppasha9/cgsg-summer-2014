/* FILENAME: GOBJPRIM.C
 * PROGRAMMER: RK2
 * PURPOSE: Primtive handle functions.
 * LAST UPDATE: 15.08.2014
 */

#include <stdlib.h>
#include <math.h>
#include <time.h>

#include "gobj.h"

/* ������� */
#define power(A, B) ((A) < 0 ? -pow(-A, B) : pow(A, B))

/* ���� ��-��������� */
rk2COLOR RK2_DefaultColor = {1, 1, 1, 1};

/* ������� �������� ���������.
 * ���������:
 *   - ����������� ��������:
 *       rk2GPRIM *P;
 *   - ��� ���������:
 *       rk2GPRIM_TYPE Type;
 *   - ��������� ����� (������, ������) ���
 *     Type == RK2_PRIM_GRID ��� ���������� ������
 *     � ��������:
 *       INT NumOfV_GW, NumOfI_GH;
 * ������������ ��������:
 *   (BOOL) TRUE ��� ������.
 */
BOOL RK2_GPrimCreate( rk2GPRIM *P, rk2GPRIM_TYPE Type,
       INT NumOfV_GW, INT NumOfI_GH )
{
  INT i, j, nv, ni, size;

  memset(P, 0, sizeof(rk2GPRIM));
  /* ����������� ��������� */
  switch (Type)
  {
  case RK2_PRIM_TRIMESH:
    nv = NumOfV_GW;
    ni = NumOfI_GH;
    break;
  case RK2_PRIM_GRID:
    nv = NumOfV_GW * NumOfI_GH;
    ni = NumOfV_GW * 2;
    break;
  }
  /* �������� ������ */
  size = sizeof(rk2GVERTEX) * nv + sizeof(INT) * ni;
  P->V = malloc(size);
  if (P->V == NULL)
    return FALSE;
  memset(P->V, 0, size);
  
  /* ��������� �������� */
  P->Size = size;
  P->Type = Type;
  P->NumOfV = nv;
  P->NumOfI = ni;
  P->GW = NumOfV_GW;
  P->GH = NumOfI_GH;
  P->I = (INT *)(P->V + nv);
  /* ��������� ��� ������� ��-��������� */
  for (i = 0; i < nv; i++)
  {
    P->V[i].C = RK2_DefaultColor;
    P->V[i].N = VecSet(0, 1, 0);
  }

  /* ��������� ������� � �������� ��� ���. ����� */
  if (Type == RK2_PRIM_GRID)
  {
    for (i = 0; i < NumOfI_GH; i++)
      for (j = 0; j < NumOfV_GW; j++)
        P->V[i * NumOfV_GW + j].T =
          RK2_UVSet(j / (NumOfV_GW - 1.0),
                    i / (NumOfI_GH - 1.0));
    for (i = 0; i < NumOfV_GW; i++)
    {
      P->I[2 * i + 0] = NumOfV_GW + i;
      P->I[2 * i + 1] = i;
    }
  }
  return TRUE;
} /* End of 'RK2_GPrimCreate' function */

/* Sphere creation function.
 * ���������:
 *   - ����������� ��������:
 *       rk2GPRIM *P;
 *   - ����� �����:
 *       rk2VEC C;
 *   - ������ �����:
 *       FLT R;
 *   - ��������� ����� (������, ������):
 *       INT M, N;
 * ������������ ��������:
 *   (BOOL) TRUE ��� ������.
 */
BOOL RK2_GPrimCreateSphere( rk2GPRIM *P, rk2VEC C, FLT R, INT M, INT N )
{
  INT i, j;
  DBL t = clock() / 1000.0, t1;

  t1 = 2 + 2 * cos(t);
  t = 2 + 2 * sin(t * 1.01 + 2);

  if (!RK2_GPrimCreate(P, RK2_PRIM_GRID, M, N))
    return FALSE;
  for (i = 0; i < N; i++)
    for (j = 0; j < M; j++)
    {
      rk2GVERTEX *V = P->V + i * M + j;
      DBL
        theta = i / (N - 1.0) * RK2_PI,
        phi = j / (M - 1.0) * 2 * RK2_PI;

      V->N = VecSet(power(sin(theta), t1) * power(sin(phi), t),
                    power(cos(theta), t1),
                    power(sin(theta), t1) * power(cos(phi), t));
      V->P = VecSumVec(VecMultNum(V->N, R), C);
    }
  return TRUE;
} /* End of 'RK2_GPrimCreateSphere' function */

/* Texture Desk creation function.
 * ARGUMENTS:
 *   - Created primitive:
 *       rk2GPRIM *P;
 *   - Coords points of desk:
 *       rk2VEC V0, V1, V2, V3;
 *   - Texture image):
 *       rk2IMAGE *Texture;
 * RETURNS:
 *   (BOOL) TRUE if success.
 */
BOOL RK2_GPrimCreateDesk( rk2GPRIM *P, rk2VEC V0, rk2VEC V1, rk2VEC V2, rk2VEC V3, rk2IMAGE *Texture )
{
  rk2GVERTEX *V = NULL;

  if (!RK2_GPrimCreate(P, RK2_PRIM_GRID, 2, 2))
    return FALSE;
  P->Type = RK2_PRIM_GRID;
  
  V = P->V;
  V->P = V0;
  V->T = RK2_UVSet(0, 0);

  V = P->V + 1;
  V->P = V1;
  V->T = RK2_UVSet(1, 0);

  V = P->V + 2;
  V->P = V2;
  V->T = RK2_UVSet(1, 1);

  V = P->V + 3;
  V->P = V3;
  V->T = RK2_UVSet(0, 1);
  return TRUE;
} /* End of 'RK2_GPrimCreateDesk' function */

/* Map heights create function.
 * ARGUMENTS:
 *   - Primitive:
 *       rk2GPRIM *P;
 *   - Raster picture with heights:
 *       CHAR *FileName;
 *   - Max hieght and scale:
 *       FLT Height, Scale;
 * RETURNS:
 *   (BOOL) TRUE if success.
 */
BOOL RK2_GPrimCreateHeightField( rk2GPRIM *P, CHAR *FileName, FLT Height, FLT Scale, CHAR *FileTextureName )
{
  INT i, j;
  rk2IMAGE Img, ImgTxt;

  memset(P, 0, sizeof(rk2GPRIM));
  if (!ImageLoad(&Img, FileName))
    return FALSE;
  if (!ImageLoad(&ImgTxt, FileTextureName))
    return FALSE;

  if (!RK2_GPrimCreate(P, RK2_PRIM_GRID, Img.W, Img.H))
    return FALSE;
  /* ������ ���������� ����� */
  for (i = 0; i < Img.H; i++)
    for (j = 0; j < Img.W; j++)
    {
      rk2GVERTEX *V = P->V + i * Img.W + j;
      BYTE rgb[4], h;
      DWORD *col = (DWORD *)rgb;
      *col = ImageGetP(&Img, j, i);
      h = (rgb[2] * 30 + rgb[1] * 59 + rgb[0] * 11) / 100;
      V->P = VecSet((j - Img.W / 2) * Scale, h * Height / 255, (i - Img.H / 2) * Scale);
    }

  /* ������ ������� ����� */
  for (i = 1; i < Img.H - 1; i++)
    for (j = 1; j < Img.W - 1; j++)
    {
      rk2VEC
        p00 = P->V[i * Img.W + j].P,
        p0_1 = P->V[i * Img.W + (j - 1)].P,
        p01 = P->V[i * Img.W + (j + 1)].P,
        p_10 = P->V[(i - 1) * Img.W + j].P,
        p10 = P->V[(i + 1) * Img.W + j].P,
        du0 = VecNormalize(VecSet((p0_1.Y - p00.Y), (p00.X - p0_1.X), 0)),
        du1 = VecNormalize(VecSet((p00.Y - p01.Y), (p01.X - p00.X), 0)),
        dv0 = VecNormalize(VecSet(0, (p00.Z - p_10.Z), (p_10.Y - p00.Y))),
        dv1 = VecNormalize(VecSet(0, (p10.Z - p00.Z), (p00.Y - p10.Y)));

      P->V[i * Img.W + j].N = VecNormalize(VecSumVec(VecSumVec(du0, du1), VecSumVec(dv0, dv1)));
      P->V[i * Img.W + j].T = RK2_UVSet((FLT)j * 1000 / ImgTxt.W, (FLT)i * 1000 / ImgTxt.H);
      /// P->V[i * Img.W + j].N = VecNormalize(VecAddVec(du0, du1));
    }
  ImageFree(&Img);
  ImageFree(&ImgTxt);

  return TRUE;
} /* End of 'RK2_GPrimCreateSphere' function */

/* ������� �������� ���������.
 * ���������:
 *   - ��������� ��������:
 *       rk2GPRIM *P;
 * ������������ ��������: ���.
 */
VOID RK2_GPrimFree( rk2GPRIM *P )
{
  if (P->V != NULL)
    free(P->V);
  memset(P, 0, sizeof(rk2GPRIM));
} /* End of 'RK2_GPrimFree' function */

/* ������� ��������� ���������.
 * ���������:
 *   - ��������:
 *       rk2GPRIM *P;
 * ������������ ��������: ���.
 */
VOID RK2_GPrimDraw( rk2GPRIM *P )
{
  INT i;

  if (P->V == NULL)
    return;

  if (P->Buffers[0] == 0)
  {
    /* ������� � ��������� ������ ������ */
    glGenBuffers(2, P->Buffers);
    glGenVertexArrays(1, &P->VertexBuffer);

    /* ������� */
    glBindBuffer(GL_ARRAY_BUFFER, P->Buffers[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(rk2GVERTEX) * P->NumOfV,
      P->V, GL_STATIC_DRAW);

    /* ������� */
    glBindBuffer(GL_ARRAY_BUFFER, P->Buffers[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(INT) * P->NumOfI,
      P->I, GL_STATIC_DRAW);
  }

  /* ���������� ������ */
  glBindVertexArray(P->VertexBuffer);
  glBindBuffer(GL_ARRAY_BUFFER, P->Buffers[0]);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, P->Buffers[1]);

  /* ������ ������� ������ */
  glVertexAttribPointer(0, 3, GL_FLOAT, FALSE, sizeof(rk2GVERTEX), (VOID *)0);
  glVertexAttribPointer(1, 2, GL_FLOAT, FALSE, sizeof(rk2GVERTEX), (VOID *)(sizeof(rk2VEC)));
  glVertexAttribPointer(2, 3, GL_FLOAT, FALSE, sizeof(rk2GVERTEX), (VOID *)(sizeof(rk2VEC) + sizeof(rk2UV)));
  glVertexAttribPointer(3, 4, GL_FLOAT, FALSE, sizeof(rk2GVERTEX), (VOID *)(2 * sizeof(rk2VEC) + sizeof(rk2UV)));
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  glEnableVertexAttribArray(2);
  glEnableVertexAttribArray(3);

  // glUseProgram(RK2_Anim.ShaderDef);
  if (P->Type == RK2_PRIM_TRIMESH)
  {
    /*
    glBegin(GL_TRIANGLES);
    for (i = 0; i < P->NumOfI; i++)
      glVertex3fv(&P->V[P->I[i]].P.X);
    glEnd();
    */
    glDrawElements(GL_TRIANGLES, P->NumOfI, GL_UNSIGNED_INT, (VOID *)0);
  }
  else
  {
    for (i = 0; i < P->GH - 1; i++)
    {
      /*
      INT start = i * P->GW, j;

      glBegin(GL_TRIANGLE_STRIP);
      for (j = 0; j < P->NumOfI; j++)
        glVertex3fv(&P->V[start + P->I[j]].P.X);
      glEnd();
      */
      glDrawElementsBaseVertex(GL_TRIANGLE_STRIP,
        P->NumOfI, GL_UNSIGNED_INT, (VOID *)0,
        i * P->GW);
      /*
      glDrawElements(GL_TRIANGLE_STRIP, P->NumOfI, GL_UNSIGNED_INT,
        (VOID *)(i * P->GW * 2 * sizeof(INT)));
      */
    }
  }
  // glUseProgram(0);
} /* End of 'RK2_GPrimFree' function */

/* END OF 'PRIM.C' FILE */
