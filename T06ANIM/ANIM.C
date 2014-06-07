/* FILENAME: ANIM.C
 * PROGRAMMER: RK2
 * PURPOSE: Base animation module
 * LAST UPDATE: 07.06.2014
 */

#include <stdlib.h>
#include <time.h>

#include "anim.h"

/* System animation context */
static rk2ANIM RK2_Anim;

/* Synchronisation time data */
static INT64
  TimeFreq,  /* Large integer time per second */
  TimeStart, /* Animation time begin */
  TimeOld,   /* Last frame time */
  TimePause, /* Pause time */
  TimeFPS;   /* Time to count FPS */
static INT
  FrameCounter; /* counter of frames */

/* Animation init function
 * ARGUMENTS:
 *   - Window handle:
 *       HWND hWnd;
 * RETURNS: None.
 */
VOID RK2_AnimInit( HWND hWnd )
{
  HDC hDC = GetDC(hWnd);
  LARGE_INTEGER li;

  RK2_Anim.hWnd = hWnd;
  RK2_Anim.hDC = CreateCompatibleDC(hDC);
  RK2_Anim.hBmFrame = CreateCompatibleBitmap(hDC, 30, 30);
  RK2_Anim.W = 30;
  RK2_Anim.H = 30;
  RK2_Anim.NumOfUnits = 0;
  ReleaseDC(hWnd, hDC);

  /* Timer init */
  QueryPerformanceFrequency(&li);
  TimeFreq = li.QuadPart;

  QueryPerformanceCounter(&li);
  TimeStart = TimeOld = TimeFPS = li.QuadPart;
  TimePause = 0;
  FrameCounter = 0;
} /* End of 'RK2_AnimInit' function */

/* Animation destructot function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID RK2_AnimClose( VOID )
{
  INT i;

  /* Units destructor caller */
  for (i = 0; i < RK2_Anim.NumOfUnits; i++)
  {
    RK2_Anim.Units[i]->Close(RK2_Anim.Units[i], &RK2_Anim);
    free(RK2_Anim.Units[i]);
  }
  DeleteObject(RK2_Anim.hBmFrame);
  DeleteDC(RK2_Anim.hDC);
  memset(&RK2_Anim, 0, sizeof(rk2ANIM));
} /* End of 'RK2_AnimInit' function */

/* Resize window animation function.
 * ARGUMENTS:
 *   - new size of window:
 *       INT W, H;
 * RETURNS: None.
 */
VOID RK2_AnimResize( INT W, INT H )
{
  HDC hDC = GetDC(RK2_Anim.hWnd);

  /* creating new buffer */
  DeleteObject(RK2_Anim.hBmFrame);
  RK2_Anim.hBmFrame = CreateCompatibleBitmap(hDC, W, H);
  SelectObject(RK2_Anim.hDC, RK2_Anim.hBmFrame);

  /* saving sizes */
  RK2_Anim.W = W;
  RK2_Anim.H = H;

  ReleaseDC(RK2_Anim.hWnd, hDC);
} /* End of 'RK2_AnimResize' function */

/* Anmation rendering function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID RK2_AnimRender( VOID )
{
  INT i;
  LARGE_INTEGER li;

  /* Update input system */
  GetKeyboardState(RK2_Anim.Keys);
  for (i = 0; i < 256; i++)
    RK2_Anim.Keys[i] >>= 7;

  /* Update timer */
  RK2_Anim.Time = (DBL)clock() / CLOCKS_PER_SEC;

  QueryPerformanceCounter(&li);

  /* global time */
  RK2_Anim.GlobalTime = (DBL)(li.QuadPart - TimeStart) / TimeFreq;
  RK2_Anim.GlobalDeltaTime = (DBL)(li.QuadPart - TimeOld) / TimeFreq;

  /* local time */
  if (RK2_Anim.IsPause)
  {
    TimePause += li.QuadPart - TimeOld;
    RK2_Anim.DeltaTime = 0;
  }
  else
    RK2_Anim.DeltaTime = RK2_Anim.GlobalDeltaTime;

  RK2_Anim.Time = (DBL)(li.QuadPart - TimeStart - TimePause) / TimeFreq;

  /* counting FPS */
  if (li.QuadPart - TimeFPS > TimeFreq)
  {
    RK2_Anim.FPS = FrameCounter / ((DBL)(li.QuadPart - TimeFPS) / TimeFreq);
    TimeFPS = li.QuadPart;
    FrameCounter = 0;
  }

  /* setting last frame time */
  TimeOld = li.QuadPart;

  /* clearing background */
  SelectObject(RK2_Anim.hDC, GetStockObject(DC_BRUSH));
  SelectObject(RK2_Anim.hDC, GetStockObject(NULL_PEN));
  SetDCBrushColor(RK2_Anim.hDC, RGB(0, 110, 0));
  Rectangle(RK2_Anim.hDC, 0, 0, RK2_Anim.W, RK2_Anim.H);

  /* Response caller for units */
  for (i = 0; i < RK2_Anim.NumOfUnits; i++)
    RK2_Anim.Units[i]->Response(RK2_Anim.Units[i], &RK2_Anim);

  /* Drawing objects */
  for (i = 0; i < RK2_Anim.NumOfUnits; i++)
  {
    SelectObject(RK2_Anim.hDC, GetStockObject(DC_BRUSH));
    SelectObject(RK2_Anim.hDC, GetStockObject(DC_PEN));
    SetDCBrushColor(RK2_Anim.hDC, RGB(0, 0, 0));
    SetDCPenColor(RK2_Anim.hDC, RGB(55, 155, 255));
    RK2_Anim.Units[i]->Render(RK2_Anim.Units[i], &RK2_Anim);
  }

  FrameCounter++;
} /* End of 'RK2_AnimRender' function */

/* Drawing to frame animation function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID RK2_AnimCopyFrame( VOID )
{
  HDC hDC;

  /* Drawing frame */
  hDC = GetDC(RK2_Anim.hWnd);
  BitBlt(hDC, 0, 0, RK2_Anim.W, RK2_Anim.H, RK2_Anim.hDC, 0, 0, SRCCOPY);
  ReleaseDC(RK2_Anim.hWnd, hDC);
} /* End of 'RK2_AnimCopyFrame' function */

/* Adding object to animation list function.
 * ARGUMENTS:
 *   - new object:
 *       rk2UNIT *Unit;
 * RETURNS: None.
 */
VOID RK2_AnimAddUnit( rk2UNIT *Unit )
{
  if (RK2_Anim.NumOfUnits < RK2_MAX_UNITS)
  {
    RK2_Anim.Units[RK2_Anim.NumOfUnits++] = Unit;
    Unit->Init(Unit, &RK2_Anim);
  }
} /* End of 'RK2_AnimAddUnit' function */

/* Switching to/from fullscreenв function
 * с учетом нескольких мониторов.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID RK2_AnimFlipFullScreen( VOID )
{
  static BOOL IsFullScreen = FALSE; /* current regime */
  static RECT SaveRC;               /* saved size */

  if (!IsFullScreen)
  {
    RECT rc;
    HMONITOR hmon;
    MONITORINFOEX moninfo;

    /* Old sizes saving */
    GetWindowRect(RK2_Anim.hWnd, &SaveRC);

    /* choosing monitor */
    hmon = MonitorFromWindow(RK2_Anim.hWnd, MONITOR_DEFAULTTONEAREST);

    /* Getting information about current monitor */
    moninfo.cbSize = sizeof(moninfo);
    GetMonitorInfo(hmon, (MONITORINFO *)&moninfo);

    /* Switch to fullscreen */
    /* for one monitor:
    rc.left = 0;
    rc.top = 0;
    rc.right = GetSystemMetrics(SM_CXSCREEN);
    rc.bottom = GetSystemMetrics(SM_CYSCREEN);
    */
    rc = moninfo.rcMonitor;

    AdjustWindowRect(&rc, GetWindowLong(RK2_Anim.hWnd, GWL_STYLE), FALSE);

    SetWindowPos(RK2_Anim.hWnd, HWND_TOP,
      rc.left, rc.top,
      rc.right - rc.left, rc.bottom - rc.top + 201,
      SWP_NOOWNERZORDER);
    IsFullScreen = TRUE;
  }
  else
  {
    /* Update last sizes */
    SetWindowPos(RK2_Anim.hWnd, HWND_TOPMOST,
      SaveRC.left, SaveRC.top,
      SaveRC.right - SaveRC.left, SaveRC.bottom - SaveRC.top,
      SWP_NOOWNERZORDER);
    IsFullScreen = FALSE;
  }
} /* End of 'RK2_AnimFlipFullScreen' function */

/* Setting animation pause function.
 * ARGUMENTS:
 *   - Pause flag:
 *       BOOL NewPauseFlag;
 * RETURNS: None.
 */
VOID RK2_AnimSetPause( BOOL NewPauseFlag )
{
  RK2_Anim.IsPause = NewPauseFlag;
} /* End of 'RK2_AnimSetPause' function */

/* END OF 'ANIM.C' FILE */
