#include <stdlib.h>
#include <string.h>
#include <pspctrl.h>
#include "global.h"
#include "psp_danzeff.h"
#include "SDL_image.h"

#define false 0
#define true 1

# define PSP_KBD_MAX_SKIN    128

  int    psp_kbd_last_skin  = 0;
  int    psp_kbd_skin       = -1;
  char  *psp_kbd_skin_dir[PSP_KBD_MAX_SKIN];
  int    psp_kbd_skin_first = 1;

static /*bool*/ int holding = false;     //user is holding a button
static /*bool*/ int dirty = true;        //keyboard needs redrawing
static /*bool*/ int shifted = false;     //user is holding shift
static int mode = 0;             //charset selected. (0 - letters or 1 - numbers)
static /*bool*/ int initialized = false; //keyboard is initialized

//Position on the 3-3 grid the user has selected (range 0-2)
static int selected_x = 1;
static int selected_y = 1;

//Variable describing where each of the images is
#define guiStringsSize 18 /* size of guistrings array */
#define PICS_BASEDIR "/graphics/"

static char *guiStrings[] = 
{
	"/keys1.png", "/keys1_t.png", "/keys1_s.png",
	"/keys4.png", "/keys4_t.png", "/keys4_s.png",
	"/keys3.png", "/keys3_t.png", "/keys3_s.png",
	"/keys2.png", "/keys2_t.png", "/keys2_s.png",
	"/keys5.png", "/keys5_t.png", "/keys5_s.png",
	"/keys1.png", "/keys1_t.png", "/keys1_s.png"
};

//amount of modes (non shifted), each of these should have a corresponding shifted mode.
#define MODE_COUNT 3
#define MAX_VKEYBOARD 6
//this is the layout of the keyboard
static unsigned char modeChar[MAX_VKEYBOARD][3][3][5] = 
{
	{
		{ 
      { '{'        , DANZEFF_F7, ','            , '7', '\0' } ,  
      { DANZEFF_COS, DANZEFF_F8, DANZEFF_COS_INV, '8', '\0' } ,  
      { '}'        , '0'       , '.'            , '9', '\0' } ,  
    },
		{ 
      { '-'           , DANZEFF_F4, '('              , '4', '\0' } ,  
      { DANZEFF_ENTER1, DANZEFF_F5, DANZEFF_BACKSPACE, '5', '\0' } ,  
      { '+'           , DANZEFF_F6, ')'              , '6', '\0' } ,  
    },
		{ 
      { '/'           , DANZEFF_F1, '['              , '1', '\0' } ,  
      { DANZEFF_SIN   , DANZEFF_F2, DANZEFF_SIN_INV  , '2', '\0' } ,  
      { '*'           , DANZEFF_F3, ']'              , '3', '\0' } ,  
    }
	},

	{
		{ 
      { DANZEFF_LN    , DANZEFF_NEGATE, DANZEFF_EXP    , DANZEFF_PI      , '\0' } ,  
      { DANZEFF_TAN   , '^'           , DANZEFF_TAN_INV, DANZEFF_EE      , '\0' } ,  
      { DANZEFF_INTEGRAL, '<'           , '>'            , DANZEFF_DERIVATE, '\0' } ,  
    },
		{ 
      { DANZEFF_INV_X , '('         , DANZEFF_SQUARE_ROOT, ')'         , '\0' } ,  
      { DANZEFF_ENTER1, '_'         , DANZEFF_BACKSPACE  , '|'         , '\0' } ,  
      { DANZEFF_SUM   , DANZEFF_INFO, ' '                , DANZEFF_BETA, '\0' } ,  
    },
		{ 
      { DANZEFF_ANGLE   , DANZEFF_INFINITE, DANZEFF_THETA      , DANZEFF_ENVIRON, '\0' } ,  
      { '='             , '+'             , DANZEFF_Y_EQ       , '-'            , '\0' } ,  
      { DANZEFF_ESCAPE  , '*'             , DANZEFF_TRIANGLE   , '/'            , '\0' } ,  
    }
	},
	{
		{ "badc",  "fehg","jilk" },
		{ "nmpo", { DANZEFF_ENTER1, '_', DANZEFF_BACKSPACE, '|', '\0' }, "rqts" },
		{ "vuxw",  "zy,\"",":; \\" }
	},

	{
		{ "BADC",  "FEHG","JILK" },
		{ "NMPO", { DANZEFF_ENTER1, '_', DANZEFF_BACKSPACE, '|', '\0' }, "RQTS" },
		{ "VUXW",  "ZY,\"",":; \\" }
	},
	{
		{ 
      { DANZEFF_ANS    , DANZEFF_2ND   , DANZEFF_CAPS   , DANZEFF_APPS    , '\0' } ,  
      { DANZEFF_CATALOG, DANZEFF_CHAR  , DANZEFF_CUSTOM , DANZEFF_CLEAR   , '\0' } ,  
      { DANZEFF_DIAMOND, DANZEFF_DEL   , DANZEFF_ESCAPE , DANZEFF_ENTRY   , '\0' } ,  
    },
		{ 
      { DANZEFF_HAND       , DANZEFF_GRAPH, DANZEFF_HOME, DANZEFF_INS, '\0' } ,  
      { DANZEFF_MATH       , DANZEFF_KEY  , DANZEFF_MODE, DANZEFF_MEM, '\0' } ,  
      { DANZEFF_MOVE_CURSOR, DANZEFF_OFF  , DANZEFF_QUIT, DANZEFF_ON , '\0' } ,  
    },
		{
      { DANZEFF_SHIFT   , DANZEFF_RCL    , DANZEFF_TABLE      , DANZEFF_STORE  , '\0' } ,  
      { DANZEFF_TBL_SET , DANZEFF_UNITS  , DANZEFF_VAR_LINK   , '.'            , '\0' } ,  
      { DANZEFF_WINDOW  , '('            , DANZEFF_ENTER2     , ')'            , '\0' } ,  
    }
	},
	{
		{ 
      { '{'        , DANZEFF_F7, ','            , '7', '\0' } ,  
      { DANZEFF_COS, DANZEFF_F8, DANZEFF_COS_INV, '8', '\0' } ,  
      { '}'        , '0'       , '.'            , '9', '\0' } ,  
    },
		{ 
      { '-'           , DANZEFF_F4, '('              , '4', '\0' } ,  
      { DANZEFF_ENTER1, DANZEFF_F5, DANZEFF_BACKSPACE, '5', '\0' } ,  
      { '+'           , DANZEFF_F6, ')'              , '6', '\0' } ,  
    },
		{ 
      { '/'           , DANZEFF_F1, '['              , '1', '\0' } ,  
      { DANZEFF_SIN   , DANZEFF_F2, DANZEFF_SIN_INV  , '2', '\0' } ,  
      { '*'           , DANZEFF_F3, ']'              , '3', '\0' } ,  
    }
	},
};

int 
danzeff_isinitialized()
{
	return initialized;
}

int 
danzeff_dirty()
{
	return dirty;
}

/** Attempts to read a character from the controller
* If no character is pressed then we return 0
* Other special values: 1 = move left, 2 = move right, 3 = select, 4 = start
* Every other value should be a standard ascii value.
* An unsigned int is returned so in the future we can support unicode input
*/
unsigned int 
danzeff_readInput(SceCtrlData pspctrl)
{
	//Work out where the analog stick is selecting
	int x = 1;
	int y = 1;
	if (pspctrl.Lx < 85)     x -= 1;
	else if (pspctrl.Lx > 170) x += 1;
	
	if (pspctrl.Ly < 85)     y -= 1;
	else if (pspctrl.Ly > 170) y += 1;
	
	if (selected_x != x || selected_y != y) //If they've moved, update dirty
	{
		dirty = true;
		selected_x = x;
		selected_y = y;
	}
	//if they are changing shift then that makes it dirty too
	if ((!shifted && (pspctrl.Buttons & PSP_CTRL_RTRIGGER)) || (shifted && !(pspctrl.Buttons & PSP_CTRL_RTRIGGER)))
		dirty = true;
	
	unsigned int pressed = 0; //character they have entered, 0 as that means 'nothing'
	shifted = (pspctrl.Buttons & PSP_CTRL_RTRIGGER)?true:false;
	
	if (!holding)
	{
		if (pspctrl.Buttons& (PSP_CTRL_CROSS|PSP_CTRL_CIRCLE|PSP_CTRL_TRIANGLE|PSP_CTRL_SQUARE)) //pressing a char select button
		{
			int innerChoice = 0;
			if      (pspctrl.Buttons & PSP_CTRL_TRIANGLE)
				innerChoice = 0;
			else if (pspctrl.Buttons & PSP_CTRL_SQUARE)
				innerChoice = 1;
			else if (pspctrl.Buttons & PSP_CTRL_CROSS)
				innerChoice = 2;
			else //if (pspctrl.Buttons & PSP_CTRL_CIRCLE)
				innerChoice = 3;
			
			//Now grab the value out of the array
			pressed = modeChar[(mode*2 + shifted) % MAX_VKEYBOARD][y][x][innerChoice];
		}
		else if (pspctrl.Buttons& PSP_CTRL_LTRIGGER) //toggle mode
		{
			dirty = true;
			mode++;
			mode %= MODE_COUNT;
		}
		else if (pspctrl.Buttons& PSP_CTRL_LEFT)
		{
			pressed = DANZEFF_LEFT;
		}
		else if (pspctrl.Buttons& PSP_CTRL_RIGHT)
		{
			pressed = DANZEFF_RIGHT;
		}
		else if (pspctrl.Buttons& PSP_CTRL_DOWN)
		{
			pressed = DANZEFF_DOWN;
		}
		else if (pspctrl.Buttons& PSP_CTRL_UP)
		{
			pressed = DANZEFF_UP;
		}
		else if (pspctrl.Buttons& PSP_CTRL_SELECT)
		{
			pressed = DANZEFF_SELECT; //SELECT
		}
		else if (pspctrl.Buttons& PSP_CTRL_START)
		{
			pressed = DANZEFF_START; //START
		}
	}

	holding = pspctrl.Buttons & ~PSP_CTRL_RTRIGGER; //RTRIGGER doesn't set holding
	
	return pressed;
}

///-----------------------------------------------------------
///These are specific to the implementation, they should have the same behaviour across implementations.
///-----------------------------------------------------------


///This is the original SDL implementation
#ifdef DANZEFF_SDL

static SDL_Surface* keyBits[guiStringsSize];
static int moved_x = 0, moved_y = 0; // location that we are moved to

///variable needed for rendering in SDL, the screen surface to draw to, and a function to set it!
static SDL_Surface* danzeff_screen;
static SDL_Rect danzeff_screen_rect;

void 
danzeff_set_screen(SDL_Surface* screen)
{
	danzeff_screen = screen;
	danzeff_screen_rect.x = 0;
	danzeff_screen_rect.y = 0;
	danzeff_screen_rect.h = screen->h;
	danzeff_screen_rect.w = screen->w;

  moved_x = danzeff_screen->w - 150;
  moved_y = danzeff_screen->h - 150;
}


///Internal function to draw a surface internally offset
//Render the given surface at the current screen position offset by screenX, screenY
//the surface will be internally offset by offsetX,offsetY. And the size of it to be drawn will be intWidth,intHeight
void 
surface_draw_offset(SDL_Surface* pixels, int screenX, int screenY, int offsetX, int offsetY, int intWidth, int intHeight)
{
	//move the draw position
	danzeff_screen_rect.x = moved_x + screenX;
	danzeff_screen_rect.y = moved_y + screenY;

	//Set up the rectangle
	SDL_Rect pixels_rect;
	pixels_rect.x = offsetX;
	pixels_rect.y = offsetY;
	pixels_rect.w = intWidth;
	pixels_rect.h = intHeight;
	
	SDL_BlitSurface(pixels, &pixels_rect, danzeff_screen, &danzeff_screen_rect);
}

///Draw a surface at the current moved_x, moved_y
void 
surface_draw(SDL_Surface* pixels)
{
	surface_draw_offset(pixels, 0, 0, 0, 0, pixels->w, pixels->h);
}

void
danzeff_init_skin()
{
  char skin_path[128];
  strcpy(skin_path, TI92.ti92_home_dir);
  strcat(skin_path, PICS_BASEDIR);

  psp_kbd_last_skin = psp_fmgr_get_dir_list(skin_path, PSP_KBD_MAX_SKIN, psp_kbd_skin_dir) - 1;
 
  /* Should not happen ! */
  if (psp_kbd_last_skin < 0) {
    fprintf(stdout, "no keyboard skin in %s directory !\n", skin_path);
    exit(1);
  }

  if ((psp_kbd_skin == -1) || (psp_kbd_skin > psp_kbd_last_skin)) {
    psp_kbd_skin_first = 0;
    for (psp_kbd_skin = 0; psp_kbd_skin <= psp_kbd_last_skin; psp_kbd_skin++) {
      if (!strcasecmp(psp_kbd_skin_dir[psp_kbd_skin], "default/")) break;
    }
    if (psp_kbd_skin > psp_kbd_last_skin) psp_kbd_skin = 0;
  }
}

/* load all the guibits that make up the OSK */
int 
danzeff_load()
{
  char tmp_filename[128];

	if (initialized) return 1;

  if (psp_kbd_skin_first) {
    danzeff_init_skin();
  }

	int a;
	for (a = 0; a < guiStringsSize; a++)
	{
    strcpy(tmp_filename, TI92.ti92_home_dir);
    strcat(tmp_filename, PICS_BASEDIR);
    strcat(tmp_filename, psp_kbd_skin_dir[psp_kbd_skin] );
    strcat(tmp_filename, guiStrings[a] );
		keyBits[a] = IMG_Load(tmp_filename);
		if (keyBits[a] == NULL)
		{
			//ERROR! out of memory.
			//free all previously created surfaces and set initialized to false
			int b;
			for (b = 0; b < a; b++)
			{
				SDL_FreeSurface(keyBits[b]);
				keyBits[b] = NULL;
			}
			initialized = false;
      fprintf(stdout, "can't load image %s\n", tmp_filename); 
			exit(1);
		}
	}
	initialized = true;
  return 1;
}

/* remove all the guibits from memory */
void 
danzeff_free()
{
	if (!initialized) return;
	
	int a;
	for (a = 0; a < guiStringsSize; a++)
	{
		SDL_FreeSurface(keyBits[a]);
		keyBits[a] = NULL;
	}
	initialized = false;
}

/* draw the keyboard at the current position */
void 
danzeff_render()
{
	dirty = false;
	
	///Draw the background for the selected keyboard either transparent or opaque
	///this is the whole background image, not including the special highlighted area
	//if center is selected then draw the whole thing opaque
  int index = (mode*2 + shifted) % MAX_VKEYBOARD;
	if (selected_x == 1 && selected_y == 1)
		surface_draw(keyBits[index*3]);
	else
		surface_draw(keyBits[index*3 + 1]);
	
	///Draw the current Highlighted Selector (orange bit)
	surface_draw_offset(keyBits[index*3 + 2], 
	//Offset from the current draw position to render at
	selected_x*43, selected_y*43, 
	//internal offset of the image
	selected_x*64,selected_y*64,
	//size to render (always the same)
	64, 64);
}

/* move the position the keyboard is currently drawn at */
void 
danzeff_moveTo(const int newX, const int newY)
{
	moved_x = danzeff_screen->w - 150 + newX;
	moved_y = danzeff_screen->h - 150 + newY;
}

void
danzeff_change_skin()
{
  danzeff_free();
  danzeff_load();
}

#endif //DANZEFF_SDL
