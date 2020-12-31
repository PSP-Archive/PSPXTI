/*
 *  xtiger port on PSP 
 *
 *  Copyright (C) 2006 Ludovic Jacomme (ludovic.jacomme@gmail.com)
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

#ifndef _GLOBAL_H
#define _GLOBAL_H

# include <psptypes.h>
#include <pspkernel.h>
#include <pspdebug.h>
#include <psppower.h>
#include <pspiofilemgr.h>

# ifndef MAX_PATH
# define MAX_PATH 128
# endif

#define TI92_WIDTH  240
#define TI92_HEIGHT 128

#define TI92_MAX_SAVE_STATE    5

  typedef struct TI92_save_t {

    ScePspDateTime  date;
    char            used;

  } TI92_save_t;

   typedef struct TI92_t {

     TI92_save_t   ti92_save_state[TI92_MAX_SAVE_STATE];

     char          ti92_home_dir[MAX_PATH];
     char          psp_screenshot_path[MAX_PATH];
     char          edit_filename[MAX_PATH];
     int           psp_screenshot_id;
     unsigned int  psp_cpu_clock;
     unsigned char psp_reverse_analog;
     int           ti92_view_fps;
     int           ti92_current_fps;
     int           psp_skip_max_frame;
     int           psp_skip_cur_frame;
 
   } TI92_t;

  extern TI92_t TI92;
  extern int    psp_screenshot_mode;
  extern int    psp_exit_now;

  extern int  psp_exit (int);

  extern void ti92_init_resource ();
  extern int  ti92_parse_configuration();
  extern int  ti92_save_configuration();

# ifdef DEBUG
  extern void pspDebugPrintf(const char *Format, ...);
# endif

#endif 
