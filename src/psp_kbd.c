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

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/time.h>
#include <fcntl.h>

#include <pspctrl.h>
#include <pspkernel.h>
#include <pspdebug.h>

#include "global.h"
#include "sysdeps.h"
#include "config.h"
#include "options.h"
#include "newcpu.h"
#include "keyboard.h"
#include "specific.h"
#include "debug.h"
#include "globinfo.h"
#include "ti_main.h"
#include "psp_irkeyb.h"

#include <SDL/SDL.h>

#include "psp_kbd.h"
#include "psp_menu.h"
#include "psp_sdl.h"
#include "psp_danzeff.h"
#include "psp_battery.h"
#include "psp_specific.h"

# define KBD_MIN_START_TIME       800000
# define KBD_MAX_EVENT_TIME       500000
# define KBD_MIN_PENDING_TIME      10000
# define KBD_MIN_EVENT_TIME       180000
# define KBD_MIN_ANALOG_TIME1     150000
# define KBD_MIN_ANALOG_TIME2     100000
# define KBD_MIN_DANZEFF_TIME      10000
# define KBD_MIN_BATTCHECK_TIME 90000000 

 static SceCtrlData    loc_button_data;
 static unsigned int   loc_last_event_time = 0;
#ifdef USE_PSP_IRKEYB
 static unsigned int   loc_last_irkbd_event_time = 0;
#endif
 static long           first_time_stamp = -1;
 static long           first_time_batt_stamp = -1;
 static char           loc_button_press[ KBD_MAX_BUTTONS ]; 
 static char           loc_button_release[ KBD_MAX_BUTTONS ]; 
 static unsigned int   loc_button_mask[ KBD_MAX_BUTTONS ] =
 {
   PSP_CTRL_UP         , /*  KBD_UP         */
   PSP_CTRL_RIGHT      , /*  KBD_RIGHT      */
   PSP_CTRL_DOWN       , /*  KBD_DOWN       */
   PSP_CTRL_LEFT       , /*  KBD_LEFT       */
   PSP_CTRL_TRIANGLE   , /*  KBD_TRIANGLE   */
   PSP_CTRL_CIRCLE     , /*  KBD_CIRCLE     */
   PSP_CTRL_CROSS      , /*  KBD_CROSS      */
   PSP_CTRL_SQUARE     , /*  KBD_SQUARE     */
   PSP_CTRL_SELECT     , /*  KBD_SELECT     */
   PSP_CTRL_START      , /*  KBD_START      */
   PSP_CTRL_HOME       , /*  KBD_HOME       */
   PSP_CTRL_HOLD       , /*  KBD_HOLD       */
   PSP_CTRL_LTRIGGER   , /*  KBD_LTRIGGER   */
   PSP_CTRL_RTRIGGER   , /*  KBD_RTRIGGER   */
 };

  static char loc_button_name[ KBD_ALL_BUTTONS ][10] =
  {
    "UP",
    "RIGHT",
    "DOWN",
    "LEFT",
    "TRIANGLE",
    "CIRCLE",
    "CROSS",
    "SQUARE",
    "SELECT",
    "START",
    "HOME",
    "HOLD",
    "LTRIGGER",
    "RTRIGGER",
    "JOY_UP",
    "JOY_RIGHT",
    "JOY_DOWN",
    "JOY_LEFT"
  };

 static char loc_button_name_L[ KBD_ALL_BUTTONS ][20] =
 {
   "L_UP",
   "L_RIGHT",
   "L_DOWN",
   "L_LEFT",
   "L_TRIANGLE",
   "L_CIRCLE",
   "L_CROSS",
   "L_SQUARE",
   "L_SELECT",
   "L_START",
   "L_HOME",
   "L_HOLD",
   "L_LTRIGGER",
   "L_RTRIGGER",
   "L_JOY_UP",
   "L_JOY_RIGHT",
   "L_JOY_DOWN",
   "L_JOY_LEFT"
 };
 
  static char loc_button_name_R[ KBD_ALL_BUTTONS ][20] =
 {
   "R_UP",
   "R_RIGHT",
   "R_DOWN",
   "R_LEFT",
   "R_TRIANGLE",
   "R_CIRCLE",
   "R_CROSS",
   "R_SQUARE",
   "R_SELECT",
   "R_START",
   "R_HOME",
   "R_HOLD",
   "R_LTRIGGER",
   "R_RTRIGGER",
   "R_JOY_UP",
   "R_JOY_RIGHT",
   "R_JOY_DOWN",
   "R_JOY_LEFT"
 };
 
 static int loc_default_mapping[ KBD_ALL_BUTTONS ] = {
    TI92_BUTTON_UP         , /*  KBD_UP         */
    TI92_BUTTON_RIGHT      , /*  KBD_RIGHT      */
    TI92_BUTTON_DOWN       , /*  KBD_DOWN       */
    TI92_BUTTON_LEFT       , /*  KBD_LEFT       */
    TI92_BUTTON_ESCAPE     , /*  KBD_TRIANGLE   */
    TI92_BUTTON_SPACE      , /*  KBD_CIRCLE     */
    TI92_BUTTON_ENTER1     , /*  KBD_CROSS      */
    TI92_BUTTON_BACKSPACE  , /*  KBD_SQUARE     */
    -1                     , /*  KBD_SELECT     */
    -1                     , /*  KBD_START      */
    -1                     , /*  KBD_HOME       */
    -1                     , /*  KBD_HOLD       */
    KBD_LTRIGGER_MAPPING   , /*  KBD_LTRIGGER   */
    KBD_RTRIGGER_MAPPING   , /*  KBD_RTRIGGER   */
    TI92_BUTTON_F1        , /*  KBD_JOY_UP     */
    TI92_BUTTON_F2        , /*  KBD_JOY_RIGHT  */
    TI92_BUTTON_F3        , /*  KBD_JOY_DOWN   */
    TI92_BUTTON_F4          /*  KBD_JOY_LEFT   */
  };

 static int loc_default_mapping_L[ KBD_ALL_BUTTONS ] = {
    TI92_BUTTON_F1              , /*  KBD_UP         */
    TI92_BUTTON_F2              , /*  KBD_RIGHT      */
    TI92_BUTTON_F3              , /*  KBD_DOWN       */
    TI92_BUTTON_F4              , /*  KBD_LEFT       */
    TI92_BUTTON_HAND            , /*  KBD_TRIANGLE   */
    TI92_BUTTON_2ND             , /*  KBD_CIRCLE     */
    TI92_BUTTON_DIAMOND         , /*  KBD_CROSS      */
    TI92_BUTTON_APPS            , /*  KBD_SQUARE     */
    -1                     , /*  KBD_SELECT     */
    -1                     , /*  KBD_START      */
    -1                     , /*  KBD_HOME       */
    -1                     , /*  KBD_HOLD       */
   KBD_LTRIGGER_MAPPING  , /*  KBD_LTRIGGER   */
   KBD_RTRIGGER_MAPPING  , /*  KBD_RTRIGGER   */
    TI92_BUTTON_UP          , /*  KBD_JOY_UP     */
    TI92_BUTTON_RIGHT       , /*  KBD_JOY_RIGHT  */
    TI92_BUTTON_DOWN        , /*  KBD_JOY_DOWN   */
    TI92_BUTTON_LEFT          /*  KBD_JOY_LEFT   */
  };

 static int loc_default_mapping_R[ KBD_ALL_BUTTONS ] = {
    TI92_BUTTON_UP              , /*  KBD_UP         */
    TI92_BUTTON_RIGHT           , /*  KBD_RIGHT      */
    TI92_BUTTON_DOWN            , /*  KBD_DOWN       */
    TI92_BUTTON_LEFT            , /*  KBD_LEFT       */
    TI92_BUTTON_DIAMOND         , /*  KBD_TRIANGLE   */
    TI92_BUTTON_RIGHT_PARENT    , /*  KBD_CIRCLE     */
    TI92_BUTTON_COMMA           , /*  KBD_CROSS      */
    TI92_BUTTON_LEFT_PARENT     , /*  KBD_SQUARE     */
    -1                     , /*  KBD_SELECT     */
    -1                     , /*  KBD_START      */
    -1                     , /*  KBD_HOME       */
    -1                     , /*  KBD_HOLD       */
    KBD_LTRIGGER_MAPPING  , /*  KBD_LTRIGGER   */
    KBD_RTRIGGER_MAPPING  , /*  KBD_RTRIGGER   */
    TI92_BUTTON_UP          , /*  KBD_JOY_UP     */
    TI92_BUTTON_RIGHT       , /*  KBD_JOY_RIGHT  */
    TI92_BUTTON_DOWN        , /*  KBD_JOY_DOWN   */
    TI92_BUTTON_LEFT          /*  KBD_JOY_LEFT   */
  };
 
# define KBD_MAX_ENTRIES   157

  int kbd_layout[KBD_MAX_ENTRIES][2] = {
    /* Key            Ascii */
   { TI92_BUTTON_A           , 'A' },
   { TI92_BUTTON_B           , 'B' },
   { TI92_BUTTON_C           , 'C' },
   { TI92_BUTTON_D           , 'D' },
   { TI92_BUTTON_E           , 'E' },
   { TI92_BUTTON_F           , 'F' },
   { TI92_BUTTON_G           , 'G' },
   { TI92_BUTTON_H           , 'H' },
   { TI92_BUTTON_I           , 'I' },
   { TI92_BUTTON_J           , 'J' },
   { TI92_BUTTON_K           , 'K' },
   { TI92_BUTTON_L           , 'L' },
   { TI92_BUTTON_M           , 'M' },
   { TI92_BUTTON_N           , 'N' },
   { TI92_BUTTON_O           , 'O' },
   { TI92_BUTTON_P           , 'P' },
   { TI92_BUTTON_Q           , 'Q' },
   { TI92_BUTTON_R           , 'R' },
   { TI92_BUTTON_S           , 'S' },
   { TI92_BUTTON_T           , 'T' },
   { TI92_BUTTON_U           , 'U' },
   { TI92_BUTTON_V           , 'V' },
   { TI92_BUTTON_W           , 'W' },
   { TI92_BUTTON_X           , 'X' },
   { TI92_BUTTON_Y           , 'Y' },
   { TI92_BUTTON_Z           , 'Z' },
   { TI92_BUTTON_a           , 'a' },
   { TI92_BUTTON_b           , 'b' },
   { TI92_BUTTON_c           , 'c' },
   { TI92_BUTTON_d           , 'd' },
   { TI92_BUTTON_e           , 'e' },
   { TI92_BUTTON_f           , 'f' },
   { TI92_BUTTON_g           , 'g' },
   { TI92_BUTTON_h           , 'h' },
   { TI92_BUTTON_i           , 'i' },
   { TI92_BUTTON_j           , 'j' },
   { TI92_BUTTON_k           , 'k' },
   { TI92_BUTTON_l           , 'l' },
   { TI92_BUTTON_m           , 'm' },
   { TI92_BUTTON_n           , 'n' },
   { TI92_BUTTON_o           , 'o' },
   { TI92_BUTTON_p           , 'p' },
   { TI92_BUTTON_q           , 'q' },
   { TI92_BUTTON_r           , 'r' },
   { TI92_BUTTON_s           , 's' },
   { TI92_BUTTON_t           , 't' },
   { TI92_BUTTON_u           , 'u' },
   { TI92_BUTTON_v           , 'v' },
   { TI92_BUTTON_w           , 'w' },
   { TI92_BUTTON_x           , 'x' },
   { TI92_BUTTON_y           , 'y' },
   { TI92_BUTTON_z           , 'z' },
   { TI92_BUTTON_0           , '0' },
   { TI92_BUTTON_1           , '1' },
   { TI92_BUTTON_2           , '2' },
   { TI92_BUTTON_3           , '3' },
   { TI92_BUTTON_4           , '4' },
   { TI92_BUTTON_5           , '5' },
   { TI92_BUTTON_6           , '6' },
   { TI92_BUTTON_7           , '7' },
   { TI92_BUTTON_8           , '8' },
   { TI92_BUTTON_9           , '9' },
   { TI92_BUTTON_NEGATE      , DANZEFF_NEGATE },
   { TI92_BUTTON_PLUS        , '+' },
   { TI92_BUTTON_MINUS       , '-' },
   { TI92_BUTTON_MULTIPLY    , '*' },
   { TI92_BUTTON_DIVIDE      , '/' },
   { TI92_BUTTON_PERIOD      , '.' },
   { TI92_BUTTON_LN          , DANZEFF_LN  },
   { TI92_BUTTON_SIN         , DANZEFF_SIN },
   { TI92_BUTTON_COS         , DANZEFF_COS },
   { TI92_BUTTON_TAN         , DANZEFF_TAN },
   { TI92_BUTTON_POWER       , '^' },
   { TI92_BUTTON_EXP         , DANZEFF_EXP },
   { TI92_BUTTON_QUIT        , DANZEFF_QUIT },
   { TI92_BUTTON_MOVE_CURSOR , DANZEFF_MOVE_CURSOR },
   { TI92_BUTTON_SIN_INV     , DANZEFF_SIN_INV },
   { TI92_BUTTON_COS_INV     , DANZEFF_COS_INV },
   { TI92_BUTTON_TAN_INV     , DANZEFF_TAN_INV },
   { TI92_BUTTON_PI          , DANZEFF_PI },
   { TI92_BUTTON_LEFT_PARENT , '(' },
   { TI92_BUTTON_RIGHT_PARENT, ')' },
   { TI92_BUTTON_COMMA       , ',' },
   { TI92_BUTTON_SPACE       , ' ' },
   { TI92_BUTTON_BRACE_LEFT  , '[' },
   { TI92_BUTTON_BRACE_RIGHT , ']' },
   { TI92_BUTTON_BRACKET_LEFT, '{' },
   { TI92_BUTTON_BRACKET_RIGHT, '}' },
   { TI92_BUTTON_INTEGRAL   ,  DANZEFF_INTEGRAL },
   { TI92_BUTTON_DERIVATE   ,  DANZEFF_DERIVATE },
   { TI92_BUTTON_INV_X      ,  DANZEFF_INV_X },
   { TI92_BUTTON_SQUARE_ROOT,  DANZEFF_SQUARE_ROOT },
   { TI92_BUTTON_SUM        ,  DANZEFF_SUM },
   { TI92_BUTTON_MATH       ,  DANZEFF_MATH },
   { TI92_BUTTON_MEM        ,  DANZEFF_MEM },
   { TI92_BUTTON_VAR_LINK   ,  DANZEFF_VAR_LINK },
   { TI92_BUTTON_EE         ,  DANZEFF_EE },
   { TI92_BUTTON_CATALOG    ,  DANZEFF_CATALOG },
   { TI92_BUTTON_CUSTOM     ,  DANZEFF_CUSTOM },
   { TI92_BUTTON_CHAR       ,  DANZEFF_CHAR },
   { TI92_BUTTON_LT         ,  '<' },
   { TI92_BUTTON_GT         ,  '>' },
   { TI92_BUTTON_ANS        ,  DANZEFF_ANS },
   { TI92_BUTTON_ENTRY      ,  DANZEFF_ENTRY },
   { TI92_BUTTON_TRIANGLE   ,  DANZEFF_TRIANGLE },
   { TI92_BUTTON_INFO       ,  DANZEFF_INFO },
   { TI92_BUTTON_UNDERSCORE , '_' },
   { TI92_BUTTON_BETA       ,  DANZEFF_BETA },
   { TI92_BUTTON_ANGLE      ,  DANZEFF_ANGLE },
   { TI92_BUTTON_INFINITE   ,  DANZEFF_INFINITE },
   { TI92_BUTTON_PIPE       , '|' },
   { TI92_BUTTON_DBL_QUOTE  , '"' },
   { TI92_BUTTON_CAPS       ,  DANZEFF_CAPS },
   { TI92_BUTTON_QUOTE      , '\'' },
   { TI92_BUTTON_SEMICOLON  , ';' },
   { TI92_BUTTON_COLON      , ':' },
   { TI92_BUTTON_OFF        , DANZEFF_OFF },
   { TI92_BUTTON_RCL        , DANZEFF_RCL },
   { TI92_BUTTON_BACKSLASH  , '\\' },
   { TI92_BUTTON_INS        , DANZEFF_INS },
   { TI92_BUTTON_HOME       , DANZEFF_HOME },
   { TI92_BUTTON_Y_EQ       , DANZEFF_Y_EQ },
   { TI92_BUTTON_WINDOW     , DANZEFF_WINDOW },
   { TI92_BUTTON_GRAPH      , DANZEFF_GRAPH },
   { TI92_BUTTON_TBL_SET    , DANZEFF_TBL_SET },
   { TI92_BUTTON_TABLE      , DANZEFF_TABLE },
   { TI92_BUTTON_UNITS      , DANZEFF_UNITS },
   { TI92_BUTTON_KEY        , DANZEFF_KEY },
   { TI92_BUTTON_DEL        , DANZEFF_DEL },
   { TI92_BUTTON_ENVIRON    , DANZEFF_ENVIRON },
   { TI92_BUTTON_SHIFT      , DANZEFF_SHIFT },
   { TI92_BUTTON_ON         , DANZEFF_ON },
   { TI92_BUTTON_DIAMOND    , DANZEFF_DIAMOND },
   { TI92_BUTTON_2ND        , DANZEFF_2ND },
   { TI92_BUTTON_MODE       , DANZEFF_MODE },
   { TI92_BUTTON_CLEAR      , DANZEFF_CLEAR },
   { TI92_BUTTON_THETA      , DANZEFF_THETA },
   { TI92_BUTTON_STORE      , DANZEFF_STORE },
   { TI92_BUTTON_EQUAL      , '=' },
   { TI92_BUTTON_BACKSPACE  , DANZEFF_BACKSPACE },
   { TI92_BUTTON_ENTER1     , DANZEFF_ENTER1 },
   { TI92_BUTTON_ENTER2     , DANZEFF_ENTER2 },
   { TI92_BUTTON_ESCAPE     , DANZEFF_ESCAPE },
   { TI92_BUTTON_APPS       , DANZEFF_APPS  },    
   { TI92_BUTTON_LEFT       , DANZEFF_LEFT  },    
   { TI92_BUTTON_DOWN       , DANZEFF_DOWN  },    
   { TI92_BUTTON_UP         , DANZEFF_UP    },    
   { TI92_BUTTON_RIGHT      , DANZEFF_RIGHT },    
   { TI92_BUTTON_F1         , DANZEFF_F1    },    
   { TI92_BUTTON_F2         , DANZEFF_F2    },    
   { TI92_BUTTON_F3         , DANZEFF_F3    },    
   { TI92_BUTTON_F4         , DANZEFF_F4    },    
   { TI92_BUTTON_F5         , DANZEFF_F5    },    
   { TI92_BUTTON_F6         , DANZEFF_F6    },    
   { TI92_BUTTON_F7         , DANZEFF_F7    },    
   { TI92_BUTTON_F8         , DANZEFF_F8    },    
   { TI92_BUTTON_HAND       , DANZEFF_HAND  }    

  };

  ti92_button_t ti92_buttons_info[ TI92_LAST_BUTTON ] =  
  {
    { "A"          , TIKEY_A,         TI92_META_SHIFT },
    { "B"          , TIKEY_B,         TI92_META_SHIFT },
    { "C"          , TIKEY_C,         TI92_META_SHIFT },
    { "D"          , TIKEY_D,         TI92_META_SHIFT },
    { "E"          , TIKEY_E,         TI92_META_SHIFT },
    { "F"          , TIKEY_F,         TI92_META_SHIFT },
    { "G"          , TIKEY_G,         TI92_META_SHIFT },
    { "H"          , TIKEY_H,         TI92_META_SHIFT },
    { "I"          , TIKEY_I,         TI92_META_SHIFT },
    { "J"          , TIKEY_J,         TI92_META_SHIFT },
    { "K"          , TIKEY_K,         TI92_META_SHIFT },
    { "L"          , TIKEY_L,         TI92_META_SHIFT },
    { "M"          , TIKEY_M,         TI92_META_SHIFT },
    { "N"          , TIKEY_N,         TI92_META_SHIFT },
    { "O"          , TIKEY_O,         TI92_META_SHIFT },
    { "P"          , TIKEY_P,         TI92_META_SHIFT },
    { "Q"          , TIKEY_Q,         TI92_META_SHIFT },
    { "R"          , TIKEY_R,         TI92_META_SHIFT },
    { "S"          , TIKEY_S,         TI92_META_SHIFT },
    { "T"          , TIKEY_T,         TI92_META_SHIFT },
    { "U"          , TIKEY_U,         TI92_META_SHIFT },
    { "V"          , TIKEY_V,         TI92_META_SHIFT },
    { "W"          , TIKEY_W,         TI92_META_SHIFT },
    { "X"          , TIKEY_X,         TI92_META_SHIFT },
    { "Y"          , TIKEY_Y,         TI92_META_SHIFT },
    { "Z"          , TIKEY_Z,         TI92_META_SHIFT },
    { "a"          , TIKEY_A,         TI92_META_NONE },
    { "b"          , TIKEY_B,         TI92_META_NONE },
    { "c"          , TIKEY_C,         TI92_META_NONE },
    { "d"          , TIKEY_D,         TI92_META_NONE },
    { "e"          , TIKEY_E,         TI92_META_NONE },
    { "f"          , TIKEY_F,         TI92_META_NONE },
    { "g"          , TIKEY_G,         TI92_META_NONE },
    { "h"          , TIKEY_H,         TI92_META_NONE },
    { "i"          , TIKEY_I,         TI92_META_NONE },
    { "j"          , TIKEY_J,         TI92_META_NONE },
    { "k"          , TIKEY_K,         TI92_META_NONE },
    { "l"          , TIKEY_L,         TI92_META_NONE },
    { "m"          , TIKEY_M,         TI92_META_NONE },
    { "n"          , TIKEY_N,         TI92_META_NONE },
    { "o"          , TIKEY_O,         TI92_META_NONE },
    { "p"          , TIKEY_P,         TI92_META_NONE },
    { "q"          , TIKEY_Q,         TI92_META_NONE },
    { "r"          , TIKEY_R,         TI92_META_NONE },
    { "s"          , TIKEY_S,         TI92_META_NONE },
    { "t"          , TIKEY_T,         TI92_META_NONE },
    { "u"          , TIKEY_U,         TI92_META_NONE },
    { "v"          , TIKEY_V,         TI92_META_NONE },
    { "w"          , TIKEY_W,         TI92_META_NONE },
    { "x"          , TIKEY_X,         TI92_META_NONE },
    { "y"          , TIKEY_Y,         TI92_META_NONE },
    { "z"          , TIKEY_Z,         TI92_META_NONE },
    { "0"          , TIKEY_0,         TI92_META_NONE },
    { "1"          , TIKEY_1,         TI92_META_NONE },
    { "2"          , TIKEY_2,         TI92_META_NONE },
    { "3"          , TIKEY_3,         TI92_META_NONE },
    { "4"          , TIKEY_4,         TI92_META_NONE },
    { "5"          , TIKEY_5,         TI92_META_NONE },
    { "6"          , TIKEY_6,         TI92_META_NONE },
    { "7"          , TIKEY_7,         TI92_META_NONE },
    { "8"          , TIKEY_8,         TI92_META_NONE },
    { "9"          , TIKEY_9,         TI92_META_NONE },
    { "(-)"        , TIKEY_NEGATE,    TI92_META_NONE },
    { "+"          , TIKEY_PLUS,      TI92_META_NONE },
    { "-"          , TIKEY_MINUS,     TI92_META_NONE },
    { "*"          , TIKEY_MULTIPLY,  TI92_META_NONE },
    { "/"          , TIKEY_DIVIDE,    TI92_META_NONE },
    { "."          , TIKEY_PERIOD,    TI92_META_NONE },
    { "LN"         , TIKEY_LN,        TI92_META_NONE },
    { "SIN"        , TIKEY_SIN,       TI92_META_NONE },
    { "COS"        , TIKEY_COS,       TI92_META_NONE },
    { "TAN"        , TIKEY_TAN,       TI92_META_NONE },
    { "POWER"      , TIKEY_POWER,     TI92_META_NONE },
    { "EXP"        , TIKEY_LN,        TI92_META_2ND  },
    { "QUIT"       , TIKEY_ESCAPE,    TI92_META_2ND  },
    { "MOVE_CURSOR", TIKEY_APPS,      TI92_META_2ND  },
    { "SIN_INV"    , TIKEY_SIN,       TI92_META_2ND  },
    { "COS_INV"    , TIKEY_COS,       TI92_META_2ND  },
    { "TAN_INV"    , TIKEY_TAN,       TI92_META_2ND  },
    { "PI"         , TIKEY_POWER,     TI92_META_2ND  },
    { "("          , TIKEY_PALEFT,    TI92_META_NONE },
    { ")"          , TIKEY_PARIGHT,   TI92_META_NONE },
    { ","          , TIKEY_COMMA,     TI92_META_NONE },
    { "SPACE"      , TIKEY_SPACE,     TI92_META_NONE },
    { "["          , TIKEY_PALEFT,    TI92_META_2ND  },
    { "]"          , TIKEY_PARIGHT,   TI92_META_2ND  },
    { "{"          , TIKEY_COMMA,     TI92_META_2ND  },
    { "}"          , TIKEY_DIVIDE,    TI92_META_2ND  },
    { "INTEGRAL"   , TIKEY_7,         TI92_META_2ND  },
    { "DERIVATE"   , TIKEY_8,         TI92_META_2ND  },
    { "INV_X"      , TIKEY_9,         TI92_META_2ND  },
    { "SQUARE_ROOT", TIKEY_MULTIPLY,  TI92_META_2ND  },
    { "SUM"       ,  TIKEY_4,         TI92_META_2ND  },
    { "MATH"      ,  TIKEY_5,         TI92_META_2ND  },
    { "MEM"       ,  TIKEY_6,         TI92_META_2ND  },
    { "VAR_LINK"  ,  TIKEY_MINUS,     TI92_META_2ND  },
    { "EE"        ,  TIKEY_1,         TI92_META_2ND  },
    { "CATALOG"   ,  TIKEY_2,         TI92_META_2ND  },
    { "CUSTOM"    ,  TIKEY_3,         TI92_META_2ND  },
    { "CHAR"      ,  TIKEY_PLUS,      TI92_META_2ND  },
    { "<"         ,  TIKEY_0,         TI92_META_2ND  },
    { ">"         ,  TIKEY_PERIOD,    TI92_META_2ND  },
    { "ANS"       ,  TIKEY_NEGATE,    TI92_META_2ND  },
    { "ENTRY"     ,  TIKEY_ENTER2,    TI92_META_2ND  },
    { "TRIANGLE"  ,  TIKEY_Y,         TI92_META_2ND  },
    { "INFO"      ,  TIKEY_I,         TI92_META_2ND  },
    { "_"         ,  TIKEY_P,         TI92_META_2ND  },
    { "BETA"      ,  TIKEY_D,         TI92_META_2ND  },
    { "ANGLE"     ,  TIKEY_F,         TI92_META_2ND  },
    { "INFINITE"  ,  TIKEY_J,         TI92_META_2ND  },
    { "|"         ,  TIKEY_K,         TI92_META_2ND  },
    { "\""        ,  TIKEY_L,         TI92_META_2ND  },
    { "CAPS"      ,  TIKEY_Z,         TI92_META_2ND  },
    { "'"         ,  TIKEY_B,         TI92_META_2ND  },
    { ";"         ,  TIKEY_M,         TI92_META_2ND  },
    { ":"         ,  TIKEY_THETA,     TI92_META_2ND  },
    { "OFF"       ,  TIKEY_ON,        TI92_META_2ND  },
    { "RCL"       ,  TIKEY_STORE,     TI92_META_2ND  },
    { "\\"        ,  TIKEY_EQUALS,    TI92_META_2ND  },
    { "INS"       ,  TIKEY_BACKSPACE, TI92_META_2ND  },
    /* Diamon keys */
    { "HOME"      ,  TIKEY_Q,         TI92_META_DIAMOND },
    { "Y="        ,  TIKEY_W,         TI92_META_DIAMOND },
    { "WINDOW"    ,  TIKEY_E,         TI92_META_DIAMOND },
    { "GRAPH"     ,  TIKEY_R,         TI92_META_DIAMOND },
    { "TBL_SET"   ,  TIKEY_T,         TI92_META_DIAMOND },
    { "TABLE"     ,  TIKEY_Y,         TI92_META_DIAMOND },
    { "UNITS"     ,  TIKEY_P,         TI92_META_DIAMOND },
    { "KEY"       ,  TIKEY_K,         TI92_META_DIAMOND },
    { "DEL"       ,  TIKEY_BACKSPACE, TI92_META_DIAMOND },
    { "ENVIRON"   ,  TIKEY_ENTER1,    TI92_META_DIAMOND },

    { "SHIFT"     ,  TIKEY_SHIFT,     TI92_META_NONE },
    { "ON"        ,  TIKEY_ON,        TI92_META_NONE },
    { "DIAMOND"   ,  TIKEY_DIAMOND,   TI92_META_NONE },
    { "2ND"       ,  TIKEY_2ND,       TI92_META_NONE },
    { "MODE"      ,  TIKEY_MODE,      TI92_META_NONE },
    { "CLEAR"     ,  TIKEY_CLEAR,     TI92_META_NONE },
    { "THETA"     ,  TIKEY_THETA,     TI92_META_NONE },
    { "STORE"     ,  TIKEY_STORE,     TI92_META_NONE },
    { "="         ,  TIKEY_EQUALS,    TI92_META_NONE },
    { "BACKSPACE" ,  TIKEY_BACKSPACE, TI92_META_NONE },
    { "ENTER1"    ,  TIKEY_ENTER1,   TI92_META_NONE },
    { "ENTER2"    ,  TIKEY_ENTER2,   TI92_META_NONE },
    { "ESCAPE"    ,  TIKEY_ESCAPE,   TI92_META_NONE },
    { "APPS"      ,  TIKEY_APPS,     TI92_META_NONE },
    { "LEFT"      ,  TIKEY_LEFT,     TI92_META_NONE },
    { "DOWN"      ,  TIKEY_DOWN,     TI92_META_NONE },
    { "UP"        ,  TIKEY_UP,       TI92_META_NONE },
    { "RIGHT"     ,  TIKEY_RIGHT,    TI92_META_NONE },
    { "F1"        ,  TIKEY_F1,       TI92_META_NONE },
    { "F2"        ,  TIKEY_F2,       TI92_META_NONE },
    { "F3"        ,  TIKEY_F3,       TI92_META_NONE },
    { "F4"        ,  TIKEY_F4,       TI92_META_NONE },
    { "F5"        ,  TIKEY_F5,       TI92_META_NONE },
    { "F6"        ,  TIKEY_F6,       TI92_META_NONE },
    { "F7"        ,  TIKEY_F7,       TI92_META_NONE },
    { "F8"        ,  TIKEY_F8,       TI92_META_NONE },
    { "HAND"      ,  TIKEY_HAND,     TI92_META_NONE },
  };

 int psp_kbd_mapping[ KBD_ALL_BUTTONS ];
 int psp_kbd_mapping_L[ KBD_ALL_BUTTONS ];
 int psp_kbd_mapping_R[ KBD_ALL_BUTTONS ];
 int psp_kbd_presses[ KBD_ALL_BUTTONS ];
 int kbd_ltrigger_mapping_active;
 int kbd_rtrigger_mapping_active;

 static int danzeff_ti92_key     = 0;
 static int danzeff_ti92_pending = 0;
 static int danzeff_mode        = 0;

#ifdef USE_PSP_IRKEYB
 static int irkeyb_ti92_key     = 0;
 static int irkeyb_ti92_pending = 0;
#endif

  /* Keystates for TI92 keys */
  UBYTE keyStates[120];
  volatile int keyWasPressed;

int
ti92_key_event(int b, int button_pressed)
{
  int ti_key;
  int meta_key;

  ti_key = ti92_buttons_info[b].code;
  meta_key = ti92_buttons_info[b].meta;
  if (button_pressed) {
    keyStates[ti_key] = 1;
    if (meta_key != TI92_META_NONE) {
      keyStates[meta_key] = 1;
    }
  } else {
    keyStates[ti_key] = 0;
    if (meta_key != TI92_META_NONE) {
      keyStates[meta_key] = 0;
    }
  }
  return 0;
}

int
ti92_get_key_from_ascii(int key_ascii)
{
  int index;
  for (index = 0; index < KBD_MAX_ENTRIES; index++) {
   if (kbd_layout[index][1] == key_ascii) return kbd_layout[index][0];
  }
  return -1;
}

int
psp_kbd_reset_mapping(void)
{
  memcpy(psp_kbd_mapping, loc_default_mapping, sizeof(loc_default_mapping));
  memcpy(psp_kbd_mapping_L, loc_default_mapping_L, sizeof(loc_default_mapping_L));
  memcpy(psp_kbd_mapping_R, loc_default_mapping_R, sizeof(loc_default_mapping_R));
  return 0;
}

int
psp_kbd_load_mapping(char *kbd_filename)
{
  FILE    *KbdFile;
  int      error = 0;

  KbdFile = fopen(kbd_filename, "r");
  error   = 1;

  if (KbdFile != (FILE*)0) {
    psp_kbd_load_mapping_file(KbdFile);
    error = 0;
    fclose(KbdFile);
  }

  kbd_ltrigger_mapping_active = 0;
  kbd_rtrigger_mapping_active = 0;
    
  return error;
}

int
psp_kbd_load_mapping_file(FILE *KbdFile)
{
  char     Buffer[512];
  char    *Scan;
  int      tmp_mapping[KBD_ALL_BUTTONS];
  int      tmp_mapping_L[KBD_ALL_BUTTONS];
  int      tmp_mapping_R[KBD_ALL_BUTTONS];
  int      ti92_key_id = 0;
  int      kbd_id = 0;

  memcpy(tmp_mapping, loc_default_mapping, sizeof(loc_default_mapping));
  memcpy(tmp_mapping_L, loc_default_mapping_L, sizeof(loc_default_mapping_R));
  memcpy(tmp_mapping_R, loc_default_mapping_R, sizeof(loc_default_mapping_R));

  while (fgets(Buffer,512,KbdFile) != (char *)0) {
      
      Scan = strchr(Buffer,'\n');
      if (Scan) *Scan = '\0';
      /* For this #@$% of windows ! */
      Scan = strchr(Buffer,'\r');
      if (Scan) *Scan = '\0';
      if (Buffer[0] == '#') continue;

      Scan = strchr(Buffer,'=');
      if (! Scan) continue;
    
      *Scan = '\0';
      ti92_key_id = atoi(Scan + 1);

      for (kbd_id = 0; kbd_id < KBD_ALL_BUTTONS; kbd_id++) {
        if (!strcasecmp(Buffer,loc_button_name[kbd_id])) {
          tmp_mapping[kbd_id] = ti92_key_id;
          //break;
        }
      }
      for (kbd_id = 0; kbd_id < KBD_ALL_BUTTONS; kbd_id++) {
        if (!strcasecmp(Buffer,loc_button_name_L[kbd_id])) {
          tmp_mapping_L[kbd_id] = ti92_key_id;
          //break;
        }
      }
      for (kbd_id = 0; kbd_id < KBD_ALL_BUTTONS; kbd_id++) {
        if (!strcasecmp(Buffer,loc_button_name_R[kbd_id])) {
          tmp_mapping_R[kbd_id] = ti92_key_id;
          //break;
        }
      }
  }

  memcpy(psp_kbd_mapping, tmp_mapping, sizeof(psp_kbd_mapping));
  memcpy(psp_kbd_mapping_L, tmp_mapping_L, sizeof(psp_kbd_mapping_L));
  memcpy(psp_kbd_mapping_R, tmp_mapping_R, sizeof(psp_kbd_mapping_R));
  
  return 0;
}

int
psp_kbd_save_mapping(char *kbd_filename)
{
  FILE    *KbdFile;
  int      kbd_id = 0;
  int      error = 0;

  KbdFile = fopen(kbd_filename, "w");
  error   = 1;

  if (KbdFile != (FILE*)0) {

    for (kbd_id = 0; kbd_id < KBD_ALL_BUTTONS; kbd_id++)
    {
      fprintf(KbdFile, "%s=%d\n", loc_button_name[kbd_id], psp_kbd_mapping[kbd_id]);
    }
    for (kbd_id = 0; kbd_id < KBD_ALL_BUTTONS; kbd_id++)
    {
      fprintf(KbdFile, "%s=%d\n", loc_button_name_L[kbd_id], psp_kbd_mapping_L[kbd_id]);
    }
    for (kbd_id = 0; kbd_id < KBD_ALL_BUTTONS; kbd_id++)
    {
      fprintf(KbdFile, "%s=%d\n", loc_button_name_R[kbd_id], psp_kbd_mapping_R[kbd_id]);
    }
    error = 0;
    fclose(KbdFile);
  }

  return error;
}

int 
psp_kbd_is_danzeff_mode()
{
  return danzeff_mode;
}

int
psp_kbd_enter_danzeff()
{
  unsigned int danzeff_key = 0;
  int          ti92_key     = 0;
  SceCtrlData  c;

  if (! danzeff_mode) {
    psp_init_keyboard();
    danzeff_mode = 1;
  }

  sceCtrlPeekBufferPositive(&c, 1);
  c.Buttons &= PSP_ALL_BUTTON_MASK;

  if (danzeff_ti92_pending) 
  {
    if ((c.TimeStamp - loc_last_event_time) > KBD_MIN_PENDING_TIME) {
      loc_last_event_time = c.TimeStamp;
      danzeff_ti92_pending = 0;
      ti92_key_event(danzeff_ti92_key, 0);
    }
    return 0;
  }

  if ((c.TimeStamp - loc_last_event_time) > KBD_MIN_DANZEFF_TIME) {
    loc_last_event_time = c.TimeStamp;
  
    sceCtrlPeekBufferPositive(&c, 1);
    c.Buttons &= PSP_ALL_BUTTON_MASK;
# ifdef USE_PSP_IRKEYB
    psp_irkeyb_set_psp_key(&c);
# endif
    danzeff_key = danzeff_readInput(c);
  }

  if (danzeff_key == DANZEFF_LEFT) {
    danzeff_key = DANZEFF_DEL;
  } else if (danzeff_key == DANZEFF_DOWN) {
    danzeff_key = DANZEFF_ENTER1;
  } else if (danzeff_key == DANZEFF_RIGHT) {
  } else if (danzeff_key == DANZEFF_UP) {
  }

  if (danzeff_key > DANZEFF_START) {
    ti92_key = ti92_get_key_from_ascii(danzeff_key);

    if (ti92_key != -1) {
      danzeff_ti92_key     = ti92_key;
      danzeff_ti92_pending = 1;
      ti92_key_event(danzeff_ti92_key, 1);
    }

  } else if (danzeff_key == DANZEFF_START) {
    danzeff_mode       = 0;
    danzeff_ti92_pending = 0;
    danzeff_ti92_key     = 0;

    psp_kbd_wait_no_button();

  } else if (danzeff_key == DANZEFF_SELECT) {
    danzeff_mode       = 0;
    danzeff_ti92_pending = 0;
    danzeff_ti92_key     = 0;
    psp_main_menu();
    psp_init_keyboard();

    psp_kbd_wait_no_button();
  }

  return 0;
}

#ifdef USE_PSP_IRKEYB
int
psp_kbd_enter_irkeyb()
{
  int ti92_key   = 0;
  int psp_irkeyb = PSP_IRKEYB_EMPTY;

  SceCtrlData  c;
  sceCtrlPeekBufferPositive(&c, 1);

  if (irkeyb_ti92_pending) 
  {
    if ((c.TimeStamp - loc_last_irkbd_event_time) > KBD_MIN_PENDING_TIME) {
      loc_last_irkbd_event_time = c.TimeStamp;
      irkeyb_ti92_pending = 0;
      ti92_key_event(irkeyb_ti92_key, 0);
    }
    return 0;
  }

  psp_irkeyb = psp_irkeyb_read_key();
  if (psp_irkeyb != PSP_IRKEYB_EMPTY) {

    if (psp_irkeyb == 0x8) {
      ti92_key = TI92_BUTTON_BACKSPACE;
    } else
    if (psp_irkeyb == 0xd) {
      ti92_key = TI92_BUTTON_ENTER1;
    } else
    if (psp_irkeyb == 0x1b) {
      ti92_key = TI92_BUTTON_ESCAPE;
    } else
    if ((psp_irkeyb >= 1) && (psp_irkeyb < 8)) {
      ti92_key = psp_irkeyb + TI92_BUTTON_F1 - 1;
    } else
    /* Ctrl-O */
    if (psp_irkeyb == 15) {
      ti92_key = TI92_BUTTON_DIAMOND;
    } else
    /* Ctrl-P */
    if (psp_irkeyb == 16) {
      ti92_key = TI92_BUTTON_2ND;
    } else
    /* Ctrl-K */
    if (psp_irkeyb == 11) {
      ti92_key = TI92_BUTTON_HAND;
    } else
    /* Ctrl-L */
    if (psp_irkeyb == 12) {
      ti92_key = TI92_BUTTON_HOME;
    } else
    if (psp_irkeyb == PSP_IRKEYB_UP) {
      ti92_key = TI92_BUTTON_UP;
    } else
    if (psp_irkeyb == PSP_IRKEYB_DOWN) {
      ti92_key = TI92_BUTTON_DOWN;
    } else
    if (psp_irkeyb == PSP_IRKEYB_LEFT) {
      ti92_key = TI92_BUTTON_LEFT;
    } else
    if (psp_irkeyb == PSP_IRKEYB_RIGHT) {
      ti92_key = TI92_BUTTON_RIGHT;
    } else {
      ti92_key = ti92_get_key_from_ascii(psp_irkeyb);
    }
    if (ti92_key != -1) {
      irkeyb_ti92_key     = ti92_key;
      irkeyb_ti92_pending = 1;
      ti92_key_event(ti92_key, 1);
    }
    return 1;
  }
  return 0;
}
# endif

int
ti92_decode_key(int psp_b, int button_pressed)
{
  int wake = 0;
  int reverse_analog = TI92.psp_reverse_analog;

  if (reverse_analog) {
    if ((psp_b >= KBD_JOY_UP  ) &&
        (psp_b <= KBD_JOY_LEFT)) {
      psp_b = psp_b - KBD_JOY_UP + KBD_UP;
    } else
    if ((psp_b >= KBD_UP  ) &&
        (psp_b <= KBD_LEFT)) {
      psp_b = psp_b - KBD_UP + KBD_JOY_UP;
    }
  }

  if (psp_b == KBD_START) {
     if (button_pressed) psp_kbd_enter_danzeff();
  } else
  if (psp_b == KBD_SELECT) {
    if (button_pressed) {
      psp_main_menu();
      psp_init_keyboard();
    }
  } else {
 
    if (psp_kbd_mapping[psp_b] >= 0) {
      wake = 1;
      if (button_pressed) {
        // Determine which buton to press first (ie which mapping is currently active)
        if (kbd_ltrigger_mapping_active) {
          // Use ltrigger mapping
          psp_kbd_presses[psp_b] = psp_kbd_mapping_L[psp_b];
          ti92_key_event(psp_kbd_presses[psp_b], button_pressed);
        } else
        if (kbd_rtrigger_mapping_active) {
          // Use rtrigger mapping
          psp_kbd_presses[psp_b] = psp_kbd_mapping_R[psp_b];
          ti92_key_event(psp_kbd_presses[psp_b], button_pressed);
        } else {
          // Use standard mapping
          psp_kbd_presses[psp_b] = psp_kbd_mapping[psp_b];
          ti92_key_event(psp_kbd_presses[psp_b], button_pressed);
        }
      } else {
          // Determine which button to release (ie what was pressed before)
          ti92_key_event(psp_kbd_presses[psp_b], button_pressed);
      }

    } else {
      if (psp_kbd_mapping[psp_b] == KBD_LTRIGGER_MAPPING) {
        kbd_ltrigger_mapping_active = button_pressed;
        kbd_rtrigger_mapping_active = 0;
      } else
      if (psp_kbd_mapping[psp_b] == KBD_RTRIGGER_MAPPING) {
        kbd_rtrigger_mapping_active = button_pressed;
        kbd_ltrigger_mapping_active = 0;
      }
    }
  }
  return 0;
}

# define ANALOG_THRESHOLD 60

void 
kbd_get_analog_direction(int Analog_x, int Analog_y, int *x, int *y)
{
  int DeltaX = 255;
  int DeltaY = 255;
  int DirX   = 0;
  int DirY   = 0;

  *x = 0;
  *y = 0;

  if (Analog_x <=        ANALOG_THRESHOLD)  { DeltaX = Analog_x; DirX = -1; }
  else 
  if (Analog_x >= (255 - ANALOG_THRESHOLD)) { DeltaX = 255 - Analog_x; DirX = 1; }

  if (Analog_y <=        ANALOG_THRESHOLD)  { DeltaY = Analog_y; DirY = -1; }
  else 
  if (Analog_y >= (255 - ANALOG_THRESHOLD)) { DeltaY = 255 - Analog_y; DirY = 1; }

  *x = DirX;
  *y = DirY;
}

int
kbd_scan_keyboard(void)
{
  SceCtrlData c;
  long        delta_stamp;
  int         event;
  int         b;

  int new_Lx;
  int new_Ly;
  int old_Lx;
  int old_Ly;

  event = 0;
  myCtrlPeekBufferPositive( &c, 1 );
  c.Buttons &= PSP_ALL_BUTTON_MASK;

# ifdef USE_PSP_IRKEYB
  psp_irkeyb_set_psp_key(&c);
# endif

  if ((c.Buttons & (PSP_CTRL_LTRIGGER|PSP_CTRL_RTRIGGER|PSP_CTRL_START)) ==
      (PSP_CTRL_LTRIGGER|PSP_CTRL_RTRIGGER|PSP_CTRL_START)) {
    /* Exit ! */
    psp_sdl_exit(0);
  }

  delta_stamp = c.TimeStamp - first_time_batt_stamp;
  if ((delta_stamp < 0) || (delta_stamp > KBD_MIN_BATTCHECK_TIME)) {
    first_time_batt_stamp = c.TimeStamp;
    if (psp_is_low_battery()) {
      psp_main_menu();
      psp_init_keyboard();
      return 0;
    }
  }

  /* Check Analog Device */
  kbd_get_analog_direction(loc_button_data.Lx,loc_button_data.Ly,&old_Lx,&old_Ly);
  kbd_get_analog_direction( c.Lx, c.Ly, &new_Lx, &new_Ly);

  /* Analog device has moved */
  if (new_Lx > 0) {
    if (old_Lx <  0) ti92_decode_key(KBD_JOY_LEFT , 0);
    if (old_Lx <= 0) ti92_decode_key(KBD_JOY_RIGHT, 1);
  } else 
  if (new_Lx < 0) {
    if (old_Lx >  0) ti92_decode_key(KBD_JOY_RIGHT, 0);
    if (old_Lx >= 0) ti92_decode_key(KBD_JOY_LEFT , 1);
  } else {
    if (old_Lx >  0) ti92_decode_key(KBD_JOY_RIGHT , 0);
    else
    if (old_Lx <  0) ti92_decode_key(KBD_JOY_LEFT, 0);
  }

  if (new_Ly > 0) {
    if (old_Ly <  0) ti92_decode_key(KBD_JOY_UP , 0);
    if (old_Ly <= 0) ti92_decode_key(KBD_JOY_DOWN, 1);
  } else 
  if (new_Ly < 0) {
    if (old_Ly >  0) ti92_decode_key(KBD_JOY_DOWN, 0);
    if (old_Ly >= 0) ti92_decode_key(KBD_JOY_UP , 1);
  } else {
    if (old_Ly >  0) ti92_decode_key(KBD_JOY_DOWN , 0);
    else
    if (old_Ly <  0) ti92_decode_key(KBD_JOY_UP, 0);
  }

  for (b = 0; b < KBD_MAX_BUTTONS; b++) 
  {
    if (c.Buttons & loc_button_mask[b]) {
      if (!(loc_button_data.Buttons & loc_button_mask[b])) {
        loc_button_press[b] = 1;
        event = 1;
      }
    } else {
      if (loc_button_data.Buttons & loc_button_mask[b]) {
        loc_button_release[b] = 1;
        loc_button_press[b] = 0;
        event = 1;
      }
    }
  }
  memcpy(&loc_button_data,&c,sizeof(SceCtrlData));

  return event;
}

void
kbd_wait_start(void)
{
  while (1)
  {
    SceCtrlData c;
    sceCtrlPeekBufferPositive(&c, 1);
    c.Buttons &= PSP_ALL_BUTTON_MASK;
    if (c.Buttons & PSP_CTRL_START) break;
  }
  psp_kbd_wait_no_button();
}

void
psp_init_keyboard(void)
{
  int i;
  for(i=0; i<120; i++) {
    keyStates[i] = 0;
  }
  kbd_ltrigger_mapping_active = 0;
  kbd_rtrigger_mapping_active = 0;
}

int
psp_is_key_pressed(int key)
{
  return keyStates[key];
}

void
psp_kbd_wait_no_button(void)
{
  SceCtrlData c;

  do {
   myCtrlPeekBufferPositive(&c, 1);
   c.Buttons &= PSP_ALL_BUTTON_MASK;

  } while (c.Buttons != 0);
} 

void
psp_kbd_wait_button(void)
{
  SceCtrlData c;

  do {
   sceCtrlPeekBufferPositive(&c, 1);
   c.Buttons &= PSP_ALL_BUTTON_MASK;
  } while (c.Buttons == 0);
} 

int
psp_update_keys(void)
{
  int         b;
  int         wake = 0;

  static char first_time = 1;
  static long first_time_stamp = -1;
  static int release_pending = 0;
  static int release_button  = 0;

  if (first_time) {

    memcpy(psp_kbd_mapping, loc_default_mapping, sizeof(loc_default_mapping));
    memcpy(psp_kbd_mapping_L, loc_default_mapping_L, sizeof(loc_default_mapping_L));
    memcpy(psp_kbd_mapping_R, loc_default_mapping_R, sizeof(loc_default_mapping_R));

    ti92_kbd_load();

    SceCtrlData c;
    sceCtrlPeekBufferPositive(&c, 1);
    c.Buttons &= PSP_ALL_BUTTON_MASK;

    if (first_time_stamp == -1) first_time_stamp = c.TimeStamp;
    if ((! c.Buttons) && ((c.TimeStamp - first_time_stamp) < KBD_MIN_START_TIME)) return 0;

    first_time      = 0;
    release_pending = 0;

    for (b = 0; b < KBD_MAX_BUTTONS; b++) {
      loc_button_release[b] = 0;
      loc_button_press[b] = 0;
    }
    sceCtrlPeekBufferPositive(&loc_button_data, 1);
    loc_button_data.Buttons &= PSP_ALL_BUTTON_MASK;

    psp_main_menu();
    psp_init_keyboard();

    return 0;
  }

  if (danzeff_mode) {
    return psp_kbd_enter_danzeff();
  }

# ifdef USE_PSP_IRKEYB
  if (psp_kbd_enter_irkeyb()) {
    return 1;
  }
# endif


  if (release_pending)
  {
    release_pending = 0;
    for (b = 0; b < KBD_MAX_BUTTONS; b++) {
      if (loc_button_release[b]) {
        loc_button_release[b] = 0;
        loc_button_press[b] = 0;
        wake |= ti92_decode_key(b, 0);
      }
    }
  }

  kbd_scan_keyboard();

  /* check press event */
  for (b = 0; b < KBD_MAX_BUTTONS; b++) {
    if (loc_button_press[b]) {
      loc_button_press[b] = 0;
      release_pending     = 0;
      wake |= ti92_decode_key(b, 1);
    }
  }
  /* check release event */
  for (b = 0; b < KBD_MAX_BUTTONS; b++) {
    if (loc_button_release[b]) {
      release_pending = 1;
      break;
    }
  }

  return wake;
}
