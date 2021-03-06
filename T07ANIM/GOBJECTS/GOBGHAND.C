/* FILENAME: GEOMHAND.C
 * PROGRAMMER: RK2
 * PURPOSE: Geometry object handle functions.
 * LAST UPDATE: 18.08.2014
 */

#include <stdlib.h>

#include "gobj.h"

/* ������� ���������� ��������� � �������.
 * ���������:
 *   - �������������� ������:
 *       rk2GOBJ *G;
 *   - ����������� ��������:
 *       rk2GMATERIAL *Mtl;
 * ������������ ��������:
 *   (INT) ����� ������������ ��������� ��� -1
 *         ��� ������.
 */
INT RK2_GObjAddMaterial( rk2GOBJ *G, rk2GMATERIAL *Mtl )
{
  INT i;
  rk2GMATERIAL *M;

  /* ���� �������� � ���������� */
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
    /* �������� ������ ������ */
    memcpy(M, G->Mtls, sizeof(rk2GMATERIAL) * G->NumOfMtls);
    /* ����������� ������ */
    free(G->Mtls);
  }
  /* ��������� */
  G->Mtls = M;
  M[G->NumOfMtls] = *Mtl;
  return G->NumOfMtls++;
} /* End of 'RK2_GObjAddMaterial' function */

/* ������� ���������� ��������� � �������.
 * ���������:
 *   - �������������� ������:
 *       rk2GOBJ *G;
 *   - ����������� ��������:
 *       rk2GPRIM *Prim;
 * ������������ ��������:
 *   (INT) ����� ������������ ��������� ��� -1
 *         ��� ������.
 */
INT RK2_GObjAddPrim( rk2GOBJ *G, rk2GPRIM *Prim )
{
  rk2GPRIM *P;

  if ((P = malloc(sizeof(rk2GPRIM) * (G->NumOfPrims + 1))) == NULL)
    return -1;
  if (G->Prims != NULL)
  {
    /* �������� ������ ������ */
    memcpy(P, G->Prims, sizeof(rk2GPRIM) * G->NumOfPrims);
    /* ����������� ������ */
    free(G->Prims);
  }
  /* ��������� */
  G->Prims = P;
  P[G->NumOfPrims] = *Prim;
  return G->NumOfPrims++;
} /* End of 'RK2_GObjAddPrim' function */

/* ������� ������������ */
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

/* ������� ��������� */
VOID RK2_GObjDraw( UINT ShadProg, rk2ANIM *Ani, rk2GOBJ *G )
{
  INT i, loc;

  /* Shader setting parametres */
  RK2_RndShadSendGlobInfo(ShadProg, Ani);

  for (i = 0; i < G->NumOfPrims; i++)
  {
    INT mtl = G->Prims[i].Mtl;

    /* Prepairing material */
    if (G->Mtls != NULL)
    {
      INT loc;

      if (G->Mtls[mtl].TexNo == 0 && G->Mtls[mtl].MapD[0] != 0)
      {
        INT j;
        rk2IMAGE Img;

        ImageLoad(&Img, G->Mtls[mtl].MapD);

        /* �������� ��������� ����� �������� */
        glGenTextures(1, &G->Mtls[mtl].TexNo);
        /* ������ �� �������� */
        glBindTexture(GL_TEXTURE_2D, G->Mtls[mtl].TexNo);
        for (j = 0; j < Img.W * Img.H; j++)
        {
          Img.Bits[j] |= 0xFF000000;
        }
        /* ���������� �������� � ����������� */
        /* gluBuild2DMipmaps(GL_TEXTURE_2D, 4, Img.W, Img.H,
          GL_BGRA_EXT, GL_UNSIGNED_BYTE, Img.Bits);
        */
        gluBuild2DMipmaps(GL_TEXTURE_2D, 4, Img.W, Img.H,
          GL_BGRA, GL_UNSIGNED_BYTE, Img.Bits);

        /* ��������� ������ */
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glBindTexture(GL_TEXTURE_2D, 0);
      }
      /* �������� ��������� */
      if (G->Mtls[mtl].TexNo != 0)
      {
        loc = glGetUniformLocation(ShadProg, "DrawTexture");
        if (loc != -1)
          glUniform1i(loc, 0);
          // glUniform1i(loc, G->Mtls[mtl].TexNo);
        glEnable(GL_TEXTURE_2D);
        /*glActiveTexture(GL_TEXTURE0);*/
        glBindTexture(GL_TEXTURE_2D, G->Mtls[mtl].TexNo);
        /*glActiveTexture(GL_TEXTURE1);*/
        glBindTexture(GL_TEXTURE_2D, G->Mtls[mtl].TexNo);
      }
      loc = glGetUniformLocation(ShadProg, "Kamb");
      if (loc != -1)
        glUniform3fv(loc, 1, &G->Mtls[mtl].Ka.X);
      loc = glGetUniformLocation(ShadProg, "Kdif");
      if (loc != -1)
        glUniform3fv(loc, 1, &G->Mtls[mtl].Kd.X);
      loc = glGetUniformLocation(ShadProg, "Ksp");
      if (loc != -1)
        glUniform3fv(loc, 1, &G->Mtls[mtl].Ks.X);
      loc = glGetUniformLocation(ShadProg, "Phong");
      if (loc != -1)
        glUniform1f(loc, G->Mtls[mtl].Phong);
      loc = glGetUniformLocation(ShadProg, "Trans");
      if (loc != -1)
        glUniform1f(loc, G->Mtls[mtl].Trans);
    }
    RK2_GPrimDraw(G->Prims + i);
    glDisable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);
  }
} /* End of 'RK2_GObjDraw' function */

/* END OF 'GEOMHAND.C' FILE */
