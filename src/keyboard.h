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



enum { TIKEY_DOWN, TIKEY_RIGHT, TIKEY_UP, TIKEY_LEFT, TIKEY_HAND,
TIKEY_SHIFT, TIKEY_DIAMOND, TIKEY_2ND, TIKEY_3, TIKEY_2, TIKEY_1,
TIKEY_F8, TIKEY_W, TIKEY_S, TIKEY_Z, TIKEY_6, TIKEY_5, TIKEY_4,
TIKEY_F3, TIKEY_E, TIKEY_D, TIKEY_X, TIKEY_9, TIKEY_8, TIKEY_7,
TIKEY_F7, TIKEY_R, TIKEY_F, TIKEY_C, TIKEY_STORE, TIKEY_COMMA,
TIKEY_PARIGHT, TIKEY_PALEFT, TIKEY_F2, TIKEY_T, TIKEY_G, TIKEY_V,
TIKEY_SPACE, TIKEY_TAN, TIKEY_COS, TIKEY_SIN, TIKEY_F6, TIKEY_Y,
TIKEY_H, TIKEY_B, TIKEY_DIVIDE, TIKEY_P, TIKEY_ENTER2, TIKEY_LN,
TIKEY_F1, TIKEY_U, TIKEY_J, TIKEY_N, TIKEY_POWER, TIKEY_MULTIPLY,
TIKEY_APPS, TIKEY_CLEAR, TIKEY_F5, TIKEY_I, TIKEY_K, TIKEY_M,
TIKEY_EQUALS, TIKEY_NU, TIKEY_ESCAPE, TIKEY_MODE, TIKEY_PLUS, TIKEY_O,
TIKEY_L, TIKEY_THETA, TIKEY_BACKSPACE, TIKEY_NEGATE, TIKEY_PERIOD,
TIKEY_0, TIKEY_F4, TIKEY_Q, TIKEY_A, TIKEY_ENTER1, TIKEY_MINUS,
TIKEY_ON, OPT_DEBUGGER, OPT_LOADFILE, OPT_QUIT, OPT_QUITNOSAVE };

int ti_update_keys(void);
UBYTE get_rowmask(UBYTE r);
void init_keyboard(void);

extern int dontSaveState;
