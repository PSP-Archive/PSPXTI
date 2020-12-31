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

# ifndef _KBD_H_
# define _KBD_H_

# define PSP_ALL_BUTTON_MASK 0xFFFF

# define KBD_UP           0
# define KBD_RIGHT        1
# define KBD_DOWN         2
# define KBD_LEFT         3
# define KBD_TRIANGLE     4
# define KBD_CIRCLE       5
# define KBD_CROSS        6
# define KBD_SQUARE       7
# define KBD_SELECT       8
# define KBD_START        9
# define KBD_HOME        10
# define KBD_HOLD        11
# define KBD_LTRIGGER    12
# define KBD_RTRIGGER    13

# define KBD_MAX_BUTTONS 14

# define KBD_JOY_UP      14
# define KBD_JOY_RIGHT   15
# define KBD_JOY_DOWN    16
# define KBD_JOY_LEFT    17

# define KBD_ALL_BUTTONS 18

# define KBD_UNASSIGNED         -1

# define KBD_LTRIGGER_MAPPING   -2
# define KBD_RTRIGGER_MAPPING   -3
# define KBD_NORMAL_MAPPING     -1

  enum ti92_button_id {

    TI92_BUTTON_A           ,
    TI92_BUTTON_B           ,
    TI92_BUTTON_C           ,
    TI92_BUTTON_D           ,
    TI92_BUTTON_E           ,
    TI92_BUTTON_F           ,
    TI92_BUTTON_G           ,
    TI92_BUTTON_H           ,
    TI92_BUTTON_I           ,
    TI92_BUTTON_J           ,
    TI92_BUTTON_K           ,
    TI92_BUTTON_L           ,
    TI92_BUTTON_M           ,
    TI92_BUTTON_N           ,
    TI92_BUTTON_O           ,
    TI92_BUTTON_P           ,
    TI92_BUTTON_Q           ,
    TI92_BUTTON_R           ,
    TI92_BUTTON_S           ,
    TI92_BUTTON_T           ,
    TI92_BUTTON_U           ,
    TI92_BUTTON_V           ,
    TI92_BUTTON_W           ,
    TI92_BUTTON_X           ,
    TI92_BUTTON_Y           ,
    TI92_BUTTON_Z           ,
    TI92_BUTTON_a           ,
    TI92_BUTTON_b           ,
    TI92_BUTTON_c           ,
    TI92_BUTTON_d           ,
    TI92_BUTTON_e           ,
    TI92_BUTTON_f           ,
    TI92_BUTTON_g           ,
    TI92_BUTTON_h           ,
    TI92_BUTTON_i           ,
    TI92_BUTTON_j           ,
    TI92_BUTTON_k           ,
    TI92_BUTTON_l           ,
    TI92_BUTTON_m           ,
    TI92_BUTTON_n           ,
    TI92_BUTTON_o           ,
    TI92_BUTTON_p           ,
    TI92_BUTTON_q           ,
    TI92_BUTTON_r           ,
    TI92_BUTTON_s           ,
    TI92_BUTTON_t           ,
    TI92_BUTTON_u           ,
    TI92_BUTTON_v           ,
    TI92_BUTTON_w           ,
    TI92_BUTTON_x           ,
    TI92_BUTTON_y           ,
    TI92_BUTTON_z           ,
    TI92_BUTTON_0           ,
    TI92_BUTTON_1           ,
    TI92_BUTTON_2           ,
    TI92_BUTTON_3           ,
    TI92_BUTTON_4           ,
    TI92_BUTTON_5           ,
    TI92_BUTTON_6           ,
    TI92_BUTTON_7           ,
    TI92_BUTTON_8           ,
    TI92_BUTTON_9           ,
    TI92_BUTTON_NEGATE      ,
    TI92_BUTTON_PLUS        ,
    TI92_BUTTON_MINUS       ,
    TI92_BUTTON_MULTIPLY    ,
    TI92_BUTTON_DIVIDE      ,
    TI92_BUTTON_PERIOD      ,
    TI92_BUTTON_LN          ,
    TI92_BUTTON_SIN         ,
    TI92_BUTTON_COS         ,
    TI92_BUTTON_TAN         ,
    TI92_BUTTON_POWER       ,
    TI92_BUTTON_EXP         ,
    TI92_BUTTON_QUIT        ,
    TI92_BUTTON_MOVE_CURSOR ,
    TI92_BUTTON_SIN_INV     ,
    TI92_BUTTON_COS_INV     ,
    TI92_BUTTON_TAN_INV     ,
    TI92_BUTTON_PI          ,
    TI92_BUTTON_LEFT_PARENT ,
    TI92_BUTTON_RIGHT_PARENT,
    TI92_BUTTON_COMMA       ,
    TI92_BUTTON_SPACE       ,
    TI92_BUTTON_BRACKET_LEFT,  /* [ */
    TI92_BUTTON_BRACKET_RIGHT, /* ] */
    TI92_BUTTON_BRACE_LEFT,    /* { */
    TI92_BUTTON_BRACE_RIGHT,   /* } */
    TI92_BUTTON_INTEGRAL   ,
    TI92_BUTTON_DERIVATE   ,
    TI92_BUTTON_INV_X      ,
    TI92_BUTTON_SQUARE_ROOT,
    TI92_BUTTON_SUM        ,
    TI92_BUTTON_MATH       ,
    TI92_BUTTON_MEM        ,
    TI92_BUTTON_VAR_LINK   ,
    TI92_BUTTON_EE         ,
    TI92_BUTTON_CATALOG    ,
    TI92_BUTTON_CUSTOM     ,
    TI92_BUTTON_CHAR       ,
    TI92_BUTTON_LT         ,
    TI92_BUTTON_GT         ,
    TI92_BUTTON_ANS        ,
    TI92_BUTTON_ENTRY      ,
    TI92_BUTTON_TRIANGLE   ,
    TI92_BUTTON_INFO       ,
    TI92_BUTTON_UNDERSCORE ,
    TI92_BUTTON_BETA       ,
    TI92_BUTTON_ANGLE      ,
    TI92_BUTTON_INFINITE   ,
    TI92_BUTTON_PIPE       ,
    TI92_BUTTON_DBL_QUOTE  ,
    TI92_BUTTON_CAPS       ,
    TI92_BUTTON_QUOTE      ,
    TI92_BUTTON_SEMICOLON  ,
    TI92_BUTTON_COLON      ,
    TI92_BUTTON_OFF        ,
    TI92_BUTTON_RCL        ,
    TI92_BUTTON_BACKSLASH  ,
    TI92_BUTTON_INS        ,
    TI92_BUTTON_HOME       ,  /* Diamon keys */
    TI92_BUTTON_Y_EQ       ,
    TI92_BUTTON_WINDOW     ,
    TI92_BUTTON_GRAPH      ,
    TI92_BUTTON_TBL_SET    ,
    TI92_BUTTON_TABLE      ,
    TI92_BUTTON_UNITS      ,
    TI92_BUTTON_KEY        ,
    TI92_BUTTON_DEL        ,
    TI92_BUTTON_ENVIRON    , /* End diamon keys */
    TI92_BUTTON_SHIFT       ,
    TI92_BUTTON_ON          ,
    TI92_BUTTON_DIAMOND     ,
    TI92_BUTTON_2ND         ,
    TI92_BUTTON_MODE        ,
    TI92_BUTTON_CLEAR       ,
    TI92_BUTTON_THETA       ,
    TI92_BUTTON_STORE       ,
    TI92_BUTTON_EQUAL       ,
    TI92_BUTTON_BACKSPACE   ,
    TI92_BUTTON_ENTER1      ,
    TI92_BUTTON_ENTER2      ,
    TI92_BUTTON_ESCAPE      ,
    TI92_BUTTON_APPS        ,
    TI92_BUTTON_LEFT        ,
    TI92_BUTTON_DOWN        ,
    TI92_BUTTON_UP          ,
    TI92_BUTTON_RIGHT       ,
    TI92_BUTTON_F1          ,
    TI92_BUTTON_F2          ,
    TI92_BUTTON_F3          ,
    TI92_BUTTON_F4          ,
    TI92_BUTTON_F5          ,
    TI92_BUTTON_F6          ,
    TI92_BUTTON_F7          ,
    TI92_BUTTON_F8          ,
    TI92_BUTTON_HAND        ,

    TI92_LAST_BUTTON
  };

# define TI92_META_NONE     -1
# define TI92_META_2ND      TIKEY_2ND
# define TI92_META_DIAMOND  TIKEY_DIAMOND
# define TI92_META_SHIFT    TIKEY_SHIFT

  typedef struct ti92_button_t {
    char                *name;
    int                  code;
    int                  meta;
  } ti92_button_t;

  extern int psp_kbd_mapping[ KBD_ALL_BUTTONS ];
  extern int psp_kbd_mapping_L[ KBD_ALL_BUTTONS ];
  extern int psp_kbd_mapping_R[ KBD_ALL_BUTTONS ];
  extern int psp_kbd_presses[ KBD_ALL_BUTTONS ];
  extern int kbd_ltrigger_mapping_active;
  extern int kbd_rtrigger_mapping_active;

  extern ti92_button_t ti92_buttons_info[ TI92_LAST_BUTTON ];

  extern int  psp_update_keys(void);
  extern int  psp_is_key_pressed(int key);
  extern void kbd_wait_start(void);
  extern void psp_init_keyboard(void);
  extern void psp_kbd_wait_no_button(void);

# endif
