/* FILENAME: CLOCK.C
 * PROGRAMMER: RK2
 * PURPOSE: Animation unit handle module.
 * LAST UPDATE: 08.06.2014
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "def.h"
#include "anim.h"

/* Drawing clock arrow function.
 * ARGUMENTS:
 *   - handle to draw:
 *       HDC hDC;
 *   - Position of arrow:
 *       INT X, Y;
 *   - Arrows length:
 *       DBL LenArrow;
 *   - Angle to draw:
 *       DBL Angle;
 * RETURNS: None.
 */
static VOID RK2_ClockDrawArrow( HDC hDC, INT X, INT Y, DBL LenArrowX, DBL LenArrowY, DBL Angle, DWORD Color )
{
  DBL ancos = cos(Angle + MATH_PI * 3 / 2), ansin = sin(Angle + MATH_PI * 3 / 2);
    
  POINT pts[] = 
  {
    {X, Y}, 
    {X + ansin * LenArrowY, Y - ancos * LenArrowY},
    {X + ancos * LenArrowX, Y + ansin * LenArrowX },
    {X - ansin * LenArrowY, Y + ancos * LenArrowY},
    {X - ancos * LenArrowY, Y - ansin * LenArrowY},
    {X + ansin * LenArrowY, Y - ancos * LenArrowY},
    {X, Y}
  };

  /* Angle += MATH_PI * 3 / 2; */

  /* Setting colors to draw */
  SelectObject(hDC, GetStockObject(DC_BRUSH));
  SelectObject(hDC, GetStockObject(NULL_PEN));
  SetDCBrushColor(hDC, Color);
  SetDCPenColor(hDC, 0x0);

  Polygon(hDC, pts, sizeof(pts) / sizeof(pts[0]));

} /* End of 'RK2_ClockDrawArrow' function */

/* Clock rendering function.
 * ARGUMENTS:
 *   - hDC context:
 *       HDC hDC;
 *   - Sizes of screen:
 *       INT W, H;
 * RETURNS: None.
 */
VOID RK2_ClockRender( HDC hDC, INT W, INT H )
{
  SYSTEMTIME SysTime;
  CHAR Str[1000];
  DBL Sum = (W + H) / 2;

  /* GetSystemTime(&SysTime); */
  GetLocalTime(&SysTime);

  /* Hour arrow */
  RK2_ClockDrawArrow(hDC, W / 2, H / 2, Sum / 6, Sum / 48,
                    ((SysTime.wHour % 12) * 60 + SysTime.wMinute) / 12.0 / 60 * MATH_PI * 2 + MATH_PI * 2, 
                    0xFFF0FF);
  /* Minute arrow */
  RK2_ClockDrawArrow(hDC, W / 2, H / 2, Sum / 4, Sum / 64,
                    (SysTime.wMinute * 60 + SysTime.wSecond) / 3600.0 * MATH_PI * 2,
                    0xF03030);

  /* Second arrow */
  RK2_ClockDrawArrow(hDC, W / 2, H / 2, Sum / 2.5, Sum / 96,
                    (SysTime.wSecond * 1000 + SysTime.wMilliseconds) / 60000.0 * MATH_PI * 2, 
                    0xF00F0F);

  TextOut(hDC, 30, 30, Str, sprintf(Str, "%2d:%2d:%2d", SysTime.wHour, SysTime.wMinute, SysTime.wSecond));
} /* End of 'RK2_ClockRender' function */

/* Switching to/from fullscreen function
* ARGUMENTS: None.
* RETURNS: None.
*/
VOID RK2_AnimFlipFullScreen( HWND hWnd )
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
    GetWindowRect(hWnd, &SaveRC);

    /* choosing monitor */
    hmon = MonitorFromWindow(hWnd, MONITOR_DEFAULTTONEAREST);

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

    AdjustWindowRect(&rc, GetWindowLong(hWnd, GWL_STYLE), FALSE);

    SetWindowPos(hWnd, HWND_TOP,
      rc.left, rc.top,
      rc.right - rc.left, rc.bottom - rc.top + 201,
      SWP_NOOWNERZORDER);
    IsFullScreen = TRUE;
  }
  else
  {
    /* Update last sizes */
    SetWindowPos(hWnd, HWND_TOPMOST,
      SaveRC.left, SaveRC.top,
      SaveRC.right - SaveRC.left, SaveRC.bottom - SaveRC.top,
      SWP_NOOWNERZORDER);
    IsFullScreen = FALSE;
  }
} /* End of 'RK2_AnimFlipFullScreen' function */

/* END OF 'CLOCK.C' FILE */
