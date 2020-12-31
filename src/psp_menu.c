/*
 *  Copyright (C) 2009 Ludovic Jacomme (ludovic.jacomme@gmail.com)
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

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/time.h>
#include <fcntl.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL/SDL.h>

#include <pspctrl.h>
#include <pspkernel.h>
#include <pspdebug.h>
#include <pspiofilemgr.h>

#include "global.h"
#include "psp_sdl.h"
#include "psp_kbd.h"
#include "psp_menu.h"
#include "psp_fmgr.h"
#include "psp_menu_kbd.h"
#include "psp_battery.h"
#include "psp_menu_help.h"
#include "psp_editor.h"
#include "psp_irkeyb.h"

extern SDL_Surface *back_surface;
static int psp_menu_dirty = 1;

# define MENU_SCREENSHOT   0

# define MENU_LOAD_SLOT    1
# define MENU_SAVE_SLOT    2
# define MENU_DEL_SLOT     3

# define MENU_HELP         4

# define MENU_LOAD_PRG     5
# define MENU_EDITOR       6
# define MENU_SETTINGS     7
# define MENU_KEYBOARD     8

# define MENU_SOFT_RESET   9
# define MENU_HARD_RESET  10
# define MENU_BACK        11
# define MENU_EXIT        12

# define MAX_MENU_ITEM (MENU_EXIT + 1)

  static menu_item_t menu_list[] =
  {
    { "Save Screenshot :" },

    { "Load Slot" },
    { "Save Slot" },
    { "Delete Slot" },

    { "Help" },

    { "Load Program" },

    { "Comments" },
    { "Settings" },
    { "Keyboard" },

    { "Reset TI-92" },
    { "Hard Reset TI-92" },
    { "Back to TI-92" },
    { "Exit" }
  };

  static int cur_menu_id = MENU_LOAD_PRG;
  static int cur_slot    = 0;

void
string_fill_with_space(char *buffer, int size)
{
  int length = strlen(buffer);
  int index;

  for (index = length; index < size; index++) {
    buffer[index] = ' ';
  }
  buffer[size] = 0;
}

void
psp_display_screen_menu_battery(void)
{
  char buffer[64];

  int Length;
  int color;

  snprintf(buffer, 50, " [%s] ", psp_get_battery_string());
  Length = strlen(buffer);

  if (psp_is_low_battery()) color = PSP_MENU_RED_COLOR;
  else                      color = PSP_MENU_GREEN_COLOR;

  psp_sdl_back2_print(240 - ((8*Length) / 2), 6, buffer, color);
}

static void 
psp_display_screen_menu(void)
{
  char buffer[64];
  int menu_id = 0;
  int slot_id = 0;
  int color   = 0;
  int x       = 0;
  int y       = 0;
  int y_step  = 0;

  if (psp_menu_dirty) 
  {
    psp_sdl_blit_background();
    //psp_menu_dirty = 0;

    psp_sdl_draw_rectangle(10,10,459,249,PSP_MENU_BORDER_COLOR,0);
    psp_sdl_draw_rectangle(11,11,457,247,PSP_MENU_BORDER_COLOR,0);

    psp_sdl_back2_print( 30, 6, " Start+L+R: EXIT ", PSP_MENU_WARNING_COLOR);

    psp_display_screen_menu_battery();

    psp_sdl_back2_print( 370, 6, " R: RESET ", PSP_MENU_NOTE_COLOR);

    psp_sdl_back2_print(30, 254, " []: Cancel  O/X: Valid  Select: Back ", 
                       PSP_MENU_BORDER_COLOR);

    psp_sdl_back2_print(370, 254, " By Zx-81 ",
                       PSP_MENU_AUTHOR_COLOR);
  }
  
  x      = 20;
  y      = 25;
  y_step = 10;
  
  for (menu_id = 0; menu_id < MAX_MENU_ITEM; menu_id++) {
    color = PSP_MENU_TEXT_COLOR;
    if (cur_menu_id == menu_id) color = PSP_MENU_SEL_COLOR;
    else
    if (menu_id == MENU_EXIT) color = PSP_MENU_WARNING_COLOR;
    else
    if (menu_id == MENU_HELP) color = PSP_MENU_GREEN_COLOR;

    psp_sdl_back2_print(x, y, menu_list[menu_id].title, color);

    if (menu_id == MENU_SCREENSHOT) {
      sprintf(buffer,"%d", TI92.psp_screenshot_id);
      string_fill_with_space(buffer, 4);
      psp_sdl_back2_print(170, y, buffer, color);
      y += y_step;
    } else
    if (menu_id == MENU_DEL_SLOT) {
      y += y_step;
    } else
    if (menu_id == MENU_HARD_RESET) {
      y += y_step;
    } else
    if (menu_id == MENU_BACK) {
      y += y_step;
    } else
    if (menu_id == MENU_KEYBOARD) {
      y += y_step;
    } else
    if (menu_id == MENU_HELP) {
      y += y_step;
    } else
    if (menu_id == MENU_LOAD_PRG) {
      y += y_step;
    }

    y += y_step;
  }
  y_step = 10;
  y      = 45;

  for (slot_id = 0; slot_id < TI92_MAX_SAVE_STATE; slot_id++) {
    if (slot_id == cur_slot) color = PSP_MENU_SEL2_COLOR;
    else                     color = PSP_MENU_TEXT_COLOR;

    if (TI92.ti92_save_state[slot_id].used) {
      ScePspDateTime* my_date = &TI92.ti92_save_state[slot_id].date;
# ifndef LINUX_MODE
      sprintf(buffer, "- %02d/%02d %02d:%02d:%02d",
         my_date->day, my_date->month, 
         my_date->hour, my_date->minute, my_date->second );
# else
      sprintf(buffer, "- used");
# endif
    } else {
      sprintf(buffer, "- empty");
    }
    string_fill_with_space(buffer, 32);
    psp_sdl_back2_print(120, y, buffer, color);

    y += y_step;
  }

}

static void
psp_main_menu_reset(void)
{
  /* Reset ! */
  psp_display_screen_menu();
  psp_sdl_back2_print(270, 80, "RESET TI92 !", PSP_MENU_WARNING_COLOR);
  psp_menu_dirty = 1;
  psp_sdl_flip();
  MC68000_reset();
  sleep(1);
}

static int
psp_main_menu_hard_reset(void)
{
  SceCtrlData  c;
  int          do_hard_reset = 0;

  /* Reset ! */
  psp_display_screen_menu();
  psp_sdl_back2_print(180, 100, "press X to hard reset ", PSP_MENU_WARNING_COLOR);
  psp_menu_dirty = 1;
  psp_sdl_flip();

  psp_kbd_wait_no_button();

  do
  {
    myCtrlPeekBufferPositive(&c, 1);
    c.Buttons &= PSP_ALL_BUTTON_MASK;

# ifdef USE_PSP_IRKEYB
    psp_irkeyb_set_psp_key(&c);
# endif

    if (c.Buttons & PSP_CTRL_CROSS) {
      psp_display_screen_menu();
      psp_sdl_back2_print(180, 100, "Hard Reset TI-92 !", 
                      PSP_MENU_WARNING_COLOR);
      psp_menu_dirty = 1;
      psp_sdl_flip();
      hard_reset_emulator();
      do_hard_reset = 1;
      sleep(1);
    }

  } while (c.Buttons == 0);

  psp_kbd_wait_no_button();

  return do_hard_reset;
}

static void
psp_main_menu_load()
{
  int ret;

  ret = psp_fmgr_menu(FMGR_FORMAT_92);
  if (ret ==  1) /* load OK */
  {
    psp_menu_dirty = 1;
    psp_display_screen_menu();
    psp_sdl_back2_print(270, 80, "File loaded !", PSP_MENU_NOTE_COLOR);
    psp_menu_dirty = 1;
    psp_sdl_flip();
    sleep(1);
  }
  else 
  if (ret == -1) /* Load Error */
  {
    psp_menu_dirty = 1;
    psp_display_screen_menu();
    psp_sdl_back2_print(270, 80, "Can't load file !", PSP_MENU_WARNING_COLOR);
    psp_menu_dirty = 1;
    psp_sdl_flip();
    sleep(1);
  }
}

static int
psp_main_menu_load_slot()
{
  int error;

  if (! TI92.ti92_save_state[cur_slot].used) {

    psp_display_screen_menu();
    psp_sdl_back2_print(270, 80, "Slot is empty !", 
                       PSP_MENU_NOTE_COLOR);
    psp_menu_dirty = 1;
    psp_sdl_flip();
    sleep(1);

    return 0;
  }

  error = ti92_snapshot_load_slot(cur_slot);

  if (! error) /* save OK */
  {
    psp_display_screen_menu();
    psp_sdl_back2_print(270, 80, "File loaded !", 
                       PSP_MENU_NOTE_COLOR);
    psp_menu_dirty = 1;
    psp_sdl_flip();
    sleep(1);

    return 1;
  }
  else 
  {
    psp_display_screen_menu();
    psp_sdl_back2_print(270, 80, "Can't load file !", 
                       PSP_MENU_WARNING_COLOR);
    psp_menu_dirty = 1;
    psp_sdl_flip();
    sleep(1);
  }
  return 0;
}

static void
psp_main_menu_save_slot()
{
  int error;
  error = ti92_snapshot_save_slot(cur_slot);

  if (! error) /* save OK */
  {
    psp_display_screen_menu();
    psp_sdl_back2_print(270, 80, "File saved !", 
                       PSP_MENU_NOTE_COLOR);
    psp_menu_dirty = 1;
    psp_sdl_flip();
    sleep(1);
  }
  else 
  {
    psp_display_screen_menu();
    psp_sdl_back2_print(270, 80, "Can't save file !", 
                       PSP_MENU_WARNING_COLOR);
    psp_menu_dirty = 1;
    psp_sdl_flip();
    sleep(1);
  }
}

static void
psp_main_menu_del_slot()
{
  int error;

  if (! TI92.ti92_save_state[cur_slot].used) {

    psp_display_screen_menu();
    psp_sdl_back2_print(270, 80, "Slot is empty !", 
                       PSP_MENU_NOTE_COLOR);
    psp_menu_dirty = 1;
    psp_sdl_flip();
    sleep(1);

    return;
  }

  error = ti92_snapshot_del_slot(cur_slot);

  if (! error) /* save OK */
  {
    psp_display_screen_menu();
    psp_sdl_back2_print(270, 80, "File deleted !", 
                       PSP_MENU_NOTE_COLOR);
    psp_menu_dirty = 1;
    psp_sdl_flip();
    sleep(1);
  }
  else 
  {
    psp_display_screen_menu();
    psp_sdl_back2_print(270, 80, "Can't delete file !", 
                       PSP_MENU_WARNING_COLOR);
    psp_menu_dirty = 1;
    psp_sdl_flip();
    sleep(1);
  }
}

static void
psp_main_menu_cur_slot(int step)
{
  if (step == 1) {
    cur_slot++; if (cur_slot >= TI92_MAX_SAVE_STATE) cur_slot = 0;
  } else if (step == -1) {
    cur_slot--; if (cur_slot < 0) cur_slot = TI92_MAX_SAVE_STATE - 1;
  }
}

static void
psp_main_menu_screenshot(void)
{
  psp_screenshot_mode = 10;
}

int
psp_main_menu_exit(void)
{
  SceCtrlData c;
  int          do_exit = 0;

  psp_display_screen_menu();
  psp_sdl_back2_print(270, 100, "press X to confirm !", PSP_MENU_WARNING_COLOR);
  psp_sdl_back2_print(240, 110, "press O to save and exit", PSP_MENU_WARNING_COLOR);
  psp_menu_dirty = 1;
  psp_sdl_flip();

  psp_kbd_wait_no_button();

  do
  {
    myCtrlPeekBufferPositive(&c, 1);
    c.Buttons &= PSP_ALL_BUTTON_MASK;

    if (c.Buttons & PSP_CTRL_CROSS) {
      do_exit = 1;
    } else 
    if (c.Buttons & PSP_CTRL_CIRCLE) {
      do_exit = 2;
    }
    if (do_exit) {
  
      if (do_exit == 2) {
        ti92_save_configuration();
        ti92_save_emulator_stuff();
      }
      psp_sdl_clear_screen(0);
      psp_sdl_flip();
      psp_sdl_clear_screen(0);
      psp_sdl_flip();
      psp_sdl_exit(0);
    }

  } while (c.Buttons == 0);

  psp_kbd_wait_no_button();

  return 0;
}

void
psp_main_menu_editor()
{
  char TmpFileName[MAX_PATH];
  sprintf(TmpFileName, "%s/comment.txt", TI92.ti92_home_dir);
  psp_editor_menu( TmpFileName );
}

int 
psp_main_menu(void)
{
  SceCtrlData c;
  long        new_pad;
  long        old_pad;
  int         last_time;
  int         end_menu;

  int         danzeff_mode = 0;
  int         danzeff_key;

  psp_kbd_wait_no_button();

  old_pad   = 0;
  last_time = 0;
  end_menu  = 0;

  psp_menu_dirty = 1;

  while (! end_menu)
  {
    psp_display_screen_menu();
    if (danzeff_mode) {
      danzeff_moveTo(-50, -50);
      danzeff_render();
    }
    psp_sdl_flip();

    while (1)
    {
      myCtrlPeekBufferPositive(&c, 1);
      c.Buttons &= PSP_ALL_BUTTON_MASK;

# ifdef USE_PSP_IRKEYB
      psp_irkeyb_set_psp_key(&c);
# endif
      if (c.Buttons) break;
    }

    new_pad = c.Buttons;

    if ((old_pad != new_pad) || ((c.TimeStamp - last_time) > PSP_MENU_MIN_TIME)) {
      last_time = c.TimeStamp;
      old_pad = new_pad;

    } else continue;

    if ((c.Buttons & (PSP_CTRL_LTRIGGER|PSP_CTRL_RTRIGGER|PSP_CTRL_START)) ==
        (PSP_CTRL_LTRIGGER|PSP_CTRL_RTRIGGER|PSP_CTRL_START)) {
      /* Exit ! */
      psp_sdl_exit(0);
    } else
    if ((c.Buttons & PSP_CTRL_LTRIGGER) == PSP_CTRL_LTRIGGER) {
      psp_settings_menu();
      psp_menu_dirty = 1;
      old_pad = new_pad = 0;
    } else
    if ((c.Buttons & PSP_CTRL_RTRIGGER) == PSP_CTRL_RTRIGGER) {
      psp_main_menu_reset();
      end_menu = 1;
    } else
    if ((new_pad & PSP_CTRL_CROSS ) || 
        (new_pad & PSP_CTRL_CIRCLE) || 
        (new_pad & PSP_CTRL_RIGHT ) ||
        (new_pad & PSP_CTRL_LEFT  )) 
    {
      int step;

      if (new_pad & PSP_CTRL_LEFT)  step = -1;
      else 
      if (new_pad & PSP_CTRL_RIGHT) step =  1;
      else                          step =  0;

      switch (cur_menu_id ) 
      {
        case MENU_LOAD_SLOT : if (step) psp_main_menu_cur_slot(step);
                              else
                              if (psp_main_menu_load_slot()) {
                                end_menu = 1;
                              }
        break;
        case MENU_SAVE_SLOT : if (step) psp_main_menu_cur_slot(step);
                              else      psp_main_menu_save_slot();
        break;
        case MENU_DEL_SLOT  : if (step) psp_main_menu_cur_slot(step);
                              else      psp_main_menu_del_slot();
        break;

        case MENU_LOAD_PRG  : psp_menu_dirty = 1;
                              psp_main_menu_load();
                              old_pad = new_pad = 0;
                              psp_menu_dirty = 1;
        break;              

        case MENU_EDITOR    : psp_menu_dirty = 1;
                              psp_main_menu_editor();
                              psp_menu_dirty = 1;
                              old_pad = new_pad = 0;
        break;

        case MENU_SETTINGS   : psp_settings_menu();
                               old_pad = new_pad = 0;
        break;

        case MENU_SCREENSHOT : psp_main_menu_screenshot();
                               end_menu = 1;
        break;              

        case MENU_HARD_RESET : if (psp_main_menu_hard_reset()) end_menu = 1;
        break;
        case MENU_KEYBOARD   : psp_keyboard_menu();
                               psp_menu_dirty = 1;
                               old_pad = new_pad = 0;
        break;
        case MENU_SOFT_RESET : psp_main_menu_reset();
                               end_menu = 1;
        break;

        case MENU_BACK      : end_menu = 1;
        break;

        case MENU_EXIT      : psp_main_menu_exit();
        break;

        case MENU_HELP      : psp_menu_dirty = 1;
                              psp_help_menu();
                              old_pad = new_pad = 0;
                              psp_menu_dirty = 1;
        break;              
      }

    } else
    if(new_pad & PSP_CTRL_UP) {

      if (cur_menu_id > 0) cur_menu_id--;
      else                 cur_menu_id = MAX_MENU_ITEM-1;

    } else
    if(new_pad & PSP_CTRL_DOWN) {

      if (cur_menu_id < (MAX_MENU_ITEM-1)) cur_menu_id++;
      else                                 cur_menu_id = 0;

    } else  
    if(new_pad & PSP_CTRL_SQUARE) {
      /* Cancel */
      end_menu = -1;
    } else 
    if((new_pad & PSP_CTRL_SELECT) == PSP_CTRL_SELECT) {
      /* Back to TI92 */
      end_menu = 1;
    }
  }
 
  psp_kbd_wait_no_button();

  psp_sdl_clear_screen( PSP_MENU_BLACK_COLOR );
  psp_sdl_flip();
  psp_sdl_clear_screen( PSP_MENU_BLACK_COLOR );
  psp_sdl_flip();

  ti92_clear_screen();

  return 1;
}

