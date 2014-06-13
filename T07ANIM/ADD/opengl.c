_CRT_SECURE_NO_WARNINGS

E:\$(UserName)\$(ProjectName)\$(ConfigurationName)

Общие определения:

#pragma warning(disable: 4244 4305)

#include <windows.h>
#include <glew.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include <mmsystem.h>

#pragma comment(lib, "opengl32")
#pragma comment(lib, "glu32")
#pragma comment(lib, "glew32")
#pragma comment(lib, "winmm")

/* Large integer data types */
typedef __int64 INT64;
typedef unsigned __int64 UINT64;

/* Common float point types */
typedef float FLT;
typedef double DBL;


Инициализация:
  PIXELFORMATDESCRIPTOR pfd = {0};

  /*** Инициализация OpenGL ***/

  /* описываем формат точки */
  pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
  pfd.dwFlags = PFD_SUPPORT_OPENGL | PFD_SUPPORT_GDI | PFD_DOUBLEBUFFER;
  pfd.iPixelType = PFD_TYPE_RGBA;
  pfd.cColorBits = 32;
  i = ChoosePixelFormat(VG4_Anim.hDC, &pfd);
  DescribePixelFormat(VG4_Anim.hDC, i, sizeof(pfd), &pfd);
  SetPixelFormat(VG4_Anim.hDC, i, &pfd);

  /* создаем контекст построения */
  VG4_Anim.hRC = wglCreateContext(VG4_Anim.hDC);
  /* делаем текущими контексты */
  wglMakeCurrent(VG4_Anim.hDC, VG4_Anim.hRC);

  /* инициализируем расширения */
  if (glewInit() != GLEW_OK ||
      !(GLEW_ARB_vertex_shader && GLEW_ARB_fragment_shader))
  {
    wglMakeCurrent(NULL, NULL);
    wglDeleteContext(VG4_Anim.hRC);
    ReleaseDC(VG4_Anim.hWnd, VG4_Anim.hDC);
    memset(&VG4_Anim, 0, sizeof(vg4ANIM));
    return FALSE;
  }
  /* параметры OpenGL по-умолчанию */
  . . .

Деинициализация:
  . . .
  /* отключаем контексты */
  wglMakeCurrent(NULL, NULL);

  wglDeleteContext(VG4_Anim.hRC);
  ReleaseDC(VG4_Anim.hWnd, VG4_Anim.hDC);
  . . .

Изменение размера области вывода:
  . . .
  /* Поле просмотра */
  glViewport(0, 0, W, H);
  . . .

Вывод:
  - очистка
  /* очистка фона */
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  - отрисовка
  glRectd(0, 0, 1, 1);

  - сброс кадра
  glFinish();
  SwapBuffers(VG4_Anim.hDC);










