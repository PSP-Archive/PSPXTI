/*
 *  xtiger port on PSP 
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <ctype.h>
#include <SDL/SDL.h>

#include "sysdeps.h"
#include "config.h"
#include "options.h"
#include "newcpu.h"
#include "keyboard.h"
#include "specific.h"
#include "debug.h"
#include "global.h"
#include "globinfo.h"

# include "psp_kbd.h"
# include "psp_sdl.h"
# include "psp_specific.h"

/* Colors */


#define ESTAT_SCREENON 1
#define ESTAT_LINKON 2
#define ESTAT_CMDS 8

int contrast = 16;
/* Planar->Chunky conversion table */
int convtab[512];

ULONG *screenBuf = NULL;
int grayPlanes = 2;
int currPlane = 0;
/* Pointer to TI92 planar screen */

unsigned char *the_screen = NULL;

void 
update_progbar(int size) 
{
}

void 
link_progress(int type, char *name, int size) 
{
}

static u32 loc_lcd_background = 0;
static u32 loc_lcd_foreground = 0;

int 
init_specific(void) 
{
  int i,j,k;
  unsigned char *tmptab = (unsigned char *)convtab;

  for(k = 0, i = 0; i<256; i++) {
    for(j = 7; j>=0; j--)
      if((i>>j)&1) tmptab[k++] = 1;
      else         tmptab[k++] = 0;
  }

  psp_init_keyboard();

  screenBuf=(ULONG*)malloc(TI92_WIDTH * TI92_HEIGHT);

  loc_lcd_background = PSP_LCD_BACKGROUND;
  loc_lcd_background |= loc_lcd_background << 16;
  loc_lcd_foreground = PSP_LCD_FOREGROUND;
  loc_lcd_foreground |= loc_lcd_foreground << 16;

  return(1);
}

#define filter(v, l, h) (v<l ? l : (v>h ? h : v))

int lastc = 0;

void 
set_contrast(int c) 
{
}

void CloseTigerWin(void)
{
# if 0
  ggiClose(v);
# endif
}

/** PutImage *************************************************/
/** Put an image on the screen.                             **/
/*************************************************************/

static void
loc_PutImage_max()
{
  u32   color;
  char *scan_buf = (char *)screenBuf;
  u16 *vram;
  u32 *scan_vram1;
  u32 *scan_vram2;
  int y;

  vram  = (u32 *)back_surface->pixels;
  scan_vram1 = vram;
  scan_vram2 = vram + PSP_LINE_SIZE;

  /* top */
# if 0
  y = 4;
  while (y-- > 0) {
    int x = TI92_WIDTH;
    while (x-- > 0) {
      *scan_vram1++ = loc_lcd_background;
      *scan_vram2++ = loc_lcd_background;
    }
    scan_vram1 += PSP_LINE_SIZE - (TI92_WIDTH);
    scan_vram2 += PSP_LINE_SIZE - (TI92_WIDTH);
  }
# else
  scan_vram1 += 4 * PSP_LINE_SIZE;
  scan_vram2 += 4 * PSP_LINE_SIZE;
# endif
  y = TI92_HEIGHT;
  while (y-- > 0) {
    int x = TI92_WIDTH;
    while (x-- > 0) {
      if (*scan_buf++) color = loc_lcd_foreground;
      else             color = loc_lcd_background;
      *scan_vram1++ = color;
      *scan_vram2++ = color;
    }
    scan_vram1 += PSP_LINE_SIZE - (TI92_WIDTH);
    scan_vram2 += PSP_LINE_SIZE - (TI92_WIDTH);
  }
  /* bottom */
# if 0
  y = 4;
  while (y-- > 0) {
    int x = TI92_WIDTH;
    while (x-- > 0) {
      *scan_vram1++ = loc_lcd_background;
      *scan_vram2++ = loc_lcd_background;
    }
    scan_vram1 += PSP_LINE_SIZE - (TI92_WIDTH);
    scan_vram2 += PSP_LINE_SIZE - (TI92_WIDTH);
  }
# endif
}

void
ti92_update_fps()
{
  static u32 next_sec_clock = 0;
  static u32 cur_num_frame = 0;
  cur_num_frame++;
  u32 curclock = SDL_GetTicks();
  if (curclock > next_sec_clock) {
    next_sec_clock = curclock + 1000;
    TI92.ti92_current_fps = cur_num_frame;
    cur_num_frame = 0;
  }
}

static void 
loc_PutImage(void)
{
  if (TI92.psp_skip_cur_frame <= 0) {

    TI92.psp_skip_cur_frame = TI92.psp_skip_max_frame;

    loc_PutImage_max();

    if (psp_kbd_is_danzeff_mode()) {
      danzeff_moveTo(0, -80);
      danzeff_render();
    }

    if (TI92.ti92_view_fps) {
      char buffer[32];
      sprintf(buffer, "%3d", (int)TI92.ti92_current_fps);
      psp_sdl_fill_print(0, 0, buffer, 0xffffff, 0 );
    }
    psp_sdl_flip();

    if (psp_screenshot_mode) {
      psp_screenshot_mode--;
      if (psp_screenshot_mode <= 0) {
        psp_save_screenshot();
        psp_screenshot_mode = 0;
      }
    }
  } else if (TI92.psp_skip_max_frame) {
    TI92.psp_skip_cur_frame--;
  }

  if (TI92.ti92_view_fps) {
    ti92_update_fps();
  }
}

void
psp_save_screenshot(void)
{
  char TmpFileName[MAX_PATH];
  sprintf(TmpFileName,"%s/scr/screenshot_%d.png", TI92.ti92_home_dir, TI92.psp_screenshot_id++);
  if (TI92.psp_screenshot_id >= 10) TI92.psp_screenshot_id = 0;
  psp_sdl_save_png(TmpFileName);
}

void 
exit_specific(void) 
{
  CloseTigerWin();
}

void set_screen_ptr(unsigned char *ptr) {
  the_screen = ptr;
}

void 
ti92_update_screen(void) 
{
  unsigned char *ptr = the_screen;
  ULONG* scan_screen = screenBuf;

  if(!grayPlanes || !currPlane) {
    int l = 128 * 30;
    while (l-- > 0) {
      short v = (*ptr++) << 1;
      *scan_screen++ = convtab[v  ];
      *scan_screen++ = convtab[v+1];
    }
  }
  else {
    int l = 128 * 30;
    while (l-- > 0) {
      short v = (*ptr++) << 1;
      *scan_screen++ += convtab[v  ];
      *scan_screen++ += convtab[v+1];
      }
  }

  if(currPlane++ >= grayPlanes) {
    loc_PutImage();
    currPlane = 0;
  }
}

void
ti92_clear_screen()
{
  psp_sdl_clear_screen(PSP_LCD_BACKGROUND);
  psp_sdl_flip();
  psp_sdl_clear_screen(PSP_LCD_BACKGROUND);
  psp_sdl_flip();
}
