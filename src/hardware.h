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


extern void update_contrast(void);
extern void update_timer(UBYTE arg);
extern void update_bitmap(void);
extern UBYTE read_keyboard_mask(void);
extern UBYTE read_onkey(void);
extern void do_period(void);


extern void link_putbyte(UBYTE arg);
extern UBYTE link_getbyte(void);
extern int link_byteavail(void);
extern void init_hardware(char*);
extern void exit_hardware(void);

#define CYCLES_PER_INSTR 10
#define CYCLES_PER_TICK 6400

extern int cycleInstr;

extern int cycle_count;

extern int onKey;
extern int timerVal;

extern int internalLink;

extern char linkReadName[64];
extern char linkWriteName[64];

static inline void do_cycles(void) {
  if(cycle_count++ >= cycleInstr) {
    do_period();
    cycle_count = 0;
  }
}
