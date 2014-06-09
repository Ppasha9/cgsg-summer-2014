/* FILENAME: ANIM.C
 * PROGRAMMER: RK2
 * PURPOSE: Base animation module
 * LAST UPDATE: 08.06.2014
 */

#include <stdlib.h>
#include <time.h>

#include "anim.h"

#define RK2_JOYSTICK_AXES_FAULT 10.0
#define RK2_JOYSTICK_AXES_COUNT(Axes) (INT)(RK2_JOYSTICK_AXES_FAULT * ((2.0 * (ji.dw##Axes##pos - jc.w##Axes##min) / (jc.w##Axes##max - jc.w##Axes##min - 1) - 1))) \
        / RK2_JOYSTICK_AXES_FAULT;


/* System animation context */
rk2ANIM RK2_Anim;

/* Synchronisation time data */
static INT64
  RK2_TimeFreq,  /* Large integer time per second */
  RK2_TimeStart, /* Animation time begin */
  RK2_TimeOld,   /* Last frame time */
  RK2_TimePause, /* Pause time */
  RK2_TimeFPS;   /* Time to count FPS */
static INT
  RK2_FrameCounter, /* counter of frames */
  /* Mouse variables */
  RK2_MouseX, RK2_MouseY,             /* Mouse coords */
  RK2_MouseXOld, RK2_MouseYOld,       /* Last mouse coords */
  RK2_MouseWheel;                     /* Mouse wheel state */

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
  RK2_TimeFreq = li.QuadPart;

  QueryPerformanceCounter(&li);
  RK2_TimeStart = RK2_TimeOld = RK2_TimeFPS = li.QuadPart;
  RK2_TimePause = 0;
  RK2_FrameCounter = 0;
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

/* Mouse main loop messages function..
 * ARGUMENTS:
 *   - Code of catching:
 *       INT Code;
 *   - word parameter - Mouse message code:
 *       WPARAM wParam;
 *   - long parameter - (MSLLHOOKSTRUCT *) for mouse messages:
 *       LPARAM lParam;
 * RETURNS:
 *   (LRESULT) - Depends from message.
 */
static LRESULT CALLBACK RK2_AnimResponseMouseHook( INT Code, WPARAM wParam, LPARAM lParam )
{
  MSLLHOOKSTRUCT *hs = (MSLLHOOKSTRUCT *)lParam;

  switch (wParam)
  {
  case WM_MOUSEMOVE:
    RK2_MouseX = hs->pt.x;
    RK2_MouseY = hs->pt.y;
    break;
  case WM_MOUSEWHEEL:
    RK2_MouseWheel = (SHORT)HIWORD(hs->mouseData);
    break;
  }
  return 0;
} /* End of 'RK2_AnimResponseMouseHook' function */

/* Animation mouse repsonse function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
static VOID RK2_AnimResponseMouse( VOID )
{
  POINT pt;
  INT i;

  pt.x= RK2_MouseX;
  pt.y = RK2_MouseY;
  ScreenToClient(RK2_Anim.hWnd, &pt);
  RK2_Anim.MouseX = pt.x;
  RK2_Anim.MouseY = pt.y;
  RK2_Anim.MouseDeltaX = RK2_Anim.MouseX - RK2_Anim.MouseXOld;
  RK2_Anim.MouseDeltaY = RK2_Anim.MouseY - RK2_Anim.MouseYOld;
  RK2_Anim.MouseXOld = RK2_Anim.MouseX;
  RK2_Anim.MouseXOld = RK2_Anim.MouseY;
  RK2_Anim.MouseWheel = RK2_MouseWheel;

  /* Update input system */

  memcpy(RK2_Anim.KeysOld, RK2_Anim.Keys, sizeof(RK2_Anim.Keys));

  GetKeyboardState(RK2_Anim.Keys);
  for (i = 0; i < 256; i++)
  {
    RK2_Anim.Keys[i] >>= 7;
    if (!RK2_Anim.KeysOld[i] && RK2_Anim.Keys[i])
      RK2_Anim.KeysClicked[i] = TRUE;
  }

  if (!RK2_Anim.KeysClicked['F'] && RK2_Anim.Keys['F'])
    RK2_AnimFlipFullScreen();
  if (!RK2_Anim.KeysClicked['F'] && RK2_Anim.Keys['P'])
    RK2_AnimSetPause();
} /* End of 'RK2_AnimResponseMouse' function */

/* Animation joystick repsonse function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
static VOID RK2_AnimResponseJoystick( VOID )
{
  INT i;
  static INT Fault = 10;
  if ((i = joyGetNumDevs()) > 0)
  {
    JOYCAPS jc;

    /* Getting joysticks information */
    if (joyGetDevCaps(JOYSTICKID1, &jc, sizeof(jc)) == JOYERR_NOERROR)
    {
      JOYINFOEX ji;

      /* Getting joystick #1 current state */
      ji.dwSize = sizeof(JOYCAPS);
      ji.dwFlags = JOY_RETURNALL;
      if (joyGetPosEx(JOYSTICKID1, &ji) == JOYERR_NOERROR)
      {
        /* Buttons */
        memcpy(RK2_Anim.JoyButsOld, RK2_Anim.JoyButs, sizeof(RK2_Anim.JoyButs));
        for (i = 0; i < 32; i++)
          RK2_Anim.JoyButs[i] = (ji.dwButtons >> i) & 1;
        for (i = 0; i < 32; i++)
          RK2_Anim.JoyButsClicked[i] = RK2_Anim.JoyButs[i] && !RK2_Anim.JoyButsOld[i];

        /* Axis */
        RK2_Anim.JoyX = RK2_JOYSTICK_AXES_COUNT(X);
        RK2_Anim.JoyY = RK2_JOYSTICK_AXES_COUNT(Y);
        
        if (jc.wCaps & JOYCAPS_HASZ)
          RK2_Anim.JoyZ = RK2_JOYSTICK_AXES_COUNT(Z);
        if (jc.wCaps & JOYCAPS_HASR)
          RK2_Anim.JoyR = RK2_JOYSTICK_AXES_COUNT(R);
        if (jc.wCaps & JOYCAPS_HASU)
          RK2_Anim.JoyU = RK2_JOYSTICK_AXES_COUNT(U);

        /* Point-Of-View */
        if (jc.wCaps & JOYCAPS_HASPOV)
        {
          if (ji.dwPOV == 0xFFFF)
            RK2_Anim.JoyPOV = 0;
          else
            RK2_Anim.JoyPOV = ji.dwPOV / 4500 + 1;
        }
      }
    }
  }
} /* End of 'RK2_AnimResponseJoystick' function */

/* Animation rendering function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID RK2_AnimRender( VOID )
{
  INT i;
  LARGE_INTEGER li;

  /* Update timer */
  RK2_Anim.Time = (DBL)clock() / CLOCKS_PER_SEC;

  QueryPerformanceCounter(&li);

  /* global time */
  RK2_Anim.GlobalTime = (DBL)(li.QuadPart - RK2_TimeStart) / RK2_TimeFreq;
  RK2_Anim.GlobalDeltaTime = (DBL)(li.QuadPart - RK2_TimeOld) / RK2_TimeFreq;

  /* local time */
  if (RK2_Anim.IsPause)
  {
    RK2_TimePause += li.QuadPart - RK2_TimeOld;
    RK2_Anim.DeltaTime = 0;
  }
  else
    RK2_Anim.DeltaTime = RK2_Anim.GlobalDeltaTime;

  RK2_Anim.Time = (DBL)(li.QuadPart - RK2_TimeStart - RK2_TimePause) / RK2_TimeFreq;

  /* counting FPS */
  if (li.QuadPart - RK2_TimeFPS > RK2_TimeFreq)
  {
    RK2_Anim.FPS = RK2_FrameCounter / ((DBL)(li.QuadPart - RK2_TimeFPS) / RK2_TimeFreq);
    RK2_TimeFPS = li.QuadPart;
    RK2_FrameCounter = 0;
  }

  /* setting last frame time */
  RK2_TimeOld = li.QuadPart;

  /* Clearing background */
  SelectObject(RK2_Anim.hDC, GetStockObject(DC_BRUSH));
  SelectObject(RK2_Anim.hDC, GetStockObject(NULL_PEN));
  SetDCBrushColor(RK2_Anim.hDC, RGB(0, 110, 100));
  Rectangle(RK2_Anim.hDC, 0, 0, RK2_Anim.W, RK2_Anim.H);

  /* Mouse response */
  RK2_AnimResponseMouse();

  /* Joystick response */
  RK2_AnimResponseJoystick();

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

  RK2_FrameCounter++;
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

/* Adding unit with parametres to animation list function.
 * ARGUMENTS:
 *   - new object:
 *       rk2UNIT *Unit;
 * RETURNS: None.
 */
VOID RK2_AnimAddUnitP( rk2UNIT *Unit )
{
  if (RK2_Anim.NumOfUnits < RK2_MAX_UNITS)
  {
    RK2_Anim.Units[RK2_Anim.NumOfUnits++] = Unit;
    Unit->Init(Unit, &RK2_Anim);
  }
} /* End of 'RK2_AnimAddUnit' function */

/* Switching to/from fullscreen function.
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
    /* MONITORINFOEX moninfo;  For WinXP */
    MONITORINFO moninfo;      /* Windows 8 */

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
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID RK2_AnimSetPause( VOID )
{
  static Pause = TRUE;
  Pause = !Pause;
  RK2_Anim.IsPause = Pause;
} /* End of 'RK2_AnimSetPause' function */

/* END OF 'ANIM.C' FILE */
