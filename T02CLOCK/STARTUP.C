/* FILENAME: STARTUP.C
 * PROGRAMMER: RK2
 * PURPOSE: Animation startup module
 * LAST UPDATE: 08.06.2014
 */

#include <stdio.h>
#include <math.h>
#include <time.h>

#include "def.h"
#include "anim.h"

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
  wc.hIcon = LoadIcon(NULL, IDI_HAND);
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
  static HDC hDC;
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
    ///hDC = GetDC(hWnd);

    SetTimer(hWnd, 0, 1, NULL);
    return 0;
  case WM_SIZE: 
    break;
  case WM_TIMER:
    hDC = BeginPaint(hWnd, &ps);
    RK2_ClockRender(hDC, 1440, 900);
    EndPaint(hWnd, &ps);
    return 0;
  case WM_CHAR:
    switch ((CHAR)wParam)
    {
    case 27:
      DestroyWindow(hWnd);
      return 0;
    case 'f':
      RK2_AnimFlipFullScreen(hWnd);
      return 0;
    }
    return 0;
  case WM_ERASEBKGND:
    return 1;
  case WM_PAINT:
    hDC = BeginPaint(hWnd, &ps);
    RK2_ClockRender(hDC, 1440, 900);
    EndPaint(hWnd, &ps);
    return 0;
  case WM_DESTROY:
    PostQuitMessage(0);
    KillTimer(hWnd, 30);
    DeleteDC(hDC);
    return 0;
  }
  return DefWindowProc(hWnd, Msg, wParam, lParam);
} /* End of 'MyWindowFunc' function */

/* END OF 'STARTUP.C' FILE */
