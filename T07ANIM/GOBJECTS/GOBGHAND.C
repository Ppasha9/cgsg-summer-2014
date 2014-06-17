/* FILENAME: GEOMHAND.C
 * PROGRAMMER: RK2
 * PURPOSE: Geometry object handle functions.
 * LAST UPDATE: 17.06.2014
 */

#include <stdlib.h>

#include "../anim.h"
#include "gobj.h"

/* Функция добавления материала к объекту.
 * АРГУМЕНТЫ:
 *   - геометрический объект:
 *       rk2GOBJ *G;
 *   - добавляемый материал:
 *       rk2GMATERIAL *Mtl;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (INT) номер добавленного материала или -1
 *         при ошибке.
 */
INT RK2_GObjAddMaterial( rk2GOBJ *G, rk2GMATERIAL *Mtl )
{
  INT i;
  rk2GMATERIAL *M;

  /* ищем материал в библиотеке */
  for (i = 0; i < G->NumOfMtls; i++)
    if (strcmp(G->Mtls[i].Name, Mtl->Name)== 0)
    {
      G->Mtls[i] = *Mtl;
      return i;
    }

  if ((M = malloc(sizeof(rk2GMATERIAL) * (G->NumOfMtls + 1))) == NULL)
    return -1;
  if (G->Mtls != NULL)
  {
    /* копируем старые данные */
    memcpy(M, G->Mtls, sizeof(rk2GMATERIAL) * G->NumOfMtls);
    /* освобождаем память */
    free(G->Mtls);
  }
  /* добавляем */
  G->Mtls = M;
  M[G->NumOfMtls] = *Mtl;
  return G->NumOfMtls++;
} /* End of 'RK2_GObjAddMaterial' function */

/* Функция добавления примитива к объекту.
 * АРГУМЕНТЫ:
 *   - геометрический объект:
 *       rk2GOBJ *G;
 *   - добавляемый примитив:
 *       rk2GPRIM *Prim;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (INT) номер добавленного примитива или -1
 *         при ошибке.
 */
INT RK2_GObjAddPrim( rk2GOBJ *G, rk2GPRIM *Prim )
{
  rk2GPRIM *P;

  if ((P = malloc(sizeof(rk2GPRIM) * (G->NumOfPrims + 1))) == NULL)
    return -1;
  if (G->Prims != NULL)
  {
    /* копируем старые данные */
    memcpy(P, G->Prims, sizeof(rk2GPRIM) * G->NumOfPrims);
    /* освобождаем память */
    free(G->Prims);
  }
  /* добавляем */
  G->Prims = P;
  P[G->NumOfPrims] = *Prim;
  return G->NumOfPrims++;
} /* End of 'RK2_GObjAddPrim' function */

/* Функция освобождения */
VOID RK2_GObjFree( rk2GOBJ *G )
{
  INT i;

  for (i = 0; i < G->NumOfPrims; i++)
    RK2_GPrimFree(G->Prims + i);
  free(G->Prims);
  for (i = 0; i < G->NumOfMtls; i++)
    glDeleteTextures(1, &G->Mtls[i].TexNo);
  free(G->Mtls);
  memset(G, 0, sizeof(rk2GOBJ));
} /* End of 'RK2_GObjFree' function */

/* Функция рисования */
VOID RK2_GObjDraw( rk2GOBJ *G )
{
  INT i, loc;
  rk2MATR4x4 WVP, MatrWorldInvTrans;
  rk2VEC V;

  /* вычислили матрицы преобразования */
  WVP = MatrMultMatr(RK2_Anim.RndMatrWorld,
    MatrMultMatr(RK2_Anim.RndMatrView, RK2_Anim.RndMatrProjection));
  MatrWorldInvTrans = MatrTranspose(MatrInverse(RK2_Anim.RndMatrWorld));

  /* выбор программы шейдеров вывода примитивов */
  glUseProgram(RK2_Anim.ShaderDef);
  loc = glGetUniformLocation(RK2_Anim.ShaderDef, "MatrWVP");
  if (loc != -1)
    glUniformMatrix4fv(loc, 1, FALSE, WVP.A[0]);

  loc = glGetUniformLocation(RK2_Anim.ShaderDef, "MatrWorldInverseTranspose");
  if (loc != -1)
    glUniformMatrix4fv(loc, 1, FALSE, MatrWorldInvTrans.A[0]);

  loc = glGetUniformLocation(RK2_Anim.ShaderDef, "MatrWorld");
  if (loc != -1)
    glUniformMatrix4fv(loc, 1, FALSE, RK2_Anim.RndMatrWorld.A[0]);

  loc = glGetUniformLocation(RK2_Anim.ShaderDef, "MatrView");
  if (loc != -1)
    glUniformMatrix4fv(loc, 1, FALSE, RK2_Anim.RndMatrView.A[0]);

  loc = glGetUniformLocation(RK2_Anim.ShaderDef, "Time");
  if (loc != -1)
    glUniform1f(loc, RK2_Anim.Time);

  V = VecSet(-RK2_Anim.RndMatrView.A[0][2],
             -RK2_Anim.RndMatrView.A[1][2],
             -RK2_Anim.RndMatrView.A[2][2]);
  loc = glGetUniformLocation(RK2_Anim.ShaderDef, "ViewDir");
  if (loc != -1)
    glUniform3fv(loc, 1, &V.X);


  for (i = 0; i < G->NumOfPrims; i++)
  {
    INT mtl = G->Prims[i].Mtl;

    /* подготавливаем материал */
    if (G->Mtls != NULL)
    {
      INT loc;

      if (G->Mtls[mtl].TexNo == 0 && G->Mtls[mtl].MapD[0] != 0)
      {
        INT j;
        rk2IMAGE Img;

        ImageLoad(&Img, G->Mtls[mtl].MapD);

        /* получаем свободный номер текстуры */
        glGenTextures(1, &G->Mtls[mtl].TexNo);
        /* делаем ее активной */
        glBindTexture(GL_TEXTURE_2D, G->Mtls[mtl].TexNo);
        for (j = 0; j < Img.W * Img.H; j++)
        {
          Img.Bits[j] |= 0x00000000;
        }
        /* отправляем картинку в видеопамять */
        /* gluBuild2DMipmaps(GL_TEXTURE_2D, 4, Img.W, Img.H,
          GL_BGRA_EXT, GL_UNSIGNED_BYTE, Img.Bits);
        */
        gluBuild2DMipmaps(GL_TEXTURE_2D, 4, Img.W, Img.H,
          GL_BGRA, GL_UNSIGNED_BYTE, Img.Bits);

        /* Параметры вывода */
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glBindTexture(GL_TEXTURE_2D, 0);
      }
      /* передаем параметры */
      if (G->Mtls[mtl].TexNo != 0)
      {
        loc = glGetUniformLocation(RK2_Anim.ShaderDef, "DrawTexture");
        if (loc != -1)
          glUniform1i(loc, 0);
        glEnable(GL_TEXTURE_2D);
        /*glActiveTexture(GL_TEXTURE0);*/
        glBindTexture(GL_TEXTURE_2D, G->Mtls[mtl].TexNo);
        /*glActiveTexture(GL_TEXTURE1);*/
        glBindTexture(GL_TEXTURE_2D, G->Mtls[mtl].TexNo);
      }
      loc = glGetUniformLocation(RK2_Anim.ShaderDef, "Ka");
      if (loc != -1)
        glUniform3fv(loc, 1, &G->Mtls[mtl].Ka.X);
      loc = glGetUniformLocation(RK2_Anim.ShaderDef, "Kd");
      if (loc != -1)
        glUniform3fv(loc, 1, &G->Mtls[mtl].Kd.X);
      loc = glGetUniformLocation(RK2_Anim.ShaderDef, "Ks");
      if (loc != -1)
        glUniform3fv(loc, 1, &G->Mtls[mtl].Ks.X);
      loc = glGetUniformLocation(RK2_Anim.ShaderDef, "Phong");
      if (loc != -1)
        glUniform1f(loc, G->Mtls[mtl].Phong);
      loc = glGetUniformLocation(RK2_Anim.ShaderDef, "Trans");
      if (loc != -1)
      {
        glUniform1f(loc, G->Mtls[mtl].Trans);
        
        if (strcmp(G->Mtls[mtl].Name, "..\\Textures\\lava.bmp") == 0)
          glUniform1f(loc, sin(RK2_Anim.Time));
        
      }
    }
    RK2_GPrimDraw(G->Prims + i);
    glDisable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);
  }
} /* End of 'RK2_GObjDraw' function */

/* END OF 'GEOMHAND.C' FILE */
