#ifndef INCLUDED_KEYBOARDS_DANZEFF_H
#define INCLUDED_KEYBOARDS_DANZEFF_H

//danzeff is BSD licensed, if you do make a new renderer then please share it back and I can add it
//to the original distribution.

//Set which renderer target to build for (currently only SDL is available)
#define DANZEFF_SDL

#define  DANZEFF_LEFT           -5
#define  DANZEFF_RIGHT          -4
#define  DANZEFF_UP             -3
#define  DANZEFF_DOWN           -2

# define DANZEFF_SELECT         1
# define DANZEFF_START          2

# define DANZEFF_NEGATE       128
# define DANZEFF_LN           129
# define DANZEFF_SIN          130
# define DANZEFF_COS          131
# define DANZEFF_TAN          132
//# define DANZEFF_POWER        133
# define DANZEFF_EXP          134
# define DANZEFF_QUIT         135
# define DANZEFF_MOVE_CURSOR  136
# define DANZEFF_SIN_INV      137
# define DANZEFF_COS_INV      138
# define DANZEFF_TAN_INV      139
# define DANZEFF_PI           140
# define DANZEFF_INTEGRAL     141
# define DANZEFF_DERIVATE     142
# define DANZEFF_INV_X        143
# define DANZEFF_SQUARE_ROOT  144
# define DANZEFF_SUM          145
# define DANZEFF_MATH         146
# define DANZEFF_MEM          147
# define DANZEFF_VAR_LINK     148
# define DANZEFF_EE           149
# define DANZEFF_CATALOG      150
# define DANZEFF_CUSTOM       151
# define DANZEFF_CHAR         152
# define DANZEFF_ANS          153
# define DANZEFF_ENTRY        154
# define DANZEFF_TRIANGLE     155
# define DANZEFF_INFO         156
# define DANZEFF_BETA         157
# define DANZEFF_ANGLE        158
# define DANZEFF_INFINITE     159
# define DANZEFF_CAPS         160
# define DANZEFF_OFF          161
# define DANZEFF_RCL          162
# define DANZEFF_INS          163
# define DANZEFF_HOME         164
# define DANZEFF_Y_EQ         165
# define DANZEFF_WINDOW       166
# define DANZEFF_GRAPH        167
# define DANZEFF_TBL_SET      168
# define DANZEFF_TABLE        169
# define DANZEFF_UNITS        170
# define DANZEFF_KEY          171
# define DANZEFF_DEL          172
# define DANZEFF_ENVIRON      173
# define DANZEFF_SHIFT        174
# define DANZEFF_ON           175
# define DANZEFF_DIAMOND      176
# define DANZEFF_2ND          177
# define DANZEFF_MODE         178
# define DANZEFF_CLEAR        179
# define DANZEFF_THETA        180
# define DANZEFF_STORE        181
# define DANZEFF_BACKSPACE    182
# define DANZEFF_ENTER1       183
# define DANZEFF_ENTER2       184
# define DANZEFF_ESCAPE       185
# define DANZEFF_APPS         186
# define DANZEFF_F1           191
# define DANZEFF_F2           192
# define DANZEFF_F3           193
# define DANZEFF_F4           194
# define DANZEFF_F5           195
# define DANZEFF_F6           196
# define DANZEFF_F7           197
# define DANZEFF_F8           198
# define DANZEFF_HAND         199

#ifdef __cplusplus
extern "C" {
#endif

 extern int   psp_kbd_skin;
 extern int   psp_kbd_last_skin;
 extern char *psp_kbd_skin_dir[];

//Initialization and de-init of the keyboard, provided as the keyboard uses alot of images, so if you aren't going to use it for a while, I'd recommend unloading it.
extern int danzeff_load(void);
extern void danzeff_free(void);

//returns true if the keyboard is initialized
extern int danzeff_isinitialized(void);

/** Attempts to read a character from the controller
* If no character is pressed then we return 0
* Other special values: 1 = move left, 2 = move right, 3 = select, 4 = start
* Every other value should be a standard ascii value.
* An unsigned int is returned so in the future we can support unicode input
*/
extern unsigned int danzeff_readInput(SceCtrlData pspctrl);

//Move the area the keyboard is rendered at to here
extern void danzeff_moveTo(const int newX, const int newY);

//Returns true if the keyboard that would be rendered now is different to the last time
//that the keyboard was drawn, this is altered by readInput/render.
extern   int danzeff_dirty();

//draw the keyboard to the screen
extern void danzeff_render();

///Functions only for particular renderers:

#include <SDL/SDL.h>
//set the screen surface for rendering on.
extern void danzeff_set_screen(SDL_Surface* screen);


#ifdef __cplusplus
}
#endif

#endif //INCLUDED_KEYBOARDS_DANZEFF_H
