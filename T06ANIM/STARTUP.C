/* FILENAME: STARTUP.C
 * PROGRAMMER: RK2
 * PURPOSE: Animation startup module
 * LAST UPDATE: 09.06.2014
 */

#include <stdio.h>
#include <math.h>
#include <time.h>

#include "units.h"

#pragma comment(lib, "winmm")

#include <mmsystem.h>

#define WND_CLASS_NAME "My Window Class Name"

/* Declaration Main Window Loop function */
LRESULT CALLBACK MainWindowFunc( HWND hWnd, UINT Msg,
                                 WPARAM wParam, LPARAM lParam );

/* Main program function.
 * ARGUMENTS:
 *   - Descriptor of application:
 *       HINSTANCE hInstance;
 *   - Descriptor of last application:
 *     (Don't use in WinNT: NULL):
 *       HINSTANCE hPrevInstance;
 *   - Command line:
 *       CHAR *CmdLine;
 *   - Flag of showing window (см. SW_SHOWNORMAL, SW_SHOWMINIMIZED, SW_***):
 *       INT ShowCmd;
 * RETURNS:
 *   (INT) Return code to system.
 */
INT WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance,
                    CHAR *CmdLine, INT ShowCmd )
{
  WNDCLASS wc;
  HWND hWnd;
  MSG msg;
  INT i;
  
  /* Mainwindow class registration */
  wc.style = CS_HREDRAW | CS_VREDRAW;
  wc.cbClsExtra = 0; /* Extra count of bytes for class */
  wc.cbWndExtra = 0; /* Extra count of bytes for windows */
  wc.hbrBackground = (HBRUSH)COLOR_WINDOW; /* Background */
  wc.hCursor = LoadCursor(NULL, IDC_HAND);
  wc.hIcon = LoadIcon(NULL, IDI_ASTERISK);
  wc.lpszMenuName = NULL;
  wc.hInstance = hInstance;
  wc.lpfnWndProc = MainWindowFunc;
  wc.lpszClassName = WND_CLASS_NAME;

  /* Class register */
  if (!RegisterClass(&wc))
  {
    MessageBox(NULL, "Error register window class", "Error", MB_ICONERROR | MB_OK);
    return 0;
  }

  /* Creating window */
  hWnd = CreateWindow(WND_CLASS_NAME, "Main Window",
    WS_OVERLAPPEDWINDOW,
    CW_USEDEFAULT, CW_USEDEFAULT, /* window position - default */
    CW_USEDEFAULT, CW_USEDEFAULT, /* Размеры окна (w, h) - default */
    NULL,                         /* Parent window descriptor */
    NULL,                         /* Menu descriptor */
    hInstance,                    /* Application Descriptor */
    NULL);                        /* Another parametres */

  ShowWindow(hWnd, ShowCmd);
  UpdateWindow(hWnd);

  /*** Adding units ***/
  RK2_AnimAddUnit(RK2_UnitAnimContolCreate()); /* Main anim response unit */

  for (i = 0; i < 30 * 30; i++)
    RK2_AnimAddUnit(RK2_CowUnitCreate());
  RK2_AnimAddUnit(RK2_UnitClockCreate());
  RK2_AnimAddUnit(RK2_UnitLogoCreate(30, 30));

  RK2_AnimAddUnit(RK2_InfoUnitCreate());

  /* Main message loop running */
  while (GetMessage(&msg, NULL, 0, 0))
  {
    TranslateMessage(&msg);
    /* Sending messages */
    DispatchMessage(&msg);
  }

  return msg.wParam;
} /* End of 'WinMain' function */

/* Main window message loop function.
 * ARGUMENTS:
 *   - Window descriptor:
 *       HWND hWnd;
 *   - Message ID:
 *       UINT Msg;
 *   - word message parameter:
 *       WPARAM wParam;
 *   - long message parameter::
 *       LPARAM lParam;
 * RETURNS:
 *   (LRESULT) - depends from message.
 */
LRESULT CALLBACK MainWindowFunc( HWND hWnd, UINT Msg,
                                 WPARAM wParam, LPARAM lParam )
{
  HDC hDC;
  PAINTSTRUCT ps;
  MINMAXINFO *minmax;
  static BOOL pause;

  switch (Msg)
  {
  case WM_GETMINMAXINFO:
    minmax = (MINMAXINFO *)lParam;
    minmax->ptMaxTrackSize.y = GetSystemMetrics(SM_CYMAXTRACK) +
      GetSystemMetrics(SM_CYCAPTION) + GetSystemMetrics(SM_CYBORDER) * 2;
    return 0;
  case WM_CREATE:
    SetTimer(hWnd, 30, 1, NULL);
    RK2_AnimInit(hWnd);
    return 0;
  case WM_SIZE:
    RK2_AnimResize(LOWORD(lParam), HIWORD(lParam));
  case WM_TIMER:
    RK2_AnimRender();
    RK2_AnimCopyFrame();
    return 0;
  case WM_ERASEBKGND:
    return 1;
  case WM_PAINT:
    hDC = BeginPaint(hWnd, &ps);
    EndPaint(hWnd, &ps);
    RK2_AnimCopyFrame();
    return 0;
  case WM_DESTROY:
    RK2_AnimClose();
    PostQuitMessage(0);
    KillTimer(hWnd, 30);
    return 0;
  }
  return DefWindowProc(hWnd, Msg, wParam, lParam);
} /* End of 'MyWindowFunc' function */

/* END OF 'STARTUP.C' FILE */
